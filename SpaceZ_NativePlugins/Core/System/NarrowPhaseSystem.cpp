#include"Core/System/NarrowPhaseSystem.h"
#include"Core/SpaceGeomBody/EPA/Polyhedron.h"
#include"Core/Tools/Math.h"
#include<algorithm>

using namespace std;
using namespace Core::Math;
namespace Core::SpaceZ
{
    void NarrowPhaseSystem::Clear()
    {
        _detectPairs.clear();
    }
    void NarrowPhaseSystem::AddDetectPair(const NarrowPhaseDetectPair& pair)
    {
        _detectPairs.push_back(pair);
    }
    const std::vector<CollisionPair>& NarrowPhaseSystem::GetCollisionPairs()
    {
        if(_detectPairs.empty())
            return _collisionPairs;
        for(auto& p : _detectPairs)
            CollisionDetection(p);

        partition(_detectPairs.begin(), _detectPairs.end(), [](const NarrowPhaseDetectPair& p)
        {
            return p.isCollision;
        });

        _collisionPairs.clear();
        for(auto& p : _detectPairs)
            if(p.isCollision)
                _collisionPairs.push_back(p.collisionPair);
            else
                break;

        return _collisionPairs;
    }

    void NarrowPhaseSystem::CollisionDetection(NarrowPhaseDetectPair& pair)
    {
        Simplex simplex;
        if(!GJK(pair.first, pair.second, simplex))
            return;
        pair.isCollision = true;
        pair.collisionPair.SetCollisionInfo(EPA(pair.first, pair.second, simplex));
    }

    bool NarrowPhaseSystem::GJK(const Collider* first, const Collider* second, Simplex& simplex)
    {
        if(!first || !second)
            return false;
        Vector3 dir = second->GetCenter() - first->GetCenter();
        if(Length(dir) < Epsilon)
            dir = Vector3::right;
        Vector3 support = Vector3::zero;

        for(int i = 0; i < MAX_ITERATION; i++)
        {
            support = GetMinkowskiDiff(first, second, dir);
            if(Dot(dir, support) < 0)
                return false;
            simplex.Add(support);
            if(simplex.ContainsOrigin(dir))
                return true;
        }
        return false;
    }
    CollisionInfo NarrowPhaseSystem::EPA(const Collider* first, const Collider* second, Simplex& simplex)
    {
        CompleteSimplex(first, second, simplex);
        Polyhedron polyhedron(simplex);
        Vector3 normal;
        float depth = 0;
        Vector3 support;
        for(int i = 0; i < MAX_ITERATION; i++)
        {
            polyhedron.SelectMinDisPlane(normal, depth);
            support = GetMinkowskiDiff(first, second, normal);
            if(polyhedron.IsApproachBound(support))
                break;
            polyhedron.AddPoint(support);
        }
        return CollisionInfo(depth, normal.normalized(), support / 2);
    }

    Vector3 GetMinkowskiDiff(const Collider* first, const Collider* second, const Vector3& dir)
    {
        if(!first || !second)
            return Vector3::zero;
        return first->Support(dir) - second->Support(-dir);
    }
    void CompleteSimplex(const Collider* first, const Collider* second, Simplex& simplex)
    {
        simplex.ClearCollineation();
        if(simplex.pointCount == 1)
            simplex.Add(GetMinkowskiDiff(first, second, -simplex.points[0].second));

        if(simplex.pointCount == 2)
        {
            auto dir = simplex.points[1].second - simplex.points[0].second;
            auto axis = (Abs(dir.x) < Abs(dir.y) && Abs(dir.x) < Abs(dir.z)) ? Vector3::right :
                        (Abs(dir.y) < Abs(dir.z) ? Vector3::up : Vector3::forward);
            auto u = Normalize(Cross(dir, axis));
            auto v = Normalize(Cross(dir, u));
            auto support = GetMinkowskiDiff(first, second, u);
            if(Length(support) < Epsilon)
                support = GetMinkowskiDiff(first, second, v);
            simplex.Add(support);
        }

        if(simplex.pointCount == 3)
        {
            auto A = simplex.points[0].second;
            auto B = simplex.points[1].second;
            auto C = simplex.points[2].second;
            auto ABC = Cross(B - A,  C - A);

            Vector3 support;
            if(Dot(ABC, -A) > 0)
                support = GetMinkowskiDiff(first, second, ABC);
            else
                support = GetMinkowskiDiff(first, second, -ABC);
            simplex.Add(support);
        }
    }
}
