#pragma once
#include"Core/Handle/Handle.h"

namespace Core::SpaceZ
{
    class SpaceObject;
    class Collider;

    using ColliderHandle = Handle<Collider>;
    using SpaceObjectHandle = Handle<SpaceObject>;
}
