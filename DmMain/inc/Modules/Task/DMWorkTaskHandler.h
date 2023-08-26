// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMWorkTaskHandler.h
// File mark:   
// File summary:内置一个线程,遍历处理Task
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-3-1
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		工作线程处理Task的内置实现
	/// </summary>
	class DMWorkTaskHandler : public DMRefNum
	{
	public:
		DMWorkTaskHandler();
		~DMWorkTaskHandler();
	public:
		DMCode InitTasks(DWORD dwTimeOut = INFINITE);		
		DMCode RunTasks();		                                
		DMCode StopTasks();		                                   
		DMCode AddTask(IDMTaskPtr pTask, bool bExec=true);			
		DMCode ExecTask(bool bSync=false, DWORD dwTimeOut=INFINITE);   
		bool BelongsToThread(DWORD dwThreadId);

	public:// 辅助
		bool InitTasksThread();
		DWORD RunTasksThread();
		bool UnInitTasksThread();
		bool IsTaskRunning();

		bool DealWithMessage();
		void GetNewTaskList();
		void DealWithTaskList();
		bool DestroyTasks();

	public:
		virtual void OnStart();
		static unsigned int WINAPI s_ThreadProc(LPVOID lp);

	public:
		HANDLE										m_hThread;				///< 线程句柄
		typedef DM::CList<IDMTaskPtr>				t_lstTaskQueue;
		t_lstTaskQueue								m_TaskWaitList;			///< 待处理的任务队列
		t_lstTaskQueue								m_TaskRunList;			///< 正在处理的任务队列
		DMLock										m_TaskLocker;			///< 任务锁
		DMLock*										m_pThreadLocker;        ///< 线程锁
		HANDLE										m_hTaskWaitEvent;		///< 等待处理任务事件
		HANDLE										m_hTaskFinishEvent;		///< 等待任务处理完成事件
		DWORD										m_dwThreadId;			///< 本线程的ID
		bool										m_bRunning;				///< 是否在运行中
		volatile bool								m_bExitTaskThread;	    ///< 是否退出
		DWORD										m_dwTimeOut;			///< 服务线程超时时间
	};
}//namespace DM