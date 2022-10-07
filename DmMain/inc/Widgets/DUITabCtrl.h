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
#include "Widgets/DMWidgetDef.h"
#include "Widgets/DUIAnimate.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUITabPage"/>��xml���Զ���
	/// </summary>
	class DUITabPageAttr:public DUIWindowAttr
	{
	public:
		static const char* OPTION_animatetype;										    ///< ��������,ʾ��:animatetype="1",1:��չ��,2:��������,3:������С
		static const char* STRING_title;											    ///< tabҳ�ı���,ʾ����title="��ǩ1"
		static const char* SKIN_itemskin;                                              ///< ��ǩ���Ƥ����ʾ��: itemskin="skin"
	};
	DMAttrValueInit(DUITabPageAttr,OPTION_animatetype)DMAttrValueInit(DUITabPageAttr,STRING_title)DMAttrValueInit(DUITabPageAttr,SKIN_itemskin)

	class DUITabCtrlAttr:public DUIWindowAttr
	{
	public:
		static const char* POINT_textpoint;                                           ///< ���������ǩ��������Ͻ�ƫ�ƣ�Ĭ��Ϊ-1��-1������-1��ʾ�˷�����У�ʾ��textpoint="-1,-1"
		static const char* OPTION_tabalign;										   ///< tabҳ�����з�ʽ,��Ϊtop��left��right��buttom,ʾ��:tabalign="top"
		static const char* SKIN_mainbgskin;							               ///< ����item(��ǩ�б�)�������ı���,������page��,ʾ��:mainbgskin="TabCtrl_Item"
		static const char* SKIN_itembgskin;										   ///< ÿ��item(��ǩ)��skin������DUITabPageAttr::SKIN_itemskin֮���Ȼ�һ��,��������item��hover-selΪͬһ״̬ʱ�򻯽�ͼ,ʾ��:itembgskin="skin"
		static const char* INT_firstitemoffset;                                       ///< ��һ��Item(��ǩ)������ڱ߿��ƫ�ƣ�ʾ��:firstitemoffset="10"
		static const char* INT_itemspace;                                             ///< Item֮��ļ�࣬ʾ��:itemspace="10"
		static const char* INT_cursel;                                                ///< ��ǰѡ����, ʾ��:cursel="1"
		static const char* INT_animatesteps;                                          ///< ��������ͼ�Զ��ٴν�����ʾ����, ���������˶�������ʱ��Ч��ʾ��:animatesteps="20"
		static const char* SIZE_itemsize;                                             ///< Item(��ǩ)��Ĵ�С��ʾ��:itemsize="78,30"
	};
	DMAttrValueInit(DUITabCtrlAttr,POINT_textpoint)DMAttrValueInit(DUITabCtrlAttr,OPTION_tabalign)DMAttrValueInit(DUITabCtrlAttr,SKIN_mainbgskin)DMAttrValueInit(DUITabCtrlAttr,SKIN_itembgskin)
	DMAttrValueInit(DUITabCtrlAttr,INT_firstitemoffset)DMAttrValueInit(DUITabCtrlAttr,INT_itemspace)DMAttrValueInit(DUITabCtrlAttr,INT_cursel)
	DMAttrValueInit(DUITabCtrlAttr,INT_animatesteps)DMAttrValueInit(DUITabCtrlAttr,SIZE_itemsize)
}

namespace DM
{
	enum{TAB_NULL=0,TAB_SLIDER,TAB_CINFLATE,TAB_CDEFLATE};
	/// <summary>
	///		 DUITabPage������ʵ�֣����ԣ�<see cref="DMAttr::DUITabPageAttr"/>
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
			DM_WSTRING_ATTRIBUTE(DMAttr::DUITabPageAttr::STRING_title, m_strTitle, DM_ECODE_NOXMLLOADPARENTREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITabPageAttr::OPTION_animatetype, m_TabAniType, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITabPageAttr::SKIN_itemskin, m_pItemSkin, DM_ECODE_NOXMLLOADPARENTREFRESH)
		DM_END_ATTRIBUTES()
	public:
		CStringW											m_strTitle;           ///< ITEM����
		int                                                 m_TabAniType;         ///< ��������
		DMSmartPtrT<IDMSkin>                                m_pItemSkin;          ///< ��ǩƤ��
	};

	/// <summary>
	///		 DUITabCtrl������ʵ�֣����ԣ�<see cref="DMAttr::DUITabCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUITabCtrl:public DUIWindow,public IDMTabAnimateOwner
	{
		DMDECLARE_CLASS_NAME(DUIWindow,DUINAME_TabCtrl,DMREG_Window);
	public:
		DUITabCtrl();

		//---------------------------------------------------
		// Function Des: ����ӿ�
		//---------------------------------------------------
		int InsertItem(DMXmlNode &XmlNode, int iInsert=-1, bool bLoading=false);
		int GetCurSel();
		bool SetCurSel(int nIndex);
		bool SetCurSel(LPCWSTR lpszTitle);
		bool SetItemTitle(int nIndex, LPCWSTR lpszTitle);
		bool GetItemRect(int nIndex, CRect &rcItem);	
		CRect GetTitleRect();													///< ���б�ǩ���ڵĴ���Σ���һ���־�������page���ڵĴ����					
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
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
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
		// Function Des: ����
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTARROWS;};	
		DMCode DV_GetChildMeasureLayout(LPRECT lpRect);	
		DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);

		//---------------------------------------------------
		// Function Des: ��������
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
				DM_ENUM_VALUE("top", AlignTop)
				DM_ENUM_VALUE("left", AlignLeft)
				DM_ENUM_VALUE("right", AlignRight)
				DM_ENUM_VALUE("bottom", AlignBottom)
			DM_ENUM_END(m_nTabAlign)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnCurSel(LPCSTR pszValue, bool bLoadXml);

	public:
		CArray<DUITabPage*>						m_PageArray;			 ///< tab��ǩҳ������ 
		DUITabAnimate*							m_pTabAnimate;
		int										m_nAnimateSteps;		 ///< ��������ͼ�Զ��ٴν�����ʾ����
		DMSmartPtrT<IDMSkin>					m_pMainBgSkin;			 ///< ����item�������ı���,������page��
		DMSmartPtrT<IDMSkin>					m_pItemBgSkin;			 ///< ��DUITabPage������ٵ�һ��,����tab��hover-sel�仯ȫ����ͬ��,���ٽ�ͼ��
		int										m_nHoverItem;		     ///< ��ǰhover��item 
		int										m_nCurSelItem;		     ///< ��ǰѡ�е�item
		int										m_nItemSpace;		     ///< Itemҳ����  
		int										m_nFirstItemOffset;		 ///< ��һ��Item������ڱ߿��ƫ�� 
		int										m_nTabAlign;			 ///< ���з�ʽ 
		CSize									m_ItemSize;				 ///< һ������ǩ�Ĵ�С��Ĭ��Ϊ-1��-1������-1��ʾ�˷���ʹ��ͼƬ״̬��С
		CPoint									m_TextPt;				 ///< ���������ǩ��������Ͻ�ƫ�ƣ�Ĭ��Ϊ-1��-1������-1��ʾ�˷������ 
	};

}//namespace DM