// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMTaskRunnerImpl.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-2-26
// ----------------------------------------------------------------
#pragma once
#include "IDMTaskRunner.h"
#include "DMEventSlotEx.h"
#include "DMUITaskHandler.h"
#include "DMWorkTaskHandler.h"


namespace DM
{
	/// <summary>
	///		跨线程事件绑定内置实现
	/// </summary>
	class DMTaskRunnerImpl : public IDMTaskRunner, public DMMapT<CStringW, RefEventSlot>
	{
		DMDECLARE_CLASS_NAME(DMTaskRunnerImpl,L"DMEventImpl",DMREG_TaskRunner);
	public:
		DMTaskRunnerImpl();
		~DMTaskRunnerImpl();
	public:
		virtual DMCode InitEvent(DWORD dwUIThreadId);												
		virtual DMCode UninstallEvent();											 
		virtual bool   IsEventExists(LPCWSTR lpszEventName);										 
		virtual DMCode RemoveEvent(LPCWSTR lpszEventName);											 
		virtual DMCode RemoveEventIfThis(void* pThis);												 
		virtual DMCode RemoveAllEvents();			
		virtual DMCode ConnectEvent(DMTaskType tty, LPCWSTR lpszEventName, const DMSlot& slot, int group = 100);
		virtual DMCode FireEvent(LPCWSTR lpszEventName);						
		virtual DMCode FireEvent(LPCWSTR lpszEventName, DMBundle& args);	
		virtual DMCode PostTask(DMTaskType tty, IDMTaskPtr pTask);
	
	public:
		DMCode ConnectSyncEvent(LPCWSTR lpszEventName, const DMSlot& slot, int group = 100);	
		DMCode ConnectUiEvent(LPCWSTR lpszEventName, const DMSlot& slot, int group = 100);    
		DMCode ConnectWorkEvent(LPCWSTR lpszEventName, const DMSlot& slot, int group = 100); 
		
		DMCode PostSyncTask(IDMTaskPtr pTask);
		DMCode PostUiTask(IDMTaskPtr pTask);
		DMCode PostWorkTask(IDMTaskPtr pTask);

		void AddEvent(LPCWSTR lpszEventName);
		RefEventSlot GetEventObject(LPCWSTR lpszEventName, bool bAutoAdd = false);
		DMUITaskHandler* GetUiSlotHandler(DWORD uiThreadId);
		bool DispatchUiEvent(DMSlotTask* pSlotTask);
		bool DispatchWorkEvent(const DMSlot& slot, const DMEventSender& sender, const DMBundle& args);
		bool HandleWorkEvent(const DMSlot& slot, const DMEventSender& sender, const DMBundle& args);
		bool InitWorkEnvironment();
		void FireEventImpl(LPCWSTR lpszEventName, DMEventSender& sender, DMBundle& args);
		
	public:
	
		// UI
		DMUITaskHandler*						m_uiSlotHandler;

		// work
		bool									m_bInitWorkTaskHandler;
		DMSmartPtrT<DMWorkTaskHandler>          m_spWorkTaskHandler;    
	};

}// namespace DM