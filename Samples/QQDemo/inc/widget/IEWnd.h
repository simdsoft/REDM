//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IEWnd.h 
// File Des: IEÖ÷´°¿Ú
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-25	1.0		
//-------------------------------------------------------
#pragma once
#include "IDUIRealWnd.h"

class CIEWnd:public DMHWnd 
{
	DECLARE_MESSAGE_MAP()	
public:
	void OnActivate(UINT nState, BOOL bMinimized, HWND wndOther);   
};

class IERealWnd:public IDUIRealWnd
{
	DMDECLARE_CLASS_NAME(DUIWindow,L"ierealwnd",DMREG_Window);
public:
	HWND OnCreateRealWnd(LPCWSTR atom,HWND hWndParent);
	DMCode OnDestoryRealWnd();
	DMCode OnShowRealWnd(BOOL bShow, UINT nStatus){return DM_ECODE_NOTIMPL;};
	DMCode OnMoveRealWnd(LPRECT lpRect);
	DMCode OnIsPopupWnd(){return DM_ECODE_OK;}
public:
	DMSmartPtrT<CIEWnd>			m_pIEWnd;
	HWND						m_hParent;

};