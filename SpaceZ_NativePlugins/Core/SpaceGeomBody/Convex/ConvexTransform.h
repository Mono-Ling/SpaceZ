#pragma once
#include"Core/SpaceGeomBody/Convex/Convex.h"
#include"Core/Tools/Vector3.h"

namespace Core::SpaceZ
{
    struct ConvexTransform
    {
        const Convex* convex;
        const Transform* transform;

        ConvexTransform(const Convex* convex, const Transform* transform) : convex(convex), transform(transform) {}

        inline Vector3 Support(const Vector3& dir) const
        {
            if(!convex || !transform)
                return Vector3::zero;
            return convex->Support(dir, *transform);
        }
        inline Vector3 GetCenter() const
        {
            if(!convex || !transform)
                return Vector3::zero;
            return transform->GetPosition();
        }
    };
}
