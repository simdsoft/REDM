#include "TGPDemoAfx.h" 
#include "TGPMainWnd.h"     
   
#define  WM_HOLDMENU WM_USER+1010
BEGIN_MSG_MAP(CTGPMainWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	MESSAGE_HANDLER_EX(WM_HOLDMENU,OnHoldMenu)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()

BEGIN_EVENT_MAP(CTGPMainWnd)
	EVENT_NAME_COMMAND(L"button_mini", OnMinimize)
	EVENT_NAME_COMMAND(L"button_close",OnClose)
	EVENT_NAME_COMMAND(L"button_menu",OnMenu)
	EVENT_HANDLER(DMEVT_OFPANEL,OnListBoxEx)
END_EVENT_MAP()

BOOL CTGPMainWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{    
	m_CfMenu.LoadMenu(L"LayOut",L"dui_cfmenu");
	return TRUE;
}   

LRESULT CTGPMainWnd::OnHoldMenu(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	return TRUE;
}

DMCode CTGPMainWnd::OnClose()
{
	DestroyWindow(); 
	return DM_ECODE_OK;
}

DMCode CTGPMainWnd::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	return DM_ECODE_OK;
}

DMCode CTGPMainWnd::OnMenu()
{ 
	TGPMenu Menu;
	Menu.LoadMenu(L"LayOut",L"dui_mainmenu");
	CRect rcMenu;
	DUIButton* pMenuBtn = FindChildByNameT<DUIButton>(L"button_menu");
	pMenuBtn->DV_GetWindowRect(rcMenu);
	ClientToScreen(rcMenu);
	POINT pt;
	GetCursorPos(&pt);
	Menu.TrackPopupMenu(0,rcMenu.left-108,rcMenu.bottom+4,m_hWnd);
	return DM_ECODE_OK;
} 

DMCode CTGPMainWnd::OnListBoxEx(DMEventArgs *pEvt)
{
	DMEventOfPanelArgs* pEvtOf = (DMEventOfPanelArgs*)pEvt;
	if (pEvtOf&&pEvtOf->m_pOrgEvt)
	{
		if (IsValidString(pEvtOf->m_pOrgEvt->m_szNameFrom) &&0 ==_wcsicmp(pEvtOf->m_pOrgEvt->m_szNameFrom,L"cf_menu"))
		{
			CRect rcMenu;
			DUIWindow* pMenuBtn = pEvtOf->m_pOrgEvt->m_pSender;
			pMenuBtn->DV_GetWindowRect(rcMenu);
			DUIItemPanel* pPanel = (DUIItemPanel*)pEvtOf->m_pPanel;
			pPanel->OnClientToScreen(rcMenu);
			POINT pt;
			GetCursorPos(&pt);
			m_CfMenu.TrackPopupMenu(TPM_RIGHTALIGN|TPM_TOPALIGN,rcMenu.right,rcMenu.bottom+4,m_hWnd);
			return DM_ECODE_OK;
		}
	}

	return DM_ECODE_OK;
}
