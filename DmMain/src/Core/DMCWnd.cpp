#include "DmMainAfx.h"
#include "DMCWnd.h"
#include "DMCWndBase.h"
#include <OleCtl.h>
namespace DM
{

	DMCWnd*    DMCWnd::ms_pThis = NULL;
	BEGIN_MSG_MAP(DMCWnd)
	END_MSG_MAP()
	DMCWnd::DMCWnd()
	{
		m_pCurrentMsg		 = NULL;
		m_pfnSuperWindowProc = ::DefWindowProc;
		m_dwState			 = 0;
		m_bMsgHandled        = FALSE;
	}

	DMCWnd::~DMCWnd()
	{
	}

	HWND DMCWnd::CreateWindowEx(LPCWSTR lpClassName, LPCWSTR lpWindowName,DWORD dwStyle, DWORD dwExStyle,
								 int x, int y, int nWidth, int nHeight, HWND hWndParent, PVOID lpParam)
	{
		DMASSERT_EXPR((NULL == m_hWnd), L"当前窗口已创建");
		ms_pThis = this;// 此处初始化ms_pThis;
		MsgThunkInit();
		m_hWnd = ::CreateWindowExW(dwExStyle, lpClassName, 
			lpWindowName,dwStyle,x, y, nWidth, nHeight, hWndParent, NULL, g_pDMAppData->m_hInst,lpParam);
		ms_pThis = NULL;// 创建后必须重置为NULL
		return m_hWnd;
	}

	void DMCWnd::OnFinalMessage(HWND/* hWnd*/)
	{
		MsgThunkUnInit();
	}

	LRESULT CALLBACK DMCWnd::FirstWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		bool bThunk = false;
		do 
		{
			if (NULL == ms_pThis)/// 外部realwnd,使用同一个窗口类注册
			{
				break;
			}

			ms_pThis->m_hWnd = hWnd;
			PVOID p = (PVOID)(DMCWnd::WindowProc);
			if (false == ms_pThis->MsgThunkRun(hWnd, (DWORD_PTR)(DMCWnd::WindowProc), ms_pThis))
			{
				LOG_ERR("[mid]thunk fail\n");
				break;
			}
			bThunk = true;
		} while (false);

		if (bThunk)
		{
			WNDPROC pProc = (WNDPROC)ms_pThis->MsgThunkGetCodeAddress();// 得到Thunk指针
			return pProc(hWnd, uMsg, wParam, lParam);
		}
		else
		{/// 只有在realwnd时才可能到这
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);	
		}
	}

	LRESULT CALLBACK DMCWnd::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DMCWnd* pThis = (DMCWnd*)hWnd; // 强转为对象指针
		MSG msg = {pThis->m_hWnd, uMsg, wParam, lParam, 0, {0, 0}};
		const MSG* pOldMsg = pThis->m_pCurrentMsg;
		pThis->m_pCurrentMsg = &msg;

		// pass to the message map to process
		LRESULT lRes;
		BOOL bRet = pThis->ProcessWindowMessage(pThis->m_hWnd, uMsg, wParam, lParam, lRes, 0);
		// restore saved value for the current message
		DMASSERT_EXPR((pThis->m_pCurrentMsg == &msg), L"当前消息不相等");
		pThis->m_pCurrentMsg = pOldMsg;
		// do the default processing if message was not handled
		if (!bRet)
		{
			if (uMsg != WM_NCDESTROY)
			{
				lRes = pThis->DefWindowProc(uMsg, wParam, lParam);
			}
			else
			{
				// unsubclass, if needed
				LONG_PTR pfnWndProc = ::GetWindowLongPtr(pThis->m_hWnd, GWLP_WNDPROC);
				lRes = pThis->DefWindowProc(uMsg, wParam, lParam);
				if (pThis->m_pfnSuperWindowProc != ::DefWindowProc 
					&& ::GetWindowLongPtr(pThis->m_hWnd, GWLP_WNDPROC) == pfnWndProc)
				{
					::SetWindowLongPtr(pThis->m_hWnd, GWLP_WNDPROC, (LONG_PTR)pThis->m_pfnSuperWindowProc);
				}

				// mark window as destryed
				pThis->m_dwState |= DMSTATE_DESTROYED;
			}
		}

		if ((pThis->m_dwState & DMSTATE_DESTROYED) && NULL == pThis->m_pCurrentMsg)
		{
			// clear out window handle
			HWND hWnd = pThis->m_hWnd;
			pThis->m_hWnd = NULL;
			pThis->m_dwState &= ~DMSTATE_DESTROYED;

			// clean up after window is destroyed
			pThis->OnFinalMessage(hWnd);
		}
		return lRes;
	}

	LRESULT DMCWnd::DefWindowProc()
	{
		const MSG* pMsg = m_pCurrentMsg;
		LRESULT lRes = 0;
		if (pMsg != NULL)
		{
			lRes = DefWindowProc(pMsg->message, pMsg->wParam, pMsg->lParam);
		}

		return lRes;
	}

	LRESULT DMCWnd::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_pfnSuperWindowProc,m_hWnd, uMsg, wParam, lParam);
	}

	LRESULT DMCWnd::ForwardNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{// 和atlwin中的ForwardNotifications保持一致
		LRESULT lResult = 0;
		switch(uMsg)
		{
		case WM_COMMAND:
		case WM_NOTIFY:
		case WM_PARENTNOTIFY:
		case WM_DRAWITEM:
		case WM_MEASUREITEM:
		case WM_COMPAREITEM:
		case WM_DELETEITEM:
		case WM_VKEYTOITEM:
		case WM_CHARTOITEM:
		case WM_HSCROLL:
		case WM_VSCROLL:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			lResult = ::SendMessage(::GetParent(m_hWnd),uMsg, wParam, lParam);
			break;
		default:
			bHandled = FALSE;
			break;
		}
		return lResult;
	}

	LRESULT DMCWnd::ReflectNotifications(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{// 和atlwin中的ReflectNotifications保持一致
		HWND hWndChild = NULL;

		switch(uMsg)
		{
		case WM_COMMAND:
			if(lParam != NULL)    // not from a menu
				hWndChild = (HWND)lParam;
			break;
		case WM_NOTIFY:
			hWndChild = ((LPNMHDR)lParam)->hwndFrom;
			break;
		case WM_PARENTNOTIFY:
			switch(LOWORD(wParam))
			{
			case WM_CREATE:
			case WM_DESTROY:
				hWndChild = (HWND)lParam;
				break;
			default:
				hWndChild = ::GetDlgItem(m_hWnd,HIWORD(wParam));
				break;
			}
			break;
		case WM_DRAWITEM:
			if(wParam)    // not from a menu
				hWndChild = ((LPDRAWITEMSTRUCT)lParam)->hwndItem;
			break;
		case WM_MEASUREITEM:
			if(wParam)    // not from a menu
				hWndChild = ::GetDlgItem(m_hWnd,((LPMEASUREITEMSTRUCT)lParam)->CtlID);
			break;
		case WM_COMPAREITEM:
			if(wParam)    // not from a menu
				hWndChild =  ((LPCOMPAREITEMSTRUCT)lParam)->hwndItem;
			break;
		case WM_DELETEITEM:
			if(wParam)    // not from a menu
				hWndChild =  ((LPDELETEITEMSTRUCT)lParam)->hwndItem;

			break;
		case WM_VKEYTOITEM:
		case WM_CHARTOITEM:
		case WM_HSCROLL:
		case WM_VSCROLL:
			hWndChild = (HWND)lParam;
			break;
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
			hWndChild = (HWND)lParam;
			break;
		default:
			break;
		}

		if(hWndChild == NULL)
		{
			bHandled = FALSE;
			return 1;
		}

		DMASSERT(::IsWindow(hWndChild));
		return ::SendMessage(hWndChild, OCM__BASE + uMsg, wParam, lParam);
	}

	BOOL DMCWnd::DefaultReflectionHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
	{
		switch(uMsg)
		{
		case OCM_COMMAND:
		case OCM_NOTIFY:
		case OCM_PARENTNOTIFY:
		case OCM_DRAWITEM:
		case OCM_MEASUREITEM:
		case OCM_COMPAREITEM:
		case OCM_DELETEITEM:
		case OCM_VKEYTOITEM:
		case OCM_CHARTOITEM:
		case OCM_HSCROLL:
		case OCM_VSCROLL:
		case OCM_CTLCOLORBTN:
		case OCM_CTLCOLORDLG:
		case OCM_CTLCOLOREDIT:
		case OCM_CTLCOLORLISTBOX:
		case OCM_CTLCOLORMSGBOX:
		case OCM_CTLCOLORSCROLLBAR:
		case OCM_CTLCOLORSTATIC:
			lResult = ::DefWindowProc(hWnd, uMsg - OCM__BASE, wParam, lParam);
			return TRUE;
		default:
			break;
		}
		return FALSE;
	}


}//namespace DM