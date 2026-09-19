#include"UnityNativePlugins/Debug/IUnityLog.h"
#include"UnityNativePlugins/Debug/NativeDebug.h"

static IUnityLog* unityLogPtr = nullptr;

typedef void (*DebugCallback)(const char* message, int size);
static DebugCallback logCallbackPtr = nullptr;
static DebugCallback warningCallbackPtr = nullptr;
static DebugCallback errorCallbackPtr = nullptr;

#pragma region Unity日志注入
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
#pragma endregion

#pragma region Debug回调注入
extern "C" void UNITY_INTERFACE_EXPORT
InjectDebugCallback(DebugCallback logPtr, DebugCallback warningPtr, DebugCallback errorPtr)
{
    logCallbackPtr = logPtr;
    warningCallbackPtr = warningPtr;
    errorCallbackPtr = errorPtr;
}
extern "C" void UNITY_INTERFACE_EXPORT
UnloadDebugCallback()
{
    logCallbackPtr = nullptr;
    warningCallbackPtr = nullptr;
    errorCallbackPtr = nullptr;
}
#pragma endregion

namespace NativePlugins::Debug
{
    void Log(const std::string& str)
    {
        if(unityLogPtr)
            UNITY_LOG(unityLogPtr, str.data());
        else
            if(logCallbackPtr)
                logCallbackPtr(str.data(),str.size());
    }
    void LogWarning(const std::string& str)
    {
        if(unityLogPtr)
            UNITY_LOG_WARNING(unityLogPtr, str.data());
        else
            if(warningCallbackPtr)
                warningCallbackPtr(str.data(),str.size());
    }
    void LogError(const std::string& str)
    {
        if(unityLogPtr)
            UNITY_LOG_ERROR(unityLogPtr, str.data());
        else
            if(errorCallbackPtr)
                errorCallbackPtr(str.data(),str.size());
    }
}
