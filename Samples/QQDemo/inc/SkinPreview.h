//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: SkinPreview.h 
// File Des: 预览窗口
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-14	1.0		
//-------------------------------------------------------
#pragma once
#include "DMResFolderImpl.h"

class SkinPreview: public DUIWindow
{
	DMDECLARE_CLASS_NAME(SkinPreview,L"SkinPreview",DMREG_Window);
public:
	SkinPreview();
public:
	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
	DM_END_MSG_MAP()
public:
	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
	void DM_OnPaint(IDMCanvas* pCanvas);
	void OnLButtonDown(UINT nFlags,CPoint pt);
	void OnLButtonUp(UINT nFlags,CPoint pt);
public:
	bool DV_IsStateChangeRedraw(){return true;}

public:
	DMSmartPtrT<IDMSkin>                         m_pSkin;
	CStringW                                     m_strThemeName;
	bool                                         m_bDown;

};