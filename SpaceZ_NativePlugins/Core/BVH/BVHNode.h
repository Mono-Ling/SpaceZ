#pragma once
#include"Core/SpaceGeomBody/Bound.h"
#include"Core/BVH/BVHNodeObject.h"

namespace Core::SpaceZ
{
    class BVHNode
    {
    public:
        static constexpr float MULTIPLE = 1.5f;
        BVHNode* left = nullptr;
        BVHNode* right = nullptr;
        BVHNode* parent = nullptr;
        Bound bound;
        BVHNodeObject obj = BVHNodeObject::null;

        BVHNode(const Bound& bound) : bound(bound) {}
        BVHNode(const Bound& bound, BVHNodeObject obj);

        bool IsLeaf() const;
        void UpdateBound();
    };
    bool IsIntersect(const BVHNode& a, const BVHNode& b);
    float MergeSurfaceArea(const BVHNode& a, const BVHNode& b);
    BVHNode* GetBro(BVHNode*& a);
}
