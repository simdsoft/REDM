// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIRect.h
// File mark:   
// File summary:四个小EDIT模拟RECT的left,top,right,bottom
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-15
// ----------------------------------------------------------------
#pragma once
#include "DUIEdit.h"

namespace DMAttr
{
	class DUIRectAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* COLOR_clrdot;                                   ///< dot颜色,示例:clrdot="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clrtext;									///< 默认文本颜色，和style的clrtext相同作用.示例:clrtext="pbgra(00,00,00,ff)"
		// 光标
		static wchar_t* COLOR_clrcaret;                                 ///< 光标颜色,示例:clrcaret="pbgra(ff,ff,ff,ff)"
		static wchar_t* INI_caretanimatecount;                          ///< 光标渐隐渐显次数，1次为正常状态，默认为6次,示例： caretanimatecount="6"
	};
	DMAttrValueInit(DUIRectAttr,COLOR_clrdot)DMAttrValueInit(DUIRectAttr,COLOR_clrtext)
	DMAttrValueInit(DUIRectAttr,COLOR_clrcaret)DMAttrValueInit(DUIRectAttr,INI_caretanimatecount)
}

namespace DM
{
	class DSEdit:public DUIEdit
	{
		DMDECLARE_CLASS_NAME(DSEdit,L"dsedit",DMREG_Window);
	public:
		DSEdit();
		int GetField();
		void SetField(int iField);
	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_CHAR(OnChar)
			MESSAGE_HANDLER_EX(WM_IME_COMPOSITION, OnImeChar)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
		DM_END_MSG_MAP()
	public:
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		LRESULT OnImeChar(UINT uMsg, WPARAM wParam, LPARAM lParam);
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		
	public:// 辅助
		bool AdjustNum();

	public: 
		int           m_iMin;
		int           m_iMax;
	};

	/// <summary>
	///		 DUIRect的内置实现，属性：<see cref="DMAttr::DUIRectAttr"/>
	/// </summary>
	class DUIRect: public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIRect,L"duirect",DMREG_Window);
	public:
		DUIRect();
		BOOL IsBlank() const;
		void ClearAddress();

		int GetAddress(int& nField0, int& nField1, int& nField2, int& nField3) const;
		int GetAddress(CRect & rcAddress) const;

		void SetAddress(CRect rcAddress);
		void SetAddress(int nField0, int nField1, int nField2, int nField3);
		void SetFieldFocus(WORD nField);
		void SetFieldRange(int nField, int nLower, int nUpper);

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
			DM_COLOR_ATTRIBUTE(DMAttr::DUIRectAttr::COLOR_clrdot,m_clrDot,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectAttr::COLOR_clrtext,OnAttrTextColor)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectAttr::COLOR_clrcaret,OnAttrCuretClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectAttr::INI_caretanimatecount,OnAttrCuretAnimateCount)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttrTextColor(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretClr(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretAnimateCount(LPCWSTR pszValue, bool bLoadXml);
	public:
		DMSmartPtrT<DSEdit>                     m_pEdit[4];
		DMColor                                 m_clrDot;
		DMColor                                 m_clrText;
		DMColor									m_clrCaret;
		int										m_iCaretAniCount;
	};
}//namespace DM