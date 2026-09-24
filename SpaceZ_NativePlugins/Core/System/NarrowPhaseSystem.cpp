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
    CollisionInfo NarrowPhaseSystem::EPA(const Collider* first, const Collider* second, const Simplex& simplex)
    {
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
}
