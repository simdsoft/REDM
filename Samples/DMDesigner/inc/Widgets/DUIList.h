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
	///		<see cref="DM::DUIList"/>��xml���Զ���
	/// </summary>
	class DUIListAttr:public DUIScrollBaseAttr
	{
	public:
		static const char* INT_itemheight;                                           ///< ���Ĭ�ϸ߶�,ʾ��:itemheight="10"
		static const char* INT_cursel;                                               ///< ��ǰѡ��,ʾ��:cursel="1"
		static const char* SKIN_expandarrskin;                                       ///< ������չ��С����,ʾ��:expandarrskin="skin"
	
		static const char* NODE_item;                                                ///< ���� 
		static const char* ITEM_height;                                              ///< ��ĸ߶�(��ѡ),ʾ��:height="10"
		static const char* ITEM_bexpand;                                             ///< ���Ƿ��ʼչ��,ʾ��:bexpand="1"
		static const char* ITEM_expandheight;                                        ///< ���չ���߶�,ʾ��:expandheight="100"
		static const char* ITEM_data;                                                ///< �������,ʾ��:data="10"
		static const char* bool_bswapline;											  ///< ��������,�����϶�������,ʾ��:bswapline="1"
		
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
		DMSmartPtrT<DUIItemPanel>       pPanel;			///< ���
		int								nHeight;		///< �߶�
		int                             nExpandHeight;  ///< ���չ���߶�
		bool                            bExpand;        ///< �Ƿ�չ��
		DMXmlDocument					XmlDoc;			///< ����XML
		LPARAM							lParam;			///< data
	}DSLBITEMEX,*LPDSLBITEMEX;

	/// <summary>
	///		 DUIList������ʵ��
	/// </summary> 
	class DUIList:public DUIScrollBase
		,public IDMItemPanelOwner
		,public DMArrayT<LPDSLBITEMEX>
	{
		DMDECLARE_CLASS_NAME(DUIList, "DUIList",DMREG_Window)
	public: 
		DUIList();  
		~DUIList();    
 
	public:              
		//---------------------------------------------------
		// Function Des: �ӿ�
		//---------------------------------------------------
		int InsertItem(int nIndex, DMXmlNode&XmlNode);
		CRect GetItemRect(int iItem);
		bool SetCurSel(int nIndex);
		int GetCurSel() const;
		void DeleteItem(int iItem);
		void DeleteAllItems(bool bUpdate = true);
		void EnsureVisible(int nIndex); 
		bool ExpandItem(int nIndex,bool bExpand = true);

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
		DMCode DV_OnSetCursor(const CPoint &pt);
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTALLKEYS;};
		void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);							  ///<  ���¹��λ��

	public:
		void PreArrayObjRemove(const LPDSLBITEMEX &obj);

	public:// ����
		void UpdateItemPanelId(int iFirst=0, int iLast = -1);							  ///< ��������
		void UpdateAllPanelLayout();                                                      ///< ����Panel����
		void NotifySelChange(int nOldSel,int nNewSel);
		void ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr);                         ///< �ı�Panel�ı���ɫ
		int HitTest(CPoint &pt,bool* pbInExpand=NULL);										  ///< �Զ��޸�pt��λ��Ϊ��Ե�ǰ���ƫ����
		void UpdateScrollRangeSize();

		void CreateDragCanvas(UINT iItem);												  ///< ��������
		void DrawDraggingState(DWORD dwDragTo);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIListAttr::INT_itemheight, m_iDefItemHei, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIListAttr::bool_bswapline,m_bSwapLine, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIListAttr::SKIN_expandarrskin,m_pExpandArrSkin,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()

	public:
		int									   m_iSelItem;						 ///< ��ǰѡ����   
		int									   m_iHoverItem;					 ///< ��ǰͣ����
		int									   m_iDefItemHei;					 ///< ���Ĭ�ϸ߶�
		DMSmartPtrT<DUIItemPanel>			   m_pCapturePanel;					 ///< ��ǰ������setcapture�����,�����ֻ����갴�µ�˲�����ã�������� 
		IDMSkinPtr                             m_pExpandArrSkin;

		// �϶���
		bool                                   m_bSwapLine;	
		bool								   m_bDragging;						///< �����϶���־
		DMSmartPtrT<IDMCanvas>				   m_pDragCanvas;					///< ��ʾ�϶����ڵ���ʱ����
		DWORD								   m_dwHitTest;						///< �϶�λ��������
		DWORD								   m_dwDragTo;						///< �Ϸ�Ŀ��������    
		CPoint								   m_ptClick;						///< ��ǰ�������
	};
}//namespace DM

