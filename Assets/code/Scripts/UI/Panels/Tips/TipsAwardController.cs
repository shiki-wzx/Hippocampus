﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UI.Panels;
using UI.Panels.Providers;
using UI.Panels.Providers.DataProviders.StaticBoard;
using UnityEngine.UI;
using UI.Panels.Providers.DataProviders;
using DG.Tweening;

namespace Tips
{

    public class TipsAwardController : UIPanel<UIDataProviderTip, TipDataProvider>
    {

        [SerializeField]
        private Text m_name = null;
        [SerializeField]
        private Text m_description = null;
        [SerializeField]
        private Image m_tipPanel = null;
        public override void Initialize(UIDataProvider uiDataProvider, UIPanelSettings settings)
        {
            base.Initialize(uiDataProvider, settings);
        }

        public override void ShowData(DataProvider data)
        {
            base.ShowData(data);
            Init(UIPanelDataProvider.Data);
        }

        public void Init(TipData data)
        {
            SetData(data);
            Show();
        }

        private void SetData(TipData data)
        {
            m_data = data;
        }

        public void Show()
        {
            m_tipPanel.transform.DOLocalMoveX(460, 1f);
            if (m_data != null)
            {
                m_name.text = $"Tips:{m_data.tip}";
                m_description.text = $"Tips:{m_data.description}";
            }
            StartCoroutine(Wait(5f, Close));
        }

        private void Close()
        {
            m_tipPanel.transform.DOLocalMoveX(1460, 1f).OnComplete(base.InvokeHidePanel);
        }

        private IEnumerator Wait(float _t, System.Action action)
        {
            float vStart = Time.time;
            while (Time.time - vStart < _t)
            {
                yield return null;
            }
            action?.Invoke();
            yield return null;
        }

        /// <summary>数据</summary>
        private TipData m_data = null;
    }
}
