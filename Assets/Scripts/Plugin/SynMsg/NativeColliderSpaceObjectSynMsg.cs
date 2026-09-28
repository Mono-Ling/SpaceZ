using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace SpaceZ
{
    [StructLayout(LayoutKind.Sequential)]
    public struct NativeColliderSpaceObjectSynMsg
    {
        public NativeHandle collider;
        public NativeHandle spaceObj;
    }
}
