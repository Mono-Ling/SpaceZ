#pragma once
#include"Core/SpaceGeomBody/Convex/Capsule.h"
#include"Core/Collider/Collider.h"

namespace Core::SpaceZ
{
    class CapsuleCollider : public Collider
    {
    private:
        Capsule _capsule;

    public:
        inline CapsuleCollider(ColliderHandle handle, SpaceObjectHandle objHandle) : _capsule(Capsule())
        {
            _handle = handle;
            _objHandle = objHandle;
        }

        inline Vector3 Support(const Vector3& dir) const override
        {
            return _capsule.Support(dir, _transform);
        }
    };
}
