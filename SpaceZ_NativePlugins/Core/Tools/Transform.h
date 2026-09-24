#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Quaternion.h"
#include"Core/Tools/Matrix4x4.h"

namespace Core
{
    struct Transform
    {
    private:
        mutable bool _dirty;

    public:
        Vector3 position;
        Vector3 scale;
        Quaternion rotation;

    private:
        mutable Matrix4x4 _localToParent;
        mutable Matrix4x4 _parentToLocal;

        Matrix4x4 _parentToWorld;
        Matrix4x4 _worldToParent;

        mutable Matrix4x4 _localToWorld;
        mutable Matrix4x4 _worldToLocal;

    public:
        Transform() : position(Vector3::zero), rotation(Quaternion::identity), scale(Vector3::one),
        _dirty(false),
        _parentToWorld(Matrix4x4::identity), _worldToParent(Matrix4x4::identity),
        _localToParent(TRS(Vector3::zero, Quaternion::identity, Vector3::one)),
        _parentToLocal(InverseTRS(Vector3::zero, Quaternion::identity, Vector3::one)),
        _localToWorld(_localToParent), _worldToLocal(_parentToLocal) {}

        Transform(Vector3 pos, Quaternion rot, Vector3 scale)
        : position(pos), rotation(rot), scale(scale),
        _dirty(false),
        _localToParent(TRS(pos, rot, scale)), _parentToLocal(InverseTRS(pos, rot, scale)),
        _localToWorld(_localToParent), _worldToLocal(_parentToLocal) {}

        void SetPosition(const Vector3& p);
        void SetRotation(const Quaternion& r);
        void SetScale(const Vector3& s);

        Matrix4x4 GetLocalToWorldMatrix() const;
        Matrix4x4 GetWorldToLocalMatrix() const;
        Vector3 GetPosition() const;
        void SetParentTransform(const Transform& parent);
        Vector3 PointToWorld(const Vector3& point) const;
        Vector3 DirToWorld(const Vector3& dir) const;
        Vector3 PointToLocal(const Vector3& point) const;
        Vector3 DirToLocal(const Vector3& dir) const;
        Vector3 NormalToLocal(const Vector3& normal) const;
    private:
        void UpdateLocalMatrix() const;
    };
}
