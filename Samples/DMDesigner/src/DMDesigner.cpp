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
#include <dbghelp.h> 
#pragma comment(lib,  "dbghelp.lib")
#include <chrono>

namespace yasio
{
	// typedefs
	typedef long long highp_time_t;
	typedef std::chrono::high_resolution_clock steady_clock_t;
	typedef std::chrono::system_clock system_clock_t;

	// The high precision nano seconds timestamp
	template <typename _Ty = steady_clock_t> inline long long xhighp_clock()
	{
		auto duration = _Ty::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
	}
	// The high precision micro seconds timestamp
	template <typename _Ty = steady_clock_t> inline long long highp_clock()
	{
		return xhighp_clock<_Ty>() / 1000LL;
	}

#if YASIO__HAS_CXX17
	using std::clamp;
#else
	template <typename _Ty> const _Ty& clamp(const _Ty& v, const _Ty& lo, const _Ty& hi)
	{
		assert(!(hi < lo));
		return v < lo ? lo : hi < v ? hi : v;
	}
#endif

	template <typename _Ty> inline void invoke_dtor(_Ty* p) { p->~_Ty(); }
} // namespace yasio

LONG __stdcall _UnhandledExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
	wchar_t szDir[MAX_PATH] = {0};
	GetRootDirW(szDir,MAX_PATH);
	CStringW  strDumpDir = szDir; 
	time_t  tt = time(NULL);
	tm* ptime = localtime(&tt);
	CStringW strDumpPath;
	strDumpPath.Format(L"%s-%04d-%02d-%02d-%02d-%02d-%02d-dmdesigner.dmp",
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
 
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{     
	SetUnhandledExceptionFilter(&_UnhandledExceptionFilter);
	DMApp theApp(hInstance); 
	Plugin plugin;
	theApp.InstallPlugin(&plugin);
	Scintilla_RegisterClasses(hInstance);


	// 安装plugin，如果有需要!
#ifdef DLL_DMMAIN// lib库下不支持插件
#ifdef _DEBUG
	theApp.LoadPlugins(L"dmpluginsd_design.cfg");
#else
	theApp.LoadPlugins(L"dmplugins_design.cfg");
#endif
#endif//DLL_DMMAIN

	theApp.LoadResPack((WPARAM)(L"DesignerRes"),NULL,NULL);					
	theApp.InitGlobal("ds_global");	

#if   0 
	DMSmartPtrT<LayoutDlg> pMainWnd; 
	pMainWnd.Attach(new LayoutDlg());
	pMainWnd->DoModal(L"ds_layoutdlg",NULL,true);
#else

	// int callCount = 2;
	// while (true) {
	// 
	// 	if (--callCount >= 1) {
	// 		DMSmartPtrT<DMDesignerWnd> pMainWnd;
	// 		pMainWnd.Attach(new DMDesignerWnd());
	// 		auto start = yasio::highp_clock();
	// 		pMainWnd->DM_CreateWindow("ds_mainwnd", 0, 0, 0, 0, NULL, false);			// 创建主窗口
	// 		auto diff = yasio::highp_clock() - start;
	// 
	// 		char buf[128];
	// 		sprintf(buf, "-----------> load cost %lf (ms)\n", diff / 1000.0);
	// 		OutputDebugStringA(buf);
	// 	}
	// 
	// 	Sleep(1000);
	// }
	DMSmartPtrT<DMDesignerWnd> pMainWnd;
	pMainWnd.Attach(new DMDesignerWnd());
	auto start = yasio::highp_clock();
	pMainWnd->DM_CreateWindow("ds_mainwnd", 0, 0, 0, 0, NULL, false);			// 创建主窗口
	auto diff = yasio::highp_clock() - start;

	char buf[128];
	sprintf(buf, "-----------> load cost %lf (ms)\n", diff / 1000.0);
	OutputDebugStringA(buf);
	pMainWnd->SendMessage(WM_INITDIALOG);
	pMainWnd->CenterWindow();
	pMainWnd->ShowWindow(SW_SHOW);
	theApp.Run(pMainWnd->GetSafeHwnd());	
#endif


	theApp.UninstallPlugin(&plugin);
	return (int) 0;
}
