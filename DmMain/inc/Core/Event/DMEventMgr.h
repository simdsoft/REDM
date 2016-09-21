//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMEventManager.h 
// File Des:  cegui的观察者事件注册模式，可参看doc
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-11-12	1.0			
//--------------------------------------------------------
#pragma once
#include "DMEventSubscriber.h"

namespace DM
{
	/// <summary>
	///		事件项，每个事件项管理多个待处理的注册函数
	/// </summary>
	class DM_EXPORT DMEventItem
	{
	public:
		DMEventItem(DWORD dwEventID);
		virtual~DMEventItem();

		DWORD GetEventID(){return m_dwEventID;};		   ///< 获取事件ID
		bool Subscribe(const DMSlotFunctorBase&slot);	   ///< 注册事件
		bool UnSubscribe(const DMSlotFunctorBase&slot);	   ///< 反注册事件
		int FindSlotFunctor(const DMSlotFunctorBase&slot);  

		void operator()(DMEventArgs& Args)
		{
			for (UINT i=0; i<m_EvtSlots.GetCount(); i++)
			{
				if ((*m_EvtSlots[i])(&Args))//循环执行注册函数
				{
					++Args.m_iHandleCount;
				}
			}
		}

	protected:
		DWORD							    m_dwEventID;
		DM::CArray<DMSlotFunctorBase*>		m_EvtSlots;
	};

	/// <summary>
	///		管理事件项，每个事件项管理多个待处理的注册函数
	/// </summary>
	class DM_EXPORT DMEventMgr
	{
	public:
		DMEventMgr(void);
		virtual ~DMEventMgr(void);

		void AddEvent(const DWORD dwEventID);		///< 增加注册ID
		void RemoveEvent(const DWORD dwEventID);	///< 移除注册ID
		bool IsEventPresent(const DWORD dwEventID); ///< 注册ID是否存在
		void RemoveAllEvent(void);					///< 移除所有的注册ID

		bool SubscribeEvent(const DWORD dwEventID, const DMSlotFunctorBase & subscriber);	///< 给ID增加注册事件
		bool UnSubscribeEvent(const DWORD dwEventID, const DMSlotFunctorBase & subscriber); ///< 给ID反注册事件

		void FireEvent(DMEventArgs& Args);			///< 事件发生时通知所有注册的观察者

		bool IsMuted();								///< 是否启用事件过滤
		void SetMuted(bool bMuted);					///< 启用、关闭事件过滤，事件过滤开启后，不会执行注册函数
	protected:
		DMEventItem *GetEventObject(const DWORD dwEventID);

	protected:
		DM::CArray<DMEventItem *>					m_EvtArr;
		bool										m_bMuted;
	};
	typedef DMEventMgr* DMEventMgrPtr;
}//namespace DM