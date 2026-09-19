#include "Core/Handle/Handle.h"

namespace Core::SpaceZ
{
    template<typename T>
    const Handle<T> Handle<T>::null = Handle<T>();

    template<typename T>
    void Handle<T>::Carry()
    {
        this->generation++;
    }
    template<typename T>
    bool Handle<T>::Equals(const Handle<T>& other) const
    {
        return this->id == other.id && this->generation == other.generation;
    }
    template<typename T>
    std::size_t Handle<T>::GetHashCode() const
    {
        std::size_t h1 = std::hash<int>{}(this->id);
        std::size_t h2 = std::hash<int>{}(this->generation);

        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }

    template <typename T>
    bool operator==(const Handle<T>& a, const Handle<T>& b)
    {
        return a.Equals(b);
    }
    template <typename T>
    bool operator!=(const Handle<T>& a, const Handle<T>& b)
    {
        return !a.Equals(b);
    }
}
