//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIRealWnd.h 
// File Des: ÕæÊµ´°¿Ú
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-13	1.0			
//-------------------------------------------------------
#pragma once
#include "IDUIRealWnd.h"

class DUIRealWnd:public IDUIRealWnd
{
	DMDECLARE_CLASS_NAME(DUIWindow,L"realwnd",DMREG_Window);

public:
	 HWND OnCreateRealWnd(LPCWSTR atom,HWND hWndParent);
	 DMCode OnDestoryRealWnd();
	 DMCode OnShowRealWnd(BOOL bShow, UINT nStatus){return DM_ECODE_NOTIMPL;};
	 DMCode OnMoveRealWnd(LPRECT lpRect);
	 DMCode OnIsPopupWnd(){return DM_ECODE_FAIL;}

public:
	HWND  m_hWnd;

};
