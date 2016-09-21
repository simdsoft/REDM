#include "DMDemoAfx.h"
#include "SkinWnd.h"
#include "MainWnd.h"

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


CSkinWnd::CSkinWnd(CMainWnd*pOwner)
{
	memset(m_szCurThemes, 0,200);
	m_pMainWnd = pOwner;
	m_pH = m_pS = m_pL = m_pA = NULL;
}

CSkinWnd::~CSkinWnd()
{
}

BOOL CSkinWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_pH = FindChildByNameT<DUISliderCtrl>(L"h_slider");
	m_pS = FindChildByNameT<DUISliderCtrl>(L"s_slider");
	m_pL = FindChildByNameT<DUISliderCtrl>(L"l_slider");
	m_pA = FindChildByNameT<DUISliderCtrl>(L"a_slider");
	DMASSERT(NULL!=m_pH&&NULL!=m_pS&&NULL!=m_pL&&NULL!=m_pA);
	m_pH->SetPos(m_pMainWnd->m_HBk);
	m_pS->SetPos(m_pMainWnd->m_SBk);
	m_pL->SetPos(m_pMainWnd->m_LBk);
	m_pA->SetPos(m_pMainWnd->m_ABk);
	g_pDMApp->AdjustHSLA(m_pMainWnd->m_HBk, DM_H);
	g_pDMApp->AdjustHSLA(m_pMainWnd->m_SBk, DM_S);
	g_pDMApp->AdjustHSLA(m_pMainWnd->m_LBk, DM_L);
	g_pDMApp->AdjustHSLA(m_pMainWnd->m_ABk, DM_A);
	m_pH->m_EventMgr.SubscribeEvent(DM::DMEventSDChangedArgs::EventID,Subscriber(&CSkinWnd::OnChangeH,this));
	m_pS->m_EventMgr.SubscribeEvent(DM::DMEventSDChangedArgs::EventID,Subscriber(&CSkinWnd::OnChangeS,this));
	m_pL->m_EventMgr.SubscribeEvent(DM::DMEventSDChangedArgs::EventID,Subscriber(&CSkinWnd::OnChangeL,this));
	m_pA->m_EventMgr.SubscribeEvent(DM::DMEventSDChangedArgs::EventID,Subscriber(&CSkinWnd::OnChangeA,this));
	return TRUE;
}

DMCode CSkinWnd::OnChangeH(DMEventArgs* pEvent)
{
	DMEventSDChangedArgs* pEvt = (DMEventSDChangedArgs*)pEvent;
	if (pEvt)
	{
		int iValue = pEvt->m_nNewPos;
		g_pDMApp->AdjustHSLA(iValue, DM_H);
		m_pMainWnd->m_HBk = iValue;
	}
	return DM_ECODE_OK;
}

DMCode CSkinWnd::OnChangeS(DMEventArgs* pEvent)
{
	DMEventSDChangedArgs* pEvt = (DMEventSDChangedArgs*)pEvent;
	if (pEvt)
	{
		int iValue = pEvt->m_nNewPos;
		g_pDMApp->AdjustHSLA(iValue, DM_S);
		m_pMainWnd->m_SBk = iValue;
	}
	return DM_ECODE_OK;
}

DMCode CSkinWnd::OnChangeL(DMEventArgs* pEvent)
{
	DMEventSDChangedArgs* pEvt = (DMEventSDChangedArgs*)pEvent;
	if (pEvt)
	{
		int iValue = pEvt->m_nNewPos;
		g_pDMApp->AdjustHSLA(iValue, DM_L);
		m_pMainWnd->m_LBk = iValue;
	}
	return DM_ECODE_OK;
}

DMCode CSkinWnd::OnChangeA(DMEventArgs* pEvent)
{
	DMEventSDChangedArgs* pEvt = (DMEventSDChangedArgs*)pEvent;
	if (pEvt)
	{
		int iValue = pEvt->m_nNewPos;
		g_pDMApp->AdjustHSLA(iValue, DM_A);
		m_pMainWnd->m_ABk = iValue;
	}
	return DM_ECODE_OK;
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