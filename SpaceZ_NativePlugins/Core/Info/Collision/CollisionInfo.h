#pragma once
#include"Core/Tools/Vector3.h"

namespace Core::SpaceZ
{
    struct CollisionInfo
    {
        float depth;
        Vector3 normal;
        Vector3 point;

        CollisionInfo() : depth(0), normal(Vector3::zero), point(Vector3::zero) {}
        CollisionInfo(float depth, const Vector3& normal, const Vector3& point)
        : depth(depth), normal(normal), point(point) {}
    };
}
