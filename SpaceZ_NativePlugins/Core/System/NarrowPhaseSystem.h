#pragma once
#include"Core/Info/Collision/CollisionPair.h"
#include"Core/Info/Collision/CollisionInfo.h"
#include"Core/SpaceGeomBody/Simplex.h"
#include"Core/SpaceGeomBody/Convex/ConvexTransform.h"
#include<vector>

namespace Core::SpaceZ
{
    struct NarrowPhaseDetectPair
    {
        bool isCollision;
        ConvexTransform first;
        ConvexTransform second;
        CollisionPair collisionPair;

        NarrowPhaseDetectPair(ConvexTransform first, ConvexTransform second, const CollisionPair& collisionPair)
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
        bool GJK(const ConvexTransform& first, const ConvexTransform& second, Simplex& simplex);
        CollisionInfo EPA(const ConvexTransform& first, const ConvexTransform& second, Simplex& simplex);
    };
    Vector3 GetMinkowskiDiff(const ConvexTransform& first, const ConvexTransform& second, const Vector3& dir);
    void CompleteSimplex(const ConvexTransform& first, const ConvexTransform& second, Simplex& simplex);
}
