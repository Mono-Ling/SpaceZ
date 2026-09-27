using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpaceZ
{
    [StructLayout(LayoutKind.Sequential)]
    public struct CollisionObjInfo
    {
        public NativeHandle collider;
        public NativeHandle spaceObject;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CollisionInfo
    {
        public float depth;
        public Vector3 normal;
        public Vector3 point;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CollisionPair
    {
        public CollisionObjInfo first;
        public CollisionObjInfo second;
        public CollisionInfo collisionInfo;
    }
}
