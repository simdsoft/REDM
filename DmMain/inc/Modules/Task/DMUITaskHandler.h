// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMUITaskHandler.h
// File mark:   
// File summary:内置一个消息窗口,遍历处理Task
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-3-1
// ----------------------------------------------------------------
#pragma once
#include "Modules/Task/DMEventSlotEx.h"

namespace DM
{
	/// <summary>
	///		内置一个消息窗口,遍历处理Task
	/// </summary>
	class DMUITaskHandler : public DMCWnd
	{
	public:
		DMUITaskHandler(DWORD dwThreadId);
		void DispatchUiEvent(DMSlotTask* pSlotTask);
		void DispatchUiTask(IDMTaskPtr pTask);

	public:
		static DMUITaskHandler* New(DWORD dwThreadId);
		static void Del(DMUITaskHandler*& refObj);

	public:
		BEGIN_MSG_MAPT(DMUITaskHandler)
			MESSAGE_HANDLER_EX(m_MsgId, OnHandlerEvent)
			MESSAGE_HANDLER_EX(m_TaskMsgId, OnHandlerEvent)
		END_MSG_MAP()
	public:
		LRESULT OnHandlerEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		const UINT							m_MsgId;
		const UINT                          m_TaskMsgId;
		DWORD								m_dwThreadId;
	};
}//namespace DM