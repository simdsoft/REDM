//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIListCtrlEx.h 
// File Des: DUIListCtrlEx��չListBoxEx��֧�ֶ���������
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-13	1.0			
//-------------------------------------------------------
#pragma once
#include "Widgets/DUIScrollBase.h"
#include "Widgets/DUIItemPanel.h"
#include "Widgets/DUIHeaderCtrl.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIListCtrlEx"/>��xml���Զ���
	/// </summary>
	class DUIListCtrlExAttr: public DUIScrollBaseAttr
	{
	public:
		static const char* INT_headerheight;                                        ///< �б�ͷ�ĸ߶�,ʾ��:headerheight="20"
		static const char* INT_itemheight;                                          ///< ���Ĭ�ϸ߶�,��ֵ��֧�ֶ�̬�仯,ʾ��:itemheight="20"
		static const char* INT_cursel;                                              ///< ��ǰѡ����,ʾ��:cursel="1"
		static const char* COLOR_clritembg;                                         ///< item��ı���ɫ,ʾ��:clritembg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemhoverbg;									 ///< item���ͣ������ɫ,ʾ��:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemselbg;                                      ///< item���ѡ�б���ɫ,ʾ��:clritemselbg="pbgra(ff,ff,ff,ff)"
		static const char* bool_bmultisel;                                          ///< �Ƿ�֧�ֶ�ѡ,Ĭ�ϲ�֧��,ʾ��:bmultisel="1"

		static const char* NODE_item;                                               ///< listctrl��xml������
		static const char* ITEM_height;                                             ///< ��ĸ߶�(��ѡ), ʾ��:height="20"
		static const char* ITEM_data;                                               ///< �������,ʾ��:data="10"
	};
	DMAttrValueInit(DUIListCtrlExAttr,INT_headerheight)DMAttrValueInit(DUIListCtrlExAttr,INT_itemheight)
	DMAttrValueInit(DUIListCtrlExAttr,INT_cursel)DMAttrValueInit(DUIListCtrlExAttr,COLOR_clritembg)DMAttrValueInit(DUIListCtrlExAttr,COLOR_clritemhoverbg)
	DMAttrValueInit(DUIListCtrlExAttr,COLOR_clritemselbg)DMAttrValueInit(DUIListCtrlExAttr,bool_bmultisel)
	DMAttrValueInit(DUIListCtrlExAttr,NODE_item)DMAttrValueInit(DUIListCtrlExAttr,ITEM_height)DMAttrValueInit(DUIListCtrlExAttr,ITEM_data)
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
		DMSmartPtrT<DUIItemPanel>       pPanel;     ///< ���
		int								nHeight;	///< �߶�
		LPARAM							lParam;     ///< data
	}LCITEMEX,*LPLCITEMEX;


	/// <summary>
	///		 DUIListCtrlEx������ʵ��
	/// </summary>
	class DM_EXPORT DUIListCtrlEx:public DUIScrollBase
		,public IDMItemPanelOwner
		,public DMArrayT<LPLCITEMEX>
		,public DMMapT<int,CRect>		///< �����б�
	{
		DMDECLARE_CLASS_NAME(DUIListCtrlEx, DUINAME_ListCtrlEx,DMREG_Window)
	public:
		DUIListCtrlEx();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ� methods
		//---------------------------------------------------
		int InsertItem(int nIndex, DMXmlNode&XmlNode, bool bUpdate = true);
		int GetTopIndex() const;
		int SetTopIndex(int nIndex);
		bool SetCurSel(int nIndex);
		bool SetCurHover(int nIndex);
		int GetCurSel() const;
		int GetColumnCount();
		int GetItemCount();
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
		DMCode OnHeaderSizeChanged(DMEventArgs *pEvt);
		DMCode OnHeaderSwap(DMEventArgs *pEvt);
		DMCode OnScrollEvent(DMEventArgs *pEvt);

		//---------------------------------------------------
		// Function Des: DV methods
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
		virtual DMCode DV_OnSetCursor(const CPoint &pt);
		virtual	void UpdateScrollBar();
		virtual int GetTotalHeight();
		virtual int GetTotalWidth();
		virtual CRect GetListRect();															    ///< �����б������ڵ�����(������head)
		virtual CRect GetItemRect(int iItem);														///< ����ĳ������(��rcClient��ͬһ����ϵ��
		virtual int HitTest(CPoint &pt);															///< �Զ��޸�pt��λ��Ϊ��Ե�ǰ���ƫ����
		int HitTotalY(int iTotalY);
		virtual void UpdateScrollRange();	
		virtual void UpdateVisibleMap();
		virtual void PreArrayObjRemove(const LPLCITEMEX &obj);

	public:// ����
		//---------------------------------------------------
		// Function Des: ���� methods
		//---------------------------------------------------
		void UpdateItemPanelId(int iFirst=0, int iLast = -1);										///< ��������
		void RelayVisibleItemsWidth(int iNewWid);													///< ���¿ɼ���������Ŀ��
		void ReLayoutVisibleItems();																///< ���¿ɼ���������
		void RelayoutItem(int iIndex,CRect rcItem);													///< ���µ���Ĳ���
		void ModifyPanelBgClr(DUIItemPanel* pPanel);												///< �ı�Panel�ı���ɫ
		void ModifySelItems(int nOldSel,int nNewSel);

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
			DM_INT_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::INT_itemheight, m_iDefItemHei,		  DM_ECODE_OK)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::COLOR_clritembg,m_crItemBg[0],	  DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::COLOR_clritemhoverbg,m_crItemBg[1],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::COLOR_clritemselbg,m_crItemBg[2],  DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::bool_bmultisel,m_bMultiSel,         DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::INT_headerheight,OnAttributeHeaderHei)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIListCtrlExAttr::INT_cursel,OnAttributeCurSel)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeHeaderHei(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeCurSel(LPCSTR lpszValue, bool bLoadXml);

	public:	
		int									   m_iSelItem;						 ///< ��ǰѡ����,��ѡΪ���ѡ�е�����.
		int									   m_iHoverItem;					 ///< ��ǰͣ����
		int									   m_iHeaderHei;					 ///< �б�ͷ�߶�
		int									   m_iDefItemHei;					 ///< ���Ĭ�ϸ߶�
		DMColor								   m_crItemBg[3];					 ///< ��������ɫ
		DMSmartPtrT<DUIItemPanel>			   m_pCapturePanel;					 ///< ��ǰ������setcapture�����
		DMSmartPtrT<DUIHeaderCtrl>             m_pHeaderCtrl;                     
		bool								   m_bHotTrack;  
		bool                                   m_bMultiSel; 
		bool                                   m_bMouseDown;
		bool                                   m_bStartSel;
		bool                                   m_bSelFrameUp;
		CPoint								   m_ptStart;
		DUIWindow*                             m_pWndRectangle;
		CRect                                  m_rcDragFrame;
	};
}//namespace DM