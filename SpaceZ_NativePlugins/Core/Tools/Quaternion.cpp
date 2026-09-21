#include "Core/Tools/Quaternion.h"
#include "Core/Tools/Math.h"

using namespace Core::Math;
namespace Core
{
    const Quaternion Quaternion::identity = Quaternion(0, 0, 0, 1);

    Quaternion operator*(const Quaternion& a, const Quaternion& b)
    {
        return Quaternion{a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                          a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
                          a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
                          a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z};
    }
    Vector3 operator*(const Quaternion& q, const Vector3& v)
    {
        auto r = Normalize(q);
        Vector3 qv{q.x, q.y, q.z};
        Vector3 uv = Cross(qv, v);
        Vector3 uuv = Cross(qv, uv);
        return v + 2.0f * (q.w * uv + uuv);
    }
    bool operator==(const Quaternion& a, const Quaternion& b)
    {
        return a.x == b.x
            && a.y == b.y
            && a.z == b.z
            && a.w == b.w;
    }
    bool operator!=(const Quaternion& a, const Quaternion& b);
    float Magnitude(const Quaternion& q)
    {
        return Sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    }
    Quaternion Normalize(const Quaternion& q)
    {
        float len = Magnitude(q);
        if (len < Epsilon)
            return Quaternion::identity;
        float invLen = 1.0f / len;
        return Quaternion{q.x * invLen, q.y * invLen, q.z * invLen, q.w * invLen};
    }
}
