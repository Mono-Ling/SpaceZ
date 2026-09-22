#pragma once
#include"Core/Tools/Vector4.h"
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Matrix3x3.h"
#include"Core/Tools/Quaternion.h"
#include"Core/Tools/ToString.h"

namespace Core
{
    struct Matrix4x4
    {
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
        float m30, m31, m32, m33;

        Matrix4x4()
        : m00(1), m01(0), m02(0), m03(0),
          m10(0), m11(1), m12(0), m13(0),
          m20(0), m21(0), m22(1), m23(0),
          m30(0), m31(0), m32(0), m33(1) {}
        Matrix4x4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4);
        Matrix4x4(const Matrix3x3& m);

        static const Matrix4x4 identity;
    };
    Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
    Vector4 operator*(const Matrix4x4& m, const Vector4& v);
    Matrix4x4 TRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale);
    Matrix4x4 InverseTRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

    template<>
    std::string ToString<Matrix4x4>(const Matrix4x4& m);
}
