#include"Core/SpaceGeomBody/Bound.h"
#include"Core/Tools/Math.h"
using namespace Core::Math;

namespace Core::SpaceZ
{
    Bound::Bound(const Vector3& center, const Vector3& extents) : center(center)
    {
        this->extents = Vector3
        (
            Abs(extents.x),
            Abs(extents.y),
            Abs(extents.z)
        );
    }
    Vector3 Bound::Min() const{return center - extents;}
    Vector3 Bound::Max() const{return center + extents;}
    float Bound::Volume() const{return extents.x * extents.y * extents.z * 8;}
    float Bound::SurfaceArea() const
    {
        return 2.0f * (extents.x * extents.y + extents.y * extents.z + extents.z * extents.x);
    }

    Bound GetBound(const Vector3& minV, const Vector3& maxV)
    {
        Vector3 realMin
        {
            Min(minV.x, maxV.x),
            Min(minV.y, maxV.y),
            Min(minV.z, maxV.z)
        };
        Vector3 realMax
        {
            Max(minV.x, maxV.x),
            Max(minV.y, maxV.y),
            Max(minV.z, maxV.z)
        };
        auto half = (realMax - realMin) / 2;
        return Bound(realMin + half, half);
    }
    bool IsIntersect(const Bound& a, const Bound& b)
    {
        auto minA = a.Min();
        auto minB = b.Min();
        auto maxA = a.Max();
        auto maxB = b.Max();
        if (minA.x > maxB.x || minB.x > maxA.x)
            return false;
        if (minA.y > maxB.y || minB.y > maxA.y)
            return false;
        if (minA.z > maxB.z || minB.z > maxA.z)
            return false;
        return true;
    }
    bool IsContains(const Bound& a, const Bound& b)
    {
        auto minA = a.Min();
        auto minB = b.Min();
        auto maxA = a.Max();
        auto maxB = b.Max();
        return maxA.x >= maxB.x && minA.x <= minB.x
            && maxA.y >= maxB.y && minA.y <= minB.y
            && maxA.z >= maxB.z && minA.z <= minB.z;
    }
    Bound operator+(const Bound& a, const Bound& b)
    {
        auto minA = a.Min();
        auto minB = b.Min();
        auto maxA = a.Max();
        auto maxB = b.Max();
        Vector3 min
        {
            Min(minA.x, minB.x),
            Min(minA.y, minB.y),
            Min(minA.z, minB.z)
        };
        Vector3 max
        {
            Max(maxA.x, maxB.x),
            Max(maxA.y, maxB.y),
            Max(maxA.z, maxB.z)
        };
        return GetBound(min,max);
    }
    Bound& operator+=(Bound& a, const Bound& b)
    {
        a = a + b;
        return a;
    }
    bool operator==(const Bound& a, const Bound& b)
    {
        return a.center == b.center
            && a.extents == b.extents;
    }
    bool operator!=(const Bound& a, const Bound& b)
    {
        return !(a == b);
    }
}
namespace Core
{
    template<>
    std::string ToString<Core::SpaceZ::Bound>(const Core::SpaceZ::Bound& b)
    {
        return "Center:" + ToString(b.center) + " Extents:" + ToString(b.extents);
    }
}
