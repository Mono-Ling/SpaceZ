#pragma once
#include<functional>
#include<cstddef>
#include "Core/Collider/Collider.h"
#include "Core/SpaceObject/SpaceObject.h"

namespace Core::SpaceZ
{
    template <typename T> struct Handle
    {
        int id = 0;
        int generation = 0;
        Handle() : id(-1), generation(0) {}
        explicit Handle(int id) : id(id), generation(1) {}

        void Carry();
        bool Equals(const Handle<T>& other) const;
        size_t GetHashCode() const;

        static const Handle null;
    };

    using ColliderHandle = Handle<Collider>;
    using SpaceObjectHandle = Handle<SpaceObject>;

    template <typename T>
    bool operator==(const Handle<T>& a, const Handle<T>& b);
    template <typename T>
    bool operator!=(const Handle<T>& a, const Handle<T>& b);
}
namespace std
{
    template <typename T>
    struct hash<Core::SpaceZ::Handle<T>>
    {
        size_t operator()(const Core::SpaceZ::Handle<T>& h) const
        {
            return h.GetHashCode();
        }
    };
}
#include"Core/Handle/Handle.tpp"
