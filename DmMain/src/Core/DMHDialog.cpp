#include "DmMainAfx.h"
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

	DMHDialog::~DMHDialog()
	{
	}

	INT_PTR DMHDialog::DoModal(LPCWSTR lpszXmlId, HWND hParent/*=NULL*/, bool bShadow/*=false*/, DM::CRect rect/* = NULL*/)
	{
		BOOL bEnableParent = FALSE;
		if(NULL == hParent)
		{
			hParent = ::GetActiveWindow();
		}
		
		if (hParent && hParent != ::GetDesktopWindow() && ::IsWindowEnabled(hParent))
		{
			::EnableWindow(hParent, FALSE);
			bEnableParent = TRUE;
		}

		if(!DM_CreateWindow(lpszXmlId, rect.left,rect.top,rect.Width(),rect.Height(), hParent, bShadow))
		{
			::EnableWindow(hParent, TRUE);
			return 0; // 此处失败了
		}

		GetClientRect(rect);
		if (!rect.IsRectEmpty())
		{
			CenterWindow();
		}

		SendMessage(WM_INITDIALOG,(WPARAM)m_hWnd);//发送init消息

		/*
		解决安装qq五笔导致“退出提醒窗口”无法出现的问题---此问题在yggui中出现
		HWND_NOTOPMOST：将窗口置于所有非顶层窗口之上（即在所有顶层窗口之后）。如果窗口已经是非顶层窗口则该标志不起作用。
		HWND_TOP:将窗口置于Z序的顶部。
		*/
		::SetWindowPos(m_hWnd, /*HWND_TOP*/HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOACTIVATE);
	    g_pDMApp->Run(m_hWnd,true);// 此内部已封装完ActiveWnd切换和DestoryWindow
		if (-1 == m_nRetCode)
		{// 防止非EndDialog退出(如此窗口的父窗口不是主窗口,而主窗口关闭时)
			DMASSERT_EXPR(0,L"此DoModel没有调用EndDialog就退出了!");
			::PostQuitMessage(1);
		}
		
		if (bEnableParent)
		{
			::EnableWindow(hParent, TRUE);
		}

		return m_nRetCode;
	}

	void DMHDialog::EndDialog( INT_PTR nResult )
	{
		DMASSERT_EXPR(-1!=nResult,L"EndDialog的返回值不能为-1!");
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