#include "Core/Tools/Matrix4x4.h"

namespace Core
{
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

    Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
    {
        Vector4 a0(m1.m00, m1.m01, m1.m02, m1.m03);
        Vector4 a1(m1.m10, m1.m11, m1.m12, m1.m13);
        Vector4 a2(m1.m20, m1.m21, m1.m22, m1.m23);
        Vector4 a3(m1.m30, m1.m31, m1.m32, m1.m33);

        Vector4 b0(m2.m00, m2.m10, m2.m20, m2.m30);
        Vector4 b1(m2.m01, m2.m11, m2.m21, m2.m31);
        Vector4 b2(m2.m02, m2.m12, m2.m22, m2.m32);
        Vector4 b3(m2.m03, m2.m13, m2.m23, m2.m33);

        Vector4 v1{Dot(a0, b0), Dot(a1, b0), Dot(a2, b0), Dot(a3, b0)};
        Vector4 v2{Dot(a0, b1), Dot(a1, b1), Dot(a2, b1), Dot(a3, b1)};
        Vector4 v3{Dot(a0, b2), Dot(a1, b2), Dot(a2, b2), Dot(a3, b2)};
        Vector4 v4{Dot(a0, b3), Dot(a1, b3), Dot(a2, b3), Dot(a3, b3)};

        return Matrix4x4(v1, v2, v3, v4);
    }
    Vector4 operator*(const Matrix4x4& m, const Vector4& v)
    {
        Vector4 m0(m.m00, m.m01, m.m02, m.m03);
        Vector4 m1(m.m10, m.m11, m.m12, m.m13);
        Vector4 m2(m.m20, m.m21, m.m22, m.m23);
        Vector4 m3(m.m30, m.m31, m.m32, m.m33);

        return Vector4{Dot(m0, v),Dot(m1, v),Dot(m2, v),Dot(m3, v)};
    }
    Matrix4x4 TRS(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
    {
        auto r = Normalize(rot);
        Vector4 v1
        {
            (1 - 2 * (r.y * r.y + r.z * r.z)) * scale.x,
            2 * (r.x * r.y + r.w * r.z) * scale.x,
            2 * (r.x * r.z - r.w * r.y) * scale.x,
            0
        };
        Vector4 v2
        {
            2 * (r.x * r.y - r.w * r.z) * scale.y,
            (1 - 2 * (r.x * r.x + r.z * r.z)) * scale.y,
            2 * (r.y * r.z + r.w * r.x) * scale.y,
            0
        };
        Vector4 v3
        {
            2 * (r.x * r.z + r.w * r.y) * scale.z,
            2 * (r.y * r.z - r.w * r.x) * scale.z,
            (1 - 2 * (r.x * r.x + r.y * r.y)) * scale.z,
            0
        };
        Vector4 v4(pos.x, pos.y, pos.z, 1);
        return Matrix4x4{v1, v2, v3, v4};
    }

    template<>
    std::string ToString<Matrix4x4>(const Matrix4x4& m)
    {
        Vector4 m0(m.m00, m.m01, m.m02, m.m03);
        Vector4 m1(m.m10, m.m11, m.m12, m.m13);
        Vector4 m2(m.m20, m.m21, m.m22, m.m23);
        Vector4 m3(m.m30, m.m31, m.m32, m.m33);
        return "["
            + ToString(m0) + "\n"
            + ToString(m1) + "\n"
            + ToString(m2) + "\n"
            + ToString(m3) + "]";
    }
}
