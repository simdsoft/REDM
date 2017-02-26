//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMMsg.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-3	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		所有真实窗口消息分发的基类
	/// </summary>
	class DM_EXPORT IDMMsgMap
	{
		// 为直接用atl的消息宏，不做返回值改变
		virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
			LRESULT& lResult, DWORD dwMsgMapID) = 0;
	};

	/// <summary>
	///		消息过滤基类
	/// </summary>
	/// <remarks>
	///		GetMessage之后，先调用PreTranslateMessage来处理，如果返回TRUE，消息传递结束!
	/// </remarks>
	class  DM_EXPORT IDMMessageFilter
	{
	public://
		IDMMessageFilter(){m_dwThreadId = ::GetCurrentThreadId();}
		virtual BOOL PreTranslateMessage(MSG* pMsg) = 0;
	public:
		DWORD              m_dwThreadId;
	};
	typedef IDMMessageFilter* IDMMessageFilterPtr;

	/// <summary>
	///		空闲处理基类
	/// </summary>
	class IDMIdleHandler
	{
	public:
		IDMIdleHandler(){m_dwThreadId = ::GetCurrentThreadId();}
		virtual BOOL OnIdle() = 0;
	public:
		DWORD              m_dwThreadId;
	};
	typedef IDMIdleHandler* IDMIdleHandlerPtr;
}