#pragma once
#include "Core/Tools/Vector3.h"
#include"Core/Tools/Quaternion.h"
#include"Core/Tools/ToString.h"

namespace Core
{
    struct Matrix3x3
    {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;

        Matrix3x3()
        : m00(1), m01(0), m02(0),
          m10(0), m11(1), m12(0),
          m20(0), m21(0), m22(1) {}
        Matrix3x3(const Vector3& v1, const Vector3& v2, const Vector3& v3);

        Matrix3x3 Transpose() const;
        void TransposeSelf();

        static const Matrix3x3 identity;
    };
    Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2);
    Vector3 operator*(const Matrix3x3& m, const Vector3& v);
    Matrix3x3 QuaternionToMatrix3x3(const Quaternion& q);

    template<>
    std::string ToString<Matrix3x3>(const Matrix3x3& m);
}
