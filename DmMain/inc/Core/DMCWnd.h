//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMCWndBase.h 
// File Des: 简单实现CWnd，同时支持trunk窗口过程
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-13	1.0			
//-------------------------------------------------------
#pragma once
namespace DM
{
	/// <summary>
	///		基础thunk、消息分发、窗口函数封装类
	/// </summary>
	class DM_EXPORT DMCWnd:public DMCWndBase		// 封装CWnd
						  ,public DMMsgThunkTool	// trunk工具
						  ,public IDMMsgMap			// 消息分发
	{
	public:
		enum{DMSTATE_DESTROYED = 0x00000001};  
	public:
		DMCWnd();
		virtual ~DMCWnd();

	public:
		/// -------------------------------------------------
		/// @brief			创建窗口
		/// @param[in]		lpClassName		类名
		/// @param[in]		lpWindowName	窗口名
		/// @param[in]		dwStyle			样式
		/// @param[in]		dwExStyle		扩展样式
		/// @param[in]		x				x坐标
		/// @param[in]		y				y坐标
		/// @param[in]		nWidth			宽度
		/// @param[in]		nHeight			高度
		/// @param[in]		hWndParent		父窗口
		/// @param[in]		lpParam			Long pointer to a value to be passed to the window through the CREATESTRUCT structure passed in the lParam parameter the WM_CREATE message	
		/// @return			HWND
		HWND CreateWindowEx(LPCWSTR lpClassName, LPCWSTR lpWindowName,DWORD dwStyle, DWORD dwExStyle, 
			int x, int y, int nWidth, int nHeight, HWND hWndParent, PVOID lpParam); 

		/// -------------------------------------------------
		/// @brief			窗口销毁前处理
		/// @param[in]		hWnd			窗口句柄
		/// @remark         可重载，子类重载处理
		/// @return			无
		virtual void OnFinalMessage(HWND hWnd);		

		BOOL SubclassWindow(HWND hWnd);
		HWND UnsubclassWindow(BOOL bForce = FALSE);

	public:
		DECLARE_MSG_MAP();// 声明ProcessWindowMessage
		static LRESULT CALLBACK FirstWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT DefWindowProc();
		virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		// copy from atlwin
		LRESULT ForwardNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT ReflectNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		static BOOL DefaultReflectionHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	public:
		const MSG*					 m_pCurrentMsg;                            // 仿ATL
		WNDPROC						 m_pfnSuperWindowProc;		
		DWORD						 m_dwState;                                // 是否被destory
		BOOL						 m_bMsgHandled;

		static DMCWnd*				 ms_pThis;
	};

}//namespace DM