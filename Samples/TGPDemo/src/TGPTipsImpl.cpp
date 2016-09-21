#include "TGPDemoAfx.h"
#include "TGPTipsImpl.h"

namespace DM
{
//----------------------------------------------------------------------------------
#define TIMERID_DELAY    1
#define TIMERID_SPAN     2
#define MARGIN_TIP       5
	BEGIN_MSG_MAP(TGPTipsImpl)
		MSG_WM_TIMER(OnTimer)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
	TGPTipsImpl::TGPTipsImpl(void)
	:m_iDelayTime(500),
	m_iSpanTime(5000),
	m_bShowing(false)
	{
		m_rcPosFlags.SetRect(-1,0,0,0);
	}

	TGPTipsImpl::~TGPTipsImpl(void)
	{
	}

	bool TGPTipsImpl::Create()
	{
		bool bRet = false;
		do 
		{
			if (IsWindow())
			{
				bRet = true;
				break;
			}
			ATOM Atom = g_pDMApp->GetClassAtom(true);  // 是否使用阴影窗口类创建
			HWND hWnd = DMCWnd::CreateWindowEx((LPCWSTR)Atom,L"tips",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0,NULL,NULL);// 创建隐藏的定时器窗口
			if (NULL == hWnd)
			{
				break;
			}
			ShowWindow(SW_HIDE);// 默认隐藏，仅使用其定时器功能
			bRet = true;
		} while (false); 
		return bRet;
	}

	bool TGPTipsImpl::CreateTipsWnd()   
	{  
		bool bRet = false;  
		do      
		{  
			if (IsTipWindows())// 已创建 
			{
				bRet = true;
				break;
			}
			m_pWnd.Attach(new TGPTipHWnd);
			if (!m_strXmlId.IsEmpty())
			{
				if (PathFileExists(m_strXmlId))// xmlid是一个文件路径
				{
					LPWSTR lpXmlPath = m_strXmlId.GetBuffer();
					DWORD dwSize = GetFileSizeW(lpXmlPath);
					DMBufT<byte> pBuf;pBuf.AllocateBytes(dwSize);
					DWORD dwRead = 0;
					GetFileBufW(lpXmlPath,(void**)&pBuf,dwSize,dwRead);	
					m_strXmlId.ReleaseBuffer();
					if (NULL == m_pWnd->DM_CreateWindowEx(pBuf,dwSize,L"tipsWnd",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0,NULL,NULL,false))// 创建主窗口
					{
						break;
					}
				}
				else if (NULL == m_pWnd->DM_CreateWindowEx(m_strXmlId,L"tipsWnd",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0,NULL,NULL,false))// 创建主窗口
				{
					break;
				}
			}
			else if (!m_strTip.IsEmpty())// 构建简单的str窗口
			{
				DMAutoDC hdc;
				CRect rcText(0,0,500,1000);
				DrawText(hdc,m_strTip,-1,&rcText,DT_CALCRECT|DT_LEFT|DT_WORDBREAK);// 计算文本大小

				CRect rcWnd = rcText;
				rcWnd.InflateRect(MARGIN_TIP,MARGIN_TIP);
				rcWnd.OffsetRect(-rcWnd.TopLeft());

				CStringW strXml;
				strXml.Format(L"<dm  initsize=\"%d,%d\"><root pos=\"0,0,-0,-0\" clrbg=\"pbgra(ff,ff,ff,ff)\" text=\"%s\"/></dm>",rcWnd.Width(),rcWnd.Height(),m_strTip);
				int ulSize = 200;
				DMBufT<BYTE>pBuf;pBuf.Allocate(ulSize);
				UnicodeToUtf8(strXml.GetBuffer(),(PCHAR)pBuf.get(),ulSize);
				strXml.ReleaseBuffer();
				if (NULL == m_pWnd->DM_CreateWindowEx(pBuf,ulSize,L"tipsWnd",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0,NULL,NULL,false))// 创建主窗口
				{
					break;
				}
			}
			CPoint pt;
			GetCursorPos(&pt);
			m_pWnd->SetWindowPos(0,pt.x,pt.y,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
			m_pWnd->ShowWindow(SW_HIDE);
			bRet = true;
		} while (false);
		return bRet;
	}

	void TGPTipsImpl::OnTimer(UINT_PTR idEvent)
	{
		switch (idEvent)
		{
		case TIMERID_DELAY:// 显示,延迟消失
			{
				m_bShowing = false;
				KillTimer(TIMERID_DELAY);      
				ShowTips(true);
				SetTimer(TIMERID_SPAN,m_iSpanTime);
			}
			break;

		case TIMERID_SPAN:
			{
				if (!IsCursorPtInRect())// 当前没有停留在tip窗口上
				{
					m_bShowing = false;
					ShowTips(false);
					KillTimer(TIMERID_SPAN);
				}
			}
			break;
		}
	}

	//
	DMCode TGPTipsImpl::Update(PDMToolTipInfo pInfo)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pInfo)
			{
				break;
			}
			CStringW strXmlId = pInfo->lpszXmlId;
			CStringW strTip   = pInfo->lpszTip;

			if (IsTipWindows())
			{
				if ((!m_strXmlId.IsEmpty()&&strXmlId == m_strXmlId)// 原来通过xmlid创建，并且新传入的xmlid和它一致
					||m_strXmlId.IsEmpty()&&!m_strTip.IsEmpty()&&strTip == m_strTip// 原来通过text创建，并且新传入的text和它一致
					)
				{ 
				}
				else
				{
					DestroyTipsWnd();
				} 
			}
			m_strXmlId	 = strXmlId;
			m_strTip	 = pInfo->lpszTip;
			m_rcPosFlags = pInfo->rcPosFlags;
			m_rcScreenTarget = pInfo->rcScreenTarget;
			Create();
			CreateTipsWnd();
	
			/// 更新数据
			m_rcTarget = pInfo->rcTarget;
			m_iDelayTime = pInfo->iDelayTime;
			m_iSpanTime  = pInfo->iSpanTime;

			if (m_pWnd->IsWindowVisible())
			{
				ShowTips(true);  
			} 

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode TGPTipsImpl::Hide()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!IsTipWindows())
			{
				break;// 这时不要创建了，因为隐藏也可能发生在宿主窗口销毁后，这时消息循环可能没处理完
			}
			ShowTips(false);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode TGPTipsImpl::Clear()
	{
		if (IsWindow())
		{
			DestroyWindow();
		}
		if (m_pWnd)
		{
			if (m_pWnd->IsWindow())
			{
				m_pWnd->DestroyWindow();
			}
			m_pWnd.Release();
		}
		return DM_ECODE_OK;
	}

	DMCode TGPTipsImpl::RelayEvent(const MSG *pMsg)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!IsTipWindows())
			{
				break;
			}

			switch (pMsg->message)
			{
			case WM_MOUSEMOVE:
				{
					CPoint pt(GET_X_LPARAM(pMsg->lParam),GET_Y_LPARAM(pMsg->lParam));

					if (!m_rcTarget.PtInRect(pt))
					{
						if (!IsCursorPtInRect())
						{
							OnTimer(TIMERID_SPAN);// 直接隐藏
						}
					}
					else if (IsTipWindows() && !m_pWnd->IsWindowVisible() && !m_bShowing)
					{
						KillTimer(TIMERID_DELAY);
						SetTimer(TIMERID_DELAY,m_iDelayTime);    
						m_bShowing = true;
						::ClientToScreen(pMsg->hwnd,&pt);
						m_pWnd->SetWindowPos(0,pt.x,pt.y,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
					}
				}
				break;

			case WM_MOUSELEAVE:
				{
					CPoint pt(GET_X_LPARAM(pMsg->lParam),GET_Y_LPARAM(pMsg->lParam));

					if (!m_rcTarget.PtInRect(pt))
					{
						if (!IsCursorPtInRect())
						{
							OnTimer(TIMERID_SPAN);// 直接隐藏
						}
					}
				}
				break;
			}

			if (!m_pWnd->IsWindowVisible())
			{
				break;
			}
			iErr = DM_ECODE_OK;

		} while (false);
		return iErr;
	}

	BOOL TGPTipsImpl::PreTranslateMessage(MSG* pMsg)
	{
		if (IsTipWindows()) 
		{
			RelayEvent(pMsg);
		}
		return FALSE;
	}

	// 辅助---------------------------------
	void TGPTipsImpl::ShowTips(bool bShow)
	{
		do 
		{
			if (!IsTipWindows())
			{
				break;
			}

			if (!bShow)
			{
				m_bShowing = false;// 隐藏时要设置为不在显示过程中
				m_rcTarget.SetRect(0,0,0,0);
				if (!m_pWnd->IsWindowVisible())
				{
					break;
				}
				m_pWnd->ShowWindow(SW_HIDE);
				KillTimer(TIMERID_DELAY);
				KillTimer(TIMERID_SPAN);
				break;// 跳出
			}

			CRect rcWnd;
			m_pWnd->GetWindowRect(&rcWnd);
			int cx = GetSystemMetrics(SM_CXSCREEN); 
			int cy = GetSystemMetrics(SM_CYSCREEN);
			// 计算固定位置
			int iFlags = m_rcPosFlags.left;
			bool bFixPos = (m_rcPosFlags.bottom!=0);
			if (-1 != iFlags)
			{
				int x = m_rcPosFlags.top;
				int y = m_rcPosFlags.right;
			
				int iWid = rcWnd.Width();
				int iHei = rcWnd.Height();
			
				if (TPM_LEFTALIGN == (iFlags&TPM_LEFTALIGN))//若设置此标志，函数使快捷菜单的左边界与m_rcScreenTarget.left+x指定的坐标对齐,同时快捷菜单的上边界和m_rcScreenTarget.bottom对齐
				{
					rcWnd.left = m_rcScreenTarget.left + x;
					rcWnd.right = rcWnd.left + iWid;
					rcWnd.top = m_rcScreenTarget.bottom;
					rcWnd.bottom = rcWnd.top + iHei;
				}
				if (TPM_RIGHTALIGN == (iFlags&TPM_RIGHTALIGN))//若设置此标志，函数使快捷菜单的右边界与m_rcScreenTarget.right+x指定的坐标对齐,同时快捷菜单的上边界和m_rcScreenTarget.bottom对齐
				{
					rcWnd.right = m_rcScreenTarget.right + x;
					rcWnd.left = rcWnd.right - iWid;
				}

				if (TPM_BOTTOMALIGN == (iFlags&TPM_BOTTOMALIGN))//若设置此标志，函数使快捷菜单的下边界与与m_rcScreenTarget.top+y指定的坐标对齐,
				{
					rcWnd.bottom = m_rcScreenTarget.top + y;
					rcWnd.top = rcWnd.bottom - iHei;
				}
			}

			if (-1 == m_rcPosFlags.left|| // 未指定显示坐标
				false == bFixPos)// 不强制固定
			{
				if (rcWnd.right>cx) 
				{
					rcWnd.OffsetRect(cx-rcWnd.right,0);
				}
				if (rcWnd.bottom>cy) 
				{
					rcWnd.OffsetRect(0,cy-rcWnd.bottom);
				}
			}
			
			m_pWnd->SetWindowPos(HWND_TOPMOST,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),SWP_NOSENDCHANGING|SWP_SHOWWINDOW|SWP_NOACTIVATE);
		} while (false);
	}

	bool TGPTipsImpl::IsCursorPtInRect()
	{
		CPoint ptScreen;
		GetCursorPos(&ptScreen);
		CRect rcWnd;
		m_pWnd->GetWindowRect(rcWnd);
		if (!rcWnd.PtInRect(ptScreen))
		{
			return false;
		}
		return true;
	}

	void TGPTipsImpl::DestroyTipsWnd()
	{
		if (m_pWnd)
		{
			if (m_pWnd->IsWindow())
			{
				m_pWnd->DestroyWindow();
			}
			m_pWnd.Release();
		}
	}
}//namespace DM