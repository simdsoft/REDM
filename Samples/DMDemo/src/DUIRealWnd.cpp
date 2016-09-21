#include "DMDemoAfx.h"
#include "DUIRealWnd.h"

HWND  DUIRealWnd::OnCreateRealWnd(LPCWSTR atom,HWND hWndParent)
{
	m_hWnd = ::CreateWindowExW(0, atom, 
		L"DuiDemo",WS_CHILD,0, 0, 211, 10,hWndParent,NULL, GetModuleHandle(NULL),NULL);

	::SetWindowTextW(m_hWnd,L"realwnd");
	return m_hWnd;
}

DMCode DUIRealWnd::OnDestoryRealWnd()
{
	if (m_hWnd&&::IsWindow(m_hWnd))
	{
		::PostMessage(m_hWnd, WM_DESTROY, 0, 0);
	}

	return DM_ECODE_OK;
}

DMCode DUIRealWnd::OnMoveRealWnd(LPRECT lpRect)
{

	return DM_ECODE_FAIL;
}