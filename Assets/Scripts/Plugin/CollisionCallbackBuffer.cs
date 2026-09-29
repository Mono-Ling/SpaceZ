using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ
{
    public struct CollisionCallbackInfo : IEquatable<CollisionCallbackInfo>
    {
        public NativeHandle collisionObj;
        public NativeHandle collisionCollider;

        public CollisionCallbackInfo(NativeHandle obj, NativeHandle collider)
        {
            this.collisionObj = obj;
            this.collisionCollider = collider;
        }
        public override bool Equals(object obj)
        {
            if(obj is not CollisionCallbackInfo info)
                return false;
            return collisionObj == info.collisionObj
                && collisionCollider == info.collisionCollider;
        }
        public bool Equals(CollisionCallbackInfo other)
        {
            return collisionObj == other.collisionObj
                && collisionCollider == other.collisionCollider;
        }
        public override int GetHashCode()
        => (collisionObj, collisionCollider).GetHashCode();
        public static (CollisionCallbackInfo, CollisionCallbackInfo) From(CollisionPair pair)
        {
            CollisionCallbackInfo first = new(pair.first.spaceObject, pair.second.collider);
            CollisionCallbackInfo second = new(pair.second.spaceObject, pair.first.collider);
            return (first, second);
        }
    }
    public struct CollisionCallbackItem
    {
        public bool visible;
        public NativeHandle spaceObj;
        public NativeHandle collisionCollider;
        public CollisionInfo collisionInfo;
        public CollisionCallbackItem(CollisionCallbackInfo callbackInfo, CollisionInfo info)
        {
            this.visible = true;
            this.spaceObj = callbackInfo.collisionObj;
            this.collisionCollider = callbackInfo.collisionCollider;
            collisionInfo = info;
        }
    }
    public class CollisionCallbackBuffer
    {
        private Dictionary<CollisionCallbackInfo, CollisionCallbackItem> _callbackDic = new();
        private List<CollisionCallbackItem> _collisionEnterList = new();
        private List<CollisionCallbackItem> _collisionStayList = new();
        private List<CollisionCallbackItem> _collisionExitList = new();

        private Dictionary<NativeHandle,(int count,IOnCollision[] callbacks)> _spaceObjCallbackDic = new();
        private List<CollisionCallbackInfo> _temp = new();

        public bool Contains(CollisionCallbackInfo info)
        => _callbackDic.ContainsKey(info);
        public void ResetVisible()
        {
            _collisionEnterList.Clear();
            _collisionStayList.Clear();
            _collisionExitList.Clear();
            foreach(var pair in _callbackDic)
            {
                var item = pair.Value;
                item.visible = false;
                _callbackDic[pair.Key] = item;
            }
        }
        public void SetVisible(CollisionCallbackInfo callbackInfo, SpaceObject obj, CollisionInfo info)
        {
            if(!obj)
                return;
            if(!_callbackDic.TryGetValue(callbackInfo, out var item))
            {
                var cs = obj.GetComponents<IOnCollision>();

                if(!_spaceObjCallbackDic.TryGetValue(callbackInfo.collisionObj, out var value))
                    _spaceObjCallbackDic.Add(callbackInfo.collisionObj,(1, cs));
                else
                {
                    value.count++;
                    _spaceObjCallbackDic[callbackInfo.collisionObj] = value;
                }

                CollisionCallbackItem newItem = new(callbackInfo, info);
                _callbackDic.Add(callbackInfo, newItem);
                _collisionEnterList.Add(newItem);
                return;
            }
            if(item.visible)
                return;
            item.visible = true;
            item.collisionInfo = info;
            _callbackDic[callbackInfo] = item;
            _collisionStayList.Add(item);
        }
        public void ClearNotVisible()
        {
            _temp.Clear();
            foreach(var pair in _callbackDic)
                if(!pair.Value.visible)
                {
                    _collisionExitList.Add(pair.Value);
                    _temp.Add(pair.Key);
                }
            foreach(var info in _temp)
                _callbackDic.Remove(info);
        }
        public void ClearSpaceObjectCallbackDic()
        {
            foreach(var info in _temp)
                if(_spaceObjCallbackDic.TryGetValue(info.collisionObj, out var value))
                {
                    if(--value.count <= 0)
                        _spaceObjCallbackDic.Remove(info.collisionObj);
                    else
                        _spaceObjCallbackDic[info.collisionObj] = value;
                }
                else
                    Debug.LogError("【碰撞回调缓存】SpaceObject碰撞回调查找失败");
        }
        public IReadOnlyCollection<CollisionCallbackItem> CollisionEnterList
        => _collisionEnterList;
        public IReadOnlyCollection<CollisionCallbackItem> CollisionStayList
        => _collisionStayList;
        public IReadOnlyCollection<CollisionCallbackItem> CollisionExitList
        => _collisionExitList;

        public IReadOnlyDictionary<NativeHandle,(int count, IOnCollision[] callbacks)> SpaceObjectCallbackDic
        => _spaceObjCallbackDic;
    }
}