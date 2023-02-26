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
	EVENT_NAME_COMMAND("skin_closebtn",OnClose)
	EVENT_NAME_COMMAND("skin_outbtn",OnOutSkin)
END_EVENT_MAP()


CSkinWnd::CSkinWnd(CQQMainWnd*pOwner)
{
	memset(m_szCurThemes, 0,200);
	m_pMainWnd = pOwner;
}

BOOL CSkinWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// �����ʼ������
	DUIWindow* pTab = FindChildByNameT<DUIWindow>("tablayout");  // ����
	DMASSERT(pTab);
	DMSmartPtrT<IDMRes> pRes;
	g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
	if (0 == dm_xmlstrcmp("DMResFolderImpl",pRes->V_GetClassName())
		||0 == dm_xmlstrcmp("DMResZipImpl",pRes->V_GetClassName()))
	{
		int  iAllThemeSize = NULL;
		pRes->SendExpandInfo((WPARAM)&iAllThemeSize,NULL);
		DMBufT<byte>pBuf; pBuf.Allocate(iAllThemeSize);
		pRes->SendExpandInfo((WPARAM)(LPVOID)pBuf, (LPARAM)iAllThemeSize);
		CStringA strValue = (char*)pBuf.get();
		CStringAList strUpdateList;
		int nCount = (int)SplitStringT(strValue,';',strUpdateList);
		CStringA strType = "png";CStringA strResName="preview";
		for (int i=0;i<nCount;i++)
		{
			unsigned long ulSize = 0;
			CStringA strTemp = strUpdateList[i];
			if (DMSUCCEEDED(pRes->GetItemSize(strType,strResName,ulSize,strTemp)))
			{
				DMBufT<byte>pBuf;
				if (DMSUCCEEDED(pRes->GetItemBuf(strType,strResName, pBuf, &ulSize,strTemp)))
				{
					// ����Ԥ��skin
					DMSmartPtrT<IDMSkin> pSkin;
					g_pDMApp->CreateRegObj((void**)&pSkin,"imglist",DMREG_Skin);
					pSkin->SetBitmap(pBuf,ulSize,strType);
				
					SkinPreview *pChild = NULL;
					g_pDMApp->CreateRegObj((void**)&pChild, "SkinPreview",DMREG_Window);
					if (pChild)
					{
						pChild->m_pSkin = pSkin;
						pChild->m_strThemeName = DMA2W(strTemp);
						pChild->SetAttribute("tiptext",strTemp);
						pChild->SetAttribute("cursor","hand");
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
		LPCWSTR lpszPath = L".\\QQRes\\outskin\\";
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
		ofn.lpstrFilter = L"ͼ���ļ�(*.bmp;*.jpg;*.png)\0*.bmp;*.jpg;*.png\0\0";
		ofn.hwndOwner = m_hWnd;
		if (::GetOpenFileNameW(&ofn))
		{// todo.hgy413 note:GetOpenFileNameW�����WM_LBUTTTONUP��Ϣ�ᷢ�͸�dui�����Բ�Ҫ����ֻ����WM_LBUTTONUP
			DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin("bg");
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
						pBitmap->LoadFromMemory(pBuf,dwSize,"");
					}
					DMSmartPtrT<IDMRes> pRes;
					g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
					pRes->SetAttribute("boutstyle","1",false);// �ⲿ����ģʽ
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