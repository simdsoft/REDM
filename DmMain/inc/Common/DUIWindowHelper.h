//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIWindowHelper.h 
// File Des: DUI窗口类DUIWnd.h的辅助数据
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-30	1.0			
//-------------------------------------------------------
#pragma once


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIWindow"/>的xml属性定义
	/// </summary>
	class DUIWindowAttr
	{
	public:
		static wchar_t* STRING_name;                                    ///< 窗口名，唯一标识, 示例:name="firstwnd"
		static wchar_t* INT_id;											///< 窗口ID，唯一标识,示例:id="10"

		static wchar_t* bool_bvisible;                                  ///< 窗口是否可见,示例:bvisible="1"
		static wchar_t* bool_bdisable;                                  ///< 窗口是否禁用,示例:bdisable="1"
		static wchar_t* bool_bplaceholder;                              ///< 窗口隐藏时是否占位,默认是占位!,不占位时启动重新布局,注意流式布局子控件隐藏时同样使用占位,示例:bplaceholder="1"
		static wchar_t* bool_bcache;								    ///< 支持窗口内容的Cache标志,缓存画布建议用在不经常改变大小、背景色的地方,示例:bcache=="1"

		static wchar_t* bool_bolnydrawclient;                           ///< 是否仅绘制客户区,示例:bolnydrawclient="1"
		static wchar_t* bool_bfocusable;							    ///< 窗口是否可获得焦点,焦点窗口可使用tab切换窗口，示例:bfocusable=="1"
		static wchar_t* bool_bshowdrawfocus;                            ///< 在有焦点窗口时,是否绘制焦点框，示例:bshowdrawfocus=="1"
		static wchar_t* bool_bmsgnohandle;							    ///< 窗口是否不处理消息,如为true,在hittest中会直接忽视这个窗口,用于static等.示例:bmsgnohandle=="1"
		static wchar_t* bool_bdrag;                                     ///< 窗口是否可拖动. 设置caption可拖动状态,示例:bdrag="1"
		
		static wchar_t* STRING_style;                                   ///< 指定使用的style,示例:style="custom"
		static wchar_t* STRING_text;                                    ///< 窗口文本,示例:text="OK"
		static wchar_t* STRING_tiptext;                                 ///< 窗口tooltip提示语，示例:tiptext="close"
		static wchar_t* STRING_tipxmlid;                                ///< 窗口tips的xmlid,示例:tipxmlid="dui_tips1"
		static wchar_t* INT_tipdelay;                                   ///< 窗口tooltip提示语延迟显示时间(默认500ms)，示例:tipdelay="10"
		static wchar_t* INT_tipspan;                                    ///< 窗口tooltip提示语显示停留时间(默认5000ms)，示例:tipspan="10"
		static wchar_t* RECT_tipflags;                                  ///< 类似TrackPopupMenu的参数，前三项分别代表UINT uFlags, int x, int y，默认uFlags为-1,则无视此参数,示例:tipflags="0,100,10,0"

		// FlowLayout，对一般的锚点布局和绝对布局无效
		static wchar_t* INT_maxwidth;                                   ///< 仅用于流式布局的子控件,窗口的最大宽度,为-1表示使用自动计算,示例:maxwidth="10"
		static wchar_t* INT_minwidth;                                   ///< 仅用于流式布局的子控件,窗口的最大小度,为-1表示使用自动计算,示例:INT_minwidth="10"
		static wchar_t* INT_maxheight;                                  ///< 仅用于流式布局的子控件,窗口的最大高度,为-1表示使用自动计算,示例:maxheight="10"
		static wchar_t* INT_minheight;                                  ///< 仅用于流式布局的子控件,窗口的最小高度,为-1表示使用自动计算,示例:minheight="10"
		static wchar_t* INT_width;                                      ///< 仅用于流式布局的子控件,窗口的正常宽度,为-1表示使用自动计算,示例:width="10"
		static wchar_t* INT_height;                                     ///< 仅用于流式布局的子控件,窗口的正常高度,为-1表示使用自动计算,示例:height="10"
		static wchar_t* RECT_rcpadd;                                    ///< 仅用于流式布局的子控件,窗口的四个边的外边距,相当于把窗口四边扩大rcpadd，再放置到流式布局中,示例:rcPadd="10,5,5,5"

	};
	DMAttrValueInit(DUIWindowAttr,STRING_name)
	DMAttrValueInit(DUIWindowAttr,INT_id)
	
	DMAttrValueInit(DUIWindowAttr,bool_bvisible)
	DMAttrValueInit(DUIWindowAttr,bool_bdisable)
	DMAttrValueInit(DUIWindowAttr,bool_bplaceholder)
	DMAttrValueInit(DUIWindowAttr,bool_bcache)

	DMAttrValueInit(DUIWindowAttr,bool_bolnydrawclient)
	DMAttrValueInit(DUIWindowAttr,bool_bfocusable)
	DMAttrValueInit(DUIWindowAttr,bool_bshowdrawfocus)
	DMAttrValueInit(DUIWindowAttr,bool_bmsgnohandle)
	DMAttrValueInit(DUIWindowAttr,bool_bdrag)

	DMAttrValueInit(DUIWindowAttr,STRING_style)DMAttrValueInit(DUIWindowAttr,STRING_text)
	DMAttrValueInit(DUIWindowAttr,STRING_tiptext)DMAttrValueInit(DUIWindowAttr,STRING_tipxmlid)DMAttrValueInit(DUIWindowAttr,INT_tipdelay)DMAttrValueInit(DUIWindowAttr,INT_tipspan)DMAttrValueInit(DUIWindowAttr,RECT_tipflags)
	DMAttrValueInit(DUIWindowAttr,INT_maxwidth)DMAttrValueInit(DUIWindowAttr,INT_minwidth)DMAttrValueInit(DUIWindowAttr,INT_maxheight)DMAttrValueInit(DUIWindowAttr,INT_minheight)
	DMAttrValueInit(DUIWindowAttr,INT_width)DMAttrValueInit(DUIWindowAttr,INT_height)DMAttrValueInit(DUIWindowAttr,RECT_rcpadd)
}

namespace DM
{
	/// <summary>
	///		所有DUI窗口消息分发的基类,函数名和DMMsgCrack.h保持一致,方便DMMsgCrack.h和DUIMsgCrack.h混用,在DUI上,使用m_bDuiMsgHandled,否则使用DMCWnd的m_bMsgHandled
	/// </summary>
	class DM_EXPORT DUIMsgHandler
	{
	public:
		DUIMsgHandler():m_bDuiMsgHandled(FALSE){}
		BOOL IsMsgHandled() const
		{
			return m_bDuiMsgHandled;
		}
		void SetMsgHandled(BOOL bHandled)
		{
			m_bDuiMsgHandled = bHandled;
		}
		BOOL m_bDuiMsgHandled;
	};

	/// <summary>
	///		<see cref="DUIWindow"/>的xml参数
	/// </summary>
	class DM_EXPORT DUIWindow_XmlInfo:public DMDataBase
	{
	public:
		DUIWindow_XmlInfo(DUIWindow*pOwner);
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_STRING_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_name,m_strName,DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_id, m_iID, DM_ECODE_OK)
			
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bvisible, OnAttributeVisible)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bdisable, OnAttributeDisable)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bplaceholder, OnAttributePlaceHolder)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bcache, OnAttributeCache)

			DM_bool_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bolnydrawclient,m_bOnlyDrawClient,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bfocusable,m_bFocusable,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bshowdrawfocus,m_bShowDrawFocus,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bmsgnohandle,m_bMsgNoHandle,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIWindowAttr::bool_bdrag,m_bDrag,DM_ECODE_OK)

			DM_STRING_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_text, m_strText,DM_ECODE_NOXMLLOADREFRESH)
			DM_STRING_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_tiptext, m_strTooltipText,DM_ECODE_OK)
			DM_STRING_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_tipxmlid, m_strTipsXmlId,DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_tipdelay, m_iTooltipDelayTime, DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_tipspan, m_iTooltipSpanTime, DM_ECODE_OK)
			DM_RECT_ATTRIBUTE(DMAttr::DUIWindowAttr::RECT_tipflags,m_rcTooltipPosFlags,DM_ECODE_OK);
			// FlowLayout
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_maxwidth,m_nMaxWidth, DM_ECODE_NOXMLRELAYOUT)
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_minwidth,m_nMinWidth, DM_ECODE_NOXMLRELAYOUT)
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_maxheight,m_nMaxHeight, DM_ECODE_NOXMLRELAYOUT)
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_minheight,m_nMinHeight, DM_ECODE_NOXMLRELAYOUT)
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_height, m_iHeight, DM_ECODE_NOXMLRELAYOUT)
			DM_INT_ATTRIBUTE(DMAttr::DUIWindowAttr::INT_width, m_iWidth, DM_ECODE_NOXMLRELAYOUT)
			DM_RECT_ATTRIBUTE(DMAttr::DUIWindowAttr::RECT_rcpadd, m_rcPadd, DM_ECODE_NOXMLRELAYOUT)

			// Style
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_style, OnAttributeStyle)
			DM_CHAIN_ATTRIBUTTE(m_pStyle)            // 允许直接定制style
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeVisible(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeDisable(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributePlaceHolder(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeCache(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeStyle(LPCWSTR lpszValue, bool bLoadXml);

	public:
		DMCode OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr);

	public:
		DUIWindow*          m_pOwner;                              ///< 拥有窗口
		CStringW            m_strName;                             ///< 窗口名，唯一标识
		int                 m_iID;                                 ///< 窗口ID，唯一标识

		bool				m_bVisible;							   ///< 窗口可见状态
		bool				m_bDisable;							   ///< 窗口禁用状态
		bool				m_bPlaceHolder;						   ///< 窗口隐藏时是否占位，不占位时启动重新布局
		bool 				m_bCacheDraw;						   ///< 支持窗口内容的Cache标志,缓存画布建议用在不经常改变大小、背景色的地方

		bool			    m_bOnlyDrawClient;					   ///< 窗口绘制时做clip客户区处理的标志,由于clip可能增加计算量，只在绘制可能走出客户区时才设置
		bool				m_bFocusable;						   ///< 窗口可获得焦点标志
		bool                m_bShowDrawFocus;                      ///< 有窗口焦点时，是否绘制焦点框
		bool				m_bMsgNoHandle;					       ///< 接收消息标志 TRUE-不处理消息
		bool                m_bDrag;                               ///< 设置caption可拖动状态
		bool			    m_bNoPreSize;						   ///< 是否不使用PreSize判断脏区

		CStringW            m_strText;							   ///< 窗口文字
		CStringW            m_strTooltipText;                      ///< 窗口简单tooltip
		CStringW            m_strTipsXmlId;						   ///< 窗口tips的xml资源ID
		int                 m_iTooltipDelayTime;                   ///< tooltip延迟显示时间,ms
		int                 m_iTooltipSpanTime;                    ///< tooltip停留显示时间,ms
		CRect               m_rcTooltipPosFlags;				   ///< 类似TrackPopupMenu的参数，前三项分别代表UINT uFlags, int x, int y

		DMSmartPtrT<IDMStyle> m_pStyle;							   ///< 窗口属性，内部保证了它总是不为空

		//-------------FlowLayout----------------------------
		int					m_nMaxWidth;						   ///< 自动计算大小时，窗口的最大宽度
		int					m_nMaxHeight;						   ///< 自动计算大小时，窗口的最大高度
		int					m_nMinWidth;						   ///< 自动计算大小时，窗口的最小宽度
		int					m_nMinHeight;						   ///< 自动计算大小时，窗口的最小高度

		int					m_iHeight;							   ///< 窗口的宽度
		int					m_iWidth;							   ///< 窗口的高度
		CRect				m_rcPadd;							   ///< 控件边界与外界的间隙，注意在WPF中，Padding指的是控件内部内容与控件边界的间隙，但这里没有必要设置内部

	};

	typedef struct stDUIWNDMSG{UINT uMsg;WPARAM	 wp;LPARAM lp;}DUIWNDMSG,*PDUIWNDMSG;
	/// <summary>
	///		DUI的其他基础属性
	/// </summary>
	class DM_EXPORT DUIWindow_Data
	{
	public:
		DUIWindow_Data();

	public:
		bool			  m_bDirty;									///< 缓存窗口脏标志 
		bool			  m_bUpdateLocked;							///< 暂时锁定更新，锁定后，不向宿主发送DM_Invalidate 
		DUIWNDMSG        *m_pCurMsg;							    ///< 当前正在处理的窗口消息
		CSize             m_PreSize;                                ///< 在每次布局完成后，在接收的WM_SIZE中判断是否size改变，如size未改变，默认不设置脏区（缓存画布）

		// 用于DM_GetCanvas、DM_ReleaseCanvas
		CRect			 m_rcGetCanvas;								///< GetCanvas的绘制区
		DWORD			 m_dcFlags;									///< DMOLEDCFLAGS
		bool			 m_bClipCanvas;

	};

	class DUIWindow;
	/// <summary>
	///		DUI的树形窗口管理
	/// </summary>
	class DM_EXPORT DUIWindowNode
	{
	public:
		DUIWindowNode();
	public:
		DUIWindow*			m_pOwner;								///< 容器Owner(一般可认为是真实的主窗口)，事件分发时，会把事件交给Owner处理，如Combox中的list
		DUIWindow*			m_pParent;								///< 父窗口 
		DUIWindow*          m_pPanelParent;                         ///< 如果是panel，指定它所属的m_ChildPanelArray父窗口
		DUIWindow*			m_pFirstChild;							///< 第一子窗口
		DUIWindow*			m_pLastChild;							///< 最后窗口
		DUIWindow*			m_pNextSibling;							///< 前一兄弟窗口
		DUIWindow*			m_pPrevSibling;							///< 后一兄弟窗口
		int					m_nChildrenCount;						///< 子窗口数量
	};

	/// <summary>
	///		DUI的绘制环境备份
	/// </summary>
	class DM_EXPORT DUIDrawEnviron
	{
	public:
		DUIDrawEnviron(): m_ClrOld(CLR_INVALID),m_dwReserve(0){}

		DMSmartPtrT<IDMFont>		 m_pOldPen;
		DMColor						 m_ClrOld;
		DWORD                        m_dwReserve;					///< 其他
	};

	// -----------------------------------------------
	// 结构定义
	typedef enum _GDW_CODE
	{
		GDW_FIRSTCHILD=0,
		GDW_LASTCHILD,
		GDW_PREVSIBLING,
		GDW_NEXTSIBLING,
		GDW_PARENT,
		GDW_OWNER,
		GDW_PANELPARENT,
	}GDW_CODE;

#define DUIWND_FIRST    ((DUIWindow*)-1)
#define DUIWND_LAST     NULL

	// State Define
	typedef enum  _DUIWND_STATE
	{
		DUIWNDSTATE_Normal       = 0x00000000UL,
		DUIWNDSTATE_Hover        = 0x00000001UL,
		DUIWNDSTATE_PushDown     = 0x00000002UL,
		DUIWNDSTATE_Check        = 0x00000004UL,
		DUIWNDSTATE_Novisible    = 0x00000008UL, // 此标志和父窗口相关，在父窗口不可见，但m_bVisible为true时，应该设置这个标志（父窗口都隐藏了，即使子窗口可见，结果当然也是不可见状态）
		DUIWNDSTATE_Disable      = 0x00000010UL, // 此标志类同于DUIWNDSTATE_Novisible
		DUIWNDSTATE_Noactive     = 0x00000020UL,
		DUIWNDSTATE_FULL         = 0xffffffffUL,
	}DUIWND_STATE;

#define IIF_STATE2(the_state, normal_value, hover_value) \
	(((the_state) & DUIWNDSTATE_Hover) ? (hover_value) : (normal_value))

#define IIF_STATE3(the_state, normal_value, hover_value, pushdown_value) \
	(((the_state) & DUIWNDSTATE_PushDown) ? (pushdown_value) : IIF_STATE2(the_state, normal_value, hover_value))

#define IIF_STATE4(the_state, normal_value, hover_value, pushdown_value, disable_value) \
	(((the_state) & DUIWNDSTATE_Disable) ? (disable_value) : IIF_STATE3(the_state, normal_value, hover_value, pushdown_value))

#define IIF_STATE5(the_state, normal_value, hover_value, pushdown_value, disable_value, noactive_value) \
	(((the_state) & DUIWNDSTATE_Noactive) ? (noactive_value) : IIF_STATE4(the_state, normal_value, hover_value, pushdown_value, disable_value))

#define  DMDLGC_WANTARROWS         0x0001 // 箭头键
#define  DMDLGC_WANTTAB            0x0002
#define  DMDLGC_WANTRETURN         0x0004
#define  DMDLGC_WANTCHARS          0x0008
#define  DMDLGC_WANTALLKEYS        0xFFFF
#define  DMDLGC_WANTSYSKEY         0x80000000

	enum// 布局标志
	{
		POS_INIT = 0x11000000,			  // 坐标的初始化值
		POS_WAIT = 0x12000000,			  // 坐标的计算依赖于其它窗口的布局
	};
	enum {NormalShow=0,ParentShow=1};     // 提供给WM_SHOWWINDOW消息识别是父窗口显示还是要显示本窗口
	enum {NormalEnable=0,ParentEnable=1}; // 提供给WM_ENABLE消息识别是父窗口可用还是直接操作当前窗口
}//namespace DM