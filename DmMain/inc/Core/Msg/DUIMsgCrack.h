//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIMsgCrack.h 
// File Des: DUI是模拟窗口，此用来模似分发消息到DUI子窗口，消息最开始来源于真实窗口
// File Summary: 
// Cur Version: 1.1
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-2	1.0		
//-------------------------------------------------------

#pragma once

namespace DM
{
#define DM_BEGIN_MSG_MAP() \
public: \
	virtual BOOL ProcessDuiMessage( \
	UINT uMsg, WPARAM wParam, \
	LPARAM lParam, LRESULT& lResult) \
	{ 

#define DM_END_MSG_MAP() \
	if (!IsMsgHandled()) \
	{ \
		return __super::ProcessDuiMessage(uMsg, wParam, lParam, lResult); \
	} \
	return TRUE; \
	}

#define DM_END_MSG_INBASE() \
	return DV_WndProc(uMsg,wParam,lParam,lResult); \
}

//-----------------------------------------------------
// BOOL OnEraseBkgnd(IDMCanvas* pCanvas)
#define DM_MSG_WM_ERASEBKGND(func) \
	if (uMsg == WM_ERASEBKGND) \
	{ \
		SetMsgHandled(TRUE); \
		lResult = (LRESULT)func((IDMCanvas *)wParam); \
		if(IsMsgHandled()) \
		return TRUE; \
	}

//-----------------------------------------------------
// void OnPaint(IDMCanvas * pCanvas)
#define DM_MSG_WM_PAINT(func) \
	if (uMsg == WM_PAINT) \
	{ \
		SetMsgHandled(TRUE); \
		func((IDMCanvas *)wParam); \
		lResult = 0; \
		if (IsMsgHandled()) \
			return TRUE; \
	}

//-----------------------------------------------------
// void OnNcPaint(IDMCanvas * pCanvas)
#define DM_MSG_WM_NCPAINT(func) \
	if (uMsg == WM_NCPAINT) \
	{ \
		SetMsgHandled(TRUE); \
		func((IDMCanvas *)wParam); \
		lResult = 0; \
		if (IsMsgHandled()) \
			return TRUE; \
	}

//-----------------------------------------------------
// void OnSetFont(IDMFont *pFont, BOOL bRedraw)
#define DM_MSG_WM_SETFONT(func) \
	if (uMsg == WM_SETFONT) \
	{ \
		SetMsgHandled(TRUE); \
		func((IDMFont*)wParam, (BOOL)LOWORD(lParam)); \
		lResult = 0; \
		if (IsMsgHandled()) \
			return TRUE; \
	}

//-----------------------------------------------------
#define DM_MSG_WM_WINPOSCHANGED(func) \
	if (uMsg == WM_WINDOWPOSCHANGED) \
	{ \
		SetMsgHandled(TRUE); \
		lResult=func((LPRECT)lParam); \
		if (IsMsgHandled()) \
			return TRUE; \
	}

//-----------------------------------------------------
// void OnSetFocus()
#define DM_MSG_WM_SETFOCUS(func) \
	if (uMsg == WM_SETFOCUS) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if (IsMsgHandled()) \
			return TRUE; \
	}

//-----------------------------------------------------
// void OnKillFocus()
#define DM_MSG_WM_KILLFOCUS(func) \
	if (uMsg == WM_KILLFOCUS) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if (IsMsgHandled()) \
			return TRUE; \
	}

//-----------------------------------------------------
// void OnEnable(BOOL bEnable,UINT uStatus)
#define DM_MSG_WM_ENABLE(func) \
	if (uMsg == WM_ENABLE) \
	{ \
		SetMsgHandled(TRUE); \
		func((BOOL)wParam,(UINT)lParam); \
		lResult = 0; \
		if (IsMsgHandled()) \
			return TRUE; \
	}

//-----------------------------------------------------
// void OnNcMouseHover(int nFlag,CPoint pt)
#define DM_MSG_WM_NCMOUSEHOVER(func) \
	if(uMsg==WM_NCMOUSEHOVER)\
	{ \
		SetMsgHandled(TRUE); \
		func(wParam,CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam))); \
		lResult = 0; \
		if (IsMsgHandled()) \
			return TRUE; \
	}

//-----------------------------------------------------
// void OnNcMouseLeave()
#define  DM_MSG_WM_NCMOUSELEAVE(func) \
	if(uMsg==WM_NCMOUSELEAVE) \
	{ \
		SetMsgHandled(TRUE); \
		func(); \
		lResult = 0; \
		if (IsMsgHandled()) \
			return TRUE; \
	}
	
//-----------------------------------------------------
// void OnTimer(char cTimerID)
#define DM_MSG_WM_TIMER(func) \
	if (uMsg == WM_TIMER) \
	{ \
		SetMsgHandled(TRUE); \
		func((char)wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
		return TRUE; \
	}

#define WM_DUITIMER    (WM_USER+1314)    //定义DUI定时器
#define DM_MSG_WM_THREADTIMER(func) \
	if (uMsg == WM_DUITIMER) \
	{ \
		SetMsgHandled(TRUE); \
		func(wParam); \
		lResult = 0; \
		if(IsMsgHandled()) \
			return TRUE; \
	}

}//namespace DM