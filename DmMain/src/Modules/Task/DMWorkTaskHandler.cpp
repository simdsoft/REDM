#include "DmMainAfx.h"
#include "DMWorkTaskHandler.h"
#include <process.h>

namespace DM
{
	DMWorkTaskHandler::DMWorkTaskHandler()
		: m_hThread(NULL),
		m_hTaskWaitEvent(NULL),
		m_hTaskFinishEvent(NULL),
		m_bExitTaskThread(false),
		m_dwThreadId(0),
		m_bRunning(false),
		m_pThreadLocker(NULL),
		m_dwTimeOut(INFINITE)
	{

	}

	DMWorkTaskHandler::~DMWorkTaskHandler()
	{

	}

	DMCode DMWorkTaskHandler::InitTasks(DWORD dwTimeOut /*= INFINITE*/)
	{
		if (dwTimeOut > 0)
		{
			m_dwTimeOut = dwTimeOut;
		}
		return DM_ECODE_OK;
	}

	DMCode DMWorkTaskHandler::RunTasks()
	{
		unsigned int uTid = 0;
		if (InitTasksThread()) 
		{
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, &DMWorkTaskHandler::s_ThreadProc, this, 0, &uTid);
		}

		return uTid > 0 ? DM_ECODE_OK : DM_ECODE_FAIL;
	}

	DMCode DMWorkTaskHandler::StopTasks()
	{
		bool bNeedTerminateThread = false;
		// 要处理闪退(任务线程还没来及启动，就请求退出)
		if (false ==IsTaskRunning()) 
		{
			::Sleep(50);
			if (NULL != m_hThread && INVALID_HANDLE_VALUE != m_hThread) 
			{
				::Sleep(100);
				bNeedTerminateThread = true;
			}
		}
		bool bSuccess = UnInitTasksThread();
		if (bSuccess && NULL != m_hThread && INVALID_HANDLE_VALUE != m_hThread) 
		{
			DWORD dwIndex = -1;
			bSuccess =(S_OK == ::CoWaitForMultipleHandles(0,5000,1,&m_hThread,&dwIndex));
			DM_CLOSEHANDLE(m_hThread);
		}

#if 1
		if (false == bSuccess || bNeedTerminateThread)
		{
			::OutputDebugStringW(L"[DM][StopTasks]强制退出\n");
			::TerminateProcess(::GetCurrentProcess(),IDOK);
		}
		DM_CLOSEHANDLE(m_hThread);
#endif
		return bSuccess ? DM_ECODE_OK : DM_ECODE_FAIL;
	}

	DMCode DMWorkTaskHandler::AddTask(IDMTaskPtr pTask, bool bExec/*=true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		if (pTask)
		{
			{
				DMAutoLock locker(&m_TaskLocker);
				m_TaskWaitList.AddTail(pTask);
			}
			iErr = bExec ? ExecTask() : DM_ECODE_OK;
		}

		return iErr;
	}

	DMCode DMWorkTaskHandler::ExecTask(bool bSync/*=false*/, DWORD dwTimeOut/*=INFINITE*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_dwThreadId == ::GetCurrentThreadId()) 
			{
                DMFAIL_MSG("task thread can't request service self, will lead dead-lock");
				break;	
			}
			{// 若是空任务就不再执行
				DMAutoLock locker(&m_TaskLocker);
				if (m_TaskWaitList.IsEmpty())
				{
					iErr = DM_ECODE_OK;
					break;
				}
			}
			if (m_hTaskWaitEvent)
			{
				if (::SetEvent(m_hTaskWaitEvent))
				{// 激活Wait事件，使_RunTasksThread执行任务队列
					iErr = DM_ECODE_OK;
				}
			}

			if (bSync && m_hTaskFinishEvent)
			{
				HRESULT hr = E_FAIL;
				DWORD dwIndex = -1;
				hr = ::CoWaitForMultipleHandles(0,dwTimeOut,1,&m_hTaskFinishEvent,&dwIndex);
				if (S_OK == hr)
				{
					iErr = DM_ECODE_OK;
				}
				else
				{
					iErr = DM_ECODE_FAIL;
				}
			}
		} while (false);
		return iErr;
	}

	bool DMWorkTaskHandler::BelongsToThread(DWORD dwThreadId)
	{
		return 0 == m_dwThreadId ? false : dwThreadId == m_dwThreadId;
	}

	bool DMWorkTaskHandler::InitTasksThread()
	{
		bool bRet = false;
		do 
		{
			if (!m_pThreadLocker)
			{
				m_pThreadLocker = new DMLock;
			}

			DMAutoLock lockIt(m_pThreadLocker);
			if (!m_hTaskWaitEvent) 
			{
				m_hTaskWaitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			}

			if (!m_hTaskFinishEvent) 
			{
				m_hTaskFinishEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			}

			if (m_hTaskWaitEvent && m_hTaskFinishEvent) 
			{
				m_bExitTaskThread = false;
				bRet = true;
			}
		} while (false);
		return bRet;
	}

	DWORD DMWorkTaskHandler::RunTasksThread()
	{
		OleInitialize(NULL);// IE需要COM环境
		DWORD dwError = ERROR_SUCCESS;
		m_dwThreadId = ::GetCurrentThreadId();
		m_bRunning = true;
		do 
		{	
			DWORD dwTickStart = ::GetTickCount();
			if (false == DealWithMessage())
			{//1.WM_QUIT退出消息循环
				break;
			}
			GetNewTaskList();// 获得任务队列
			DealWithTaskList();// 执行任务队列
			DWORD dwTimeOut = m_dwTimeOut;
			if (INFINITE != dwTimeOut)
			{
				DWORD dwTick = ::GetTickCount() - dwTickStart;
				if (dwTick > m_dwTimeOut)
				{
					dwTimeOut = 0;
				}
				else
				{
					dwTimeOut -= dwTick;
				}
			}
			dwError = ::MsgWaitForMultipleObjects(1, &m_hTaskWaitEvent, FALSE, dwTimeOut, QS_ALLINPUT);// QS_ALLEVENTS可接受Post消息，An input, WM_TIMER, WM_PAINT, WM_HOTKEY, or posted message is in the queue.
		} while (false == m_bExitTaskThread && WAIT_FAILED != dwError);

		//  Delete task
		POSITION Pos = m_TaskRunList.GetHeadPosition();
		while (Pos)// 循环处理
		{
			IDMTaskPtr& pTask = m_TaskRunList.GetNext(Pos);
			if (pTask)
			{
				pTask->Release();
			}
		}
		m_TaskRunList.RemoveAll();

		// 正常退出流程！！！
		// 关闭任务是否完成信号m_hTaskFinishEvent。
		if (m_hTaskFinishEvent)
		{
			::SetEvent(m_hTaskFinishEvent);
		}
		m_bRunning = false;
		m_dwThreadId = 0;
		return TRUE;
	}

	bool DMWorkTaskHandler::UnInitTasksThread()
	{
		bool bRet = false;
		do 
		{
			m_bExitTaskThread = true;///< 退出线程的另一条件
			if (m_dwThreadId)
			{
				::PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);//唤醒_RunTasksThread
				m_dwTimeOut = 1000;///< 为防止消息丢失,加入超时退出
			}

			// 默认等待3秒。
			bRet = DMSUCCEEDED(ExecTask(true, 3000));
			DestroyTasks();
			DM_DELETE(m_pThreadLocker);
		} while (false);
		return bRet;
	}

	bool DMWorkTaskHandler::IsTaskRunning()
	{
		return m_bRunning;
	}

	bool DMWorkTaskHandler::DealWithMessage()
	{
		MSG msg;

		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			// If it is a quit message, exit.
			if (msg.message == WM_QUIT)  
				return false; 

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

		} // End of PeekMessage while loop.

		return true;
	}

	void DMWorkTaskHandler::GetNewTaskList()
	{
		DMAutoLock lockIt(&m_TaskLocker);
		if (!m_TaskRunList.IsEmpty())
		{// 非空时，清除以前的任务列表
			m_TaskRunList.RemoveAll();
		}
		m_TaskRunList.AddTailList(&m_TaskWaitList);
		m_TaskWaitList.RemoveAll();
	}

	void DMWorkTaskHandler::DealWithTaskList()
	{
		// Run task
		POSITION Pos = m_TaskRunList.GetHeadPosition();
		while (Pos)// 循环处理
		{
			IDMTaskPtr& pTask = m_TaskRunList.GetNext(Pos);
			if (pTask)
			{
				pTask->Run();
			}
		}

		// Delete task
		if (!m_TaskRunList.IsEmpty())
		{
			t_lstTaskQueue clearTaskRunList;
			clearTaskRunList.AddTailList(&m_TaskRunList);
			m_TaskRunList.RemoveAll();

			POSITION Pos = clearTaskRunList.GetHeadPosition();
			while (Pos)// 循环处理
			{
				IDMTaskPtr& pTask = clearTaskRunList.GetNext(Pos);
				if (pTask)
				{
					pTask->Release();
				}
			}
			if (m_hTaskFinishEvent)
			{
				::SetEvent(m_hTaskFinishEvent);
			}
		}
	}

	bool DMWorkTaskHandler::DestroyTasks()
	{
		m_bExitTaskThread = true;
		DMAutoLock lockIt(m_pThreadLocker);
		if (m_hTaskFinishEvent)
		{
			::SetEvent(m_hTaskFinishEvent);
		}
		// 关闭任务执行信号m_hTaskWaitEvent。
		if (m_hTaskWaitEvent)
		{
			::SetEvent(m_hTaskWaitEvent);
		}
		::Sleep(100);
		DM_CLOSEHANDLE(m_hTaskFinishEvent);
		DM_CLOSEHANDLE(m_hTaskWaitEvent);
		return true;
	}

	void DMWorkTaskHandler::OnStart()
	{

	}

	unsigned int WINAPI DMWorkTaskHandler::s_ThreadProc(LPVOID lp)
	{
		DWORD dwResult = 0;
		DMWorkTaskHandler* pThis = static_cast<DMWorkTaskHandler*>(lp);
		if (pThis) 
		{
			pThis->OnStart();
			dwResult = pThis->RunTasksThread();
		}
		return dwResult;
	}

}//namespace DM