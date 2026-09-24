#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Transform.h"
#include"Core/SpaceGeomBody/Convex/Convex.h"

namespace Core::SpaceZ
{
    class Box : public Convex
    {
        Vector3 _extents;

    public:
        Box();
        Box(const Vector3& extents) : _extents(extents) {}

        void SetBox(const Vector3& extents);
        virtual Vector3 SupportLocal(const Vector3& localDir) const override;
    };
}
