#include"Core/BVH/BVHNode.h"
#include<cfloat>
namespace Core::SpaceZ
{
    BVHNode::BVHNode(BVHNodeObject obj) : obj(obj), count(1)
    {
        this->bound = Bound
        (
            obj.bound.center,
            obj.bound.extents * MULTIPLE
        );
    }
    bool BVHNode::IsLeaf() const
    {
        return this->obj != BVHNodeObject::null
            && this->left == nullptr
            && this->right == nullptr;
    }
    void BVHNode::Update()
    {
        if(this->IsLeaf())
            return;
        this->bound = this->left->bound + this->right->bound;
        this->count = this->left->count + this->right->count;
    }
    void BVHNode::ClearPtr()
    {
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }
    bool IsIntersect(const BVHNode& a, const BVHNode& b)
    {
        return IsIntersect(a.bound, b.bound);
    }
    float MergeCost(const BVHNode* node, const Bound& bound)
    {
        if(!node)
            return FLT_MAX;
        auto p = node->bound + bound;
        return (p.SurfaceArea() - node->bound.SurfaceArea()) * (node->count + 1);
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
