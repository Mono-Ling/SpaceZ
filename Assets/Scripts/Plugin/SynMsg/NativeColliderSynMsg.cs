using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpaceZ
{
    [StructLayout(LayoutKind.Sequential)]
    public struct NativeBoxColliderSynMsg
    {
        public NativeHandle handle;
        public Vector3 extents;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct NativeSphereColliderSynMsg
    {
        public NativeHandle handle;
        public float radius;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct NativeCapsuleColliderSynMsg
    {
        public NativeHandle handle;
        public float height;
        public float radius;
    }
}
