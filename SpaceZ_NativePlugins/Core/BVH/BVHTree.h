#pragma once
#include "Core/BVH/BVHNode.h"
#include "Core/BVH/BVHTreeObject.h"
#include "Core/Handle/Handle.h"
#include "Core/Collider/Collider.h"
#include "Core/Info/Collision/CollisionPair.h"
#include <unordered_map>
#include <vector>

namespace Core::SpaceZ
{
    class BVHTree
    {
    private:
        BVHNode* _root;
        std::unordered_map<ColliderHandle, BVHTreeObject> _objMap;

    public:
        BVHTree() : _objMap(std::unordered_map<ColliderHandle, BVHTreeObject>()), _root(nullptr) {}
        ~BVHTree();

        void Clear();
        BVHTreeObject* TryGetObject(const ColliderHandle& handle);
        bool Insert(const Collider& collider);
        bool Remove(const ColliderHandle& handle);
        void Update(const Collider& colider);
        std::vector<CollisionPair> GetCollisionPairs() const;

    private:
        void InsertNode(BVHNode* node);
        void Merge(BVHNode* before, BVHNode* after);
        BVHNode* GetBestBro(const Bound& after);
        void RemoveNode(BVHNode* node);
        void CollectCrossPairs(BVHNode* a, BVHNode* b, std::vector<CollisionPair>& ans) const;
    };
}
