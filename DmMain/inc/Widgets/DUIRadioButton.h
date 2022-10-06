//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIRadioButton.h 
// File Des: ���õ�ѡ��ťʵ��
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-18	1.0			
//--------------------------------------------------------
#pragma once
#include "Widgets/DMWidgetDef.h"


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIRadioButton"/>��xml���Զ���
	/// </summary>
	class DUIRadioButtonAttr:public DUIWindowAttr
	{
	public:
		static const char* bool_bautogroup;                                ///< �Ƿ��Զ�������,ͬһ���������µ���radio���ᱻtab�л�����,ʾ��:bautogroup="1"
		static const char* bool_brightbox;                                 ///< boxͼ���Ƿ����ұ�,ʾ��:brightbox="1"
		static const char* bool_bmultilines;								///< �Ƿ�֧�ֶ���,ʾ��:bmultilines="1"
		static const char* INT_lineinter;                                  ///< �ڶ�����,�����м��,ʾ��:lineinter="1"
		static const char* SKIN_radioskin;                                 ///< radioͼ,8��ͼ��2��ͼ��1��ͼ,ǰ4Ϊδѡ������״̬����4Ϊѡ��ʱ����״̬,ʾ��:radioskin="PNG:radiobutton"
		static const char* SKIN_iconskin;                                  ///< iconͼ,��ʹ����iconͼ,��text���ٻ���,ʾ��:iconskin="PNG:iconbtn"
		static const char* INT_textoffset;                                 ///< �ı�����icon��ƫ�ƣ�Ĭ��Ϊ0,ʾ��:textoffset="10"
	};
	DMAttrValueInit(DUIRadioButtonAttr,bool_bautogroup)
	DMAttrValueInit(DUIRadioButtonAttr,bool_brightbox)
	DMAttrValueInit(DUIRadioButtonAttr,bool_bmultilines)
	DMAttrValueInit(DUIRadioButtonAttr,INT_lineinter)
	DMAttrValueInit(DUIRadioButtonAttr,SKIN_radioskin)
	DMAttrValueInit(DUIRadioButtonAttr,SKIN_iconskin)
	DMAttrValueInit(DUIRadioButtonAttr,INT_textoffset)
}

namespace DM
{
	/// <summary>
	///		 DUIRadioButton������ʵ�֣����ԣ�<see cref="DMAttr::DUIRadioButtonAttr"/>
	/// </summary>
	class DM_EXPORT DUIRadioButton:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIRadioButton,DUINAME_RadioButton,DMREG_Window);
	public:
		DUIRadioButton();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MESSAGE_HANDLER_EX(WM_SETFOCUS,DM_OnSetFocus)
			MSG_WM_LBUTTONUP(OnLButtonUp)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		LRESULT DM_OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnLButtonUp(UINT nFlags,CPoint pt);

	public:
		//---------------------------------------------------
		// Function Des: ����DUIWindow
		//---------------------------------------------------
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);
		DMCode DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState);
		DUIWindow* DV_GetSelSiblingInGroup();
		bool DV_IsStateChangeRedraw(){return true;}
	    bool DV_IsSiblingsAutoGroup(){return m_bAutoGroup;}

	public:// ����
		int GetDrawState();
		CRect GetRadioRect();
		
	public:  
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUIRadioButtonAttr::bool_bautogroup,  m_bAutoGroup,  DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIRadioButtonAttr::bool_brightbox,   m_bRightBox,   DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIRadioButtonAttr::bool_bmultilines, m_bMultiLines, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIRadioButtonAttr::INT_textoffset,    m_nTextOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIRadioButtonAttr::INT_lineinter,     m_nLineInter,  DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIRadioButtonAttr::SKIN_radioskin,m_pRadioSkin,  DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIRadioButtonAttr::SKIN_iconskin, m_pIconSkin,   DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()

	public:
		bool                                m_bAutoGroup;
		bool                                m_bRightBox;
		bool                                m_bMultiLines;
		int                                 m_nTextOffset;
		int                                 m_nLineInter;
		IDMSkinPtr                          m_pRadioSkin;
		IDMSkinPtr                          m_pIconSkin;
	};
}//namespace DM