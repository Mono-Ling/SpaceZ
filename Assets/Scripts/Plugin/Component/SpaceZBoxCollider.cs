using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static UnityEngine.Mathf;

namespace SpaceZ
{
    public class SpaceZBoxCollider : SpaceZCollider
    {
        public Color color = Color.green;
        public Vector3 extents
        {
            get => _extents;
            set
            {
                value = new(Abs(value.x), Abs(value.y), Abs(value.z));
                if(value == _extents)
                    return;
                _extents = value;
                this.UpdateBoxCollider(extents);
            }
        }
        [SerializeField]
        private Vector3 _extents = Vector3.one / 2f;
        void OnEnable()
        {
            CreateCollider(ColliderType.Box);
            this.UpdateBoxCollider(extents);
        }
        void OnDrawGizmosSelected()
        {
            Gizmos.color = color;
            Gizmos.matrix = transform.localToWorldMatrix;
            Gizmos.DrawWireCube(Vector3.zero, _extents * 2);
            Gizmos.matrix = Matrix4x4.identity;
        }
        [ContextMenu("刷新碰撞盒大小")]
        private void RefreshSize()
        => this.UpdateBoxCollider(_extents);
    }
}