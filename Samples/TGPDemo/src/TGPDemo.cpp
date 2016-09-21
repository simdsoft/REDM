//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.      
//        
// File Name: TGP测试专用demo
// File Des:      
// File Summary:           
// Cur Version: 1.0 
// Author:
// Create Data: 
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2016-4-21	1.0			
//--------------------------------------------------------
#include "TGPDemoAfx.h" 
#include "TGPMainWnd.h"  
#include "TGPListBoxEx.h"
#include "TGPTipsImpl.h"
#include "TGPComboboxEx.h" 
#include "TGPLoginWnd.h"
#include "DMResZipParam.h"

CTGPMainWnd* g_pMainWnd = NULL;
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{                       
	OleInitialize(NULL);
	DMApp theApp(hInstance);   
	// 安装plugin，如果有需要! 
#ifdef DLL_DMMAIN// lib库下不支持插件
#ifdef _DEBUG
	theApp.LoadPlugins(L"dmpluginsd.cfg");
#else
	theApp.LoadPlugins(L"dmplugins.cfg");
#endif
#endif//DLL_DMMAIN
 
	do  
	{  
#ifdef DLL_DMMAIN// DLL下演示插件支持多zip提前加载，提前释放----------------------------------------------------------
		//1. login窗口登陆 
		DMResZipParam ziploginres(L"TGPRes\\zips\\zip_login.zip"); 
		theApp.LoadResPack((WPARAM)(&ziploginres),(LPARAM)L"loginzip",L"DMResMultZipImpl");		///< 注册loginzip为key的zip包

		// main窗口资源可以提前加入，当然为了节省，内存你也可以在使用时再加入
		DMResZipParam zipres(L"TGPRes\\zips\\zip_main.zip");
		theApp.LoadResPack((WPARAM)(&zipres),(LPARAM)L"mainzip",L"DMResMultZipImpl");			///< 注册mainzip为key的zip包

		theApp.InitGlobal(L"global_login");	
		DMSmartPtrT<CTGPLoginWnd> pLoginWnd; 
		pLoginWnd.Attach(new CTGPLoginWnd());
		pLoginWnd->DM_CreateWindow(L"dui_loginwnd",0,0,0,0,NULL,false);							///< 创建登陆窗口
		pLoginWnd->SendMessage(WM_INITDIALOG);
		pLoginWnd->CenterWindow();
		pLoginWnd->ShowWindow(SW_SHOW);
		theApp.Run(pLoginWnd->GetSafeHwnd());													///< 此函数执行后，退出登陆窗口

		DMSmartPtrT<IDMRes> pRes;
		theApp.GetDefRegObj((void**)&pRes, DMREG_Res);											///< 得到TGPResMultZipImpl对象
		pRes->SendExpandInfo(NULL,(LPARAM)L"loginzip");											///< 在扩展中释放loginzip包
#else
		theApp.LoadResPack((WPARAM)(L"TGPRes"),NULL,NULL);										///< 路径总是相对于生成目录
#endif//DLL_DMMAIN


		//2.主窗口
		theApp.InitGlobal();																	///< 初始化指定的全局skin、style、默认字体
		theApp.Register(DMRegHelperT<TGPListBoxEx>(),true);
		theApp.Register(DMRegHelperT<TGPTipsImpl>(),true);										///< 初始化外部的tip类型
		theApp.Register(DMRegHelperT<TGPComboboxEx>(),true);

		DMSmartPtrT<CTGPMainWnd> pMainWnd;
		pMainWnd.Attach(new CTGPMainWnd());
		pMainWnd->DM_CreateWindow(L"dui_mainwnd",0,0,0,0,NULL,false);							///< 创建主窗口
		g_pMainWnd =  pMainWnd;
		pMainWnd->SendMessage(WM_INITDIALOG);
		pMainWnd->CenterWindow();
		pMainWnd->ShowWindow(SW_SHOW);
		theApp.Run(pMainWnd->GetSafeHwnd());													///< 运行当前线程的消息循环，并加入消息队列管理中
	} while (false);
	return (int) 0;
}