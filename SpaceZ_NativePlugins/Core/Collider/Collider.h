#pragma once
#include"Core/Handle/Handle.h"
#include"Core/SpaceGeomBody/Bound.h"
#include"Core/SpaceObject/SpaceObject.h"
#include"Core/Tools/Transform.h"
#include"Core/Info/SynMsg/TransformSynMsg.h"
namespace Core::SpaceZ
{
    class Collider
    {
    protected:
        Handle<Collider> _handle;
        SpaceObjectHandle _objHandle;
        Bound _bound;
        Transform _transform;

    public:
        virtual ~Collider() = default;
        Collider()
        : _handle(Handle<Collider>::null), _objHandle(SpaceObjectHandle::null), _transform(Transform()) {}

        Handle<Collider> GetColliderHandle() const;
        SpaceObjectHandle GetSpaceObjectHandle() const;
        const Bound& GetBound()  const;
        Vector3 GetCenter() const;
        void UpdateParentTransform(const Transform& parent);

        virtual Vector3 Support(const Vector3& dir) const = 0;
        virtual void UpdateLocalTransform(const TransformSynMsg& syn);
        virtual void UpdateBound();
    };
    using ColliderHandle = Handle<Collider>;
}
