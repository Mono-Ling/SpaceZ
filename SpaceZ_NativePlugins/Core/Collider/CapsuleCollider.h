#pragma once
#include"Core/SpaceGeomBody/Convex/Capsule.h"
#include"Core/Collider/Collider.h"
#include"Core/Info/SynMsg/CapsuleSynMsg.h"

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
        inline ConvexTransform GetConvex() const override
        {
            return ConvexTransform(&_capsule, &_transform);
        }

        inline void SetCapsule(const CapsuleSynMsg& msg)
        {
            _capsule.SetCapsule(msg.height, msg.radius);
            Collider::UpdateBound();
        }
    };
}
