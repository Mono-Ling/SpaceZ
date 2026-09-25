#include"Core/Handle/HandleLifeCycle.h"

namespace Core::SpaceZ
{
    template<typename T>
    HandleLifeCycle<T>::HandleLifeCycle()
    {
        _handleBuffer.push(Handle<T>(0));
    }

    template<typename T>
    Handle<T> HandleLifeCycle<T>::CreateHandle(T* ptr)
    {
        Handle<T> handle = Handle<T>::null;
        if(!ptr)
            return handle;
        if(!_handleBuffer.empty())
        {
            handle = _handleBuffer.front();
            _handleBuffer.pop();
        }
        else
            handle = Handle<T>(_usedHandleMap.size());
        auto result =_usedHandleMap.insert({handle, ptr});

        // 冲突覆盖（理论上不存在这种情况）
        if(!result.second)
            _usedHandleMap[handle] = ptr;
        return handle;
    }
    template<typename T>
    bool HandleLifeCycle<T>::Contains(const Handle<T>& handle)
    {
        return _usedHandleMap.count(handle);
    }
    template<typename T>
    bool HandleLifeCycle<T>::TryGet(const Handle<T>& handle, T*& item)
    {
        auto it = _usedHandleMap.find(handle);
        if(it == _usedHandleMap.end())
            return false;
        item = it->second;
        return true;
    }
    template<typename T>
    void HandleLifeCycle<T>::Remove(const Handle<T>& handle)
    {
        TryRemove(handle);
    }
    template<typename T>
    bool HandleLifeCycle<T>::TryRemove(const Handle<T>& handle)
    {
        auto it = _usedHandleMap.find(handle);
        if(it == _usedHandleMap.end())
            return false;
        auto destroy = it->first;
        destroy.Carry();
        _handleBuffer.push(destroy);
        _usedHandleMap.erase(it);
        return true;
    }
    template<typename T>
    const T* HandleLifeCycle<T>::operator[](const Handle<T>& handle)
    {
        auto it = _usedHandleMap.find(handle);
        if(it == _usedHandleMap.end())
            return nullptr;
        return it->second;
    }
}
