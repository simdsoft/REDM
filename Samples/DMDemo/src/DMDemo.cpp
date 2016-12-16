#include "DMDemoAfx.h"
#include "MainWnd.h"
#include "IEWnd.h"   ///< realwnd由外部自己实现接口传入
#include "DUIRealWnd.h"
#include "DUIWebkit.h"

#if _MSC_VER==1500
#if !defined(_WIN64)
#include "vld.h"
#pragma comment(lib, "vld.lib")// VLD仅在mDd模式下才能有效
#endif
#endif
CMainWnd* g_pMainWnd = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{

	OleInitialize(NULL);
	DMCode iErr = DM_ECODE_OK;
	DMApp theApp(hInstance);

	// 安装plugin，如果有需要!
#ifdef DLL_DMMAIN// lib库下不支持插件
#ifdef _DEBUG
	theApp.LoadPlugins(L"dmpluginsd.cfg");
#else
	theApp.LoadPlugins(L"dmplugins.cfg");
#endif
#endif//DLL_DMMAIN

	iErr = theApp.SetDefRegObj(L"DMResFolderImpl",DMREG_Res);// 设置使用内置的文件夹解析Res方式
	iErr = theApp.LoadResPack((WPARAM)(L"DMRes"),NULL,NULL); // 路径总是相对于生成目录
	iErr = theApp.InitGlobal();								 // 初始化指定的全局skin、style、默认字体
	iErr = theApp.Register(DMRegHelperT<IERealWnd>(),true);  // 初始化外部控制的Pop RealWnd
	//iErr = theApp.Register(DMRegHelperT<DUIRealWnd>(),true); // 初始化外部控制的Child RealWnd
	iErr = theApp.Register(DMRegHelperT<DUIWebKit>(),true);  // 初始化外部控制的Webkit

	DMSmartPtrT<CMainWnd> pMainWnd;
	if (DMSUCCEEDED(iErr))
	{
		pMainWnd.Attach(new CMainWnd());
		g_pMainWnd = pMainWnd;
		pMainWnd->DM_CreateWindow(L"dui_main",0,0,0,0,NULL,false);				// 创建主窗口
		pMainWnd->SendMessage(WM_INITDIALOG);
		pMainWnd->CenterWindow();
		pMainWnd->ShowWindow(SW_SHOW);

		theApp.Run(pMainWnd->GetSafeHwnd());				// 运行当前线程的消息循环，并加入消息队列管理中
	}
	return (int) 0;
}