#include "DMSpyAfx.h"
#include "SkinWnd.h"

BEGIN_MSG_MAP(CSkinWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(DMHWnd)
END_MSG_MAP()
BEGIN_EVENT_MAP(CSkinWnd)
	EVENT_NAME_COMMAND(L"btn_skinclose",OnClose)
	EVENT_NAME_COMMAND(L"skin1",OnChangeSkin1)
	EVENT_NAME_COMMAND(L"skin2",OnChangeSkin2)
	EVENT_NAME_COMMAND(L"skin3",OnChangeSkin3)
END_EVENT_MAP()


CSkinWnd::CSkinWnd()
{
	memset(m_szCurThemes, 0,200);
}

CSkinWnd::~CSkinWnd()
{

}

BOOL CSkinWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	return TRUE;
}

DMCode CSkinWnd::OnChangeSkin1()
{
	DMSmartPtrT<IDMRes> m_pRes;
	g_pDMApp->GetDefRegObj((void**)&m_pRes, DMREG_Res);
	LPCWSTR lptheme = L"theme1";
	m_pRes->SetCurTheme(lptheme);
	memset(m_szCurThemes, 0,200);
	memcpy(m_szCurThemes,lptheme,2*wcslen(lptheme));
	return DM_ECODE_OK;
}

DMCode CSkinWnd::OnChangeSkin2()
{
	DMSmartPtrT<IDMRes> m_pRes;
	g_pDMApp->GetDefRegObj((void**)&m_pRes, DMREG_Res);

	LPCWSTR lptheme = L"theme2";
	if (!DMSUCCEEDED(m_pRes->SetCurTheme(lptheme)))// 开始没有加载主题包2时，尝试外部加载
	{
		m_pRes->LoadTheme((WPARAM)lptheme,(LPARAM)L"DMRes\\themes\\theme2\\dmindex.xml");
		m_pRes->SetCurTheme(lptheme);
	}
	memset(m_szCurThemes, 0,200);
	memcpy(m_szCurThemes,lptheme,2*wcslen(lptheme));
	return DM_ECODE_OK;
}

DMCode CSkinWnd::OnChangeSkin3()
{
	DMSmartPtrT<IDMRes> m_pRes;
	g_pDMApp->GetDefRegObj((void**)&m_pRes, DMREG_Res);
	LPCWSTR lptheme = L"default";
	m_pRes->SetCurTheme(lptheme);
	memset(m_szCurThemes, 0,200);
	memcpy(m_szCurThemes,lptheme,2*wcslen(lptheme));
	return DM_ECODE_OK;

}

DMCode CSkinWnd::OnClose()
{
	DM_AnimateWindow(200,AW_VER_NEGATIVE|AW_HIDE);
	DestroyWindow();
	g_pDMApp->RemoveSkinPoolItem(L"skinprivate");
	return DM_ECODE_OK;
}