// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ThemeDlg.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-18
// ----------------------------------------------------------------
#pragma once

class ThemeDlg : public DMHDialog
{
public: 
	ThemeDlg(bool bEditMode = false,CStringW strThemeName = L"",CStringW strDir = L"");
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP() 

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);

protected:
	DMCode OnOK();

private:
	bool							m_bEditMode;
	CStringW                        m_strThemeName;
	CStringW                        m_strDir;
};

