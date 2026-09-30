# SpaceZ

自研 3D 碰撞检测插件（Alpha）。C++20 原生核心编译为 Windows 原生插件，通过 C ABI 导出接口，由 Unity 托管层（C#）驱动；物理世界不挂在任何场景对象上，纯静态驱动，进出 Play 模式自动创建与销毁。

## 功能现状

- **宽相**：动态 BVH（`BVHTree` / `BVHNode`），按脏数据增量重建
- **窄相**：GJK 相交判定 + EPA 求穿透信息（深度 / 法线 / 接触点，迭代上限 36）
- **碰撞体**：Box / Sphere / Capsule，抽象 `Collider` 暴露 `Support(dir)` 接口，为 Mesh 凸包等更多凸体预留
- **托管接入**：组件式挂载、Transform 脏标记同步、Enter / Stay / Exit 碰撞回调
- **尚未实现**：刚体动力学（施力、积分、约束求解），`SpaceSystem` 目前只提供碰撞检测查询

## 架构

```
┌─────────────────────────────────────────────────────────┐
│  Unity 托管层（Assets/Scripts，C#）                       │
│  组件挂载 → 脏数据收集 → 批量封送 → 取回碰撞对 → 回调分发  │
└───────────────────────────┬─────────────────────────────┘
                            │ C ABI
                            │ （Editor：LoadLibrary 动态加载；Player：DllImport）
┌───────────────────────────▼─────────────────────────────┐
│  UnityNativePlugins/（导出绑定层）                        │
│  extern "C" __declspec(dllexport)，空指针守卫 + 异常上报   │
└───────────────────────────┬─────────────────────────────┘
┌───────────────────────────▼─────────────────────────────┐
│  Core/（平台无关 C++20 核心，控制台与 DLL 共用）           │
│  SpaceSystem 门面 → BVH 宽相 → GJK/EPA 窄相               │
│  Handle<T>（id + generation）对象池 · 自研数学库           │
└─────────────────────────────────────────────────────────┘
```

### 运行时生命周期

托管层由 `[RuntimeInitializeOnLoadMethod(BeforeSceneLoad)]` 启动 `RunTimeManager`，它反射扫描全程序集带 `[RunTimeStart(order)]` / `[RunTimeEnd(order)]` 的静态方法并按 `order` 排序调用：

| 顺序 | 阶段 |
|---|---|
| `-100` | 加载原生 DLL，解析全部导出函数指针 |
| `-90` | `StartSpaceSystem()` 创建原生 `SpaceSystem`，初始化同步缓冲，向 `PlayerLoop` 注入自定义帧循环 |
| 每帧 | `SpaceZUpdate` 收集组件脏数据 → `LateSpaceZUpdate` 批量推送原生、更新碰撞对缓存、分发碰撞回调 |
| `90` | `StopSpaceSystem()` 销毁原生世界 |
| `100` | 卸载 DLL（Editor 下由 `playModeStateChanged` 触发，Player 下由 `Application.quitting` 触发） |

自定义帧循环（`SpaceZUpdate` / `LateSpaceZUpdate`）注入在 `PreUpdate` 之前，以 0.02 秒节流（约 50 Hz），帧更新间隔`SpaceZTime.deltaTime` 取自 `Time.realtimeSinceStartup` 差值。

### 使用SpaceZ帧循环

可通过`SpaceZUpdate.AddListener`和`SpaceZUpdate.RemoveListener`注册/注销帧循环回调（`LateSpaceZUpdate`同理）
推荐在SpaceZUpdate操作存在SpaceZ相关组件的游戏对象，以便在`LateSpaceZUpdate`正确同步至原生层

### Unity组件

- **SpaceObject**：空间物体，碰撞回调的作用对象，可管理多个碰撞体。发生碰撞后插件会获取SpaceObject所在GameObject上所有继承`IOnCollision`的组件并缓存，后续所有碰撞回调函数均调用该缓存
**注意** 当前尚未提供碰撞期间更新缓存回调对象的API，如有需要可在`Scripts/Plugin/CollisionCallbackBuffer.cs`进行拓展

- **SpaceZCollider派生**：碰撞体组件，碰撞检测的基本单位，创建时会自动沿Transform层级寻找SpaceObject归属
**注意**插件允许无SpaceObject归属的静态碰撞体（NativeHandle.NULL占位），但静态碰撞体间碰撞在宽相检测阶段会被过滤，且与非静态碰撞体的碰撞不会触发其所在 GameObject 的碰撞回调

### 数据同步设计

- **变了才同步**：`SpaceObject` 每帧把 `position / rotation / lossyScale` 打包成 `NativeTransformSynMsg` 与上帧比较，仅脏数据进入 `NativeList<T>`（`Allocator.Persistent`），经 `GetUnsafeReadOnlyPtr` 指针批量封送。
- **默认在`LateSpaceZUpdate`同步脏数据**：托管层脏数据默认在`LateSpaceZUpdate`批量同步原生层，同步API为私有，如有需要可将`Assets/Scripts/Plugin/SpaceZManaged.cs/SynDirtyData`函数设为公共
- **碰撞体相对变换**：`SpaceZCollider` 用父级 `SpaceObject` 的 `worldToLocalMatrix` 折算自身上报，父级变化时（`OnTransformParentChanged`）即时改归属。
- **Handle 防悬挂**：`NativeHandle { int id; int generation; }`，代数递增使旧句柄自然失效；导出到 C 边界的结构体保持纯聚合，避免构造函数引入的封送 ABI 问题。
- **双加载模式**：Editor 下经 `NativeLoader`（`LoadLibrary` / `GetProcAddress` + 显式声明的桥接委托）动态加载，便于域重载；Player 下直接 `[DllImport("SpaceZ_NativePlugins")]`。

## 目录结构

```
SpaceZ/
├── Assets/
│   ├── Plugins/x86_64/SpaceZ_NativePlugins.dll   # 原生插件产物（随仓库提交）
│   ├── Scenes/SampleScene.unity                  # 测试场景：Box/Sphere/Capsule 各一对两两对撞
│   └── Scripts/
│       ├── Framework/RunTime/          # RunTimeManager、PlayerLoop 注入、SpaceZTime
│       ├── Framework/PluginFramework/  # NativeLoader 动态加载器（Editor）
│       └── Plugin/
│           ├── SpaceZNative.cs / SpaceZManaged.cs   # 原生绑定 static partial class SpaceZPlugin
│           ├── CollisionCallbackBuffer.cs           # Enter/Stay/Exit 差分缓冲
│           ├── Component/                           # SpaceObject、SpaceZCollider（Box/Sphere/Capsule）、IOnCollision
│           ├── Connect/                             # NativeHandle、函数指针委托、碰撞数据结构
│           └── SynMsg/                              # Transform/Collider 同步消息结构
└── SpaceZ_NativePlugins/              # 原生插件工程（与 Assets 同级，不被 Unity 导入）
    ├── Core/                          # 平台无关核心
    │   ├── BVH/                       # 宽相：BVHTree / BVHNode / BVHNodeObject
    │   ├── Collider/                  # 碰撞体：Box / Sphere / Capsule
    │   ├── SpaceGeomBody/             # 凸体几何（Box/Sphere/Capsule/Mesh）、Simplex、EPA
    │   ├── System/                    # SpaceSystem 门面、NarrowPhaseSystem（GJK/EPA）
    │   ├── Handle/                    # Handle<T>、HandleLifeCycle 对象池
    │   ├── Info/                      # CollisionPair / CollisionInfo、同步消息
    │   ├── SpaceObject/
    │   └── Tools/                     # Vector3/4、Matrix3x3/4x4、Quaternion、Transform
    ├── UnityNativePlugins/            # C ABI 导出绑定层 + 原生日志回调
    ├── Main/                          # 控制台冒烟测试入口（SpaceZ_Console.exe）
    └── tools/build.bat                # 唯一构建入口
```

## 快速开始

### 环境要求

- Unity **2022.3 LTS**（本工程基于 2022.3.62f3c1，内置渲染管线）；依赖 `com.unity.collections` 提供 `NativeList<T>`
- Visual Studio 2022（含 MSVC x64 工具集与 Windows SDK）——仅修改/构建原生插件时需要

### 构建原生插件

产物 DLL 已随仓库提交，克隆后直接打开 Unity 工程即可运行。修改了原生代码后重新构建：

```bat
cd SpaceZ_NativePlugins
tools\build.bat plugin-release
```

四个目标：`plugin-release` / `plugin-debug`（产物直接覆盖 `Assets/Plugins/x86_64/SpaceZ_NativePlugins.dll`）、`console-debug`（编译 `Main/Debug/SpaceZ_Console.exe` 控制台冒烟测试）、`clean`。

### 在 Unity 中使用

无需任何场景级配置，三步接入：

1. 给物理对象的根物体挂 **`SpaceObject`**；
2. 在其子物体上挂 **`SpaceZBoxCollider` / `SpaceZSphereCollider` / `SpaceZCapsuleCollider`**；
3. 需要碰撞通知的物体实现 **`IOnCollision`** 接口：

```csharp
using SpaceZ;
using UnityEngine;

public class CollisionDemo : MonoBehaviour, IOnCollision
{
    public void OnSpaceZCollisionEnter(SpaceZCollider collider, CollisionInfo info)
    {
        // info.depth：穿透深度；info.normal：法线；info.point：接触点
    }

    public void OnSpaceZCollisionStay(SpaceZCollider collider, CollisionInfo info) { }
    public void OnSpaceZCollisionExit(SpaceZCollider collider, CollisionInfo info) { }
}
```

`Assets/Scripts/Test.cs` 是一个完整示例：碰撞进入 / 结束时切换材质，运行 `Assets/Scenes/SampleScene.unity` 即可看到效果。

## 原生导出 API

全部为 `extern "C" __declspec(dllexport)`，统一经 `SpaceSystem` 门面（`Core/System/SpaceSystem.h`）对接核心：

| 类别 | 导出函数 | 说明 |
|---|---|---|
| 系统 | `StartSpaceSystem` / `StopSpaceSystem` | 创建 / 销毁物理世界 |
| 对象 | `CreateSpaceObject` / `DestroySpaceObject` | 返回 `SpaceObjectHandle` |
| 碰撞体 | `CreateCollider` / `DestroyCollider` / `UpdateColliderSpaceObject` | 类型：Box / Sphere / Capsule，挂到指定 `SpaceObjectHandle`；最后一项变更碰撞体的归属对象 |
| 变换同步 | `UpdateSpaceObjectTransform` / `UpdateColliderTransform` | `(Handle, TransformSynMsg)` 数组批量推送 |
| 形状参数 | `UpdateBoxCollider` / `UpdateSphereCollider` / `UpdateCapsuleCollider` | 半尺寸 / 半径 / 半径+高度 |
| 查询 | `GetCollisionPairsCount` / `GetCollisionPairs` | 取回碰撞对缓存，见下 |
| 调试 | `InjectDebugCallback` / `UnloadDebugCallback` | Editor 下把原生日志重定向到 Unity Console |

> 窄相步进发生在 `GetCollisionPairsCount()` 内部：托管层每帧在 `LateSpaceZUpdate` 调用它触发宽相→窄相流水线，再经 `GetCollisionPairs(buffer, ref count)` 把 `CollisionPair` 拷入 C# 预分配缓冲。回调的 Enter / Stay / Exit 由托管侧 `CollisionCallbackBuffer` 对连续帧碰撞对做差分得出。