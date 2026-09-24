#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Transform.h"

namespace Core::SpaceZ
{
    class Convex
    {
    public:
        virtual ~Convex() = default;

        inline virtual Vector3 Support(const Vector3& dir, const Transform& transform) const
        {
            auto localDir = transform.NormalToLocal(dir);
            return transform.PointToWorld(SupportLocal(localDir));
        }
        virtual Vector3 SupportLocal(const Vector3& localDir) const = 0;
    };
}
