//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIMonthCalCtrl.h
// File Des: ��������ʵ��
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2018-12-4	1.0			
//--------------------------------------------------------
#pragma once
#include "Common/DMDateTime.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIMonthCalCtrl"/>��xml���Զ���
	/// </summary>
	class DUIMonthCalCtrlAttr:public DUIWindowAttr
	{
	public:
		static const char* INT_headerheight;												  ///< ͷ���ĸ߶�,Ĭ��Ϊ24,ʾ��:headerheight="24"
		static const char* INT_btnoffset;													  ///< ͷ������С��������ڱ߿��ƫ�ƣ�Ĭ��Ϊ4,ʾ��:btnoffset="4"
		static const char* COLOR_clrheaderbg;												  ///< ͷ���ı���ɫ,ʾ��:clrheaderbg="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_headerbgskin;                                                ///< ͷ���ı���ͼ,ʾ��:headerbgskin="bgskin"
		static const char* SKIN_leftbtnskin;                                                 ///< ��ߵ�С���ǰ�ťƤ����ʾ��:leftbtnskin="skinbtn"
		static const char* SKIN_rightbtnskin;                                                ///< �ұߵ�С���ǰ�ťƤ����ʾ��:rightbtnskin="skinbtn"
		static const char* SIZE_btnsize;													  ///< С���ǵĿ��,Ϊ-1,-1ʱ��ʾʹ��btnskin��size,��ֱ����,ʾ��:btnsize="-1,-1"
		static const char* COLOR_clrheadertext;                                              ///< ͷ���м���ı���ɫ,ʾ��:clrheadertext="pbgra(ff,ff,ff,ff)"
		static const char* FONT_headerfont;												  ///< ͷ���м���ı�����,ʾ��:headerfont="face:����,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:��weight:���λ������:������1λ,����ո�!�ڲ����˿ո�ȥ������
		static const char* STRING_headertextformat;                                          ///< ͷ���м���ı�format,���и�ʾ�����ֵķ���,ʾ��:headertextformat="%04d��%02d��"				
		//
		static const char* INT_titleheight;												  ///< title�ĸ߶�,Ĭ��Ϊ24,ʾ��:titleheight="24"
		static const char* STRING_weektitle;												  ///< ����ı�ʾ��,�Զ��ŷָ�,ʾ��:weektitle="��,һ, ��, ��, ��, ��, ��"
		static const char* COLOR_clrtitletext;												  ///< title������ɫ,ʾ��:clrtitletext="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtitlebg;												  ///< title�ı���ɫ,ʾ��:clrtitlebg="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_titlebgskin;                                                 ///< title�ı���ͼ,ʾ��:titlebgskin="bgskin"
		static const char* FONT_titlefont;													  ///< title���ı�����,ʾ��:titlefont="face:����,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:��weight:���λ������:������1λ,����ո�!�ڲ����˿ո�ȥ������
		//
		static const char* bool_bhidegraycell;                                               ///< �Ƿ�����gray cell,ʾ��:bhidegraycell="0"
		static const char* COLOR_clrgraycelltext;											  ///< graycell��normarl�ı�ɫ(�����µ�cellΪgreycell),ʾ��:clrgraycelltext="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrgraycelltexthover;									      ///< graycell��hover�ı�ɫ(�����µ�cellΪgreycell),ʾ��:clrgraycelltexthover="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrgraycelltextpush;									      ///< graycell��push�ı�ɫ(�����µ�cellΪgreycell),ʾ��:clrgraycelltextpush="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrcell;													  ///< ����cell�ı���ɫ,ʾ��:clrcell="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrcellhover;												  ///< ����cell��hover����ɫ,ʾ��:clrcellhover="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrcellpush;												  ///< ����cell��push����ɫ,ʾ��:clrcellpush="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_cellskin;                                                    ///< ����cell����̬����ͼ,ʾ��:cellskin="skin"
		static const char* bool_bshowjieri;									    		  ///< �Ƿ���ʾ����,�����ʾ,��cell���ı�ռ�Ϸ���2/3,����ռ�·�1/3,ʾ��:bshowjieri="1"
		static const char* COLOR_clrjieritext;												  ///< ���յ�����ɫ,ʾ��:clrjieritext="pbgra(ff,ff,ff,ff)"
		static const char* FONT_jierifont;													  ///< ���յ��ı�����,ʾ��:jierifont="face:����,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:��weight:���λ������:������1λ,����ո�!�ڲ����˿ո�ȥ������
		//
		static const char* INT_todayheight;												  ///< today�ĸ߶�,Ĭ��Ϊ24,ʾ��:todayheight="24"
		static const char* COLOR_clrtodaytext;												  ///< today��normarl�ı�ɫ,ʾ��:clrtodaytext="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtodaytexthover;									      ///< today��hover�ı�ɫ,ʾ��:clrtodaytexthover="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtodaytextpush;											  ///< today��push�ı�ɫ,ʾ��:clrtodaytextpush="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtoday;													  ///< today�ı���ɫ,ʾ��:clrtoday="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtodayhover;											  ///< today��hover����ɫ,ʾ��:clrtodayhover="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtodaypush;												  ///< today��push����ɫ,ʾ��:clrtodaypush="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_todayskin;                                                   ///< ÿ��today����̬����ͼ,ʾ��:todayskin="skin"
		static const char* FONT_todayfont;													  ///< today���ı�����,ʾ��:todayfont="face:����,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:��weight:���λ������:������1λ,����ո�!�ڲ����˿ո�ȥ������
		static const char* STRING_todaytextformat;                                           ///< today���ı�format,���и�ʾ�����ֵķ���,ʾ��:todaytextformat="����:%04d/%02d/%02d"			

	};
	DMAttrValueInit(DUIMonthCalCtrlAttr,INT_headerheight)DMAttrValueInit(DUIMonthCalCtrlAttr,INT_btnoffset)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrheaderbg)
	DMAttrValueInit(DUIMonthCalCtrlAttr,SKIN_headerbgskin)DMAttrValueInit(DUIMonthCalCtrlAttr,SKIN_leftbtnskin)DMAttrValueInit(DUIMonthCalCtrlAttr,SKIN_rightbtnskin)
	DMAttrValueInit(DUIMonthCalCtrlAttr,SIZE_btnsize)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrheadertext)DMAttrValueInit(DUIMonthCalCtrlAttr,FONT_headerfont)DMAttrValueInit(DUIMonthCalCtrlAttr,STRING_headertextformat)
	//
	DMAttrValueInit(DUIMonthCalCtrlAttr,INT_titleheight)DMAttrValueInit(DUIMonthCalCtrlAttr,STRING_weektitle)
	DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrtitlebg)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrtitletext)DMAttrValueInit(DUIMonthCalCtrlAttr,SKIN_titlebgskin)DMAttrValueInit(DUIMonthCalCtrlAttr,FONT_titlefont)
	//
	DMAttrValueInit(DUIMonthCalCtrlAttr,bool_bhidegraycell)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrgraycelltext)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrgraycelltexthover)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrgraycelltextpush)
	DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrcell)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrcellhover)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrcellpush)DMAttrValueInit(DUIMonthCalCtrlAttr,SKIN_cellskin)
	DMAttrValueInit(DUIMonthCalCtrlAttr,bool_bshowjieri)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrjieritext)DMAttrValueInit(DUIMonthCalCtrlAttr,FONT_jierifont)
	//
	DMAttrValueInit(DUIMonthCalCtrlAttr,INT_todayheight)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrtodaytext)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrtodaytexthover)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrtodaytextpush)
	DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrtoday)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrtodayhover)DMAttrValueInit(DUIMonthCalCtrlAttr,COLOR_clrtodaypush)
	DMAttrValueInit(DUIMonthCalCtrlAttr,SKIN_todayskin)DMAttrValueInit(DUIMonthCalCtrlAttr,FONT_todayfont)DMAttrValueInit(DUIMonthCalCtrlAttr,STRING_todaytextformat)
}

namespace DM
{
	typedef struct stDMCalendarCell
	{
		DMDateTime						date;		///< ����	
		CStringW						title;		///< ����
		CStringW						jieri;      ///< ����                  
		bool							bGray;		///< �Ƿ�Ϊ��
		bool							bMark;		///< �Ƿ񱻱��
		CRect                           rect;       ///< ��ռ������
	}DMCalendarCell,*LPDMCalendarCell;
	#define CALENDAR_ROWS			6				///< ��ʾ�����ж�����
	#define CALENDAR_COLUMNS		7				///< ��ʾ�����ж�����

	/// <summary>
	///		 DUIMonthCalCtrl������ʵ�֣����ԣ�<see cref="DMAttr::DUIMonthCalCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUIMonthCalCtrl:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIMonthCalCtrl,DUINAME_MonthCalCtrl,DMREG_Window);
	public:
		DUIMonthCalCtrl();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ� methods
		//---------------------------------------------------
		DMDateTime GetDisplayDate() const;
		void SetDisplayDate(DMDateTime date, bool bNotify=true);

		void UpdateDisplayCells();																	///< ���µ�ǰ��cells�б�
		void UpdateDisplayCellsRect();																///< ���µ�ǰ��cells�б�����

		//---------------------------------------------------
		// Function Des: Draw methods
		//---------------------------------------------------
		virtual void DrawHeader(IDMCanvas* pCanvas);
		virtual void DrawTitle(IDMCanvas* pCanvas);
		virtual void DrawCell(IDMCanvas* pCanvas, int iRow, int iCol);
		virtual void DrawToday(IDMCanvas* pCanvas);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_SIZE(OnSize)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)        
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
		DM_END_MSG_MAP()

	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnSize(UINT nType, CSize size);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	public:// ����
		enum HeadType{HEADER, LEFTBTN, RIGHTBTN, MID, MIX};
		CRect CalcHeaderRect(HeadType type);													    ///< ���Header���ֵĸ�����
		CRect CalcTitleRect();																		///< ���Title���ֵ�����
		CRect CalcTodayRect();                                                                      ///< ���Today���ֵ�����
		CStringW CalcJieri(int iMon, int iDay);														///< ���㷵�ؽ����ı�
		int CalcHitCell(CPoint &pt);																///< ����õ������cell����,-1��ʾδ���						

		void OnPreviousClick();
		void OnNextClick();
		void OnCellClick();
		void OnTodayClick();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::INT_headerheight, OnAttributeHeaderHeight)
			DM_INT_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::INT_btnoffset, m_iBtnOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrheaderbg, m_crHeaderBg, DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrheadertext, m_crHeaderText, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::SKIN_headerbgskin, m_pHeaderBgSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::SKIN_leftbtnskin, m_pBtnSkin[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::SKIN_rightbtnskin, m_pBtnSkin[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_SIZE_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::SIZE_btnsize, m_BtnSize, DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::FONT_headerfont, m_ftHeader, DM_ECODE_NOXMLLOADREFRESH)
			DM_WSTRING_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::STRING_headertextformat, m_strHeaderTextFormat, DM_ECODE_NOXMLLOADREFRESH)

			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::INT_titleheight, OnAttributeTitleHeight)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::STRING_weektitle, OnAttributeWeekTitle)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrtitletext, m_crTitleText, DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrtitlebg, m_crTitleBg, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::SKIN_titlebgskin, m_pTitleBgSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::FONT_titlefont, m_ftTitle, DM_ECODE_NOXMLLOADREFRESH)

			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrgraycelltext, m_crGrayCellText[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrgraycelltexthover, m_crGrayCellText[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrgraycelltextpush, m_crGrayCellText[2], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrcell, m_crCell[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrcellhover, m_crCell[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrcellpush, m_crCell[2], DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::SKIN_cellskin, m_pCellSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::bool_bhidegraycell, OnAttributebHideGrayCell)
			DM_bool_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::bool_bshowjieri, m_bShowJieri, DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrjieritext, m_crJieriText, DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::FONT_jierifont, m_ftJieri, DM_ECODE_NOXMLLOADREFRESH)

			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::INT_todayheight, OnAttributeTodayHeight)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrtodaytext, m_crTodayText[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrtodaytexthover, m_crTodayText[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrtodaytextpush, m_crTodayText[2], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrtoday, m_crToday[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrtodayhover, m_crToday[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::COLOR_clrtodaypush, m_crToday[2], DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::SKIN_todayskin, m_pTodaySkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::FONT_todayfont, m_ftToday, DM_ECODE_NOXMLLOADREFRESH)
			DM_WSTRING_ATTRIBUTE(DMAttr::DUIMonthCalCtrlAttr::STRING_todaytextformat, m_strTodayTextFormat, DM_ECODE_NOXMLLOADREFRESH)

		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeHeaderHeight(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeTitleHeight(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeTodayHeight(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeWeekTitle(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributebHideGrayCell(LPCSTR lpszValue, bool bLoadXml);

	public:
		int									   m_iHeaderHei;						 ///< ͷ���߶� 
		int                                    m_iBtnOffset;                         ///< ͷ��С����ƫ��
		DMColor                                m_crHeaderBg;                         ///< ͷ���ı���ɫ
		IDMSkinPtr                             m_pHeaderBgSkin;						 ///< ͷ���ı���ͼ
		IDMSkinPtr							   m_pBtnSkin[2];						 ///< ͷ��С���ǰ�ť��Դ   
		CSize								   m_BtnSize;                            ///< ͷ��С���ǰ�ť�ĳ���
		DWORD								   m_dwBtnState[2];						 ///< ͷ��С���ǰ�ť״̬  
		DMColor								   m_crHeaderText;                       ///< ͷ���м�������ɫ
		DMSmartPtrT<IDMFont>				   m_ftHeader;                           ///< ͷ���м�����
		CStringW                               m_strHeaderTextFormat;                ///< ͷ���м��ı���ʾ��ʽ
		//
		int                                    m_iTitleHei;                          ///< Title�߶�
		CStringW                               m_strWeekTitle[7];                    ///< Title���������ʶ  
		DMColor                                m_crTitleText;                        ///< Title��������ɫ
		DMColor                                m_crTitleBg;                          ///< Title�ı���ɫ
		IDMSkinPtr                             m_pTitleBgSkin;						 ///< Title�ı���ͼ
		DMSmartPtrT<IDMFont>				   m_ftTitle;                            ///< Title������
		//
		bool                                   m_bHideGrayCell;                      ///< �Ƿ����ػҵ���cell,Ĭ����ʾ
		DMColor                                m_crGrayCellText[3];					 ///< �ҵ�cell���ı���ɫ(ǰһ���ºͺ�һ����Ϊ�ҵ�cell,Ĭ�ϵ�cell��ʹ��style���ı���ɫ)
		DMColor								   m_crCell[3];							 ///< ����Cell�ı�����ɫ
		IDMSkinPtr                             m_pCellSkin;                          ///< ����Cell�ı���ͼ
		bool                                   m_bShowJieri;                         ///< ����cell�Ƿ���ʾ�����ı�
		DMColor								   m_crJieriText;                        ///< �����ı���ɫ
		DMSmartPtrT<IDMFont>				   m_ftJieri;                            ///< �����ı�������
		//
		int                                    m_iTodayHei;                          ///< Today�ĸ߶�
		DMColor                                m_crTodayText[3];                     ///< Today���ı���ɫ
		DMColor                                m_crToday[3];                         ///< Today�ı�����ɫ
		IDMSkinPtr                             m_pTodaySkin;						 ///< Today�ı���ͼ
		DMSmartPtrT<IDMFont>				   m_ftToday;                            ///< Today���ı�����
		CStringW                               m_strTodayTextFormat;                 ///< Today���ı���ʾ��ʽ
		DWORD								   m_dwTodayState;						 ///< Today��״̬
		//
		DMCalendarCell						   m_dayCells[CALENDAR_ROWS][CALENDAR_COLUMNS];   
		DMDateTime							   m_displayDate;						 ///< ��ǰ��ʾʱ��
		int				                       m_iselCell;							 ///< ѡ��Cell
		int			                           m_ihoverCell;						 ///< ͣ��Cell
		CRect								   m_rcCells;							 ///< Cells��������
	};

}// namespace DM