
#include "stdafx.h"
#include "MacroTool.h"
#include "MacroToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMacroToolApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMacroToolApp::CMacroToolApp()
{
	
}

CMacroToolApp theApp;


BOOL CMacroToolApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CMacroToolDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	
	}
	else if (nResponse == IDCANCEL)
	{
		
	}

	return FALSE;
}
