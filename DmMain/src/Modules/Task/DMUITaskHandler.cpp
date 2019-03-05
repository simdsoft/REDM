#include "DmMainAfx.h"
#include "DMUITaskHandler.h"

namespace DM
{
	DMUITaskHandler::DMUITaskHandler(DWORD dwThreadId): m_MsgId(WM_USER + 1768), m_TaskMsgId(WM_USER + 1769), m_dwThreadId(dwThreadId)
	{
	}

	void DMUITaskHandler::DispatchUiEvent(DMSlotTask* pSlotTask)
	{
		if (NULL != pSlotTask)
		{
			if (m_hWnd != NULL)
			{
				this->PostMessage(m_MsgId, (WPARAM)pSlotTask, 0);
			}
			else
			{
				pSlotTask->Release();
			}
		}
	}

	void DMUITaskHandler::DispatchUiTask(IDMTaskPtr pTask)
	{
		if (pTask)
		{
			if (m_hWnd != NULL)
			{
				this->PostMessage(m_TaskMsgId,(WPARAM)pTask,0);
			}
			else
			{
				pTask->Release();
			}
		}
	}

	DMUITaskHandler* DMUITaskHandler::New(DWORD dwThreadId)
	{
		DMUITaskHandler* obj = new DMUITaskHandler(dwThreadId);
		if (obj)
		{
			ATOM Atom = g_pDMApp->GetClassAtom();
			if (obj->DMCWnd::CreateWindowEx((LPCWSTR)Atom,NULL,0,0,0,0,1,1,HWND_MESSAGE,0))
			{
				return obj;
			}
			else 
			{
				obj->m_dwThreadId = 0;
			}
		}
		return NULL;
	}

	void DMUITaskHandler::Del(DMUITaskHandler*& refObj)
	{
		if (refObj)
		{
			if (refObj->IsWindow())
			{
				if (::GetCurrentThreadId () != refObj->m_dwThreadId)
				{
					DMASSERT_EXPR(FALSE, L"强烈建议UninstallEvent在主UI线程中执行！！");
					refObj->PostMessage(WM_CLOSE);
					Sleep(1000);
				}
				else
				{
					refObj->DestroyWindow();
				}
			}
			delete refObj;
		}
	}

	LRESULT DMUITaskHandler::OnHandlerEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (m_MsgId == uMsg)
		{
			DMSlotTask* pSlotTask =  reinterpret_cast<DMSlotTask*>(wParam);
			if (pSlotTask)
			{
				pSlotTask->Run();
				pSlotTask->Release();
			}
		}

		if (m_TaskMsgId == uMsg)
		{
			IDMTaskPtr pTask =  reinterpret_cast<IDMTaskPtr>(wParam);
			if (pTask)
			{
				pTask->Run();
				pTask->Release();
			}
		}

		return S_OK;
	}
}//namespace DM