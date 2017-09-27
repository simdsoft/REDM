//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUITreeCtrlEx.h 
// File Des: 
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
#include "DMTreeT.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUITreeCtrlEx"/>的xml属性定义
	/// </summary>
	class DUITreeCtrlExAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* SKIN_toggleskin;								    ///< 展开、收起小三角图案,六种状态(收起三种+展开三种),示例:toggleskin="treetoggle"
		static wchar_t* SKIN_checkskin;                                     ///< 选中、不选中，半选中图案,用于启用了checkbox状态，9种状态(正常三种+选中三种+半选中三种),示例:checkskin="treecheck"
		static wchar_t* SKIN_itembgskin;                                    ///< 项所在的背景图案,正常/停留/选中,示例:itembgskin="itembgskin"
		static wchar_t* COLOR_clritembg;                                    ///< item项的背景色,示例:clritembg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemhoverbg;                               ///< item项的停留背景色,示例:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemselbg;                                 ///< item项的选中背景色,示例:clritemselbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* INT_childoffset;                                    ///< 子项相对于父项的偏移,可被具体项覆盖,示例:childoffset="20"
		static wchar_t* INT_itemheight;									    ///< 每项高度,可被具体项覆盖,示例:itemheight="20"
		static wchar_t* INT_itemwidth;									    ///< 每项宽度,可被具体项覆盖,示例:itemwidth="20"
		static wchar_t* bool_brightclicksel;                                ///< 是否支持右击选中,示例:brightclicksel="1"
		static wchar_t* bool_bcheckbox;                                     ///< 是否启用checkbox,示例:bcheckbox="1"

		static wchar_t* NODE_treeitem;										///< treectrl的xml项结点名
		static wchar_t* ITEM_bcollapsed;									///< 项是否收缩,示例:bcollapsed="1"
		static wchar_t* ITEM_height;                                        ///< 项的高度(可选),示例:height="10"
		static wchar_t* ITEM_width;                                         ///< 项的宽度(可选),示例:weidth="10"
		static wchar_t* ITEM_childoffset;                                   ///< 项的宽度(可选),示例:childoffset="10"
		static wchar_t* ITEM_data;											///< 项的数据,示例:data="1"
	};
	DMAttrValueInit(DUITreeCtrlExAttr,SKIN_toggleskin)DMAttrValueInit(DUITreeCtrlExAttr,SKIN_checkskin)DMAttrValueInit(DUITreeCtrlExAttr,SKIN_itembgskin)DMAttrValueInit(DUITreeCtrlExAttr,COLOR_clritembg)DMAttrValueInit(DUITreeCtrlExAttr,COLOR_clritemhoverbg)
		DMAttrValueInit(DUITreeCtrlExAttr,COLOR_clritemselbg)DMAttrValueInit(DUITreeCtrlExAttr,INT_childoffset)DMAttrValueInit(DUITreeCtrlExAttr,INT_itemheight)DMAttrValueInit(DUITreeCtrlExAttr,INT_itemwidth)
		DMAttrValueInit(DUITreeCtrlExAttr,bool_brightclicksel)DMAttrValueInit(DUITreeCtrlExAttr,bool_bcheckbox)DMAttrValueInit(DUITreeCtrlExAttr,NODE_treeitem)DMAttrValueInit(DUITreeCtrlExAttr,ITEM_bcollapsed)
		DMAttrValueInit(DUITreeCtrlExAttr,ITEM_height)DMAttrValueInit(DUITreeCtrlExAttr,ITEM_width)DMAttrValueInit(DUITreeCtrlExAttr,ITEM_childoffset)DMAttrValueInit(DUITreeCtrlExAttr,ITEM_data)
}

namespace DM
{
	enum 
	{
		DMTVEX_COLLAPSE = 1,///< 收缩
		DMTVEX_EXPAND,	   ///< 展开
		DMTVEX_TOGGLE,	   ///< 自动，原来是展开就收缩
	};

	enum ///< 绘制属性
	{
		DMTVEXMask_Toggle         = 0x00000001UL, 
		DMTVEXMask_CheckBox       = 0x00000002UL, 
	};

	enum ///< 鼠标是否停留在小三角的矩形区域
	{
		DMTVEXBtn_None,					               
		DMTVEXBtn_Toggle, 
		DMTVEXBtn_CheckBox,
	};

	enum 
	{
		DMTVEXCheckBox_UnChecked,			///< 未选中
		DMTVEXCheckBox_Checked,				///< 选中
		DMTVEXCheckBox_PartChecked			///< 部分选中
	};

	/// <summary>
	///		
	/// </summary>
	typedef struct stTVITMEX
	{
		stTVITMEX(DMXmlNode XmlNode, IDMItemPanelOwner* pItemOwner)
		{
			pPanel.Attach(new DUIItemPanel(XmlNode,pItemOwner));
			hItem = iHeight = iChildOffset = nLevel = NULL;
			lParam = NULL;
			bVisible = true;
			iWidth = 20; 
			iCheckValue = DMTVEXCheckBox_UnChecked;
			bCollapsed = true;
			bHasChildren = false;
			dwToggleState = DUIWNDSTATE_Normal;
			dwCheckBoxState	= DUIWNDSTATE_Normal;
		}
		virtual~stTVITMEX()
		{
			pPanel.Release();
		}
		DMSmartPtrT<DUIItemPanel>       pPanel;			 ///< 面板
		HDMTREEITEM						hItem;			 ///< 在树中的标识  
		int								nLevel;			 ///< 所处的层次	
		int								iHeight;		 ///< 高度
		int								iWidth;			 ///< 宽度
		int                             iChildOffset;    ///< 子项相对于父项的偏移,在有toggleskin时,在计算时会自动加上toggleskin和checkskin的宽度（首项也会被全局的ichildoffset影响到,所以首项一般需自己配个ichildoffset=0）
		int								iCheckValue;	 ///< checkbox当前状态（正常,选中,半选中）
		DWORD							dwCheckBoxState; ///< checkbox状态(normal,hover,sel)
		DWORD							dwToggleState;	 ///< 折叠小三角的状态(normal,hover,sel)
		bool							bCollapsed;      ///< 折叠状态
		bool							bVisible;		 ///< 是否可见
		bool							bHasChildren;    ///< 是否存在子树
		LPARAM							lParam;			 ///< data
	}TVITEMEX,*LPTVITEMEX;


	/// <summary>
	///		 DUITreeCtrlEx的内置实现，属性：<see cref="DMAttr::DUITreeCtrlExAttr"/>
	/// </summary>
	class DM_EXPORT DUITreeCtrlEx 
		: public DUIScrollBase
		, public IDMItemPanelOwner
		, public DMTreeT<LPTVITEMEX>
		, public DMMapT<HDMTREEITEM,CRect>
	{
		DMDECLARE_CLASS_NAME(DUITreeCtrlEx, DUINAME_TreeCtrlEx,DMREG_Window)
	public:
		DUITreeCtrlEx();

	public:
		//---------------------------------------------------
		// Function Des: 对外接口 methods
		//---------------------------------------------------
		bool RemoveAllItems();
		bool RemoveItem(HDMTREEITEM hItem);
		HDMTREEITEM InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=false);
		bool EnsureVisible(HDMTREEITEM hItem,bool bFirstVisible = false);
		bool Expand(HDMTREEITEM hItem,UINT nCode = DMTVEX_EXPAND);
		bool SetCheckState(HDMTREEITEM hItem,bool bCheck);   
		bool SelectItem(HDMTREEITEM hItem,bool bEnsureVisible=true);

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
		virtual void DrawItem(IDMCanvas* pCanvas,CRect& rc,HDMTREEITEM hItem);
		virtual void RedrawItem(HDMTREEITEM hItem);

		//---------------------------------------------------
		// Function Des: DV methods
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_OnSetCursor(const CPoint &pt);
		virtual DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
		virtual bool DV_IsSupportFastRefresh();
		virtual void LoadBranch(HDMTREEITEM hParent,DMXmlNode &XmlItem);
		virtual void LoadItemAttribute(DMXmlNode &XmlItem, LPTVITEMEX pData);
		virtual bool OnScroll(bool bVert,UINT uCode,int nPos);
		virtual int GetScrollLineSize(bool bVert){return m_iDefItemHei;};
		virtual int GetItemWidth(HDMTREEITEM hItem);
		virtual int GetTotalHeight();
		virtual int GetTotalWidth();
		virtual int GetItemHeightWithAllChild(HDMTREEITEM hItem);									///< 计算某一项以及它的所有可见子项的总高度
		virtual int GetItemWidthWithAllChild(HDMTREEITEM hItem);									///< 计算某一项以及它的所有可见子项的总宽度
		virtual int GetItemXOffset(HDMTREEITEM hItem);                                              ///< 计算某项到根结点的偏移（不可见设置为0）
		virtual int GetItemYOffset(HDMTREEITEM hItem);                                              ///< 计算某项到根结点的偏移（不可见设置为0）
		virtual bool GetItemRect(HDMTREEITEM hItem,CRect &rcItem);									///< 计算某项区域(和rcClient处同一坐标系）
		virtual bool GetToggleRect(HDMTREEITEM hItem,CRect &rcToggle);                              ///< 计算某项toggle区域(和rcClient处同一坐标系）
		virtual bool GetCheckBoxRect(HDMTREEITEM hItem,CRect &rcCheckBox);                          ///< 计算某项checkbox区域(和rcClient处同一坐标系）
		virtual HDMTREEITEM HitTest(CPoint &pt);													///< 自动修改pt的位置为相对当前项的偏移量(如处于XOffset那一段区域,则pt.x为负值)
		virtual int ItemHitTest(HDMTREEITEM hItem,CPoint &pt);										///<这里pt已被转换成为相对当前项的偏移量，只需判断x即可
		virtual void UpdateScrollRange();
		virtual void UpdateVisibleMap();

		virtual void OnNodeFree(LPTVITEMEX &pItemData);

	public:// 辅助
		//---------------------------------------------------
		// Function Des: 辅助 methods
		//---------------------------------------------------
		HDMTREEITEM InsertItem(LPTVITEMEX pData,HDMTREEITEM hParent,HDMTREEITEM hInsertAfter,bool bEnsureVisible);
		void SetChildrenVisible(HDMTREEITEM hItem,bool bVisible);
		void SetChildrenState(HDMTREEITEM hItem,int iCheckValue);
		void CheckState(HDMTREEITEM hItem, bool bCheck,bool bCheckChild = true);  
		bool CheckChildrenState(HDMTREEITEM hItem,bool bCheck);			// 子树节点中有和bCheck不一样的状态,则返回false，全一致返回true
		bool IsAncestor(HDMTREEITEM hItem1,HDMTREEITEM hItem2);         // hItem2是否为hItem1的子项

		void ModifyToggleState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ModifyChekcBoxState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ItemLButtonDown(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemLButtonUp(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemLButtonDbClick(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemMouseMove(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemMouseLeave(HDMTREEITEM hItem);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_SIZE(OnSize)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_RBUTTONDOWN(OnRButtonDown);
		MSG_WM_MOUSELEAVE(OnMouseLeave)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
			MSG_WM_CHAR(OnChar)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
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
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonDbClick(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnRButtonDown(UINT nFlags, CPoint pt);
		void OnMouseLeave();
		void DM_OnSetFocus();
		void DM_OnKillFocus();
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags);
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		LRESULT OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
		LRESULT OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);


	public:
		DM_BEGIN_ATTRIBUTES()  
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::SKIN_toggleskin,OnAttributeToggleSkin)  
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::SKIN_checkskin,OnAttributeCheckSkin)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::SKIN_itembgskin, m_pItemBgSkin, DM_ECODE_NOXMLLOADREFRESH)  

			DM_INT_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::INT_childoffset, m_iDefChildOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::INT_itemheight,  m_iDefItemHei,     DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::INT_itemwidth,  m_iDefItemWid,     DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::bool_brightclicksel, m_bRightClickSel, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::bool_bcheckbox, m_bCheckBox, DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::COLOR_clritembg,		m_crItemBg[0],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::COLOR_clritemhoverbg,m_crItemBg[1],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlExAttr::COLOR_clritemselbg,	m_crItemBg[2],DM_ECODE_NOXMLLOADREFRESH)
			DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeToggleSkin(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttributeCheckSkin(LPCWSTR pszValue, bool bLoadXml);

	public:
		IDMSkinPtr								m_pToggleSkin;			///< 小三角6连图
		IDMSkinPtr								m_pCheckSkin;			///< checkbox9连图
		IDMSkinPtr								m_pItemBgSkin;			///< 背景三连图
		DMColor									m_crItemBg[3];			///< 背景三连色

		int										m_iDefItemHei;		    ///< 项的默认高度
		int										m_iDefItemWid;		    ///< 项的默认宽度
		int										m_iDefChildOffset;      ///< 子项相对父项默认偏移

		bool									m_bRightClickSel;       ///< 是否支持右击选中
		bool									m_bCheckBox;			///< 是否启用checkbox

		// 辅助
		HDMTREEITEM								m_hSelItem;				///< 当前选中项
		HDMTREEITEM								m_hHoverItem;           ///< 当前停留项
		HDMTREEITEM								m_hCaptureItem;         ///< 获得capture项(用于鼠标点击后再回复)

		// 
		int										m_nItemHoverBtn;		///< 对应小三角和checkbox
		int										m_nItemPushDownBtn;
		CSize                                   m_szCheck;				///< 记录checkbox的大小
		CSize                                   m_szToggle;				///< 记录Toggle的大小
		DMSmartPtrT<DUIItemPanel>				m_pCapturePanel;		///< 当前调用了setcapture的面板,此面板只在鼠标按下的瞬间设置，弹起清空
		DWORD                                   m_dwStartTime;
	};
}