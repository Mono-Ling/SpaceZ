#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Transform.h"
#include"Core/SpaceGeomBody/Convex/Convex.h"
#include<vector>

namespace Core::SpaceZ
{
    class Mesh : public Convex
    {
        std::vector<Vector3> _points;

    public:
        Mesh() : _points(std::vector<Vector3>()) {}
        Mesh(const std::vector<Vector3>& points)
        : _points(std::vector<Vector3>(points.begin(), points.end())) {}

        void SetPoints(const std::vector<Vector3>& points);
        virtual Vector3 SupportLocal(const Vector3& localDir) const override;
    };
}
