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
#include "IDmMain/IDMTaskRunner.h"
#include "Modules/Task/DMEventSlotEx.h"
#include "Modules/Task/DMUITaskHandler.h"
#include "Modules/Task/DMWorkTaskHandler.h"


namespace DM
{
	/// <summary>
	///		跨线程事件绑定内置实现
	/// </summary>
	class DMTaskRunnerImpl : public IDMTaskRunner, public DMMapT<CStringA, RefEventSlot>
	{
		DMDECLARE_CLASS_NAME(DMTaskRunnerImpl,"DMEventImpl",DMREG_TaskRunner);
	public:
		DMTaskRunnerImpl();
		~DMTaskRunnerImpl();
	public:
		virtual DMCode InitEvent(DWORD dwUIThreadId);												
		virtual DMCode UninstallEvent();											 
		virtual bool   IsEventExists(LPCSTR lpszEventName);
		virtual DMCode RemoveEvent(LPCSTR lpszEventName);
		virtual DMCode RemoveEventIfThis(void* pThis);												 
		virtual DMCode RemoveAllEvents();			
		virtual DMCode ConnectEvent(DMTaskType tty, LPCSTR lpszEventName, const DMSlot& slot, int group = 100);
		virtual DMCode FireEvent(LPCSTR lpszEventName);
		virtual DMCode FireEvent(LPCSTR lpszEventName, DMBundle& args);
		virtual DMCode PostTask(DMTaskType tty, IDMTaskPtr pTask);
	
	public:
		DMCode ConnectSyncEvent(LPCSTR lpszEventName, const DMSlot& slot, int group = 100);
		DMCode ConnectUiEvent(LPCSTR lpszEventName, const DMSlot& slot, int group = 100);
		DMCode ConnectWorkEvent(LPCSTR lpszEventName, const DMSlot& slot, int group = 100);
		
		DMCode PostSyncTask(IDMTaskPtr pTask);
		DMCode PostUiTask(IDMTaskPtr pTask);
		DMCode PostWorkTask(IDMTaskPtr pTask);

		void AddEvent(LPCSTR lpszEventName);
		RefEventSlot GetEventObject(LPCSTR lpszEventName, bool bAutoAdd = false);
		DMUITaskHandler* GetUiSlotHandler(DWORD uiThreadId);
		bool DispatchUiEvent(DMSlotTask* pSlotTask);
		bool DispatchWorkEvent(const DMSlot& slot, const DMEventSender& sender, const DMBundle& args);
		bool HandleWorkEvent(const DMSlot& slot, const DMEventSender& sender, const DMBundle& args);
		bool InitWorkEnvironment();
		void FireEventImpl(LPCSTR lpszEventName, DMEventSender& sender, DMBundle& args);
		
	public:
	
		// UI
		DMUITaskHandler*						m_uiSlotHandler;

		// work
		bool									m_bInitWorkTaskHandler;
		DMSmartPtrT<DMWorkTaskHandler>          m_spWorkTaskHandler;    
	};

}// namespace DM