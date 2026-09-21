#pragma once
#include"Core/Collider/Collider.h"
#include"Core/SpaceObject/SpaceObject.h"

namespace Core::SpaceZ
{
    struct CollisionObjInfo
    {
        ColliderHandle colliderHandle;
        SpaceObjectHandle spaceObjHandle;

        CollisionObjInfo(ColliderHandle collider, SpaceObjectHandle spaceObj)
        : colliderHandle(collider), spaceObjHandle(spaceObj) {}
    };
}
