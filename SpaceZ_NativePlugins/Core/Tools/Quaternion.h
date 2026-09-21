#pragma once
#include"Core/Tools/Vector3.h"

namespace Core
{
    struct Quaternion
    {
        float x;
        float y;
        float z;
        float w;

        Quaternion() = default;
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        static const Quaternion identity;
    };
    bool operator==(const Quaternion& a, const Quaternion& b);
    bool operator!=(const Quaternion& a, const Quaternion& b);
    Quaternion operator*(const Quaternion& a, const Quaternion& b);
    Vector3 operator*(const Quaternion& q, const Vector3& v);
    float Magnitude(const Quaternion& q);
    Quaternion Normalize(const Quaternion& q);
}
