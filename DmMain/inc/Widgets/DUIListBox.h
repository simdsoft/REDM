//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIListBox.h 
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

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIListBox"/>��xml���Զ���
	/// </summary>
	class DUIListBoxAttr:public DUIScrollBaseAttr
	{
	public:
		static const char* INT_itemheight;                                       ///< ���Ĭ�ϸ߶�,Ĭ��Ϊ20,ʾ��:itemheight="20"
		static const char* POINT_iconpoint;                                      ///< �����Item�����Ͻ�ƫ��,Ĭ��Ϊ-1��-1������-1��ʾʹ��Ĭ�ϸ߶ȵ�1/6,ʾ��:iconpoint="-1,-1"
		static const char* POINT_textpoint;                                      ///< �����Item�����Ͻ�ƫ�ƣ�Ĭ��Ϊ-1��-1������-1��ʾ�˷������,ʾ��:textpoint="-1,-1"
		static const char* SKIN_iconskin;                                        ///< ÿ���Դ���ICONͼ��,һ��Ϊ״̬��ͼ,ʾ��:iconskin="skin"
		static const char* SKIN_itembgskin;                                      ///< �����ڵı���ͼ��,ʾ����itembgskin="skin"
		static const char* COLOR_clritembg;                                      ///< ������item�ı���ɫ,ʾ��:clritembg="pbgra(FF,FF,FF,FF)"
		static const char* COLOR_clritembg2;                                     ///< ˫����item�ı���ɫ,�粻����,��Ĭ��ʹ�õ�����ı���ɫ,ʾ��:clritembg2="pbgra(FF,FF,cc,FF)"
		static const char* COLOR_clritemhoverbg;                                 ///< item���hover����ɫ,ʾ��:clritemhoverbg="pbgra(ff,00,00,ff)"
		static const char* COLOR_clritemselbg;                                   ///< item���ѡ�б���ɫ,ʾ��:clritemselbg="pbgra(ff,00,00,ff)"
		static const char* COLOR_clritemtext;                                    ///< item����ı�ɫ,ʾ��:clritemtext="pbgra(ff,00,00,ff)"
		static const char* COLOR_clritemhovertext;                                ///< item���hover�ı�ɫ,ʾ��:clritemhoverext="pbgra(ff,00,00,ff)"
		static const char* COLOR_clritemseltext;                                 ///< item���ѡ���ı�ɫ,ʾ��:clritemseltext="pbgra(ff,00,00,ff)"
		static const char* bool_bhottrack;                                       ///< ��ָ���µ����Զ�׷��ѡ����ʾ,ʾ��:bhottrack="1"
		static const char* INT_cursel;                                           ///< ����Ϊѡ���ʾ��:cursel="1"
		//
		static const char* NODE_item;                                            ///< ListBox��xml������
		static const char* ITEM_height;                                          ///< ��ĸ߶�(��ѡ),��ָ����ʹ��Ĭ����,height="30"
		static const char* ITEM_text;                                            ///< ����ı�,ʾ��:text="item0"
		static const char* ITEM_icon;                                            ///< ���icon����,-1��ʾ��ʹ��iconͼ,ʾ��:icon="1"
		static const char* ITEM_data;                                            ///< ��ĸ�������,ʾ��:data="100"
	};
	DMAttrValueInit(DUIListBoxAttr,INT_itemheight)DMAttrValueInit(DUIListBoxAttr,POINT_iconpoint)DMAttrValueInit(DUIListBoxAttr,POINT_textpoint)
	DMAttrValueInit(DUIListBoxAttr,SKIN_iconskin)DMAttrValueInit(DUIListBoxAttr,SKIN_itembgskin)DMAttrValueInit(DUIListBoxAttr,COLOR_clritembg)
	DMAttrValueInit(DUIListBoxAttr,COLOR_clritemhoverbg)DMAttrValueInit(DUIListBoxAttr,COLOR_clritembg2)DMAttrValueInit(DUIListBoxAttr,COLOR_clritemselbg)
	DMAttrValueInit(DUIListBoxAttr,COLOR_clritemtext)DMAttrValueInit(DUIListBoxAttr,COLOR_clritemhovertext)DMAttrValueInit(DUIListBoxAttr,COLOR_clritemseltext)
	DMAttrValueInit(DUIListBoxAttr,bool_bhottrack)DMAttrValueInit(DUIListBoxAttr,NODE_item)DMAttrValueInit(DUIListBoxAttr,ITEM_height)
	DMAttrValueInit(DUIListBoxAttr,ITEM_text)DMAttrValueInit(DUIListBoxAttr,ITEM_icon)DMAttrValueInit(DUIListBoxAttr,ITEM_data)DMAttrValueInit(DUIListBoxAttr,INT_cursel)
}

namespace DM
{
	typedef struct stLBITEM
	{
		stLBITEM()
		{
			nImage = -1;
			lParam = NULL;
		}
		CStringW    strText;    ///< �ı�
		int			nHeight;	///< �߶�
		int         nImage;		///< icon��־
		LPARAM      lParam;     ///< data
		void assign(stLBITEM* pItem)
		{
			strText = (LPCWSTR)pItem->strText;
			nHeight = pItem->nHeight;
			nImage = pItem->nImage;
			lParam = pItem->lParam;
		}
	}LBITEM, *LPLBITEM;

	/// <summary>
	///		 DUIListBox������ʵ��
	/// </summary>
	class DM_EXPORT DUIListBox: public DUIScrollBase,public DMArrayT<LPLBITEM>
	{
		DMDECLARE_CLASS_NAME(DUIListBox, DUINAME_ListBox,DMREG_Window)
	public:
		DUIListBox();
		~DUIListBox();
		void Init();
		void UnInit();
	public:
		//---------------------------------------------------
		// Function Des: �ӿ�
		//---------------------------------------------------
		int InsertItem(int nIndex, LPLBITEM pItem);
		int GetTopIndex() const;
		int SetTopIndex(int nIndex);
		int GetItemHeight() const;
		int GetItemHeight(int nIndex) const;
		int SetItemHeight(int nIndex, int cyItemHeight);
		int GetText(int nIndex, CStringW& strText);
		int GetTextLen(int nIndex) ;
		int DeleteString(int nIndex);
		int AddString(LPCWSTR lpszItem, int nHei=-1, int nImage = -1, LPARAM lParam = 0);
		int InsertString(int nIndex, LPCTSTR lpszItem, int nHei=-1, int nImage = -1, LPARAM lParam = 0);
		LPARAM GetItemData(int nIndex);
		bool SetItemData(int nIndex, LPARAM lParam);
		bool SetCurSel(int nIndex);
		int GetCurSel() const;
		void DeleteAll();
		void EnsureVisible(int nIndex);

	public:// ���ƣ�������
		virtual void DrawItem(IDMCanvas* pCanvas, CRect& rc, int iItem);
		void RedrawItem(int iItem);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_SIZE(OnSize)
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_CHAR(OnChar)
			MSG_WM_SHOWWINDOW(OnShowWindow)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void OnDestroy();
		void OnSize(UINT nType,CSize size);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonDbClick(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void OnKeyDown(TCHAR nChar,UINT nRepCnt,UINT nFlags);          // ֧�����¼�ͷ������
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnShowWindow(BOOL bShow, UINT nStatus);

	public:
		//---------------------------------------------------
		// Function Des: �����غ���
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;

	public:// ����
		int HitTest(CPoint &pt);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIListBoxAttr::INT_itemheight, m_nDefItemHei, DM_ECODE_NOXMLLOADREFRESH)

			DM_POINT_ATTRIBUTE(DMAttr::DUIListBoxAttr::POINT_iconpoint,m_IconPt, DM_ECODE_NOXMLLOADREFRESH)
			DM_POINT_ATTRIBUTE(DMAttr::DUIListBoxAttr::POINT_textpoint,m_TextPt, DM_ECODE_NOXMLLOADREFRESH)
			
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIListBoxAttr::SKIN_iconskin,   m_pIconSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIListBoxAttr::SKIN_itembgskin, m_pItemBgSkin,DM_ECODE_NOXMLLOADREFRESH)

			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritembg,m_crItemBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritembg2, m_crItemBg2,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemhoverbg,m_crItemHoverBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemselbg,m_crItemSelBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemtext,m_crItemText,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemhovertext,m_crItemHoverText,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIListBoxAttr::COLOR_clritemseltext,m_crItemSelText,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIListBoxAttr::bool_bhottrack,m_bHotTrack, DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIListBoxAttr::INT_cursel,OnAttributeCurSel)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeCurSel(LPCSTR lpszValue, bool bLoadXml);
	public:
		int									   m_iSelItem;						 ///< ��ǰѡ����   
		int									   m_iHoverItem;					 ///< ��ǰͣ����
		int									   m_nDefItemHei;					 ///< ��ĸ߶�

		IDMSkinPtr							   m_pItemBgSkin;
		IDMSkinPtr							   m_pIconSkin;

		DMColor								   m_crItemBg; 
		DMColor							       m_crItemBg2;
		DMColor                                m_crItemHoverBg;
		DMColor							       m_crItemSelBg; 
		DMColor							       m_crItemText;  
		DMColor                                m_crItemHoverText;
		DMColor					               m_crItemSelText; 

		CPoint								   m_IconPt;						///< �����Item�����Ͻ�ƫ��
		CPoint								   m_TextPt;						///< �����Item�����Ͻ�ƫ�ƣ�Ĭ��Ϊ-1��-1������-1��ʾ�˷������ 

		bool		    					   m_bHotTrack; 
	};

}//namespace DM