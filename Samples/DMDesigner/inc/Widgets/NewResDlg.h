// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	NewResDlg.h
// File mark:   
// File summary:新建资源窗口
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-6-22
// ----------------------------------------------------------------
#pragma once

class DUIRecentListBox : public DUIListBox
{
	DMDECLARE_CLASS_NAME(RecentListBox, L"recentlist",DMREG_Window)
public: 
	DUIRecentListBox();
	DM_BEGIN_MSG_MAP()
		MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
	DM_END_MSG_MAP()
	void OnLButtonDbClick(UINT nFlags,CPoint pt);
};

class NewResDlg : public DMHDialog
{
public:
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP()
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	DMCode OnOpenResPath();
	DMCode OnOpenRecentDir(DMEventArgs *pEvt);
	
protected:
	DMCode OnOK();
	DMCode CreateResDir();

public:
	DUIEdit*							      m_pResNameEdit;
	DUIEdit*                                  m_pResPathEdit;
	DUIListBox*                               m_pRecentList;

	CStringW								  m_strResName;					///< 资源文件夹名（如QQRes）
	CStringW								  m_strResPath;					///< 资源文件夹所在目录(如E:\\)
	CStringW								  m_strResDir;                  ///< 资源文件夹全路径(m_strResPath+m_strResName，如E:\\QQRes)
};