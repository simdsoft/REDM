#include "QQDemoAfx.h"
#include "SkinWnd.h"
#include "QQMainWnd.h"
#include "SkinPreview.h"
#include <commdlg.h>

BEGIN_MSG_MAP(CSkinWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_SIZE(OnSize)
	MSG_WM_SHOWWINDOW(OnShowWindow)
	CHAIN_MSG_MAP(DMHWnd)
END_MSG_MAP()
BEGIN_EVENT_MAP(CSkinWnd)
	EVENT_NAME_COMMAND(L"skin_closebtn",OnClose)
	EVENT_NAME_COMMAND(L"skin_outbtn",OnOutSkin)
END_EVENT_MAP()


CSkinWnd::CSkinWnd(CQQMainWnd*pOwner)
{
	memset(m_szCurThemes, 0,200);
	m_pMainWnd = pOwner;
}

BOOL CSkinWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// 这里初始化子项
	DUIWindow* pTab = FindChildByNameT<DUIWindow>(L"tablayout");  // 主题
	DMASSERT(pTab);
	DMSmartPtrT<IDMRes> pRes;
	g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
	if (0 == _wcsicmp(L"DMResFolderImpl",pRes->V_GetClassName())
		||0 == _wcsicmp(L"DMResZipImpl",pRes->V_GetClassName()))
	{
		int  iAllThemeSize = NULL;
		pRes->SendExpandInfo((WPARAM)&iAllThemeSize,NULL);
		DMBufT<byte>pBuf; pBuf.Allocate(iAllThemeSize);
		pRes->SendExpandInfo((WPARAM)(LPVOID)pBuf, (LPARAM)iAllThemeSize);
		CStringW strValue = (wchar_t*)pBuf.get();
		CStringWList strUpdateList;
		int nCount = SplitStringT(strValue,L';',strUpdateList);
		CStringW strType = L"png";CStringW strResName=L"preview";
		for (int i=0;i<nCount;i++)
		{
			unsigned long ulSize = 0;
			CStringW strTemp = strUpdateList[i];
			if (DMSUCCEEDED(pRes->GetItemSize(strType,strResName,ulSize,strTemp)))
			{
				DMBufT<byte>pBuf;pBuf.Allocate(ulSize);
				if (DMSUCCEEDED(pRes->GetItemBuf(strType,strResName, pBuf, ulSize,strTemp)))
				{
					// 创建预览skin
					DMSmartPtrT<IDMSkin> pSkin;
					g_pDMApp->CreateRegObj((void**)&pSkin,L"imglist",DMREG_Skin);
					pSkin->SetBitmap(pBuf,ulSize,strType);
				
					SkinPreview *pChild = NULL;
					g_pDMApp->CreateRegObj((void**)&pChild, L"SkinPreview",DMREG_Window);
					if (pChild)
					{
						pChild->m_pSkin = pSkin;
						pChild->m_strThemeName = strTemp;
						pChild->SetAttribute(L"tiptext",strTemp);
						pChild->SetAttribute(L"cursor",L"hand");
						pTab->DM_InsertChild(pChild);
					}
				}
			}
		}
		pTab->DV_UpdateChildLayout();
	} 

	return TRUE;
}

int CSkinWnd::OnCreate(LPVOID)
{
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetPosition(0, 0);
	SetMsgHandled(FALSE);
	return 0;
}

DMCode CSkinWnd::OnOutSkin()
{
	do 
	{
		LPWSTR lpszPath = L".\\QQRes\\outskin\\";
		wchar_t szPath[MAX_PATH] = {0};
		GetRootFullPath(lpszPath,szPath,MAX_PATH);

		wchar_t szFileTitle[_MAX_FNAME] = {0};  
		wchar_t szFileName[_MAX_PATH]   = {0};    
		OPENFILENAMEW ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = _MAX_PATH;
		ofn.lpstrDefExt = L"gif";
		ofn.lpstrInitialDir = szPath;
		ofn.lpstrFileTitle = (LPTSTR)szFileTitle;
		ofn.nMaxFileTitle = _MAX_FNAME;
		ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING| OFN_NOCHANGEDIR;
		ofn.lpstrFilter = L"图像文件(*.bmp;*.jpg;*.png)\0*.bmp;*.jpg;*.png\0\0";
		ofn.hwndOwner = m_hWnd;
		if (::GetOpenFileNameW(&ofn))
		{// todo.hgy413 note:GetOpenFileNameW点击后WM_LBUTTTONUP消息会发送给dui，所以不要随意只处理WM_LBUTTONUP
			DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"bg");
			if (pSkin)
			{
				DMSmartPtrT<IDMBitmap> pBitmap;
				pSkin->GetBitmap(&pBitmap);
				if (pBitmap)
				{
					DWORD dwSize = DM::GetFileSizeW(szFileName);
					if (dwSize!=0)
					{
						DMBufT<byte>pBuf;pBuf.Allocate(dwSize);
						DWORD dwRead = 0;
						DM::GetFileBufW(szFileName,(void**)&pBuf,dwSize,dwRead);
						pBitmap->LoadFromMemory(pBuf,dwSize,L"");
					}
					DMSmartPtrT<IDMRes> pRes;
					g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
					pRes->SetAttribute(L"boutstyle",L"1",false);// 外部设置模式
					g_pDMApp->RedrawAll();
				}
			}
		}
	} while (false);
	return DM_ECODE_OK;
}

void CSkinWnd::OnSize(UINT nType, CSize size)
{
	if (!IsIconic()) 
	{
		CRect rcWnd;
		::GetWindowRect(m_hWnd, &rcWnd);
		::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);
		HRGN hWndRgn = ::CreateRoundRectRgn(0, 0, rcWnd.right, rcWnd.bottom,1,1);
		SetWindowRgn(hWndRgn, TRUE);
		::DeleteObject(hWndRgn);
	}
	m_WndShadow.SetPosition(0, 0);
	SetMsgHandled(FALSE);
}

void CSkinWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(bShow)
	{
		DM_AnimateWindow(300,AW_CENTER);
		m_WndShadow.SetPosition(0, 0);
	}
}

DMCode CSkinWnd::OnClose()
{
	DM_AnimateWindow(200,AW_VER_NEGATIVE|AW_HIDE);
	DestroyWindow();
	return DM_ECODE_OK;
}