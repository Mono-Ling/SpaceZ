#pragma once
#include<string>
#include"ToString.h"

namespace Core
{
    struct Vector3
    {
        float x;
        float y;
        float z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vector3 operator-() const;
        Vector3 normalized() const;
        float length() const;

        static const Vector3 up;
        static const Vector3 forward;
        static const Vector3 right;
        static const Vector3 zero;
        static const Vector3 one;
    };

    Vector3 operator+(const Vector3& a, const Vector3& b);
    Vector3 operator-(const Vector3& a, const Vector3& b);
    Vector3 operator*(const Vector3& v, float scalar);
    Vector3 operator*(float scalar, const Vector3& v);
    Vector3 operator/(const Vector3& v, float scalar);
    bool operator==(const Vector3& a,const Vector3& b);
    bool operator!=(const Vector3& a,const Vector3& b);

    float Dot(const Vector3& a, const Vector3& b);
    float Length(const Vector3& v);
    float Distance(const Vector3& a, const Vector3& b);
    Vector3 Normalize(const Vector3& v);
    Vector3 Cross(const Vector3& a, const Vector3& b);
    Vector3 Lerp(const Vector3& a, const Vector3& b, float f);

    template<>
    std::string ToString<Vector3>(const Vector3& v);
}
