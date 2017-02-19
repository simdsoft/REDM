//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: MainWnd.h 
// File Des: 主窗口
// File Summary: 
// Cur Version: 1.0
// Author: 
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-7	1.0		
//-------------------------------------------------------
#pragma once
#include "WndShadow.h"
#include "SkinWnd.h"
#include "SpyWnd.h" 
#include "WidgetWnd.h"

class CQQMainWnd:public DMHWnd
{
public:
	CQQMainWnd();
	~CQQMainWnd();
	DECLARE_MESSAGE_MAP()						// 仿MFC消息映射宏，也可以使用BEGIN_MSG_MAPT宏使消息处理在头文件
	DECLARE_EVENT_MAP()							// 事件分发映射宏,也可以使用BEGIN_EVENT_MAPT宏使事件处理在头文件

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	int OnCreate(LPVOID);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);	
	void OnShowWindow(BOOL bShow, UINT nStatus);
	LRESULT OnShowTrayMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);// 菜单测试

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnMinimize();
	DMCode OnNameEditBtn();
	DMCode OnSkin();
	DMCode OnSpy();
	DMCode OnWidget();

	//
	DMCode OnNameEditKillFocus(DMEventArgs* pEvent);
	DMCode OnSubListTree(DMEventArgs* pEvent);
	DMCode OnSubListEx(DMEventArgs* pEvent);

public:
	CWndShadow								m_WndShadow;
	DUIListBoxEx*                           m_pListEx;
	DUITreeCtrl*                            m_pSubListTree[3];
	DUIListBoxEx*                           m_pSubListEx[3];

public:
	DMSmartPtrT<CSkinWnd>					m_pSkinWnd;
	DMSmartPtrT<CSpyWnd>                    m_pSpyWnd;
	DMSmartPtrT<CWidgetWnd>                 m_pWidgetWnd;
};