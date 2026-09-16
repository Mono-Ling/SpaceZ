using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public static class SpaceZ_NativePlugins
{
#if UNITY_EDITOR
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int AddDelegate(int a, int b);
    private static NativeLoader _nativeLoader;
    private static AddDelegate _add;
    public static void Initialize()
    {
        if (_nativeLoader != null) return;
        try
        {
            _nativeLoader = new NativeLoader(typeof(SpaceZ_NativePlugins).Name);
            _nativeLoader.GetFunction("Add", out _add);
        }
        catch (Exception ex)
        {
            Debug.LogError($"Native插件初始化失败：{ex}");
            _add = null;
        }
    }
    public static void Uninitialize()
    {
        if (_nativeLoader != null)
        {
            _nativeLoader.Dispose();
            _nativeLoader = null;
        }
        _add = null;
    }
    public static int Add(int a,int b) => _add?.Invoke(a,b) ?? default;
#else
    [DllImport(typeof(SpaceZ_NativePlugins).Name, EntryPoint = "Add")]
    public static extern int Add(int a,int b);
#endif
}