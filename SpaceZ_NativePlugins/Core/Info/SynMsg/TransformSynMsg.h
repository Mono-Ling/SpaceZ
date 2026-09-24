#pragma once
#include"Core/Tools/Vector3.h"
#include"Core/Tools/Quaternion.h"

namespace Core::SpaceZ
{
    struct TransformSynMsg
    {
        Vector3 position;
        Vector3 scale;
        Quaternion rotation;

        TransformSynMsg() : position(Vector3::zero), scale(Vector3::one), rotation(Quaternion::identity) {}
        TransformSynMsg(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
        : position(pos), rotation(rot), scale(scale) {}
    };
}
