//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUITreeEx.h 
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
	///		<see cref="DM::DUITreeEx"/>的xml属性定义
	/// </summary>
	class DUITreeAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* SKIN_toggleskin;								    ///< 展开、收起小三角图案,六种状态(收起三种+展开三种),示例:toggleskin="treetoggle"
		static wchar_t* SKIN_checkskin;                                     ///< 选中、不选中，半选中图案,用于启用了checkbox状态，9种状态(正常三种+选中三种+半选中三种),示例:checkskin="treecheck"
		static wchar_t* SKIN_eyeskin;                                       ///< 9种状态(正常三种+选中三种+半选中三种),示例:eyeskin="treecheck"
		static wchar_t* SKIN_lockskin;                                      ///< 9种状态(正常三种+选中三种+半选中三种),示例:lockskin="treecheck"
		
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
	DMAttrValueInit(DUITreeAttr,SKIN_toggleskin)DMAttrValueInit(DUITreeAttr,SKIN_checkskin)DMAttrValueInit(DUITreeAttr,SKIN_eyeskin)DMAttrValueInit(DUITreeAttr,SKIN_lockskin)
		DMAttrValueInit(DUITreeAttr,SKIN_itembgskin)DMAttrValueInit(DUITreeAttr,COLOR_clritembg)DMAttrValueInit(DUITreeAttr,COLOR_clritemhoverbg)
		DMAttrValueInit(DUITreeAttr,COLOR_clritemselbg)DMAttrValueInit(DUITreeAttr,INT_childoffset)DMAttrValueInit(DUITreeAttr,INT_itemheight)DMAttrValueInit(DUITreeAttr,INT_itemwidth)
		DMAttrValueInit(DUITreeAttr,bool_brightclicksel)DMAttrValueInit(DUITreeAttr,bool_bcheckbox)DMAttrValueInit(DUITreeAttr,NODE_treeitem)DMAttrValueInit(DUITreeAttr,ITEM_bcollapsed)
		DMAttrValueInit(DUITreeAttr,ITEM_height)DMAttrValueInit(DUITreeAttr,ITEM_width)DMAttrValueInit(DUITreeAttr,ITEM_childoffset)DMAttrValueInit(DUITreeAttr,ITEM_data)
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
		DMTVEXBtn_Eye,
		DMTVEXBtn_Lock,
	};

	enum 
	{
		DMTVEXCheckBox_UnChecked,			///< 未选中
		DMTVEXCheckBox_Checked,				///< 选中
		DMTVEXCheckBox_PartChecked			///< 部分选中
	};

	enum 
	{
		DMTVEXLock_UnLocked,				///< 未锁定
		DMTVEXLock_Locked,					///< 锁定
		DMTVEXLock_PartLocked				///< 部分锁定
	};
 
	enum 
	{ 
		DMTVEXEye_Eyed,						///< 可见
		DMTVEXEye_UnEye,					///< 不可见
		DMTVEXEye_PartUnEyed				///< 部分不可见
	};

	/// <summary>
	///		
	/// </summary>
	typedef struct stTVITMEX
	{
		stTVITMEX(DMXmlNode XmlNode, IDMItemPanelOwner* pItemOwner)
		{
			pPanel.Attach(new DUIItemPanel(XmlNode,pItemOwner));
			hItem = iHeight = iChildOffset = nLevel = lParam = NULL;
			bVisible = true;
			iWidth = 20; 
			iCheckValue = DMTVEXCheckBox_UnChecked;
			iLockValue = DMTVEXLock_UnLocked;
			iEyeValue = DMTVEXEye_Eyed;
			bCollapsed = true;
			bHasChildren = false;
			dwToggleState = DUIWNDSTATE_Normal;
			dwCheckBoxState	= DUIWNDSTATE_Normal;
			dwLockState = DUIWNDSTATE_Normal;
			dwEyeState = DUIWNDSTATE_Normal;
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
		int                             iLockValue;      ///< lock的当前状态(无锁，半锁，解锁)
		int                             iEyeValue;       ///< eye的当前状态(不可见,半可见,可见)
		DWORD							dwCheckBoxState; ///< checkbox状态(normal,hover,sel)
		DWORD							dwToggleState;	 ///< 折叠小三角的状态(normal,hover,sel)
		DWORD                           dwLockState;     ///< lock的的状态(normal,hover,sel)
		DWORD                           dwEyeState;      ///< eye的的状态(normal,hover,sel)
		
		bool							bCollapsed;      ///< 折叠状态
		bool							bVisible;		 ///< 是否可见
		bool							bHasChildren;    ///< 是否存在子树
		LPARAM							lParam;			 ///< data
	}TVITEMEX,*LPTVITEMEX;


	/// <summary>
	///		 DUITreeEx的内置实现，属性：<see cref="DMAttr::DUITreeAttr"/>
	/// </summary>
	class DUITreeEx 
		: public DUIScrollBase
		, public IDMItemPanelOwner
		, public DMTreeT<LPTVITEMEX>
	{
		DMDECLARE_CLASS_NAME(DUITreeEx, DUINAME_TreeCtrlEx,DMREG_Window)
	public:
		DUITreeEx();
		~DUITreeEx();

	public:
		//---------------------------------------------------
		// Function Des: 对外接口
		//---------------------------------------------------
		bool RemoveAllItems();																			///< 移除所有结点
		bool RemoveItem(HDMTREEITEM hItem);
		bool SetItemRect(HDMTREEITEM hItem, CRect rcItem);												///< 设置某项的区域

		///---------------------------------
		/// 插入函数
		HDMTREEITEM InsertItem(LPTVITEMEX pData,HDMTREEITEM hParent,HDMTREEITEM hInsertAfter,BOOL bEnsureVisible);
		HDMTREEITEM InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,BOOL bEnsureVisible=FALSE);

		///---------------------------------
		/// 获取系列函数，可参考CTreeCtrl的成员函数列表
		HDMTREEITEM GetRootItem();
		HDMTREEITEM GetRootItem(HDMTREEITEM hItem);
		HDMTREEITEM GetNextSiblingItem(HDMTREEITEM hItem);
		HDMTREEITEM GetPrevSiblingItem(HDMTREEITEM hItem);
		HDMTREEITEM GetChildItem(HDMTREEITEM hItem,bool bFirst =true);
		HDMTREEITEM GetParentItem(HDMTREEITEM hItem);
		HDMTREEITEM GetSelectedItem();
		HDMTREEITEM GetNextItem(HDMTREEITEM hItem);
		bool SelectItem(HDMTREEITEM hItem,bool bEnsureVisible=true);
		bool HoverItem(HDMTREEITEM hItem,bool bEnsureVisible=true);
		bool ItemHasChildren(HDMTREEITEM hItem);
		bool SetItemData(HDMTREEITEM hItem, LPARAM lParam);
		LPARAM GetItemData(HDMTREEITEM hItem) const;

		bool GetCheckState(HDMTREEITEM hItem) const;    
		bool SetCheckState(HDMTREEITEM hItem,bool bCheck); 
		bool SetLockState(HDMTREEITEM hItem,bool bLock);
		bool SetEyeState(HDMTREEITEM hItem,bool bUnEye);

		bool Expand(HDMTREEITEM hItem,UINT nCode = DMTVEX_EXPAND);
		bool EnsureVisible(HDMTREEITEM hItem,bool bFirstVisible = false);

	public:// 绘制！！！！
		virtual void DrawItem(IDMCanvas* pCanvas,CRect& rc,HDMTREEITEM hItem);
		virtual void RedrawItem(HDMTREEITEM hItem);

	public: 
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_RBUTTONDOWN(OnRButtonDown);
		MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
			MSG_WM_CHAR(OnChar)
			MSG_WM_KEYDOWN(OnKeyDown)
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
		void OnDestroy();
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonDbClick(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnRButtonDown(UINT nFlags, CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void DM_OnSetFocus();
		void DM_OnKillFocus();
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags);
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		LRESULT OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
		LRESULT OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);

	public:
		//---------------------------------------------------
		// Function Des: 可重载函数
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_OnSetCursor(const CPoint &pt);
		virtual DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
		virtual void LoadBranch(HDMTREEITEM hParent,DMXmlNode &XmlItem);
		virtual void LoadItemAttribute(DMXmlNode &XmlItem, LPTVITEMEX pData);      	
		virtual void OnNodeFree(LPTVITEMEX &pItemData);
		virtual int GetScrollLineSize(bool bVert){return m_iDefItemHei;};


		// 抽象实现
		DUIWindow* GetOwnerWindow();   
		DMCode OnSetCapture(DUIItemPanel* pPanel);
		DMCode OnReleaseCapture(DUIItemPanel* pPanel);
		DMCode OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect);							///< 请使用*lpRect赋值！

	public:// 辅助
		void SetChildrenVisible(HDMTREEITEM hItem,bool bVisible);
		void SetChildrenState(HDMTREEITEM hItem,int iCheckValue);
		void CheckState(HDMTREEITEM hItem, bool bCheck,bool bCheckChild = true);  
		bool CheckChildrenState(HDMTREEITEM hItem,bool bCheck);    
		bool IsAncestor(HDMTREEITEM hItem1,HDMTREEITEM hItem2);         // hItem2是否为hItem1的子项
		bool ItemIsValid(HDMTREEITEM hItem);

		void ModifyToggleState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ModifyChekcBoxState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ModifyLockState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ModifyEyeState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ItemLButtonDown(HDMTREEITEM hItem, UINT nFlags,CPoint pt,CPoint ptsrc);
		void ItemLButtonUp(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemLButtonDbClick(HDMTREEITEM hItem, UINT nFlags,CPoint pt,CPoint ptsrc);
		void ItemMouseMove(HDMTREEITEM hItem, UINT nFlags,CPoint pt,CPoint ptsrc);
		void ItemMouseLeave(HDMTREEITEM hItem);

		// eye+lock
		bool IsParentLocked(HDMTREEITEM hItem);
		bool IsParentUnEyed(HDMTREEITEM hItem);
		void SetChildrenLockState(HDMTREEITEM hItem,bool bLocked);
		void SetChildrenEyeState(HDMTREEITEM hItem,bool bUnEye);


		// 位置相关
		HDMTREEITEM HitTest(CPoint &pt);													///< 自动修改pt的位置为相对当前项的偏移量(如处于XOffset那一段区域,则pt.x为负值)
		int ItemHitTest(HDMTREEITEM hItem,CPoint &pt, CPoint &ptsrc);						///<这里pt已被转换成为相对当前项的偏移量，ptsrc为原始的,只需判断x即可

		int GetTotalHeight();
		int GetTotalWidth();
		int GetItemHeightWithAllChild(HDMTREEITEM hItem);									///< 计算某一项以及它的所有可见子项的总高度
		int GetItemWidthWithAllChild(HDMTREEITEM hItem);									///< 计算某一项以及它的所有可见子项的总宽度
		int GetItemXOffset(HDMTREEITEM hItem);                                              ///< 计算某项到根结点的偏移（不可见设置为0）
		int GetItemYOffset(HDMTREEITEM hItem);                                              ///< 计算某项到根结点的偏移（不可见设置为0）
		bool GetItemRect(HDMTREEITEM hItem,CRect &rcItem);									///< 计算某项区域(和rcClient处同一坐标系）
		bool GetToggleRect(HDMTREEITEM hItem,CRect &rcToggle);                              ///< 计算某项toggle区域(和rcClient处同一坐标系）
		bool GetCheckBoxRect(HDMTREEITEM hItem,CRect &rcCheckBox);                          ///< 计算某项checkbox区域(和rcClient处同一坐标系）
		bool GetLockRect(HDMTREEITEM hItem,CRect &rcLock);								///< 计算某项lock区域(和rcClient处同一坐标系）
		bool GetEyeRect(HDMTREEITEM hItem,CRect &rcEye);								///< 计算某项eye区域(和rcClient处同一坐标系）
		void UpdateScrollRangeSize();

	public:
		DM_BEGIN_ATTRIBUTES()  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_toggleskin, m_pToggleSkin, DM_ECODE_NOXMLLOADREFRESH)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_checkskin, m_pCheckSkin, DM_ECODE_NOXMLLOADREFRESH)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_eyeskin, m_pEyeSkin, DM_ECODE_NOXMLLOADREFRESH)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_lockskin, m_pLockSkin, DM_ECODE_NOXMLLOADREFRESH)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_itembgskin, m_pItemBgSkin, DM_ECODE_NOXMLLOADREFRESH)  

			DM_INT_ATTRIBUTE(DMAttr::DUITreeAttr::INT_childoffset, m_iDefChildOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITreeAttr::INT_itemheight,  m_iDefItemHei,     DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITreeAttr::INT_itemwidth,  m_iDefItemWid,     DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUITreeAttr::bool_brightclicksel, m_bRightClickSel, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUITreeAttr::bool_bcheckbox, m_bCheckBox, DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeAttr::COLOR_clritembg,		m_crItemBg[0],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeAttr::COLOR_clritemhoverbg,m_crItemBg[1],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeAttr::COLOR_clritemselbg,	m_crItemBg[2],DM_ECODE_NOXMLLOADREFRESH)
			DM_END_ATTRIBUTES()

	public:
		IDMSkinPtr					m_pToggleSkin;			///< 小三角6连图
		IDMSkinPtr					m_pCheckSkin;			///< checkbox9连图
		IDMSkinPtr		            m_pEyeSkin;
		IDMSkinPtr                  m_pLockSkin;
		IDMSkinPtr					m_pItemBgSkin;			///< 背景三连图
		DMColor					    m_crItemBg[3];			///< 背景三连色

		int						    m_iDefItemHei;		    ///< 项的默认高度
		int						    m_iDefItemWid;		    ///< 项的默认宽度
		int							m_iDefChildOffset;      ///< 子项相对父项默认偏移

		bool						m_bRightClickSel;       ///< 是否支持右击选中
		bool						m_bCheckBox;			///< 是否启用checkbox



		// 辅助
		HDMTREEITEM					m_hSelItem;				///< 当前选中项
		HDMTREEITEM					m_hHoverItem;           ///< 当前停留项
		HDMTREEITEM					m_hCaptureItem;         ///< 获得capture项(用于鼠标点击后再回复)

		// 
		int							m_nItemHoverBtn;		///< 对应小三角和checkbox
		int							m_nItemPushDownBtn;

		//
		DMSmartPtrT<DUIItemPanel>	m_pCapturePanel;		///< 当前调用了setcapture的面板,此面板只在鼠标按下的瞬间设置，弹起清空
	};

}//namespace DM