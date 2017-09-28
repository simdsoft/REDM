//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMDropWnd.h 
// File Des: 
// File Summary:  
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-2	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class DMDropWnd;
	/// <summary>
	///		下拉真实窗口的拥有者
	/// </summary>
	class DM_EXPORT IDMDropOwner
	{
	public:
		virtual DUIWindow* GetOwnerWindow() = 0;                            
		virtual DMCode OnDropDown(DMDropWnd *pDropDown) = 0;               ///< 指示列表框正处于下拉状态
		virtual DMCode OnCloseUp(DMDropWnd *pDropDown,UINT uCode) = 0;     ///< 指示列表框已关闭
	};
	/// <summary>
	///		下拉真实窗口
	/// </summary>
	class DM_EXPORT DMDropWnd: public DMHWnd, public IDMMessageFilter
	{
	public:
		DMDropWnd(IDMDropOwner* pOwner);
		virtual ~DMDropWnd();

	public: 
		virtual DMCode Show(LPCRECT lpRect);
		virtual DMCode Hide(UINT uCode = IDCANCEL);
		virtual DMCode Create(LPCRECT lpRect,DWORD dwStyle=WS_POPUP,DWORD dwExStyle=WS_EX_TOOLWINDOW|WS_EX_TOPMOST);

	public:
		//---------------------------------------------------
		// Function Des: 消息分发系列函数
		//---------------------------------------------------
		LRESULT OnNcCreate(LPCREATESTRUCT lpCreateStruct);	
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags, CPoint point);
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);   
		void OnKillFocus(HWND wndFocus);
		void OnDestroy();
		void OnActivate(UINT nState, BOOL bMinimized, HWND wndOther){}// 中断消息处理，防止设置焦点
		void OnActivateApp(BOOL bActive, DWORD dwThreadID);
		int OnMouseActivate(HWND wndTopLevel, UINT nHitTest, UINT message);

	public:
		DECLARE_MSG_MAP()										// 仿MFC消息映射宏

	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual void OnFinalMessage(HWND);
		virtual DMCode OnReleaseCaptureWnd();

	public:// 辅助
		DMCode PreMsg();
		DMCode UnPreMsg();

	public:
		IDMDropOwner*				    m_pOwner; 
		bool							m_bClick; 
		bool                            m_bShow;
		UINT							m_uExitCode; 
	};


}//namespace DM