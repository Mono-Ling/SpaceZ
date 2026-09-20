#include"Core/Collider/Collider.h"

namespace Core::SpaceZ
{
    ColliderHandle Collider::GetColliderHandle() const
    {
        return this->_handle;
    }
    SpaceObjectHandle Collider::GetSpaceObjectHandle() const
    {
        return this->_objHandle;
    }
}
