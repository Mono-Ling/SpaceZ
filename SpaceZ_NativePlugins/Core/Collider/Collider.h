#pragma once
#include"Core/Handle/HandleInstance.h"
#include"Core/SpaceGeomBody/Bound.h"
#include"Core/Tools/Transform.h"
#include"Core/Info/SynMsg/TransformSynMsg.h"
#include"Core/SpaceGeomBody/Convex/ConvexTransform.h"
namespace Core::SpaceZ
{
    class Collider
    {
    protected:
        ColliderHandle _handle;
        SpaceObjectHandle _objHandle;
        Bound _bound;
        Transform _transform;

    public:
        virtual ~Collider() = default;
        Collider()
        : _handle(ColliderHandle::null), _objHandle(SpaceObjectHandle::null), _transform(Transform()) {}

        ColliderHandle GetColliderHandle() const;
        SpaceObjectHandle GetSpaceObjectHandle() const;
        const Bound& GetBound()  const;
        Vector3 GetCenter() const;
        void UpdateParentTransform(const Transform& parent);

        virtual Vector3 Support(const Vector3& dir) const = 0;
        virtual ConvexTransform GetConvex() const = 0;
        virtual void UpdateLocalTransform(const TransformSynMsg& syn);
        virtual void UpdateBound();
    };
}
