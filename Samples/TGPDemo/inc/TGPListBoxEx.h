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
	///		<see cref="DM::TGPListBoxEx"/>��xml���Զ���
	/// </summary>
	class TGPListBoxExAttr:public DUIScrollBaseAttr
	{
	public:
		static const char* INT_itemheight;                                           ///< ���Ĭ�ϸ߶�,ʾ��:itemheight="10"
		static const char* INT_cursel;                                               ///< ��ǰѡ��,ʾ��:cursel="1"
		static const char* COLOR_clritembg;                                          ///< item��ı���ɫ,ʾ��:clritembg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemselbg;                                       ///< item���ѡ�б���ɫ,ʾ��:clritemselbg="pbgra(ff,ff,ff,ff)"
		static const char* bool_bnodrawbg;                                           ///< ΪQQdemo���ã���ץȡ��������Ȼ����͸��

		static const char* NODE_item;                                                ///< ���� 
		static const char* ITEM_height;                                              ///< ��ĸ߶�(��ѡ),ʾ��:height="10"
		static const char* ITEM_data;                                                ///< �������,ʾ��:data="10"
		static const char* bool_bswapline;											  ///< ��������,�����϶�������,ʾ��:bswapline="1"
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
		DMSmartPtrT<DUIItemPanel>       pPanel;     ///< ���
		int								nHeight;	///< �߶�
		DMXmlDocument					XmlDoc;   ///< ����XML
		LPARAM							lParam;     ///< data
	}TGPLBITEMEX,*LPTGPLBITEMEX;
  
	/// <summary>
	///		�ر��¼�
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
	///		 TGPListBoxEx������ʵ��
	/// </summary>
	class TGPListBoxEx:public DUIScrollBase
		,public IDMItemPanelOwner
		,public DMArrayT<LPTGPLBITEMEX>
	{
		DMDECLARE_CLASS_NAME(TGPListBoxEx, "TGPListBoxEx",DMREG_Window)
	public:
		TGPListBoxEx();
		~TGPListBoxEx();  

	public:
		//---------------------------------------------------
		// Function Des: �ӿ�
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

	public:// ���ƣ�������
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
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void OnDestroy();																	///< �����ڴ˴��ͷ�panel����Ȼpanel�޷��ͷţ���
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
		// Function Des: IDMItemPanelOwnerʵ��
		//---------------------------------------------------   
		DUIWindow* GetOwnerWindow();   
		DMCode OnSetCapture(DUIItemPanel* pPanel);
		DMCode OnReleaseCapture(DUIItemPanel* pPanel);
		DMCode OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect);							///< ��ʹ��*lpRect��ֵ��

	public:
		//---------------------------------------------------
		// Function Des: �����غ���
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
		DMCode DV_OnSetCursor(const CPoint &pt);
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTALLKEYS;};
		void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);							  ///<  ���¹��λ��

	public:
		void PreArrayObjRemove(const LPTGPLBITEMEX &obj);

	public:// ����
		void UpdateItemPanelId(int iFirst=0, int iLast = -1);							  ///< ��������
		void UpdateAllPanelLayout();                                                      ///< ����Panel����
		void ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr);                         ///< �ı�Panel�ı���ɫ
		int HitTest(CPoint &pt);														  ///< �Զ��޸�pt��λ��Ϊ��Ե�ǰ���ƫ����
		void UpdateScrollRangeSize();

		void CreateDragCanvas(UINT iItem);												  ///< ��������
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
		int									   m_iSelItem;						 ///< ��ǰѡ����   
		int									   m_iHoverItem;					 ///< ��ǰͣ����
		int									   m_iDefItemHei;					 ///< ���Ĭ�ϸ߶�
		DMColor								   m_crItemBg; 
		DMColor							       m_crItemSelBg; 
		DMSmartPtrT<DUIItemPanel>			   m_pCapturePanel;					 ///< ��ǰ������setcapture�����,�����ֻ����갴�µ�˲�����ã��������
		bool                                   m_bNoDrawBg;     

		// �϶���
		bool                                   m_bSwapLine;	
		bool								   m_bDragging;						///< �����϶���־
		DMSmartPtrT<IDMCanvas>				   m_pDragCanvas;					///< ��ʾ�϶����ڵ���ʱ����
		DWORD								   m_dwHitTest;						///< �϶�λ��������
		DWORD								   m_dwDragTo;						///< �Ϸ�Ŀ��������    
		CPoint								   m_ptClick;						///< ��ǰ�������
	};
}//namespace DM

