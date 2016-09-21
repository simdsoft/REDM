#include "DMSpyAfx.h"
#include "DUIFindBtn.h"
#include "resource.h"
#include "MainWnd.h"
#include <Tlhelp32.h>

HWND g_hWnd = NULL;								// 目标窗口
DWORD g_pId = 0;                                // 目标进程ID
int g_nHex  = 1;								// 标志"16进制"是否被选中

DUIFindBtn::DUIFindBtn()
{
	bLButtonDown = false;
	DMADDEVENT(DMSpyInitArgs::EventID);//
}

DUIFindBtn::~DUIFindBtn()
{
	
}

void DUIFindBtn::OnLButtonDown(UINT nFlags,CPoint pt)
{
	do 
	{
		bLButtonDown = true;
		CMainWnd* pMainWnd = ((CMainWnd*)g_pMainWnd);
		pMainWnd->DM_KillTimer(10);
		DM_SetCapture();
		DM_ModifyState(DUIWNDSTATE_PushDown, 0, true);
		HCURSOR hCursor = ::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_FIND_CURSOR));
		::SetCursor(hCursor);
		GetContainer()->OnRegisterTimeline(this);
	} while (false);
}

void DUIFindBtn::OnLButtonUp(UINT nFlags,CPoint pt)
{
	bLButtonDown = false;
	DM_ReleaseCapture();
	DM_ModifyState(0, DUIWNDSTATE_PushDown,true);
	HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
	::SetCursor(hCursor);
	GetContainer()->OnUnregisterTimeline(this);

	// todo.----------------------------
	InitCurHWnd();

	// 将信息发给主窗口
	DMSpyInitArgs Evt(this);
	Evt.m_hwnd = g_hWnd;
	DV_FireEvent(Evt);
}

void DUIFindBtn::OnMouseLeave()
{
	// 不恢复状态
}

void DUIFindBtn::OnDestroy()
{
	GetContainer()->OnUnregisterTimeline(this);
}

DMCode DUIFindBtn::DV_OnSetCursor(const CPoint &pt)
{
	if (bLButtonDown)
	{
		HCURSOR hCursor = ::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_FIND_CURSOR));
		::SetCursor(hCursor);
	}
	else
	{
		HCURSOR hCursor = ::LoadCursor(NULL, IDC_ARROW);
		::SetCursor(hCursor);
	}
	
	return DM_ECODE_OK;
}

DMCode DUIFindBtn::OnTimeline()
{
	static int ncount = 40;
	do 
	{
		RECT rc;
		CMainWnd* pMainWnd = ((CMainWnd*)g_pMainWnd);
		if (pMainWnd->m_hRootWnd == InitCurHWnd()&&pMainWnd->m_bInit)
		{
			pMainWnd->HitTest(rc);
		}
		else
		{
			::GetWindowRect(g_hWnd, &rc);
			if (rc.left<0)
			{
				rc.left = 0;
			}

			if (rc.top<0)
			{
				rc.top = 0;
			}
		}

		ncount--;
		if (ncount!=1)
		{
			break;
		}

		HWND DeskHwnd = ::GetDesktopWindow();//取得桌面句柄
		HDC DeskDC    = ::GetWindowDC(DeskHwnd);
		int oldRop2   = SetROP2(DeskDC, R2_NOTXORPEN);
	
	

		HPEN newPen = ::CreatePen(PS_DASHDOTDOT, 1, RGB(125, 0, 125));
		HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);
		::Rectangle(DeskDC, rc.left, rc.top,rc.right, rc.bottom);
		Sleep(400); //设置闪烁时间间隔
		::Rectangle(DeskDC, rc.left, rc.top,rc.right, rc.bottom);

		::SetROP2(DeskDC, oldRop2);
		::SelectObject(DeskDC, oldPen);
		::DeleteObject(newPen);
		::ReleaseDC(DeskHwnd, DeskDC);
		DeskDC = NULL;
		ncount = 40;

	} while (false);
	return DM_ECODE_OK;
}

// 辅助
void DUIFindBtn::RefreshFindInfo(HWND hWnd)
{
	DUIEdit *pHwndEdit = g_pMainWnd->FindChildByNameT<DUIEdit>(L"edithwnd");
	DUIEdit *pPidEdit  = g_pMainWnd->FindChildByNameT<DUIEdit>(L"editpid");
	DUIEdit *pPathEdit = g_pMainWnd->FindChildByNameT<DUIEdit>(L"editpath");
	if (NULL == pHwndEdit
		||NULL == pPidEdit
		||NULL == pPathEdit)
	{
		DMASSERT(0);
	}
	pHwndEdit->SetWindowText(Display((int)hWnd));
	GetWindowThreadProcessId(hWnd, &g_pId);
	pPidEdit->SetWindowText(Display((int)g_pId));
	pPathEdit->SetWindowText(GetProcPath(g_pId));
}

CStringT DUIFindBtn::Display(int nVal)
{
	CStringT str;
	if (1 == g_nHex)
	{
		str.Format(_T("0x%x"), nVal);
		str.MakeUpper();
	}
	else
	{
		str.Format(_T("%d"), nVal);
	}
	return str;
}

CStringT DUIFindBtn::GetProcPath(int PID)
{
	HANDLE hModule = NULL;
	hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	MODULEENTRY32 *minfo = new MODULEENTRY32;
	minfo->dwSize = sizeof(MODULEENTRY32);
	CStringT str;
	if (Module32First(hModule, minfo))
	{
		str.Format(_T("%s"), minfo->szExePath);
		CloseHandle(hModule);
		if (minfo)
		{
			delete minfo;
		}
	}
	else
	{
		str.Format(_T("无法取得路径,err:%d"), GetLastError());
	}

	return str;
}

HWND DUIFindBtn::InitCurHWnd()
{
	POINT pnnt;
	::GetCursorPos(&pnnt);
	g_hWnd = ::WindowFromPoint(pnnt);
	HWND grayHwnd = ::GetWindow(g_hWnd, GW_CHILD);
	RECT tempRc;
	BOOL bFind = FALSE;
	while (grayHwnd)
	{
		::GetWindowRect(grayHwnd, &tempRc);
		if (::PtInRect(&tempRc, pnnt))
		{
			bFind = TRUE;
			break;
		}
		else
		{
			grayHwnd = ::GetWindow(grayHwnd, GW_HWNDNEXT);
		}
	}
	if (bFind)
	{
		bFind = FALSE;
		g_hWnd = grayHwnd;
	}
	RefreshFindInfo(g_hWnd);
	return g_hWnd;
}