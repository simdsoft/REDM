// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	MsgBox.h
// File mark:   
// File summary:专为弹框准备的
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-13
// ----------------------------------------------------------------
#pragma once

class MsgBox : public DMHDialog
{
public:
	MsgBox(LPCWSTR lpszXmlId = L"ds_msgbox");
	int MessageBox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType); 

private:
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP()
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);
	DMCode OnBtnClick(int uID);
private:
	CStringW                        m_strXmlId;
	CStringW                        m_strText;
	CStringW                        m_strCaption;
	UINT                            m_uType;
};