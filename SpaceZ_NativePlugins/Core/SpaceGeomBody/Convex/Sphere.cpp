#include"Core/SpaceGeomBody/Convex/Sphere.h"

namespace Core::SpaceZ
{
    void Sphere::SetRadius(float radius)
    {
        _radius = radius;
    }
    Vector3 Sphere::SupportLocal(const Vector3& dir) const
    {
        return dir.normalized() * _radius;
    }
}
