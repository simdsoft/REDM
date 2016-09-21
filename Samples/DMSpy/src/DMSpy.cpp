#include "DMSpyAfx.h"
#include "MainWnd.h"
#include "DUIFindBtn.h"
DUIWindow* g_pMainWnd = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	OleInitialize(NULL);
	DMCode iErr = DM_ECODE_OK;
	DMApp theApp(hInstance);
	iErr = theApp.SetDefRegObj(L"DMResFolderImpl",DMREG_Res);// 设置使用内置的文件夹解析Res方式
	iErr = theApp.LoadResPack((WPARAM)(L"DMRes"),NULL,NULL); // 路径总是相对于生成目录
	iErr = theApp.InitGlobal();								 // 初始化指定的全局skin、style、默认字体
	iErr = theApp.Register(DMRegHelperT<DUIFindBtn>(),true);
	DMSmartPtrT<CMainWnd> pMainWnd;
	if (DMSUCCEEDED(iErr))
	{
		pMainWnd.Attach(new CMainWnd());
		pMainWnd->DM_CreateWindow(L"dui_spy");				// 创建主窗口
		pMainWnd->SendMessage(WM_INITDIALOG);
		pMainWnd->CenterWindow();
		pMainWnd->ShowWindow(SW_SHOW);
		g_pMainWnd = pMainWnd;

		theApp.Run(pMainWnd->GetSafeHwnd());				// 运行当前线程的消息循环，并加入消息队列管理中
		g_pMainWnd = NULL;
	}
	return (int) 0;
}