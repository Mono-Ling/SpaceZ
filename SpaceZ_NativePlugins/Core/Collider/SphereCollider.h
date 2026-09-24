#pragma once
#include"Core/Collider/Collider.h"
#include"Core/SpaceGeomBody/Convex/Sphere.h"

namespace Core::SpaceZ
{
    class SphereCollider : public Collider
    {
    private:
        Sphere _sphere;

    public:
        inline SphereCollider(ColliderHandle handle, SpaceObjectHandle objHandle) : _sphere(Sphere())
        {
            _handle = handle;
            _objHandle = objHandle;
        }

        inline Vector3 Support(const Vector3& dir) const override
        {
            return _sphere.Support(dir, _transform);
        }
    };
}
