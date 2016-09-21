#include "DmMainAfx.h"
#include "DMDragWnd.h"

namespace DM
{
	DMDragWnd * DMDragWnd:: ms_pCurDragWnd = NULL;
	DMDragWnd::DMDragWnd(void)
	{
	}

	DMDragWnd::~DMDragWnd()
	{
	}

	//---------------------------------------------------
	// Function Des: 接口
	bool DMDragWnd::DragBegin(IDMCanvas*pCanvas,POINT ptHot,COLORREF crKey, BYTE byAlpha,DWORD dwFlags)
	{
		bool bRet = false;
		do 
		{
			if (NULL == pCanvas)
			{// 没有拖动时的临时画布
				break;
			}
			CSize sz;
			if (!DMSUCCEEDED(pCanvas->GetSize(sz)))
			{
				break;
			}
			if (0 == sz.cx||0 == sz.cy)
			{
				break;
			}
			if (ms_pCurDragWnd)
			{// 正在拖动中！
				break;
			}

		    ms_pCurDragWnd = new DMDragWnd;
			ATOM Atom = g_pDMApp->GetClassAtom(false);  // 是否使用阴影窗口类创建
			HWND hWnd = ((DMCWnd*)ms_pCurDragWnd)->CreateWindowEx((LPCWSTR)Atom,NULL,WS_POPUP,WS_EX_TRANSPARENT|WS_EX_TOOLWINDOW|WS_EX_TOPMOST,0,0,sz.cx,sz.cy,NULL,NULL);
			if(NULL == hWnd)
			{
				DM_DELETE(ms_pCurDragWnd);
				break;
			}
			ms_pCurDragWnd->ModifyStyleEx(0,WS_EX_LAYERED);
			HDC hdc    = ms_pCurDragWnd->GetDC();
			HDC dcMem  = pCanvas->GetDC();

			BLENDFUNCTION bf = {AC_SRC_OVER,0,byAlpha,AC_SRC_ALPHA};
			ms_pCurDragWnd->UpdateLayeredWindow(hdc,&CPoint(0,0),&sz,dcMem,&CPoint(0,0),crKey,&bf,LWA_ALPHA);

			pCanvas->ReleaseDC(dcMem);
			ms_pCurDragWnd->ReleaseDC(hdc);
			ms_pCurDragWnd->m_ptHot   = ptHot;
			bRet = true;
		} while (false);
		return bRet;
	}

	void DMDragWnd::DragMove(POINT pt)
	{
		if (ms_pCurDragWnd)
		{
			ms_pCurDragWnd->SetWindowPos(HWND_TOPMOST,pt.x-ms_pCurDragWnd->m_ptHot.x,pt.y-ms_pCurDragWnd->m_ptHot.y,0,0,SWP_NOSIZE|SWP_NOSENDCHANGING|SWP_NOOWNERZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);
		}
	}

	void DMDragWnd::DragEnd()
	{
		if (ms_pCurDragWnd)
		{
			ms_pCurDragWnd->DestroyWindow();
			DM_DELETE(ms_pCurDragWnd);
		}
	}

}//namespace DM