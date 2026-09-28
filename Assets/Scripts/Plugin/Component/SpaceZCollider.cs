using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ
{
    public class SpaceZCollider : MonoBehaviour
    {
        public NativeHandle Handle => _handle;
        protected NativeHandle _handle;
        protected NativeHandle _spaceObj;

        protected virtual void OnDisable()
        => this.Destroy();
        protected void CreateCollider(ColliderType type)
        {
            var obj = transform.GetComponentInParent<SpaceObject>();
            _spaceObj = obj != null ? obj.Handle : NativeHandle.NULL;
            _handle = this.Create(type, _spaceObj);
        }
    }
}