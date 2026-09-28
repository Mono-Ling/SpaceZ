using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ
{
    [DefaultExecutionOrder(-10)]
    public class SpaceObject : MonoBehaviour
    {
        public NativeHandle Handle => _handle;
        public HashSet<NativeHandle> colliderSet = new();
        private NativeHandle _handle;
        void Awake()
        => _handle = this.Create();
        void OnDestroy()
        => this.Destroy();
    }
}
