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
    public:
        Bound bound;
        Collider(Handle<Collider> handle, SpaceObjectHandle objHandle)
        : _handle(handle), _objHandle(objHandle) {}
        Collider(Handle<Collider> handle, SpaceObjectHandle objHandle, Bound bound)
        : _handle(handle), _objHandle(objHandle), bound(bound) {}

        Handle<Collider> GetColliderHandle() const;
        SpaceObjectHandle GetSpaceObjectHandle() const;
    };
    using ColliderHandle = Handle<Collider>;
}
