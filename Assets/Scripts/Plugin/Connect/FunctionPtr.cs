using System.Runtime.InteropServices;

namespace SpaceZ
{
public static class FunctionPtr
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void NativePluginLifeCycle();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate NativeHandle CreateCollider(ColliderType type, NativeHandle spaceObj);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate NativeHandle CreateSpaceObject();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate bool DestroyNativeInstance(NativeHandle handle);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public unsafe delegate void UpdateNativeTransform(NativeTransformSynMsg* msgs, int count);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public unsafe delegate void UpdateBoxCollider(NativeBoxColliderSynMsg* msgs, int count);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public unsafe delegate void UpdateSphereCollider(NativeSphereColliderSynMsg* msgs, int count);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public unsafe delegate void UpdateCapsuleCollider(NativeCapsuleColliderSynMsg* msgs, int count);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int GetCount();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public unsafe delegate void GetCollisionPairs(CollisionPair* buffer, ref int bufferSize);
}
}
