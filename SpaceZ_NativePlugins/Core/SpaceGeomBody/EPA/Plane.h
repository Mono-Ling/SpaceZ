#pragma once
#include"Core/Tools/Vector3.h"

namespace Core::SpaceZ
{
    struct Plane
    {
        bool used;
        float distance;
        Vector3 normal;
        Vector3 A;
        Vector3 B;
        Vector3 C;

        Plane()
        : used(false), distance(0.0f), normal(Vector3::zero),
          A(Vector3::zero), B(Vector3::zero), C(Vector3::zero) {}
        Plane(const Vector3& a, const Vector3& b, const Vector3& c);

        void Fill(const Vector3& a, const Vector3& b, const Vector3& c);
        bool HasEdge(const Vector3& a, const Vector3& b);
    };
}
