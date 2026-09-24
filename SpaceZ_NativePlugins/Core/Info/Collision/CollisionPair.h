#pragma once
#include"Core/Info/Collision/CollisionObjInfo.h"
#include"Core/Info/Collision/CollisionInfo.h"

namespace Core::SpaceZ
{
    struct CollisionPair
    {
        CollisionObjInfo first;
        CollisionObjInfo second;
        CollisionInfo collisionInfo;// first -> second

        CollisionPair(CollisionObjInfo first, CollisionObjInfo second) : first(first), second(second) {}

        inline void SetCollisionInfo(const CollisionInfo info)
        {
            collisionInfo = info;
        }
    };
}
