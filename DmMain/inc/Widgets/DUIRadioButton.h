//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIRadioButton.h 
// File Des: 内置单选按钮实现
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-18	1.0			
//--------------------------------------------------------
#pragma once
#include "DMWidgetDef.h"


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIRadioButton"/>的xml属性定义
	/// </summary>
	class DUIRadioButtonAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* bool_bautogroup;                                ///< 是否自动加入组,同一个父窗口下的组radio不会被tab切换焦点,示例:bautogroup="1"
		static wchar_t* bool_brightbox;                                 ///< box图标是否在右边,示例:brightbox="1"
		static wchar_t* bool_bmultilines;								///< 是否支持多行,示例:bmultilines="1"
		static wchar_t* INT_lineinter;                                  ///< 在多行下,设置行间距,示例:lineinter="1"
		static wchar_t* SKIN_radioskin;                                 ///< radio图,8分图或2分图或1分图,前4为未选中四种状态，后4为选中时四种状态,示例:radioskin="PNG:radiobutton"
		static wchar_t* SKIN_iconskin;                                  ///< icon图,如使用了icon图,则text不再绘制,示例:iconskin="PNG:iconbtn"
		static wchar_t* INT_textoffset;                                 ///< 文本距离icon的偏移，默认为0,示例:textoffset="10"
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
	///		 DUIRadioButton的内置实现，属性：<see cref="DMAttr::DUIRadioButtonAttr"/>
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
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		LRESULT DM_OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnLButtonUp(UINT nFlags,CPoint pt);

	public:
		//---------------------------------------------------
		// Function Des: 重载DUIWindow
		//---------------------------------------------------
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);
		DMCode DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState);
		DUIWindow* DV_GetSelSiblingInGroup();
		bool DV_IsStateChangeRedraw(){return true;}
	    bool DV_IsSiblingsAutoGroup(){return m_bAutoGroup;}

	public:// 辅助
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