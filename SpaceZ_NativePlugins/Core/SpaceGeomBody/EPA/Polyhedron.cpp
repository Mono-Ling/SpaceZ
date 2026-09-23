#include"Core/SpaceGeomBody/EPA/Polyhedron.h"
#include"Core/Tools/Math.h"
#include<algorithm>
#include<cfloat>

using namespace std;
using namespace Core::Math;
namespace Core::SpaceZ
{
    Polyhedron::Polyhedron(const Simplex& simplex)
    {
        points = vector<Vector3>();
        planes = vector<Plane>();
        for (auto& p : simplex.points)
            if (p.first)
                points.push_back(p.second);
        if (simplex.pointCount < 3)
            return;
        auto A = simplex.points[0].second;
        auto B = simplex.points[1].second;
        auto C = simplex.points[2].second;
        auto D = simplex.points[3].second;
        planes.push_back(Plane(A, B, C));
        if (simplex.pointCount == 3)
            return;
        planes.push_back(Plane(A, B, D));
        planes.push_back(Plane(A, C, D));
        planes.push_back(Plane(B, C, D));
    }

    bool Polyhedron::SelectMinDisPlane(Vector3& normal, float& depth) const
    {
        auto index = MinDisPlaneIndex();
        if(index < 0)
            return false;
        normal = planes[index].normal;
        depth = planes[index].distance;
        return Length(normal) > Epsilon;
    }
    bool Polyhedron::IsApproachBound(const Vector3& newestPoint)
    {
        int best = MinDisPlaneIndex();
        if(best < 0)
            return true;
        const Plane& cur = planes[best];
        float len = Length(cur.normal);
        if (len < Epsilon)
            return true;
        return (Dot(cur.normal, newestPoint) / len) - cur.distance < Epsilon;
    }
    void Polyhedron::AddPoint(const Vector3& point)
    {
        points.push_back(point);
        for(auto& plane : planes)
        {
            if(!plane.used)
                break;
            if(Dot(plane.normal, point - plane.A) > Epsilon)
                plane.used = false;
        }
        CompactPlane();
        auto horizon = GetVisibleHorizon();
        RemoveVisiblePlane();
        for(auto& edge : horizon)
            planes.push_back(Plane(edge.first, edge.second, point));
    }

    int Polyhedron::MinDisPlaneIndex() const
    {
        float minDis = FLT_MAX;
        int minPlane = 0;
        for(int i = 0; i < planes.size(); i++)
        {
            if(!planes[i].used)
                break;
            if(planes[i].distance < minDis)
            {
                minPlane = i;
                minDis = planes[i].distance;
            }
        }
        if(Abs(minDis - FLT_MAX) < Epsilon)
            return -1;
        return minPlane;
    }
    void Polyhedron::CompactPlane()
    {
        partition(planes.begin(), planes.end(), [](const Plane& p) { return p.used; });
    }
    void Polyhedron::RemoveVisiblePlane()
    {
        planes.erase(remove_if(planes.begin(), planes.end(), [](const Plane& p) { return !p.used; }),
                     planes.end());
    }
    vector<pair<Vector3, Vector3>> Polyhedron::GetVisibleHorizon()
    {
        vector<pair<Vector3, Vector3>> ans;
        for (auto it = planes.rbegin(); it != planes.rend(); it++)
        {
            if (it->used)
                break;
            if(HasVisiblePlaneSharedEdge(it->A, it->B))
                ans.push_back({it->A, it->B});

            if(HasVisiblePlaneSharedEdge(it->B, it->C))
                ans.push_back({it->B, it->C});

            if(HasVisiblePlaneSharedEdge(it->C, it->A))
                ans.push_back({it->C, it->A});
        }
        return ans;
    }

    bool Polyhedron::HasVisiblePlaneSharedEdge(const Vector3& a, const Vector3& b)
    {
        int shared = 0;
        for (auto it = planes.rbegin(); it != planes.rend(); it++)
        {
            if (it->used)
                break;
            if (!it->HasEdge(a, b))
                continue;
            if(++shared >= 2)
                break;
        }
        return shared == 1;
    }
}
