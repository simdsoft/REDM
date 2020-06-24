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

	INT_PTR DMHDialog::DoModal(LPCWSTR lpszXmlId, HWND hWndParent/*=NULL*/, bool bShadow/*=false*/, DM::CRect rect/* = NULL*/)
	{
		BOOL bEnableParent = FALSE;
		if(NULL == hWndParent)
			hWndParent = ::GetActiveWindow();
		
		if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
		{
			::EnableWindow(hWndParent, FALSE);
			bEnableParent = TRUE;
		}

		if(DM_CreateWindow(lpszXmlId, rect.left,rect.top,rect.Width(),rect.Height(), hWndParent, bShadow))
		{	
			SendMessage(WM_INITDIALOG, (WPARAM)m_hWnd);//����init��Ϣ

			// �޸��ͺ���Ϣ����ҵ�������InitDialog�������ڴ�С�ͺ��ܾ���
			GetClientRect(rect);
			if (!rect.IsRectEmpty())
				CenterWindow();

			/*
			�����װqq��ʵ��¡��˳����Ѵ��ڡ��޷����ֵ�����---��������yggui�г���
			HWND_NOTOPMOST���������������зǶ��㴰��֮�ϣ��������ж��㴰��֮�󣩡���������Ѿ��ǷǶ��㴰����ñ�־�������á�
			HWND_TOP:����������Z��Ķ�����
			*/
			::SetWindowPos(m_hWnd, /*HWND_TOP*/HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
			g_pDMApp->Run(m_hWnd); // RunModalLoop, ����û�к�mfcһ������m_nFlags��Ǽ��ContinueModal

			// [mfc] hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW |
					SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
			if (-1 == m_nRetCode)
			{// ��ֹ��EndDialog�˳�(��˴��ڵĸ����ڲ���������,�������ڹر�ʱ)
				DMASSERT_EXPR(0, L"��DoModelû�е���EndDialog���˳���!");
				::PostQuitMessage(1);
			}
		}

		if (bEnableParent)
			::EnableWindow(hWndParent, TRUE);
		if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
			::SetActiveWindow(hWndParent);

		// destroy modal window
		DestroyWindow();

		return m_nRetCode;
	}

	void DMHDialog::EndDialog( INT_PTR nResult )
	{
		DMASSERT_EXPR(-1!=nResult,L"EndDialog�ķ���ֵ����Ϊ-1!");
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