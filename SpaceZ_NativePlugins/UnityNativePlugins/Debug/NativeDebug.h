#pragma once
#include<string>

namespace NativePlugins::Debug
{
    void Log(const std::string& str);
    void LogWarning(const std::string& str);
    void LogError(const std::string& str);
}
