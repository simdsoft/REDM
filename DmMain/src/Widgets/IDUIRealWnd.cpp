#include "DmMainAfx.h"
#include "IDUIRealWnd.h"


namespace DM
{	
	IDUIRealWnd::IDUIRealWnd()
	{
		m_hRealWnd = NULL;
	}

	int IDUIRealWnd::OnCreate(LPVOID)
	{
		HWND hWndParent = GetContainer()->OnGetHWnd();
		wchar_t wcClassName[MAX_PATH] = {0};
		::GetClassNameW(hWndParent, wcClassName, MAX_PATH);
		m_hRealWnd = OnCreateRealWnd(wcClassName,hWndParent);// 初始化
		if (NULL == m_hRealWnd)
		{
			DMASSERT_EXPR(0,L"创建realwnd失败");
			return 1;// 失败
		}
		g_pDMDWndPool->AddRealDUIWnd(GetDUIWnd());
		return 0;
	}

	void IDUIRealWnd::DM_OnPaint(IDMCanvas* pCanvas)
	{
		//CatchRealWndScreen(pCanvas);
	}


	void IDUIRealWnd::OnDestroy()
	{
		g_pDMDWndPool->RemoveRealDUIWnd(GetDUIWnd());
		if (!DMSUCCEEDED(OnDestoryRealWnd()))
		{
			if (m_hRealWnd&&::IsWindow(m_hRealWnd))
			{
				::DestroyWindow(m_hRealWnd);
				m_hRealWnd = NULL;
			}
		}
	}

	void IDUIRealWnd::OnSize(UINT nType, CSize size)
	{
		if (m_hRealWnd&&::IsWindow(m_hRealWnd))
		{
			CRect rcWnd;
			DV_GetClientRect(&rcWnd);
			if (!DMSUCCEEDED(OnMoveRealWnd(rcWnd)))
			{
				::SetWindowPos(m_hRealWnd,0,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),SWP_NOZORDER);
			}
		}
	}

	void IDUIRealWnd::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		__super::OnShowWindow(bShow, nStatus);
		if (!DMSUCCEEDED(OnShowRealWnd(bShow,nStatus)))
		{
			if (::IsWindow(m_hRealWnd))
			{
				ShowWindow(m_hRealWnd,DM_IsVisible(true) ? SW_SHOWNOACTIVATE: SW_HIDE);
			}
			MoveRealWnd();
		}
	}

	void IDUIRealWnd::OnMove(CPoint pt)
	{
		if (DMSUCCEEDED(OnIsPopupWnd()))
		{
			MoveRealWnd();
		}	
	}

	void IDUIRealWnd::MoveRealWnd()
	{
		do 
		{
			if (NULL == m_hRealWnd||!::IsWindow(m_hRealWnd)||!::IsWindowVisible(m_hRealWnd))
			{
				break;
			}
			CRect rcWnd;
			DV_GetClientRect(&rcWnd);
			if (!DMSUCCEEDED(OnMoveRealWnd(rcWnd)))
			{
				::SetWindowPos(m_hRealWnd,0,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
			}
		} while (false);
	}

	void IDUIRealWnd::CatchRealWndScreen(IDMCanvas* pCanvas)
	{
		CRect rcWnd;
		DV_GetClientRect(&rcWnd);
		if (m_hRealWnd&&::IsWindow(m_hRealWnd)&&::IsWindowVisible(m_hRealWnd))
		{// 抓屏
			HDC hdc = ::GetWindowDC(m_hRealWnd);
			HDC dcMem = pCanvas->GetDC();
			//::BitBlt(dcMem, rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), hdc, 0, 0, SRCCOPY);
			BLENDFUNCTION bf={AC_SRC_OVER,0,0XFF,AC_SRC_ALPHA};
			::AlphaBlend(dcMem,rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(),hdc,0, 0, rcWnd.Width(), rcWnd.Height(),bf);
			pCanvas->ReleaseDC(dcMem);
			::ReleaseDC(m_hRealWnd, hdc);
		}
	}

}// namespace DM