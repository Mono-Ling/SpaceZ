#include"Core/Tools/Transform.h"

namespace Core
{
    void Transform::SetPosition(const Vector3& p)
    {
        position = p;
        _dirty = true;
    }
    void Transform::SetRotation(const Quaternion& r)
    {
        rotation = r;
        _dirty = true;
    }
    void Transform::SetScale(const Vector3& s)
    {
        scale = s;
        _dirty = true;
    }

    Matrix4x4 Transform::GetLocalToWorldMatrix() const
    {
        UpdateLocalMatrix();
        return _localToWorld;
    }
    Matrix4x4 Transform::GetWorldToLocalMatrix() const
    {
        UpdateLocalMatrix();
        return _worldToLocal;
    }
    Vector3 Transform::GetPosition() const
    {
        Vector4 v(position, 1);
        v = _parentToWorld * v;
        return Vector3(v.x, v.y, v.z);
    }
    void Transform::SetParentTransform(const Transform& parent)
    {
        UpdateLocalMatrix();
        _parentToWorld = parent.GetLocalToWorldMatrix();
        _localToWorld = _parentToWorld * _localToParent;

        _worldToParent = parent.GetWorldToLocalMatrix();
        _worldToLocal = _parentToLocal * _worldToParent;
    }
    Vector3 Transform::PointToWorld(const Vector3& point) const
    {
        UpdateLocalMatrix();
        Vector4 v(point, 1);
        v = _localToWorld * v;
        return Vector3(v.x, v.y , v.z);
    }
    Vector3 Transform::DirToWorld(const Vector3& dir) const
    {
        UpdateLocalMatrix();
        auto v = _localToWorld * dir;
        return Vector3(v.x, v.y , v.z);
    }
    Vector3 Transform::PointToLocal(const Vector3& point) const
    {
        UpdateLocalMatrix();
        Vector4 v(point, 1);
        v = _worldToLocal * v;
        return Vector3(v.x, v.y , v.z);
    }
    Vector3 Transform::DirToLocal(const Vector3& dir) const
    {
        UpdateLocalMatrix();
        auto v = _worldToLocal * dir;
        return Vector3(v.x, v.y , v.z);
    }
    Vector3 Transform::NormalToLocal(const Vector3& normal) const
    {
        UpdateLocalMatrix();
        return _localToWorld.ToMatrix3x3().Transpose() * normal;
    }
    void Transform::UpdateLocalMatrix() const
    {
        if(!_dirty)
            return;
        _localToParent = TRS(position, rotation, scale);
        _localToWorld = _parentToWorld * _localToParent;

        _parentToLocal = InverseTRS(position, rotation, scale);
        _worldToLocal = _parentToLocal * _worldToParent;
        _dirty = false;
    }
}
