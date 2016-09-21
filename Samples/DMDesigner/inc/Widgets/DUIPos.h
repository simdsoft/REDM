// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIPos.h
// File mark:   
// File summary:模似pos的锚点布局坐标系
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-29
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		下拉list,主要用于更新tooltips
	/// </summary>
	class PosList : public DUIListBox
	{
		DMDECLARE_CLASS_NAME(PosList,L"poslist",DMREG_Window);
	public:
		CRect GetItemRect(int iItem);
		virtual DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);	
	};

	/// <summary>
	///		edit,加入滚动变化效果
	/// </summary>
	class PosEdit : public DUIEdit
	{
		DMDECLARE_CLASS_NAME(PosEdit,L"posedit",DMREG_Window);
	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_CHAR(OnChar)
			MESSAGE_HANDLER_EX(WM_IME_COMPOSITION, OnImeChar)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
		DM_END_MSG_MAP()
	public:
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		LRESULT OnImeChar(UINT uMsg, WPARAM wParam, LPARAM lParam);
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	}; 

	/// <summary>
	///		模拟POS_ITEM 
	/// </summary>   
	class PosItem : public DUIWindow
	{
		DMDECLARE_CLASS_NAME(PosItem,L"positem",DMREG_Window);
	public:
		PosItem();

		DMCode InitPosItem(POS_ITEM* pItem,Layout* pLayout);
		DMCode UnInitPosItem();
		
		//
		DMCode OnSelChange(DMEventArgs *pEvt);
		DMCode OnEditChange(DMEventArgs *pEvt);

		DMCode OnItemChange(POS_ITEM* pItem = NULL);
	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_SIZE(OnSize)
		DM_END_MSG_MAP()
	public:
		void OnSize(UINT nType, CSize size);
	public: 
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);

	public: 
		POS_ITEM*                        m_pItem;
		DMSmartPtrT<Layout>				 m_pLayout;///< 指向要编辑的layout

		PosEdit*			             m_pEdit;
		DUIComboBox*                     m_pCbx;
	};

	/// <summary>   
	///		模拟锚点pos
	/// </summary>
	class DUIPos: public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIPos,L"duipos",DMREG_Window);
	public:
		DMCode InitLayout(IDMLayout* pLayout);
		DMCode UnInitLayout();
			
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_SIZE(OnSize)
		DM_END_MSG_MAP()

	public:
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnSize(UINT nType, CSize size);

		void InitItem();

	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);

	public:
		DMSmartPtrT<PosItem>                    m_pItem[4];
		DMSmartPtrT<Layout>						m_pLayout;///< 指向要编辑的layout
	};
}