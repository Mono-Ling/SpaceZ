#include"Core/Collider/Collider.h"

namespace Core::SpaceZ
{
    ColliderHandle Collider::GetColliderHandle() const
    {
        return this->_handle;
    }
    SpaceObjectHandle Collider::GetSpaceObjectHandle() const
    {
        return this->_objHandle;
    }
    const Bound& Collider::GetBound() const
    {
        return this->_bound;
    }
    void Collider::UpdateParentTransform(const Transform& parent)
    {
        _transform.SetParentTransform(parent);
        UpdateBound();
    }

    void Collider::UpdateLocalTransform(const TransformSynMsg& syn)
    {
        _transform.SetPosition(syn.position);
        _transform.SetRotation(syn.rotation);
        _transform.SetScale(syn.scale);

        UpdateBound();
    }
    void Collider::UpdateBound()
    {
        Vector3 maxP{Support(Vector3::right).x, Support(Vector3::up).y, Support(Vector3::forward).z};
        Vector3 minP{Support(-Vector3::right).x, Support(-Vector3::up).y, Support(-Vector3::forward).z};
        _bound = CreateBound(minP, maxP);
    }
}
