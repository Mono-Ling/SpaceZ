#include"UnityNativePlugins/Debug/IUnityLog.h"
#include"UnityNativePlugins/Debug/NativeDebug.h"

static IUnityLog* unityLogPtr = nullptr;

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginLoad(IUnityInterfaces* interfacePtr)
{
    if (interfacePtr)
        unityLogPtr = interfacePtr->Get<IUnityLog>();
}
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
UnityPluginUnload()
{
    unityLogPtr = nullptr;
}
namespace NativePlugins::Debug
{
    void Log(const std::string& str)
    {
        if(unityLogPtr)
            UNITY_LOG(unityLogPtr, str.data());
    }
    void LogWarning(const std::string& str)
    {
        if(unityLogPtr)
            UNITY_LOG_WARNING(unityLogPtr, str.data());
    }
    void LogError(const std::string& str)
    {
        if(unityLogPtr)
            UNITY_LOG_ERROR(unityLogPtr, str.data());
    }
}
