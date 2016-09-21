#include "DMDemoAfx.h"
#include "IEWnd.h"
#include "MainWnd.h"
extern CMainWnd* g_pMainWnd;

HWND IERealWnd::OnCreateRealWnd(LPCWSTR atom,HWND hWndParent)
{
	HWND hWnd = NULL;
	do 
	{
		m_pIEWnd.Attach(new CIEWnd);
		if (NULL == m_pIEWnd->DM_CreateWindow(L"dui_ie",0,0,0,0,hWndParent))
		{
			break;
		}
		m_hParent = hWndParent;
		hWnd      = m_pIEWnd->m_hWnd;
		if (0xff!=g_pMainWnd->m_pHWndXmlInfo->m_byAlpha)
		{
			CStringW strValue;
			strValue.Format(L"%d\n",g_pMainWnd->m_pHWndXmlInfo->m_byAlpha);
			m_pIEWnd->SetAttribute(L"alpha",strValue);
		}
	} while (false);
	return hWnd;
}

DMCode IERealWnd::OnDestoryRealWnd()
{
	if (m_pIEWnd)
	{
		if (m_pIEWnd->IsWindow())
		{
			m_pIEWnd->DestroyWindow();
		}
	}
	return DM_ECODE_OK;
}

DMCode IERealWnd::OnMoveRealWnd(LPRECT lpRect)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		CRect rcPos = lpRect;
		if (rcPos.IsRectEmpty())
		{
			break;
		}
		if (NULL == m_pIEWnd||!m_pIEWnd->IsWindow()||!m_pIEWnd->IsWindowVisible())
		{
			break;
		}
		::ClientToScreen(m_hParent, (LPPOINT)&rcPos);
		::ClientToScreen(m_hParent, ((LPPOINT)&rcPos)+1);
		m_pIEWnd->SetWindowPos(NULL,rcPos.left,rcPos.top,rcPos.Width(),rcPos.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		iErr = DM_ECODE_OK;

	} while (false);
	return iErr;
}
