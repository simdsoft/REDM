// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMDesigner.h
// File mark:    
// File summary:入口   
// Author:		guoyouhuang
// Edition:     1.0 
// Create date: 2016-7-7
// ----------------------------------------------------------------
#include "DMDesignerAfx.h"     
#include "Plugin.h" 
 
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{     
	OleInitialize(NULL);  
	DMApp theApp(hInstance); 

	Plugin plugin;
	theApp.InstallPlugin(&plugin);

	theApp.LoadResPack((WPARAM)(L"DesignerRes"),NULL,NULL);					
	theApp.InitGlobal(L"ds_global");	

#if   0 
	DMSmartPtrT<LayoutDlg> pMainWnd; 
	pMainWnd.Attach(new LayoutDlg());
	pMainWnd->DoModal(L"ds_layoutdlg",NULL,true);
#else
	DMSmartPtrT<DMDesignerWnd> pMainWnd;
	pMainWnd.Attach(new DMDesignerWnd());
	pMainWnd->DM_CreateWindow(L"ds_mainwnd",0,0,0,0,NULL,false);			// 创建主窗口
	pMainWnd->SendMessage(WM_INITDIALOG);
	pMainWnd->CenterWindow();
	pMainWnd->ShowWindow(SW_SHOW);
	theApp.Run(pMainWnd->GetSafeHwnd());	
#endif


	theApp.UninstallPlugin(&plugin);
	return (int) 0;
}
