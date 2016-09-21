#include "DmMainAfx.h"
#include "DMEventMgr.h"

namespace DM
{

	DMEventItem::DMEventItem(DWORD dwEventID):m_dwEventID(dwEventID)
	{
	}

	DMEventItem::~DMEventItem()
	{
		for (UINT i=0; i<m_EvtSlots.GetCount(); i++)
		{
			delete m_EvtSlots[i];
		}

		m_EvtSlots.RemoveAll();
	}

	bool DMEventItem::Subscribe(const DMSlotFunctorBase&slot)
	{
		if (-1!=FindSlotFunctor(slot))
		{
			return false;
		}

		m_EvtSlots.Add(slot.Clone());

		return true;
	}

	bool DMEventItem::UnSubscribe(const DMSlotFunctorBase&slot)
	{
		int idx = FindSlotFunctor(slot);
		if (-1==idx)
		{
			return false;
		}

		delete m_EvtSlots[idx];
		m_EvtSlots.RemoveAt(idx);
		return true;
	}

	int DMEventItem::FindSlotFunctor(const DMSlotFunctorBase&slot)
	{
		for (UINT i=0; i<m_EvtSlots.GetCount(); i++)
		{
			if (m_EvtSlots[i]->Equal(slot))
			{
				return i;
			}
		}
		return -1;
	}


	//------------------------------DMEventMgr
	DMEventMgr::DMEventMgr(void)
		:m_bMuted(false)
	{
	}

	DMEventMgr::~DMEventMgr()
	{
		RemoveAllEvent();
	}

	void DMEventMgr::AddEvent(const DWORD dwEventID)
	{
		if (!IsEventPresent(dwEventID))
		{
			m_EvtArr.Add(new DMEventItem(dwEventID));
		}
	}

	void DMEventMgr::RemoveEvent(const DWORD dwEventID)
	{
		for (UINT i=0; i<m_EvtArr.GetCount(); i++)
		{
			if (m_EvtArr[i]->GetEventID() == dwEventID)
			{
				delete m_EvtArr[i];
				m_EvtArr.RemoveAt(i);
				return;
			}
		}
	}

	bool DMEventMgr::IsEventPresent(const DWORD dwEventID)
	{
		return GetEventObject(dwEventID) != NULL;
	}

	DMEventItem *DMEventMgr::GetEventObject(const DWORD dwEventID)
	{
		for (UINT i=0; i<m_EvtArr.GetCount(); i++)
		{
			if (m_EvtArr[i]->GetEventID() == dwEventID)
			{
				return m_EvtArr[i];
			}
		}

		return NULL;
	}

	void DMEventMgr::RemoveAllEvent(void)
	{
		for (UINT i=0; i<m_EvtArr.GetCount(); i++)
		{
			delete m_EvtArr[i];
		}
		m_EvtArr.RemoveAll();
	}

	bool DMEventMgr::SubscribeEvent(const DWORD dwEventID, const DMSlotFunctorBase & subscriber)
	{
		if (!IsEventPresent(dwEventID))
		{
			m_EvtArr.Add(new DMEventItem(dwEventID));
		}

		return GetEventObject(dwEventID)->Subscribe(subscriber);

	}

	bool DMEventMgr::UnSubscribeEvent(const DWORD dwEventID, const DMSlotFunctorBase & subscriber)
	{
		if (!IsEventPresent(dwEventID))
		{
			return false;
		}

		return GetEventObject(dwEventID)->UnSubscribe(subscriber);
	}


	void DMEventMgr::FireEvent(DMEventArgs& Args )
	{
		// find event object
		DMEventItem* ev = GetEventObject(Args.GetEventID());

		// fire the event if present and set is not muted
		if ((ev != 0) && !m_bMuted)
		{
			(*ev)(Args);
		}
	}

	bool DMEventMgr::IsMuted()
	{
		return m_bMuted;
	}

	void DMEventMgr::SetMuted(bool bMuted)
	{
		m_bMuted = bMuted;
	}

}//namespace DM