#include"Core/SpaceGeomBody/Convex/Sphere.h"
#include"Core/Tools/Math.h"

namespace Core::SpaceZ
{
    void Sphere::SetRadius(float radius)
    {
        _radius = Math::Abs(radius);
    }
    Vector3 Sphere::SupportLocal(const Vector3& dir) const
    {
        return dir.normalized() * _radius;
    }
}
