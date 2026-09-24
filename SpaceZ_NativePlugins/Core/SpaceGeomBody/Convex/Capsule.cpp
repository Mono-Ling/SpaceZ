#include"Core/SpaceGeomBody/Convex/Capsule.h"
#include"Core/Tools/Math.h"

using namespace Core::Math;
namespace Core::SpaceZ
{
    void Capsule::SetCapsule(float height, float radius)
    {
        _height = height;
        _radius = radius;
    }
    Vector3 Capsule::SupportLocal(const Vector3& localDir) const
    {
        float halfHeight = _height / 2.0f;

        Vector3 A(0, -halfHeight, 0);
        Vector3 B(0, halfHeight, 0);
        Vector3 Q = Dot(localDir, B) > Dot(localDir, A) ? B : A;
        return Q + localDir.normalized() * _radius;
    }
}
