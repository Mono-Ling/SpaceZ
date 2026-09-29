using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ
{
    public class SpaceZCollider : MonoBehaviour , ISpaceZUpdate
    {
        public NativeHandle Handle => _handle;
        public NativeHandle SpaceObject => _spaceObj;
        protected NativeHandle _handle;
        protected NativeHandle _spaceObj;

        protected NativeTransformSynMsg _preTransform;
        protected Transform _spaceObjTransform;

        protected virtual void OnTransformParentChanged()
        {
            var obj = transform.GetComponentInParent<SpaceObject>(true);
            var curr = obj?.Handle ?? NativeHandle.NULL;
            if(curr != _spaceObj)
                this.UpdateSpaceObject(curr);
            _spaceObjTransform = obj?.transform ?? null;
            _spaceObj = curr;
        }
        public virtual void SpaceZUpdate()
        {
            NativeTransformSynMsg curr = new();
            curr.handle = _handle;
            if(_spaceObjTransform == null)
            {
                curr.position = transform.position;
                curr.rotation = transform.rotation;
                curr.scale = transform.lossyScale;
            }
            else
            {
                var m = _spaceObjTransform.worldToLocalMatrix * transform.localToWorldMatrix;
                curr.position = m.GetPosition();
                curr.rotation = m.rotation;
                curr.scale = m.lossyScale;
            }
            if(curr != _preTransform)
                this.UpdateTransform(curr);
            _preTransform = curr;
        }
        protected virtual void OnDisable()
        => this.Destroy();
        protected virtual void OnDestroy()
        => this.Destroy();
        protected void CreateCollider(ColliderType type)
        {
            var obj = transform.GetComponentInParent<SpaceObject>(true);
            _spaceObj = obj?.Handle ?? NativeHandle.NULL;
            _spaceObjTransform = obj?.transform ?? null;
            _handle = this.Create(type, _spaceObj);
        }
        public void SetHandle(NativeHandle handle)
        => _handle = handle;
        public void SetSpaceObject(NativeHandle handle)
        =>_spaceObj = handle;
    }
}