// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMEventSlot.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-3-2
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		槽扩展接口
	/// </summary>
	interface IDMSlot : public DMRefNum
	{
		virtual bool Connected() const = 0;
	};

	/// <summary>
	///		同步槽,同步调用槽函数
	/// </summary>
	class DMSyncSlot : public IDMSlot
	{
	public:
		DMSyncSlot(const DMSlot& slot);
		bool operator==(const DMSyncSlot& src) const;
		virtual ~DMSyncSlot();

	public:
		bool Connected() const;
		void Disconnect();
		bool IsMuted() const;
		void SetMuted(bool bMuted);
		bool CompareIfThis(void* pThis) const;	
		const DMSlot GetSlot() const;

	public:
		virtual bool Invoke(DMEventSender& sender, DMBundle& args);
		virtual DMSyncSlot* Clone() const;

	public:
		bool									m_bMuted;		///< 为true时不调用事件
		int										m_SlotId;		///< 槽Id号
		DMSlot									m_Slot;		    ///< 槽实现体
	};


	/// <summary>
	///		事件槽,事件名称---同步/异步/UI槽
	/// </summary>
	class DMEventSlot : public DMRefNum
	{
	public:
		typedef DMSmartPtrT<DMSyncSlot>		   Connection;
		typedef	int							   Group;
	public:
		DMEventSlot(LPCSTR lpszEventName);
		void operator()(DMEventSender& sender, DMBundle& args);
		~DMEventSlot();

	public:
		const CStringA& GetName(void) const;
		bool EmptySlot(void) const;
		Connection ConnectSlot(const DMSyncSlot& slot, Group group = -1);
		bool DisconnectIfThis(void* pThis);

	public:
		CStringA								m_strEventName;					   ///< 事件名
		DM::CMap<int, DM::CArray<Connection>>   m_slotContainer;                   ///< 事件槽容器，组别越大越优先调用,MAP会自动把数字按从小到大排序
	};
	typedef DMSmartPtrT<DMEventSlot>			RefEventSlot;

}//namespace DM
