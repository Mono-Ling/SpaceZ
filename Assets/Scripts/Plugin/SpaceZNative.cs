using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using SpaceZ.Framework.Plugin;
using SpaceZ.Framework.RunTime;
using UnityEngine;

namespace SpaceZ
{
public class SpaceZNative
{
    private const string PLUGIN_NAME = "SpaceZ_NativePlugins";
#if UNITY_EDITOR
    private static FunctionPtr.NativePluginLifeCycle _startPlugin;
    private static FunctionPtr.NativePluginLifeCycle _stopPlugin;

    private static FunctionPtr.CreateCollider _createCollider;
    private static FunctionPtr.CreateSpaceObject _createSpaceObject;
    private static FunctionPtr.DestroyNativeInstance _destroyCollider;
    private static FunctionPtr.DestroyNativeInstance _destroySpaceObject;

    private static FunctionPtr.UpdateNativeTransform _updateColliderTransform;
    private static FunctionPtr.UpdateNativeTransform _updateSpaceObjectTransform;
    private static FunctionPtr.UpdateBoxCollider _updateBoxCollider;
    private static FunctionPtr.UpdateSphereCollider _updateSphereCollider;
    private static FunctionPtr.UpdateCapsuleCollider _updateCapsuleCollider;

    private static FunctionPtr.GetCount _getCollisionPairsCount;
    private static FunctionPtr.GetCollisionPairs _getCollisionPairs;

    private static NativeLoader _nativeLoader;
    [RunTimeStart(-100)]
    private static void Initialize()
    {
        if(_nativeLoader != null)
            return;
        try
        {
            _nativeLoader = new NativeLoader(PLUGIN_NAME);

            _nativeLoader.GetFunction("StartSpaceSystem", out _startPlugin);
            _nativeLoader.GetFunction("StopSpaceSystem", out _stopPlugin);

            _nativeLoader.GetFunction("CreateCollider", out _createCollider);
            _nativeLoader.GetFunction("CreateSpaceObject", out _createSpaceObject);
            _nativeLoader.GetFunction("DestroyCollider", out _destroyCollider);
            _nativeLoader.GetFunction("DestroySpaceObject", out _destroySpaceObject);

            _nativeLoader.GetFunction("UpdateColliderTransform", out _updateColliderTransform);
            _nativeLoader.GetFunction("UpdateSpaceObjectTransform", out _updateSpaceObjectTransform);
            _nativeLoader.GetFunction("UpdateBoxCollider", out _updateBoxCollider);
            _nativeLoader.GetFunction("UpdateSphereCollider", out _updateSphereCollider);
            _nativeLoader.GetFunction("UpdateCapsuleCollider", out _updateCapsuleCollider);

            _nativeLoader.GetFunction("GetCollisionPairsCount", out _getCollisionPairsCount);
            _nativeLoader.GetFunction("GetCollisionPairs", out _getCollisionPairs);
        }
        catch (Exception ex)
        {
            Debug.LogError($"【SpaceZ Native】Native插件初始化失败：{ex}");
            return;
        }
        Debug.Log("【SpaceZ Native】Native插件加载");
    }
    [RunTimeEnd(100)]
    private static void Uninitialize()
    {
        Debug.Log("【SpaceZ Native】Native插件卸载");
        if (_nativeLoader != null)
        {
            _nativeLoader.Dispose();
            _nativeLoader = null;
        }
        _startPlugin = null;
        _stopPlugin = null;

        _createCollider = null;
        _createSpaceObject = null;
        _destroyCollider = null;
        _destroySpaceObject = null;

        _updateColliderTransform = null;
        _updateSpaceObjectTransform = null;
        _updateBoxCollider = null;
        _updateSphereCollider = null;
        _updateCapsuleCollider = null;

        _getCollisionPairsCount = null;
        _getCollisionPairs = null;
    }
    [RunTimeStart(-90)]
    private static void StartSpaceSystem() => _startPlugin?.Invoke();
    [RunTimeEnd(90)]
    private static void StopSpaceSystem() => _stopPlugin?.Invoke();

    public static NativeHandle CreateCollider(ColliderType type, NativeHandle spaceObj)
    => _createCollider?.Invoke(type, spaceObj) ?? NativeHandle.NULL;
    public static NativeHandle CreateSpaceObject()
    => _createSpaceObject?.Invoke() ?? NativeHandle.NULL;
    public static bool DestroyCollider(NativeHandle handle)
    => _destroyCollider?.Invoke(handle) ?? false;
    public static bool DestroySpaceObject(NativeHandle handle)
    => _destroySpaceObject?.Invoke(handle) ?? false;

    public static unsafe void UpdateColliderTransform(NativeTransformSynMsg* msgs, int count)
    => _updateColliderTransform?.Invoke(msgs, count);
    public static unsafe void UpdateSpaceObjectTransform(NativeTransformSynMsg* msgs, int count)
    => _updateSpaceObjectTransform?.Invoke(msgs, count);
    public static unsafe void UpdateBoxCollider(NativeBoxColliderSynMsg* msgs, int count)
    => _updateBoxCollider?.Invoke(msgs, count);
    public static unsafe void UpdateSphereCollider(NativeSphereColliderSynMsg* msgs, int count)
    => _updateSphereCollider?.Invoke(msgs, count);
    public static unsafe void UpdateCapsuleCollider(NativeCapsuleColliderSynMsg* msgs, int count)
    => _updateCapsuleCollider?.Invoke(msgs, count);

    public static int GetCollisionPairsCount()
    => _getCollisionPairsCount?.Invoke() ?? -1;
    public static unsafe void GetCollisionPairs(CollisionPair* buffer, ref int bufferSize)
    => _getCollisionPairs?.Invoke(buffer, ref bufferSize);
#else
    [RunTimeStart(-90)]
    [DllImport(PLUGIN_NAME, EntryPoint = "StartSpaceSystem")]
    private static extern void StartSpaceSystem();

    [RunTimeEnd(90)]
    [DllImport(PLUGIN_NAME, EntryPoint = "StopSpaceSystem")]
    private static extern void StopSpaceSystem();


    [DllImport(PLUGIN_NAME, EntryPoint = "CreateCollider")]
    public static extern NativeHandle CreateCollider(ColliderType type, NativeHandle spaceObj);

    [DllImport(PLUGIN_NAME, EntryPoint = "CreateSpaceObject")]
    public static extern NativeHandle CreateSpaceObject();

    [DllImport(PLUGIN_NAME, EntryPoint = "DestroyCollider")]
    public static extern bool DestroyCollider(NativeHandle handle);

    [DllImport(PLUGIN_NAME, EntryPoint = "DestroySpaceObject")]
    public static extern bool DestroySpaceObject(NativeHandle handle);


    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateColliderTransform")]
    public static unsafe extern void UpdateColliderTransform(NativeTransformSynMsg* msgs, int count);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateSpaceObjectTransform")]
    public static unsafe extern void UpdateSpaceObjectTransform(NativeTransformSynMsg* msgs, int count);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateBoxCollider")]
    public static unsafe extern void UpdateBoxCollider(NativeBoxColliderSynMsg* msgs, int count);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateSphereCollider")]
    public static unsafe extern void UpdateSphereCollider(NativeSphereColliderSynMsg* msgs, int count);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateCapsuleCollider")]
    public static unsafe extern void UpdateCapsuleCollider(NativeCapsuleColliderSynMsg* msgs, int count);


    [DllImport(PLUGIN_NAME, EntryPoint = "GetCollisionPairsCount")]
    public static extern int GetCollisionPairsCount();

    [DllImport(PLUGIN_NAME, EntryPoint = "GetCollisionPairs")]
    public static unsafe extern void GetCollisionPairs(CollisionPair* buffer, ref int bufferSize);
#endif
}
}
