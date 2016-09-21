//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDUIRealWnd.h 
// File Des: 真实窗口基类
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-4	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{	
	/// <summary>
	///		此为RealWnd的接口类，外部需实现接口，并注册进来
	/// </summary>
	class DM_EXPORT IDUIRealWnd:public DUIWindow	
	{
	public:
		//---------------------------------------------------
		// Function Des: 需外部实现的扩展接口
		//---------------------------------------------------
		virtual HWND OnCreateRealWnd(LPCWSTR atom,HWND hWndParent) = 0;			///< 外部创建窗口，可为child也可为pupop		
		virtual DMCode OnDestoryRealWnd() = 0;									///< 外部销毁窗口，
		virtual DMCode OnShowRealWnd(BOOL bShow, UINT nStatus) = 0;		        
		virtual DMCode OnIsPopupWnd() = 0;                                      ///< 是否为child窗口
		virtual DMCode OnMoveRealWnd(LPRECT lpRect) = 0;						///< 同步移动窗口，lpRect在pupop时仍为相对于父窗口坐标，需子窗口自己转换
	
	public:
		IDUIRealWnd();

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_CREATE(OnCreate)
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_SIZE(OnSize)
			MSG_WM_SHOWWINDOW(OnShowWindow)
		DM_END_MSG_MAP()

	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		int OnCreate(LPVOID);									// MSDN: 不为0则发送Destory消息
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnDestroy();
		void OnSize(UINT nType, CSize size);
		void OnShowWindow(BOOL bShow, UINT nStatus);
		void OnMove(CPoint pt);

	public:
		bool DV_IsStateChangeRedraw(){return false;}

	public:// 辅助
		void MoveRealWnd();
		void CatchRealWndScreen(IDMCanvas* pCanvas);

	public:
		HWND                              m_hRealWnd;
	};

}// namespace DM