// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIList.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-10
// ----------------------------------------------------------------
#pragma once
#include "DUIScrollBase.h"
#include "DUIItemPanel.h"  
#include "DMDragWnd.h"     
      
namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIList"/>的xml属性定义
	/// </summary>
	class DUIListAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* INT_itemheight;                                           ///< 项的默认高度,示例:itemheight="10"
		static wchar_t* INT_cursel;                                               ///< 当前选项,示例:cursel="1"
		static wchar_t* SKIN_expandarrskin;                                       ///< 收缩，展开小三角,示例:expandarrskin="skin"
	
		static wchar_t* NODE_item;                                                ///< 项结点 
		static wchar_t* ITEM_height;                                              ///< 项的高度(可选),示例:height="10"
		static wchar_t* ITEM_bexpand;                                             ///< 项是否初始展开,示例:bexpand="1"
		static wchar_t* ITEM_expandheight;                                        ///< 项的展开高度,示例:expandheight="100"
		static wchar_t* ITEM_data;                                                ///< 项的数据,示例:data="10"
		static wchar_t* bool_bswapline;											  ///< 允许交换行,允许拖动交换行,示例:bswapline="1"
		
	};
	DMAttrValueInit(DUIListAttr,INT_itemheight)DMAttrValueInit(DUIListAttr,INT_cursel)DMAttrValueInit(DUIListAttr,SKIN_expandarrskin)
	DMAttrValueInit(DUIListAttr,NODE_item)DMAttrValueInit(DUIListAttr,ITEM_height)DMAttrValueInit(DUIListAttr,ITEM_bexpand)DMAttrValueInit(DUIListAttr,ITEM_expandheight)
	DMAttrValueInit(DUIListAttr,ITEM_data)DMAttrValueInit(DUIListAttr,bool_bswapline)
}   

namespace DM  
{
	typedef struct stDSLBITEMEX
	{
		stDSLBITEMEX(DMXmlNode XmlNode, IDMItemPanelOwner* pItemOwner)
		{
			pPanel.Attach(new DUIItemPanel(XmlNode,pItemOwner));
			lParam   = NULL;
			nHeight  = nExpandHeight = 0;
			bExpand  = false;
		}
		~stDSLBITEMEX() 
		{
			pPanel.Release();
		}
		DMSmartPtrT<DUIItemPanel>       pPanel;			///< 面板
		int								nHeight;		///< 高度
		int                             nExpandHeight;  ///< 项的展开高度
		bool                            bExpand;        ///< 是否展开
		DMXmlDocument					XmlDoc;			///< 备份XML
		LPARAM							lParam;			///< data
	}DSLBITEMEX,*LPDSLBITEMEX;

	/// <summary>
	///		 DUIList的内置实现
	/// </summary> 
	class DUIList:public DUIScrollBase
		,public IDMItemPanelOwner
		,public DMArrayT<LPDSLBITEMEX>
	{
		DMDECLARE_CLASS_NAME(DUIList, L"DUIList",DMREG_Window)
	public: 
		DUIList();  
		~DUIList();    
 
	public:              
		//---------------------------------------------------
		// Function Des: 接口
		//---------------------------------------------------
		int InsertItem(int nIndex, DMXmlNode&XmlNode);
		CRect GetItemRect(int iItem);
		bool SetCurSel(int nIndex);
		int GetCurSel() const;
		void DeleteItem(int iItem);
		void DeleteAllItems(bool bUpdate = true);
		void EnsureVisible(int nIndex); 
		bool ExpandItem(int nIndex,bool bExpand = true);

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
		DMCode DV_OnSetCursor(const CPoint &pt);
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTALLKEYS;};
		void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);							  ///<  更新光标位置

	public:
		void PreArrayObjRemove(const LPDSLBITEMEX &obj);

	public:// 辅助
		void UpdateItemPanelId(int iFirst=0, int iLast = -1);							  ///< 更新索引
		void UpdateAllPanelLayout();                                                      ///< 更新Panel布局
		void NotifySelChange(int nOldSel,int nNewSel);
		void ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr);                         ///< 改变Panel的背景色
		int HitTest(CPoint &pt,bool* pbInExpand=NULL);										  ///< 自动修改pt的位置为相对当前项的偏移量
		void UpdateScrollRangeSize();

		void CreateDragCanvas(UINT iItem);												  ///< 创建画布
		void DrawDraggingState(DWORD dwDragTo);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIListAttr::INT_itemheight, m_iDefItemHei, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIListAttr::bool_bswapline,m_bSwapLine, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIListAttr::SKIN_expandarrskin,m_pExpandArrSkin,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()

	public:
		int									   m_iSelItem;						 ///< 当前选中项   
		int									   m_iHoverItem;					 ///< 当前停留项
		int									   m_iDefItemHei;					 ///< 项的默认高度
		DMSmartPtrT<DUIItemPanel>			   m_pCapturePanel;					 ///< 当前调用了setcapture的面板,此面板只在鼠标按下的瞬间设置，弹起清空 
		IDMSkinPtr                             m_pExpandArrSkin;

		// 拖动行
		bool                                   m_bSwapLine;	
		bool								   m_bDragging;						///< 正在拖动标志
		DMSmartPtrT<IDMCanvas>				   m_pDragCanvas;					///< 显示拖动窗口的临时画布
		DWORD								   m_dwHitTest;						///< 拖动位置所在项
		DWORD								   m_dwDragTo;						///< 拖放目标所在项    
		CPoint								   m_ptClick;						///< 当前点击坐标
	};
}//namespace DM

