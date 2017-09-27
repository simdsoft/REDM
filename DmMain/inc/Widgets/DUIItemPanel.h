//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIItemPanel.h 
// File Des: DUI窗口面板
// File Summary:  
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-11	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class DUIItemPanel;
	/// <summary>
	///		面板的拥有者
	/// </summary>
	class DM_EXPORT IDMItemPanelOwner
	{
	public:
		virtual DUIWindow* GetOwnerWindow() = 0;   
		virtual DMCode OnSetCapture(DUIItemPanel* pPanel) = 0;
		virtual DMCode OnReleaseCapture(DUIItemPanel* pPanel) = 0;
		virtual DMCode OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect) = 0; ///< 请使用*lpRect赋值！
	};

	/// <summary>
	///		面板的内置实现
	/// </summary>
	class DM_EXPORT DUIItemPanel:public DUIWindow,public DMContainerImpl
	{
	public:
		DUIItemPanel(DMXmlNode XmlNode, IDMItemPanelOwner *pItemOwner = NULL);
		~DUIItemPanel();

	public://绘制
		void DrawItem(IDMCanvas* pCanvas, CRect& rc);
		void ModifyState(DWORD dwStateAdd, DWORD dwStateRemove);
		void SetItemId(LPARAM id);										///< 设置标识,保证在一个集合中是唯一的
		LPARAM GetItemId();
		
	public:
		virtual void OnFinalRelease();

	public:// 控制
		DMCode OnSetCaptureWnd(DUIWND DUIWnd,DUIWNDPtr pOldDUIWnd=NULL);
		DMCode OnReleaseCaptureWnd();
		DMCode OnGetContainerRect(LPRECT lpRect);                       ///< 获取面板项的区域

	public:// 重载IDMContainer
		LRESULT OnFrameEvent(UINT uMsg,WPARAM wp,LPARAM lp);
		DMCode OnFireEvent(DMEventArgs &Evt);							///< 将原始消息包装成面板消息,面板消息中带有原始消息
		DMCode OnGetDraw(IDMDraw** ppObj);
		DMCode OnGetCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas**ppCanvas);
		DMCode OnReleaseCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas*pCanvas);
		HWND OnGetHWnd();											    ///< 转发
		DMCode OnIsTranslucent();                                       ///< 转发
		DMCode OnUpdateWindow();										///< 转发
		DMCode OnForceUpdateWindow();									///< 转发
		DMCode OnUpdateRect(LPCRECT lpRect,DUIWND hDUIWnd);             ///< 面板内控件更新刷新
		DMCode OnClientToScreen(LPRECT lpRect);                         ///< 转发  
		DMCode OnRegisterTimeline(IDMTimeline *pHandler);				///< 转发
		DMCode OnUnregisterTimeline(IDMTimeline *pHandler);				///< 转发
		CStringW OnGetTransId();										///< 转发

	public:// 重载DUIWindow
		bool DV_IsStateChangeRedraw(){return true;}
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);	///< 转发

	public:// 辅助
		bool PreHandleFrameEvent(UINT uMsg,WPARAM wp,LPARAM lp);        ///< 预处理消息,在这里设置面板背景的状态

	public:
		DUIWindow*                                  m_pDUIWnd;			///< 面板项所在的DUI窗口
		IDMItemPanelOwner*                          m_pItemOwner;		///< 拥有面板项的宿主，用于和外部交互、
		LPARAM										m_dwData;			///< 额外数据
		LPARAM										m_dwItemId;         ///< 面板的唯一外部标识
	};

}//namespace DM
