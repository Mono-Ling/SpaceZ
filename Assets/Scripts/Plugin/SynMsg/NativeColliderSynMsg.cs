using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpaceZ
{
    [StructLayout(LayoutKind.Sequential)]
    public struct NativeBoxColliderSynMsg
    {
        NativeHandle handle;
        Vector3 extents;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct NativeSphereColliderSynMsg
    {
        NativeHandle handle;
        float radius;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct NativeCapsuleColliderSynMsg
    {
        NativeHandle handle;
        float height;
        float radius;
    }
}
