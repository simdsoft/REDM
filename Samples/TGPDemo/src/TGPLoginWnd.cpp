#include "TGPDemoAfx.h"
#include "TGPLoginWnd.h"

BEGIN_MSG_MAP(CTGPLoginWnd)
	MSG_WM_INITDIALOG(OnInitDialog)  
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
 
BEGIN_EVENT_MAP(CTGPLoginWnd)
	EVENT_NAME_COMMAND(L"button_mini", OnMinimize)  
	EVENT_NAME_COMMAND(L"button_close",OnClose)
END_EVENT_MAP()


BOOL CTGPLoginWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{    
	return TRUE; 
}   

DMCode CTGPLoginWnd::OnClose()
{
	DestroyWindow(); 
	return DM_ECODE_OK;
}

DMCode CTGPLoginWnd::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	return DM_ECODE_OK;
}
