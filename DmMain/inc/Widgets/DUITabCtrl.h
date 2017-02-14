//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUITabCtrl.h 
// File Des: TabCtrl
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-10	1.0			
//-------------------------------------------------------
#pragma once
#include "DMWidgetDef.h"
#include "DUIAnimate.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUITabPage"/>的xml属性定义
	/// </summary>
	class DUITabPageAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* OPTION_animatetype;										    ///< 动画类型,示例:animatetype="1",1:边展开,2:中心扩大,3:中心缩小
		static wchar_t* STRING_title;											    ///< tab页的标题,示例：title="标签1"
		static wchar_t* SKIN_itemskin;                                              ///< 标签项的皮肤，示例: itemskin="skin"
	};
	DMAttrValueInit(DUITabPageAttr,OPTION_animatetype)DMAttrValueInit(DUITabPageAttr,STRING_title)DMAttrValueInit(DUITabPageAttr,SKIN_itemskin)

	class DUITabCtrlAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* POINT_textpoint;                                           ///< 相对于主标签区域的左上角偏移，默认为-1，-1，其中-1表示此方向居中，示例textpoint="-1,-1"
		static wchar_t* OPTION_tabalign;										   ///< tab页的排列方式,分为top、left、right、buttom,示例:tabalign="top"
		static wchar_t* SKIN_mainbgskin;							               ///< 整个item(标签列表)所在区的背景,不包括page区,示例:mainbgskin="TabCtrl_Item"
		static wchar_t* SKIN_itembgskin;										   ///< 每个item(标签)的skin，即在DUITabPageAttr::SKIN_itemskin之下先绘一次,用于所有item的hover-sel为同一状态时简化截图,示例:itembgskin="skin"
		static wchar_t* INT_firstitemoffset;                                       ///< 第一个Item(标签)的相对于边框的偏移，示例:firstitemoffset="10"
		static wchar_t* INT_itemspace;                                             ///< Item之间的间距，示例:itemspace="10"
		static wchar_t* INT_cursel;                                                ///< 当前选中项, 示例:cursel="1"
		static wchar_t* INT_animatesteps;                                          ///< 整个动画图以多少次渐渐显示出来, 仅在设置了动画类型时有效，示例:animatesteps="20"
		static wchar_t* SIZE_itemsize;                                             ///< Item(标签)项的大小，示例:itemsize="78,30"
	};
	DMAttrValueInit(DUITabCtrlAttr,POINT_textpoint)DMAttrValueInit(DUITabCtrlAttr,OPTION_tabalign)DMAttrValueInit(DUITabCtrlAttr,SKIN_mainbgskin)DMAttrValueInit(DUITabCtrlAttr,SKIN_itembgskin)
	DMAttrValueInit(DUITabCtrlAttr,INT_firstitemoffset)DMAttrValueInit(DUITabCtrlAttr,INT_itemspace)DMAttrValueInit(DUITabCtrlAttr,INT_cursel)
	DMAttrValueInit(DUITabCtrlAttr,INT_animatesteps)DMAttrValueInit(DUITabCtrlAttr,SIZE_itemsize)
}

namespace DM
{
	enum{TAB_NULL=0,TAB_SLIDER,TAB_CINFLATE,TAB_CDEFLATE};
	/// <summary>
	///		 DUITabPage的内置实现，属性：<see cref="DMAttr::DUITabPageAttr"/>
	/// </summary>
	class DM_EXPORT DUITabPage:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIWindow,DUINAME_TabPage,DMREG_Window);
	public:
		DUITabPage();
	public:
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_STRING_ATTRIBUTE(DMAttr::DUITabPageAttr::STRING_title, m_strTitle, DM_ECODE_NOXMLLOADPARENTREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITabPageAttr::OPTION_animatetype, m_TabAniType, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITabPageAttr::SKIN_itemskin, m_pItemSkin, DM_ECODE_NOXMLLOADPARENTREFRESH)
		DM_END_ATTRIBUTES()
	public:
		CStringW											m_strTitle;           ///< ITEM标题
		int                                                 m_TabAniType;         ///< 动画类型
		DMSmartPtrT<IDMSkin>                                m_pItemSkin;          ///< 标签皮肤
	};

	/// <summary>
	///		 DUITabCtrl的内置实现，属性：<see cref="DMAttr::DUITabCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUITabCtrl:public DUIWindow,public IDMTabAnimateOwner
	{
		DMDECLARE_CLASS_NAME(DUIWindow,DUINAME_TabCtrl,DMREG_Window);
	public:
		DUITabCtrl();

		//---------------------------------------------------
		// Function Des: 对外接口
		//---------------------------------------------------
		int InsertItem(DMXmlNode &XmlNode, int iInsert=-1, bool bLoading=false);
		int GetCurSel();
		bool SetCurSel(int nIndex);
		bool SetCurSel(LPCWSTR lpszTitle);
		bool SetItemTitle(int nIndex, LPCWSTR lpszTitle);
		bool GetItemRect(int nIndex, CRect &rcItem);	
		CRect GetTitleRect();													///< 所有标签所在的大矩形，另一部分就是所有page所在的大矩形					
		int GetItemCount();
		DUITabPage* GetItem(int nIndex);
		bool RemoveItem(int nIndex, int nSelPage=0);
		bool RemoveItem(DUIWindow* pPage,int nSelPage=0);
		void RemoveAllItems(void);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_KEYDOWN(OnKeyDown)
		DM_END_MSG_MAP()

	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnDestroy();
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnMouseLeave();
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		int HitTest(CPoint pt);

	public:
		virtual void DrawItem(IDMCanvas* pCanvas,const CRect &rcItem,int iItem,DWORD dwState);

	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTARROWS;};	
		DMCode DV_GetChildMeasureLayout(LPRECT lpRect);	
		DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);

		//---------------------------------------------------
		// Function Des: 动画处理
		//---------------------------------------------------
		int OnTabAnimateType(int iOld, int iCur, int iType);
		DUIWindow* GetOwnerWindow(){return this;}
		DMCode OnTabAnimateFinish();

	public:
		enum{AlignTop,AlignLeft,AlignBottom,AlignRight};
		DM_BEGIN_ATTRIBUTES()
			DM_POINT_ATTRIBUTE(DMAttr::DUITabCtrlAttr::POINT_textpoint,m_TextPt, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITabCtrlAttr::SKIN_mainbgskin, m_pMainBgSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITabCtrlAttr::SKIN_itembgskin, m_pItemBgSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITabCtrlAttr::INT_animatesteps,m_nAnimateSteps,   DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUITabCtrlAttr::INT_firstitemoffset, m_nFirstItemOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITabCtrlAttr::INT_itemspace, m_nItemSpace,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUITabCtrlAttr::INT_cursel, OnCurSel)
			DM_SIZE_ATTRIBUTE(DMAttr::DUITabCtrlAttr::SIZE_itemsize,m_ItemSize, DM_ECODE_NOXMLLOADREFRESH)
			DM_ENUM_BEGIN(DMAttr::DUITabCtrlAttr::OPTION_tabalign, int, DM_ECODE_NOXMLRELAYOUT)
				DM_ENUM_VALUE(L"top", AlignTop)
				DM_ENUM_VALUE(L"left", AlignLeft)
				DM_ENUM_VALUE(L"right", AlignRight)
				DM_ENUM_VALUE(L"bottom", AlignBottom)
			DM_ENUM_END(m_nTabAlign)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnCurSel(LPCWSTR pszValue, bool bLoadXml);

	public:
		CArray<DUITabPage*>						m_PageArray;			 ///< tab标签页面链表 
		DUITabAnimate*							m_pTabAnimate;
		int										m_nAnimateSteps;		 ///< 整个动画图以多少次渐渐显示出来
		DMSmartPtrT<IDMSkin>					m_pMainBgSkin;			 ///< 整个item所在区的背景,不包括page区
		DMSmartPtrT<IDMSkin>					m_pItemBgSkin;			 ///< 在DUITabPage绘完后再叠一次,比如tab的hover-sel变化全是相同的,减少截图量
		int										m_nHoverItem;		     ///< 当前hover的item 
		int										m_nCurSelItem;		     ///< 当前选中的item
		int										m_nItemSpace;		     ///< Item页面间距  
		int										m_nFirstItemOffset;		 ///< 第一个Item的相对于边框的偏移 
		int										m_nTabAlign;			 ///< 排列方式 
		CSize									m_ItemSize;				 ///< 一个主标签的大小，默认为-1，-1，其中-1表示此方向使用图片状态大小
		CPoint									m_TextPt;				 ///< 相对于主标签区域的左上角偏移，默认为-1，-1，其中-1表示此方向居中 
	};

}//namespace DM