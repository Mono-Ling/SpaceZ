using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using SpaceZ;
using UnityEngine;


namespace SpaceZ
{
    [StructLayout(LayoutKind.Sequential)]
    public struct NativeTransformSynMsg
    {
        NativeHandle handle;
        Vector3 position;
        Vector3 scale;
        Quaternion rotation;
    }
}
