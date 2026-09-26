#pragma once
#include"Core/Handle/HandleInstance.h"
#include"Core/Handle/HandleLifeCycle.h"
#include"Core/Collider/Collider.h"
#include"Core/SpaceObject/SpaceObject.h"
#include"Core/BVH/BVHTree.h"
#include"Core/System/NarrowPhaseSystem.h"

#include"Core/Info/SynMsg/CapsuleSynMsg.h"
#include"Core/Info/SynMsg/TransformSynMsg.h"

#include<unordered_set>
#include<utility>

namespace Core::SpaceZ
{
    enum class ColliderType
    {
        /// @brief 盒
        Box,

        /// @brief 球体
        Sphere,

        /// @brief 胶囊体
        Capsule,
    };
    class SpaceSystem
    {
    private:
        std::unordered_set<ColliderHandle> _dirtyColliderSet;
        HandleLifeCycle<SpaceObject> _spaceObjects;
        HandleLifeCycle<Collider> _colliders;

        NarrowPhaseSystem _narrowPhaseSystem;
        BVHTree _breadthPhaseSystem;

    public:
        SpaceSystem() = delete;
        ~SpaceSystem();

        ColliderHandle CreateCollider(ColliderType type, const SpaceObjectHandle& spaceObj);
        SpaceObjectHandle CreateSpaceObject();
        bool DestroyCollider(const ColliderHandle& handle);
        bool DestroySpaceObject(const SpaceObjectHandle& handle);

        void UpdateColliderTransform(const ColliderHandle& handle, const TransformSynMsg& msg);
        void UpdateSpaceObjectTransform(const SpaceObjectHandle& handle, const TransformSynMsg& msg);

        void UpdateBoxCollider(const std::pair<ColliderHandle,Vector3>& synMsg);
        void UpdateSphereCollider(const std::pair<ColliderHandle,float>& synMsg);
        void UpdateCapsuleCollider(const std::pair<ColliderHandle,CapsuleSynMsg>& synMsg);

        const std::vector<CollisionPair>& GetCollisionPairs();
        void SynBreadthPhaseSystem();

        private:
        template<typename T>
        T* TryGetCollider(const ColliderHandle& handle);
    };
}
