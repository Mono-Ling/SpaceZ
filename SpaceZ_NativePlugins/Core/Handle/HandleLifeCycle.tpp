#include"Core/Handle/HandleLifeCycle.h"

namespace Core::SpaceZ
{
    template<typename T>
    Handle<T> HandleLifeCycle<T>::CreateHandle()
    {
        Handle<T> handle = Handle<T>::null;
        if(!_handleBuffer.empty())
        {
            handle = _handleBuffer.front();
            _handleBuffer.pop();
        }
        else
            handle = Handle<T>(_usedHandleMap.size());
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
    T*& HandleLifeCycle<T>::operator[](const Handle<T>& handle)
    {
        return _usedHandleMap[handle];
    }
    template<typename T>
    template<typename F>
    void HandleLifeCycle<T>::ForEachUsed(F&& func)
    {
        for(auto& item : _usedHandleMap)
            func(item.first, item.second);
    }
}
