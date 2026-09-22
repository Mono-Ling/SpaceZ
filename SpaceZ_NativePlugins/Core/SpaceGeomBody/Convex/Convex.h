#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Transform.h"

namespace Core::SpaceZ
{
    class Convex
    {
    public:
        virtual ~Convex() = default;
        virtual Vector3 Support(const Vector3& dir, const Transform& transform) = 0;
    };
}
