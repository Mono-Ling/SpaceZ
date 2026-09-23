#include "Core/SpaceGeomBody/Simplex.h"
#include "Core/Tools/Math.h"
#include <algorithm>

using namespace std;
using namespace Core::Math;
namespace Core::SpaceZ
{
    Simplex::Simplex()
    {
        pointCount = 0;
        for (auto& i : points)
            i.first = false;
    }

    void Simplex::Compact()
    {
        stable_partition(points.begin(), points.end(),
                         [](const pair<bool, Vector3>& a) { return a.first; });
    }
    void Simplex::Add(const Vector3& newPoint)
    {
        if (pointCount >= 4)
            return;
        points[pointCount++] = {true, newPoint};
    }
    Vector3 Simplex::Newest() const
    {
        if (!pointCount)
            return Vector3::zero;
        for (auto it = points.rbegin(); it != points.rend(); it++)
            if (it->first)
                return it->second;
        return Vector3::zero;
    }
    bool Simplex::ContainsOrigin(Vector3& updateDir)
    {
        switch(pointCount)
        {
            case 1 :
                return PointContainsOrigin(updateDir);
            break;

            case 2:
                return LineContainsOrigin(updateDir);
            break;

            case 3:
                return TriangleContainsOrigin(updateDir);
            break;

            case 4:
                return TetrahedronContainsOrigin(updateDir);
            break;
        }
        return false;
    }

    bool Simplex::PointContainsOrigin(Vector3& updateDir)
    {
        if (pointCount != 1)
            return false;
        updateDir = -points[0].second;
        return false;
    }
    bool Simplex::LineContainsOrigin(Vector3& updateDir)
    {
        if (pointCount != 2)
            return false;
        auto A = points[1].second;
        auto B = points[0].second;
        auto AB = B - A;
        auto AO = -A;

        if (Dot(AB, AO) > 0)
        {
            updateDir = Cross(Cross(AB, AO), AB);
            return false;
        }
        else
        {
            points[0] = {true, A};
            points[1].first = false;
            updateDir = AO;

            pointCount--;
            Compact();
            return false;
        }
    }
    bool Simplex::TriangleContainsOrigin(Vector3& updateDir)
    {
        if (pointCount != 3)
            return false;
        auto A = points[2].second;
        auto B = points[1].second;
        auto C = points[0].second;

        auto AB = B - A;
        auto AC = C - A;
        auto AO = -A;
        auto ABC = Cross(AB, AC);
        if (Dot(Cross(ABC, AC), AO) > 0)
        {
            if (Dot(AC, AO) > 0)
            {
                points[0] = {true, C};
                points[1] = {true, A};
                points[2].first = false;
                updateDir = Cross(Cross(AC, AO), AC);

                pointCount--;
                Compact();
            }
            else
            {
                points[0] = {true, A};
                points[1].first = false;
                points[2].first = false;
                updateDir = AO;

                pointCount -= 2;
                Compact();
            }
            return false;
        }
        if (Dot(Cross(AB, ABC), AO) > 0)
        {
            if (Dot(AB, AO) > 0)
            {
                points[0] = {true, B};
                points[1] = {true, A};
                points[2].first = false;
                updateDir = Cross(Cross(AB, AO), AB);

                pointCount--;
                Compact();
            }
            else
            {
                points[0] = {true, A};
                points[1].first = false;
                points[2].first = false;
                updateDir = AO;

                pointCount -= 2;
                Compact();
            }
            return false;
        }
        if (Dot(ABC, AO) > 0)
            updateDir = ABC;
        else
        {
            points[0] = {true, B};
            points[1] = {true, C};
            points[2] = {true, A};
            updateDir = -ABC;
        }
        return false;
    }
    bool Simplex::TetrahedronContainsOrigin(Vector3& updateDir)
    {
        if (pointCount != 4)
            return false;

        auto A = points[3].second;
        auto B = points[2].second;
        auto C = points[1].second;
        auto D = points[0].second;

        auto AB = B - A;
        auto AC = C - A;
        auto AD = D - A;
        auto AO = -A;

        auto ABC = Cross(AB, AC);
        auto ACD = Cross(AC, AD);
        auto ADB = Cross(AD, AB);
        auto BCD = Cross(B - D, C - D);

        // 原点在 ABC 面外侧 → 丢弃D点，保留ABC三角形
        if (Dot(ABC, AO) > Epsilon)
        {
            points[0]={true,C};
            points[1]={true,B};
            points[2]={true,A};
            points[3].first = false;
            updateDir = ABC;

            pointCount--;
            Compact();
            return false;
        }
        // 原点在 ACD 面外侧 → 丢弃B点，保留ACD三角形
        if (Dot(ACD, AO) > Epsilon)
        {
            points[0]={true,D};
            points[1]={true,C};
            points[2]={true,A};
            points[3].first = false;
            updateDir = ACD;

            pointCount--;
            Compact();
            return false;
        }
        // 原点在 ADB 面外侧 → 丢弃C点，保留ADB三角形
        if (Dot(ADB, AO) > Epsilon)
        {
            points[0] = {true, B};
            points[1] = {true, D};
            points[2] = {true, A};
            points[3].first = false;
            updateDir = ADB;

            pointCount--;
            Compact();
            return false;
        }
        // 原点在 BCD 面外侧 → 丢弃A点，保留BCD三角形
        if (Dot(BCD, AO) > Epsilon)
        {
            points[0]={true,D};
            points[1]={true,C};
            points[2]={true,B};
            points[3].first = false;
            updateDir = BCD;

            pointCount--;
            Compact();
            return false;
        }
        return true;
    }
}
