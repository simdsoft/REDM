//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 测试专用demo
// File Des:  
// File Summary: 
// Cur Version: 1.0
// Author:   
// Create Data: 
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-7	1.0			
//--------------------------------------------------------
#include "QQDemoAfx.h"
#include "QQMainWnd.h" 
#include "WndShadow.h"
#include "SkinPreview.h"
#include "SpyFindBtn.h"
#include "IEWnd.h"
#include "DUIWebkit.h"
#include "DMResZipParam.h"
#include "DMTipsImpl.h"
#include "DMScriptImpl.h"

#include <dbghelp.h> 
#include <time.h>
#pragma comment(lib,  "dbghelp.lib")

#if (_MSC_VER==1500)
#if !defined(_WIN64)
#include "vld.h"
#pragma comment(lib, "vld.lib")// VLD仅在mDd模式下才能有效，其他v s版本请使用其他版本vld
#endif
#endif 

LONG __stdcall _UnhandledExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	wchar_t szDir[MAX_PATH] = {0};
	GetRootDirW(szDir,MAX_PATH);
	CStringW  strDumpDir = szDir; 
	time_t  tt = time(NULL);
	tm* ptime = localtime(&tt);
	CStringW strDumpPath;
	strDumpPath.Format(L"%s-%04d-%02d-%02d-%02d-%02d-%02d.dmp",
		strDumpDir,
		1900 + ptime->tm_year,
		(ptime->tm_mon + 1) ,
		ptime->tm_mday,
		ptime->tm_hour,
		ptime->tm_min,
		ptime->tm_sec);
	HANDLE  hFile = CreateFile(strDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{ 
		MINIDUMP_EXCEPTION_INFORMATION   ExInfo; 
		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = ExceptionInfo;
		ExInfo.ClientPointers = NULL;

		BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory,&ExInfo, NULL, NULL);
		CloseHandle(hFile); 
	} 
	return EXCEPTION_EXECUTE_HANDLER;
}

CQQMainWnd* g_pMainWnd = NULL;
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{                      
	SetUnhandledExceptionFilter(&_UnhandledExceptionFilter);
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

	CWndShadow::Initialize(hInstance);
	do 
	{ 
		//DMResZipParam zipres(L"QQRes.zip");
		//theApp.LoadResPack((WPARAM)(&zipres),NULL,L"DMResZipImpl");			// 路径总是相对于生成目录-zip方式
		theApp.LoadResPack((WPARAM)(L"QQRes"),NULL,NULL);						// 路径总是相对于生成目录
		theApp.InitGlobal();													// 初始化指定的全局skin、style、默认字体
		theApp.Register(DMRegHelperT<SkinPreview>(),true);
		theApp.Register(DMRegHelperT<SpyFindBtn>(),true);
		theApp.Register(DMRegHelperT<IERealWnd>(),true);						// 初始化外部控制的Pop RealWnd
		theApp.Register(DMRegHelperT<DUIWebKit>(),true);						// 初始化外部控制的Webkit
		theApp.Register(DMRegHelperT<DMTipsImpl>(),true);                       // 初始化外部的tip类型
		theApp.Register(DMRegHelperT<DMScriptImpl>(),true);						// 初始化外部的脚本实现

		// 增加语言包两种，在widget中可使用菜单切换
		theApp.LoadTrans((WPARAM)L"dmlg_fh");
		theApp.LoadTrans((WPARAM)L"dmlg_en");
		//theApp.SetTrans(L"dmlg_en");                                            // 默认使用dmlg_en语言包

		DMSmartPtrT<CQQMainWnd> pMainWnd;
		pMainWnd.Attach(new CQQMainWnd());
		pMainWnd->DM_CreateWindow(L"dui_mainwnd",0,0,0,0,NULL,false);			// 创建主窗口
		g_pMainWnd =  pMainWnd;
		pMainWnd->SendMessage(WM_INITDIALOG);
		pMainWnd->CenterWindow();
		pMainWnd->ShowWindow(SW_SHOW);
		theApp.Run(pMainWnd->GetSafeHwnd());								    // 运行当前线程的消息循环，并加入消息队列管理中
	} while (false);
	return (int) 0;
}