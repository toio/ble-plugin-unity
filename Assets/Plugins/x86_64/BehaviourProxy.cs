
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace toio.Windows
{
    internal class BehaviourProxy : MonoBehaviour
    {
        private System.Action onUpdate;
        public static BehaviourProxy Create(System.Action updateFunc)
        {
            var gmo = new GameObject("BehaviourProxy");
            gmo.hideFlags = HideFlags.HideAndDontSave;
            Object.DontDestroyOnLoad(gmo);
            var proxy = gmo.AddComponent<BehaviourProxy>();
            proxy.onUpdate = updateFunc;
            return proxy;
        }
        void Update()
        {
            if(onUpdate != null) { 
                onUpdate(); 
            }
        }
    }
}

#endif