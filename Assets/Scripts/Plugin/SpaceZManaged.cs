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

        private static NativeList<NativeColliderSpaceObjectSynMsg> _dirtyColliderSpaceObjList;
        private static NativeList<NativeTransformSynMsg> _dirtySpaceObjectTransformList;
        private static NativeList<NativeTransformSynMsg> _dirtyColliderTransformList; 

        private static NativeList<NativeBoxColliderSynMsg> _dirtyBoxColliderList;
        private static NativeList<NativeSphereColliderSynMsg> _dirtySphereColliderList;
        private static NativeList<NativeCapsuleColliderSynMsg> _dirtyCapsuleColliderList;


        private static List<ISpaceZUpdate> _tempUpdateList = new();

        [RunTimeStart(-90)]
        private static void InitializeManaged()
        {
            _dirtyColliderSpaceObjList = new(Allocator.Persistent);
            _dirtyColliderTransformList = new(Allocator.Persistent);
            _dirtySpaceObjectTransformList = new(Allocator.Persistent);

            _dirtyBoxColliderList = new(Allocator.Persistent);
            _dirtySphereColliderList = new(Allocator.Persistent);
            _dirtyCapsuleColliderList = new(Allocator.Persistent);

            SpaceZUpdate.AddListener(ComponentUpdate);
            LateSpaceZUpdate.AddListener(OnLateSpaceZUpdate);
        }
        [RunTimeEnd(90)]
        private static void UninitializeManaged()
        {
            SpaceZUpdate.RemoveListener(ComponentUpdate);
            LateSpaceZUpdate.RemoveListener(OnLateSpaceZUpdate);

            TryDisposeNativeList(_dirtyColliderSpaceObjList);
            TryDisposeNativeList(_dirtyColliderTransformList);
            TryDisposeNativeList(_dirtySpaceObjectTransformList);

            TryDisposeNativeList(_dirtyBoxColliderList);
            TryDisposeNativeList(_dirtySphereColliderList);
            TryDisposeNativeList(_dirtyCapsuleColliderList);
        }
        private static void OnLateSpaceZUpdate()
        {
            SynDirtyData();
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
                GetReadOnlyPtrAndClear(_dirtyColliderSpaceObjList, UpdateColliderSpaceObject);
                GetReadOnlyPtrAndClear(_dirtySpaceObjectTransformList, UpdateSpaceObjectTransform);
                GetReadOnlyPtrAndClear(_dirtyColliderTransformList, UpdateColliderTransform);

                GetReadOnlyPtrAndClear(_dirtyBoxColliderList, UpdateBoxCollider);
                GetReadOnlyPtrAndClear(_dirtySphereColliderList, UpdateSphereCollider);
                GetReadOnlyPtrAndClear(_dirtyCapsuleColliderList, UpdateCapsuleCollider);
            }
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
            _spaceObjDic.Remove(obj.Handle);
            return DestroySpaceObject(obj.Handle);
        }
        public static bool Destroy(this SpaceZCollider collider)
        {
            if(collider == null)
                return false;
            if(!TryGetCollider(collider.Handle))
                return false;
            return DestroyCollider(collider.Handle);
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
        public static void UpdateSpaceObject(this SpaceZCollider collider, NativeColliderSpaceObjectSynMsg spaceObj)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            _dirtyColliderSpaceObjList.Add(spaceObj);
        }
        public static void UpdateBoxCollider(this SpaceZCollider collider, NativeBoxColliderSynMsg msg)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            _dirtyBoxColliderList.Add(msg);
        }
        public static void UpdateSphereCollider(this SpaceZCollider collider, NativeSphereColliderSynMsg msg)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            _dirtySphereColliderList.Add(msg);
        }
        public static void UpdateCapsuleCollider(this SpaceZCollider collider, NativeCapsuleColliderSynMsg msg)
        {
            if(!TryGetCollider(collider.Handle))
                return;
            _dirtyCapsuleColliderList.Add(msg);
        }
#endregion
        private static bool TryGetCollider(NativeHandle handle)
        {
            if(!_colliderDic.ContainsKey(handle))
            {
                Debug.LogWarning($"【SpaceZ Plugin】不存在Collider{handle}");
                return false;
            }
            return true;
        }
    }
}