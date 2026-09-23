#include"Core/SpaceGeomBody/EPA/Plane.h"
namespace Core::SpaceZ
{
    Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        Fill(a, b, c);
    }
    void Plane::Fill(const Vector3& a, const Vector3& b, const Vector3& c)
    {
        A = a;
        B = b;
        C = c;
        normal = Cross(B - A, C - A);
        if (Dot(normal, A) < 0)
        {
            Vector3 t = B;
            B = C;
            C = t;
            normal = -normal;
        }
        distance = PlaneToPointDistance(normal, A, Vector3::zero);
        used = true;
    }
    bool Plane::HasEdge(const Vector3& a, const Vector3& b)
    {
        return (A == a && B == b) || (A == b && B == a) || (B == a && C == b) ||
               (B == b && C == a) || (C == a && A == b) || (C == b && A == a);
    }
}
