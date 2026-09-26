#pragma once
#include"Core/Collider/Collider.h"
#include"Core/SpaceGeomBody/Convex/Box.h"
#include"Core/Tools/Math.h"

namespace Core::SpaceZ
{
    class BoxCollider : public Collider
    {
    private:
        Box _box;

    public:
        inline BoxCollider(ColliderHandle handle, SpaceObjectHandle objHandle) : _box(Box())
        {
            _handle = handle;
            _objHandle = objHandle;
        }

        inline Vector3 Support(const Vector3& dir) const override
        {
            return _box.Support(dir, _transform);
        }
        inline ConvexTransform GetConvex() const override
        {
            return ConvexTransform(&_box, &_transform);
        }

        inline void SetBox(const Vector3& extents)
        {
            _box.SetBox(extents);
            Collider::UpdateBound();
        }
    };
}
