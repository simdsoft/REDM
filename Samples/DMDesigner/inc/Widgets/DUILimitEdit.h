// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUILimitEdit.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-18
// ----------------------------------------------------------------
#pragma once
 
namespace DMAttr
{
	class DUILimitEditAttr:public DUIEditAttr
	{
	public:
		static wchar_t* STRING_includechars;									///< 指定只能输入它指定的文字,示例:includechars="123456789"
		static wchar_t* STRING_excludechars;									///< 指定排除它指定的文字,示例:excludechars="123456789"
		static wchar_t* STRING_firstincludechars;								///< 指定首字符只能输入它指定的文字,示例:firstincludechars="123456789"
		static wchar_t* STRING_firstexcludechars;								///< 指定首字符排除它指定的文字,示例:firstexcludechars="123456789"
		static wchar_t* bool_bnocase;                                           ///< 无视大小写,默认为1,示例:bnocase = "1"
	}; 
	DMAttrValueInit(DUILimitEditAttr,STRING_includechars)DMAttrValueInit(DUILimitEditAttr,STRING_excludechars)
	DMAttrValueInit(DUILimitEditAttr,STRING_firstincludechars)DMAttrValueInit(DUILimitEditAttr,STRING_firstexcludechars)DMAttrValueInit(DUILimitEditAttr,bool_bnocase)
}

class DUILimitEdit : public DUIEdit
{
	DMDECLARE_CLASS_NAME(DUIEdit, L"limitedit", DMREG_Window)
public:
	DUILimitEdit();

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

public:
	DM_BEGIN_ATTRIBUTES()
		DM_STRING_ATTRIBUTE(DMAttr::DUILimitEditAttr::STRING_includechars,m_strIncludeChars,DM_ECODE_NOXMLLOADREFRESH)
		DM_STRING_ATTRIBUTE(DMAttr::DUILimitEditAttr::STRING_excludechars,m_strExcludeChars,DM_ECODE_NOXMLLOADREFRESH)
		DM_STRING_ATTRIBUTE(DMAttr::DUILimitEditAttr::STRING_firstincludechars,m_strFirstIncludeChars,DM_ECODE_NOXMLLOADREFRESH)
		DM_STRING_ATTRIBUTE(DMAttr::DUILimitEditAttr::STRING_firstexcludechars,m_strFirstExcludeChars,DM_ECODE_NOXMLLOADREFRESH)
	DM_END_ATTRIBUTES()

public: 
	CStringW             m_strIncludeChars;				///< 字符限制在此类字符串内
	CStringW             m_strExcludeChars;				///< 排除以下字符串
	CStringW             m_strFirstIncludeChars;		///< 首字符限制在此类字符串内
	CStringW             m_strFirstExcludeChars;		///< 首字符限制排除此类字符
	bool                 m_bNoCase;						///< 忽视大小
};