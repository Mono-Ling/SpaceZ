using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ.Framework.RunTime
{
    public static class SpaceZTime
    {
        public const float FRAME_DELAY = 0.02f;
        public static float deltaTime {get; private set;}
        private static float _preFrameTime;
        public static void OnFrame()
        {
            deltaTime = Time.realtimeSinceStartup - _preFrameTime;
            if(_preFrameTime < Mathf.Epsilon)
                deltaTime = FRAME_DELAY;
            _preFrameTime = Time.realtimeSinceStartup;
        }
    }
}
