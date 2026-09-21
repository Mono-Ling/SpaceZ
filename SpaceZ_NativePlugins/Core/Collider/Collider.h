#pragma once
#include"Core/Handle/Handle.h"
#include"Core/SpaceGeomBody/Bound.h"
#include"Core/SpaceObject/SpaceObject.h"
namespace Core::SpaceZ
{
    class Collider
    {
    private:
        Handle<Collider> _handle;
        SpaceObjectHandle _objHandle;
        Bound _bound;
    public:
        Collider(Handle<Collider> handle, SpaceObjectHandle objHandle)
        : _handle(handle), _objHandle(objHandle) {}
        Collider(Handle<Collider> handle, SpaceObjectHandle objHandle, Bound bound)
        : _handle(handle), _objHandle(objHandle), _bound(bound) {}

        Handle<Collider> GetColliderHandle() const;
        SpaceObjectHandle GetSpaceObjectHandle() const;
        const Bound& GetBound()  const;
    };
    using ColliderHandle = Handle<Collider>;
}
