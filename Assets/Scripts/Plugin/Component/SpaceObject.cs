using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ
{
    [DefaultExecutionOrder(-10)]
    public class SpaceObject : MonoBehaviour , ISpaceZUpdate
    {
        public NativeHandle Handle => _handle;
        public HashSet<NativeHandle> colliderSet = new();
        private NativeHandle _handle;

        protected NativeTransformSynMsg _preTransform;

        void Awake()
        {
            _handle = this.Create();

            var colliders = GetComponentsInChildren<SpaceZCollider>();
            foreach(var collider in colliders)
                if(collider.Handle != NativeHandle.NULL)
                    collider.TryUpdateSpaceObject();
        }
        public void SpaceZUpdate()
        {
            NativeTransformSynMsg curr = new()
            {
                handle = _handle,
                position = transform.position,
                rotation = transform.rotation,
                scale = transform.lossyScale
            };
            if(curr != _preTransform)
                this.UpdateTransform(curr);
            _preTransform = curr;
        }
        void OnDestroy()
        => this.Destroy();
    }
}
