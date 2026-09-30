using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ
{
    public class SpaceZCapsuleCollider : SpaceZCollider
    {
        public float height
        {
            get => _height;
            set
            {
                value = Mathf.Abs(value);
                if(value == _height)
                    return;
                this.UpdateCapsuleCollider(_height, _radius);
            }
        }
        public float radius
        {
            get => _radius;
            set
            {
                value = Mathf.Abs(value);
                if(value == _radius)
                    return;
                this.UpdateCapsuleCollider(_height, _radius);
            }
        }
        [SerializeField]
        private float _height = 1;
        [SerializeField]
        private float _radius = 0.5f;
        void OnEnable()
        {
            CreateCollider(ColliderType.Capsule);
            this.UpdateCapsuleCollider(_height, _radius);
        }
        [ContextMenu("刷新胶囊体")]
        void RefreshCapsule()
        => this.UpdateCapsuleCollider(_height, _radius);
    }
}
