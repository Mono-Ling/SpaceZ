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

        /// @brief 单纯形是否包含原点(GJK)
        /// @param updateDir 更新方向
        /// @return 是否包含原点
        bool ContainsOrigin(Vector3& updateDir);

        /// @brief 是否存在共线顶点
        /// @param a 共线顶点索引
        /// @param b 共线顶点索引
        /// @param c 共线顶点索引
        /// @return 是否存在共线顶点
        bool IsCollineation(int& a, int& b, int& c) const;

        /// @brief 共线顶点取直
        void ClearCollineation();

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

        void SetPoints(const Vector3 A, const Vector3& B);
    };
}
