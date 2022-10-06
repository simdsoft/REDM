//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIGroup.h 
// File Des: ����Groupʵ��
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
	///		<see cref="DM::DUIGroup"/>��xml���Զ���
	/// </summary>
	class DUIGroupAttr:public DUIWindowAttr
	{
	public:
		static const char* INT_round;								///< ����group��Բ�ǰ뾶��С,ʾ��:round="4"
		static const char* INT_linewid;                            ///< �����Ŀ��,ʾ��:linewid="1"
		static const char* RECT_rcinset;							///< ����group���Ӵ��ں������ı߾�,ʾ��:rcinset="10,5,5,5"
		static const char* COLOR_clrline;							///< ����group������ɫ,ʾ��:clrline=pbgra(ff,00,00,80) 
	};
	DMAttrValueInit(DUIGroupAttr,INT_round)
	DMAttrValueInit(DUIGroupAttr,INT_linewid)
	DMAttrValueInit(DUIGroupAttr,RECT_rcinset)
	DMAttrValueInit(DUIGroupAttr,COLOR_clrline)
}

namespace DM
{
	/// <summary>
	///		DUIGroup������ʵ�֣����ԣ�<see cref="DMAttr::DUIGroupAttr"/>
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
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
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
		CRect									   m_rcInset;       ///< �Ӵ��ں������ı߾�
		int										   m_iRound;        ///< Բ�ǰ뾶	
		int                                        m_iLineWid;      ///< �����Ŀ��
	};
}//namespace DM