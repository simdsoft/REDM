#include "Plugin_ExpandAfx.h"
#include "DMAnimateWndBase.h"

namespace DM
{
	BEGIN_MSG_MAP(DMAnimateWndBase)
		MSG_WM_TIMER(OnTimer)
	END_MSG_MAP()
	DMAnimateWndBase::DMAnimateWndBase()
	{
		m_byFadeAlpha      = 0xff;
		m_iFadeSpeed       = 10;
		m_bFadeEnd         = false;
		m_bFadeing         = false;
	}

	DMCode DMAnimateWndBase::BeginFade(bool bEnd,int iSpeed,int iTimerDelta)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			if (0>=iSpeed||0>=iTimerDelta)
			{
				break;
			}

			m_bFadeEnd = bEnd;
			m_iFadeSpeed = iSpeed;
			if (m_bFadeEnd)
			{
				m_byFadeAlpha = 0xff;
			}
			else
			{
				m_byFadeAlpha = 0;
			}
			m_bFadeing = true;
			SetTimer(TIMER_FADE,iTimerDelta,NULL);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMAnimateWndBase::Create(LPCRECT lpRect,DWORD dwStyle,DWORD dwExStyle)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			if (IsWindow())
			{
				iErr = DM_ECODE_OK;
				break;
			}

			ATOM Atom = g_pDMApp->GetClassAtom(false);  // 是否使用阴影窗口类创建
			HWND hWnd = DMCWnd::CreateWindowEx((LPCWSTR)Atom,NULL,dwStyle,dwExStyle,lpRect->left,lpRect->top,lpRect->right-lpRect->left,lpRect->bottom-lpRect->top,NULL,0);
			if (NULL == hWnd)
			{
				break;
			}
			ModifyStyleEx(0,WS_EX_LAYERED);	
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	void DMAnimateWndBase::OnTimer(UINT_PTR idEvent)
	{
		if (TIMER_FADE == idEvent)
		{
			if (m_bFadeEnd)
			{
				m_byFadeAlpha -= m_iFadeSpeed;
				if (m_byFadeAlpha<0)
				{
					m_byFadeAlpha = 0;
					m_bFadeing = false;
					KillTimer(TIMER_FADE);
					EndFadeFinish();
				}
			}
			else
			{
				m_byFadeAlpha += m_iFadeSpeed;
				if (m_byFadeAlpha>255)
				{
					m_byFadeAlpha = 255;
					m_bFadeing = false;
					KillTimer(TIMER_FADE);
					StartFadeFinish();
				}
			}
			if (IsWindow())
			{
				Invalidate(false);
			}
		}
	}

	// 辅助
	BOOL DMAnimateWndBase::DMANI_ClientToScreen(HWND hWnd,CRect &rc)
	{
		BOOL bRet = FALSE;
		do 
		{
			if (!::IsWindow(hWnd))
			{
				break;
			}
			::ClientToScreen(hWnd, (LPPOINT)&rc);
			::ClientToScreen(hWnd, ((LPPOINT)&rc)+1);
			bRet = TRUE;
		} while (FALSE);
		return bRet;
	}

	BOOL DMAnimateWndBase::UpdateLayerWnd(IDMCanvas*pCanvas,byte alpha,CPoint pt)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		ClientToScreen(rcClient);
		DMAutoDC hdcDst;
		BLENDFUNCTION bf = {AC_SRC_OVER,0,alpha,AC_SRC_ALPHA};
		HDC dcMem = pCanvas->GetDC();
		::UpdateLayeredWindow(m_hWnd,hdcDst,&rcClient.TopLeft(),&rcClient.Size(),dcMem,&pt,0,&bf,ULW_ALPHA);
		pCanvas->ReleaseDC(dcMem);
		return TRUE;
	}
}//namespace DM