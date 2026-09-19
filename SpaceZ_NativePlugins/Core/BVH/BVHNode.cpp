#include"Core/BVH/BVHNode.h"

namespace Core::SpaceZ
{
    BVHNode::BVHNode(const Bound& bound, BVHNodeObject obj) : obj(obj)
    {
        this->bound = Bound
        (
            bound.center,
            bound.extents * MULTIPLE
        );
    }
    bool BVHNode::IsLeaf() const
    {
        return this->obj != BVHNodeObject::null
            && this->left == nullptr
            && this->right == nullptr;
    }
    void BVHNode::UpdateBound()
    {
        if(this->IsLeaf())
            return;
        this->bound = this->left->bound + this->right->bound;
    }
    bool IsIntersect(const BVHNode& a, const BVHNode& b)
    {
        return IsIntersect(a.bound, b.bound);
    }
    float MergeSurfaceArea(const BVHNode& a, const BVHNode& b)
    {
        auto bound = a.bound + b.bound;
        return bound.SurfaceArea();
    }
    BVHNode* GetBro(BVHNode*& a)
    {
        if(!a || !a->parent)
            return nullptr;
        auto parent = a->parent;
        if(a == parent->left)
            return parent->right;
        else
            return parent->left;
    }
}
