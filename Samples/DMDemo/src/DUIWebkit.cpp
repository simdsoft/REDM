#include "DMDemoAfx.h"
#include "DUIWebkit.h"

DUIWebkitHelper::DUIWebkitHelper()
{
	m_hDMWebkit = LoadLibrary(_T("DMWebkit.dll"));
	if (m_hDMWebkit)
	{
		m_funDMInit				= (FunDMInit)GetProcAddress(m_hDMWebkit,"DMInit");
		m_funDMShutdown			= (FunDMShutdown)GetProcAddress(m_hDMWebkit,"DMShutdown");
		m_funDMCreateWebView	= (FunDMCreateWebView)GetProcAddress(m_hDMWebkit,"DMCreateWebView");
		m_funDMDestroyWebView	= (FunWkeDestroyWebView)GetProcAddress(m_hDMWebkit,"DMDestroyWebView");
		if (NULL == m_funDMInit
			||NULL == m_funDMShutdown
			||NULL == m_funDMCreateWebView
			||NULL == m_funDMDestroyWebView)
		{
			FreeLibrary(m_hDMWebkit);
			m_hDMWebkit = NULL;
		}
		else
		{
			m_funDMInit();// 初始化
		}
	}
}

DUIWebkitHelper::~DUIWebkitHelper()
{
	if (m_hDMWebkit)
	{
		m_funDMShutdown();
		//FreeLibrary(m_hDMWebkit);// 由系统自己释放，不然可能退出时机不对，引发崩溃
		m_hDMWebkit = NULL;
	}
}

//------------------------------------------------------------------
DUIWebKit::DUIWebKit(void)
{
	Init();
}

DUIWebKit::~DUIWebKit(void)
{
	UnInit();
}

void DUIWebKit::Init()
{
	m_pWebView = NULL;
	m_byAlpha  = 0xFF;
	m_bEnableZoom= true;
	m_pHelper = DUIWebkitHelper::getSingletonPtr();
	if (NULL == m_pHelper)
	{
		m_pHelper.Attach(new DUIWebkitHelper);
		if (m_pHelper->m_hDMWebkit)
		{
			m_pHelper->SetRefCount(1);
		}
		else
		{
			m_pHelper.Release();
		}
	}
}

void DUIWebKit::UnInit()
{
	m_pHelper.Release();
}

//---------------------------------------------------
// Function Des: 对外接口
//---------------------------------------------------
IDMWebViewPtr DUIWebKit::GetWebView()
{
	return m_pWebView;
}

//---------------------------------------------------
// Function Des: DUI的消息分发系列函数
//---------------------------------------------------
int DUIWebKit::OnCreate(LPVOID)
{
	int iRet = 0;
	do 
	{
		if (NULL == m_pHelper)
		{
			iRet = 1;
			break;
		}
		typedef IDMWebViewPtr (*FunDMCreateWebView)();
		m_pWebView = m_pHelper->m_funDMCreateWebView();
		if (NULL == m_pWebView)
		{
			iRet = 1;
			m_pHelper.Release();
			break;
		}

		m_pWebView->setDUIWndHost(this);
		m_pWebView->loadURL(m_strUrl);
		g_pDMApp->AddIdleHandler(this);
		DM_SetTimer(TM_TICKER,70); // 由于timer不够及时，idle又限制了只在当前的消息循环中有效，使用timer和onidle一起更新浏览器
	} while (false);
	return iRet;
}

void DUIWebKit::OnDestroy()
{
	do 
	{
		g_pDMApp->RemoveIdleHandler(this);
		DM_KillTimer(TM_TICKER);
		if (m_pHelper)
		{
			m_pHelper->m_funDMDestroyWebView(m_pWebView);
			m_pWebView = NULL;
		}
	} while (false);
}

void DUIWebKit::DM_OnPaint(IDMCanvas* pCanvas)
{
	do 
	{
		CRect rcClip;
		pCanvas->GetClipBox(&rcClip);
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		CRect rcInvalid;
		rcInvalid.IntersectRect(&rcClip,&rcClient);

		HDC hdc = pCanvas->GetDC();
		if (0xFF!=m_byAlpha)
		{
			BLENDFUNCTION bf={AC_SRC_OVER,0,m_byAlpha,AC_SRC_ALPHA};
			::AlphaBlend(hdc,rcInvalid.left,rcInvalid.top,rcInvalid.Width(),rcInvalid.Height(),m_pWebView->getViewDC(),rcInvalid.left-rcClient.left,rcInvalid.top-rcClient.top,rcInvalid.Width(),rcInvalid.Height(),bf);
		}
		else
		{
			::BitBlt(hdc,rcInvalid.left,rcInvalid.top,rcInvalid.Width(),rcInvalid.Height(),m_pWebView->getViewDC(),rcInvalid.left-rcClient.left,rcInvalid.top-rcClient.top,SRCCOPY);     
		}
		pCanvas->ReleaseDC(hdc);
	} while (false);
}

void DUIWebKit::OnSize(UINT nType, CSize size)
{
	do 
	{
		__super::OnSize(nType,size);
		if (NULL == m_pWebView||NULL == DUIWebkitHelper::getSingletonPtr()->m_hDMWebkit)
		{
			break;
		}
		m_pWebView->resize(size.cx,size.cy);
		m_pWebView->tick();
	} while (false);
}

LRESULT DUIWebKit::OnMouseEvent(UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	do 
	{
		if (WM_LBUTTONDOWN == uMsg
			||WM_MBUTTONDOWN == uMsg
			||WM_RBUTTONDOWN == uMsg)
		{
			DV_SetFocusWnd();
			DM_SetCapture();
		}
		else if(WM_LBUTTONUP == uMsg
			||WM_MBUTTONUP == uMsg
			||WM_RBUTTONUP == uMsg)
		{
			DM_ReleaseCapture();
		}

		CRect rcClient;
		DV_GetClientRect(&rcClient);
		int x = GET_X_LPARAM(lParam)-rcClient.left;
		int y = GET_Y_LPARAM(lParam)-rcClient.top;

		unsigned int flags = 0;

		if (wParam & MK_CONTROL)flags |= DM_CONTROL;
		if (wParam & MK_SHIFT)  flags |= DM_SHIFT;
		if (wParam & MK_LBUTTON)flags |= DM_LBUTTON;
		if (wParam & MK_MBUTTON)flags |= DM_MBUTTON;
		if (wParam & MK_RBUTTON)flags |= DM_RBUTTON;

		BOOL bHandled = (true == m_pWebView->mouseEvent(uMsg, x, y, flags));
		SetMsgHandled(bHandled);
	} while (false);

	return 0;
}

LRESULT DUIWebKit::OnKeyDown( UINT uMsg, WPARAM wParam,LPARAM lParam )
{
	do 
	{
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)	  flags |= DM_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED) flags |= DM_EXTENDED;

		BOOL bHandled = (true == m_pWebView->keyDown((unsigned int)wParam, flags, false));
		SetMsgHandled(bHandled);

	} while (false);
	return 0;
}

LRESULT DUIWebKit::OnKeyUp(UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	do 
	{
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)  flags |= DM_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)flags |= DM_EXTENDED;
		BOOL bHandled = (true == m_pWebView->keyUp((unsigned int)wParam, flags, false));
		SetMsgHandled(bHandled);
	} while (false);
	return 0;
}


static int s_currentZoom = 100;
static int s_zoomLevels[] = {30, 50, 67, 80, 90, 100, 110, 120, 133, 150, 170, 200, 240, 300};
void DUIWebKit::Zoom(bool zoomIn)
{
	do 
	{
		int count = sizeof(s_zoomLevels) / sizeof(int);
		int i = 0;
		for (i = 0; i < count; ++i)
		{
			if (s_zoomLevels[i] == s_currentZoom)
			{
				break;
			}
		}

		if (zoomIn)
		{
			i = i+1;
		}
		else
		{
			i = i-1;
		}

		if (i<0)
		{
			i = 0;
		}

		if (i>=count)
		{
			i = count-1;
		}

		s_currentZoom = s_zoomLevels[i];
		m_pWebView->setZoomFactor(s_currentZoom/100.f);
	} while (false);
}

LRESULT DUIWebKit::OnMouseWheel(UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	LRESULT lr = 0;
	do 
	{
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (wParam & MK_CONTROL
			&& m_bEnableZoom)
		{
			if (delta<0)
			{
				Zoom(false);
			}
			else
			{
				Zoom(true);
			}
			break;// 使用ctrl放大.缩小,不再滚动了
		}

		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);



		unsigned int flags = 0;

		if (wParam & MK_CONTROL)flags |= DM_CONTROL;
		if (wParam & MK_SHIFT)  flags |= DM_SHIFT;
		if (wParam & MK_LBUTTON)flags |= DM_LBUTTON;
		if (wParam & MK_MBUTTON)flags |= DM_MBUTTON;
		if (wParam & MK_RBUTTON)flags |= DM_RBUTTON;

		BOOL handled = (true == m_pWebView->mouseWheel(pt.x, pt.y, delta, flags));
		SetMsgHandled(handled);
	} while (false);
	return lr;
}

LRESULT DUIWebKit::OnChar(UINT uMsg, WPARAM wParam,LPARAM lParam)
{
	do 
	{
		unsigned int charCode = (unsigned int)wParam;
		unsigned int flags = 0;
		if (HIWORD(lParam) & KF_REPEAT)  flags |= DM_REPEAT;
		if (HIWORD(lParam) & KF_EXTENDED)flags |= DM_EXTENDED;

		SetMsgHandled(m_pWebView->keyPress(charCode, flags, false));
	} while (false);
	return 0;
}

LRESULT DUIWebKit::OnImeStartComposition( UINT uMsg, WPARAM wParam,LPARAM lParam )
{
	do 
	{
		DMRect caret = m_pWebView->getCaret();

		CRect rcClient;
		DV_GetClientRect(&rcClient);

		CANDIDATEFORM form;
		form.dwIndex		= 0;
		form.dwStyle		= CFS_EXCLUDE;
		form.ptCurrentPos.x = caret.x + rcClient.left;
		form.ptCurrentPos.y = caret.y + caret.h + rcClient.top;
		form.rcArea.top		= caret.y + rcClient.top;
		form.rcArea.bottom  = caret.y + caret.h + rcClient.top;
		form.rcArea.left	= caret.x + rcClient.left;
		form.rcArea.right	= caret.x + caret.w + rcClient.left;
		COMPOSITIONFORM compForm;
		compForm.ptCurrentPos= form.ptCurrentPos;
		compForm.rcArea		 = form.rcArea;
		compForm.dwStyle	 = CFS_POINT;

		HWND hWnd = GetContainer()->OnGetHWnd();
		HIMC hIMC = ImmGetContext(hWnd);
		ImmSetCandidateWindow(hIMC, &form);
		ImmSetCompositionWindow(hIMC,&compForm);
		ImmReleaseContext(hWnd, hIMC);
	} while (false);
	return 0;
}

void DUIWebKit::DM_OnSetFocus()
{
	do 
	{
		m_pWebView->focus();
	} while (false);
}

void DUIWebKit::DM_OnKillFocus()
{
	do 
	{
		m_pWebView->unfocus();
	} while (false);
}

void DUIWebKit::OnDUITimer(char id)
{
	do 
	{
		if (TM_TICKER == id)
		{
			m_pWebView->tick();
		}
	} while (false);
}

void DUIWebKit::DUIWndUpdate(const HDC hdc,int x, int y, int cx, int cy)
{
	do 
	{
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		CRect rcInvalid(CPoint(x,y),CSize(cx,cy));
		rcInvalid.OffsetRect(rcClient.TopLeft());
		DM_InvalidateRect(rcInvalid);
	} while (false);
}

DMCode DUIWebKit::DV_OnSetCursor(const CPoint &pt)
{
	return DM_ECODE_OK;
}

BOOL DUIWebKit::OnIdle()
{
	do 
	{
		m_pWebView->tick();
	} while (false);
	return TRUE;
}

bool DUIWebKit::OnAttrUrl(LPCWSTR lpszValue, bool bLoadXml)
{
	m_strUrl = lpszValue;
	if (!bLoadXml) 
	{	
		if (NULL == m_pWebView||NULL == m_pHelper)
		{
		}
		else
		{
			m_pWebView->loadURL(m_strUrl);
		}
	}
	return !bLoadXml;
}

