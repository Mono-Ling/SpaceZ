using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using UnityEngine;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace SpaceZ.Framework.RunTime
{
public static class RunTimeManager
{
    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
    private static void RunTimeStart()
    {
        InvokeRunTimeAttributeMethods<RunTimeStartAttribute>();
#if UNITY_EDITOR
        EditorApplication.playModeStateChanged += OnPlayModeStateChanged;
#else
        Application.quitting += RunTimeEnd;
#endif
    }
    private static void RunTimeEnd()
    {
        InvokeRunTimeAttributeMethods<RunTimeEndAttribute>();
#if UNITY_EDITOR
        EditorApplication.playModeStateChanged -= OnPlayModeStateChanged;
#else
        Application.quitting -= RunTimeEnd;
#endif
    }
#if UNITY_EDITOR
    private static void OnPlayModeStateChanged(PlayModeStateChange state)
    {
        if (state == PlayModeStateChange.ExitingPlayMode)
        {
            RunTimeEnd();
            EditorApplication.playModeStateChanged -= OnPlayModeStateChanged;
        }
    }
#endif
    private static void InvokeRunTimeAttributeMethods<T>() where T : RunTimeAttribute
    {
        List<(int order, MethodInfo method)> methods = new();
        foreach (var asm in AppDomain.CurrentDomain.GetAssemblies())
        {
            foreach (var type in asm.GetTypes())
            {
                foreach (var method in type.GetMethods(
                    BindingFlags.Static
                  | BindingFlags.Public
                  | BindingFlags.NonPublic))
                {
                    var attr = method.GetCustomAttributes(typeof(T), false);
                    if (attr.Length > 0 && attr[0] is T runTimeAttr)
                        methods.Add((runTimeAttr.Order, method));
                }
            }
        }
        methods.Sort((a, b) => a.order.CompareTo(b.order));
        foreach (var (order, method) in methods)
        {
            try
            {
                method.Invoke(null, null);
            }
            catch (Exception ex)
            {
                Debug.LogError($"【RunTimeManager】{typeof(T).Name}方法执行失败：{ex}");
            }
        }
    }
}
}