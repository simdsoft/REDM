#include "QQDemoAfx.h"
#include "TrayIconWndMgr.h"

BEGIN_MSG_MAP(CTrayIconWndMgr)
	MESSAGE_HANDLER_EX(m_CbMsg, OnTrayIcon)
	MESSAGE_HANDLER_EX(m_RestartMsg, OnTaskbarRestart)
END_MSG_MAP()
CTrayIconWndMgr::CTrayIconWndMgr()
{
	m_CbMsg		   = ::RegisterWindowMessage(_T("TaskbarNotifyMsg"));// 接收回调消息
	m_RestartMsg   = ::RegisterWindowMessage(_T("TaskbarCreated"));  // 当任务栏崩溃时会接收到此消息

	m_hParentWnd   = NULL;
	m_bInstall	   = false;
	m_bExitBalloon = true;
	m_dwOldTime    = 0;

	memset(&m_Nid, 0, sizeof(NOTIFYICONDATA));
	m_Nid.cbSize   = sizeof(NOTIFYICONDATA);
}

CTrayIconWndMgr::~CTrayIconWndMgr()
{
	UnInstallTrayIcon();
}

bool CTrayIconWndMgr::InstallTrayIcon(LPCWSTR lpszToolTip, HWND hParentWnd, HICON hIcon, UINT nID)
{
	do 
	{
		if (NULL == hParentWnd || !::IsWindow(hParentWnd))
		{
			break;
		}
		m_hParentWnd = hParentWnd;

		ATOM Atom = g_pDMApp->GetClassAtom();
		DMCWnd::CreateWindowEx((LPCWSTR)Atom,NULL,WS_OVERLAPPEDWINDOW,0,0,0,1,1,NULL,0);
		if (NULL == m_hWnd)
		{
			break;
		}

		m_Nid.hWnd	= m_hWnd;
		m_Nid.uID   = nID;
		m_Nid.hIcon = hIcon;
		m_Nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		m_Nid.uCallbackMessage = m_CbMsg;

		if (NULL != lpszToolTip)
		{
			wcscpy_s(m_Nid.szTip,countof(m_Nid.szTip),lpszToolTip);
		}

		m_bInstall = ::Shell_NotifyIconW(NIM_ADD, &m_Nid)? true : false;
		ShowWindow(SW_HIDE);
	} while (false);
	return m_bInstall;
}

bool CTrayIconWndMgr::UnInstallTrayIcon()
{
	if (false == m_bInstall)
	{
		return false;
	}

	m_Nid.uFlags = 0;
	m_bExitBalloon = true;
	return ::Shell_NotifyIconW(NIM_DELETE, &m_Nid)? true : false;
}

bool CTrayIconWndMgr::SetTipText(LPCWSTR lpszTipText)
{
	if (NULL == lpszTipText)
	{
		return false;
	}

	m_Nid.uFlags = NIF_TIP;
	wcscpy_s(m_Nid.szTip,countof(m_Nid.szTip),lpszTipText);
	return ::Shell_NotifyIconW(NIM_MODIFY, &m_Nid)? true : false;
}

bool CTrayIconWndMgr::SetBalloonDetails(LPCWSTR lpszBalloonText, LPCWSTR lpszBalloonCaption, UINT nTimeout,
										DWORD style, HICON hUserIcon, bool bNoSound)
{
	bool bRet = false;
	do 
	{
		HideBalloon();

		m_Nid.uFlags	 |= NIF_INFO;
		m_Nid.dwInfoFlags = style;
		m_Nid.uTimeout	  = nTimeout;

		if (bNoSound)
		{
			m_Nid.dwInfoFlags |= NIIF_NOSOUND;
		}
		else
		{
			m_Nid.dwInfoFlags &= ~NIIF_NOSOUND;
		}

		memset(m_Nid.szInfoTitle,0,countof(m_Nid.szInfoTitle));
		memset(m_Nid.szInfo,0,countof(m_Nid.szInfo));
		if (lpszBalloonCaption)
		{
			wcscpy_s(m_Nid.szInfoTitle,countof(m_Nid.szInfoTitle),lpszBalloonCaption);
		}
		if (lpszBalloonText)
		{
			wcscpy_s(m_Nid.szInfo,countof(m_Nid.szInfo),lpszBalloonText);
		}

		bRet = ::Shell_NotifyIconW(NIM_MODIFY, &m_Nid)? true : false;
		if (bRet)
		{
			m_dwOldTime = GetTickCount();
			m_bExitBalloon = false;
			::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)StartTimeCheck,this,0,NULL);
		}

	} while (false);
	return bRet;
}

bool CTrayIconWndMgr::HideBalloon()
{
	bool bRet = false;
	do 
	{
		m_bExitBalloon = true;
		if (NULL == m_Nid.hWnd || !::IsWindow(m_Nid.hWnd))
		{
			break;
		}
		m_Nid.uFlags = NIF_INFO;
		memset(m_Nid.szInfoTitle,0,countof(m_Nid.szInfoTitle));
		memset(m_Nid.szInfo,0,countof(m_Nid.szInfo));

		bRet = ::Shell_NotifyIconW(NIM_MODIFY, &m_Nid)? true : false;
	} while (false);
	return bRet;
}

LRESULT CTrayIconWndMgr::OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT Msg = (UINT)LOWORD(lParam);
	switch (Msg)
	{
	case WM_RBUTTONUP:
		{
			::PostMessage(m_hParentWnd, WM_SHOWTRAYMENU, 0 ,NULL);
		}
		break;

	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONUP:
		{
			if (NULL != m_hParentWnd && ::IsWindow(m_hParentWnd))
			{
				// 这里可以发个消息让窗口置顶
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CTrayIconWndMgr::OnTaskbarRestart(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_bInstall && m_Nid.hWnd && ::IsWindow(m_Nid.hWnd))
	{
		m_Nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		return ::Shell_NotifyIcon(NIM_ADD, &m_Nid);
	}
	return -3;
}


// static 
DWORD CTrayIconWndMgr::StartTimeCheck(LPVOID lp)
{
	CTrayIconWndMgr* pThis = (CTrayIconWndMgr*)lp;
	DWORD dwOldTime = pThis->m_dwOldTime;
	DWORD dwTimeOut = pThis->m_Nid.uTimeout;
	while (!pThis->m_bExitBalloon)
	{
		DWORD dwCurTime = GetTickCount();
		if (dwCurTime - dwOldTime >= dwTimeOut)
		{
			pThis->HideBalloon();
			return TRUE;
		}
		Sleep(10);
	}
	return TRUE;
}