#pragma once
#include"Core/Tools/Vector3.h"
#include<utility>
#include<array>

namespace Core::SpaceZ
{
    struct Simplex
    {
        int pointCount;
        std::array<std::pair<bool,Vector3>, 4> points;
        Simplex();

        void Compact();
        void Add(const Vector3& newPoint);
        Vector3 Newest() const;
        bool ContainsOrigin(Vector3& updateDir);

    private:
        /// @brief 点包含原点检测
        /// @param points 点集
        /// @return 是否包含原点
        bool PointContainsOrigin(Vector3& updateDir);

        /// @brief 线段包含原点检测
        /// @param points 点集
        /// @return 是否包含原点
        bool LineContainsOrigin(Vector3& updateDir);

        /// @brief 三角形包含原点检测
        /// @param points 点集
        /// @return 是否包含原点
        bool TriangleContainsOrigin( Vector3& updateDir);

        /// @brief 四面体包含原点检测
        /// @param points 点集
        /// @return 是否包含原点
        bool TetrahedronContainsOrigin(Vector3& updateDir);
    };
}
