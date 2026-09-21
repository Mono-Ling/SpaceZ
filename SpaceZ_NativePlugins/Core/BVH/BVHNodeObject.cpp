#include"Core/BVH/BVHNodeObject.h"
#include"Core/SpaceGeomBody/Bound.h"

namespace Core::SpaceZ
{
    const BVHNodeObject BVHNodeObject::null =
        BVHNodeObject(Bound(), ColliderHandle::null, SpaceObjectHandle::null);

    bool operator<(const BVHNodeObject& a, const BVHNodeObject& b)
    {
        return a.colliderHandle < b.colliderHandle;
    }
    bool operator==(const BVHNodeObject& a, const BVHNodeObject& b)
    {
        return a.colliderHandle == b.colliderHandle
            && a.spaceObjHandle == b.spaceObjHandle;
    }
    bool operator!=(const BVHNodeObject& a, const BVHNodeObject& b)
    {
        return !(a == b);
    }
    bool IsIntersect(const BVHNodeObject& a, const BVHNodeObject& b)
    {
        if(a == BVHNodeObject::null || b == BVHNodeObject::null)
            return false;
        if(a.spaceObjHandle == b.spaceObjHandle)
            return false;
        return IsIntersect(a.bound,b.bound);
    }
    CollisionPair GetCollisionPair(const BVHNodeObject& a, const BVHNodeObject& b)
    {
        return CollisionPair(
            {a.colliderHandle,a.spaceObjHandle},
            {b.colliderHandle,b.spaceObjHandle});
    }
}
