//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIListBox.h 
// File Des: ListBox
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-26	1.0			
//-------------------------------------------------------
#pragma once
#include "DUIScrollBase.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIListBox"/>的xml属性定义
	/// </summary>
	class DUIListBoxAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* INT_itemheight;                                       ///< 项的默认高度,默认为20,示例:itemheight="20"
		static wchar_t* POINT_iconpoint;                                      ///< 相对于Item的左上角偏移,默认为-1，-1，其中-1表示使用默认高度的1/6,示例:iconpoint="-1,-1"
		static wchar_t* POINT_textpoint;                                      ///< 相对于Item的左上角偏移，默认为-1，-1，其中-1表示此方向居中,示例:textpoint="-1,-1"
		static wchar_t* SKIN_iconskin;                                        ///< 每项自带的ICON图案,一般为状态连图,示例:iconskin="skin"
		static wchar_t* SKIN_itembgskin;                                      ///< 项所在的背景图案,示例：itembgskin="skin"
		static wchar_t* COLOR_clritembg;                                      ///< 单数项item的背景色,示例:clritembg="pbgra(FF,FF,FF,FF)"
		static wchar_t* COLOR_clritembg2;                                     ///< 双数项item的背景色,如不设置,则默认使用单数项的背景色,示例:clritembg2="pbgra(FF,FF,cc,FF)"
		static wchar_t* COLOR_clritemhoverbg;                                 ///< item项的hover背景色,示例:clritemhoverbg="pbgra(ff,00,00,ff)"
		static wchar_t* COLOR_clritemselbg;                                   ///< item项的选中背景色,示例:clritemselbg="pbgra(ff,00,00,ff)"
		static wchar_t* COLOR_clritemtext;                                    ///< item项的文本色,示例:clritemtext="pbgra(ff,00,00,ff)"
		static wchar_t* COLOR_clritemhovertext;                                ///< item项的hover文本色,示例:clritemhoverext="pbgra(ff,00,00,ff)"
		static wchar_t* COLOR_clritemseltext;                                 ///< item项的选中文本色,示例:clritemseltext="pbgra(ff,00,00,ff)"
		static wchar_t* bool_bhottrack;                                       ///< 让指针下的项自动追踪选中显示,示例:bhottrack="1"
		static wchar_t* INT_cursel;                                           ///< 此项为选中项，示例:cursel="1"
		//
		static wchar_t* NODE_item;                                            ///< ListBox的xml项结点名
		static wchar_t* ITEM_height;                                          ///< 项的高度(可选),不指定则使用默认项,height="30"
		static wchar_t* ITEM_text;                                            ///< 项的文本,示例:text="item0"
		static wchar_t* ITEM_icon;                                            ///< 项的icon索引,-1表示不使用icon图,示例:icon="1"
		static wchar_t* ITEM_data;                                            ///< 项的附加数据,示例:data="100"
	};
	DMAttrValueInit(DUIListBoxAttr,INT_itemheight)DMAttrValueInit(DUIListBoxAttr,POINT_iconpoint)DMAttrValueInit(DUIListBoxAttr,POINT_textpoint)
	DMAttrValueInit(DUIListBoxAttr,SKIN_iconskin)DMAttrValueInit(DUIListBoxAttr,SKIN_itembgskin)DMAttrValueInit(DUIListBoxAttr,COLOR_clritembg)
	DMAttrValueInit(DUIListBoxAttr,COLOR_clritemhoverbg)DMAttrValueInit(DUIListBoxAttr,COLOR_clritembg2)DMAttrValueInit(DUIListBoxAttr,COLOR_clritemselbg)
	DMAttrValueInit(DUIListBoxAttr,COLOR_clritemtext)DMAttrValueInit(DUIListBoxAttr,COLOR_clritemhovertext)DMAttrValueInit(DUIListBoxAttr,COLOR_clritemseltext)
	DMAttrValueInit(DUIListBoxAttr,bool_bhottrack)DMAttrValueInit(DUIListBoxAttr,NODE_item)DMAttrValueInit(DUIListBoxAttr,ITEM_height)
	DMAttrValueInit(DUIListBoxAttr,ITEM_text)DMAttrValueInit(DUIListBoxAttr,ITEM_icon)DMAttrValueInit(DUIListBoxAttr,ITEM_data)DMAttrValueInit(DUIListBoxAttr,INT_cursel)
}

namespace DM
{
	typedef struct stLBITEM
	{
		stLBITEM()
		{
			nImage = -1;
			lParam = NULL;
		}
		CStringW    strText;    ///< 文本
		int			nHeight;	///< 高度
		int         nImage;		///< icon标志
		LPARAM      lParam;     ///< data
		void assign(stLBITEM* pItem)
		{
			strText = (LPCWSTR)pItem->strText;
			nHeight = pItem->nHeight;
			nImage = pItem->nImage;
			lParam = pItem->lParam;
		}
	}LBITEM, *LPLBITEM;

	/// <summary>
	///		 DUIListBox的内置实现
	/// </summary>
	class DM_EXPORT DUIListBox: public DUIScrollBase,public DMArrayT<LPLBITEM>
	{
		DMDECLARE_CLASS_NAME(DUIListBox, DUINAME_ListBox,DMREG_Window)
	public:
		DUIListBox();
		~DUIListBox();
		void Init();
		void UnInit();
	public:
		//---------------------------------------------------
		// Function Des: 接口
		//---------------------------------------------------
		int InsertItem(int nIndex, LPLBITEM pItem);
		int GetTopIndex() const;
		int SetTopIndex(int nIndex);
		int GetItemHeight() const;
		int GetItemHeight(int nIndex) const;
		int SetItemHeight(int nIndex, int cyItemHeight);
		int GetText(int nIndex, CStringW& strText);
		int GetTextLen(int nIndex) ;
		int DeleteString(int nIndex);
		int AddString(LPCWSTR lpszItem, int nHei=-1, int nImage = -1, LPARAM lParam = 0);
		int InsertString(int nIndex, LPCTSTR lpszItem, int nHei=-1, int nImage = -1, LPARAM lParam = 0);
		LPARAM GetItemData(int nIndex);
		bool SetItemData(int nIndex, LPARAM lParam);
		bool SetCurSel(int nIndex);
		int GetCurSel() const;
		void DeleteAll();
		void EnsureVisible(int nIndex);

	public:// 绘制！！！！
		virtual void DrawItem(IDMCanvas* pCanvas, CRect& rc, int iItem);
		void RedrawItem(int iItem);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_SIZE(OnSize)
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_CHAR(OnChar)
			MSG_WM_SHOWWINDOW(OnShowWindow)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void OnDestroy();
		void OnSize(UINT nType,CSize size);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonDbClick(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void OnKeyDown(TCHAR nChar,UINT nRepCnt,UINT nFlags);          // 支持上下箭头控制项
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnShowWindow(BOOL bShow, UINT nStatus);

	public:
		//---------------------------------------------------
		// Function Des: 可重载函数
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);

	public:// 辅助
		int HitTest(CPoint &pt);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIListBoxAttr::INT_itemheight, m_nDefItemHei, DM_ECODE_NOXMLLOADREFRESH)

			DM_POINT_ATTRIBUTE(DMAttr::DUIListBoxAttr::POINT_iconpoint,m_IconPt, DM_ECODE_NOXMLLOADREFRESH)
			DM_POINT_ATTRIBUTE(DMAttr::DUIListBoxAttr::POINT_textpoint,m_TextPt, DM_ECODE_NOXMLLOADREFRESH)
			
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIListBoxAttr::SKIN_iconskin,   m_pIconSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIListBoxAttr::SKIN_itembgskin, m_pItemBgSkin,DM_ECODE_NOXMLLOADREFRESH)

			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritembg,m_crItemBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritembg2, m_crItemBg2,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemhoverbg,m_crItemHoverBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemselbg,m_crItemSelBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemtext,m_crItemText,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemhovertext,m_crItemHoverText,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemseltext,m_crItemSelText,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIListBoxAttr::bool_bhottrack,m_bHotTrack, DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIListBoxAttr::INT_cursel,OnAttributeCurSel)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeCurSel(LPCWSTR lpszValue, bool bLoadXml);
	public:
		int									   m_iSelItem;						 ///< 当前选中项   
		int									   m_iHoverItem;					 ///< 当前停留项
		int									   m_nDefItemHei;					 ///< 项的高度

		IDMSkinPtr							   m_pItemBgSkin;
		IDMSkinPtr							   m_pIconSkin;

		DMColor								   m_crItemBg; 
		DMColor							       m_crItemBg2;
		DMColor                                m_crItemHoverBg;
		DMColor							       m_crItemSelBg; 
		DMColor							       m_crItemText;  
		DMColor                                m_crItemHoverText;
		DMColor					               m_crItemSelText; 

		CPoint								   m_IconPt;						///< 相对于Item的左上角偏移
		CPoint								   m_TextPt;						///< 相对于Item的左上角偏移，默认为-1，-1，其中-1表示此方向居中 

		bool		    					   m_bHotTrack; 
	};

}//namespace DM