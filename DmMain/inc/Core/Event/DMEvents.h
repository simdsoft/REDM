﻿//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMEvents.h 
// File Des: 事件系统整体copy自CEGUI
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-3	1.0			
//--------------------------------------------------------
#pragma once

namespace DM
{
	class DUIWindow;
	/// <summary>
	///		事件基类,所有需注册的新事件必须先创建一个子类继承于它，并分配一个新的事件ID
	/// </summary>
	class DM_EXPORT DMEventArgs
	{
	public:
		DMEventArgs(DUIWindow *pSender);
		virtual ~DMEventArgs();
		virtual bool IsValid();								  // 判断是否合法，可发出去
		virtual UINT GetEventID() = 0;
		virtual LPCSTR GetEventName(){return NULL;};

	public:
		int								     m_iHandleCount;  ///< 当事件被处理时，m_iHandleCount增加
		int									 m_IdFrom;        ///< 事件发送者ID
		LPCSTR								 m_szNameFrom;    ///< 事件发送者name
		DUIWindow*							 m_pSender;       ///< 产生事件的原始窗口对象
	};

	/// <summary>
	///		事件ID
	/// </summary>
	enum DMEVT_ID
	{
		DMEVT_CMD			   = 10000,	
		DMEVT_HOVERCMD,
		DMEVT_LEAVECMD,
		DMEVT_SIZECHANGED,
		DMEVT_CHECKCHANGING,
		DMEVT_CHECKCHANGED,
		DMEVT_RBUTTONMENU,

		DMEVT_TAB_SELCHANGING  = 11000,			
		DMEVT_TAB_SELCHANGED,
		DMEVT_TAB_DELING,
		DMEVT_TAB_DELETED,

		
		DMEVT_TAB_3DVIEW       = 12000,

		DMEVT_SCROLLBAR        = 13000,

		DMEVT_RENOTIFY         = 14000,
		DMEVT_REWANTRETURN,

		DMEVT_SETFOCUS         = 15000,
		DMEVT_KILLFOCUS,

		DMEVT_LB_SELCHANGING   = 16000,
		DMEVT_LB_SELCHANGED,
		DMEVT_LB_GETDISPINFO,

		DMEVT_OFPANEL          = 17000,
		DMEVT_OFPANELCLICK     = 17001,
		DMEVT_OFPANELRCLICK    = 17002,

		DMEVT_CB_SELCHANGE     = 18000,

		DMEVT_TC_SELCHANGING   = 19000,
		DMEVT_TC_SELCHANGED,
		DMEVT_TC_CHECKSTATE,
		DMEVT_TC_EXPAND,
		DMEVT_TC_DBCLICK,

		DMEVT_HEADER_CLICK	   = 20000,
		DMEVT_HEADER_ITEMCHANGING,
		DMEVT_HEADER_ITEMCHANGED,
		DMEVT_HEADER_ITEMSWAP,
		
		DMEVT_LC_GETDISPINFO   = 21000,
		DMEVT_LC_SELCHANGING,
		DMEVT_LC_SELCHANGED,
		DMEVT_LC_ITEMDELETED,

		DMEVT_SD_POSCHANG      = 22000,
		DMEVT_SD_POSCHANGED,

		DMEVT_SC_ONSCROLL      = 22100,

		DMEVT_CL_SETDATE       = 22150,
		DMEVT_CL_CLICKDATECHANGED,

		DMEVT_RT_BOXCHANG      = 22200,
		DMEVT_RT_BOXCHANGED,

		DMEVT_CHILDLAYOUT_FINISHED = 22250,


		// 预留1000空间给script设置事件
		DMEVT_SCRIPT_START     = 23000,
		DMEVT_SCRIPT_END       = 24000,

		DMEVT_END				= 10000000,
	};

	/// <summary>
	///		通用事件
	/// </summary>
	class DM_EXPORT DMEventCmdArgs:public DMEventArgs
	{
	public:
		DMEventCmdArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID = DMEVT_CMD};
		UINT GetEventID(){return EventID;};
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_CMD);}
	};

	/// <summary>
	///		通用鼠标停留事件
	/// </summary>
	class DM_EXPORT DMEventHoverCmdArgs:public DMEventArgs
	{
	public:
		DMEventHoverCmdArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID = DMEVT_HOVERCMD};
		UINT GetEventID(){return EventID;};
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_HOVERCMD);}
	};

	/// <summary>
	///		通用鼠标离开事件
	/// </summary>
	class DM_EXPORT DMEventLeaveCmdArgs:public DMEventArgs
	{
	public:
		DMEventLeaveCmdArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID = DMEVT_LEAVECMD};
		UINT GetEventID(){return EventID;};
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_LEAVECMD);}
	};


	/// <summary>
	///		通用布局完成消息
	/// </summary>
	class DM_EXPORT DMEventSizeChangedCmdArgs:public DMEventArgs
	{
	public:
		DMEventSizeChangedCmdArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID = DMEVT_SIZECHANGED};
		UINT GetEventID(){return EventID;};
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_SIZECHANGED);}
	};

	/// <summary>
	///		通用Check改变消息
	/// </summary>
	class DM_EXPORT DMEventCheckChangingCmdArgs:public DMEventArgs
	{
	public:
		DMEventCheckChangingCmdArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_bChecking(false),m_bCancel(false){}
		enum{EventID = DMEVT_CHECKCHANGING};
		UINT GetEventID(){return EventID;};
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_CHECKCHANGING);}
		bool                m_bChecking;        ///< 从uncheck变为check，此值为真，否则为假
		bool				m_bCancel;			///< 是否取消当前操作
	};

	class DM_EXPORT DMEventCheckChangedCmdArgs:public DMEventArgs
	{
	public:
		DMEventCheckChangedCmdArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID = DMEVT_CHECKCHANGED};
		UINT GetEventID(){return EventID;};
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_CHECKCHANGED);}
	};

	/// <summary>
	///		通用右键菜单消息
	/// </summary>
	class DM_EXPORT DMEventRButtonMenuCmdArgs:public DMEventArgs
	{
	public:
		DMEventRButtonMenuCmdArgs(DUIWindow *pWnd):DMEventArgs(pWnd){m_pt.x=m_pt.y=0;}
		enum{EventID = DMEVT_RBUTTONMENU};
		UINT GetEventID(){return EventID;};
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_RBUTTONMENU);}
		POINT				m_pt;
	};

	/// <summary>
	///		Tab页切换中事件
	/// </summary>
	class DM_EXPORT DMEventTabSelChangingArgs:public DMEventArgs
	{
	public:
		DMEventTabSelChangingArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_bCancel(false){}
		enum{EventID=DMEVT_TAB_SELCHANGING};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TAB_SELCHANGING);}
		UINT				m_uOldSel;			///<先前选中页
		UINT				m_uNewSel;			///<当前选中页
		bool				m_bCancel;          ///<是否取消当前切换页的操作
	};

	/// <summary>
	///		Tab页切换后事件
	/// </summary>
	class DM_EXPORT DMEventTabSelChangedArgs:public DMEventArgs
	{
	public:
		DMEventTabSelChangedArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_TAB_SELCHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TAB_SELCHANGED);}
		UINT				m_uOldSel;			///<先前选中页
		UINT				m_uNewSel;			///<当前选中页
	};

	/// <summary>
	///		Tab页准备删除事件
	/// </summary>
	class DM_EXPORT DMEventTabCtrlDelingArgs : public DMEventArgs
	{
	public:
		DMEventTabCtrlDelingArgs(DUIWindow* pWnd):DMEventArgs(pWnd),m_bCancel(false),m_pPage(NULL){}
		enum{EventID=DMEVT_TAB_DELING};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TAB_DELING);}
		DUIWindow*            m_pPage;            ///< 当前准备删除的项
		bool                  m_bCancel;          ///< 是否取消删除
	};

	/// <summary>
	///		Tab页删除后事件
	/// </summary>
	class DMEventTabCtrlDelArgs : public DMEventArgs
	{
	public:
		DMEventTabCtrlDelArgs(DUIWindow* pWnd):DMEventArgs(pWnd),m_pPage(NULL),m_bDelAll(false){}
		enum{EventID=DMEVT_TAB_DELETED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TAB_DELETED);}
		DUIWindow*            m_pPage;            ///< 当前已删除的项
		bool                  m_bDelAll;          ///< 在调用DeleteAllItems时设置为true
	};

	/// <summary>
	///		3D转动事件
	/// </summary>
	class DM_EXPORT DMEvent3dViewArgs:public DMEventArgs
	{
	public:
		DMEvent3dViewArgs(DUIWindow *pWnd,bool bTurn2Front):DMEventArgs(pWnd),m_bTurn2Front(bTurn2Front){}
		enum{EventID=DMEVT_TAB_3DVIEW};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TAB_3DVIEW);}
		bool				m_bTurn2Front;     
	};

	/// <summary>
	///		滚动条事件
	/// </summary>
	class DM_EXPORT DMEventScrollArgs:public DMEventArgs
	{
	public:
		DMEventScrollArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_SCROLLBAR};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_SCROLLBAR);}
		UINT				 m_uSbCode;		  ///<当前触发的位置
		int					 m_nPos;		  ///<滑条的pos
		bool				 m_bVert;         ///<水平、竖直滚动条
	};


	/// <summary>
	///		RichEdit事件
	/// </summary>
	class DM_EXPORT DMEventRENotifyArgs:public DMEventArgs
	{
	public:
		DMEventRENotifyArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_RENOTIFY};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_RENOTIFY);}
		DWORD				 m_iNotify;
		LPVOID				 m_pv;
		HRESULT              m_hr;
	};

	/// <summary>
	///		RichEdit事件,在设置了wantReturn状态时，而且为单行时，发此消息，用于配合网页显示控件
	/// </summary>
	class DM_EXPORT DMEventREWantReturnArgs:public DMEventArgs
	{
	public:
		DMEventREWantReturnArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_REWANTRETURN};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_REWANTRETURN);}
	};

	/// <summary>
	///		设置焦点事件
	/// </summary>
	class DM_EXPORT DMEventSetFocusArgs : public DMEventArgs
	{
	public:
		DMEventSetFocusArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_SETFOCUS};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_SETFOCUS);}
	};

	/// <summary>
	///		失去焦点事件
	/// </summary>
	class DM_EXPORT DMEventKillFocusArgs : public DMEventArgs
	{
	public:
		DMEventKillFocusArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_KILLFOCUS};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_KILLFOCUS);}
	};

	/// <summary>
	///		LISTBOX事件
	/// </summary>
	class DM_EXPORT DMEventLBGetDispInfoArgs : public DMEventArgs
	{
	public:
		DMEventLBGetDispInfoArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_LB_GETDISPINFO};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_LB_GETDISPINFO);}
		int						m_iItem;
		DUIWindow*				m_pItem;
		bool					m_bSel;
		bool					m_bHover;
	};

	/// <summary>
	///		LISTBOX选择中事件
	/// </summary>
	class DM_EXPORT DMEventLBSelChangingArgs : public DMEventArgs
	{
	public:
		DMEventLBSelChangingArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_bCancel(false){}
		enum{EventID=DMEVT_LB_SELCHANGING};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_LB_SELCHANGING);}
		int						m_nNewSel;           ///<先前选中项
		int						m_nOldSel;			 ///<当前选中项
		bool					m_bCancel;			  ///<是否取消选中项
	};

	/// <summary>
	///		LISTBOX选择后事件
	/// </summary>
	class DM_EXPORT DMEventLBSelChangedArgs : public DMEventArgs
	{
	public:
		DMEventLBSelChangedArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_nNewSel(-1),m_nOldSel(-1){}
		enum{EventID=DMEVT_LB_SELCHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_LB_SELCHANGED);}
		int						 m_nNewSel;
		int						 m_nOldSel;
	};

	/// <summary>
	///		PANEL选择后事件
	/// </summary>
	class DM_EXPORT DMEventOfPanelArgs : public DMEventArgs
	{
	public:
		DMEventOfPanelArgs(DUIWindow* pPanel, DMEventArgs *pOrgEvt)
			:DMEventArgs(pPanel),m_pPanel(pPanel),m_pOrgEvt(pOrgEvt){}
		enum{EventID=DMEVT_OFPANEL};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_OFPANEL);}

		PVOID					m_pPanel; // 请自行转换成DUIItemPanel
		DMEventArgs*			m_pOrgEvt;
	};

	/// <summary>
	///		PANEL被左击事件
	/// </summary>
	class DM_EXPORT DMEventOfPanelClickArgs : public DMEventArgs
	{
	public:
		DMEventOfPanelClickArgs(DUIWindow* pPanel)
			:DMEventArgs(pPanel),m_pPanel(pPanel){}
		enum{EventID=DMEVT_OFPANELCLICK};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_OFPANELCLICK);}
		PVOID					m_pPanel; // 请自行转换成DUIItemPanel
	};

	/// <summary>
	///		PANEL被右击事件
	/// </summary>
	class DM_EXPORT DMEventOfPanelRClickArgs : public DMEventArgs
	{
	public:
		DMEventOfPanelRClickArgs(DUIWindow* pPanel)
			:DMEventArgs(pPanel),m_pPanel(pPanel){}
		enum{EventID=DMEVT_OFPANELRCLICK};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_OFPANELRCLICK);}

		PVOID					m_pPanel; // 请自行转换成DUIItemPanel
	};

	/// <summary>
	///		COMBOX事件
	/// </summary>
	class DM_EXPORT DMEventCBSelChangeArgs : public DMEventArgs
	{
	public:
		DMEventCBSelChangeArgs(DUIWindow *pWnd,int nCurSel):DMEventArgs(pWnd),m_nCurSel(nCurSel){}
		enum{EventID=DMEVT_CB_SELCHANGE};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_CB_SELCHANGE);}
		int						m_nCurSel;
	};

	/// <summary>
	///		TREECTRL选择中事件
	/// </summary>
	class DM_EXPORT DMEventTCSelChangingArgs : public DMEventArgs
	{
	public:
		DMEventTCSelChangingArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_bCancel(FALSE){}
		enum{EventID=DMEVT_TC_SELCHANGING};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TC_SELCHANGING);}
		HDMTREEITEM				m_hOldSel;
		HDMTREEITEM				m_hNewSel;
		BOOL					m_bCancel;
	};

	/// <summary>
	///		TREECTRL选择后事件
	/// </summary>
	class DM_EXPORT DMEventTCSelChangedArgs : public DMEventArgs
	{
	public:
		DMEventTCSelChangedArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_TC_SELCHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TC_SELCHANGED);}
		HDMTREEITEM				m_hOldSel;
		HDMTREEITEM				m_hNewSel;
	};

	/// <summary>
	///		TREECTRL Check事件
	/// </summary>
	class DM_EXPORT DMEventTCCheckStateArgs : public DMEventArgs
	{
	public:
		DMEventTCCheckStateArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_TC_CHECKSTATE};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TC_CHECKSTATE);}
		HDMTREEITEM				 m_hItem;
		UINT					 m_uCheckState;
	};

	/// <summary>
	///		TREECTRL 展开事件
	/// </summary>
	class DM_EXPORT DMEventTCExpandArgs : public DMEventArgs
	{
	public:
		DMEventTCExpandArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_TC_EXPAND};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TC_EXPAND);}
		HDMTREEITEM				 m_hItem;
		BOOL					 m_bCollapsed;
	};

	class DM_EXPORT DMEventTCDbClickArgs : public DMEventArgs
	{
	public:
		DMEventTCDbClickArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_TC_DBCLICK};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TC_DBCLICK);}
		HDMTREEITEM				 m_hItem;  //双击选中的节点
	};

	/// <summary>
	///		列表头 事件
	/// </summary>
	class DM_EXPORT DMEventHeaderClickArgs : public DMEventArgs
	{
	public:
		DMEventHeaderClickArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_HEADER_CLICK};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_HEADER_CLICK);}
		int						 m_iItem;
	};

	/// <summary>
	///		列表头切换中事件
	/// </summary>
	class DM_EXPORT DMEventHeaderItemChangingArgs : public DMEventArgs
	{
	public:
		DMEventHeaderItemChangingArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_bCancel(FALSE){}
		enum{EventID=DMEVT_HEADER_ITEMCHANGING};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_HEADER_ITEMCHANGING);}
		int						m_iItem;
		int						m_nWidth;	//外部可设置新的宽度
		BOOL                    m_bCancel;  //可取消设置
	};

	/// <summary>
	///		列表头切换后事件
	/// </summary>
	class DM_EXPORT DMEventHeaderItemChangedArgs : public DMEventArgs
	{
	public:
		DMEventHeaderItemChangedArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_HEADER_ITEMCHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_HEADER_ITEMCHANGED);}
		int						m_iItem;
		int						m_nWidth;
	};

	/// <summary>
	///		列表头拖动事件
	/// </summary>
	class DM_EXPORT DMEventHeaderItemSwapArgs : public DMEventArgs
	{
	public:
		DMEventHeaderItemSwapArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_HEADER_ITEMSWAP};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_HEADER_ITEMSWAP);}
		int					   m_iOldIndex;
		int					   m_iNewIndex;
	};

	/// <summary>
	///		LISTCTRL事件
	/// </summary>
	class DM_EXPORT DMEventLCGetDispInfoArgs : public DMEventArgs
	{
	public:
		DMEventLCGetDispInfoArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_LC_GETDISPINFO};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_LC_GETDISPINFO);}
		int						m_iItem;
		DUIWindow*				m_pItem;
		bool					m_bSel;
		bool					m_bHover;
	};

	/// <summary>
	///		LISTCTRL改变中事件
	/// </summary>
	class DM_EXPORT DMEventLCSelChangingArgs : public DMEventArgs
	{
	public:
		DMEventLCSelChangingArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_bCancel(false){}
		enum{EventID=DMEVT_LC_SELCHANGING};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_LC_SELCHANGING);}
		int						m_nNewSel;
		int						m_nOldSel;
		bool					m_bCancel;
	};

	/// <summary>
	///		LISTCTRL改变后事件
	/// </summary>
	class DM_EXPORT DMEventLCSelChangedArgs : public DMEventArgs
	{
	public:
		DMEventLCSelChangedArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID=DMEVT_LC_SELCHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_LC_SELCHANGED);}
		int						m_nNewSel;
		int						m_nOldSel;
	};

	/// <summary>
	///		LISTCTRL删除事件
	/// </summary>
	class DM_EXPORT DMEventLCItemDeletedArgs : public DMEventArgs
	{
	public:
		DMEventLCItemDeletedArgs(DUIWindow *pWnd) :DMEventArgs(pWnd){}
		enum{EventID=DMEVT_LC_ITEMDELETED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_LC_ITEMDELETED);}
		int						m_nItem;
		DWORD					m_dwData;
	};

	/// <summary>
	///		Slider Pos改变事件
	/// </summary>
	class DM_EXPORT DMEventSDChangingArgs : public DMEventArgs
	{
	public:
		DMEventSDChangingArgs(DUIWindow *pWnd) :DMEventArgs(pWnd),m_bCacel(false){}
		enum{EventID=DMEVT_SD_POSCHANG};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_SD_POSCHANG);}
		int                     m_nOldPos;
		int                     m_nNewPos;
		bool                    m_bCacel;
	};

	/// <summary>
	///		Slider Pos改变后事件
	/// </summary>
	class DM_EXPORT DMEventSDChangedArgs : public DMEventArgs
	{
	public:
		DMEventSDChangedArgs(DUIWindow *pWnd) :DMEventArgs(pWnd){}
		enum{EventID=DMEVT_SD_POSCHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_SD_POSCHANGED);}
		int                     m_nOldPos;
		int                     m_nNewPos;
	};

	/// <summary>
	///		滚动事件
	/// </summary>
	class DM_EXPORT DMEventOnScrollArgs : public DMEventArgs
	{
	public:
		DMEventOnScrollArgs(DUIWindow *pWnd) :DMEventArgs(pWnd),m_iSbCode(-1),m_iPos(0),m_bVert(true){}
		enum{EventID=DMEVT_SC_ONSCROLL};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_SC_ONSCROLL);}
		int						m_iSbCode;
		int                     m_iPos;
		bool                    m_bVert;

	};

	/// <summary>
	///		日历 设置日期时触发
	/// </summary>
	class DM_EXPORT DMEventCalenderSetDateArgs : public DMEventArgs
	{
	public:
		DMEventCalenderSetDateArgs(DUIWindow *pWnd) :DMEventArgs(pWnd),m_iYear(-1),m_iMonth(-1),m_iDay(-1){}
		enum{EventID=DMEVT_CL_SETDATE};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_CL_SETDATE);}
		int					   m_iYear;			
		int                    m_iMonth;
		int                    m_iDay;
	};

	/// <summary>
	///		日历 CELL被点击改变后触发
	/// </summary>
	class DM_EXPORT DMEventCalenderClickDateChangedArgs : public DMEventArgs
	{// 如果新旧相差一个月,说明点击到了灰色的cell，自动执行了跳转
	public:
		DMEventCalenderClickDateChangedArgs(DUIWindow *pWnd) :DMEventArgs(pWnd),m_iOldYear(-1),m_iOldMonth(-1),m_iOldDay(-1),m_iNewYear(-1),m_iNewMonth(-1),m_iNewDay(-1){}
		enum{EventID=DMEVT_CL_CLICKDATECHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_CL_CLICKDATECHANGED);}
		int					   m_iOldYear;			
		int                    m_iOldMonth;
		int                    m_iOldDay;
		int					   m_iNewYear;			
		int                    m_iNewMonth;
		int                    m_iNewDay;
	};

	/// <summary>
	///		 RectTracker的box区域改变时触发
	/// </summary>
	class DM_EXPORT DMEventRTBOXChangingArgs : public DMEventArgs
	{
	public:
		DMEventRTBOXChangingArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_bCancel(FALSE){}
		enum{EventID=DMEVT_RT_BOXCHANG};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_RT_BOXCHANG);}
		CRect                   m_rcBoxOld;
		CRect                   m_rcBoxNew;
		BOOL					m_bCancel;
	};

	/// <summary>
	///		 RectTracker的box区域改变后触发
	/// </summary>
	class DM_EXPORT DMEventRTBOXChangedArgs : public DMEventArgs
	{
	public:
		DMEventRTBOXChangedArgs(DUIWindow *pWnd):DMEventArgs(pWnd),m_message(0){}
		enum{EventID=DMEVT_RT_BOXCHANGED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_RT_BOXCHANGED);}
		CRect                   m_rcBoxOld;
		CRect                   m_rcBoxNew;
		UINT                    m_message;
	};

	/// <summary>
	///		 子窗口布局完成后通知
	/// </summary>
	class DM_EXPORT DMEventChildLayoutFinishedArgs : public DMEventArgs
	{
	public:
		DMEventChildLayoutFinishedArgs(DUIWindow *pWnd) :DMEventArgs(pWnd){}
		enum{EventID=DMEVT_CHILDLAYOUT_FINISHED};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_CHILDLAYOUT_FINISHED);}
	};

	/// <summary>
	///		脚本事件
	/// </summary>
	class DM_EXPORT DMEventScriptStartArgs : public DMEventArgs
	{
	public:
		DMEventScriptStartArgs(DUIWindow *pWnd) :DMEventArgs(pWnd){}
		enum{EventID=DMEVT_SCRIPT_START};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_SCRIPT_START);}
	};

	class DM_EXPORT DMEventScriptEndArgs : public DMEventArgs
	{
	public:
		DMEventScriptEndArgs(DUIWindow *pWnd) :DMEventArgs(pWnd){}
		enum{EventID=DMEVT_SCRIPT_END};
		virtual UINT GetEventID(){return EventID;}
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_SCRIPT_END);}
	};

}//namespace DM