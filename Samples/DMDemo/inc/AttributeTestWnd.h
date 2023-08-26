﻿//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: AttributeTestWnd.h 
// File Des:  对指定窗口SetAttribute测试属性值
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-16	1.0		
//-------------------------------------------------------
#pragma once

class CMainWnd;
class CAttributeTestWnd:public DMHWnd
{
public:
	CAttributeTestWnd(CMainWnd* pMain);
	~CAttributeTestWnd();

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	// Input Parameter:
	// Output Parameter:
	// return:
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnSetAttribute();

protected:
	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	BEGIN_EVENT_MAPT(CAttributeTestWnd)
		EVENT_NAME_COMMAND("btn_attrtestclose",OnClose)
		EVENT_NAME_COMMAND("btn_testattr",OnSetAttribute)
	END_EVENT_MAP()

public:
	CMainWnd*							  m_pMainWnd;
	DUIEdit*                              m_pNameEdit;
	DUIEdit*                              m_pAttrNameEdit;
	DUIEdit*                              m_pAttrValueEdit;
	DUIButton*                            m_pBtn;
	DUIRichEdit*                          m_pInfoEdit;
};

