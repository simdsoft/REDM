#include "DmMainAfx.h"
#include "DMContainerImpl.h"

namespace DM
{
	DMContainerImpl::DMContainerImpl(DUIWindow *pHWnd)
		:m_pContainWnd(pHWnd)
		,m_FocusMgr(pHWnd)
		,m_DropTarget(pHWnd)
	{
		m_hDUICaptureWnd = 0;
		m_hDUIHoverWnd	 = 0;
		m_bNcHover		 = false;
	}

	LRESULT DMContainerImpl::OnFrameEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		m_pContainWnd->AddRef();
		m_pContainWnd->SetMsgHandled(TRUE);
		switch (uMsg)
		{// 此部分消息可直接参考DMMsgCrack.h宏
		case WM_MOUSEMOVE:
			{
				OnFrameMouseMove((UINT)wParam,CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)));
			}
			break;

		case WM_MOUSEHOVER:
			{
				OnFrameMouseEvent(uMsg,wParam,lParam);
			}
			break;

		case WM_MOUSELEAVE:
			{
				OnFrameMouseLeave();
			}
			break;

		case WM_SETCURSOR:
			{
				lRet = DMSUCCEEDED(OnFrameSetCursor(CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam))));
			}
			break;

		case WM_KEYDOWN:
			{
				OnFrameKeyDown((UINT)wParam,(UINT)lParam&0xFFFF, (UINT)((lParam&0xFFFF0000)>>16));
			}
			break;

		case WM_SETFOCUS:
			{
				OnActivate(WA_ACTIVE);
			}
			break;

		case WM_KILLFOCUS:
			{
				OnActivate(WA_INACTIVE);
			}
			break;

		case WM_ACTIVATE:// WM_ACTIVATE只发给被激活或失去激活的指定窗口
			{
				OnActivate(LOWORD(wParam));
			}
			break;

		case WM_ACTIVATEAPP:// WM_ACTIVATEAPP发给被激活或失去激活的应用程序所拥有的所有窗口
			{
				OnActivateApp((BOOL)wParam,(DWORD)lParam);
			}
			break;

		case WM_IME_STARTCOMPOSITION:
		case WM_IME_ENDCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_IME_CHAR:
			{
				OnFrameKeyEvent(uMsg,wParam,lParam);
			}
			break;

		default:
			{
				if (uMsg>=WM_KEYFIRST&&uMsg<=WM_KEYLAST)// KeyDown相关
				{
					OnFrameKeyEvent(uMsg,wParam,lParam);
				}
				else if(uMsg>=WM_MOUSEFIRST&&uMsg<= WM_MOUSELAST)// 鼠标点击相关
				{
					OnFrameMouseEvent(uMsg,wParam,lParam);
				}
				else
				{
					m_pContainWnd->SetMsgHandled(FALSE);
				}
			}
			break;
		}
		m_pContainWnd->Release();
		return lRet;
	}

	DMCode DMContainerImpl::OnSetFocusWnd(DUIWND DUIWnd)
	{
		HWND hWnd = OnGetHWnd();
		if (DUIWnd && ::GetFocus()!=hWnd&& !(::GetWindowLongPtr(hWnd,GWL_EXSTYLE)&WS_EX_TOOLWINDOW))
		{
			::SetFocus(hWnd);
		}

		m_FocusMgr.SetFocusedWnd(DUIWnd);
		return DM_ECODE_OK;
	}

	DMCode DMContainerImpl::OnGetFocusWnd(DUIWND &DUIWnd)
	{
		DUIWnd = m_FocusMgr.GetFocusedWnd();
		return DM_ECODE_OK;
	}

	DMCode DMContainerImpl::OnSetCaptureWnd(DUIWND DUIWnd,DUIWNDPtr pOldDUIWnd/*=NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DUIWindow *pWnd = g_pDMDWndPool->FindDUIWnd(DUIWnd);
			if (NULL == pWnd
				||pWnd->DM_IsDisable(true)
				)
			{
				break;
			}

			if (pOldDUIWnd)
			{
				*pOldDUIWnd = m_hDUICaptureWnd;
			}
			m_hDUICaptureWnd = DUIWnd;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMContainerImpl::OnGetCaptureWnd(DUIWND &DUIWnd)
	{
		DUIWnd = m_hDUICaptureWnd;
		return DM_ECODE_OK;
	}

	DMCode DMContainerImpl::OnReleaseCaptureWnd()
	{
		m_hDUICaptureWnd = 0;
		return DM_ECODE_OK;
	}

	DMCode DMContainerImpl::OnRegisterDragDrop(DUIWND hDUIWnd, IDropTarget *pDropTarget)
	{
		if (m_DropTarget.RegisterDragDrop(hDUIWnd,pDropTarget))
		{
			return DM_ECODE_OK;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMContainerImpl::OnUnRegisterDragDrap(DUIWND hDUIWnd)
	{
		if (m_DropTarget.UnRegisterDragDrap(hDUIWnd))
		{
			return DM_ECODE_OK;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMContainerImpl::OnRegisterTimeline(IDMTimeline *pHandler)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pHandler)
			{
				break;
			}
			POSITION pos = m_TimelineList.Find(pHandler);
			if (pos)
			{
				break;
			}

			m_TimelineList.AddTail(pHandler);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMContainerImpl::OnUnregisterTimeline(IDMTimeline *pHandler)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pHandler)
			{
				break;
			}
			POSITION pos = m_TimelineList.Find(pHandler);
			if (!pos)
			{
				break;
			}
			m_TimelineList.RemoveAt(pos);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMContainerImpl::OnTimeline()
	{
		DMTimelineHanderList TimelineList; 
		POSITION Pos = m_TimelineList.GetHeadPosition();
		while (Pos)// 复制一份，防止原数据被污染
		{
			IDMTimelinePtr &t = m_TimelineList.GetNext(Pos);
			TimelineList.AddTail(t);
		}

		Pos = TimelineList.GetHeadPosition();
		while (Pos)// 循环处理
		{
			IDMTimelinePtr pHandler = TimelineList.GetNext(Pos);
			pHandler->OnTimeline();
		}
		return DM_ECODE_OK;
	}

	// 辅助 -------------------------------------------
	// 处理host window WM_MOUSEMOVE消息
	/*
	逻辑：
	1.设置了Capture窗口（捕获鼠标窗口，接受鼠标所有消息，无论光标焦点是否在DUI窗口内）
		1.1.鼠标停留在窗口区域内，此时Capture窗口即为新的MouseHover窗口(pHoverWnd==pCaptureWnd)
			1.1.1.取得旧的MouseHover窗口(pOldHoverWnd)
			1.1.2.如(pOldHoverWnd!=NULL&&pOldHoverWnd!=pHoverWnd)，如原为非客户区Hover状态(true==m_bNcHover)则发送WM_NCMOUSELEAVE，否则发送WM_MOUSELEAVE
			1.1.3.设置非客户区Hover状态(m_bNcHover),设置新的MouseHover窗口句柄（m_hDUIHoverWnd = hDUIHoverWnd）
			1.1.4.如(pHoverWnd!=pOldHoverWnd),如(true==m_bNcHover)则发送WM_NCMOUSEHOVER，否则发送WM_MOUSEHOVER
		1.2.鼠标未停留在窗口区域内,此时新的MouseHover窗口为NULL
			1.2.1.取得旧的MouseHover窗口(pOldHoverWnd)
			1.2.2.如(pOldHoverWnd!=NULL)，如原为非客户区Hover状态(true==m_bNcHover)则发送WM_NCMOUSELEAVE，否则发送WM_MOUSELEAVE
			1.2.3.设置新的MouseHover窗口句柄为0，设置鼠标未停留在非客户区
	1.3.Capture窗口如原为非客户区Hover状态(true==m_bNcHover)则发送WM_NCMOUSEMOVE，否则发送WM_MOUSEMOVE
	2.未设置Capture窗口，逻辑和1类似
	*/
	void DMContainerImpl::OnFrameMouseMove(UINT uFlag,CPoint pt)
	{
		do 
		{
			DUIWindow *pCaptureWnd = g_pDMDWndPool->FindDUIWnd(m_hDUICaptureWnd);
			if (pCaptureWnd)
			{//1 Capture下，如果在Capture窗口内，就对Capture窗口发hover消息，对原始hover窗口发leave消息，同时更新hover窗口句柄
				CRect rcWindow;
				pCaptureWnd->DV_GetWindowRect(rcWindow);
				DUIWindow* pHoverWnd = rcWindow.PtInRect(pt)?pCaptureWnd:NULL;
				DUIWND hDUIHoverWnd = pHoverWnd?pHoverWnd->GetDUIWnd():0;
				if (hDUIHoverWnd != m_hDUIHoverWnd)
				{
					DUIWindow* pOldHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);//1.1.1
					m_hDUIHoverWnd = hDUIHoverWnd;
					if (pOldHoverWnd)
					{
						 pOldHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSELEAVE:WM_MOUSELEAVE);
					}
					if (pHoverWnd)
					{
						pHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSEHOVER:WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
					}
				}
				pCaptureWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSEMOVE:WM_MOUSEMOVE,uFlag,MAKELPARAM(pt.x,pt.y));
				break;// Capture结束！
			}

			// 2.非Capture
			DUIWND hDUIHoverWnd = m_pContainWnd->HitTestPoint(pt);
			DUIWindow* pHoverWnd = g_pDMDWndPool->FindDUIWnd(hDUIHoverWnd);
			if (hDUIHoverWnd!=m_hDUIHoverWnd)
			{// 2.1新Hover窗口不同于原始hover窗口，如存在，原始发lever(nc+client),新的发(hover+nchover)消息
				DUIWindow* pOldHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);
				m_hDUIHoverWnd = hDUIHoverWnd;
				if (pOldHoverWnd)
				{
					if (m_bNcHover) 
					{
						pOldHoverWnd->DM_SendMessage(WM_NCMOUSELEAVE);
					}
					pOldHoverWnd->DM_SendMessage(WM_MOUSELEAVE);
				}

				if (pHoverWnd&&!pHoverWnd->DM_IsDisable(true))
				{
					m_bNcHover = DMSUCCEEDED(pHoverWnd->DV_OnNcHitTest(pt));
					if (m_bNcHover) 
					{
						pHoverWnd->DM_SendMessage(WM_NCMOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
					}
					pHoverWnd->DM_SendMessage(WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
				}
			}
			else if (pHoverWnd&&!pHoverWnd->DM_IsDisable(true))
			{// 2.2前提，新hover窗口==原始hover窗口，此时计算nchover状态，发mouse消息
				bool bNcHover = DMSUCCEEDED(pHoverWnd->DV_OnNcHitTest(pt));
				if (bNcHover != m_bNcHover)
				{
					m_bNcHover = bNcHover;
					if (m_bNcHover)
					{
						pHoverWnd->DM_SendMessage(WM_MOUSELEAVE);
						pHoverWnd->DM_SendMessage(WM_NCMOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
					}
					else
					{
						pHoverWnd->DM_SendMessage(WM_NCMOUSELEAVE);
						pHoverWnd->DM_SendMessage(WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
					}
				}
			}
			if (pHoverWnd&&!pHoverWnd->DM_IsDisable(TRUE))
			{
				pHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSEMOVE:WM_MOUSEMOVE,uFlag,MAKELPARAM(pt.x,pt.y));
			}

		} while (FALSE);

#if 0
		do 
		{
			DUIWindow *pCaptureWnd = g_pDMDWndPool->FindDUIWnd(m_hDUICaptureWnd);
			if (pCaptureWnd)
			{//1
				CRect rcWindow;
				pCaptureWnd->DV_GetWindowRect(rcWindow);
				bool bInRect = false;
				if (rcWindow.PtInRect(pt))
				{//1.1
					bInRect = true;
				}

				if (bInRect)
				{
					DUIWindow* pOldHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);//1.1.1
					if (pOldHoverWnd&&pOldHoverWnd!=pCaptureWnd)//pOldHoverWnd==pCaptureWnd 表示即为原来Hover窗口
					{//1.1.2
						pOldHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSELEAVE:WM_MOUSELEAVE);
					}	
					m_bNcHover		= pCaptureWnd->DMSUCCEEDED(DV_OnNcHitTest(pt));//1.1.3
					m_hDUIHoverWnd  = pCaptureWnd->GetDUIWnd();	   //1.1.3
					if (pOldHoverWnd != pCaptureWnd)
					{
						pCaptureWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSEHOVER:WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));//1.1.4
					}
				}
				else
				{//1.2
					DUIWindow* pOldHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);//1.2.1
					if (pOldHoverWnd)
					{//1.2.2
						pOldHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSELEAVE:WM_MOUSELEAVE);
					}	
					m_hDUIHoverWnd = 0;			//1.2.3
					m_bNcHover     = false;		//1.2.3
				}
				pCaptureWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSEMOVE:WM_MOUSEMOVE,uFlag,MAKELPARAM(pt.x,pt.y));
			}
			else
			{
				DUIWND hDUIHoverWnd = m_pContainWnd->HitTestPoint(pt);
				DUIWindow* pHoverWnd = g_pDMDWndPool->FindDUIWnd(hDUIHoverWnd);
				if (hDUIHoverWnd!=m_hDUIHoverWnd)
				{
					DUIWindow* pOldHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);
					m_hDUIHoverWnd = hDUIHoverWnd;
					if (pOldHoverWnd)
					{
						pOldHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSELEAVE:WM_MOUSELEAVE);
					}

					if (pHoverWnd&&!pHoverWnd->DM_IsDisable(true))
					{
						m_bNcHover = pHoverWnd->DMSUCCEEDED(DV_OnNcHitTest(pt));
						pHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSEHOVER:WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
					}
				}
				else if (pHoverWnd&&!pHoverWnd->DM_IsDisable(true))
				{
					bool bNcHover = pHoverWnd->DMSUCCEEDED(DV_OnNcHitTest(pt));
					if (bNcHover != m_bNcHover)
					{
						m_bNcHover = bNcHover;
						if (m_bNcHover)
						{
							pHoverWnd->DM_SendMessage(WM_MOUSELEAVE);
							pHoverWnd->DM_SendMessage(WM_NCMOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
						}
						else
						{
							pHoverWnd->DM_SendMessage(WM_NCMOUSELEAVE);
							pHoverWnd->DM_SendMessage(WM_MOUSEHOVER,uFlag,MAKELPARAM(pt.x,pt.y));
						}
					}
				}
				if (pHoverWnd&&!pHoverWnd->DM_IsDisable(TRUE))
				{
					pHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSEMOVE:WM_MOUSEMOVE,uFlag,MAKELPARAM(pt.x,pt.y));
				}
			}	

		} while (false);
#endif //0
	}

	//-----------------------------------------------
	// 处理鼠标点击相关消息及MOUSEHOVER消息
	void DMContainerImpl::OnFrameMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		DUIWindow* pCaptureWnd = g_pDMDWndPool->FindDUIWnd(m_hDUICaptureWnd);
		if (pCaptureWnd)
		{// Capture窗口捕获所有鼠标点击消息，比如滚动条
			if (m_bNcHover&&WM_MOUSEWHEEL!=uMsg)
			{
				uMsg += WM_NCMOUSEFIRST-WM_MOUSEFIRST;// 转换成NC对应的消息
			}
			BOOL bMsgHandled = FALSE;
			pCaptureWnd->DM_SendMessage(uMsg,wParam,lParam,&bMsgHandled);// 转发消息
			m_pContainWnd->SetMsgHandled(bMsgHandled);
		}
		else
		{
			m_hDUIHoverWnd = m_pContainWnd->HitTestPoint(CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam)));
			DUIWindow* pHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);
			if (pHoverWnd&&!pHoverWnd->DM_IsDisable(true))
			{
				if (m_bNcHover&&WM_MOUSEWHEEL!=uMsg)
				{
					uMsg += WM_NCMOUSEFIRST-WM_MOUSEFIRST;// 转换成NC对应的消息
				}
				BOOL bMsgHandled = FALSE;
				pHoverWnd->DM_SendMessage(uMsg,wParam,lParam,&bMsgHandled);// 转发消息
				m_pContainWnd->SetMsgHandled(bMsgHandled);
			}
			else
			{
				m_pContainWnd->SetMsgHandled(FALSE);
			}
		}
	}

	// 处理WM_MOUSELEAVE
	void DMContainerImpl::OnFrameMouseLeave()
	{
		DUIWindow* pCaptureWnd = g_pDMDWndPool->FindDUIWnd(m_hDUICaptureWnd);
		if (pCaptureWnd)
		{
			pCaptureWnd->DM_SendMessage(WM_MOUSELEAVE);
		}
		else if (m_hDUIHoverWnd)
		{
			DUIWindow* pHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);
			if (pHoverWnd&&!pHoverWnd->DM_IsDisable(true))
			{
				pHoverWnd->DM_SendMessage(m_bNcHover?WM_NCMOUSELEAVE:WM_MOUSELEAVE);
			}
		}

		m_hDUIHoverWnd = 0;
	}

	// 
	DMCode DMContainerImpl::OnFrameSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DUIWindow* pCaptureWnd = g_pDMDWndPool->FindDUIWnd(m_hDUICaptureWnd);
			if (pCaptureWnd)
			{
				iErr = pCaptureWnd->DV_OnSetCursor(pt);
			}
			else
			{	
				DUIWindow* pHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);
				if (pHoverWnd&&!pHoverWnd->DM_IsDisable(true))
				{
					iErr = pHoverWnd->DV_OnSetCursor(pt);
				}
			}

			if (!DMSUCCEEDED(iErr))
			{// 设置为标准的箭头
				HCURSOR hCursor = ::LoadCursorW(NULL, IDC_ARROW);
				::SetCursor(hCursor);
			}
		} while (false);
		return iErr;
	}

	void DMContainerImpl::OnFrameKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		DUIWindow* pFocusWnd = g_pDMDWndPool->FindDUIWnd(m_FocusMgr.GetFocusedWnd());
		if (pFocusWnd)
		{
			BOOL bMsgHandled = FALSE;
			pFocusWnd->DM_SendMessage(uMsg,wParam,lParam,&bMsgHandled);
			m_pContainWnd->SetMsgHandled(bMsgHandled);
		}
		else
		{
			m_pContainWnd->SetMsgHandled(FALSE);
		}
	}

	// 处理WM_KEYDOWN消息
	void DMContainerImpl::OnFrameKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (m_FocusMgr.OnKeyDown(nChar))
			{// 首先处理焦点切换
				break;
			}
			OnFrameKeyEvent(WM_KEYDOWN,(WPARAM)nChar,MAKELPARAM(nRepCnt,nFlags));
		} while (FALSE);
	}

	void DMContainerImpl::OnActivate(UINT nState)
	{
		if (WA_INACTIVE == nState)
		{
			m_FocusMgr.StoreFocuseWnd();  // 存储当前焦点窗口
		}
		else if (WA_ACTIVE == nState)
		{
			m_FocusMgr.RestoreFocuseWnd();// 恢复先前焦点窗口
		}
	}

	void DMContainerImpl::OnActivateApp( BOOL bActive, DWORD dwThreadID )
	{
		MSG Msg = {0,WM_ACTIVATEAPP,bActive,dwThreadID,0};
		m_pContainWnd->DM_DispatchMessage(&Msg);
	}



}//namespace DM