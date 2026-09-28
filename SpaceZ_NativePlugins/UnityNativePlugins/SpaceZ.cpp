#include"Core/System/SpaceSystem.h"
#include"Core/Tools/Vector3.h"
#include"UnityNativePlugins/Debug/NativeDebug.h"
#include<string>
#include<utility>
using namespace std;
using namespace Core;
using namespace Core::SpaceZ;
using namespace NativePlugins::Debug;

SpaceSystem* spaceSystemPtr = nullptr;

const SpaceSystem* GetSystemPtrDebug()
{
    if(!spaceSystemPtr)
        LogError("【SpaceZ Native】SpaceSystem指针为空");
    return spaceSystemPtr;
}

#pragma region 生命周期
extern "C" void __declspec(dllexport) StartSpaceSystem()
{
    if(spaceSystemPtr) return;
    spaceSystemPtr = new SpaceSystem();
}
extern "C" void __declspec(dllexport) StopSpaceSystem()
{
    if(!spaceSystemPtr) return;
    delete spaceSystemPtr;
    spaceSystemPtr = nullptr;
}
#pragma endregion

#pragma region 原生对象生命周期
extern "C" ColliderHandle __declspec(dllexport) CreateCollider(ColliderType type, SpaceObjectHandle spaceObj)
{
    if(!GetSystemPtrDebug())
        return ColliderHandle::null;
    try
    {
        return spaceSystemPtr->CreateCollider(type, spaceObj);
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】创建碰撞体异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】创建碰撞体异常Unknown exception");
    }
    return ColliderHandle::null;
}
extern "C" SpaceObjectHandle __declspec(dllexport) CreateSpaceObject()
{
    if(!GetSystemPtrDebug())
        return SpaceObjectHandle::null;
    try
    {
        return spaceSystemPtr->CreateSpaceObject();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】创建空间物体异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】创建空间物体异常Unknown exception");
    }
    return SpaceObjectHandle::null;
}
extern "C" bool __declspec(dllexport) DestroyCollider(ColliderHandle handle)
{
    if(!GetSystemPtrDebug())
        return false;
    try
    {
        return spaceSystemPtr->DestroyCollider(handle);
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】销毁碰撞体异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】销毁碰撞体异常Unknown exception");
    }
    return false;
}
extern "C" bool __declspec(dllexport) DestroySpaceObject(SpaceObjectHandle handle)
{
    if(!GetSystemPtrDebug())
        return false;
    try
    {
        return spaceSystemPtr->DestroySpaceObject(handle);
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】销毁空间物体异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】销毁空间物体异常Unknown exception");
    }
    return false;
}
#pragma endregion

#pragma region 原生对象更新
extern "C" void __declspec(dllexport)
UpdateColliderTransform(pair<ColliderHandle,TransformSynMsg>* msgs, int count)
{
    if(!GetSystemPtrDebug())
        return;
    try
    {
        for(int i = 0; i < count; i++)
            spaceSystemPtr->UpdateColliderTransform(msgs[i].first, msgs[i].second);
        spaceSystemPtr->SynBreadthPhaseSystem();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】碰撞体Transform更新异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】碰撞体Transform更新异常Unknown exception");
    }
}
extern "C" void __declspec(dllexport)
UpdateSpaceObjectTransform(pair<SpaceObjectHandle,TransformSynMsg>* msgs, int count)
{
    if(!GetSystemPtrDebug())
        return;
    try
    {
        for(int i = 0; i < count; i++)
            spaceSystemPtr->UpdateSpaceObjectTransform(msgs[i].first, msgs[i].second);
        spaceSystemPtr->SynBreadthPhaseSystem();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】空间物体Transform更新异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】空间物体Transform更新异常Unknown exception");
    }
}
extern "C" void __declspec(dllexport)
UpdateColliderSpaceObject(pair<ColliderHandle,SpaceObjectHandle>* msgs, int count)
{
    if(!GetSystemPtrDebug())
        return;
    try
    {
        for(int i = 0; i < count; i++)
            spaceSystemPtr->SetColliderSpaceObject(msgs[i].first, msgs[i].second);
        spaceSystemPtr->SynBreadthPhaseSystem();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】碰撞体SpaceObject更新异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】碰撞体SpaceObject更新异常Unknown exception");
    }
}
extern "C" void __declspec(dllexport)
UpdateBoxCollider(pair<ColliderHandle,Vector3>* msgs, int count)
{
    if(!GetSystemPtrDebug())
        return;
    try
    {
        for(int i = 0; i < count; i++)
            spaceSystemPtr->UpdateBoxCollider(msgs[i]);
        spaceSystemPtr->SynBreadthPhaseSystem();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】BoxCollider更新异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】BoxCollider更新异常Unknown exception");
    }
}
extern "C" void __declspec(dllexport)
UpdateSphereCollider(pair<ColliderHandle,float>* msgs, int count)
{
    if(!GetSystemPtrDebug())
        return;
    try
    {
        for(int i = 0; i < count; i++)
            spaceSystemPtr->UpdateSphereCollider(msgs[i]);
        spaceSystemPtr->SynBreadthPhaseSystem();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】SphereCollider更新异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】SphereCollider更新异常Unknown exception");
    }
}
extern "C" void __declspec(dllexport)
UpdateCapsuleCollider(pair<ColliderHandle,CapsuleSynMsg>* msgs, int count)
{
    if(!GetSystemPtrDebug())
        return;
    try
    {
        for(int i = 0; i < count; i++)
            spaceSystemPtr->UpdateCapsuleCollider(msgs[i]);
        spaceSystemPtr->SynBreadthPhaseSystem();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】CapsuleCollider更新异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】CapsuleCollider更新异常Unknown exception");
    }
}
#pragma endregion

#pragma region 碰撞对
extern "C" int __declspec(dllexport) GetCollisionPairsCount()
{
    if(!GetSystemPtrDebug())
        return -1;
    try
    {
        return spaceSystemPtr->UpdateCollisionPairs().size();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】碰撞对更新异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】碰撞对更新异常Unknown exception");
    }
    return -1;
}
extern "C" void __declspec(dllexport) GetCollisionPairs(CollisionPair* buffer, int& bufferSize)
{
    if(!GetSystemPtrDebug())
        return;
    if(!buffer)
    {
        LogError("【SpaceZ Native】缓冲区为空，碰撞对获取失败");
        return;
    }
    try
    {
        auto& pairs = spaceSystemPtr->GetCollisionPairs();
        if(bufferSize < pairs.size())
        {
            LogWarning("【SpaceZ Native】碰撞对数量溢出，封送失败");
            return;
        }
        for(int i = 0; i < pairs.size(); i++)
            buffer[i] = pairs[i];
        bufferSize = pairs.size();
    }
    catch(const std::exception& e)
    {
        LogError("【SpaceZ Native】碰撞对获取异常" + string(e.what()));
    }
    catch (...)
    {
        LogError("【SpaceZ Native】碰撞对获取Unknown exception");
    }
}
#pragma endregion
