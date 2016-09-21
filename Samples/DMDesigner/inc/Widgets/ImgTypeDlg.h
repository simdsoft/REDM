// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ImgTypeDlg.h  
// File mark:   
// File summary:增加、编辑主题包下的img类型
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once

class ImgTypeDlg : public DMHDialog
{
public:
	ImgTypeDlg(bool bEditMode = false,CStringW strName = L"");
	DECLARE_MESSAGE_MAP()
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);

protected:
	DMCode OnOK();

private:
	bool                            m_bEditMode;
	CStringW                        m_strName;
};