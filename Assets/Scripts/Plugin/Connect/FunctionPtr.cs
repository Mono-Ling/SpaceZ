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
    public delegate void ActionFuncPtr<T>(T value) where T : unmanaged;

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public unsafe delegate void UnsafeAction<T,W>(T* ptr, W arg) where T : unmanaged where W : unmanaged;

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int GetCount();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public unsafe delegate void GetCollisionPairs(CollisionPair* buffer, ref int bufferSize);
}
}
