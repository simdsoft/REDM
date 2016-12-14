//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: WidgetWnd.h 
// File Des: 测试Widget的主窗口
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-20	1.0		
//-------------------------------------------------------
#pragma once
#include "DUIWebkit.h"
#include "AttributeTestWnd.h"
#include "DMScriptHelper.h" 
class CWidgetWnd:public DMHWnd, public IDMWebEvent
{
public: 
	CWidgetWnd();
	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);// 菜单测试

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnMaximize();
	DMCode OnRestore();
	DMCode OnMinimize();
	DMCode OnBtnMenu();
	DMCode OnAttrTest();
	DMCode OnEditChange(DMEventArgs *pEvt);
	DMCode ListCtrlExHeaderClick(DMEventArgs* pEvt);

	// Webkit测试
	DMCode OnBtnWebBack();
	DMCode OnBtnWebRefresh(); 
	DMCode OnBtnWebfront();
	DMCode RefreshWeb(DMEventArgs* pEvent);
	static void OnTitleChanged(const DMClientHandler*, DMString title);
	static void OnURLChanged(const DMClientHandler*, DMString url);

	// IE测试
	DMCode NavigateComplete2(DUIWND hWnd,DMIN IDispatch *pDisp,DMIN wchar_t *pUrl);

	DMCode OnFireEvent(DMEventArgs &Evt);
	void OnDUITimer(char id);

public:
	DUIWebKit*					    m_pWebkit;
	DMClientHandler					m_handler;// 必须是全局的，不然会崩
	IDUIWeb*						m_pIE;
	DMSmartPtrT<CAttributeTestWnd>	m_pAttrTestWnd;
	DMScriptHelper                  m_ScriptHelper; 
};