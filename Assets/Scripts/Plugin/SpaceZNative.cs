using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using SpaceZ.Framework.Plugin;
using SpaceZ.Framework.RunTime;
using UnityEngine;
using static SpaceZ.FunctionPtr;

namespace SpaceZ
{
public static partial class SpaceZPlugin
{
    private const string PLUGIN_NAME = "SpaceZ_NativePlugins";
#if UNITY_EDITOR
    private static NativePluginLifeCycle _startPlugin;
    private static NativePluginLifeCycle _stopPlugin;

    private static CreateCollider _createCollider;
    private static CreateSpaceObject _createSpaceObject;
    private static DestroyNativeInstance _destroyCollider;
    private static DestroyNativeInstance _destroySpaceObject;

    private static UpdateColliderSpaceObject _updateColliderSpaceObj;
    private static UpdateNativeTransform _updateColliderTransform;
    private static UpdateNativeTransform _updateSpaceObjectTransform;
    private static UpdateBoxCollider _updateBoxCollider;
    private static UpdateSphereCollider _updateSphereCollider;
    private static UpdateCapsuleCollider _updateCapsuleCollider;

    private static GetCount _getCollisionPairsCount;
    private static GetCollisionPairs _getCollisionPairs;

    private static NativeLoader _nativeLoader;
    [RunTimeStart(-100)]
    private static void InitializeNative()
    {
        if(_nativeLoader != null)
            return;
        try
        {
            _nativeLoader = new NativeLoader(PLUGIN_NAME);

            _nativeLoader.GetFunction("StartSpaceSystem", out _startPlugin);
            _nativeLoader.GetFunction("StopSpaceSystem", out _stopPlugin);

            _nativeLoader.GetFunction("UpdateColliderSpaceObject", out _updateColliderSpaceObj);
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
    private static void UninitializeNative()
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

        _updateColliderSpaceObj = null;
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

    private static NativeHandle CreateCollider(ColliderType type, NativeHandle spaceObj)
    => _createCollider?.Invoke(type, spaceObj) ?? NativeHandle.NULL;
    private static NativeHandle CreateSpaceObject()
    => _createSpaceObject?.Invoke() ?? NativeHandle.NULL;
    private static bool DestroyCollider(NativeHandle handle)
    => _destroyCollider?.Invoke(handle) ?? false;
    private static bool DestroySpaceObject(NativeHandle handle)
    => _destroySpaceObject?.Invoke(handle) ?? false;

    private static void UpdateColliderSpaceObject(NativeColliderSpaceObjectSynMsg msgs)
    => _updateColliderSpaceObj?.Invoke(msgs);
    private static unsafe void UpdateColliderTransform(NativeTransformSynMsg* msgs, int count)
    => _updateColliderTransform?.Invoke(msgs, count);
    private static unsafe void UpdateSpaceObjectTransform(NativeTransformSynMsg* msgs, int count)
    => _updateSpaceObjectTransform?.Invoke(msgs, count);
    private static unsafe void UpdateBoxCollider(NativeBoxColliderSynMsg* msgs, int count)
    => _updateBoxCollider?.Invoke(msgs, count);
    private static unsafe void UpdateSphereCollider(NativeSphereColliderSynMsg* msgs, int count)
    => _updateSphereCollider?.Invoke(msgs, count);
    private static unsafe void UpdateCapsuleCollider(NativeCapsuleColliderSynMsg* msgs, int count)
    => _updateCapsuleCollider?.Invoke(msgs, count);

    private static int GetCollisionPairsCount()
    => _getCollisionPairsCount?.Invoke() ?? -1;
    private static unsafe void GetCollisionPairs(CollisionPair* buffer, ref int bufferSize)
    => _getCollisionPairs?.Invoke(buffer, ref bufferSize);
#else
    [RunTimeStart(-90)]
    [DllImport(PLUGIN_NAME, EntryPoint = "StartSpaceSystem")]
    private static extern void StartSpaceSystem();

    [RunTimeEnd(90)]
    [DllImport(PLUGIN_NAME, EntryPoint = "StopSpaceSystem")]
    private static extern void StopSpaceSystem();


    [DllImport(PLUGIN_NAME, EntryPoint = "CreateCollider")]
    private static extern NativeHandle CreateCollider(ColliderType type, NativeHandle spaceObj);

    [DllImport(PLUGIN_NAME, EntryPoint = "CreateSpaceObject")]
    private static extern NativeHandle CreateSpaceObject();

    [DllImport(PLUGIN_NAME, EntryPoint = "DestroyCollider")]
    private static extern bool DestroyCollider(NativeHandle handle);

    [DllImport(PLUGIN_NAME, EntryPoint = "DestroySpaceObject")]
    private static extern bool DestroySpaceObject(NativeHandle handle);


    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateColliderSpaceObject")]
    private static extern void UpdateColliderSpaceObject(NativeColliderSpaceObjectSynMsg msgs);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateColliderTransform")]
    private static unsafe extern void UpdateColliderTransform(NativeTransformSynMsg* msgs, int count);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateSpaceObjectTransform")]
    private static unsafe extern void UpdateSpaceObjectTransform(NativeTransformSynMsg* msgs, int count);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateBoxCollider")]
    private static unsafe extern void UpdateBoxCollider(NativeBoxColliderSynMsg* msgs, int count);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateSphereCollider")]
    private static unsafe extern void UpdateSphereCollider(NativeSphereColliderSynMsg* msgs, int count);

    [DllImport(PLUGIN_NAME, EntryPoint = "UpdateCapsuleCollider")]
    private static unsafe extern void UpdateCapsuleCollider(NativeCapsuleColliderSynMsg* msgs, int count);


    [DllImport(PLUGIN_NAME, EntryPoint = "GetCollisionPairsCount")]
    private static extern int GetCollisionPairsCount();

    [DllImport(PLUGIN_NAME, EntryPoint = "GetCollisionPairs")]
    private static unsafe extern void GetCollisionPairs(CollisionPair* buffer, ref int bufferSize);
#endif
}
}