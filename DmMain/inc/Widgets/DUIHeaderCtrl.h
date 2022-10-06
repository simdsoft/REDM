//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIHeaderCtrl.h 
// File Des: �����б�ͷʵ��
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-13	1.0			
//--------------------------------------------------------
#pragma once
#include "Core/DMDragWnd.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIHeaderCtrl"/>��xml���Զ���
	/// </summary>
	class DUIHeaderCtrlAttr:public DUIWindowAttr
	{
	public:
		static const char* SKIN_itemskin;                                             ///< item״̬ͼ,ʾ��:itemskin="itemskin"
		static const char* SKIN_sortskin;                                             ///< Ĭ�ϵ�sort״̬ͼ(С����),ʾ��:sortskin="sortskin"
		static const char* COLOR_clritembg;										   ///< item��ı���ɫ,ʾ��:clritembg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemhoverbg;									   ///< item���ͣ������ɫ,ʾ��:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemselbg;                                        ///< item��İ��±���ɫ,ʾ��:clritemselbg="pbgra(ff,ff,ff,ff)"
		static const char* RECT_rcitemmargin;										   ///< item����ı߿�,��˼����С�ı�,�ٻ�skin����ɫ,ʾ��:rcitemmargin="0,0,0,0"
		static const char* INT_itemdelta;                                             ///< ÿ��Item�Լ����ҵĲ�ֵ����,ʾ��:itemdelta="2"
		static const char* bool_bfixwidth;											   ///< �̶���ȣ�Ĭ���ǿɱ��ȵ�,ʾ��:bfixwidth="1"
		static const char* bool_bswap;                                                ///< ��������,�����϶�������,ʾ��:bswap="1"
		static const char* bool_bsort;                                                ///< ��������sort,���list����������Ϣ,ʾ��:bsort="1"

		/// item�����
		static const char* NODE_item;                                                 ///< ��XML���
		static const char* ITEM_width;                                                ///< ��Ŀ��,ʾ��:width="100"
		static const char* ITEM_skin;                                                 ///< ���״̬��ͼ,�粻���ڣ���ʹ��Ĭ�ϵ�item״̬ͼ,ʾ��,skin="skin"
		static const char* ITEM_text;                                                 ///< ����ı�,ʾ��:text="���϶�"
		static const char* ITEM_data;                                                 ///< ��ĸ�������,ʾ��:data="100"
		static const char* ITEM_stflag;                                               ///< �������(���list),��Ϊ����0����С����1,�Ӵ�С2��ʾ��:stflag="0"
	};
	DMAttrValueInit(DUIHeaderCtrlAttr,SKIN_itemskin)DMAttrValueInit(DUIHeaderCtrlAttr,SKIN_sortskin)
	DMAttrValueInit(DUIHeaderCtrlAttr,COLOR_clritembg)DMAttrValueInit(DUIHeaderCtrlAttr,COLOR_clritemhoverbg)DMAttrValueInit(DUIHeaderCtrlAttr,COLOR_clritemselbg)
	DMAttrValueInit(DUIHeaderCtrlAttr,RECT_rcitemmargin)DMAttrValueInit(DUIHeaderCtrlAttr,INT_itemdelta)
	DMAttrValueInit(DUIHeaderCtrlAttr,bool_bfixwidth)DMAttrValueInit(DUIHeaderCtrlAttr,bool_bswap)DMAttrValueInit(DUIHeaderCtrlAttr,bool_bsort)
	DMAttrValueInit(DUIHeaderCtrlAttr,NODE_item)DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_width)DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_skin)
	DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_text)DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_data)DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_stflag)
}

namespace DM
{
#define DMHDI_WIDTH               0x0001
#define DMHDI_TEXT                0x0002
#define DMHDI_SORTFLAG            0x0004
#define DMHDI_LPARAM              0x0008
#define DMHDI_ORDER               0x0010
	/// <summary>
	///		 ��¼��ǰ������״̬
	/// </summary>
	typedef enum _DMHDSORTFLAG
	{
		DMT_NULL = 0,
		DMT_UP,
		DMT_DOWN,
	}DMHDSORTFLAG;
	/// <summary>
	///		 ��HDITEM,����ÿ��itemʹ��һ��״̬��ͼ,ulState��¼��״̬
	/// </summary>
	typedef struct _DMHDITEM
	{
		_DMHDITEM()
		{
			lpszText = NULL;
			pSkin    = NULL;
			ulState  = DUIWNDSTATE_Normal;
		}
		~_DMHDITEM()
		{
			if (lpszText)
			{
				free(lpszText);
			}
		}
		UINT					  mask; 
		int						  cxy; 
		IDMSkinPtr			      pSkin;	
		LPWSTR					  lpszText; 
		int						  cchTextMax; 
		int						  fmt; 
		LPARAM					  lParam; 
		UINT                      ulState;   
	    DMHDSORTFLAG			  stFlag;
		int						  iOrder;
		
	} DMHDITEM, *LPDMHDITEM;

	/// <summary>
	///		 DUIHeaderCtrl������ʵ��
	/// </summary>
	class DM_EXPORT DUIHeaderCtrl: public DUIWindow,public DMArrayT<LPDMHDITEM>
	{
		DMDECLARE_CLASS_NAME(DUIHeaderCtrl, DUINAME_HeaderCtrl,DMREG_Window)
	public:
		DUIHeaderCtrl();
		~DUIHeaderCtrl();
	public:
		//---------------------------------------------------
		// Function Des: �ӿ�
		//---------------------------------------------------
		int InsertItem(int nIndex, DMXmlNode&XmlNode);
		bool GetItem(int iItem,DMHDITEM* pItem);
		size_t GetItemCount() ;
		int GetTotalWidth();
		int GetItemWidth(int iItem);
		int SetItemWidth(int iItem,int iWid,bool bFire=true);
		bool DeleteItem(int iItem);
		void DeleteAllItems();


	public:// ���ƣ�������
		virtual void DrawItem(IDMCanvas* pCanvas, CRect& rc, const LPDMHDITEM pItem);
		void RedrawItem(int iItem);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_ACTIVATEAPP(OnActivateApp)
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void OnActivateApp(BOOL bActive, DWORD dwThreadID);
		void OnDestroy();

	public:
		//---------------------------------------------------
		// Function Des: �����غ���
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
	    DMCode DV_OnSetCursor(const CPoint &pt);             // ���õ�ǰ�Ĺ��,��������item֮����IDC_SIZEWE���


	public:
		void PreArrayObjRemove(const LPDMHDITEM &obj);

	public:// ����
		DWORD HitTest(CPoint pt);													 ///< ���λ������item֮�䣬��λ�͵�λ��ͬ��������ͬ
		bool IsItemHover(DWORD dwHitTest);
		void CreateDragCanvas(UINT iItem);
	    void DrawDraggingState(DWORD dwDragTo);
		CRect GetItemRect(UINT iItem);

	public:  
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::SKIN_itemskin,OnAttributeItemSkin)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::SKIN_sortskin,m_pSortSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::COLOR_clritembg,m_crItemBg[0],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::COLOR_clritemhoverbg,m_crItemBg[1],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::COLOR_clritemselbg,m_crItemBg[2],DM_ECODE_NOXMLLOADREFRESH)
			DM_RECT_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::RECT_rcitemmargin,m_rcItemMargin,DM_ECODE_NOXMLLOADREFRESH);
			DM_INT_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::INT_itemdelta,m_nItemDelta, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::bool_bfixwidth,m_bFixWidth,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::bool_bswap,m_bSwap,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::bool_bsort,m_bSort,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeItemSkin(LPCSTR lpszValue, bool bLoadXml);
	public:
		IDMSkin*											m_pItemSkin;			///< ��ͷ�����Skin
		IDMSkin*											m_pSortSkin;			///< �����־Skin
		DMColor											    m_crItemBg[3];          ///< item��������̬,��itemû��skinʱʹ��
		CRect                                               m_rcItemMargin;			///< item����ı߿�,��˼����С�ı�,�ٻ�skin����ɫ
		bool											    m_bFixWidth;			///< �����ȹ̶�����
		bool												m_bSort;				///< ��ͷ���Ե������
		bool											    m_bSwap;				///< �����϶�����λ�ÿ���

		bool												m_bDragging;			///< �����϶���־
		DMSmartPtrT<IDMCanvas>								m_pDragCanvas;		    ///< ��ʾ�϶����ڵ���ʱ����
		CPoint												m_ptClick;				///< ��ǰ�������
		DWORD												m_dwHitTest;			///< ���λ��
		DWORD												m_dwDragTo;				///< �Ϸ�Ŀ��    
		int													m_nAdjItemOldWidth;		///< ���汻�϶����ԭʼ���
		int													m_nItemDelta;           ///< ÿ��Item�Լ����ҵ�С���²�ֵ,
	};

}//namespace DM