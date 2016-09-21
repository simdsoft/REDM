#include "DmMainAfx.h"
#include "DMMsgLoop.h"

namespace DM
{
	DMMsgLoop::DMMsgLoop()
	{
		memset(&m_msg, 0, sizeof(MSG));
	}

	DMMsgLoop::~DMMsgLoop()
	{
		if (g_pDMMsgLoopTool->GetMessageLoop() == this)
		{
			DMASSERT(0 == GetRefCount());
			g_pDMMsgLoopTool->RemoveMessageLoop();
		}
	}

	int DMMsgLoop::Run()
	{
		BOOL bDoIdle   = TRUE;
		int nIdleCount = 0;
		BOOL bRet;

		for (;;)
		{
			while (bDoIdle && !::PeekMessage(&m_msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if (!OnIdle(nIdleCount++))
					bDoIdle = FALSE;
			}

			bRet = ::GetMessage(&m_msg, NULL, 0, 0);

			if (bRet == -1)
			{
				continue;   // error, don't process
			}
			else if(!bRet)
			{
				break;   // WM_QUIT, exit message loop
			}

			if (!PreTranslateMessage(&m_msg))
			{
				::TranslateMessage(&m_msg);
				::DispatchMessage(&m_msg);
			}

			if (IsIdleMessage(&m_msg))
			{
				bDoIdle = TRUE;
				nIdleCount = 0;
			}
		}

		return (int)m_msg.wParam;
	}

	BOOL DMMsgLoop::AddMessageFilter(IDMMessageFilter* pMessageFilter)
	{
		BOOL bRet = FALSE;
		do 
		{
			if (NULL == pMessageFilter)
			{
				break;
			}
			POSITION pos = m_MsgFilterList.Find(pMessageFilter);
			if (pos)
			{
				break;
			}

			m_MsgFilterList.AddTail(pMessageFilter);
			bRet = TRUE;
		} while (false);
		return bRet;
	}

	BOOL DMMsgLoop::RemoveMessageFilter(IDMMessageFilter* pMessageFilter)
	{
		BOOL bRet = FALSE;
		do 
		{
			if (NULL == pMessageFilter)
			{
				break;
			}
			POSITION pos = m_MsgFilterList.Find(pMessageFilter);
			if (!pos)
			{
				break;
			}

			m_MsgFilterList.RemoveAt(pos);
			bRet = TRUE;
		} while (false);
		return bRet;
	}

	// Idle handler operations
	BOOL DMMsgLoop::AddIdleHandler(IDMIdleHandler* pIdleHandler)
	{
		BOOL bRet = FALSE;
		do 
		{
			if (NULL == pIdleHandler)
			{
				break;
			}
			POSITION pos = m_IdleHandlerList.Find(pIdleHandler);
			if (pos)
			{
				break;
			}

			m_IdleHandlerList.AddTail(pIdleHandler);
			bRet = TRUE;
		} while (false);
		return bRet;
	}

	BOOL DMMsgLoop::RemoveIdleHandler(IDMIdleHandler* pIdleHandler)
	{
		BOOL bRet = FALSE;
		do 
		{
			if (NULL == pIdleHandler)
			{
				break;
			}
			POSITION pos = m_IdleHandlerList.Find(pIdleHandler);
			if (!pos)
			{
				break;
			}

			m_IdleHandlerList.RemoveAt(pos);
			bRet = TRUE;
		} while (false);
		return bRet;
	}

	BOOL DMMsgLoop::PreTranslateMessage(MSG* pMsg)
	{
		IDMMessageFilterList MsgFilterList; 
		POSITION Pos = m_MsgFilterList.GetHeadPosition();
		while (Pos)// 复制一份，防止原数据被污染
		{
			IDMMessageFilterPtr &t = m_MsgFilterList.GetNext(Pos);
			MsgFilterList.AddTail(t);
		}

		Pos = MsgFilterList.GetHeadPosition();
		while (Pos)// 循环处理
		{
			IDMMessageFilterPtr pMessageFilter = MsgFilterList.GetNext(Pos);
			if (pMessageFilter != NULL && pMessageFilter->PreTranslateMessage(pMsg))
			{
				return TRUE;
			}
		}

		return FALSE;   // not translated
	}

	BOOL DMMsgLoop::OnIdle(int /*nIdleCount*/)
	{
		IDMIdleHandlerList IdleHandlerList; 
		POSITION Pos = m_IdleHandlerList.GetHeadPosition();
		while (Pos)// 复制一份，防止原数据被污染
		{
			IDMIdleHandlerPtr &t = m_IdleHandlerList.GetNext(Pos);
			IdleHandlerList.AddTail(t);
		}

		Pos = IdleHandlerList.GetHeadPosition();
		while (Pos)// 循环处理
		{
			IDMIdleHandlerPtr pIdleHandler = IdleHandlerList.GetNext(Pos);
			if (pIdleHandler)
			{
				pIdleHandler->OnIdle();
			}
		}

		return FALSE;   // not translated
	}

	BOOL DMMsgLoop::IsIdleMessage(MSG* pMsg)
	{
		// These messages should NOT cause idle processing
		switch (pMsg->message)
		{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
		case WM_PAINT:
		case 0x0118:	// WM_SYSTIMER (caret blink)
			return FALSE;
		}

		return TRUE;
	}

	// DMMsgLoopTool-----------------------------------------
	DMMsgLoopTool::DMMsgLoopTool()
	{
		m_dwMainThreadID = ::GetCurrentThreadId();
	}

	DMMsgLoopTool::~DMMsgLoopTool()
	{

	}

	bool DMMsgLoopTool::AddMessageLoop(DMMsgLoop* pMsgLoop)
	{
		DMAutoLock autolock(&m_Lock);
		bool bRet = false;
		do 
		{
			if (NULL == pMsgLoop)
			{
				LOG_ERR("[mid]loop对象指针为空\n");
				break;
			}

			if (IsKeyExist(::GetCurrentThreadId()))
			{
				LOG_ERR("[mid]当前线程消息loop对象已存在\n");
				break;
			}

			bRet = AddKey(::GetCurrentThreadId(), pMsgLoop);
		} while (false);
		return bRet;
	}

	bool DMMsgLoopTool::RemoveMessageLoop()
	{
		DMAutoLock autolock(&m_Lock);

		bool bRet = RemoveKey(::GetCurrentThreadId());

		return bRet;
	}

	DMMsgLoop* DMMsgLoopTool::GetMessageLoop(DWORD dwThreadID /*= ::GetCurrentThreadId()*/) 
	{
		DMAutoLock autolock(&m_Lock);

		DMMsgLoop* pLoop = NULL;
		if (IsKeyExist(dwThreadID)) //因为key不存在，DM::CMap会自动创建，所以先判断key存在
		{
			pLoop = GetObjByKey(dwThreadID);
		}

		return pLoop;
	}

	void DMMsgLoopTool::Term()
	{
		// 整体销毁通知.后续实现
	}

}//namespace DM