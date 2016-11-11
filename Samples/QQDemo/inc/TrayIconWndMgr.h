// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	TrayIconWndMgr.h
// File mark:   针对托盘菜单的一个封装
// File summary:
// Author:		lzlong
// Edition:     1.0
// Create date: 2016-11-11
// ----------------------------------------------------------------
#pragma once
#include <shellapi.h>

#define  WM_SHOWTRAYMENU     WM_USER + 1244
/// <summary>
///		简单的托盘实现代码
/// </summary>
class CTrayIconWndMgr : public DMCWnd, public DMSingletonT<CTrayIconWndMgr>
{
public:
	CTrayIconWndMgr();
	~CTrayIconWndMgr();

	/// -------------------------------------------------
	/// @brief			安装托盘图标
	/// @param[in]		lpszToolTip       托盘的提示
	/// @param[in]		hParentWnd		  托盘的父窗口
	/// @param[in]		hIcon	          将要添加/修改/删除的图标句柄
	/// @param[in]		nID				  应用程序定义的任务栏图标的ID			
	/// @return			true or false
	bool InstallTrayIcon(LPCWSTR lpszToolTip, HWND hParentWnd, HICON hIcon, UINT nID);

	/// -------------------------------------------------
	/// @brief			移除托盘图标
	bool UnInstallTrayIcon();

	/// -------------------------------------------------
	/// @brief			设置文本提示,设置、隐藏汽泡提示
	bool SetTipText(LPCWSTR lpszTipText);
	bool SetBalloonDetails(LPCWSTR lpszBalloonText, LPCWSTR lpszBalloonCaption = NULL,UINT nTimeout = 1000, 
							DWORD style = NIIF_INFO, HICON hUserIcon = NULL, bool bNoSound = false);
	bool HideBalloon();

protected:
	LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnTaskbarRestart(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/);

public:
	DECLARE_MESSAGE_MAP()	
	static DWORD StartTimeCheck(LPVOID lp);

public:
	NOTIFYICONDATA					  m_Nid;
	UINT							  m_CbMsg;
	UINT                              m_RestartMsg;

	HWND                              m_hParentWnd;
	bool                              m_bInstall;
	bool                              m_bExitBalloon;

	DWORD                             m_dwOldTime;

};