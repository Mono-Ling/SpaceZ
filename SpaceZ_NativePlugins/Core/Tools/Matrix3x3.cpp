#include "Core/Tools/Matrix3x3.h"
#include <algorithm>

namespace Core
{
    const Matrix3x3 Matrix3x3::identity = Matrix3x3();

    Matrix3x3::Matrix3x3(const Vector3& v1, const Vector3& v2, const Vector3& v3)
    {
        m00 = v1.x;
        m10 = v1.y;
        m20 = v1.z;

        m01 = v2.x;
        m11 = v2.y;
        m21 = v2.z;

        m02 = v3.x;
        m12 = v3.y;
        m22 = v3.z;
    }
    Matrix3x3 Matrix3x3::Transpose() const
    {
        Matrix3x3 res;
        res.m00 = m00;
        res.m01 = m10;
        res.m02 = m20;
        res.m10 = m01;
        res.m11 = m11;
        res.m12 = m21;
        res.m20 = m02;
        res.m21 = m12;
        res.m22 = m22;
        return res;
    }
    void Matrix3x3::TransposeSelf()
    {
        std::swap(m01, m10);
        std::swap(m02, m20);
        std::swap(m12, m21);
    }

    Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2)
    {
        Matrix3x3 r;
        r.m00 = m1.m00 * m2.m00 + m1.m01 * m2.m10 + m1.m02 * m2.m20;
        r.m01 = m1.m00 * m2.m01 + m1.m01 * m2.m11 + m1.m02 * m2.m21;
        r.m02 = m1.m00 * m2.m02 + m1.m01 * m2.m12 + m1.m02 * m2.m22;

        r.m10 = m1.m10 * m2.m00 + m1.m11 * m2.m10 + m1.m12 * m2.m20;
        r.m11 = m1.m10 * m2.m01 + m1.m11 * m2.m11 + m1.m12 * m2.m21;
        r.m12 = m1.m10 * m2.m02 + m1.m11 * m2.m12 + m1.m12 * m2.m22;

        r.m20 = m1.m20 * m2.m00 + m1.m21 * m2.m10 + m1.m22 * m2.m20;
        r.m21 = m1.m20 * m2.m01 + m1.m21 * m2.m11 + m1.m22 * m2.m21;
        r.m22 = m1.m20 * m2.m02 + m1.m21 * m2.m12 + m1.m22 * m2.m22;
        return r;
    }
    Vector3 operator*(const Matrix3x3& m, const Vector3& v)
    {
        return Vector3{m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
                       m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
                       m.m20 * v.x + m.m21 * v.y + m.m22 * v.z};
    }
    Matrix3x3 QuaternionToMatrix3x3(const Quaternion& q)
    {
        auto nq = Normalize(q);
        float x = nq.x, y = nq.y, z = nq.z, w = nq.w;
        float xx = 2 * x * x, yy = 2 * y * y, zz = 2 * z * z;
        float xy = 2 * x * y, xz = 2 * x * z;
        float yz = 2 * y * z, wx = 2 * w * x, wy = 2 * w * y, wz = 2 * w * z;

        Matrix3x3 r;
        r.m00 = 1 - yy - zz;
        r.m01 = xy - wz;
        r.m02 = xz + wy;
        r.m10 = xy + wz;
        r.m11 = 1 - xx - zz;
        r.m12 = yz - wx;
        r.m20 = xz - wy;
        r.m21 = yz + wx;
        r.m22 = 1 - xx - yy;
        return r;
    }

    template <> std::string ToString<Matrix3x3>(const Matrix3x3& m)
    {
        Vector3 m0(m.m00, m.m01, m.m02);
        Vector3 m1(m.m10, m.m11, m.m12);
        Vector3 m2(m.m20, m.m21, m.m22);
        return "[" + ToString(m0) + "\n" + ToString(m1) + "\n" + ToString(m2) + "]";
    }
}
