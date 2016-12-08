//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIListCtrlEx.h 
// File Des: ListCtrlEx扩展ListBoxEx，支持多列组合面板
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-13	1.0			
//-------------------------------------------------------
#pragma once
#include "DUIScrollBase.h"
#include "DUIItemPanel.h"
#include "DUIHeaderCtrl.h"
namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIListCtrlEx"/>的xml属性定义
	/// </summary>
	class DUIListCtrlExAttr: public DUIScrollBaseAttr
	{
	public:
		static wchar_t* INT_headerheight;                                        ///< 列表头的高度,示例:headerheight="20"
		static wchar_t* INT_itemheight;                                          ///< 项的默认高度,此值不支持动态变化,示例:itemheight="20"
		static wchar_t* INT_cursel;                                              ///< 当前选中项,示例:cursel="1"
		static wchar_t* COLOR_clritembg;                                         ///< item项的背景色,示例:clritembg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemselbg;                                      ///< item项的选中背景色,示例:clritemselbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* bool_bhottrack;                                          ///< 让指针下的项目自动高亮显示,示例:bhottrack="1"

		static wchar_t* NODE_item;                                               ///< listctrl的xml项结点名
		static wchar_t* ITEM_height;                                             ///< 项的高度(可选), 示例:height="20"
		static wchar_t* ITEM_data;                                               ///< 项的数据,示例:data="10"
	};
	DMAttrValueInit(DUIListCtrlExAttr,INT_headerheight)DMAttrValueInit(DUIListCtrlExAttr,INT_itemheight)
	DMAttrValueInit(DUIListCtrlExAttr,INT_cursel)DMAttrValueInit(DUIListCtrlExAttr,COLOR_clritembg)
	DMAttrValueInit(DUIListCtrlExAttr,COLOR_clritemselbg)DMAttrValueInit(DUIListCtrlExAttr,bool_bhottrack)
	DMAttrValueInit(DUIListCtrlExAttr,NODE_item)DMAttrValueInit(DUIListCtrlExAttr,ITEM_height)
	DMAttrValueInit(DUIListCtrlExAttr,ITEM_data)
}

namespace DM
{
	typedef struct stLCITEMEX
	{
		stLCITEMEX(DMXmlNode XmlNode, IDMItemPanelOwner* pItemOwner)
		{
			pPanel.Attach(new DUIItemPanel(XmlNode,pItemOwner));
			lParam   = NULL;
			nHeight  = 0;
		}
		~stLCITEMEX()
		{
			pPanel.Release();
		}
		DMSmartPtrT<DUIItemPanel>       pPanel;     ///< 面板
		int								nHeight;	///< 高度
		LPARAM							lParam;     ///< data
	}LCITEMEX,*LPLCITEMEX;

	/// <summary>
	///		 DUIListCtrlEx的内置实现
	/// </summary>
	class DM_EXPORT DUIListCtrlEx:public DUIScrollBase
								 ,public IDMItemPanelOwner
								 ,public DMArrayT<LPLCITEMEX>
	{
		DMDECLARE_CLASS_NAME(DUIListCtrlEx, DUINAME_ListCtrlEx,DMREG_Window)
	public:
		DUIListCtrlEx();
		~DUIListCtrlEx();

	public:
		//---------------------------------------------------
		// Function Des: 接口
		//---------------------------------------------------
		int InsertItem(int nIndex, DMXmlNode&XmlNode);
		int GetTopIndex() const;
		int SetTopIndex(int nIndex);
		int GetTotalHeight();
		int GetColumnCount();
		CRect GetItemRect(int iItem);
		bool SetCurSel(int nIndex);
		int GetCurSel() const;
		void DeleteItem(int iItem);
		void DeleteAllItems(bool bUpdate = true);
		void EnsureVisible(int nIndex);
		LPARAM GetItemData(int nIndex);
		bool SetItemData(int nIndex, LPARAM lParam);

	public:// 绘制！！！！
		virtual void DrawItem(IDMCanvas* pCanvas, CRect& rc, int iItem);
		void RedrawItem(int iItem);

	public:// 交互消息
		DMCode OnHeaderSizeChanging(DMEventArgs *pEvt);
		DMCode OnHeaderSwap(DMEventArgs *pEvt);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)

			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_CHAR(OnChar)
			MSG_WM_SIZE(OnSize)
			MSG_WM_NCCALCSIZE(OnNcCalcSize)
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
		void OnDestroy();																	///< 必须在此处释放panel，不然panel无法释放！！
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnSize(UINT nType,CSize size);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void DM_OnSetFocus();
		void DM_OnKillFocus();
		void OnMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags);
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
		LRESULT OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
		LRESULT OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);

	public:
		//---------------------------------------------------
		// Function Des: IDMItemPanelOwner实现
		//---------------------------------------------------   
		DUIWindow* GetOwnerWindow();   
		DMCode OnSetCapture(DUIItemPanel* pPanel);
		DMCode OnReleaseCapture(DUIItemPanel* pPanel);
		DMCode OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect);							///< 请使用*lpRect赋值！

	public:
		//---------------------------------------------------
		// Function Des: 可重载函数
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
		DMCode DV_OnSetCursor(const CPoint &pt);
		void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);							  ///<  更新光标位置
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTALLKEYS;};
		bool OnScroll(bool bVert,UINT uCode,int nPos);                                     ///< 滚动时调整header的位置
		void UpdateScrollBar();

	public:
		void PreArrayObjRemove(const LPLCITEMEX &obj);
		DMCode OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr);

	public:// 辅助
		void UpdateItemPanelId(int iFirst=0, int iLast = -1);							   ///< 更新索引
		void UpdateAllPanelLayout();                                                       ///< 更新Panel布局
	    void ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr);                          ///< 改变Panel的背景色
	    void UpdateHeaderCtrl();
		void SetLCScrollRange();
		CRect GetListRect();
		int HitTest(CPoint &pt);
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::INT_itemheight, m_iDefItemHei,		  DM_ECODE_OK)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::COLOR_clritembg,m_crItemBg,		  DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::COLOR_clritemselbg,m_crItemSelBg,	  DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::bool_bhottrack,m_bHotTrack,		  DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::INT_headerheight,OnAttributeHeaderHei)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::INT_cursel,OnAttributeCurSel)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeHeaderHei(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeCurSel(LPCWSTR lpszValue, bool bLoadXml);
	public:
		int									   m_iSelItem;						 ///< 当前选中项   
		int									   m_iHoverItem;					 ///< 当前停留项
		int									   m_iHeaderHei;					 ///< 列表头高度
		int									   m_iDefItemHei;					 ///< 项的默认高度
		DMColor								   m_crItemBg; 
		DMColor							       m_crItemSelBg; 
		DMSmartPtrT<DUIItemPanel>			   m_pCapturePanel;					 ///< 当前调用了setcapture的面板
		DMSmartPtrT<DUIHeaderCtrl>             m_pHeaderCtrl;                     
		bool								   m_bHotTrack;  
	};

}//namespace DM