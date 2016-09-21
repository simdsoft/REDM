//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: TGPListBoxEx.h  
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
#include "DMDragWnd.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::TGPListBoxEx"/>的xml属性定义
	/// </summary>
	class TGPListBoxExAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* INT_itemheight;                                           ///< 项的默认高度,示例:itemheight="10"
		static wchar_t* INT_cursel;                                               ///< 当前选项,示例:cursel="1"
		static wchar_t* COLOR_clritembg;                                          ///< item项的背景色,示例:clritembg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemselbg;                                       ///< item项的选中背景色,示例:clritemselbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* bool_bnodrawbg;                                           ///< 为QQdemo设置，不抓取背景，不然可能透明

		static wchar_t* NODE_item;                                                ///< 项结点 
		static wchar_t* ITEM_height;                                              ///< 项的高度(可选),示例:height="10"
		static wchar_t* ITEM_data;                                                ///< 项的数据,示例:data="10"
		static wchar_t* bool_bswapline;											  ///< 允许交换行,允许拖动交换行,示例:bswapline="1"
	};
	DMAttrValueInit(TGPListBoxExAttr,INT_itemheight)DMAttrValueInit(TGPListBoxExAttr,INT_cursel)DMAttrValueInit(TGPListBoxExAttr,COLOR_clritembg)
	DMAttrValueInit(TGPListBoxExAttr,bool_bnodrawbg)
	DMAttrValueInit(TGPListBoxExAttr,COLOR_clritemselbg)DMAttrValueInit(TGPListBoxExAttr,NODE_item)DMAttrValueInit(TGPListBoxExAttr,ITEM_height)
	DMAttrValueInit(TGPListBoxExAttr,ITEM_data)DMAttrValueInit(TGPListBoxExAttr,bool_bswapline)
}   
 
namespace DM  
{
	typedef struct stTGPLBITEMEX
	{
		stTGPLBITEMEX(DMXmlNode XmlNode, IDMItemPanelOwner* pItemOwner)
		{
			pPanel.Attach(new DUIItemPanel(XmlNode,pItemOwner));
			lParam   = NULL;
			nHeight  = 0;
		}
		~stTGPLBITEMEX() 
		{
			pPanel.Release();
		}
		DMSmartPtrT<DUIItemPanel>       pPanel;     ///< 面板
		int								nHeight;	///< 高度
		DMXmlDocument					XmlDoc;   ///< 备份XML
		LPARAM							lParam;     ///< data
	}TGPLBITEMEX,*LPTGPLBITEMEX;
  
	/// <summary>
	///		关闭事件
	/// </summary>
#define  DMEVT_TGPLISTCLOSE_CMD    24001
	class DMTgpListCloseUpEventCmdArgs:public DMEventArgs
	{
	public:
		DMTgpListCloseUpEventCmdArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
		enum{EventID = DMEVT_TGPLISTCLOSE_CMD};
		UINT GetEventID(){return EventID;};
		LPCSTR GetEventName(){return EVEIDNAME(DMEVT_TGPLISTCLOSE_CMD);}
	};

	/// <summary>
	///		 TGPListBoxEx的内置实现
	/// </summary>
	class TGPListBoxEx:public DUIScrollBase
		,public IDMItemPanelOwner
		,public DMArrayT<LPTGPLBITEMEX>
	{
		DMDECLARE_CLASS_NAME(TGPListBoxEx, L"TGPListBoxEx",DMREG_Window)
	public:
		TGPListBoxEx();
		~TGPListBoxEx();  

	public:
		//---------------------------------------------------
		// Function Des: 接口
		//---------------------------------------------------
		int InsertItem(int nIndex, DMXmlNode&XmlNode);
		int GetItemHeight(int nIndex) const; 
		int GetAllItemHeight(); 
		int SetAllItemHeight(int cyItemHeight, bool bUpdate = true);
		int SetItemHeight(int nIndex, int cyItemHeight, bool bUpdate = true);
		int GetTopIndex() const;
		int SetTopIndex(int nIndex); 
		CRect GetItemRect(int iItem);
		bool SetCurSel(int nIndex);
		int GetCurSel() const;
		void DeleteItem(int iItem);
		void DeleteAllItems(bool bUpdate = true);
		void EnsureVisible(int nIndex); 

	public:// 绘制！！！！
		virtual void DrawItem(IDMCanvas* pCanvas, CRect& rc, int iItem);
		void RedrawItem(int iItem);

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
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
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
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
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
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTALLKEYS;};
		void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);							  ///<  更新光标位置

	public:
		void PreArrayObjRemove(const LPTGPLBITEMEX &obj);

	public:// 辅助
		void UpdateItemPanelId(int iFirst=0, int iLast = -1);							  ///< 更新索引
		void UpdateAllPanelLayout();                                                      ///< 更新Panel布局
		void ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr);                         ///< 改变Panel的背景色
		int HitTest(CPoint &pt);														  ///< 自动修改pt的位置为相对当前项的偏移量
		void UpdateScrollRangeSize();

		void CreateDragCanvas(UINT iItem);												  ///< 创建画布
		void DrawDraggingState(DWORD dwDragTo);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::TGPListBoxExAttr::INT_itemheight, m_iDefItemHei, DM_ECODE_OK)
			DM_COLOR_ATTRIBUTE(DMAttr::TGPListBoxExAttr::COLOR_clritembg,m_crItemBg, DM_ECODE_OK)
			DM_COLOR_ATTRIBUTE(DMAttr::TGPListBoxExAttr::COLOR_clritemselbg,m_crItemSelBg,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::TGPListBoxExAttr::bool_bnodrawbg,m_bNoDrawBg, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::TGPListBoxExAttr::bool_bswapline,m_bSwapLine, DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()

	public:
		int									   m_iSelItem;						 ///< 当前选中项   
		int									   m_iHoverItem;					 ///< 当前停留项
		int									   m_iDefItemHei;					 ///< 项的默认高度
		DMColor								   m_crItemBg; 
		DMColor							       m_crItemSelBg; 
		DMSmartPtrT<DUIItemPanel>			   m_pCapturePanel;					 ///< 当前调用了setcapture的面板,此面板只在鼠标按下的瞬间设置，弹起清空
		bool                                   m_bNoDrawBg;     

		// 拖动行
		bool                                   m_bSwapLine;	
		bool								   m_bDragging;						///< 正在拖动标志
		DMSmartPtrT<IDMCanvas>				   m_pDragCanvas;					///< 显示拖动窗口的临时画布
		DWORD								   m_dwHitTest;						///< 拖动位置所在项
		DWORD								   m_dwDragTo;						///< 拖放目标所在项    
		CPoint								   m_ptClick;						///< 当前点击坐标
	};
}//namespace DM

