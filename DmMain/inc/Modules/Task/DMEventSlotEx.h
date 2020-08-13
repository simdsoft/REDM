// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMEventSlotEx.h
// File mark:   
// File summary:DMEventSlot的扩展
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-2-27
// ----------------------------------------------------------------
#pragma once
#include "IDmMain/IDMTaskRunner.h"
#include "Modules/Task/DMEventSlot.h"
#include "Common/Template/DMTaskT.h"

namespace DM
{
	/// <summary>
	///		将槽转换成Task
	/// </summary>
	class DMSlotTask : public IDMTask
	{
	public:
		DMSlotTask(const DMSlot& Slot,const DMEventSender& Sender,const DMBundle& Bundle,IDMSlot* pSlot);
		~DMSlotTask();

		void Run();
	private:
		DMSmartPtrT<IDMSlot>					m_spSlot;
		DMSlot									m_Slot;
		DMEventSender							m_Sender;
		DMBundle								m_Bundle;
	};


	/// <summary>
	///		UI槽
	/// </summary>
	class DMUISlot : public DMSyncSlot
	{
	public:
		DMUISlot(const DMSlot& slot,IDMTaskRunner* pOwner);

	public:
		virtual bool Invoke(DMEventSender& sender, DMBundle& args);
		virtual DMSyncSlot* Clone() const;

	public:
		IDMTaskRunner*                     m_pOwner;
	};

	/// <summary>
	///		异步槽
	/// </summary>
	class DMWorkSlot : public DMSyncSlot
	{
	public:
		DMWorkSlot(const DMSlot& slot, IDMTaskRunner* pOwner);

	public:
		virtual bool Invoke(DMEventSender& sender, DMBundle& args);
		virtual DMSyncSlot* Clone() const;

	public:
		IDMTaskRunner*                     m_pOwner;
	};

}//namespace DM