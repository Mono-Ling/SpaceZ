using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ.Framework.RunTime
{
    public static class SpaceZUpdate
    {
        private static Action _update;
        private static float _preTime;
        public static void AddListener(Action action)
        => _update += action;
        public static void RemoveListener(Action action)
        => _update -= action;
        public static void Update()
        {
            if(Time.realtimeSinceStartup - _preTime < SpaceZTime.FRAME_DELAY)
                return;
            SpaceZTime.OnFrame();
            try
            {
                _update?.Invoke();
            }
            catch(Exception e)
            {
                Debug.LogException(e);
            }
            _preTime = Time.realtimeSinceStartup;
        }
    }
    public static class LateSpaceZUpdate
    {
        private static Action _update;
        private static float _preTime;
        public static void AddListener(Action action)
        => _update += action;
        public static void RemoveListener(Action action)
        => _update -= action;
        public static void Update()
        {
            if(Time.realtimeSinceStartup - _preTime < SpaceZTime.FRAME_DELAY)
                return;
            try
            {
                _update?.Invoke();
            }
            catch(Exception e)
            {
                Debug.LogException(e);
            }
            _preTime = Time.realtimeSinceStartup;
        }
    }
}
