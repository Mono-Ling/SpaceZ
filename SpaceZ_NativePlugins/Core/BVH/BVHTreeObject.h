#pragma once
#include "Core/SpaceGeomBody/Bound.h"
#include "Core/BVH/BVHNode.h"

namespace Core::SpaceZ
{
    struct BVHTreeObject
    {
        BVHNode* currentNode;
        Bound currentBound;
        BVHTreeObject(BVHNode* node, Bound bound) : currentNode(node), currentBound(bound) {}
    };
}
