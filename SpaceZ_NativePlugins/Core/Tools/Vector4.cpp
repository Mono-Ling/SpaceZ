#include "Core/Tools/Vector4.h"
#include "Core/Tools/Math.h"

using namespace Core::Math;
namespace Core
{
    const Vector4 Vector4::one = Vector4(1, 1, 1, 1);
    const Vector4 Vector4::zero = Vector4(0, 0, 0, 0);
    Vector4 Vector4::normalized() const
    {
        return Normalize(*this);
    }
    float Vector4::length() const
    {
        return Length(*this);
    }
    void Vector4::operator=(const Vector3& vec3)
    {
        this->x = vec3.x;
        this->y = vec3.y;
        this->z = vec3.z;
        this->w = 0;
    }
    Vector4 Vector4::operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    Vector4 operator+(const Vector4& a, const Vector4& b)
    {
        return Vector4{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    }
    Vector4 operator-(const Vector4& a, const Vector4& b)
    {
        return Vector4{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    }
    Vector4 operator*(const Vector4& v, float scalar)
    {
        return Vector4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
    }
    Vector4 operator*(float scalar, const Vector4& v)
    {
        return v * scalar;
    }
    Vector4 operator/(const Vector4& v, float scalar)
    {
        return Vector4(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
    }
    bool operator==(const Vector4& a, const Vector4& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }
    bool operator!=(const Vector4& a, const Vector4& b)
    {
        return !(a == b);
    }

    float Length(const Vector4& v)
    {
        return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    }
    Vector4 Normalize(const Vector4& v)
    {
        auto length = Length(v);
        if (length < Epsilon)
            return Vector4::zero;
        return v / length;
    }
    float Dot(const Vector4& a,const Vector4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    template <> std::string ToString<Vector4>(const Vector4& v)
    {
        return "("
        + ToString(v.x) + ","
        + ToString(v.y) + ","
        + ToString(v.z) + ","
        + ToString(v.w) + ")";
    }
}
