#include"Core/SpaceGeomBody/Convex/Mesh.h"

using namespace std;
namespace Core::SpaceZ
{
    void Mesh::SetPoints(const std::vector<Vector3>& points)
    {
        _points = vector<Vector3>(points.begin(), points.end());
    }
    Vector3 Mesh::SupportLocal(const Vector3& localDir) const
    {
        if (_points.empty())
            return Vector3::zero;

        Vector3 ans = _points[0];
        float maxDot = Dot(ans, localDir);
        for(auto& point : _points)
        {
            float curr = Dot(point, localDir);
            if(curr > maxDot)
            {
                maxDot = curr;
                ans = point;
            }
        }
        return ans;
    }
}
