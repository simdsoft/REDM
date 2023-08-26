﻿#include "DmMainAfx.h"
#include "DMHDialog.h"

namespace DM
{
	BEGIN_MSG_MAP(DMHDialog)
		MSG_WM_KEYDOWN(OnKeyDown)
	    MSG_WM_CLOSE(OnCancel)
		CHAIN_MSG_MAP(DMHWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	BEGIN_EVENT_MAP(DMHDialog)
		EVENT_ID_COMMAND(IDOK,OnOK)
		EVENT_ID_COMMAND(IDCANCEL,OnCancel)
	END_EVENT_MAP()

	DMHDialog::DMHDialog()
	{
		m_nRetCode = -1;
	}

	INT_PTR DMHDialog::DoModal(LPCSTR lpszXmlId, HWND hWndParent/*=NULL*/, bool bShadow/*=false*/, DM::CRect rect/* = NULL*/)
	{
		BOOL bEnableParent = FALSE;
		if(NULL == hWndParent)
			hWndParent = ::GetActiveWindow();
		
		if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
		{
			::EnableWindow(hWndParent, FALSE);
			bEnableParent = TRUE;
		}

		if(DM_CreateWindow(lpszXmlId, rect.left,rect.top,rect.Width(),rect.Height(), hWndParent, bShadow ? NWSDS_DWMSHADOW : NWSDS_NULL))
		{	
			GetClientRect(rect);
			if (!rect.IsRectEmpty())
				CenterWindow();

			SendMessage(WM_INITDIALOG, (WPARAM)m_hWnd);//发送init消息

			/*
			解决安装qq五笔导致“退出提醒窗口”无法出现的问题---此问题在yggui中出现
			HWND_NOTOPMOST：将窗口置于所有非顶层窗口之上（即在所有顶层窗口之后）。如果窗口已经是非顶层窗口则该标志不起作用。
			HWND_TOP:将窗口置于Z序的顶部。
			*/
			::SetWindowPos(m_hWnd, /*HWND_TOP*/HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
			g_pDMApp->Run(m_hWnd); // RunModalLoop, 这里没有和mfc一样利用m_nFlags标记检查ContinueModal

			// [mfc] hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW |
					SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
			if (-1 == m_nRetCode)
			{// 防止非EndDialog退出(如此窗口的父窗口不是主窗口,而主窗口关闭时)
				DMFAIL_MSG("Modal Dialog must ended by EndDialog");
				::PostQuitMessage(1);
			}
		}

		if (bEnableParent)
			::EnableWindow(hWndParent, TRUE);
		if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
			::SetActiveWindow(hWndParent);

		// Destroy modal window
		if(IsWindow())
			DestroyWindow();

		return m_nRetCode;
	}

	void DMHDialog::EndDialog( INT_PTR nResult )
	{
		DMASSERT_MSG(-1!=nResult,"EndDialog can't be -1!");
		m_nRetCode = (-1==nResult)?IDCANCEL:nResult;
		PostMessage(WM_QUIT);
	} 

	// Protect
	DMCode DMHDialog::OnOK()
	{
		EndDialog(IDOK);
		return DM_ECODE_OK;
	}

	DMCode DMHDialog::OnCancel()
	{
		EndDialog(IDCANCEL);
		return DM_ECODE_OK;
	}

	void DMHDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			DMHWnd::OnKeyEvent(WM_KEYDOWN,(WPARAM)nChar,MAKELPARAM(nRepCnt,nFlags));
			if (DMHWnd::IsMsgHandled())
			{
				break;
			}
			if (VK_ESCAPE!=nChar&&VK_RETURN!=nChar)
			{
				break;
			}
			int iId = VK_ESCAPE==nChar?IDCANCEL:IDOK;
			DUIWindow* pExitWnd = FindChildById(iId);
			if (pExitWnd)
			{
				DMEventCmdArgs Evt(pExitWnd);
				pExitWnd->DV_FireEvent(Evt);
			}
		} while (false);
	}

}// namespace DM