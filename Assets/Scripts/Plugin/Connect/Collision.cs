using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpaceZ
{
    [StructLayout(LayoutKind.Sequential)]
    public struct CollisionObjInfo
    {
        NativeHandle collider;
        NativeHandle spaceObject;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct CollisionInfo
    {
        float depth;
        Vector3 normal;
        Vector3 point;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CollisionPair
    {
        CollisionObjInfo first;
        CollisionObjInfo second;
        CollisionInfo collisionInfo;
    }
}
