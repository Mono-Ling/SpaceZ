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
        public NativeHandle handle;
        public Vector3 position;
        public Vector3 scale;
        public Quaternion rotation;

        public override bool Equals(object obj)
        {
            if(obj is not NativeTransformSynMsg transform)
                return false;
            return handle == transform.handle
                && position == transform.position
                && rotation == transform.rotation
                && scale == transform.scale;
        }
        public override int GetHashCode()
        => (handle,position,rotation,scale).GetHashCode();
        public static bool operator==(NativeTransformSynMsg a, NativeTransformSynMsg b)
        {
            return a.handle == b.handle
                && a.position == b.position
                && a.rotation == b.rotation
                && a.scale == b.scale;
        }
        public static bool operator!=(NativeTransformSynMsg a, NativeTransformSynMsg b)
        => !(a == b);
    }
}
