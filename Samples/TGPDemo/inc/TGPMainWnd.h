// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved. 
// 
// File name:	TGPMainWnd.h
// File mark:   
// File summary: 

// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-4-21
// ----------------------------------------------------------------
#pragma once  
 
class CTGPMainWnd:public DMHWnd
{ 
public:
	DECLARE_MESSAGE_MAP()						// 仿MFC消息映射宏，也可以使用BEGIN_MSG_MAPT宏使消息处理在头文件
	DECLARE_EVENT_MAP()							// 事件分发映射宏,也可以使用BEGIN_EVENT_MAPT宏使事件处理在头文件

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	LRESULT OnHoldMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);	

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnMinimize();
	DMCode OnMenu();
	DMCode OnListBoxEx(DMEventArgs *pEvt);

public:
	 TGPMenu		m_CfMenu;
};