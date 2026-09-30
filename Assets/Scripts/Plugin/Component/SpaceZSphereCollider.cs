using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ
{
    public class SpaceZSphereCollider : SpaceZCollider
    {
        public Color color = Color.green;
        public float radius
        {
            get => _radius;
            set
            {
                value = Mathf.Abs(value);
                if(value == _radius)
                    return;
                _radius = value;
                this.UpdateSphereCollider(_radius);
            }
        }
        [SerializeField]
        private float _radius = 0.5f;
        void OnEnable()
        {
            CreateCollider(ColliderType.Sphere);
            this.UpdateSphereCollider(_radius);
        }

        void OnDrawGizmosSelected()
        {
            Gizmos.color = color;
            Gizmos.matrix = transform.localToWorldMatrix;
            Gizmos.DrawWireSphere(Vector3.zero, _radius);
            Gizmos.matrix = Matrix4x4.identity;
        }
        [ContextMenu("刷新碰撞球半径")]
        private void RefreshRadius()
        => this.UpdateSphereCollider(_radius);
    }
}
