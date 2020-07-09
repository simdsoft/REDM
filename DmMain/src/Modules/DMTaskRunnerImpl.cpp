#include "DmMainAfx.h"
#include "DMTaskRunnerImpl.h"

namespace DM
{
	DMTaskRunnerImpl::DMTaskRunnerImpl()
		: m_bInitWorkTaskHandler(false)
		, m_uiSlotHandler(NULL)
	{

	}

	DMTaskRunnerImpl::~DMTaskRunnerImpl()
	{
		if (m_bInitWorkTaskHandler)
		{
			m_spWorkTaskHandler->StopTasks();
			m_bInitWorkTaskHandler = false;
		}
		if (m_uiSlotHandler)
		{
			DMUITaskHandler::Del(m_uiSlotHandler);
			m_uiSlotHandler = NULL;
		}
	}

	DMCode DMTaskRunnerImpl::InitEvent(DWORD dwUIThreadId)
	{
		return (0 != dwUIThreadId && GetUiSlotHandler(dwUIThreadId)) ? DM_ECODE_OK : DM_ECODE_FAIL;
	}

	DMCode DMTaskRunnerImpl::UninstallEvent()
	{
		if (m_uiSlotHandler)
		{
			DMUITaskHandler::Del(m_uiSlotHandler);
			m_uiSlotHandler = NULL;
		}
		if (m_spWorkTaskHandler && m_spWorkTaskHandler->BelongsToThread(::GetCurrentThreadId()))
		{
			DMASSERT_EXPR(FALSE, L"亲,在Work线程执行UninstallEvent是不会关掉Task线程的!");
		}
		else
		{
			if (m_bInitWorkTaskHandler)
			{
				m_spWorkTaskHandler->StopTasks();
				m_bInitWorkTaskHandler = false;
			}
		}

		return DM_ECODE_OK;
	}

	bool DMTaskRunnerImpl::IsEventExists(LPCSTR lpszEventName)
	{
		if (IsValidString(lpszEventName))
		{
			return IsKeyExist(lpszEventName);
		}
		return false;
	}

	DMCode DMTaskRunnerImpl::RemoveEvent(LPCSTR lpszEventName)
	{
		if (IsValidString(lpszEventName))
		{
			return (true == RemoveKey(lpszEventName)) ? DM_ECODE_OK : DM_ECODE_FAIL;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMTaskRunnerImpl::RemoveEventIfThis(void* pThis)
	{
		POSITION pos = m_Map.GetStartPosition();
		while(pos)
		{
			DM::CMap<CStringA, RefEventSlot>::CPair *p = m_Map.GetNext(pos);
			RefEventSlot refEventSlot = p->m_value;
			if (refEventSlot.isValid())
			{
				refEventSlot->DisconnectIfThis(pThis);
			}
		}
		return DM_ECODE_OK;
	}

	DMCode DMTaskRunnerImpl::RemoveAllEvents()
	{
		RemoveAll();

		return DM_ECODE_OK;
	}

	DMCode DMTaskRunnerImpl::ConnectEvent(DMTaskType tty, LPCSTR lpszEventName, const DMSlot& slot, int group /*= 100*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		switch (tty)
		{
		case DMTT_SYNC:		iErr = ConnectSyncEvent(lpszEventName, slot, group);break;
		case DMTT_ASYN_UI:	iErr = ConnectUiEvent(lpszEventName, slot, group);	break;
		case DMTT_ASYN_WORK:iErr = ConnectWorkEvent(lpszEventName, slot, group);break;
		default:break;
		}
		return iErr;
	}

	DMCode DMTaskRunnerImpl::FireEvent(LPCSTR lpszEventName)
	{
		if (IsValidString(lpszEventName))
		{
			DMEventSender sender;
			DMBundle args;
			FireEventImpl(lpszEventName, sender, args);
			return DM_ECODE_OK;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMTaskRunnerImpl::FireEvent(LPCSTR lpszEventName, DMBundle& args)
	{
		if (IsValidString(lpszEventName))
		{
			DMEventSender sender;
			FireEventImpl(lpszEventName, sender, args);
			return DM_ECODE_OK;
		}
		return DM_ECODE_FAIL;
	}


	DMCode DMTaskRunnerImpl::PostTask(DMTaskType tty, IDMTaskPtr pTask)
	{
		DMCode iErr = DM_ECODE_FAIL;
		switch (tty)
		{
		case DMTT_SYNC:		iErr = PostSyncTask(pTask);	break;
		case DMTT_ASYN_UI:	iErr = PostUiTask(pTask);	break;
		case DMTT_ASYN_WORK:iErr = PostWorkTask(pTask);	break;
		default:break;
		}
		return iErr;
	}

	DMCode DMTaskRunnerImpl::ConnectSyncEvent(LPCSTR lpszEventName, const DMSlot& slot, int group /*= 100*/)
	{
		if (IsValidString(lpszEventName))
		{
			DMSmartPtrT<DMSyncSlot> spSlot;spSlot.Attach(new DMSyncSlot(slot));
			RefEventSlot RefEventSlot = GetEventObject(lpszEventName, true);
			DMEventSlot::Connection connect = RefEventSlot->ConnectSlot(*spSlot, DMABS(group));
			return connect->Connected() ? DM_ECODE_OK : DM_ECODE_FAIL;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMTaskRunnerImpl::ConnectUiEvent(LPCSTR lpszEventName, const DMSlot& slot, int group /*= 100*/)
	{
		if (IsValidString(lpszEventName))
		{

			DMSmartPtrT<DMUISlot> spUISlot;spUISlot.Attach(new DMUISlot(slot,this));
			RefEventSlot RefEventSlot = GetEventObject(lpszEventName, true);
			DMEventSlot::Connection connect = RefEventSlot->ConnectSlot(*spUISlot, DMABS(group));
			return connect->Connected() ? DM_ECODE_OK : DM_ECODE_FAIL;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMTaskRunnerImpl::ConnectWorkEvent(LPCSTR lpszEventName, const DMSlot& slot, int group /*= 100*/)
	{
		if (IsValidString(lpszEventName))
		{
			DMSmartPtrT<DMWorkSlot> spSlot;spSlot.Attach(new DMWorkSlot(slot,this));
			RefEventSlot RefEventSlot = GetEventObject(lpszEventName, true);
			DMEventSlot::Connection connect = RefEventSlot->ConnectSlot(*spSlot, DMABS(group));
			return connect->Connected() ? DM_ECODE_OK : DM_ECODE_FAIL;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMTaskRunnerImpl::PostSyncTask(IDMTaskPtr pTask)
	{
		if (pTask)
		{
			pTask->Run();
			pTask->Release();
			return DM_ECODE_OK;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMTaskRunnerImpl::PostUiTask(IDMTaskPtr pTask)
	{
		if (pTask)
		{
			if (m_uiSlotHandler) 
			{
				m_uiSlotHandler->DispatchUiTask(pTask);
				return DM_ECODE_OK;
			}
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMTaskRunnerImpl::PostWorkTask(IDMTaskPtr pTask)
	{
		if (pTask)
		{
			if (InitWorkEnvironment())
			{
				return m_spWorkTaskHandler->AddTask(pTask, true);
			}
		}
		return DM_ECODE_FAIL;
	}

	void DMTaskRunnerImpl::AddEvent(LPCSTR lpszEventName)
	{
		if (IsValidString(lpszEventName) && !IsEventExists(lpszEventName)) 
		{
			RefEventSlot pEventSlot; pEventSlot.Attach(new DMEventSlot(lpszEventName));
			AddKey(lpszEventName,pEventSlot);
		}
	}

	RefEventSlot DMTaskRunnerImpl::GetEventObject(LPCSTR lpszEventName, bool bAutoAdd /*= false*/)
	{
		if (IsValidString(lpszEventName))
		{
			if (!IsEventExists(lpszEventName))
			{
				if (bAutoAdd)
				{
					AddEvent(lpszEventName);
					return m_Map[lpszEventName];
				}
			}
			else
			{
				return m_Map[lpszEventName];
			}
		}
		return RefEventSlot();
	}

	DMUITaskHandler* DMTaskRunnerImpl::GetUiSlotHandler(DWORD uiThreadId)
	{
		if (0 == uiThreadId)
		{
			return NULL;
		}
		if (NULL == m_uiSlotHandler)
		{
			m_uiSlotHandler	= DMUITaskHandler::New(uiThreadId);
		}
		return m_uiSlotHandler;
	}

	bool DMTaskRunnerImpl::DispatchUiEvent(DMSlotTask* pSlotTask)
	{
		if (m_uiSlotHandler) 
		{
			m_uiSlotHandler->DispatchUiEvent(pSlotTask);
			return true;
		}
		return false;
	}

	bool DMTaskRunnerImpl::DispatchWorkEvent(const DMSlot& slot, const DMEventSender& sender, const DMBundle& args)
	{
		if (InitWorkEnvironment())
		{
			return DMSUCCEEDED(m_spWorkTaskHandler->AddTask(DMNewRunnableMethod(&DMTaskRunnerImpl::HandleWorkEvent, this, slot, sender, args), true));
		}
		return false;
	}

	bool DMTaskRunnerImpl::HandleWorkEvent(const DMSlot& slot, const DMEventSender& sender, const DMBundle& args)
	{
		return slot(sender, args);
	}

	bool DMTaskRunnerImpl::InitWorkEnvironment()
	{
		if (false == m_bInitWorkTaskHandler) 
		{
			if (m_spWorkTaskHandler.isNull())
			{
				m_spWorkTaskHandler = new DMWorkTaskHandler();
				m_spWorkTaskHandler->InitTasks();
			}
			if (m_spWorkTaskHandler.isValid())
			{
				m_bInitWorkTaskHandler = DMSUCCEEDED(m_spWorkTaskHandler->RunTasks());
			}
		}
		return m_bInitWorkTaskHandler;
	}

	void DMTaskRunnerImpl::FireEventImpl(LPCSTR lpszEventName, DMEventSender& sender, DMBundle& args)
	{
		RefEventSlot refEventSlot = GetEventObject(lpszEventName);
		if (refEventSlot.isValid()) 
		{
			refEventSlot->operator()(sender, args);
		}
	}

}// namespace DM