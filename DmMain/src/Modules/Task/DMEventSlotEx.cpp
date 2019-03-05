#include "DmMainAfx.h"
#include "DMEventSlotEx.h"
#include "DMTaskRunnerImpl.h"

namespace DM
{
	/// DMSlotTask--------------------------------
	DMSlotTask::DMSlotTask(const DMSlot& Slot,const DMEventSender& Sender,const DMBundle& Bundle,IDMSlot* pSlot)
		: m_Slot(Slot)
		, m_Sender(Sender)
		, m_Bundle(Bundle)
	{
		m_spSlot = pSlot;
	}

	DMSlotTask::~DMSlotTask()
	{
		m_spSlot.Release();
	}

	void DMSlotTask::Run()
	{
		if (m_spSlot && m_spSlot->Connected()) 
		{
			if (m_Slot)
			{
				m_Slot(m_Sender, m_Bundle);
			}
		}
	}

	/// DMUISlot------------------------------------
	DMUISlot::DMUISlot(const DMSlot& slot,IDMTaskRunner* pOwner) 
		: DMSyncSlot(slot),m_pOwner(pOwner)
	{
		DMASSERT(m_pOwner);
	}

	bool DMUISlot::Invoke(DMEventSender& sender, DMBundle& args)
	{
		DMSlotTask* pSlotTask = new DMSlotTask(GetSlot(), sender, args, this);
		bool bRet = ((DMTaskRunnerImpl*)m_pOwner)->DispatchUiEvent(pSlotTask);
		if (!bRet)
		{
			pSlotTask->Release();
		}
		return bRet;
	}

	DMSyncSlot* DMUISlot::Clone() const
	{
		return new DMUISlot(GetSlot(),m_pOwner);
	}

	/// DMWorkSlot-----------------------------
	DMWorkSlot::DMWorkSlot(const DMSlot& slot, IDMTaskRunner* pOwner)
		: DMSyncSlot(slot),m_pOwner(pOwner)
	{
		DMASSERT(m_pOwner);
	}

	bool DMWorkSlot::Invoke(DMEventSender& sender, DMBundle& args)
	{
		return ((DMTaskRunnerImpl*)m_pOwner)->DispatchWorkEvent(GetSlot(), sender, args);
	}

	DMSyncSlot* DMWorkSlot::Clone() const
	{
		return new DMWorkSlot(GetSlot(),m_pOwner);
	}

}//namespace DM