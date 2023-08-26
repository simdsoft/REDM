//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIMonthCalCtrl.h
// File Des: 内置日历实现
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
	///		<see cref="DM::DUIMonthCalCtrl"/>的xml属性定义
	/// </summary>
	class DUIMonthCalCtrlAttr:public DUIWindowAttr
	{
	public:
		static const char* INT_headerheight;												  ///< 头部的高度,默认为24,示例:headerheight="24"
		static const char* INT_btnoffset;													  ///< 头部左右小三角相对于边框的偏移，默认为4,示例:btnoffset="4"
		static const char* COLOR_clrheaderbg;												  ///< 头部的背景色,示例:clrheaderbg="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_headerbgskin;                                                ///< 头部的背景图,示例:headerbgskin="bgskin"
		static const char* SKIN_leftbtnskin;                                                 ///< 左边的小三角按钮皮肤，示例:leftbtnskin="skinbtn"
		static const char* SKIN_rightbtnskin;                                                ///< 右边的小三角按钮皮肤，示例:rightbtnskin="skinbtn"
		static const char* SIZE_btnsize;													  ///< 小三角的宽高,为-1,-1时表示使用btnskin的size,竖直居中,示例:btnsize="-1,-1"
		static const char* COLOR_clrheadertext;                                              ///< 头部中间的文本颜色,示例:clrheadertext="pbgra(ff,ff,ff,ff)"
		static const char* FONT_headerfont;												  ///< 头部中间的文本字体,示例:headerfont="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		static const char* STRING_headertextformat;                                          ///< 头部中间的文本format,带有格示化数字的符号,示例:headertextformat="%04d年%02d月"				
		//
		static const char* INT_titleheight;												  ///< title的高度,默认为24,示例:titleheight="24"
		static const char* STRING_weektitle;												  ///< 七天的表示法,以逗号分隔,示例:weektitle="日,一, 二, 三, 四, 五, 六"
		static const char* COLOR_clrtitletext;												  ///< title的字体色,示例:clrtitletext="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtitlebg;												  ///< title的背景色,示例:clrtitlebg="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_titlebgskin;                                                 ///< title的背景图,示例:titlebgskin="bgskin"
		static const char* FONT_titlefont;													  ///< title的文本字体,示例:titlefont="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		//
		static const char* bool_bhidegraycell;                                               ///< 是否隐藏gray cell,示例:bhidegraycell="0"
		static const char* COLOR_clrgraycelltext;											  ///< graycell的normarl文本色(上下月的cell为greycell),示例:clrgraycelltext="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrgraycelltexthover;									      ///< graycell的hover文本色(上下月的cell为greycell),示例:clrgraycelltexthover="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrgraycelltextpush;									      ///< graycell的push文本色(上下月的cell为greycell),示例:clrgraycelltextpush="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrcell;													  ///< 所有cell的背景色,示例:clrcell="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrcellhover;												  ///< 所有cell的hover背景色,示例:clrcellhover="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrcellpush;												  ///< 所有cell的push背景色,示例:clrcellpush="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_cellskin;                                                    ///< 所有cell的三态背景图,示例:cellskin="skin"
		static const char* bool_bshowjieri;									    		  ///< 是否显示节日,如果显示,则cell的文本占上方的2/3,节日占下方1/3,示例:bshowjieri="1"
		static const char* COLOR_clrjieritext;												  ///< 节日的字体色,示例:clrjieritext="pbgra(ff,ff,ff,ff)"
		static const char* FONT_jierifont;													  ///< 节日的文本字体,示例:jierifont="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		//
		static const char* INT_todayheight;												  ///< today的高度,默认为24,示例:todayheight="24"
		static const char* COLOR_clrtodaytext;												  ///< today的normarl文本色,示例:clrtodaytext="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtodaytexthover;									      ///< today的hover文本色,示例:clrtodaytexthover="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtodaytextpush;											  ///< today的push文本色,示例:clrtodaytextpush="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtoday;													  ///< today的背景色,示例:clrtoday="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtodayhover;											  ///< today的hover背景色,示例:clrtodayhover="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtodaypush;												  ///< today的push背景色,示例:clrtodaypush="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_todayskin;                                                   ///< 每个today的三态背景图,示例:todayskin="skin"
		static const char* FONT_todayfont;													  ///< today的文本字体,示例:todayfont="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		static const char* STRING_todaytextformat;                                           ///< today的文本format,带有格示化数字的符号,示例:todaytextformat="今天:%04d/%02d/%02d"			

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
		DMDateTime						date;		///< 日期	
		CStringW						title;		///< 标题
		CStringW						jieri;      ///< 节日                  
		bool							bGray;		///< 是否为灰
		bool							bMark;		///< 是否被标记
		CRect                           rect;       ///< 所占的区域
	}DMCalendarCell,*LPDMCalendarCell;
	#define CALENDAR_ROWS			6				///< 显示区域有多少行
	#define CALENDAR_COLUMNS		7				///< 显示区域有多少列

	/// <summary>
	///		 DUIMonthCalCtrl的内置实现，属性：<see cref="DMAttr::DUIMonthCalCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUIMonthCalCtrl:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIMonthCalCtrl,DUINAME_MonthCalCtrl,DMREG_Window);
	public:
		DUIMonthCalCtrl();

	public:
		//---------------------------------------------------
		// Function Des: 对外接口 methods
		//---------------------------------------------------
		DMDateTime GetDisplayDate() const;
		void SetDisplayDate(DMDateTime date, bool bNotify=true);

		void UpdateDisplayCells();																	///< 更新当前的cells列表
		void UpdateDisplayCellsRect();																///< 更新当前的cells列表区域

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
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnSize(UINT nType, CSize size);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	public:// 辅助
		enum HeadType{HEADER, LEFTBTN, RIGHTBTN, MID, MIX};
		CRect CalcHeaderRect(HeadType type);													    ///< 获得Header部分的各区域
		CRect CalcTitleRect();																		///< 获得Title部分的区域
		CRect CalcTodayRect();                                                                      ///< 获得Today部分的区域
		CStringW CalcJieri(int iMon, int iDay);														///< 计算返回节日文本
		int CalcHitCell(CPoint &pt);																///< 计算得到点击的cell索引,-1表示未点击						

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
		int									   m_iHeaderHei;						 ///< 头部高度 
		int                                    m_iBtnOffset;                         ///< 头部小三角偏移
		DMColor                                m_crHeaderBg;                         ///< 头部的背景色
		IDMSkinPtr                             m_pHeaderBgSkin;						 ///< 头部的背景图
		IDMSkinPtr							   m_pBtnSkin[2];						 ///< 头部小三角按钮资源   
		CSize								   m_BtnSize;                            ///< 头部小三角按钮的长宽
		DWORD								   m_dwBtnState[2];						 ///< 头部小三角按钮状态  
		DMColor								   m_crHeaderText;                       ///< 头部中间文字颜色
		DMSmartPtrT<IDMFont>				   m_ftHeader;                           ///< 头部中间字体
		CStringW                               m_strHeaderTextFormat;                ///< 头部中间文本显示格式
		//
		int                                    m_iTitleHei;                          ///< Title高度
		CStringW                               m_strWeekTitle[7];                    ///< Title日历七天标识  
		DMColor                                m_crTitleText;                        ///< Title的字体颜色
		DMColor                                m_crTitleBg;                          ///< Title的背景色
		IDMSkinPtr                             m_pTitleBgSkin;						 ///< Title的背景图
		DMSmartPtrT<IDMFont>				   m_ftTitle;                            ///< Title的字体
		//
		bool                                   m_bHideGrayCell;                      ///< 是否隐藏灰掉的cell,默认显示
		DMColor                                m_crGrayCellText[3];					 ///< 灰掉cell的文本三色(前一个月和后一个月为灰掉cell,默认的cell就使用style的文本三色)
		DMColor								   m_crCell[3];							 ///< 所有Cell的背景三色
		IDMSkinPtr                             m_pCellSkin;                          ///< 所有Cell的背景图
		bool                                   m_bShowJieri;                         ///< 所有cell是否显示节日文本
		DMColor								   m_crJieriText;                        ///< 节日文本颜色
		DMSmartPtrT<IDMFont>				   m_ftJieri;                            ///< 节日文本的字体
		//
		int                                    m_iTodayHei;                          ///< Today的高度
		DMColor                                m_crTodayText[3];                     ///< Today的文本三色
		DMColor                                m_crToday[3];                         ///< Today的背景三色
		IDMSkinPtr                             m_pTodaySkin;						 ///< Today的背景图
		DMSmartPtrT<IDMFont>				   m_ftToday;                            ///< Today的文本字体
		CStringW                               m_strTodayTextFormat;                 ///< Today的文本显示格式
		DWORD								   m_dwTodayState;						 ///< Today的状态
		//
		DMCalendarCell						   m_dayCells[CALENDAR_ROWS][CALENDAR_COLUMNS];   
		DMDateTime							   m_displayDate;						 ///< 当前显示时间
		int				                       m_iselCell;							 ///< 选中Cell
		int			                           m_ihoverCell;						 ///< 停留Cell
		CRect								   m_rcCells;							 ///< Cells的总区域
	};

}// namespace DM