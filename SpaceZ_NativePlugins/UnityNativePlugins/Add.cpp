#include "Core/CoreAdd.h"
extern "C" __declspec(dllexport) int Add(int a, int b)
{
    return CoreAdd(a, b);
}
