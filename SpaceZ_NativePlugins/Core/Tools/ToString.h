#pragma once
#include <concepts>
#include <string>

namespace Core
{
    template <typename T>
    std::string ToString(const T&)
    {
        return std::string();
    }

    template <typename T>
        requires requires(T t)
        {
            { std::to_string(t) } -> std::same_as<std::string>;
        }
    std::string ToString(const T& t)
    {
        return std::to_string(t);
    }
}
