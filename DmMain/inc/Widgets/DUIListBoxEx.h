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
#include "Widgets/DUIScrollBase.h"
#include "Widgets/DUIItemPanel.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIListBoxEx"/>��xml���Զ���
	/// </summary>
	class DUIListBoxExAttr:public DUIScrollBaseAttr
	{
	public:
		static const char* INT_itemheight;                                          ///< ���Ĭ�ϸ߶�,��ֵ��֧�ֶ�̬�仯,ʾ��:itemheight="20"
		static const char* INT_cursel;                                              ///< ��ǰѡ����,ʾ��:cursel="1"
		static const char* COLOR_clritembg;                                         ///< item��ı���ɫ,ʾ��:clritembg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemhoverbg;									 ///< item���ͣ������ɫ,ʾ��:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemselbg;                                      ///< item���ѡ�б���ɫ,ʾ��:clritemselbg="pbgra(ff,ff,ff,ff)"

		static const char* NODE_item;                                                ///< ���� 
		static const char* ITEM_height;                                              ///< ��ĸ߶�(��ѡ),ʾ��:height="10"
		static const char* ITEM_data;                                                ///< �������,ʾ��:data="10"
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
		DMSmartPtrT<DUIItemPanel>       pPanel;     ///< ���
		int								nHeight;	///< �߶�
		LPARAM							lParam;     ///< data
	}LBITEMEX,*LPLBITEMEX;


	/// <summary>
	///		 DUIListBoxEx������ʵ��
	/// </summary>
	class DM_EXPORT DUIListBoxEx:public DUIScrollBase
		,public IDMItemPanelOwner
		,public DMArrayT<LPLBITEMEX>
		,public DMMapT<int,CRect>		///< �����б�
	{
		DMDECLARE_CLASS_NAME(DUIListBoxEx, DUINAME_ListBoxEx,DMREG_Window)
	public:
		DUIListBoxEx();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ� methods
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
		virtual CRect GetItemRect(int iItem);														///< ����ĳ������(��rcClient��ͬһ����ϵ��
		virtual int HitTest(CPoint &pt);															///< �Զ��޸�pt��λ��Ϊ��Ե�ǰ���ƫ����
		virtual void UpdateScrollRange();	
		virtual void UpdateVisibleMap();
		virtual void PreArrayObjRemove(const LPLBITEMEX &obj);

	public:// ����
		//---------------------------------------------------
		// Function Des: ���� methods
		//---------------------------------------------------
		void UpdateItemPanelId(int iFirst=0, int iLast = -1);										///< ��������
		void ReLayoutVisibleItems();																///< ���¿ɼ���������
		void RelayoutItem(int iItem,CRect rcItem);													///< ���µ���Ĳ���
		void ModifyPanelBgClr(DUIItemPanel* pPanel);												///< �ı�Panel�ı���ɫ
		void ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr);									///< �ɰ�ӿڣ�����

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
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
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
		DMCode OnAttributeCurSel(LPCSTR lpszValue, bool bLoadXml);

	public:
		int									   m_iSelItem;						 ///< ��ǰѡ����
		int									   m_iHoverItem;					 ///< ��ǰͣ����
		int									   m_iDefItemHei;					 ///< ���Ĭ�ϸ߶�
		DMColor								   m_crItemBg[3];					 ///< ��������ɫ
		DMSmartPtrT<DUIItemPanel>			   m_pCapturePanel;					 ///< ��ǰ������setcapture�����,�����ֻ����갴�µ�˲�����ã��������
	};

}//namespace DM

