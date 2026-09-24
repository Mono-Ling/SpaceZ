#pragma once
#include"Core/SpaceGeomBody/Convex/Convex.h"
#include"Core/Tools/Transform.h"
#include"Core/Tools/Vector3.h"

namespace Core::SpaceZ
{
    class Capsule : public Convex
    {
        float _height;
        float _radius;

    public:
        Capsule() : _height(1.0f), _radius(0.5f) {}
        Capsule(float height, float radius) : _height(height), _radius(radius) {}

        void SetCapsule(float height, float radius);
        virtual Vector3 SupportLocal(const Vector3& localDir) const override;
    };
}
