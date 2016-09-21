// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	Events.h
// File mark:   
// File summary: 事件系统整体copy自CEGUI
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-11
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		事件ID
	/// </summary>
	enum EVT_ID
	{
		// DUIProp
		DMEVT_PROP_DELING = 24050,
		DMEVT_PROP_DEL,
		DMEVT_PROP_VALUECHANGED,

		// DUIRect
		DMEVT_DSRECT_CHANGED = 24100,

		// DUISize
		DMEVT_DSIZE_CHANGED  = 24200,

		// DUITreeEx
		DMEVT_DTREE_LOCKCHANGED = 24300,
		DMEVT_DTREE_EYECHANGED,

		// DUIRecentListBox
		DMEVT_DRLISTBOX_DBLCLICK = 24400,
	};

	/// <summary>
	///		DUIPropCtrl删除中事件
	/// </summary>
	class IProp;
	class PropDelingArgs:public DMEventArgs
	{
	public:
		PropDelingArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_bCancel(false),m_pSel(NULL){}
		enum{EventID=DMEVT_PROP_DELING};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_PROP_DELING);}
		IProp*				m_pSel;				///<当前选中项
		bool				m_bCancel;          ///<是否取消当前操作
	};

	/// <summary>
	///		DUIPropCtrl删除事件
	/// </summary>
	class PropDelArgs:public DMEventArgs
	{
	public:
		PropDelArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_pSel(NULL){}
		enum{EventID=DMEVT_PROP_DEL};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_PROP_DEL);}
		IProp*				m_pSel;				///<当前删除选中项
	};

	class PropValueChangedArgs:public DMEventArgs
	{
	public:
		PropValueChangedArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_pSel(NULL){}
		enum{EventID=DMEVT_PROP_VALUECHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_PROP_VALUECHANGED);}
		IProp*				m_pSel;				///<当前选中项
		CStringW            m_strOldValue;
	};

	/// <summary>
	///		当值改变时,DUIRect发送此消息
	/// </summary>
	class DUIRectChangedArgs : public DMEventArgs
	{
	public:
		DUIRectChangedArgs(DUIWindow *pWnd) :DMEventArgs(pWnd){}
		enum{EventID=DMEVT_DSRECT_CHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_DSRECT_CHANGED);}
	};

	/// <summary>
	///		当值改变时,DUIList发送此消息
	/// </summary>
	class DUIListChangedArgs : public DMEventArgs
	{
	public:
		DUIListChangedArgs(DUIWindow *pWnd) :DMEventArgs(pWnd){}
		enum{EventID=DMEVT_DSIZE_CHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_DSIZE_CHANGED);}
	};

	/// <summary>
	///		当LOCK改变后，DUITreeEx发送此消息
	/// </summary>
	class DUITreeExLockChangedArgs :  public DMEventArgs
	{
	public:
		DUITreeExLockChangedArgs(DUIWindow *pWnd) :DMEventArgs(pWnd),m_hItem(NULL),m_iOldLockValue(0){}
		enum{EventID=DMEVT_DTREE_LOCKCHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_DTREE_LOCKCHANGED);}
		HDMTREEITEM                m_hItem; ///< 改变lock的项
		int                        m_iOldLockValue;
	};

	/// <summary>
	///		当EYE改变后，DUITreeEx发送此消息
	/// </summary>
	class DUITreeExEyeChangedArgs :  public DMEventArgs
	{
	public:
		DUITreeExEyeChangedArgs(DUIWindow *pWnd) :DMEventArgs(pWnd),m_hItem(NULL),m_iOldEyeValue(0){}
		enum{EventID=DMEVT_DTREE_EYECHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_DTREE_EYECHANGED);}
		HDMTREEITEM                m_hItem; ///< 改变eye的项
		int                        m_iOldEyeValue;
	};

	/// <summary>
	///		当双击DUIRecentListBox的项时，发送此消息
	/// </summary>
	class DUIRecentListBoxDBLClickArgs :  public DMEventArgs
	{
	public:
		DUIRecentListBoxDBLClickArgs(DUIWindow *pWnd) :DMEventArgs(pWnd){}
		enum{EventID=DMEVT_DRLISTBOX_DBLCLICK};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_DRLISTBOX_DBLCLICK);}
		CStringW                  m_strDir;
	};
}//namespace DM