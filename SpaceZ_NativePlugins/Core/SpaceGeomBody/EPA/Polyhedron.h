#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/SpaceGeomBody/Simplex.h"
#include"Core/SpaceGeomBody/EPA/Plane.h"
#include<vector>

namespace Core::SpaceZ
{
    struct Polyhedron
    {
        std::vector<Vector3> points;
        std::vector<Plane> planes;
        Polyhedron() : points(std::vector<Vector3>()), planes(std::vector<Plane>()) {}
        Polyhedron(const Simplex& simplex);

        bool SelectMinDisPlane(Vector3& normal, float& depth) const;
        bool IsApproachBound(const Vector3& newestPoint);
        void AddPoint(const Vector3& point);

    private:
        void CompactPlane();
        void RemoveVisiblePlane();
        std::vector<std::pair<Vector3,Vector3>> GetVisibleHorizon();
        bool HasVisiblePlaneSharedEdge(const Vector3& a, const Vector3& b);
        int MinDisPlaneIndex() const;
    };
}
