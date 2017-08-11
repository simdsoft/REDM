//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIPAddressCtrl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-8-8	1.0			
//-------------------------------------------------------
#pragma once
#include "DUIEdit.h"

namespace DMAttr
{
	class DUIPAddressCtrlAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* COLOR_clrdot;                                   ///< dot颜色,示例:clrdot="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clrtext;									///< 默认文本颜色，和style的clrtext相同作用.示例:clrtext="pbgra(00,00,00,ff)"
		static wchar_t* OPTION_align;									///< 段落排布，和style的align相同作用,默认left,可设置center,right,示例:align="left"
		// 光标
		static wchar_t* COLOR_clrcaret;                                 ///< 光标颜色,示例:clrcaret="pbgra(ff,ff,ff,ff)"
		static wchar_t* INI_caretanimatecount;                          ///< 光标渐隐渐显次数，1次为正常状态，默认为6次,示例： caretanimatecount="6"
	};
	DMAttrValueInit(DUIPAddressCtrlAttr,COLOR_clrdot)DMAttrValueInit(DUIPAddressCtrlAttr,COLOR_clrtext)
	DMAttrValueInit(DUIPAddressCtrlAttr,COLOR_clrcaret)DMAttrValueInit(DUIPAddressCtrlAttr,INI_caretanimatecount)
	DMAttrValueInit(DUIPAddressCtrlAttr,OPTION_align);
}

namespace DM
{
	class DM_EXPORT DUIPEdit:public DUIEdit
	{
		DMDECLARE_CLASS_NAME(DUIPEdit,DUINAME_IPEdit,DMREG_Window);
	public:
		DUIPEdit();
		BYTE GetField();
		void SetField(BYTE byField);
	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_CHAR(OnChar)
		DM_END_MSG_MAP()
	public:
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	public:// 辅助
		bool AdjustNum();

	public: 
		BYTE           m_byMin;
		BYTE           m_byMax;
	};

	/// <summary>
	///		 DUIPAddressCtrl的内置实现，属性：<see cref="DMAttr::DUIPAddressCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUIPAddressCtrl: public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIPAddressCtrl,DUINAME_IPAddress,DMREG_Window);
	public:
		DUIPAddressCtrl();
		// Determines if all fields in the IP address control are blank.
		BOOL IsBlank() const;

		// Clears the contents of the IP address control.
		void ClearAddress();

		// Retrieves the address values for all four fields in the IP address control.
		int GetAddress(BYTE& nField0, BYTE& nField1, BYTE& nField2, BYTE& nField3) const;
		int GetAddress(DWORD& dwAddress) const;
		
		// Sets the address values for all four fields in the IP address control.
		void SetAddress(DWORD dwAddress);
		void SetAddress(BYTE nField0, BYTE nField1, BYTE nField2, BYTE nField3);

		// Sets the keyboard focus to the specified field in the IP address control.
		void SetFieldFocus(WORD nField);

		// Sets the valid range for the specified field in the IP address control.
		void SetFieldRange(int nField, BYTE nLower, BYTE nUpper);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_SIZE(OnSize)
		DM_END_MSG_MAP()
	public:
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnSize(UINT nType, CSize size);
	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_COLOR_ATTRIBUTE(DMAttr::DUIPAddressCtrlAttr::COLOR_clrdot,m_clrDot,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIPAddressCtrlAttr::COLOR_clrtext,OnAttrTextColor)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIPAddressCtrlAttr::COLOR_clrcaret,OnAttrCuretClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIPAddressCtrlAttr::INI_caretanimatecount,OnAttrCuretAnimateCount)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIPAddressCtrlAttr::OPTION_align,OnAttrAlign)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttrTextColor(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretClr(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretAnimateCount(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrAlign(LPCWSTR pszValue, bool bLoadXml);
	public:
		DMSmartPtrT<DUIPEdit>                   m_pEdit[4];
		DMColor                                 m_clrDot;
		DMColor                                 m_clrText;
		DMColor									m_clrCaret;
		int										m_iCaretAniCount;
		CStringW                                m_strAlign;
	};
}//namespace DM