using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        SpaceZ_NativePlugins.Initialize();
        Debug.Log($"【Test】Add(1,2) = {SpaceZ_NativePlugins.Add(1,2)}");
    }
    void OnDestroy()
    => SpaceZ_NativePlugins.Uninitialize();
}
