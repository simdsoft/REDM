#include "DmMainAfx.h"
#include "DMEventSlot.h"

namespace DM
{
	int g_NextSlotId = 0;
	DMSyncSlot::DMSyncSlot(const DMSlot& slot)
		: m_Slot(slot)
		, m_SlotId(++g_NextSlotId)
		, m_bMuted(false)
	{
	}

	bool DMSyncSlot::operator==(const DMSyncSlot& src) const
	{
		return m_Slot == src.m_Slot;
	}

	DMSyncSlot::~DMSyncSlot()
	{
	}

	bool DMSyncSlot::Connected() const
	{
		return !m_Slot.empty();
	}

	void DMSyncSlot::Disconnect()
	{
		m_Slot.clear();
	}

	bool DMSyncSlot::IsMuted(void) const
	{
		return m_bMuted;
	}

	void DMSyncSlot::SetMuted(bool bMuted)
	{
		m_bMuted = bMuted;
	}

	bool DMSyncSlot::CompareIfThis(void* pThis) const
	{
		class SlotThis : public DM::DelegateMemento
		{
		public:
			SlotThis(const DM::DelegateMemento& self) : DM::DelegateMemento(self) {}
			void* This() const {return this->m_pthis;}
		};
		SlotThis pThisMem(const_cast<DMSlot&>(m_Slot).GetMemento());
		if (pThisMem.This() == pThis) 
		{
			return true;
		}
		return false;
	}

	const DMSlot DMSyncSlot::GetSlot() const
	{
		return m_Slot;
	}

	bool DMSyncSlot::Invoke(DMEventSender& sender, DMBundle& args)
	{
		if (m_Slot)
		{
			return m_Slot(sender, args);
		}
		return false;
	}

	DMSyncSlot* DMSyncSlot::Clone() const
	{
		return new DMSyncSlot(m_Slot);
	}

	///DMEventSlot---------------------------------------------------------------------------------------------
	DMEventSlot::DMEventSlot(LPCSTR lpszEventName) : m_strEventName(lpszEventName)
	{
	}

	void DMEventSlot::operator()(DMEventSender& sender, DMBundle& args)
	{
		POSITION pos = m_slotContainer.GetStartPosition();
		while(pos)
		{
			DM::CMap<int, DM::CArray<Connection>>::CPair *p = m_slotContainer.GetNext(pos);
			int iSize = (int)p->m_value.GetCount();
			for (int i=0; i<iSize; i++)
			{
				Connection copySlot = p->m_value[i];
				if (copySlot && false == copySlot->IsMuted()) 
				{
					copySlot->Invoke(sender, args);
				}
			}
		}
	}

	DMEventSlot::~DMEventSlot()
	{
		POSITION pos = m_slotContainer.GetStartPosition();
		while(pos)
		{
			DM::CMap<int, DM::CArray<Connection>>::CPair *p = m_slotContainer.GetNext(pos);
			int iSize = (int)p->m_value.GetCount();
			for (int i=0; i<iSize; i++)
			{
				Connection copySlot = p->m_value[i];
				if (copySlot) 
				{
					copySlot->Disconnect();
				}
			}
		}
		m_slotContainer.RemoveAll();
	}

	const CStringA& DMEventSlot::GetName(void) const
	{ 
		return m_strEventName; 
	}

	bool DMEventSlot::EmptySlot(void) const
	{
		return m_slotContainer.IsEmpty();
	}

	DMEventSlot::Connection DMEventSlot::ConnectSlot(const DMSyncSlot& slot, Group group/* = -1*/)
	{
		DMEventSlot::Connection _conn; _conn.Attach(slot.Clone());

		DM::CArray<Connection> &connection = m_slotContainer[group];
		int iSize = (int)connection.GetCount();
		bool bFind = false;
		for (int i=0; i<iSize; i++)
		{
			if (_conn->m_Slot == connection[i]->m_Slot)
			{
				bFind = true;
				break;
			}
		}
		if (false == bFind)
		{
			m_slotContainer[group].Add(_conn);
		}
		return _conn;
	}

	bool DMEventSlot::DisconnectIfThis(void* pThis)
	{
		bool bRet = false;

		POSITION pos = m_slotContainer.GetStartPosition();
		while(pos)
		{
			DM::CMap<int, DM::CArray<Connection>>::CPair *p = m_slotContainer.GetNext(pos);
			int iSize = (int)p->m_value.GetCount();
			DM::CArray<int> RemoveArray; // 统一
			for (int i=0; i<iSize; i++)
			{
				Connection copySlot = p->m_value[i];
				if (copySlot && copySlot->CompareIfThis(pThis))
				{
					copySlot->Disconnect();	// 先断开事件，再移除
					RemoveArray.Add(i);
					bRet = true;
				}
			}

			int iRemoveSize = (int)RemoveArray.GetCount();
			for (int j=iRemoveSize-1; j>=0; j--)// 从后向前移除
			{
				p->m_value.RemoveAt(RemoveArray[j]);
			}

			if (p->m_value.IsEmpty())// 为空就从Map中移除
			{
				m_slotContainer.RemoveKey(p->m_key);// 自测确认这样直接移除对m_slotContainer整体遍历没有影响
			}
		}
		return bRet;
	}

}//namespace DM
