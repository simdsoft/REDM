//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIListBoxEx.h 
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
#include "DUIItemPanel.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIListBoxEx"/>的xml属性定义
	/// </summary>
	class DUIListBoxExAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* INT_itemheight;                                          ///< 项的默认高度,此值不支持动态变化,示例:itemheight="20"
		static wchar_t* INT_cursel;                                              ///< 当前选中项,示例:cursel="1"
		static wchar_t* COLOR_clritembg;                                         ///< item项的背景色,示例:clritembg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemhoverbg;									 ///< item项的停留背景色,示例:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemselbg;                                      ///< item项的选中背景色,示例:clritemselbg="pbgra(ff,ff,ff,ff)"

		static wchar_t* NODE_item;                                                ///< 项结点 
		static wchar_t* ITEM_height;                                              ///< 项的高度(可选),示例:height="10"
		static wchar_t* ITEM_data;                                                ///< 项的数据,示例:data="10"
	};
	DMAttrValueInit(DUIListBoxExAttr,INT_itemheight)DMAttrValueInit(DUIListBoxExAttr,INT_cursel)DMAttrValueInit(DUIListBoxExAttr,COLOR_clritembg)
	DMAttrValueInit(DUIListBoxExAttr,COLOR_clritemhoverbg)DMAttrValueInit(DUIListBoxExAttr,COLOR_clritemselbg)DMAttrValueInit(DUIListBoxExAttr,NODE_item)
	DMAttrValueInit(DUIListBoxExAttr,ITEM_height)DMAttrValueInit(DUIListBoxExAttr,ITEM_data)
}

namespace DM
{
	typedef struct stLBITEMEX
	{
		stLBITEMEX(DMXmlNode XmlNode, IDMItemPanelOwner* pItemOwner)
		{
			pPanel.Attach(new DUIItemPanel(XmlNode,pItemOwner));
			lParam   = NULL;
			nHeight  = 0;
		}
		~stLBITEMEX()
		{
			pPanel.Release();
		}
		DMSmartPtrT<DUIItemPanel>       pPanel;     ///< 面板
		int								nHeight;	///< 高度
		LPARAM							lParam;     ///< data
	}LBITEMEX,*LPLBITEMEX;


	/// <summary>
	///		 DUIListBoxEx的内置实现
	/// </summary>
	class DM_EXPORT DUIListBoxEx:public DUIScrollBase
		,public IDMItemPanelOwner
		,public DMArrayT<LPLBITEMEX>
		,public DMMapT<int,CRect>		///< 可视列表
	{
		DMDECLARE_CLASS_NAME(DUIListBoxEx, DUINAME_ListBoxEx,DMREG_Window)
	public:
		DUIListBoxEx();

	public:
		//---------------------------------------------------
		// Function Des: 对外接口 methods
		//---------------------------------------------------
		int InsertItem(int nIndex, DMXmlNode&XmlNode, bool bUpdate = true);
		int GetTopIndex() const;
		int SetTopIndex(int nIndex);
		int GetItemHeight(int nIndex);
		int GetAllItemHeight();
		int SetAllItemHeight(int cyItemHeight, bool bUpdate = true);
		int SetItemHeight(int nIndex, int cyItemHeight, bool bUpdate = true);
		bool SetCurSel(int nIndex);
		bool SetCurHover(int nIndex);
		int GetCurSel() const;
		int GetCount();
		void DeleteItem(int nIndex);
		void DeleteAllItems(bool bUpdate = true);
		void EnsureVisible(int nIndex);
		LPARAM GetItemData(int nIndex);
		bool SetItemData(int nIndex, LPARAM lParam);

		//---------------------------------------------------
		// Function Des: IDMItemPanelOwner methods
		//---------------------------------------------------
		virtual DUIWindow* GetOwnerWindow();   
		virtual DMCode OnSetCapture(DUIItemPanel* pPanel);
		virtual DMCode OnReleaseCapture(DUIItemPanel* pPanel);
		virtual DMCode OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect);

		//---------------------------------------------------
		// Function Des: Draw methods
		//---------------------------------------------------
		virtual void DrawItem(IDMCanvas* pCanvas, CRect& rcItem, int iItem);
		virtual void RedrawItem(int iItem);
		DMCode OnScrollEvent(DMEventArgs *pEvt);

		//---------------------------------------------------
		// Function Des: DV methods
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
		virtual DMCode DV_OnSetCursor(const CPoint &pt);
		virtual CRect GetItemRect(int iItem);														///< 计算某项区域(和rcClient处同一坐标系）
		virtual int HitTest(CPoint &pt);															///< 自动修改pt的位置为相对当前项的偏移量
		virtual void UpdateScrollRange();	
		virtual void UpdateVisibleMap();
		virtual void PreArrayObjRemove(const LPLBITEMEX &obj);

	public:// 辅助
		//---------------------------------------------------
		// Function Des: 辅助 methods
		//---------------------------------------------------
		void UpdateItemPanelId(int iFirst=0, int iLast = -1);										///< 更新索引
		void ReLayoutVisibleItems();																///< 更新可见的所有项
		void RelayoutItem(int iItem,CRect rcItem);													///< 更新单项的布局
		void ModifyPanelBgClr(DUIItemPanel* pPanel);												///< 改变Panel的背景色
		void ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr);									///< 旧版接口，保留

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_SIZE(OnSize)
			MSG_WM_DESTROY(OnDestroy)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
			MSG_WM_CHAR(OnChar)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
			MSG_WM_KEYDOWN(OnKeyDown)
			MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST,WM_MOUSELAST,OnMouseEvent)
			MESSAGE_RANGE_HANDLER_EX(WM_KEYFIRST,WM_KEYLAST,OnKeyEvent)
			MESSAGE_RANGE_HANDLER_EX(WM_IME_STARTCOMPOSITION,WM_IME_KEYLAST,OnKeyEvent)
			MESSAGE_HANDLER_EX(WM_IME_CHAR,OnKeyEvent)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnSize(UINT nType, CSize size);
		void OnDestroy();
		void DM_OnSetFocus();
		void DM_OnKillFocus();
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags);
		LRESULT OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
		LRESULT OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIListBoxExAttr::INT_itemheight, m_iDefItemHei,		DM_ECODE_OK)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxExAttr::COLOR_clritembg,m_crItemBg[0],	    DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxExAttr::COLOR_clritemhoverbg,m_crItemBg[1],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxExAttr::COLOR_clritemselbg,m_crItemBg[2],  DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeCurSel(LPCWSTR lpszValue, bool bLoadXml);

	public:
		int									   m_iSelItem;						 ///< 当前选中项
		int									   m_iHoverItem;					 ///< 当前停留项
		int									   m_iDefItemHei;					 ///< 项的默认高度
		DMColor								   m_crItemBg[3];					 ///< 背景三连色
		DMSmartPtrT<DUIItemPanel>			   m_pCapturePanel;					 ///< 当前调用了setcapture的面板,此面板只在鼠标按下的瞬间设置，弹起清空
	};

}//namespace DM

