// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMTrayIconImpl.h
// File mark:   ������̲˵���һ����װ
// File summary:
/* �÷�˵����

1.�����ڱ���̳���DMTrayIconImpl�� ���磺
class DMMainWnd : public DMTrayIconImpl<DMMainWnd>
{
};
2.�������ڵ�BEGIN_MSG_MAP �� END_MSG_MAP֮������:CHAIN_MSG_MAP(DMTrayIconImpl)  ���磺
	BEGIN_MSG_MAP(DMMainWnd)
 		CHAIN_MSG_MAP(DMTrayIconImpl)
	END_MSG_MAP()

 */
// Author:		lzlong
// Edition:     1.0
// Create date: 2019-4-12
// ----------------------------------------------------------------

#pragma once
#include <shellapi.h>

namespace DM
{
#define TRAYBEGIN_MSG_MAP(theClass) \
	/* "handled" management for cracked handlers */ \
	BOOL IsMsgHandled() \
	{ \
	T* pT = static_cast<T*>(this);\
	return pT->m_bMsgHandled; \
	} \
	void SetMsgHandled(BOOL bHandled) \
	{ \
	T* pT = static_cast<T*>(this);\
	pT->m_bMsgHandled = bHandled; \
	} \
	BOOL  ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID/* = 0*/) \
	{ \
	T* pT = static_cast<T*>(this);\
	BOOL bOldMsgHandled = pT->m_bMsgHandled; \
	BOOL bRet = _ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
	pT->m_bMsgHandled = bOldMsgHandled; \
	return bRet; \
	} \
	BOOL _ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID) \
	{ \
	BOOL bHandled = TRUE; \
	hWnd; \
	uMsg; \
	wParam; \
	lParam; \
	lResult; \
	bHandled; \
	switch(dwMsgMapID) \
	{ \
	case 0:

	//-------------------------------------------------------
	// ����
#define TRAYEND_MSG_MAP() \
	break; \
		default: \
			DMASSERT_EXPR(0, L"msgMapID����,��Ϣδ������");\
			break; \
		} \
		return FALSE; \
	}

	//-------------------------------------------------------
	// ��Ϣӳ��
#define TRAYMESSAGE_HANDLER_EX(msg, func) \
	if(uMsg == msg) \
			{ \
		SetMsgHandled(TRUE); \
		lResult = func(uMsg, wParam, lParam); \
		if(IsMsgHandled()) \
		return TRUE; \
			}

	/// <summary>
	///		Wrapper class for the Win32 NOTIFYICONDATA structure
	/// </summary>
	class CNotifyIconData : public NOTIFYICONDATA
	{
	public:
		CNotifyIconData()
		{
			memset(this, 0, sizeof(NOTIFYICONDATA));
			cbSize = sizeof(NOTIFYICONDATA);
		}
	};

	/// <summary>
	///		����ʵ�ִ���
	/// </summary>
	template <class T>
	class DMTrayIconImpl
	{
	private:
		UINT				WM_TRAYICON;
		UINT				WM_TASKBARCREATED;
		CNotifyIconData		m_nid;
		bool				m_bInstalled;
		// ģ��DMCWnd
		//BOOL				m_bMsgHandled; //�����ٴ�����  ���׸������ڵ�m_bMsgHandled��ɳ�ͻ
	public:
		DMTrayIconImpl() : m_bInstalled(false)
		{
			WM_TRAYICON = ::RegisterWindowMessage(_T("WM_TRAYICON"));		// ���ջص���Ϣ
			WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));	// ������������ʱ����յ�����Ϣ
		}

		~DMTrayIconImpl()
		{
			// Remove the icon
			RemoveIcon();
		}

		//---------------------------------------------------
		// Function Des:Install a taskbar icon
		// 	lpszToolTip 	- The tooltip to display
		//	hIcon 		- The icon to display
		// 	nID		- The resource ID of the context menu
		/// returns true on success
		bool InstallIcon(LPCTSTR lpszToolTip, HICON hIcon, UINT nID)
		{
			T* pT = static_cast<T*>(this);
			// Fill in the data		
			m_nid.hWnd = pT->m_hWnd;
			m_nid.uID = nID;
			m_nid.hIcon = hIcon;
			m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
			m_nid.uCallbackMessage = WM_TRAYICON;
			_tcscpy_s(m_nid.szTip, countof(m_nid.szTip), lpszToolTip ? lpszToolTip : _T(""));
			// Install
			m_bInstalled = ::Shell_NotifyIcon(NIM_ADD, &m_nid) ? true : false;
			return m_bInstalled;
		}

		//---------------------------------------------------
		// Function Des:Remove taskbar icon
		// returns true on success
		BOOL RemoveIcon()
		{
			if (!m_bInstalled)
				return false;
			// Remove
			m_nid.uFlags = 0;
			m_bInstalled = false;
			return Shell_NotifyIcon(NIM_DELETE, &m_nid) ? true : false;
		}

		//---------------------------------------------------
		// Function Des:��ʾ������ʽ��ʾ��
		BOOL ShowTrayBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle, DWORD dwInfoFlags = NIIF_INFO, UINT uTimeout = 5000)
		{
			m_nid.cbSize = sizeof(NOTIFYICONDATA);
			m_nid.uFlags = NIF_INFO;
			m_nid.uTimeout = uTimeout;
			m_nid.dwInfoFlags = dwInfoFlags;
			_tcscpy_s(m_nid.szInfo, countof(m_nid.szTip), szMsg ? szMsg : _T(""));
			_tcscpy_s(m_nid.szInfoTitle, countof(m_nid.szInfoTitle), szTitle ? szTitle : _T(""));

			return 0 != Shell_NotifyIcon(NIM_MODIFY, &m_nid);
		}

		//---------------------------------------------------
		// Function Des:Set the icon tooltip text
		// returns true on success
		BOOL SetTrayTooltipText(LPCTSTR pszTooltipText)
		{
			if (pszTooltipText == NULL)
				return FALSE;
			// Fill the structure
			m_nid.uFlags = NIF_TIP;
			_tcscpy_s(m_nid.szTip, countof(m_nid.szTip), pszTooltipText);
			return Shell_NotifyIcon(NIM_MODIFY, &m_nid) ? true : false;
		}

		TRAYBEGIN_MSG_MAP(DMTrayIconImpl)
			TRAYMESSAGE_HANDLER_EX(WM_TRAYICON, OnTrayIcon)
			TRAYMESSAGE_HANDLER_EX(WM_TASKBARCREATED, OnTaskbarRestart)
			TRAYEND_MSG_MAP()

		virtual int OnTrayOnRButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam){ return 0; };
		virtual int OnTrayMsgNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam){ return 0; };

		//---------------------------------------------------
		// Function Des:��Ϣ��Ӧ
		LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			// Is this the ID we want?
			if (wParam != m_nid.uID)
				return 0;
			T* pT = static_cast<T*>(this);
			// Was the right-button clicked?
			if (LOWORD(lParam) == WM_RBUTTONUP)
			{
				OnTrayOnRButtonUp(uMsg, wParam, lParam);
			}
			else if (LOWORD(lParam) == WM_LBUTTONDBLCLK || LOWORD(lParam) == WM_LBUTTONDOWN)
			{
				int nMsgHandle = OnTrayMsgNotify(uMsg, wParam, lParam);
				if (nMsgHandle == 0)//���Ϊ0��ʾ���Լ����������Ϣ
					SetForegroundWindow(pT->m_hWnd);
			}
			else if (LOWORD(lParam) == 0x404)//win10�������˳��ᴥ������Ϣ
			{
			}
			return 0;
		}

		LRESULT OnTaskbarRestart(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
		{
			if (m_bInstalled && m_nid.hWnd && ::IsWindow(m_nid.hWnd))
			{
				m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
				return ::Shell_NotifyIcon(NIM_ADD, &m_nid);
			}
			return -1;
		}
	};
}//end namespace DM