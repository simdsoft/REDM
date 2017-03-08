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
	///		<see cref="DM::DUIMenu"/>的xml属性定义
	/// </summary>
	class DUIMenuAttr
	{
	public:
		static wchar_t* SKIN_bgskin;											///< 整个菜单的背景图,示例:bgskin="menubg"
		static wchar_t* SKIN_itemselbgskin;										///< 选中菜单项背景皮肤,示例:itemselbgskin="menuselbg"
		static wchar_t* SKIN_iconskin;											///< 菜单图标连图,示例:iconskin="menuicons"
		static wchar_t* SKIN_sepskin;											///< 分割栏皮肤,示例:sepskin="sepskin"
		static wchar_t* SKIN_checkskin;											///< 选中状态,包含两种状态:勾选+圈选,示例:checkSkin="menucheck"
		static wchar_t* INT_itemhei;											///< 菜单项高度,不设置时默认为16,如菜单项含有图片或高度，则以图片或高度的最大值为准,示例:itemhei="16"
		static wchar_t* INT_iconoffset;											///< 图标左边缘偏移,示例:iconoffset="10"
		static wchar_t* INT_textoffset;											///< 文本左边缘偏移,示例:textoffset="10"
		static wchar_t* INT_stepoffset;											///< Step左边缘偏移,示例:stepoffset="10"
		static wchar_t* INT_stephei;											///< Step的高度，默认为1,示例:stephei="10"
		static wchar_t* INT_maxwidth;											///< 菜单项的最大宽度,默认为-1表示自动计算,示例:maxwidth="10",要注意绘制窗口宽度会比这个设置大14（系统设置的）
		static wchar_t* bool_bshadow;											///< 菜单阴影是否开启，默认为0,示例:bshadow="0"
		static wchar_t* bool_bhideframe;                                        ///< 是否隐藏菜单的主边框,默认为0,如果隐藏,则菜单项item的左右和主边框的左右相同,示例:bhideframe="1"
		static wchar_t* SIZE_iconsize;											///< 图标尺寸，默认为16*16,示例:iconsize="16,16"
		static wchar_t* SIZE_roundsize;											///< 圆角尺寸，默认为0*0,示例:roundsize="1,1"
		static wchar_t* COLOR_clrbg;											///< 整个菜单的背景色,在背景图不存在时使用,示例:clrbg="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrselbg;                                         ///< 选中菜单的背景色,在背景图不存在时使用,示例:clrselbg="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrstep;											///< 分割栏的背景色,在背景图不存在时使用,示例:clrstep="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrtext;											///< 正常文本颜色,示例:clrtext="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrtextsel;										///< 选中文本颜色,示例:clrtextsel="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrtextgray;										///< 灰度文本颜色,示例:clrtextgray="pbgra(ff,00,ff,ff)"
		static wchar_t* FONT_font;												///< 菜单默认字体,目前菜单不同状态使用了相同字体,如有需要,可自己扩展,示例:font="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		static wchar_t* BYTE_alpha;											    ///< 窗口透明度,透明窗口有两种,详见http://hgy413.com/1865.html 示例:alpha="0xc0"
		static wchar_t* STRING_transid;										    ///< 使用的transid,为NULL使用默认custom 示例:transid="custom"

		static wchar_t* NODE_sep;												///< 菜单的xml项结点名(对应一条分隔栏)
		static wchar_t* NODE_item;                                              ///< 菜单的xml项结点名(对应一条菜单项)
		static wchar_t* ITEM_height;                                            ///< 菜单项的高度(可选)，如果菜单项高度小于菜单项的背景连图的高度，则以背景连图高度为准, 示例:height="20"
		static wchar_t* ITEM_icon;                                              ///< 菜单项的icon位置（-1表示没有icon）示例:icon="1"
		static wchar_t* ITEM_text;                                              ///< 菜单项的文本  示例:text="打开PE文件..."
		static wchar_t* ITEM_skin;                                              ///< 菜单项的背景连图(正常-选中-灰掉)  示例:skin="itemskin"
		static wchar_t* ITEM_id;												///< 菜单项的id(不能为0,在1到65535之间)，用于点击时发送消息  示例:id="10"
		static wchar_t* ITEM_bcheck;                                            ///< 菜单项的check项（配合SKIN_checkskin）  示例:bcheck="1"
		static wchar_t* ITEM_bradio;                                            ///< 菜单项的radio项（配合SKIN_checkskin，和check项二选一）  示例:bradio="1"
		static wchar_t* ITEM_bdisable;                                          ///< 禁用菜单项  示例:bdisable="1"
	};
	DMAttrValueInit(DUIMenuAttr,SKIN_bgskin)DMAttrValueInit(DUIMenuAttr,SKIN_itemselbgskin)DMAttrValueInit(DUIMenuAttr,SKIN_iconskin)
	DMAttrValueInit(DUIMenuAttr,SKIN_sepskin)DMAttrValueInit(DUIMenuAttr,SKIN_checkskin)DMAttrValueInit(DUIMenuAttr,INT_itemhei)
	DMAttrValueInit(DUIMenuAttr,INT_iconoffset)DMAttrValueInit(DUIMenuAttr,INT_textoffset)DMAttrValueInit(DUIMenuAttr,INT_stepoffset)
	DMAttrValueInit(DUIMenuAttr,INT_stephei)DMAttrValueInit(DUIMenuAttr,INT_maxwidth)DMAttrValueInit(DUIMenuAttr,bool_bshadow)DMAttrValueInit(DUIMenuAttr,bool_bhideframe)DMAttrValueInit(DUIMenuAttr,SIZE_iconsize)
	DMAttrValueInit(DUIMenuAttr,SIZE_roundsize)DMAttrValueInit(DUIMenuAttr,COLOR_clrbg)DMAttrValueInit(DUIMenuAttr,COLOR_clrselbg)DMAttrValueInit(DUIMenuAttr,COLOR_clrstep)
	DMAttrValueInit(DUIMenuAttr,COLOR_clrtext)DMAttrValueInit(DUIMenuAttr,COLOR_clrtextsel)DMAttrValueInit(DUIMenuAttr,COLOR_clrtextgray)
	DMAttrValueInit(DUIMenuAttr,FONT_font)DMAttrValueInit(DUIMenuAttr,BYTE_alpha)DMAttrValueInit(DUIMenuAttr,STRING_transid)
	DMAttrValueInit(DUIMenuAttr,NODE_sep)DMAttrValueInit(DUIMenuAttr,NODE_item)
	DMAttrValueInit(DUIMenuAttr,ITEM_height)DMAttrValueInit(DUIMenuAttr,ITEM_icon)DMAttrValueInit(DUIMenuAttr,ITEM_text)
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
			DM_STRING_ATTRIBUTE(DMAttr::DUIMenuAttr::STRING_transid,m_strTransId,DM_ECODE_OK)

		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttrMaxWidth(LPCWSTR pszValue, bool bLoadXml);

	public:
		IDMSkin*                    m_pBgSkin;       ///< 整个菜单的背景图
		IDMSkin*					m_pItemSelBgSkin;///< 选中菜单项背景皮肤
		IDMSkin*					m_pIconSkin;     ///< 菜单图标
		IDMSkin*					m_pSepSkin;		 ///< 分割栏皮肤
		IDMSkin*					m_pCheckSkin;    ///< 选中状态,包含两种状态:勾选+圈选
		int							m_nItemHei;      ///< 菜单项高度
		int							m_IconOffset;    ///< 图标左边缘偏移
		int							m_TextOffset;    ///< 文本左边缘偏移
		int						    m_StepOffset;    ///< Step左边缘偏移
		int                         m_StepHei;       ///< Step的高度，默认为1
		int                         m_MaxWidth;		 ///< 菜单项的最大宽度,默认为-1表示自动计算，要注意绘制的宽度要比它大14（系统自动扩展14,如强制禁止,则子菜单和父菜单间有14空隙）
		bool                        m_bAutoCalc;
		DMColor                     m_crBg;          ///< 整个菜单的背景色,在背景图不存在时使用
		DMColor                     m_crSelBg;       ///< 选中菜单的背景色,在背景图不存在时使用
		DMColor                     m_crStep;        ///< 分割栏的背景色
		DMColor						m_crTextNormal;  ///< 正常文本颜色
		DMColor						m_crTextSel;     ///< 选中文本颜色
		DMColor					    m_crTextGray;    ///< 灰文本颜色
		CSize						m_szIcon;        ///< 图标尺寸
		CSize                       m_szRound;       ///< 圆角尺寸
		BYTE						m_byAlpha;		 ///< 透明度
		bool                        m_bShadow;       ///< 菜单阴影
		bool                        m_bHideFrame;    ///< 隐藏菜单主边框
		DMSmartPtrT<IDMFont>        m_hFont;         ///< 文字
		CStringW                    m_strTransId;    ///< 语言包的Node标识
	};

	struct DMMenuItemInfo
	{
		int				nHeight;
		int				iIcon;						  ///< icon位置
		IDMSkin*		pSkin;						  ///< 图片
		CStringW		strText;					  ///< 文字
		DWORD           dwData;                       ///< 其他数据
	};

	struct DMMenuItemData
	{
		HMENU		   hMenu;
		UINT_PTR       nID;
		DMMenuItemInfo itemInfo; 
	};

	/// <summary>
	///		 简版不对非自绘系统菜单进行处理,如需处理,请使用skinhgy lib库
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
		HWND									m_hWnd;			///< 窗体或控件句柄
		HWND									m_hMenuOwner;   ///< DUI的宿主窗口
		WNDPROC									m_pOldProc;		///< 旧的窗口过程
		WNDPROC								    m_pNowProc;		///< 目前的窗口过程

		bool									m_bAlpha;		///< 是否设置了透明
		DWORD									m_dwCurSel;
		DMSmartPtrT<DUIMenu_XmlInfo>			m_pDUIMenuXmlInfo;
	};

}//namespace DM