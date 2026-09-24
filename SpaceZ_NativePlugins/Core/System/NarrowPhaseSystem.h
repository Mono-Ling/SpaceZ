#pragma once
#include"Core/Info/Collision/CollisionPair.h"
#include"Core/Info/Collision/CollisionInfo.h"
#include"Core/SpaceGeomBody/Simplex.h"
#include<vector>

namespace Core::SpaceZ
{
    struct NarrowPhaseDetectPair
    {
        bool isCollision;
        Collider* first;
        Collider* second;
        CollisionPair collisionPair;

        NarrowPhaseDetectPair(Collider* first, Collider* second, const CollisionPair& collisionPair)
        : isCollision(false), first(first), second(second), collisionPair(collisionPair) {}
    };

    struct NarrowPhaseSystem
    {
    private:
        std::vector<CollisionPair> _collisionPairs;
        std::vector<NarrowPhaseDetectPair> _detectPairs;
    public:
        static constexpr int MAX_ITERATION = 36;
        NarrowPhaseSystem() = default;

        void Clear();
        void AddDetectPair(const NarrowPhaseDetectPair& pair);
        const std::vector<CollisionPair>& GetCollisionPairs();

    private:
        void CollisionDetection(NarrowPhaseDetectPair& pair);
        bool GJK(const Collider* first, const Collider* second, Simplex& simplex);
        CollisionInfo EPA(const Collider* first, const Collider* second, const Simplex& simplex);
    };
    Vector3 GetMinkowskiDiff(const Collider* first, const Collider* second, const Vector3& dir);
}
