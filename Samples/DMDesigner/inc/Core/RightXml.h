// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	RightXml.h
// File mark:   
// File summary:管理右边属性视图
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once

class RightXml : public AttrBase
{
public:
	RightXml();
	virtual~RightXml();
	DMCode UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel);

	DMCode InitRightXml();
	DMCode UnInitRightXml();
	DMCode UpdataInfo(HDMTREEITEM hSel);
	DMCode OnEyeChanged(DMEventArgs* pEvt);
	DMCode UpdateEye(HDMTREEITEM hItem);// eye不放在AttrBase中，因为AttrBase被多个类继承,可能一个event被调用多次

public:
	static DMCode s_DMHandleEvent(DMEventArgs *pEvt);
	static BOOL s_ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP()

public:
	NameAttr*                                           m_pNameAttr;
	HostAttr*                                           m_pHostAttr;
	WidgetAttr*                                         m_pWidgetAttr;
	PosAttr*                                            m_pPosAttr;

	DUIList*                                            m_pList;

	// 用于转发消息
	static RightXml*                                    ms_pthis;
};