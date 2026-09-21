#include "Core/BVH/BVHTree.h"
#include "Core/BVH/BVHNodeObject.h"
#include "Core/SpaceGeomBody/Bound.h"
#include <stack>
#include <utility>
#include <cfloat>
using namespace std;
namespace Core::SpaceZ
{
    BVHTree::~BVHTree()
    {
        Clear();
    }
    void BVHTree::Clear()
    {
        if (!_root)
            return;
        std::stack<std::pair<BVHNode*, bool>> st;
        st.push({_root, false});
        while (!st.empty())
        {
            auto [node, visited] = st.top();
            st.pop();
            if (!visited)
            {
                st.push({node, true});
                if (node->right)
                    st.push({node->right, false});
                if (node->left)
                    st.push({node->left, false});
            }
            else
                delete node;
        }
        _root = nullptr;
        _objMap.clear();
    }
    BVHTreeObject* BVHTree::TryGetObject(const ColliderHandle& handle)
    {
        auto it = _objMap.find(handle);
        if (it == _objMap.end())
            return nullptr;
        return &it->second;
    }
    bool BVHTree::Insert(const Collider& collider)
    {
        auto handle = collider.GetColliderHandle();
        if (handle == ColliderHandle::null || TryGetObject(handle))
            return false;
        BVHNodeObject nodeObj(collider);
        auto node = new BVHNode(nodeObj);
        BVHTreeObject treeObj(node, collider.GetBound());

        InsertNode(node);
        _objMap.insert({handle, treeObj});
        return true;
    }
    bool BVHTree::Remove(const ColliderHandle& handle)
    {
        auto objPtr = TryGetObject(handle);
        if (!objPtr)
            return false;
        auto obj = *objPtr;
        RemoveNode(obj.currentNode);
        delete obj.currentNode;
        _objMap.erase(handle);
        return true;
    }
    void BVHTree::Update(const Collider& colider)
    {
        auto handle = colider.GetColliderHandle();
        auto objPtr = TryGetObject(handle);
        if (!objPtr)
            return;
        auto& obj = *objPtr;

        auto bound = colider.GetBound();
        if (bound == obj.currentBound)
            return;
        if (IsContains(obj.currentNode->bound, bound))
        {
            obj.currentNode->obj.bound = bound;
            obj.currentBound = bound;
            return;
        }
        auto node = obj.currentNode;
        RemoveNode(node);
        node->ClearPtr();
        node->bound = Bound(bound.center, bound.extents * BVHNode::MULTIPLE);
        node->obj.bound = bound;
        obj.currentBound = bound;
        InsertNode(node);
    }
    vector<CollisionPair> BVHTree::GetCollisionPairs() const
    {
        vector<CollisionPair> ans;
        if(!_root || _root->IsLeaf())
            return ans;
        stack<BVHNode*> st;
        st.push(_root);
        while (!st.empty())
        {
            auto node = st.top();
            st.pop();
            if (!node || node->IsLeaf())
                continue;
            CollectCrossPairs(node->left, node->right, ans);
            st.push(node->left);
            st.push(node->right);
        }
        return ans;
    }
    void BVHTree::CollectCrossPairs(BVHNode* left, BVHNode* right, std::vector<CollisionPair>& ans) const
    {
        if(!left || !right || !IsIntersect(*left, *right))
            return;
        stack<pair<BVHNode*,BVHNode*>> st;
        st.push({left, right});
        while (!st.empty())
        {
            auto curr = st.top();
            st.pop();
            if(!curr.first || !curr.second)
                continue;
            auto a = curr.first;
            auto b = curr.second;
            if(!IsIntersect(*a, *b))
                continue;

            if(a->IsLeaf() && b->IsLeaf())
            {
                if(IsIntersect(a->obj, b->obj))
                    ans.push_back(GetCollisionPair(a->obj, b->obj));
            }
            else if(a->IsLeaf() || b->IsLeaf())
            {
                if(a->IsLeaf())
                {
                    st.push({curr.first, b->left});
                    st.push({curr.first, b->right});
                }
                else
                {
                    st.push({a->left, curr.second});
                    st.push({a->right, curr.second});
                }
            }
            else
            {
                st.push({a->left,  b->left});
                st.push({a->left,  b->right});
                st.push({a->right, b->left});
                st.push({a->right, b->right});
            }
        }
    }
    void BVHTree::InsertNode(BVHNode* node)
    {
        if (!node)
            return;
        if (!_root)
        {
            _root = node;
            return;
        }
        auto bestBro = GetBestBro(node->bound);
        Merge(bestBro, node);
    }
    void BVHTree::Merge(BVHNode* before, BVHNode* after)
    {
        if (!before || !after)
            return;
        auto parent = before->parent;
        Bound parentBound = before->bound + after->bound;
        BVHNode* newParent = new BVHNode(parentBound);

        if (!parent)
            _root = newParent;
        else if (before == parent->left)
            parent->left = newParent;
        else
            parent->right = newParent;

        newParent->left = before;
        newParent->right = after;
        before->parent = newParent;
        after->parent = newParent;
        newParent->parent = parent;

        auto curr = newParent;
        while (curr)
        {
            curr->Update();
            curr = curr->parent;
        }
    }
    void BVHTree::RemoveNode(BVHNode* node)
    {
        if (!node)
            return;
        auto bro = GetBro(node);
        if (!bro)
        {
            _root = nullptr;
            return;
        }
        auto parent = node->parent;
        auto grandparent = parent->parent;
        if (!grandparent)
        {
            _root = bro;
            bro->parent = nullptr;
            delete parent;
            return;
        }
        if (parent == grandparent->left)
            grandparent->left = bro;
        else
            grandparent->right = bro;
        bro->parent = grandparent;
        delete parent;

        auto curr = grandparent;
        while (curr)
        {
            curr->Update();
            curr = curr->parent;
        }
    }
    BVHNode* BVHTree::GetBestBro(const Bound& after)
    {
        auto curr = _root;
        while (curr && !curr->IsLeaf())
        {
            float currCost = (curr->bound + after).SurfaceArea();
            auto inheritanceCost = currCost - curr->bound.SurfaceArea();
            auto costL = MergeCost(curr->left, after) + inheritanceCost;
            auto costR = MergeCost(curr->right, after) + inheritanceCost;
            if (costL > currCost && costR > currCost)
                break;
            curr = costL > costR ? curr->right : curr->left;
        }
        return curr;
    }
}
