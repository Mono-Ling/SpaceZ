using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace SpaceZ
{
    public interface IOnCollision
    {
        void OnSpaceZCollisionEnter(SpaceZCollider collider, CollisionInfo info);
        void OnSpaceZCollisionStay(SpaceZCollider collider, CollisionInfo info);
        void OnSpaceZCollisionExit(SpaceZCollider collider, CollisionInfo info);
    }
}
