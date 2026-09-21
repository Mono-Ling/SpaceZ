#pragma once
#include"Core/Tools/Vector3.h"
#include"ToString.h"

namespace Core
{
    struct Vector4
    {
        float x;
        float y;
        float z;
        float w;

        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Vector4(Vector3 vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(0) {}

        Vector4 normalized() const;
        float length() const;
        void operator=(const Vector3& vec3);

        static const Vector4 zero;
        static const Vector4 one;
    };

    Vector4 operator+(const Vector4& a, const Vector4& b);
    Vector4 operator-(const Vector4& a, const Vector4& b);
    Vector4 operator*(const Vector4& v, float scalar);
    Vector4 operator*(float scalar, const Vector4& v);
    Vector4 operator/(const Vector4& v, float scalar);
    bool operator==(const Vector4& a,const Vector4& b);
    bool operator!=(const Vector4& a,const Vector4& b);

    float Length(const Vector4& v);
    Vector4 Normalize(const Vector4& v);
    float Dot(const Vector4& a,const Vector4& b);

    template<>
    std::string ToString<Vector4>(const Vector4& v);
}
