#pragma once
#include"Core/Tools/Vector4.h"
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Quaternion.h"
#include"Core/Tools/ToString.h"

namespace Core
{
    struct Matrix4x4
    {
        float m00 = 0;
        float m01 = 0;
        float m02 = 0;
        float m03 = 0;

        float m10 = 0;
        float m11 = 0;
        float m12 = 0;
        float m13 = 0;

        float m20 = 0;
        float m21 = 0;
        float m22 = 0;
        float m23 = 0;

        float m30 = 0;
        float m31 = 0;
        float m32 = 0;
        float m33 = 0;

        Matrix4x4() = default;
        Matrix4x4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4);
    };
    Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
    Vector4 operator*(const Matrix4x4& m, const Vector4& v);
    Matrix4x4 TRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

    template<>
    std::string ToString<Matrix4x4>(const Matrix4x4& m);
}
