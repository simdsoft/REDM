// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	AddXml.h
// File mark:    
// File summary:Add模式
// Author:		guoyouhuang
// Edition:     1.0 
// Create date: 2016-9-6
// ----------------------------------------------------------------
#pragma once
#include "AddExpandWnd.h"

class AddXml : public AttrBase
{
public:
	AddXml();
	~AddXml();
	DMCode UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel);

	DMCode InitAddXml();
	DMCode UnInitAddXml();

	DMCode OnExpand();

	void OnLButtonDown(UINT nFlags, CPoint point);

private:
	CRect GetExpandRect();

public:
	static DMCode s_DMHandleEvent(DMEventArgs *pEvt);
	static BOOL s_ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP()

public:
	DUITabCtrl*										  m_pTools;
	DMSmartPtrT<AddExpandWnd>						  m_pExpandWnd;

	// 用于转发消息
	static AddXml*                                    ms_pthis;
};