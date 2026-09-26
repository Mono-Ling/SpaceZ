#include"Core/System/SpaceSystem.h"
#include"Core/Collider/BoxCollider.h"
#include"Core/Collider/SphereCollider.h"
#include"Core/Collider/CapsuleCollider.h"

using namespace std;
namespace Core::SpaceZ
{
    SpaceSystem::~SpaceSystem()
    {
        vector<SpaceObjectHandle> objs;
        _spaceObjects.ForEachUsed([&](Handle<SpaceObject> handle, SpaceObject* ptr)
        {
            objs.push_back(handle);
        });
        for(auto& obj : objs)
            DestroySpaceObject(obj);

        _colliders.ForEachUsed([](Handle<Collider> handle, Collider* ptr)
        {
            // 清理静态碰撞体
            delete ptr;
        });
    }

    ColliderHandle SpaceSystem::CreateCollider(ColliderType type, const SpaceObjectHandle& spaceObj)
    {
        ColliderHandle handle = ColliderHandle::null;
        Collider* collider = nullptr;
        handle = _colliders.CreateHandle();
        switch (type)
        {
        case ColliderType::Box:
            collider = new BoxCollider(handle, spaceObj);
            break;
        case ColliderType::Sphere:
            collider = new SphereCollider(handle, spaceObj);
            break;
        case ColliderType::Capsule:
            collider = new CapsuleCollider(handle, spaceObj);
            break;

        default:
            return ColliderHandle::null;
        }
        _colliders[handle] = collider;
        SpaceObject* obj = nullptr;

        // 无SpaceObject归属碰撞体允许存在，记为null占位作为静态碰撞体
        if(_spaceObjects.TryGet(spaceObj, obj) && obj)
            obj->AddCollider(handle);
        return handle;
    }
    SpaceObjectHandle SpaceSystem::CreateSpaceObject()
    {
        auto handle = _spaceObjects.CreateHandle();
        _spaceObjects[handle] = new SpaceObject();
        return handle;
    }
    bool SpaceSystem::DestroyCollider(const ColliderHandle& handle)
    {
        Collider* collider = nullptr;
        if(!_colliders.TryGet(handle, collider))
            return false;

        _breadthPhaseSystem.Remove(handle);
        if(!collider)
            return _colliders.TryRemove(handle);

        SpaceObject* obj = nullptr;
        if(_spaceObjects.TryGet(collider->GetSpaceObjectHandle(), obj))
            if(obj) obj->RemoveCollider(handle);
        delete collider;

        return _colliders.TryRemove(handle);
    }
    bool SpaceSystem::DestroySpaceObject(const SpaceObjectHandle& handle)
    {
        SpaceObject* obj = nullptr;
        if(!_spaceObjects.TryGet(handle, obj))
            return false;
        if(!obj)
            return _spaceObjects.TryRemove(handle);
        auto colliderSet = obj->GetColliders();
        for(auto& collider : colliderSet)
            DestroyCollider(collider);
        delete obj;
        return _spaceObjects.TryRemove(handle);
    }
    void SpaceSystem::UpdateColliderTransform(const ColliderHandle& handle, const TransformSynMsg& msg)
    {
        Collider* collider = nullptr;
        if(!_colliders.TryGet(handle, collider) || !collider)
            return;
        collider->UpdateLocalTransform(msg);
        _dirtyColliderSet.insert(handle);

    }
    void SpaceSystem::UpdateSpaceObjectTransform(const SpaceObjectHandle& handle, const TransformSynMsg& msg)
    {
        SpaceObject* obj = nullptr;
        if(!_spaceObjects.TryGet(handle, obj) || !obj)
            return;
        obj->UpdateTransform(msg);

        auto colliderSet = obj->GetColliders();
        Collider* colliderPtr = nullptr;
        for(auto& collider : colliderSet)
            if(_colliders.TryGet(collider, colliderPtr) && colliderPtr)
            {
                colliderPtr->UpdateParentTransform(obj->GetTransform());
                _dirtyColliderSet.insert(collider);
            }
    }
    const vector<CollisionPair>& SpaceSystem::GetCollisionPairs()
    {
        auto pairs = _breadthPhaseSystem.GetCollisionPairs();
        _narrowPhaseSystem.Clear();
        for(auto& p : pairs)
        {
            Collider* first = nullptr;
            Collider* second = nullptr;
            if(!_colliders.TryGet(p.first.colliderHandle, first) || !first)
                continue;
            if(!_colliders.TryGet(p.second.colliderHandle, second) || !second)
                continue;
            _narrowPhaseSystem.AddDetectPair(NarrowPhaseDetectPair(first, second, p));
        }
        return _narrowPhaseSystem.GetCollisionPairs();
    }
    void SpaceSystem::SynBreadthPhaseSystem()
    {
        if(_dirtyColliderSet.empty())
            return;
        Collider* collider = nullptr;
        for(auto& handle : _dirtyColliderSet)
        {
            if(_colliders.TryGet(handle, collider) && collider)
                if(_breadthPhaseSystem.TryGetObject(handle))
                    _breadthPhaseSystem.Update(*collider);
                else
                    _breadthPhaseSystem.Insert(*collider);
        }
        _dirtyColliderSet.clear();
    }
}
