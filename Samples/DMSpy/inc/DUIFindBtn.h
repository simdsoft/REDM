//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIFindBtn.h 
// File Des: 搜索窗口
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-28	1.0			
//--------------------------------------------------------
#pragma once

#include "DUIButton.h"
extern DUIWindow* g_pMainWnd;

/// <summary>
///		在窗口查找结束时发给自己的主窗口
/// </summary>
class DMSpyInitArgs:public DMEventArgs
{
public:
	DMSpyInitArgs(DUIWindow *pWnd):DMEventArgs(pWnd){}
	enum{EventID = DMEVT_END+1};
	UINT GetEventID(){return EventID;}
public:
	HWND               m_hwnd;
};

/// <summary>
///		查找按钮
/// </summary>
class DUIFindBtn:public DUIWindow,public IDMTimeline
{
	DMDECLARE_CLASS_NAME(DUIWindow,L"findbtn",DMREG_Window);
public:
	DUIFindBtn();
	~DUIFindBtn();
public:
	DM_BEGIN_MSG_MAP()
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_DESTROY(OnDestroy)
	DM_END_MSG_MAP()// 基类使用DM_END_MSG_INBASE，其余子类使用DM_END_MSG_MAP

public:
	void OnLButtonDown(UINT nFlags,CPoint pt);
	void OnLButtonUp(UINT nFlags,CPoint pt);
	void OnMouseLeave();
	void OnDestroy();

public:
	DMCode DV_OnSetCursor(const CPoint &pt);					    ///< 设置当前的光标
	DMCode OnTimeline();


public:// 辅助
	void RefreshFindInfo(HWND hWnd);
	CStringT Display(int nVal);
	CStringT GetProcPath(int PID);
	HWND InitCurHWnd();

protected:
	bool                       bLButtonDown;
};