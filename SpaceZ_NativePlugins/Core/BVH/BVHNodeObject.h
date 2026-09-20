#pragma once
#include"Core/SpaceGeomBody/Bound.h"
#include"Core/Handle/Handle.h"
#include"Core/Collider/Collider.h"

namespace Core::SpaceZ
{
    struct BVHNodeObject
    {
        Bound bound;
        ColliderHandle colliderHandle;
        SpaceObjectHandle spaceObjHandle;
        BVHNodeObject(Bound bound, ColliderHandle collider, SpaceObjectHandle spaceObj)
          : bound(bound),
            colliderHandle(collider),
            spaceObjHandle(spaceObj) {}
        BVHNodeObject(const Collider& colider)
          : bound(colider.bound),
            colliderHandle(colider.GetColliderHandle()),
            spaceObjHandle(colider.GetSpaceObjectHandle()) {}

        static const BVHNodeObject null;
    };
    bool operator==(const BVHNodeObject& a, const BVHNodeObject& b);
    bool operator!=(const BVHNodeObject& a, const BVHNodeObject& b);
    bool IsIntersect(const BVHNodeObject& a, const BVHNodeObject& b);
}
