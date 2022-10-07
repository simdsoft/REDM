//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIWindowHelper.h 
// File Des: DUI������DUIWnd.h�ĸ�������
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-30	1.0			
//-------------------------------------------------------
#pragma once

// Tests whether compiler has fully c++11 support
// About preprocessor '_MSC_VER', please see:
// https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=vs-2019
#if defined(_MSC_VER)
#  if _MSC_VER < 1900
#    define noexcept throw()
#    define DM__HAS_FULL_CXX11 0
#  else
#    define DM__HAS_FULL_CXX11 1
#    if _MSC_VER > 1900 // VS2017 or later
#      include <vcruntime.h>
#      include <sdkddkver.h>
#    endif
#  endif
#else
#  define DM__HAS_FULL_CXX11 1
#endif

#if !(DM__HAS_FULL_CXX11)
#define override
#endif

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIWindow"/>��xml���Զ���
	/// </summary>
	class DUIWindowAttr
	{
	public:
		static const char* STRING_name;                                    ///< ��������Ψһ��ʶ, ʾ��:name="firstwnd"
		static const char* INT_id;											///< ����ID��Ψһ��ʶ,ʾ��:id="10"

		static const char* bool_bvisible;                                  ///< �����Ƿ�ɼ�,ʾ��:bvisible="1"
		static const char* bool_bdisable;                                  ///< �����Ƿ����,ʾ��:bdisable="1"
		static const char* bool_bplaceholder;                              ///< ��������ʱ�Ƿ�ռλ,Ĭ����ռλ!,��ռλʱ�������²���,ע����ʽ�����ӿؼ�����ʱͬ��ʹ��ռλ,ʾ��:bplaceholder="1"
		static const char* bool_bcache;								    ///< ֧�ִ������ݵ�Cache��־,���滭���������ڲ������ı��С������ɫ�ĵط�,ʾ��:bcache=="1"

		static const char* bool_bolnydrawclient;                           ///< �Ƿ�����ƿͻ���,ʾ��:bolnydrawclient="1"
		static const char* bool_bfocusable;							    ///< �����Ƿ�ɻ�ý���,���㴰�ڿ�ʹ��tab�л����ڣ�ʾ��:bfocusable=="1"
		static const char* bool_bshowdrawfocus;                            ///< ���н��㴰��ʱ,�Ƿ���ƽ����ʾ��:bshowdrawfocus=="1"
		static const char* bool_bmsgnohandle;							    ///< �����Ƿ񲻴�����Ϣ,��Ϊtrue,��hittest�л�ֱ�Ӻ����������,����static��.ʾ��:bmsgnohandle=="1"
		static const char* bool_bdrag;                                     ///< �����Ƿ���϶�. ����caption���϶�״̬,ʾ��:bdrag="1"
		
		static const char* STRING_style;                                   ///< ָ��ʹ�õ�style,ʾ��:style="custom"
		static const char* STRING_text;                                    ///< �����ı�,ʾ��:text="OK"
		static const char* STRING_tiptext;                                 ///< ����tooltip��ʾ�ʾ��:tiptext="close"
		static const char* STRING_tipxmlid;                                ///< ����tips��xmlid,ʾ��:tipxmlid="dui_tips1"
		static const char* INT_tipdelay;                                   ///< ����tooltip��ʾ���ӳ���ʾʱ��(Ĭ��500ms)��ʾ��:tipdelay="10"
		static const char* INT_tipspan;                                    ///< ����tooltip��ʾ����ʾͣ��ʱ��(Ĭ��5000ms)��ʾ��:tipspan="10"
		static const char* RECT_tipflags;                                  ///< ����TrackPopupMenu�Ĳ�����ǰ����ֱ����UINT uFlags, int x, int y��Ĭ��uFlagsΪ-1,�����Ӵ˲���,ʾ��:tipflags="0,100,10,0"

		// FlowLayout����һ���ê�㲼�ֺ;��Բ�����Ч
		static const char* INT_maxwidth;                                   ///< ��������ʽ���ֵ��ӿؼ�,���ڵ������,Ϊ-1��ʾʹ���Զ�����,ʾ��:maxwidth="10"
		static const char* INT_minwidth;                                   ///< ��������ʽ���ֵ��ӿؼ�,���ڵ����С��,Ϊ-1��ʾʹ���Զ�����,ʾ��:INT_minwidth="10"
		static const char* INT_maxheight;                                  ///< ��������ʽ���ֵ��ӿؼ�,���ڵ����߶�,Ϊ-1��ʾʹ���Զ�����,ʾ��:maxheight="10"
		static const char* INT_minheight;                                  ///< ��������ʽ���ֵ��ӿؼ�,���ڵ���С�߶�,Ϊ-1��ʾʹ���Զ�����,ʾ��:minheight="10"
		static const char* INT_width;                                      ///< ��������ʽ���ֵ��ӿؼ�,���ڵ��������,Ϊ-1��ʾʹ���Զ�����,ʾ��:width="10"
		static const char* INT_height;                                     ///< ��������ʽ���ֵ��ӿؼ�,���ڵ������߶�,Ϊ-1��ʾʹ���Զ�����,ʾ��:height="10"
		static const char* RECT_rcpadd;                                    ///< ��������ʽ���ֵ��ӿؼ�,���ڵ��ĸ��ߵ���߾�,�൱�ڰѴ����ı�����rcpadd���ٷ��õ���ʽ������,ʾ��:rcPadd="10,5,5,5"

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
	///		����DUI������Ϣ�ַ��Ļ���,��������DMMsgCrack.h����һ��,����DMMsgCrack.h��DUIMsgCrack.h����,��DUI��,ʹ��m_bDuiMsgHandled,����ʹ��DMCWnd��m_bMsgHandled
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
	///		<see cref="DUIWindow"/>��xml����
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

			DM_WSTRING_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_text, m_strText,DM_ECODE_NOXMLLOADREFRESH)
			DM_WSTRING_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_tiptext, m_strTooltipText,DM_ECODE_OK)
			DM_WSTRING_ATTRIBUTE(DMAttr::DUIWindowAttr::STRING_tipxmlid, m_strTipsXmlId,DM_ECODE_OK)
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
			DM_CHAIN_ATTRIBUTTE(m_pStyle)            // ����ֱ�Ӷ���style
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeVisible(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeDisable(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributePlaceHolder(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeCache(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeStyle(LPCSTR lpszValue, bool bLoadXml);

	public:
		DMCode OnAttributeFinished(LPCSTR pszAttribute, LPCSTR pszValue,bool bLoadXml,DMCode iErr);

	public:
		DUIWindow*          m_pOwner;                              ///< ӵ�д���
		CStringA            m_strName;                             ///< ��������Ψһ��ʶ
		int                 m_iID;                                 ///< ����ID��Ψһ��ʶ

		bool				m_bVisible;							   ///< ���ڿɼ�״̬
		bool				m_bDisable;							   ///< ���ڽ���״̬
		bool				m_bPlaceHolder;						   ///< ��������ʱ�Ƿ�ռλ����ռλʱ�������²���
		bool 				m_bCacheDraw;						   ///< ֧�ִ������ݵ�Cache��־,���滭���������ڲ������ı��С������ɫ�ĵط�

		bool			    m_bOnlyDrawClient;					   ///< ���ڻ���ʱ��clip�ͻ�������ı�־,����clip�������Ӽ�������ֻ�ڻ��ƿ����߳��ͻ���ʱ������
		bool				m_bFocusable;						   ///< ���ڿɻ�ý����־
		bool                m_bShowDrawFocus;                      ///< �д��ڽ���ʱ���Ƿ���ƽ����
		bool				m_bMsgNoHandle;					       ///< ������Ϣ��־ TRUE-��������Ϣ
		bool                m_bDrag;                               ///< ����caption���϶�״̬
		bool			    m_bNoPreSize;						   ///< �Ƿ�ʹ��PreSize�ж�����

		CStringW            m_strText;							   ///< ��������
		CStringW            m_strTooltipText;                      ///< ���ڼ�tooltip
		CStringW            m_strTipsXmlId;						   ///< ����tips��xml��ԴID
		int                 m_iTooltipDelayTime;                   ///< tooltip�ӳ���ʾʱ��,ms
		int                 m_iTooltipSpanTime;                    ///< tooltipͣ����ʾʱ��,ms
		CRect               m_rcTooltipPosFlags;				   ///< ����TrackPopupMenu�Ĳ�����ǰ����ֱ����UINT uFlags, int x, int y

		DMSmartPtrT<IDMStyle> m_pStyle;							   ///< �������ԣ��ڲ���֤�������ǲ�Ϊ��

		//-------------FlowLayout----------------------------
		int					m_nMaxWidth;						   ///< �Զ������Сʱ�����ڵ������
		int					m_nMaxHeight;						   ///< �Զ������Сʱ�����ڵ����߶�
		int					m_nMinWidth;						   ///< �Զ������Сʱ�����ڵ���С���
		int					m_nMinHeight;						   ///< �Զ������Сʱ�����ڵ���С�߶�

		int					m_iHeight;							   ///< ���ڵĿ��
		int					m_iWidth;							   ///< ���ڵĸ߶�
		CRect				m_rcPadd;							   ///< �ؼ��߽������ļ�϶��ע����WPF�У�Paddingָ���ǿؼ��ڲ�������ؼ��߽�ļ�϶��������û�б�Ҫ�����ڲ�

	};

	typedef struct stDUIWNDMSG{UINT uMsg;WPARAM	 wp;LPARAM lp;}DUIWNDMSG,*PDUIWNDMSG;
	/// <summary>
	///		DUI��������������
	/// </summary>
	class DM_EXPORT DUIWindow_Data
	{
	public:
		DUIWindow_Data();

	public:
		bool			  m_bDirty;									///< ���洰�����־ 
		bool			  m_bUpdateLocked;							///< ��ʱ�������£������󣬲�����������DM_Invalidate 
		DUIWNDMSG        *m_pCurMsg;							    ///< ��ǰ���ڴ���Ĵ�����Ϣ
		CSize             m_PreSize;                                ///< ��ÿ�β�����ɺ��ڽ��յ�WM_SIZE���ж��Ƿ�size�ı䣬��sizeδ�ı䣬Ĭ�ϲ��������������滭����

		// ����DM_GetCanvas��DM_ReleaseCanvas
		CRect			 m_rcGetCanvas;								///< GetCanvas�Ļ�����
		DWORD			 m_dcFlags;									///< DMOLEDCFLAGS
		bool			 m_bClipCanvas;

	};

	class DUIWindow;
	/// <summary>
	///		DUI�����δ��ڹ���
	/// </summary>
	class DM_EXPORT DUIWindowNode
	{
	public:
		DUIWindowNode();
	public:
		DUIWindow*			m_pOwner;								///< ����Owner(һ�����Ϊ����ʵ��������)���¼��ַ�ʱ������¼�����Owner������Combox�е�list
		DUIWindow*			m_pParent;								///< ������ 
		DUIWindow*          m_pPanelParent;                         ///< �����panel��ָ����������m_ChildPanelArray������
		DUIWindow*			m_pFirstChild;							///< ��һ�Ӵ���
		DUIWindow*			m_pLastChild;							///< ��󴰿�
		DUIWindow*			m_pNextSibling;							///< ǰһ�ֵܴ���
		DUIWindow*			m_pPrevSibling;							///< ��һ�ֵܴ���
		int					m_nChildrenCount;						///< �Ӵ�������
	};

	/// <summary>
	///		DUI�Ļ��ƻ�������
	/// </summary>
	class DM_EXPORT DUIDrawEnviron
	{
	public:
		DUIDrawEnviron(): m_ClrOld(CLR_INVALID),m_dwReserve(0){}

		DMSmartPtrT<IDMFont>		 m_pOldPen;
		DMColor						 m_ClrOld;
		LPARAM                       m_dwReserve;					///< ����
	};

	// -----------------------------------------------
	// �ṹ����
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
		DUIWNDSTATE_Novisible    = 0x00000008UL, // �˱�־�͸�������أ��ڸ����ڲ��ɼ�����m_bVisibleΪtrueʱ��Ӧ�����������־�������ڶ������ˣ���ʹ�Ӵ��ڿɼ��������ȻҲ�ǲ��ɼ�״̬��
		DUIWNDSTATE_Disable      = 0x00000010UL, // �˱�־��ͬ��DUIWNDSTATE_Novisible
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

#define  DMDLGC_WANTARROWS         0x0001 // ��ͷ��
#define  DMDLGC_WANTTAB            0x0002
#define  DMDLGC_WANTRETURN         0x0004
#define  DMDLGC_WANTCHARS          0x0008
#define  DMDLGC_WANTALLKEYS        0xFFFF
#define  DMDLGC_WANTSYSKEY         0x80000000

	enum// ���ֱ�־
	{
		POS_INIT = 0x11000000,			  // ����ĳ�ʼ��ֵ
		POS_WAIT = 0x12000000,			  // ����ļ����������������ڵĲ���
	};
	enum {NormalShow=0,ParentShow=1};     // �ṩ��WM_SHOWWINDOW��Ϣʶ���Ǹ�������ʾ����Ҫ��ʾ������
	enum {NormalEnable=0,ParentEnable=1}; // �ṩ��WM_ENABLE��Ϣʶ���Ǹ����ڿ��û���ֱ�Ӳ�����ǰ����
}//namespace DM