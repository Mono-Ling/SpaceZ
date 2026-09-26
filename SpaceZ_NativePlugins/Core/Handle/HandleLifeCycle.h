#pragma once
#include"Core/Handle/Handle.h"
#include<queue>
#include<unordered_map>

namespace Core::SpaceZ
{
    template<typename T>
    class HandleLifeCycle
    {
    private:
        std::queue<Handle<T>> _handleBuffer;
        std::unordered_map<Handle<T>, T*> _usedHandleMap;

    public:
        HandleLifeCycle() = default;

        Handle<T> CreateHandle();
        bool Contains(const Handle<T>& handle);
        bool TryGet(const Handle<T>& handle, T*& item);
        void Remove(const Handle<T>& handle);
        bool TryRemove(const Handle<T>& handle);
        T*& operator[](const Handle<T>& handle);
        template<typename F>
        void ForEachUsed(F&& func);
    };
}
#include"Core/Handle/HandleLifeCycle.tpp"
