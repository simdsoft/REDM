//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: SkinWnd.h 
// File Des:  换肤子窗口测试
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-16	1.0		
//-------------------------------------------------------
#pragma once
#include "WndShadow.h"
class CQQMainWnd;
class CSkinWnd:public DMHWnd
{
public:
	CSkinWnd(CQQMainWnd*pOwner);

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	int OnCreate(LPVOID);
	void OnSize(UINT nType, CSize size);
	void OnShowWindow(BOOL bShow, UINT nStatus);
	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnOutSkin();
protected:
	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	DECLARE_EVENT_MAP()

public:
	CWndShadow								m_WndShadow;
	CQQMainWnd*								m_pMainWnd;
	wchar_t									m_szCurThemes[100];
};
