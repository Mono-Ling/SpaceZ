#include "Core/CoreAdd.h"
#include "UnityNativePlugins/Debug/NativeDebug.h"
extern "C" __declspec(dllexport) int Add(int a, int b)
{
    NativePlugins::Debug::Log("【NativePlugs】function add");
    return CoreAdd(a, b);
}
