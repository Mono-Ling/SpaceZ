using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Collections;
using SpaceZ.Framework.RunTime;
using Unity.Collections.LowLevel.Unsafe;
using static SpaceZ.FunctionPtr;

namespace SpaceZ
{
    public static partial class SpaceZPlugin
    {
        private static Dictionary<NativeHandle, SpaceObject> _spaceObjDic = new();
        private static Dictionary<NativeHandle, SpaceZCollider> _colliderDic = new();

        private static NativeList<NativeTransformSynMsg> _dirtySpaceObjectTransformList;
        private static NativeList<NativeTransformSynMsg> _dirtyColliderTransformList; 

        private static NativeList<NativeBoxColliderSynMsg> _dirtyBoxColliderList;
        private static NativeList<NativeSphereColliderSynMsg> _dirtySphereColliderList;
        private static NativeList<NativeCapsuleColliderSynMsg> _dirtyCapsuleColliderList;

        private static NativeList<CollisionPair> _collisionPairBuffer;
        private static CollisionCallbackBuffer _collisionCallback = new();

        private static List<ISpaceZUpdate> _tempUpdateList = new();

#region 托管引擎更新
        [RunTimeStart(-90)]
        private static void InitializeManaged()
        {
            _dirtyColliderTransformList = new(Allocator.Persistent);
            _dirtySpaceObjectTransformList = new(Allocator.Persistent);

            _dirtyBoxColliderList = new(Allocator.Persistent);
            _dirtySphereColliderList = new(Allocator.Persistent);
            _dirtyCapsuleColliderList = new(Allocator.Persistent);

            _collisionPairBuffer = new(Allocator.Persistent);

            SpaceZUpdate.AddListener(ComponentUpdate);
            LateSpaceZUpdate.AddListener(OnLateSpaceZUpdate);
        }
        [RunTimeEnd(90)]
        private static void UninitializeManaged()
        {
            SpaceZUpdate.RemoveListener(ComponentUpdate);
            LateSpaceZUpdate.RemoveListener(OnLateSpaceZUpdate);

            TryDisposeNativeList(_dirtyColliderTransformList);
            TryDisposeNativeList(_dirtySpaceObjectTransformList);

            TryDisposeNativeList(_dirtyBoxColliderList);
            TryDisposeNativeList(_dirtySphereColliderList);
            TryDisposeNativeList(_dirtyCapsuleColliderList);

            TryDisposeNativeList(_collisionPairBuffer);
        }
        private static void OnLateSpaceZUpdate()
        {
            SynDirtyData();
            UpdateCollisionPairs();
        }
        private static void ComponentUpdate()
        {
            _tempUpdateList.Clear();
            foreach(var item in _spaceObjDic.Values)
                if(item is ISpaceZUpdate update)
                    _tempUpdateList.Add(update);
            foreach(var item in _colliderDic.Values)
                if(item is ISpaceZUpdate update)
                    _tempUpdateList.Add(update);
            
            foreach(var item in _tempUpdateList)
                item.SpaceZUpdate();
        }
        private static void SynDirtyData()
        {
            unsafe
            {
                GetReadOnlyPtrAndClear(_dirtySpaceObjectTransformList, UpdateSpaceObjectTransform);
                GetReadOnlyPtrAndClear(_dirtyColliderTransformList, UpdateColliderTransform);

                GetReadOnlyPtrAndClear(_dirtyBoxColliderList, UpdateBoxCollider);
                GetReadOnlyPtrAndClear(_dirtySphereColliderList, UpdateSphereCollider);
                GetReadOnlyPtrAndClear(_dirtyCapsuleColliderList, UpdateCapsuleCollider);
            }
        }
        private static void UpdateCollisionPairs()
        {
            int count = GetCollisionPairsCount();
            if(count < 0)
            {
                Debug.LogError("【SpaceZ Plugin】碰撞对数量获取异常");
                return;
            }
            _collisionPairBuffer.Clear();
            _collisionPairBuffer.Length = count;
            if(count > 0)
            {
                unsafe
                {
                    var ptr = _collisionPairBuffer.GetUnsafePtr();
                    GetCollisionPairs(ptr, ref count);
                }
            }
            _collisionCallback.ResetVisible();
            for(int i = 0; i < count; i++)
            {
                var collisionInfo = _collisionPairBuffer[i].collisionInfo;
                var callbackInfos = CollisionCallbackInfo.From(_collisionPairBuffer[i]);

                if(callbackInfos.Item1.collisionCollider == NativeHandle.NULL
                || callbackInfos.Item2.collisionCollider == NativeHandle.NULL)
                    continue;

                SpaceObject obj = null;
                if(_spaceObjDic.TryGetValue(callbackInfos.Item1.collisionObj, out obj))
                    _collisionCallback.SetVisible(callbackInfos.Item1, obj, collisionInfo);
                
                collisionInfo.normal = -collisionInfo.normal;

                if(_spaceObjDic.TryGetValue(callbackInfos.Item2.collisionObj, out obj))
                    _collisionCallback.SetVisible(callbackInfos.Item2, obj, collisionInfo);
            }
            _collisionCallback.ClearNotVisible();

            foreach(var item in _collisionCallback.CollisionEnterList)
                if(_colliderDic.TryGetValue(item.collisionCollider, out var collider))
                    if(_collisionCallback.SpaceObjectCallbackDic.TryGetValue(item.spaceObj, out var value))
                        foreach(var callback in value.callbacks)
                            callback?.OnSpaceZCollisionEnter(collider, item.collisionInfo);

            foreach(var item in _collisionCallback.CollisionStayList)
                if(_colliderDic.TryGetValue(item.collisionCollider, out var collider))
                    if(_collisionCallback.SpaceObjectCallbackDic.TryGetValue(item.spaceObj, out var value))
                        foreach(var callback in value.callbacks)
                            callback?.OnSpaceZCollisionStay(collider, item.collisionInfo);

            foreach(var item in _collisionCallback.CollisionExitList)
            {
                SpaceZCollider collider = null;
                if(!_colliderDic.TryGetValue(item.collisionCollider, out collider))
                    collider = null;
                if(_collisionCallback.SpaceObjectCallbackDic.TryGetValue(item.spaceObj, out var value))
                    foreach(var callback in value.callbacks)
                        callback?.OnSpaceZCollisionExit(collider, item.collisionInfo);
            }
            _collisionCallback.ClearSpaceObjectCallbackDic();
        }

        private static void GetReadOnlyPtrAndClear<T>(NativeList<T> list, UnsafeAction<T,int> action) where T : unmanaged
        {
            if(list.IsEmpty)
                return;
            unsafe
            {
                T* ptr = list.GetUnsafeReadOnlyPtr();
                action.Invoke(ptr, list.Length);
            }
            list.Clear();
        }
        private static bool TryDisposeNativeList<T>(NativeList<T> list) where T : unmanaged
        {
            if(!list.IsCreated)
                return false;
            list.Dispose();
            return true;
        }
#endregion

#region 组件生命周期
        public static NativeHandle Create(this SpaceObject obj)
        {
            if(obj == null)
                return NativeHandle.NULL;
            var handle = CreateSpaceObject();
            if(handle == NativeHandle.NULL)
            {
                Debug.LogError("【SpaceZ Plugin】SpaceObject创建失败返回NULL");
                return handle;
            }
            if(_spaceObjDic.ContainsKey(handle))
            {
                Debug.LogError($"【SpaceZ Plugin】SpaceObject{handle}冲突");
                return handle;
            }
            _spaceObjDic.Add(handle, obj);
            return handle;
        }
        public static NativeHandle Create(this SpaceZCollider collider, ColliderType type, NativeHandle spaceObj)
        {
            if(collider == null)
                return NativeHandle.NULL;    
            var handle = CreateCollider(type, spaceObj);
            if(handle == NativeHandle.NULL)
            {
                Debug.LogError("【SpaceZ Plugin】Collider创建失败返回NULL");
                return handle;
            }
            if(_colliderDic.ContainsKey(handle))
            {
                Debug.LogError($"【SpaceZ Plugin】Collider{handle}冲突");
                return handle;
            }

            if(_spaceObjDic.TryGetValue(spaceObj, out var obj))
                obj.colliderSet.Add(handle);
            _colliderDic.Add(handle, collider);
            return handle;
        }
        public static bool Destroy(this SpaceObject obj)
        {
            if(obj == null)
                return false;
            if(!_spaceObjDic.ContainsKey(obj.Handle))
            {
                Debug.LogWarning($"【SpaceZ Plugin】不存在SpaceObject{obj.Handle}");
                return false;
            }

            if(!DestroySpaceObject(obj.Handle))
                return false;

            _spaceObjDic.Remove(obj.Handle);
            foreach(var collider in obj.colliderSet)
                if(DestroyManagedCollider(collider))
                    DestroyCollider(collider);
            return true;
        }
        public static bool Destroy(this SpaceZCollider collider)
        {
            if(collider == null)
                return false;
            if(!TryGetCollider(collider.Handle))
                return false;

            if(!DestroyCollider(collider.Handle))
                return false;
            if(_spaceObjDic.TryGetValue(collider.SpaceObject, out var obj))
                obj?.colliderSet.Remove(collider.Handle);

            DestroyManagedCollider(collider.Handle);
            return true;
        }
        private static bool DestroyManagedCollider(NativeHandle collider)
        {
            if(!TryGetCollider(collider))
                return false;
            _colliderDic[collider].SetHandle(NativeHandle.NULL);
            _colliderDic.Remove(collider);
            return true;
        }
#endregion

#region 组件更新
        public static void UpdateTransform(this SpaceObject obj, NativeTransformSynMsg msg)
        {
            if(!_spaceObjDic.ContainsKey(obj.Handle))
            {
                Debug.LogWarning($"【SpaceZ Plugin】不存在SpaceObject{obj.Handle}");
                return;
            }
            _dirtySpaceObjectTransformList.Add(msg);
        }
        public static void UpdateTransform(this SpaceZCollider collider, NativeTransformSynMsg msg)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            _dirtyColliderTransformList.Add(msg);
        }
        /// <summary>
        /// 更新碰撞体所属空间物体
        /// 需要先于修改_spaceObj成员前上报
        /// </summary>
        /// <param name="collider"></param>
        /// <param name="spaceObj"></param>
        public static void UpdateSpaceObject(this SpaceZCollider collider, NativeHandle spaceObj)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            if(spaceObj == collider.SpaceObject)
                return;

            if(_spaceObjDic.TryGetValue(collider.SpaceObject, out var oldObj))
                oldObj?.colliderSet.Remove(collider.Handle);
            if(_spaceObjDic.TryGetValue(spaceObj, out var newObj))
                newObj?.colliderSet.Add(collider.Handle);
            collider.SetSpaceObject(spaceObj);
                
            UpdateColliderSpaceObject(new(){collider = collider.Handle, spaceObj = spaceObj});
        }
        public static void UpdateBoxCollider(this SpaceZCollider collider, Vector3 extents)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            _dirtyBoxColliderList.Add(new(){handle = collider.Handle, extents = extents});
        }
        public static void UpdateSphereCollider(this SpaceZCollider collider, float radius)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            _dirtySphereColliderList.Add(new(){handle = collider.Handle, radius = radius});
        }
        public static void UpdateCapsuleCollider(this SpaceZCollider collider, float height, float radius)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            _dirtyCapsuleColliderList.Add(new(){handle = collider.Handle, height = height, radius = radius});
        }
#endregion
        private static bool TryGetCollider(NativeHandle handle)
        {
            if(handle == NativeHandle.NULL)
                return false;
            if(!_colliderDic.ContainsKey(handle))
            {
                Debug.LogWarning($"【SpaceZ Plugin】不存在Collider{handle}");
                return false;
            }
            return true;
        }
    }
}