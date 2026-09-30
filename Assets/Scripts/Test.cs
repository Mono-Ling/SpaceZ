using System.Collections;
using System.Collections.Generic;
using SpaceZ;
using SpaceZ.Plugin;
using UnityEngine;

public class Test : MonoBehaviour, IOnCollision
{
    public Material collisionEnter;
    public Material collisionExit;
    private Renderer _renderer;
    void Awake()
    {
        if(!collisionEnter || !collisionExit)
            Debug.LogError("【Test】材质缺失");
        _renderer = GetComponent<Renderer>();
        if(_renderer == null)
            Debug.LogError("【Test】渲染器获取失败");
    }
    public void OnSpaceZCollisionEnter(SpaceZCollider collider, CollisionInfo info)
    {
        Debug.Log("【Test】碰撞开始",this);
        if(_renderer)
            _renderer.material = collisionEnter;
    }

    public void OnSpaceZCollisionExit(SpaceZCollider collider, CollisionInfo info)
    {
        Debug.Log("【Test】碰撞结束",this);
        if(_renderer)
            _renderer.material = collisionExit;
    }

    public void OnSpaceZCollisionStay(SpaceZCollider collider, CollisionInfo info)
    { }
}
