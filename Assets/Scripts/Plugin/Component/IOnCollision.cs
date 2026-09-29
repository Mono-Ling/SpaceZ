using System.Collections;
using System.Collections.Generic;
using UnityEngine;


namespace SpaceZ
{
    public interface IOnCollision
    {
        void OnCollisionEnter(SpaceZCollider collider, CollisionInfo info);
        void OnCollisionStay(SpaceZCollider collider, CollisionInfo info);
        void OnCollisionExit(SpaceZCollider collider, CollisionInfo info);
    }
}
