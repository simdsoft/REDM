// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	PoolDlg.h 
// File mark:   
// File summary:Ôö¼Ó¡¢±à¼­skin.style³Ø
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once

class PoolDlg : public DMHDialog
{
public:
	PoolDlg(bool bSkinPool = true,bool bEditMode = false,CStringW strName = L"");
	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);

protected:
	DMCode OnOK();

private:
	bool                            m_bEditMode;
	bool                            m_bSkinPool;
	CStringW                        m_strName;
};