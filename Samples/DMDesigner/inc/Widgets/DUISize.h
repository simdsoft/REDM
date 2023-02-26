// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUISize.h
// File mark:   
// File summary:ģ��size,point
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-11
// ----------------------------------------------------------------
#pragma once
#include "DUIEdit.h"

namespace DMAttr
{
	class DUISizeAttr:public DUIWindowAttr
	{
	public:
		static const char* COLOR_clrdot;                                   ///< dot��ɫ,ʾ��:clrdot="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clrtext;									///< Ĭ���ı���ɫ����style��clrtext��ͬ����.ʾ��:clrtext="pbgra(00,00,00,ff)"
		// ���
		static const char* COLOR_clrcaret;                                 ///< �����ɫ,ʾ��:clrcaret="pbgra(ff,ff,ff,ff)"
		static const char* INI_caretanimatecount;                          ///< ��꽥�����Դ�����1��Ϊ����״̬��Ĭ��Ϊ6��,ʾ���� caretanimatecount="6"
	};
	DMAttrValueInit(DUISizeAttr,COLOR_clrdot)DMAttrValueInit(DUISizeAttr,COLOR_clrtext)
		DMAttrValueInit(DUISizeAttr,COLOR_clrcaret)DMAttrValueInit(DUISizeAttr,INI_caretanimatecount)
}
 
namespace DM
{
	class DSizeEdit:public DUIEdit
	{
		DMDECLARE_CLASS_NAME(DSizeEdit,"dsizeedit",DMREG_Window);
	public:
		DSizeEdit();
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
		
	public:// ����
		bool AdjustNum();

	public: 
		int           m_iMin;
		int           m_iMax;
	};

	/// <summary>
	///		 DUISize������ʵ�֣����ԣ�<see cref="DMAttr::DUISizeAttr"/>
	/// </summary>
	class DUISize: public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUISize,"duisize",DMREG_Window);
	public:
		DUISize();
		BOOL IsBlank() const;
		void ClearSize();

		int GetSize(int& nField0, int& nField1) const;
		int GetSize(CSize & rcSize) const;

		void SetSize(CSize rcSize);
		void SetSize(int nField0, int nField1);
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
		// Function Des: ����
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_COLOR_ATTRIBUTE(DMAttr::DUISizeAttr::COLOR_clrdot,m_clrDot,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUISizeAttr::COLOR_clrtext,OnAttrTextColor)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUISizeAttr::COLOR_clrcaret,OnAttrCuretClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUISizeAttr::INI_caretanimatecount,OnAttrCuretAnimateCount)
			DM_END_ATTRIBUTES()
	public:
		DMCode OnAttrTextColor(LPCSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretClr(LPCSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretAnimateCount(LPCSTR pszValue, bool bLoadXml);
	public:
		DMSmartPtrT<DSizeEdit>                     m_pEdit[2];
		DMColor									   m_clrDot;
		DMColor									   m_clrText;
		DMColor									   m_clrCaret;
		int										   m_iCaretAniCount;
	};
}//namespace DM

