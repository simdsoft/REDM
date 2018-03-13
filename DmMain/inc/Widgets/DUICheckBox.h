//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUICheckBox.h 
// File Des: 内置CheckBox实现
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
	///		<see cref="DM::DUICheckBox"/>的xml属性定义
	/// </summary>
	class DUICheckBoxAttr :public DUIWindowAttr
	{
	public:
		static wchar_t* bool_brightbox;                                 ///< box图标是否在右边,示例:brightbox="1"
		static wchar_t* bool_bmultilines;								///< 是否支持多行,示例:bmultilines="1"
		static wchar_t* INT_lineinter;                                  ///< 在多行下,设置行间距,示例:lineinter="1"
		static wchar_t* SKIN_checkskin;                                 ///< check图,8分图或2分图或1分图,8分图,前4为未选中四种状态，后4为选中时四种状态,示例:checkskin="PNG:checkbox"
		static wchar_t* SKIN_iconskin;                                  ///< icon图,如使用了icon图,则text不再绘制,示例:iconskin="PNG:iconbtn"
		static wchar_t* INT_textoffset;                                 ///< 文本距离icon的偏移，默认为0,示例:textoffset=10
		static wchar_t* bool_bchecked;									///< 是否默认被选中,示例:bchecked="1"
	};
	DMAttrValueInit(DUICheckBoxAttr, bool_brightbox)
	DMAttrValueInit(DUICheckBoxAttr, bool_bmultilines)
	DMAttrValueInit(DUICheckBoxAttr, INT_lineinter)
	DMAttrValueInit(DUICheckBoxAttr, SKIN_checkskin)
	DMAttrValueInit(DUICheckBoxAttr, SKIN_iconskin)
	DMAttrValueInit(DUICheckBoxAttr, INT_textoffset)
	DMAttrValueInit(DUICheckBoxAttr, bool_bchecked)
}

namespace DM
{
	/// <summary>
	///		DUICheckBox的内置实现，属性：<see cref="DMAttr::DUICheckBoxAttr"/>
	/// </summary>
	class DM_EXPORT DUICheckBox :public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUICheckBox, DUINAME_CheckBox, DMREG_Window)
	public:
		DUICheckBox();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonUp(UINT nFlags, CPoint pt);

	public:
		//---------------------------------------------------
		// Function Des: 重载DUIWindow
		//---------------------------------------------------
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf, int cchText, LPRECT lpRect, UINT uFormat);
		bool DV_IsStateChangeRedraw() { return true; }

		//---------------------------------------------------
		// Function Des: 换肤
		//---------------------------------------------------
		DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);

	public:// 辅助
		int GetDrawState();
		CRect GetCheckRect();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUICheckBoxAttr::bool_brightbox, m_bRightBox, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUICheckBoxAttr::bool_bmultilines, m_bMultiLines, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUICheckBoxAttr::INT_lineinter, m_nLineInter, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUICheckBoxAttr::INT_textoffset, m_nTextOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUICheckBoxAttr::SKIN_checkskin, m_pCheckSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUICheckBoxAttr::SKIN_iconskin, m_pIconSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUICheckBoxAttr::bool_bchecked, OnAttributeChecked)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeChecked(LPCWSTR lpszValue, bool bLoadXml);

	public:
		bool                                m_bRightBox;
		bool                                m_bMultiLines;
		int                                 m_nLineInter;
		int                                 m_nTextOffset;
		IDMSkinPtr                          m_pCheckSkin;
		IDMSkinPtr                          m_pIconSkin;
	};

}//namespace DM