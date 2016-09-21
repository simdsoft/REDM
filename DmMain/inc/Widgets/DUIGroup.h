//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIGroup.h 
// File Des: 内置Group实现
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-16	1.0			
//--------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIGroup"/>的xml属性定义
	/// </summary>
	class DUIGroupAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* INT_round;								///< 设置group的圆角半径大小,示例:round="4"
		static wchar_t* INT_linewid;                            ///< 线条的宽度,示例:linewid="1"
		static wchar_t* RECT_rcinset;							///< 设置group的子窗口和外框的四边距,示例:rcinset="10,5,5,5"
		static wchar_t* COLOR_clrline;							///< 设置group的线颜色,示例:clrline=pbgra(ff,00,00,80) 
	};
	DMAttrValueInit(DUIGroupAttr,INT_round)
	DMAttrValueInit(DUIGroupAttr,INT_linewid)
	DMAttrValueInit(DUIGroupAttr,RECT_rcinset)
	DMAttrValueInit(DUIGroupAttr,COLOR_clrline)
}

namespace DM
{
	/// <summary>
	///		DUIGroup的内置实现，属性：<see cref="DMAttr::DUIGroupAttr"/>
	/// </summary>
	class DM_EXPORT DUIGroup:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIGroup, DUINAME_Group,DMREG_Window)
	public:
		DUIGroup();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);

	public:
		DMCode DV_GetChildMeasureLayout(LPRECT lpRect);

	public:  
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIGroupAttr::INT_round,m_iRound, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIGroupAttr::INT_linewid,m_iLineWid, DM_ECODE_NOXMLLOADREFRESH)
			DM_RECT_ATTRIBUTE(DMAttr::DUIGroupAttr::RECT_rcinset,m_rcInset, DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIGroupAttr::COLOR_clrline,m_crLine, DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
	public:
		DMColor									   m_crLine;
		CRect									   m_rcInset;       ///< 子窗口和外框的四边距
		int										   m_iRound;        ///< 圆角半径	
		int                                        m_iLineWid;      ///< 线条的宽度
	};
}//namespace DM