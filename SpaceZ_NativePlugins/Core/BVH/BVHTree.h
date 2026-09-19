#pragma once
#include "Core/BVH/BVHNode.h"
#include "Core/BVH/BVHTreeObject.h"
#include <unordered_map>

namespace Core::SpaceZ
{
    class BVHTree
    {
    private:
        BVHNode* root;
        std::unordered_map<ColliderHandle, BVHTreeObject> objMap;

    public:
        BVHTree() : objMap(std::unordered_map<ColliderHandle, BVHTreeObject>()), root(nullptr) {}

    private:
        void Merge(BVHNode* before, BVHNode* after);
        BVHNode* GetBestBro(const BVHNode* after);
        void DestroyNode(BVHNode* node);
    };
}
