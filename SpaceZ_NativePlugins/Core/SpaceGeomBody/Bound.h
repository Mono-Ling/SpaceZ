#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Math.h"
namespace Core::SpaceZ
{
    struct Bound
    {
        Vector3 center;
        Vector3 extents;
        Bound() : center(Vector3()),extents(Vector3()){}
        Bound(const Vector3& center, const Vector3& extents);

        Vector3 Max() const;
        Vector3 Min() const;
        float Volume() const;
        float SurfaceArea() const;

        void operator=(const Bound& bound);
    };

    Bound operator+(const Bound& a, const Bound& b);
    Bound& operator+=(Bound& a, const Bound& b);
    bool operator==(const Bound& a, const Bound& b);
    bool operator!=(const Bound& a, const Bound& b);

    Bound GetBound(const Vector3& minV, const Vector3& maxV);
    bool IsIntersect(const Bound& a, const Bound& b);
    bool IsContains(const Bound& a, const Bound& b);
}
namespace Core
{
    template<>
    std::string ToString<Core::SpaceZ::Bound>(const Core::SpaceZ::Bound& b);
}
