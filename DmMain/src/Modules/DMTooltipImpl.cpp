#include "DmMainAfx.h"
#include "DMTooltipImpl.h"

namespace DM
{
#define TIMERID_DELAY    1
#define TIMERID_SPAN     2
#define MARGIN_TIP       5

	BEGIN_MSG_MAP(DMTooltipImpl)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_TIMER(OnTimer)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	DMTooltipImpl::DMTooltipImpl(void)
	:m_iDelayTime(500),
	m_iSpanTime(5000),
	m_font(NULL),
	m_bShowing(false)
	{
	}

	DMTooltipImpl::~DMTooltipImpl(void)
	{
		if (m_font) 
		{
			DeleteObject(m_font);
		}
	}

	bool DMTooltipImpl::Create()
	{
		bool bRet = false;
		do 
		{
			ATOM Atom = g_pDMApp->GetClassAtom(true);  // 是否使用阴影窗口类创建
			HWND hWnd = DMCWnd::CreateWindowEx((LPCWSTR)Atom,L"tooltip",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0,NULL,NULL);
			if (NULL == hWnd)
			{
				break;
			}
			

			LOGFONTW lf;
			GetObject(GetStockObject(DEFAULT_GUI_FONT),sizeof(lf),&lf);
			lf.lfHeight = -12;
			wcscpy_s(lf.lfFaceName, 32, L"宋体");
			m_font = CreateFontIndirectW(&lf);
			bRet = true;
		} while (false);
		return bRet;
	}

	void DMTooltipImpl::OnPaint(HDC hdc)
	{
		do 
		{
			CRect rcClient;
			GetClientRect(&rcClient);

			HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_INFOBK));
			HGDIOBJ hOld = SelectObject(hdc,hBrush);
			Rectangle(hdc,rcClient.left,rcClient.top,rcClient.right,rcClient.bottom);
			SelectObject(hdc,hOld);
			DeleteObject(hBrush);

			rcClient.DeflateRect(MARGIN_TIP,MARGIN_TIP);
			SetBkMode(hdc,TRANSPARENT);
			HGDIOBJ hOldFont = SelectObject(hdc,m_font);
			::DrawText(hdc,m_strTip,-1,&rcClient,DT_WORDBREAK);
			SelectObject(hdc,hOldFont);
		} while (false);
	}

	void DMTooltipImpl::OnTimer(UINT_PTR idEvent)
	{
		switch (idEvent)
		{
		case TIMERID_DELAY:
			{
				m_bShowing = false;
				KillTimer(TIMERID_DELAY);       
				ShowTooltip(true);
				SetTimer(TIMERID_SPAN,m_iSpanTime);
			}
			break;

		case TIMERID_SPAN:
			{
				m_bShowing = false;
				ShowTooltip(false);
				KillTimer(TIMERID_SPAN);
			}
			break;
		}
	}

	DMCode DMTooltipImpl::Update(PDMToolTipInfo pInfo)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pInfo)
			{
				break;
			}
			if (pInfo->strTip.IsEmpty())
			{
				break;
			}
			if (!IsWindow())
			{
				if (!Create())
				{
					break;
				}
			}

			/// 更新数据
			m_rcTarget = pInfo->rcTarget;
			m_strTip   = pInfo->strTip;
			m_strTip.Replace(L"\\r\\n",L"\r\n");//什么XML中\R\N不能被直接识别，因为XML文件中的"/n",会被认为是一个字符串"///n"是两个字符'//'和'/n'，而不是转义字符"/n",
			m_strTip.Replace(L"\\n",L"\\r");
			m_iDelayTime = pInfo->iDelayTime;
			m_iSpanTime  = pInfo->iSpanTime;

			if (IsWindowVisible())
			{
				ShowTooltip(true);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMTooltipImpl::Hide()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!IsWindow())
			{
				//if (!Create())// 这时不要创建了，因为隐藏也可能发生在宿主窗口销毁后，这时消息循环可能没处理完
				{
					break;
				}
			}
			ShowTooltip(false);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMTooltipImpl::Clear()
	{
		if (IsWindow())
		{
			ShowTooltip(false);
			DestroyWindow();
		}
		
		return DM_ECODE_OK;
	}

	DMCode DMTooltipImpl::RelayEvent(const MSG *pMsg)
	{
		switch (pMsg->message)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
			OnTimer(TIMERID_SPAN);// 直接隐藏
			break;
		case WM_MOUSEMOVE:
			{
				CPoint pt(GET_X_LPARAM(pMsg->lParam),GET_Y_LPARAM(pMsg->lParam));
				if (!m_rcTarget.PtInRect(pt))
				{
					OnTimer(TIMERID_SPAN);// 直接隐藏
				}
				else if (!IsWindowVisible() && !m_strTip.IsEmpty() && !m_bShowing)
				{
					KillTimer(TIMERID_DELAY);
					SetTimer(TIMERID_DELAY,m_iDelayTime);    
					m_bShowing = true;
					::ClientToScreen(pMsg->hwnd,&pt);
					SetWindowPos(0,pt.x,pt.y+24,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOSENDCHANGING|SWP_NOACTIVATE);
				}
			}
			break;
		case WM_MOUSELEAVE:
			{
				CPoint pt(GET_X_LPARAM(pMsg->lParam),GET_Y_LPARAM(pMsg->lParam));
				if (!m_rcTarget.PtInRect(pt))
				{
					OnTimer(TIMERID_SPAN);// 直接隐藏
				}
			}
			break;
		}
		return DM_ECODE_OK;
	}

	BOOL DMTooltipImpl::PreTranslateMessage(MSG* pMsg)
	{
		if (IsWindow()) 
		{
			RelayEvent(pMsg);
		}
		return FALSE;
	}

	void DMTooltipImpl::ShowTooltip(bool bShow)
	{
		do 
		{
			if (!bShow)
			{
				m_bShowing = false;
				m_rcTarget.SetRect(0,0,0,0);
				m_strTip = L"";
				if (!IsWindowVisible())
				{
					break;
				}
				ShowWindow(SW_HIDE);
				KillTimer(TIMERID_DELAY);
				KillTimer(TIMERID_SPAN);
				break;
			}

			if (m_strTip.IsEmpty())
			{
				break;
			}
			DMAutoDC hdc;
			CRect rcText(0,0,500,1000);
			HFONT oldFont = (HFONT)SelectObject(hdc,m_font);
			::DrawText(hdc,m_strTip,-1,&rcText,DT_CALCRECT|DT_LEFT|DT_WORDBREAK);// 计算文本大小
			SelectObject(hdc,oldFont);
			CRect rcWnd;
			GetWindowRect(&rcWnd);
			rcWnd.right  = rcWnd.left+rcText.right+2*MARGIN_TIP;
			rcWnd.bottom = rcWnd.top+rcText.bottom+2*MARGIN_TIP;
			SetWindowPos(HWND_TOPMOST,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),SWP_NOSENDCHANGING|SWP_SHOWWINDOW|SWP_NOACTIVATE);
		} while (false);
	}

}//namespace DM