#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Transform.h"
#include"Core/SpaceGeomBody/Convex/Convex.h"

namespace Core::SpaceZ
{
    class Sphere : public Convex
    {
        float _radius;
    public:
        Sphere() : _radius(1.0f) {}
        Sphere(float radius) : _radius(radius) {}

        void SetRadius(float radius);
        virtual Vector3 SupportLocal(const Vector3& localDir) const override;
    };
}
