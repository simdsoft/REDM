#include "DmMainAfx.h"
#include "DMThreadMgr.h"

namespace DM
{
	DMThreadActiveWndTool::DMThreadActiveWndTool()
	{
		m_pLock = new DMLock;
	}

	DMThreadActiveWndTool::~DMThreadActiveWndTool()
	{
		DM_DELETE(m_pLock);
	}

	HWND DMThreadActiveWndTool::SetActiveWnd(HWND hWnd)
	{
		DMAutoLock autolock(m_pLock);
		HWND hLastActiveWnd = GetActiveWnd();
		SetKey(::GetCurrentThreadId(), hWnd);//注意不要用AddKey!，因为要强制改变
		::SetActiveWindow(hWnd);
		return hLastActiveWnd;
	}

	HWND DMThreadActiveWndTool::GetActiveWnd()
	{
		DMAutoLock autolock(m_pLock);
		HWND hWnd = NULL;
		GetObjByKey(::GetCurrentThreadId(), hWnd);
		return hWnd;
	}

}//namespace DM