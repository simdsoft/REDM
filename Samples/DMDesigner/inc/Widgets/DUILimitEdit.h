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
		static const char* STRING_includechars;									///< ָ��ֻ��������ָ��������,ʾ��:includechars="123456789"
		static const char* STRING_excludechars;									///< ָ���ų���ָ��������,ʾ��:excludechars="123456789"
		static const char* STRING_firstincludechars;								///< ָ�����ַ�ֻ��������ָ��������,ʾ��:firstincludechars="123456789"
		static const char* STRING_firstexcludechars;								///< ָ�����ַ��ų���ָ��������,ʾ��:firstexcludechars="123456789"
		static const char* bool_bnocase;                                           ///< ���Ӵ�Сд,Ĭ��Ϊ1,ʾ��:bnocase = "1"
	}; 
	DMAttrValueInit(DUILimitEditAttr,STRING_includechars)DMAttrValueInit(DUILimitEditAttr,STRING_excludechars)
	DMAttrValueInit(DUILimitEditAttr,STRING_firstincludechars)DMAttrValueInit(DUILimitEditAttr,STRING_firstexcludechars)DMAttrValueInit(DUILimitEditAttr,bool_bnocase)
}

class DUILimitEdit : public DUIEdit
{
	DMDECLARE_CLASS_NAME(DUIEdit, "limitedit", DMREG_Window)
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
		DM_WSTRING_ATTRIBUTE(DMAttr::DUILimitEditAttr::STRING_includechars,m_strIncludeChars,DM_ECODE_NOXMLLOADREFRESH)
		DM_WSTRING_ATTRIBUTE(DMAttr::DUILimitEditAttr::STRING_excludechars,m_strExcludeChars,DM_ECODE_NOXMLLOADREFRESH)
		DM_WSTRING_ATTRIBUTE(DMAttr::DUILimitEditAttr::STRING_firstincludechars,m_strFirstIncludeChars,DM_ECODE_NOXMLLOADREFRESH)
		DM_WSTRING_ATTRIBUTE(DMAttr::DUILimitEditAttr::STRING_firstexcludechars,m_strFirstExcludeChars,DM_ECODE_NOXMLLOADREFRESH)
	DM_END_ATTRIBUTES()

public: 
	CStringW             m_strIncludeChars;				///< �ַ������ڴ����ַ�����
	CStringW             m_strExcludeChars;				///< �ų������ַ���
	CStringW             m_strFirstIncludeChars;		///< ���ַ������ڴ����ַ�����
	CStringW             m_strFirstExcludeChars;		///< ���ַ������ų������ַ�
	bool                 m_bNoCase;						///< ���Ӵ�С
};