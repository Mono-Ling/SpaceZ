#pragma once
#include"Core/Handle/HandleInstance.h"
#include"Core/Tools/Transform.h"
#include"Core/Info/SynMsg/TransformSynMsg.h"
#include<unordered_set>

namespace Core::SpaceZ
{
    class SpaceObject
    {
    private:
        Transform _transform;
        std::unordered_set<ColliderHandle> _colliders;

    public:
        SpaceObject() = default;

        inline void UpdateTransform(const TransformSynMsg& syn)
        {
            _transform.SetPosition(syn.position);
            _transform.SetRotation(syn.rotation);
            _transform.SetScale(syn.scale);
        }
        inline bool AddCollider(const ColliderHandle collider)
        {
            if(_colliders.count(collider))
                return false;
            _colliders.insert(collider);
            return true;
        }
        inline bool RemoveCollider(const ColliderHandle collider)
        {
            if(!_colliders.count(collider))
                return false;
            _colliders.erase(collider);
            return true;
        }
        inline const std::unordered_set<ColliderHandle>& GetColliders() const
        {
            return _colliders;
        }
        inline const Transform& GetTransform() const
        {
            return _transform;
        }
    };
}
