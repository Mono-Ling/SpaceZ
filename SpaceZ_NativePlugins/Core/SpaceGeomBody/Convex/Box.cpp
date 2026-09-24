#include"Core/SpaceGeomBody/Convex/Box.h"

namespace Core::SpaceZ
{
    Box::Box()
    {
        SetBox(Vector3::one);
    }
    void Box::SetBox(const Vector3& extents)
    {
        this->_extents = extents;
    }
    Vector3 Box::SupportLocal(const Vector3& localDir) const
    {
        return Vector3
        {
            _extents.x * (localDir.x > 0 ? 1 : -1),
            _extents.y * (localDir.y > 0 ? 1 : -1),
            _extents.z * (localDir.z > 0 ? 1 : -1)
        };
    }
}
