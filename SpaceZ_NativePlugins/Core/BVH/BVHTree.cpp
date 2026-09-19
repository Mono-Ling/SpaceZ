#include"Core/BVH/BVHTree.h"
#include"Core/BVH/BVHNodeObject.h"
#include<stack>
#include<cfloat>
using namespace std;
namespace Core::SpaceZ
{
    void BVHTree::Merge(BVHNode* before, BVHNode* after)
    {
        if(!before || !after)
            return;
        auto parent = before->parent;
        Bound parentBound = before->bound + after->bound;
        BVHNode* newParent = new BVHNode(parentBound);

        if(!parent)
            root = newParent;
        else if(before == parent->left)
            parent->left = newParent;
        else
            parent->right = newParent;

        newParent->left = before;
        newParent->right = after;
        before->parent = newParent;
        after->parent = newParent;
        newParent->parent = parent;

        auto curr = newParent->parent;
        while(curr)
        {
            curr->UpdateBound();
            curr = curr->parent;
        }
    }
    void BVHTree::DestroyNode(BVHNode* node)
    {
        if(!node)
            return;
        auto bro = GetBro(node);
        if(!bro)
        {
            delete node;
            root = nullptr;
            return;
        }
        auto parent = node->parent;
        auto grandparent = parent->parent;
        if(!grandparent)
        {
            root = bro;
            delete parent;
            delete node;
            return;
        }
        if(parent == grandparent->left)
            grandparent->left = bro;
        else
            grandparent->right = bro;
        bro->parent = grandparent;
        delete parent;
        delete node;

        auto curr = grandparent;
        while(curr)
        {
            curr->UpdateBound();
            curr = curr->parent;
        }
    }
    BVHNode* BVHTree::GetBestBro(const BVHNode* after)
    {
        if(!root || !after)
            return nullptr;
        stack<BVHNode*> st;
        st.push(root);
        BVHNode* bestBro = nullptr;
        float minSA = FLT_MAX;
        while (!st.empty())
        {
            auto curr = st.top();
            st.pop();
            if(!curr)
                continue;
            float currSA = MergeSurfaceArea(*curr,*after);

            if(currSA > minSA)
                continue;
            if(curr->IsLeaf())
            {
                bestBro = curr;
                minSA = currSA;
            }
            else
            {
                st.push(curr->left);
                st.push(curr->right);
            }
        }
        return bestBro;
    }
}
