#include "StdAfx.h"
#include "MainWnd.h"

BEGIN_MSG_MAP(CMainWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
BEGIN_EVENT_MAP(CMainWnd)
	EVENT_NAME_COMMAND(L"closebutton",OnClose)
	EVENT_NAME_COMMAND(L"maxbutton",OnMaximize)
	EVENT_NAME_COMMAND(L"restorebutton",OnRestore)
	EVENT_NAME_COMMAND(L"minbutton", OnMinimize)
	EVENT_NAME_COMMAND(L"convert_btn", OnConvert)
END_EVENT_MAP()

BOOL CMainWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_pEdit = FindChildByNameT<DUIRichEdit>(L"dest_edit");


	// 加载三份消息宏
	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\DMMsgCrack.h",szPath,MAX_PATH);
	m_MacroHelper.LoadFile(szPath,L"#define DECLARE_MESSAGE_MAP()");

	memset(szPath,0,MAX_PATH);
	GetRootFullPath(L".\\DUIMsgCrack.h",szPath,MAX_PATH);
	m_MacroHelper.LoadFile(szPath,L"#define DM_BEGIN_MSG_MAP()");

	memset(szPath,0,MAX_PATH);
	GetRootFullPath(L".\\DMXmlCrack.h",szPath,MAX_PATH);
	m_MacroHelper.LoadFile(szPath,L"#define DM_BEGIN_ATTRIBUTES()");


	return TRUE;
}

void CMainWnd::OnSize(UINT nType, CSize size)
{
	if (0 != size.cx&&0 != size.cy)
	{
		if (SIZE_MAXIMIZED == nType)
		{
			FindChildByName(L"maxbutton")->DM_SetVisible(false);
			FindChildByName(L"restorebutton")->DM_SetVisible(true);
		}
		else if (SIZE_RESTORED == nType)
		{
			FindChildByName(L"maxbutton")->DM_SetVisible(true);
			FindChildByName(L"restorebutton")->DM_SetVisible(false);
		}
	}
	SetMsgHandled(FALSE);  // 由DMHWnd继续处理OnSize消息
}

DMCode CMainWnd::OnClose()
{
	DestroyWindow(); 
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnRestore()
{
	SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	return DM_ECODE_OK;
}


DMCode CMainWnd::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnConvert()
{
	CStringW strMacro = m_pEdit->GetWindowText();
	if (!strMacro.IsEmpty()&&0 != m_MacroHelper.m_VecMacro.size())
	{
		m_pEdit->SetWindowText(m_MacroHelper.Convert(strMacro));
	}

	return DM_ECODE_OK;
}
