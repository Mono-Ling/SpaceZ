using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public static class SpaceZ_NativePlugins
{
    private const string PLUGIN_NAME = nameof(SpaceZ_NativePlugins);
#if UNITY_EDITOR
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int AddDelegate(int a, int b);
    private static NativeLoader _nativeLoader;
    private static AddDelegate _add;
    [RunTimeStart(-100)]
    private static void Initialize()
    {
        Debug.Log("【SpaceZ_NativePlugins】Native插件初始化");
        if (_nativeLoader != null) return;
        try
        {
            _nativeLoader = new NativeLoader(PLUGIN_NAME);
            _nativeLoader.GetFunction("Add", out _add);
        }
        catch (Exception ex)
        {
            Debug.LogError($"【SpaceZ_NativePlugins】Native插件初始化失败：{ex}");
            _add = null;
        }
    }
    [RunTimeEnd(100)]
    private static void Uninitialize()
    {
        Debug.Log("【SpaceZ_NativePlugins】Native插件卸载");
        if (_nativeLoader != null)
        {
            _nativeLoader.Dispose();
            _nativeLoader = null;
        }
        _add = null;
    }
    public static int Add(int a,int b) => _add?.Invoke(a,b) ?? default;
#else
    [DllImport(PLUGIN_NAME, EntryPoint = "Add")]
    public static extern int Add(int a,int b);
#endif
}