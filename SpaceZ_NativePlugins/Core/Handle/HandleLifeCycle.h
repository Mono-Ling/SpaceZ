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
        HandleLifeCycle();

        Handle<T> CreateHandle(T* info);
        bool Contains(const Handle<T>& handle);
        bool TryGet(const Handle<T>& handle, T*& item);
        void Remove(const Handle<T>& handle);
        bool TryRemove(const Handle<T>& handle);
        const T* operator[](const Handle<T>& handle);
    };
}
#include"Core/Handle/HandleLifeCycle.tpp"
