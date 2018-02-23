#include "DmMainAfx.h"
#include "DMDropWnd.h"

namespace DM
{
	BEGIN_MSG_MAP(DMDropWnd)
		MSG_WM_NCCREATE(OnNcCreate)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_KEYDOWN(OnKeyDown);
		MSG_WM_KILLFOCUS(OnKillFocus)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_ACTIVATE(OnActivate)
			MSG_WM_ACTIVATEAPP(OnActivateApp)
			MSG_WM_MOUSEACTIVATE(OnMouseActivate)
			CHAIN_MSG_MAP(DMHWnd)
	END_MSG_MAP()
	DMDropWnd::DMDropWnd(IDMDropOwner* pOwner)
		{
			m_pOwner    = pOwner;
			m_bClick    = false;
			m_bShow     = false;
			m_uExitCode = IDCANCEL;
			g_pDMApp->CreateRegObj((void**)&m_pToolTip,NULL,DMREG_ToolTip);
			PreMsg();
		}

		DMDropWnd::~DMDropWnd()
		{
			UnPreMsg();
			if (IsWindow())
			{
				DestroyWindow();
			}
			int nCount = GetRefCount();
			if (nCount>0)
			{
				SetRefCount(0);
			}
		}

		DMCode DMDropWnd::Show(LPCRECT lpRect)
		{
			DMCode iErr = DM_ECODE_FAIL;
			do 
			{
				PreMsg();
				if (!IsWindow())
				{
					iErr = Create(lpRect);
					if (!DMSUCCEEDED(iErr))
					{
						break;
					}
				}
				MoveWindow(lpRect);
				m_bShow = true;
				if (m_pOwner)
				{
					m_pOwner->OnDropDown(this);///< 让拥有者自己操作显示吧
				}
				SetCapture();
				iErr = DM_ECODE_OK;
			} while (false);
			return iErr;
		}

		DMCode DMDropWnd::Hide(UINT uCode)
		{
			DMCode iErr = DM_ECODE_FAIL;
			do 
			{
				if (!IsWindow()
					||!IsWindowVisible())
				{
					break;
				}
				m_uExitCode = uCode;
				UnPreMsg();
				ShowWindow(SW_HIDE);
				m_bShow = false;
				m_bClick = false;
				ReleaseCapture();
				if (m_pOwner)
				{
					m_pOwner->OnCloseUp(this,m_uExitCode);///< 让拥有者自己操作
					DUIWindow* pOwnerWnd = m_pOwner->GetOwnerWindow();
					if (pOwnerWnd)
					{
						HWND hWnd = pOwnerWnd->GetContainer()->OnGetHWnd();
						::SetActiveWindow(hWnd);
					}
				}

				iErr = DM_ECODE_OK;
			} while (false);
			return iErr;
		}

		DMCode DMDropWnd::Create(LPCRECT lpRect,DWORD dwStyle,DWORD dwExStyle)
		{
			DMCode iErr = DM_ECODE_FAIL;
			do 
			{	
				if (NULL == m_pOwner)
				{
					break;
				}
				DUIWindow *pOwnerWnd = m_pOwner->GetOwnerWindow();
				if (NULL == pOwnerWnd)
				{
					break;
				}
				HWND hParent = pOwnerWnd->GetContainer()->OnGetHWnd();
				ATOM Atom = g_pDMApp->GetClassAtom(false);
				HWND hWnd = DMCWnd::CreateWindowEx((LPCWSTR)Atom,NULL,dwStyle,dwExStyle,lpRect->left,lpRect->top,lpRect->right-lpRect->left,lpRect->bottom-lpRect->top,hParent,0);
				if (NULL == hWnd)
				{
					break;
				}
				iErr = DM_ECODE_OK;
			} while (false);
			return iErr;
		}

		//---------------------------------------------------
		// Function Des: 消息分发系列函数
		//---------------------------------------------------
		LRESULT DMDropWnd::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
		{
			return true;
		}

		void DMDropWnd::OnLButtonDown(UINT nFlags, CPoint point)
		{
			CRect rcClient;
			GetClientRect(&rcClient);
			if (!rcClient.PtInRect(point))
			{
				Hide();
			}
			else
			{
				m_bClick = true;
				SetMsgHandled(FALSE);
			}
		}

		void DMDropWnd::OnLButtonUp(UINT nFlags, CPoint point)
		{
			if (m_bClick)
			{
				HWND hWnd = m_hWnd;
				CRect rcClient;
				GetClientRect(&rcClient);
				LRESULT lRes = 0;
				DMHWnd::ProcessWindowMessage(m_hWnd,WM_LBUTTONUP,nFlags,MAKELPARAM(point.x,point.y),lRes);
				if (::IsWindow(hWnd) && !rcClient.PtInRect(point))
				{
					Hide();// 强制关闭弹出窗口
				}
			} 
		}

		void DMDropWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
		{
			if (VK_RETURN == nChar)
			{
				Hide(IDOK);
			}
			else if (VK_ESCAPE == nChar)
			{
				Hide();
			}
			else 
			{
				SetMsgHandled(FALSE);
			}
		}

		void DMDropWnd::OnKillFocus(HWND wndFocus)
		{
			if (wndFocus != m_hWnd)
			{
				Hide();
			}
		}

		void DMDropWnd::OnDestroy()
		{
			if (IsWindow()&&IsWindowVisible()&&m_pOwner)
			{
				m_pOwner->OnCloseUp(this,m_uExitCode);
			}
			m_pOwner = NULL;
			SetMsgHandled(FALSE);
		}

		void DMDropWnd::OnActivateApp(BOOL bActive, DWORD dwThreadID)
		{
			if (!bActive)
			{
				Hide();
			}
		}

		int DMDropWnd::OnMouseActivate( HWND wndTopLevel, UINT nHitTest, UINT message )
		{
			return MA_NOACTIVATE;// Do not activate CWnd object
		}

		BOOL DMDropWnd::PreTranslateMessage(MSG* pMsg)
		{
			BOOL bRet = FALSE;
			do 
			{
				if (!IsWindow())
				{
					break;
				}
				if (WM_LBUTTONDOWN == pMsg->message && pMsg->hwnd != m_hWnd && IsWindowVisible())
				{// 防止先按下window+D,再鼠标点击恢复的这种测试@_@ 
					Hide(IDOK);
					break;
				}
			
				if (WM_ACTIVATEAPP == pMsg->message)
				{
					SendMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
					break;
				}

				if (WM_MOUSEMOVE == pMsg->message)
				{// 由于窗口显示后就调用了setcapture，导致收不到setcursor消息，这里在WM_MOUSEMOVE消息里模拟一个setcursor消息。
					SendMessage(WM_SETCURSOR,(WPARAM)m_hWnd,MAKELPARAM(HTCLIENT,WM_MOUSEMOVE));
					break;
				}

				if (!(pMsg->message>=WM_KEYFIRST && pMsg->message<=WM_KEYLAST) 
					&& WM_MOUSEWHEEL!=pMsg->message) 
				{
					break;
				}
				SendMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
				bRet = TRUE;
			} while (FALSE);
			return bRet;
		}

		void DMDropWnd::OnFinalMessage(HWND hWnd)
		{
			__super::OnFinalMessage(hWnd);
			delete this;
		}

		DMCode DMDropWnd::OnReleaseCaptureWnd()
		{
			DMCode iErr = DMHWnd::OnReleaseCaptureWnd();
			if (m_bShow)
			{
				SetCapture();// DMHWnd::OnReleaseCaptureWnd()会ReleaseCapture,此函数可能由子DUI触发
			}
			return iErr;
		}

		// 辅助
		DMCode DMDropWnd::PreMsg()
		{
			m_pCurMsgLoop->AddMessageFilter(m_pToolTip.get());
			g_pDMApp->AddMessageFilter(this);
			return DM_ECODE_OK;
		}

		DMCode DMDropWnd::UnPreMsg()
		{
			g_pDMApp->RemoveMessageFilter(m_pToolTip.get());
			g_pDMApp->RemoveMessageFilter(this);
			return DM_ECODE_OK;
		}

}//namespace DM