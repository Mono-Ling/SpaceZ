#include "Core/Tools/Matrix4x4.h"
#include "Core/Tools/Matrix3x3.h"

namespace Core
{
    const Matrix4x4 Matrix4x4::identity = Matrix4x4();

    Matrix4x4::Matrix4x4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4)
    {
        m00 = v1.x;
        m10 = v1.y;
        m20 = v1.z;
        m30 = v1.w;

        m01 = v2.x;
        m11 = v2.y;
        m21 = v2.z;
        m31 = v2.w;

        m02 = v3.x;
        m12 = v3.y;
        m22 = v3.z;
        m32 = v3.w;

        m03 = v4.x;
        m13 = v4.y;
        m23 = v4.z;
        m33 = v4.w;
    }
    Matrix4x4::Matrix4x4(const Matrix3x3& m3x3)
    {
        m00 = m3x3.m00; m01 = m3x3.m01; m02 = m3x3.m02; m03 = 0;
        m10 = m3x3.m10; m11 = m3x3.m11; m12 = m3x3.m12; m13 = 0;
        m20 = m3x3.m20; m21 = m3x3.m21; m22 = m3x3.m22; m23 = 0;
        m30 = 0;     m31 = 0;     m32 = 0;     m33 = 1;
    }

    Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
    {
        Matrix4x4 r;
        r.m00 = m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20 + m1.m03 * m2.m30;
        r.m01 = m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21 + m1.m03 * m2.m31;
        r.m02 = m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22 + m1.m03 * m2.m32;
        r.m03 = m1.m00 * m2.m03 + m1.m01 * m2.m13 + m1.m02 * m2.m23 + m1.m03 * m2.m33;

        r.m10 = m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20 + m1.m13 * m2.m30;
        r.m11 = m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21 + m1.m13 * m2.m31;
        r.m12 = m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22 + m1.m13 * m2.m32;
        r.m13 = m1.m10 * m2.m03 + m1.m11 * m2.m13 + m1.m12 * m2.m23 + m1.m13 * m2.m33;

        r.m20 = m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20 + m1.m23 * m2.m30;
        r.m21 = m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21 + m1.m23 * m2.m31;
        r.m22 = m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22 + m1.m23 * m2.m32;
        r.m23 = m1.m20 * m2.m03 + m1.m21 * m2.m13 + m1.m22 * m2.m23 + m1.m23 * m2.m33;

        r.m30 = m1.m30 * m2.m00 + m1.m31 * m2.m10 + m1.m32 * m2.m20 + m1.m33 * m2.m30;
        r.m31 = m1.m30 * m2.m01 + m1.m31 * m2.m11 + m1.m32 * m2.m21 + m1.m33 * m2.m31;
        r.m32 = m1.m30 * m2.m02 + m1.m31 * m2.m12 + m1.m32 * m2.m22 + m1.m33 * m2.m32;
        r.m33 = m1.m30 * m2.m03 + m1.m31 * m2.m13 + m1.m32 * m2.m23 + m1.m33 * m2.m33;
        return r;
    }
    Vector4 operator*(const Matrix4x4& m, const Vector4& v)
    {
        return Vector4{m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w,
                       m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w,
                       m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w,
                       m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w};
    }
    Matrix4x4 TRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
    {
        auto r = Normalize(rot);
        Matrix4x4 S;
        S.m00 = scale.x; S.m11 = scale.y; S.m22 = scale.z; S.m33 = 1.0f;

        Matrix3x3 R = QuaternionToMatrix3x3(r);

        Matrix4x4 T;
        T.m00 = 1; T.m11 = 1; T.m22 = 1; T.m33 = 1;
        T.m03 = pos.x;
        T.m13 = pos.y;
        T.m23 = pos.z;

        return T * R * S;
    }
    Matrix4x4 InverseTRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
    {
        auto r = Normalize(rot);
        Matrix4x4 S;
        S.m00 = 1.0f / scale.x;
        S.m11 = 1.0f / scale.y;
        S.m22 = 1.0f / scale.z;
        S.m33 = 1.0f;

        Matrix3x3 R = QuaternionToMatrix3x3(r);
        R.TransposeSelf();

        Matrix4x4 T;
        T.m00 = 1; T.m11 = 1; T.m22 = 1; T.m33 = 1;
        T.m03 = -pos.x;
        T.m13 = -pos.y;
        T.m23 = -pos.z;

        return S * R * T;
    }

    template <> std::string ToString<Matrix4x4>(const Matrix4x4& m)
    {
        Vector4 m0(m.m00, m.m01, m.m02, m.m03);
        Vector4 m1(m.m10, m.m11, m.m12, m.m13);
        Vector4 m2(m.m20, m.m21, m.m22, m.m23);
        Vector4 m3(m.m30, m.m31, m.m32, m.m33);
        return "[" + ToString(m0) + "\n" + ToString(m1) + "\n" + ToString(m2) + "\n" +
               ToString(m3) + "]";
    }
}
