#include"Vector3.h"
#include"Math.h"
using namespace Core::Math;
namespace Core
{
    const Vector3 Vector3::forward = Vector3(0,0,1);
    const Vector3 Vector3::up = Vector3(0,1,0);
    const Vector3 Vector3::right = Vector3(1,0,0);
    const Vector3 Vector3::zero = Vector3(0,0,0);
    const Vector3 Vector3::one = Vector3(1,1,1);

    Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    }
    Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }
    Vector3 operator*(const Vector3& v, float scalar)
    {
        return Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
    }
    Vector3 operator*(float scalar, const Vector3& v)
    {
        return v * scalar;
    }
    Vector3 operator/(const Vector3& v, float scalar)
    {
        return Vector3(v.x / scalar,v.y / scalar, v.z / scalar);
    }
    bool operator==(const Vector3& a,const Vector3& b)
    {
        return Abs(a.x - b.x) < Epsilon
            && Abs(a.y - b.y) < Epsilon
            && Abs(a.z - b.z) < Epsilon;
    }
    bool operator!=(const Vector3& a,const Vector3& b)
    {
        return !(a == b);
    }
    float Dot(const Vector3& a, const Vector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    float Length(const Vector3& v)
    {
        return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    }
    float Distance(const Vector3& a, const Vector3& b)
    {
        return Length(a - b);
    }
    Vector3 Normalize(const Vector3& v)
    {
        float len = Length(v);
        if(len < Epsilon)
            return Vector3::zero;
        return v / len;
    }
    Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        return Vector3
        (
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    Vector3 Vector3::operator-() const
    {
        return Vector3(-x, -y, -z);
    }
    Vector3 Vector3::normalized() const
    {
        return Normalize(*this);
    }
    float Vector3::length() const
    {
        return Length(*this);
    }
    Vector3 Lerp(const Vector3& a, const Vector3& b, float f)
    {
        return Vector3
        {
            Core::Math::Lerp(a.x,b.x,f),
            Core::Math::Lerp(a.y,b.y,f),
            Core::Math::Lerp(a.z,b.z,f)
        };
    }

    template<>
    std::string ToString<Vector3>(const Vector3& v)
    {
        return "("+ ToString(v.x) + "," + ToString(v.y) + "," + ToString(v.z) + ")";
    }
}
