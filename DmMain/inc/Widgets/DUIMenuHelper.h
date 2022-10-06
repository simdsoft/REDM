//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIMenuHelper.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-10	1.0			
//--------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIMenu"/>��xml���Զ���
	/// </summary>
	class DUIMenuAttr
	{
	public:
		static const char* SKIN_bgskin;											///< �����˵��ı���ͼ,ʾ��:bgskin="menubg"
		static const char* SKIN_itemselbgskin;										///< ѡ�в˵����Ƥ��,ʾ��:itemselbgskin="menuselbg"
		static const char* SKIN_iconskin;											///< �˵�ͼ����ͼ,ʾ��:iconskin="menuicons"
		static const char* SKIN_sepskin;											///< �ָ���Ƥ��,ʾ��:sepskin="sepskin"
		static const char* SKIN_checkskin;											///< ѡ��״̬,��������״̬:��ѡ+Ȧѡ,ʾ��:checkSkin="menucheck"
		static const char* INT_itemhei;											///< �˵���߶�,������ʱĬ��Ϊ16,��˵����ͼƬ��߶ȣ�����ͼƬ��߶ȵ����ֵΪ׼,ʾ��:itemhei="16"
		static const char* INT_iconoffset;											///< ͼ�����Եƫ��,ʾ��:iconoffset="10"
		static const char* INT_textoffset;											///< �ı����Եƫ��,ʾ��:textoffset="10"
		static const char* INT_stepoffset;											///< Step���Եƫ��,ʾ��:stepoffset="10"
		static const char* INT_stephei;											///< Step�ĸ߶ȣ�Ĭ��Ϊ1,ʾ��:stephei="10"
		static const char* INT_maxwidth;											///< �˵���������,Ĭ��Ϊ-1��ʾ�Զ�����,ʾ��:maxwidth="10",Ҫע����ƴ��ڿ�Ȼ��������ô�14��ϵͳ���õģ�
		static const char* bool_bshadow;											///< �˵���Ӱ�Ƿ�����Ĭ��Ϊ0,ʾ��:bshadow="0"
		static const char* bool_bhideframe;                                        ///< �Ƿ����ز˵������߿�,Ĭ��Ϊ0,�������,��˵���item�����Һ����߿��������ͬ,ʾ��:bhideframe="1"
		static const char* SIZE_iconsize;											///< ͼ��ߴ磬Ĭ��Ϊ16*16,ʾ��:iconsize="16,16"
		static const char* SIZE_roundsize;											///< Բ�ǳߴ磬Ĭ��Ϊ0*0,ʾ��:roundsize="1,1"
		static const char* COLOR_clrbg;											///< �����˵��ı���ɫ,�ڱ���ͼ������ʱʹ��,ʾ��:clrbg="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrselbg;                                         ///< ѡ�в˵��ı���ɫ,�ڱ���ͼ������ʱʹ��,ʾ��:clrselbg="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrstep;											///< �ָ����ı���ɫ,�ڱ���ͼ������ʱʹ��,ʾ��:clrstep="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrtext;											///< �����ı���ɫ,ʾ��:clrtext="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrtextsel;										///< ѡ���ı���ɫ,ʾ��:clrtextsel="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrtextgray;										///< �Ҷ��ı���ɫ,ʾ��:clrtextgray="pbgra(ff,00,ff,ff)"
		static const char* FONT_font;												///< �˵�Ĭ������,Ŀǰ�˵���ͬ״̬ʹ������ͬ����,������Ҫ,���Լ���չ,ʾ��:font="face:����,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:��weight:���λ������:������1λ,����ո�!�ڲ����˿ո�ȥ������
		static const char* BYTE_alpha;											    ///< ����͸����,͸������������,���http://hgy413.com/1865.html ʾ��:alpha="0xc0"
		static const char* STRING_transid;										    ///< ʹ�õ�transid,ΪNULLʹ��Ĭ��custom ʾ��:transid="custom"

		static const char* NODE_sep;												///< �˵���xml������(��Ӧһ���ָ���)
		static const char* NODE_item;                                              ///< �˵���xml������(��Ӧһ���˵���)
		static const char* ITEM_height;                                            ///< �˵���ĸ߶�(��ѡ)������˵���߶�С�ڲ˵���ı�����ͼ�ĸ߶ȣ����Ա�����ͼ�߶�Ϊ׼, ʾ��:height="20"
		static const char* ITEM_maxwidth;											///< �˵���������(��ѡ),ʾ��:maxwidth="10",Ҫע����ƴ��ڿ�Ȼ��������ô�14��ϵͳ���õģ�
		static const char* ITEM_icon;                                              ///< �˵����iconλ�ã�-1��ʾû��icon��ʾ��:icon="1"
		static const char* ITEM_text;                                              ///< �˵�����ı�  ʾ��:text="��PE�ļ�..."
		static const char* ITEM_iconsize;											///< �˵����ͼ��ߴ�(��ѡ)��Ĭ��Ϊ16*16,�����,�͸���INT_iconsize,ʾ��:iconsize="16,16"
		static const char* ITEM_iconoffset;										///< �˵����ͼ�����Եƫ��(��ѡ),�����,�͸���INT_iconoffset,ʾ��:iconoffset="10"
		static const char* ITEM_textoffset;										///< �˵�����ı��ı����Եƫ��(��ѡ),�����,�͸���INT_textoffset,ʾ��:textoffset="10"
		static const char* ITEM_skin;                                              ///< �˵���ı�����ͼ(����-ѡ��-�ҵ�)������һ������,��ʹ���Զ�������ʱ,��skin����,����û������ITEM_maxwidthʱ,��ȡ��ǰ��skin��Ϊ��ǰ�˵��Ŀ��,������Ӳ˵���Ҫ��ͬ�Ŀ��,����ͨ�����ַ�ʽ��ָ��, ʾ��:skin="itemskin"
		static const char* ITEM_id;												///< �˵����id(����Ϊ0,��1��65535֮��)�����ڵ��ʱ������Ϣ  ʾ��:id="10"
		static const char* ITEM_bcheck;                                            ///< �˵����check����SKIN_checkskin��  ʾ��:bcheck="1"
		static const char* ITEM_bradio;                                            ///< �˵����radio����SKIN_checkskin����check���ѡһ��  ʾ��:bradio="1"
		static const char* ITEM_bdisable;                                          ///< ���ò˵���  ʾ��:bdisable="1"
	};
	DMAttrValueInit(DUIMenuAttr,SKIN_bgskin)DMAttrValueInit(DUIMenuAttr,SKIN_itemselbgskin)DMAttrValueInit(DUIMenuAttr,SKIN_iconskin)
	DMAttrValueInit(DUIMenuAttr,SKIN_sepskin)DMAttrValueInit(DUIMenuAttr,SKIN_checkskin)DMAttrValueInit(DUIMenuAttr,INT_itemhei)
	DMAttrValueInit(DUIMenuAttr,INT_iconoffset)DMAttrValueInit(DUIMenuAttr,INT_textoffset)DMAttrValueInit(DUIMenuAttr,INT_stepoffset)
	DMAttrValueInit(DUIMenuAttr,INT_stephei)DMAttrValueInit(DUIMenuAttr,INT_maxwidth)DMAttrValueInit(DUIMenuAttr,bool_bshadow)DMAttrValueInit(DUIMenuAttr,bool_bhideframe)DMAttrValueInit(DUIMenuAttr,SIZE_iconsize)
	DMAttrValueInit(DUIMenuAttr,SIZE_roundsize)DMAttrValueInit(DUIMenuAttr,COLOR_clrbg)DMAttrValueInit(DUIMenuAttr,COLOR_clrselbg)DMAttrValueInit(DUIMenuAttr,COLOR_clrstep)
	DMAttrValueInit(DUIMenuAttr,COLOR_clrtext)DMAttrValueInit(DUIMenuAttr,COLOR_clrtextsel)DMAttrValueInit(DUIMenuAttr,COLOR_clrtextgray)
	DMAttrValueInit(DUIMenuAttr,FONT_font)DMAttrValueInit(DUIMenuAttr,BYTE_alpha)DMAttrValueInit(DUIMenuAttr,STRING_transid)
	DMAttrValueInit(DUIMenuAttr,NODE_sep)DMAttrValueInit(DUIMenuAttr,NODE_item)
	DMAttrValueInit(DUIMenuAttr,ITEM_height)DMAttrValueInit(DUIMenuAttr,ITEM_maxwidth)
	DMAttrValueInit(DUIMenuAttr,ITEM_icon)DMAttrValueInit(DUIMenuAttr,ITEM_text)
	DMAttrValueInit(DUIMenuAttr,ITEM_iconsize)DMAttrValueInit(DUIMenuAttr,ITEM_iconoffset)DMAttrValueInit(DUIMenuAttr,ITEM_textoffset)
	DMAttrValueInit(DUIMenuAttr,ITEM_skin)DMAttrValueInit(DUIMenuAttr,ITEM_id)
	DMAttrValueInit(DUIMenuAttr,ITEM_bcheck)DMAttrValueInit(DUIMenuAttr,ITEM_bradio)DMAttrValueInit(DUIMenuAttr,ITEM_bdisable)

}

namespace DM
{
#define		MN_SETHMENU						0x01E0  
#define		MN_GETHMENU						0x01E1  
#define		MN_SIZEWINDOW					0x01E2  
#define		MN_OPENHIERARCHY				0x01E3  
#define		MN_CLOSEHIERARCHY				0x01E4   
#define		MN_SELECTITEM					0x01E5 
#define		MN_CANCELMENUS					0x01E6  
#define		MN_SELECTFIRSTVALIDITEM			0x01E7  


#define		MN_GETPPOPUPMENU				0x01EA  
#define		MN_FINDMENUWINDOWFROMPOINT		0x01EB  
#define		MN_SHOWPOPUPWINDOW				0x01EC  
#define		MN_BUTTONDOWN					0x01ED  
#define		MN_MOUSEMOVE					0x01EE  
#define		MN_BUTTONUP						0x01EF  

#define		MN_SETTIMERTOOPENHIERARCHY		0x01F0  
#define		MN_DBLCLK						0x01F1  
#define		MN_ENDMENU						0x01F2  
#define		MN_DODRAGDROP					0x01F3  
//#define		MN_ENDMENU						0x01F4   
#define DMMENU_PROP_OBJ		_T("DMMENU_PROP_OBJ")
	//-------------------------------------------------------------
	class DUIMenu_XmlInfo:public DMDataBase
	{
	public:
		DUIMenu_XmlInfo();

	public: 
		DM_BEGIN_ATTRIBUTES() 
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMenuAttr::SKIN_bgskin, m_pBgSkin,DM_ECODE_OK)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMenuAttr::SKIN_itemselbgskin,m_pItemSelBgSkin,DM_ECODE_OK)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMenuAttr::SKIN_iconskin,m_pIconSkin,DM_ECODE_OK)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMenuAttr::SKIN_sepskin,m_pSepSkin,DM_ECODE_OK)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMenuAttr::SKIN_checkskin,m_pCheckSkin,DM_ECODE_OK)

			DM_INT_ATTRIBUTE(DMAttr::DUIMenuAttr::INT_itemhei,m_nItemHei,DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIMenuAttr::INT_iconoffset,m_IconOffset,DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIMenuAttr::INT_textoffset,m_TextOffset,DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIMenuAttr::INT_stepoffset,m_StepOffset,DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIMenuAttr::INT_stephei,m_StepHei,DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIMenuAttr::INT_maxwidth,OnAttrMaxWidth)
			DM_bool_ATTRIBUTE(DMAttr::DUIMenuAttr::bool_bshadow,m_bShadow,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIMenuAttr::bool_bhideframe,m_bHideFrame,DM_ECODE_OK)
			DM_SIZE_ATTRIBUTE(DMAttr::DUIMenuAttr::SIZE_iconsize,m_szIcon,DM_ECODE_OK)
			DM_SIZE_ATTRIBUTE(DMAttr::DUIMenuAttr::SIZE_roundsize,m_szRound,DM_ECODE_OK)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMenuAttr::COLOR_clrbg,m_crBg,DM_ECODE_OK);
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMenuAttr::COLOR_clrselbg,m_crSelBg,DM_ECODE_OK);
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMenuAttr::COLOR_clrstep,m_crStep,DM_ECODE_OK);
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMenuAttr::COLOR_clrtext,m_crTextNormal,DM_ECODE_OK);
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMenuAttr::COLOR_clrtextsel,m_crTextSel,DM_ECODE_OK);
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMenuAttr::COLOR_clrtextgray,m_crTextGray,DM_ECODE_OK);
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIMenuAttr::FONT_font,m_hFont,DM_ECODE_OK)
			DM_UINT_ATTRIBUTE(DMAttr::DUIMenuAttr::BYTE_alpha, m_byAlpha, DM_ECODE_OK)
			DM_WSTRING_ATTRIBUTE(DMAttr::DUIMenuAttr::STRING_transid,m_strTransId,DM_ECODE_OK)

		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttrMaxWidth(LPCSTR pszValue, bool bLoadXml);

	public:
		IDMSkin*                    m_pBgSkin;       ///< �����˵��ı���ͼ
		IDMSkin*					m_pItemSelBgSkin;///< ѡ�в˵����Ƥ��
		IDMSkin*					m_pIconSkin;     ///< �˵�ͼ��
		IDMSkin*					m_pSepSkin;		 ///< �ָ���Ƥ��
		IDMSkin*					m_pCheckSkin;    ///< ѡ��״̬,��������״̬:��ѡ+Ȧѡ
		int							m_nItemHei;      ///< �˵���߶�
		int							m_IconOffset;    ///< ͼ�����Եƫ��
		int							m_TextOffset;    ///< �ı����Եƫ��
		int						    m_StepOffset;    ///< Step���Եƫ��
		int                         m_StepHei;       ///< Step�ĸ߶ȣ�Ĭ��Ϊ1
		int                         m_MaxWidth;		 ///< �˵���������,Ĭ��Ϊ-1��ʾ�Զ����㣬Ҫע����ƵĿ��Ҫ������14��ϵͳ�Զ���չ14,��ǿ�ƽ�ֹ,���Ӳ˵��͸��˵�����14��϶��
		bool                        m_bAutoCalc;
		DMColor                     m_crBg;          ///< �����˵��ı���ɫ,�ڱ���ͼ������ʱʹ��
		DMColor                     m_crSelBg;       ///< ѡ�в˵��ı���ɫ,�ڱ���ͼ������ʱʹ��
		DMColor                     m_crStep;        ///< �ָ����ı���ɫ
		DMColor						m_crTextNormal;  ///< �����ı���ɫ
		DMColor						m_crTextSel;     ///< ѡ���ı���ɫ
		DMColor					    m_crTextGray;    ///< ���ı���ɫ
		CSize						m_szIcon;        ///< ͼ��ߴ�
		CSize                       m_szRound;       ///< Բ�ǳߴ�
		BYTE						m_byAlpha;		 ///< ͸����
		bool                        m_bShadow;       ///< �˵���Ӱ
		bool                        m_bHideFrame;    ///< ���ز˵����߿�
		DMSmartPtrT<IDMFont>        m_hFont;         ///< ����
		CStringW                    m_strTransId;    ///< ���԰���Node��ʶ
	};

	struct DMMenuItemInfo
	{
		DMMenuItemInfo(){nHeight = 0; maxWidth = -1; iIcon = -1; pSkin = NULL; iconOffset = 0; textOffset = 0; dwData = 0;}
		int				nHeight;
		int             maxWidth;                     ///< �˵���������
		int				iIcon;						  ///< iconλ��
		IDMSkin*		pSkin;						  ///< ͼƬ
		CStringW		strText;					  ///< ����
		CSize           szIcon;                       ///< ͼ��ߴ�
		int             iconOffset;                   ///< ͼ�����Եƫ��
		int				textOffset;                   ///< �ı����Եƫ��
		DWORD           dwData;                       ///< ��������
	};

	struct DMMenuItemData
	{
		HMENU		   hMenu;
		UINT_PTR       nID;
		DMMenuItemInfo itemInfo;
	};

	/// <summary>
	///		 ��治�Է��Ի�ϵͳ�˵����д���,���账��,��ʹ��skinhgy lib��
	/// </summary>
	class DM_EXPORT DUIMenuItem
	{
	public:
		DUIMenuItem();
		virtual ~DUIMenuItem();

	public:
		virtual void OnRelease();
		virtual LRESULT MenuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		virtual void Render(HDC hdc = NULL);
		virtual void DrawSeparator(IDMCanvas* pCanvas, CRect& rcItem);
		virtual void DrawOwnerItem(IDMCanvas* pCanvas, CRect& rcItem, MENUITEMINFO &mii);
		virtual void DrawIconSub(IDMCanvas* pCanvas, RECT& rcItem, MENUITEMINFO& mii);

	public:
		LRESULT On_WM_NCDESTROY(WPARAM wParam, LPARAM lParam);
		LRESULT On_WM_NCPAINT(WPARAM wParam,LPARAM lParam);
		LRESULT On_WM_WINDOWPOSCHANGING(WPARAM wParam,LPARAM lParam);
		LRESULT On_WM_CREATE(WPARAM wParam, LPARAM lParam);
		LRESULT On_WM_KEYFIRST(WPARAM wParam, LPARAM lParam);
		LRESULT On_MN_SELECTITEM(WPARAM wParam, LPARAM lParam);
		LRESULT On_WM_PAINT(WPARAM wParam, LPARAM lParam);
		LRESULT On_WM_PRINT(WPARAM wParam, LPARAM lParam);
		LRESULT On_MN_SIZEWINDOW(WPARAM wParam,LPARAM lParam);
		LRESULT On_WM_SIZE(WPARAM wParam, LPARAM lParam);	

	public:
		void AddHook(HWND hWnd, WNDPROC WndProc);
		inline bool IsMenuWindowExist()
		{
			return NULL!=m_hWnd && ::IsWindow(m_hWnd);
		}

	public:
		static LRESULT CALLBACK SubMenuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		HMENU									m_hMenu;
		HWND									m_hWnd;			///< �����ؼ����
		HWND									m_hMenuOwner;   ///< DUI����������
		WNDPROC									m_pOldProc;		///< �ɵĴ��ڹ���
		WNDPROC								    m_pNowProc;		///< Ŀǰ�Ĵ��ڹ���

		bool									m_bAlpha;		///< �Ƿ�������͸��
		DWORD									m_dwCurSel;
		DMSmartPtrT<DUIMenu_XmlInfo>			m_pDUIMenuXmlInfo;
	};

}//namespace DM