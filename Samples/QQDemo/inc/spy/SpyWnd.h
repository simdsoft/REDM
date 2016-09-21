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
//      guoyou		2015-2-2	1.0		
//-------------------------------------------------------
#pragma once
#include "SpyFindBtn.h"

class CSpyWnd:public DMHWnd
{
public:
	CSpyWnd();	
	~CSpyWnd();
	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnDUITimer(char id);

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnMinimize();
	DMCode OnSpyInitEvent(DMEventArgs *pEvt);
	DMCode OnTreeSelChanged(DMEventArgs *pEvt);


public:// 辅助
	void InitTreeCtrl(HWND hRootWnd);
	void InsertTreeItem(DMXmlNode &XmlNode,HDMTREEITEM hParentItem);
	void ReflashEditInfo();
	bool HitTest(RECT &rc);

public:
	DUITreeCtrl*            m_pTreeCtrl;
	DUIRichEdit*            m_pTreeEdit;
	DUIStatic*              m_pSearchSta;
	PDMSpyEnum              m_pSpyEnum;
	HWND                    m_hRootWnd;
	bool                    m_bInit;

	// 绘制
	bool                    m_bHittest;
	bool                    m_bDraw;
	CRect                   m_rcDraw;

};