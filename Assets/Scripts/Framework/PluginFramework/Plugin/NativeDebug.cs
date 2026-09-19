using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using SpaceZ.Framework.Plugin;
using SpaceZ.Framework.RunTime;
using UnityEngine;

namespace SpaceZ.Plugin
{
public static class NativeDebug
{
#if UNITY_EDITOR
    private const string PLUGIN_NAME = "SpaceZ_NativePlugins";
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void DebugCallback(IntPtr msgPtr, int size);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void InjectDebugCallback(DebugCallback logPtr, DebugCallback warningPtr, DebugCallback errorPtr);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void UnloadDebugCallback();

    [AOT.MonoPInvokeCallback(typeof(DebugCallback))]
    static void OnLogDebug(IntPtr msgPtr, int size)
    => Debug.Log(Marshal.PtrToStringUTF8(msgPtr,size));

    [AOT.MonoPInvokeCallback(typeof(DebugCallback))]
    static void OnWarningDebug(IntPtr msgPtr, int size)
    => Debug.LogWarning(Marshal.PtrToStringUTF8(msgPtr,size));

    [AOT.MonoPInvokeCallback(typeof(DebugCallback))]
    static void OnErrorDebug(IntPtr msgPtr, int size)
    => Debug.LogError(Marshal.PtrToStringUTF8(msgPtr,size));
    private static NativeLoader _nativeLoader;
    private static InjectDebugCallback _injectCallback;
    private static UnloadDebugCallback _unloadCallback;

    private static DebugCallback _logCallback;
    private static DebugCallback _warningCallback;
    private static DebugCallback _errorCallback;
    [RunTimeStart(-90)]
    private static void Initialize()
    {
        if (_nativeLoader != null) return;
        try
        {
            _nativeLoader = new NativeLoader(PLUGIN_NAME);
            _nativeLoader.GetFunction("InjectDebugCallback", out _injectCallback);
            _nativeLoader.GetFunction("UnloadDebugCallback",out _unloadCallback);
            
            _logCallback = OnLogDebug;
            _warningCallback = OnWarningDebug;
            _errorCallback = OnErrorDebug;
            _injectCallback?.Invoke(_logCallback,_warningCallback,_errorCallback);
        }
        catch (Exception ex)
        {
            Debug.LogError($"【NativeDebug】Native插件初始化失败：{ex}");
            _injectCallback = null;
        }
    }
    [RunTimeEnd(90)]
    private static void Uninitialize()
    {
        try
        {
            _unloadCallback?.Invoke();
        }
        catch (Exception ex)
        {
            Debug.LogError($"【NativeDebug】卸载回调异常：{ex}");
        }

        if (_nativeLoader != null)
        {
            _nativeLoader.Dispose();
            _nativeLoader = null;
        }
        _injectCallback = null;
    }
#endif
}
}
