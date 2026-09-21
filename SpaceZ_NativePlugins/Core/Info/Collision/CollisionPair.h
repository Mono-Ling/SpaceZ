#pragma once
#include"Core/Info/Collision/CollisionObjInfo.h"

namespace Core::SpaceZ
{
    struct CollisionPair
    {
        CollisionObjInfo first;
        CollisionObjInfo second;
        // 碰撞点、法线等

        CollisionPair(CollisionObjInfo first, CollisionObjInfo second)
        : first(first), second(second) {}
    };
}
