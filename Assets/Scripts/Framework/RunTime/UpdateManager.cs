using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.LowLevel;
using UnityEngine.PlayerLoop;

namespace SpaceZ.Framework.RunTime
{
    public static class UpdateManager
    {
        [RunTimeStart(-100)]
        private static void InjectUpdate()
        {
            var loop = PlayerLoop.GetCurrentPlayerLoop();
            PlayerLoopSystem custom = new()
            {
                type = typeof(LateSpaceZUpdate),
                updateDelegate = LateSpaceZUpdate.Update,
                subSystemList = null
            };
            if(!InsertLoopBefore(ref loop, typeof(PreUpdate), custom))
            {
                Debug.LogError($"【SpaceZ UpdateManager】{typeof(LateSpaceZUpdate)}帧更新插入失败");
                return;
            }

            custom = new()
            {
                type = typeof(SpaceZUpdate),
                updateDelegate = SpaceZUpdate.Update,
                subSystemList = null
            };
            if(!InsertLoopBefore(ref loop, typeof(LateSpaceZUpdate), custom))
                Debug.LogError($"【SpaceZ UpdateManager】{typeof(SpaceZUpdate)}帧更新插入失败");
            else
                PlayerLoop.SetPlayerLoop(loop);
        }
        private static bool InsertLoopBefore(ref PlayerLoopSystem root, Type target, PlayerLoopSystem insert)
        {
            if(root.subSystemList == null || root.subSystemList.Length == 0)
                return false;
            for(int i = 0; i < root.subSystemList.Length; i++)
            {
                if(root.subSystemList[i].type == target)
                {
                    List<PlayerLoopSystem> list = new(root.subSystemList);
                    list.Insert(i, insert);
                    root.subSystemList = list.ToArray();
                    return true;
                }
                if(InsertLoopBefore(ref root.subSystemList[i], target, insert))
                    return true;
            }
            return false;
        }
    }
}