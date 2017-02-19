#include "DMDemoAfx.h"
#include "MainWnd.h"
#include <commdlg.h>
#include "DUIListBox.h"
#include "IEWnd.h"

BEGIN_MSG_MAP(CMainWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SIZE(OnSize)
	MSG_WM_COMMAND(OnCommand)
	CHAIN_MSG_MAP(DMHWnd)
END_MSG_MAP()

BEGIN_EVENT_MAP(CMainWnd)
	EVENT_NAME_COMMAND(L"test", OnTest)
	EVENT_NAME_COMMAND(L"btn_close",OnClose)
	EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
	EVENT_ID_COMMAND(10, OnRestore)
	EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
	EVENT_NAME_COMMAND(L"btn_skin",OnSkin)
	EVENT_NAME_COMMAND(L"btn_attrtest",OnAttrTest)
	EVENT_NAME_COMMAND(L"dlgtest",OnDlgTest)
	EVENT_NAME_COMMAND(L"accelbtn",OnDlgTest)
	EVENT_NAME_COMMAND(L"listtest",OnListTest)
	EVENT_NAME_COMMAND(L"replaygifbtn",OnReplayGif)
	EVENT_NAME_COMMAND(L"opengifbtn",OnOpenGif)
	EVENT_NAME_COMMAND(L"btn_menu",OnBtnMenu)
	EVENT_NAME_COMMAND(L"startbtn",OnStartBtn)
	EVENT_NAME_COMMAND(L"stopbtn",OnStopBtn)

	EVENT_NAME_COMMAND(L"webback",OnBtnWebBack)
	EVENT_NAME_COMMAND(L"webrefresh",OnBtnWebRefresh)
	EVENT_NAME_COMMAND(L"webfront",OnBtnWebfront)

	EVENT_NAME_COMMAND(L"ieback",OnBtnIeBack)
	EVENT_NAME_COMMAND(L"ierefresh",OnBtnIeRefresh)
	EVENT_NAME_COMMAND(L"iefront",OnBtnIefront)
	// 动画
	EVENT_NAME_COMMAND(L"screen_4btn",OnScreen4Start)
	EVENT_NAME_COMMAND(L"screen_meltbtn",OnScreenMeltStart)
	EVENT_NAME_COMMAND(L"line_openbtn",OnLineOpenAnimateStart)
	EVENT_NAME_COMMAND(L"line_closetbtn",OnLineCloseAnimateStart)
	EVENT_NAME_COMMAND(L"3dstartbtn",On3DAnimateStart)

END_EVENT_MAP()

CMainWnd::CMainWnd()
{
	m_pWebkit     = NULL;
	m_bStartTimer = false;

	m_HBk = 0;
	m_SBk = 100;
	m_ABk = 255;
	m_LBk = 100;
}

CMainWnd::~CMainWnd()
{
}

BOOL CMainWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_pWebkit = FindChildByNameT<DUIWebKit>(L"webobj");
	DUIRichEdit *pRichEdit = FindChildByNameT<DUIRichEdit>(L"web_url");
	if (pRichEdit)
	{
		pRichEdit->SetWindowText(L"http://ka.duowan.com/");
		pRichEdit->m_EventMgr.SubscribeEvent(DM::DMEventREWantReturnArgs::EventID, Subscriber(&CMainWnd::RefreshWeb, this));
	}

	if (m_pWebkit)
	{
		m_handler.onTitleChanged = CMainWnd::OnTitleChanged;
		m_handler.onURLChanged = CMainWnd::OnURLChanged;
		m_handler.pdata = this;
		m_pWebkit->GetWebView()->setClientHandler(&m_handler);
		//http://www.baidu.com/s?ie=utf-8&f=8&wd=素材
		//http://www.baidu.com/s?ie=utf-8&f=8&wd=%E7%B4%A0%E6%9D%90
		//CStringA szUrl = "http://www.baidu.com/s?ie=utf-8&f=8&wd=素材";
		m_pWebkit->GetWebView()->loadURL(L"http://ka.duowan.com/");
	}

	IERealWnd * pIEWnd = FindChildByNameT<IERealWnd>(L"ieobj");
	if (pIEWnd)
	{
		m_pIE = pIEWnd->m_pIEWnd->FindChildByNameT<IDUIWeb>(L"ie");
		if (m_pIE)
		{
			m_pIE->SetEvtHandler(this);
		}
	}
	DUIRichEdit* pRichEditIE = FindChildByNameT<DUIRichEdit>(L"ieurl");
	if (pRichEditIE)
	{
		pRichEditIE->m_EventMgr.SubscribeEvent(DM::DMEventREWantReturnArgs::EventID, Subscriber(&CMainWnd::IERefreshWeb, this));
	}

	CStringW strScroll = L"";
	for (int i=0;i<2;i++)
	{
		strScroll.Format(L"scrollbar%d",i+1);
		DUIScrollBase* pScrollBar = FindChildByNameT<DUIScrollBase>(strScroll);
		if (pScrollBar)
		{
			pScrollBar->m_szRange.SetSize(600,600);
			pScrollBar->UpdateScrollBar();
		}
	}

	DUIRadioButton *pR = FindChildByNameT<DUIRadioButton>(L"global_radio");
	if (pR)
	{
		pR->DM_SetCheck(true);
	}

	DUISliderCtrl *px = FindChildByNameT<DUISliderCtrl>(L"3d_x");
	DUISliderCtrl *py = FindChildByNameT<DUISliderCtrl>(L"3d_y");
	DUISliderCtrl *pz = FindChildByNameT<DUISliderCtrl>(L"3d_z");
	DUISliderCtrl *pf = FindChildByNameT<DUISliderCtrl>(L"3d_offset");
	DMASSERT(NULL!=px&&NULL!=py&&NULL!=pz&&NULL!=pf);
	px->m_EventMgr.SubscribeEvent(DM::DMEventSDChangedArgs::EventID,Subscriber(&CMainWnd::On3Dx,this));
	py->m_EventMgr.SubscribeEvent(DM::DMEventSDChangedArgs::EventID,Subscriber(&CMainWnd::On3Dy,this));
	pz->m_EventMgr.SubscribeEvent(DM::DMEventSDChangedArgs::EventID,Subscriber(&CMainWnd::On3Dz,this));
	pf->m_EventMgr.SubscribeEvent(DM::DMEventSDChangedArgs::EventID,Subscriber(&CMainWnd::On3Df,this));

	m_ABk = m_pHWndXmlInfo->m_byAlpha;

	return TRUE;
}

void CMainWnd::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);  // 由DMHWnd继续处理
	if (0 == size.cx||0 == size.cy)
	{
		return;
	}
	if (SIZE_MAXIMIZED == nType)
	{
		DUIWindow*pMaxWnd     = FindChildByName(L"btn_max");
		DUIWindow*pRestoreWnd = FindChildById(10);
		if (pMaxWnd&&pRestoreWnd)
		{
			pMaxWnd->DM_SetVisible(false);
			pRestoreWnd->DM_SetVisible(true);
		}
	}
	else if (SIZE_RESTORED == nType)
	{
		DUIWindow*pMaxWnd     = FindChildByName(L"btn_max");
		DUIWindow*pRestoreWnd = FindChildById(10);
		if (pMaxWnd&&pRestoreWnd)
		{
			pMaxWnd->DM_SetVisible(true);
			pRestoreWnd->DM_SetVisible(false);
		}
	}
}

DMCode CMainWnd::OnTest()
{
	DUIScrollBase *pScrollBase = FindChildByNameT<DUIScrollBase>(L"test");
	if (pScrollBase)
	{
		pScrollBase->ShowScrollBar(DMSB_VERT,true);
		SCROLLINFO si = {0};
		si.cbSize = sizeof(SCROLLINFO);
		si.nPage  = pScrollBase->m_rcWindow.Height();
		si.nMax   = 2*si.nPage;
		si.nTrackPos = si.nPage;
		pScrollBase->SetScrollInfo(true,&si, true);
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnClose()
{
	if (m_pSkinWnd&&m_pSkinWnd->IsWindow())
	{
		m_pSkinWnd->DestroyWindow();
	}
	if (m_pAttrTestWnd&&m_pAttrTestWnd->IsWindow())
	{
		m_pAttrTestWnd->DestroyWindow();
	}
	DM_AnimateWindow(400,AW_SLIDE|AW_HOR_POSITIVE|AW_HIDE);
	DestroyWindow();
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	SetActiveWindow();
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnRestore()
{
	SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	SetActiveWindow();
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnSkin()
{
	if (m_pSkinWnd&&m_pSkinWnd->IsWindow())
	{
		return DM_ECODE_OK;
	}
	m_pSkinWnd.Release();
	m_pSkinWnd.Attach(new CSkinWnd(this));
	m_pSkinWnd->DM_CreateWindow(L"dui_skin");				// 创建主窗口
	m_pSkinWnd->SendMessage(WM_INITDIALOG);
	m_pSkinWnd->CenterWindow();
	CRect rcWindow = m_rcWindow;
	ClientToScreen(rcWindow);
	m_pSkinWnd->SetWindowPos(HWND_TOPMOST, rcWindow.right-m_pSkinWnd->m_rcWindow.Width()-10,rcWindow.top+230,0, 0, SWP_NOSIZE);
	m_pSkinWnd->ShowWindow(SW_SHOW);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnAttrTest()
{
	if (m_pAttrTestWnd&&m_pAttrTestWnd->IsWindow())
	{
		return DM_ECODE_OK;
	}
	m_pAttrTestWnd.Release();
	m_pAttrTestWnd.Attach(new CAttributeTestWnd(this));
	m_pAttrTestWnd->DM_CreateWindow(L"dui_attrtest",0,0,0,0,m_hWnd);				// 创建主窗口
	m_pAttrTestWnd->SendMessage(WM_INITDIALOG);
	m_pAttrTestWnd->CenterWindow();
	m_pAttrTestWnd->ShowWindow(SW_SHOW);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnDlgTest()
{
	DMSmartPtrT<DMHDialog> pDlg;
	pDlg.Attach(new DMHDialog());
	pDlg->DoModal(L"dui_dlgtest",m_hWnd);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnListTest()
{
	DUIListBox *pList = FindChildByNameT<DUIListBox>(L"listbox");
	if (pList)
	{
		pList->EnsureVisible(3);
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnReplayGif()
{
	DUIGif *pGifWnd = FindChildByNameT<DUIGif>(L"biggif");
	if (pGifWnd)
	{
		pGifWnd->Restart();
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnOpenGif()
{
	DUIGif *pGifWnd = FindChildByNameT<DUIGif>(L"biggif");
	if (pGifWnd)
	{
		wchar_t szFileTitle[_MAX_FNAME] = {0};  
		wchar_t szFileName[_MAX_PATH]   = {0};    
		 OPENFILENAMEW ofn;
		 memset(&ofn, 0, sizeof(ofn));
		 ofn.lStructSize = sizeof(ofn);
		 ofn.lpstrFile = szFileName;
		 ofn.nMaxFile = _MAX_PATH;
		 ofn.lpstrDefExt = L"gif";
		 ofn.lpstrFileTitle = (LPTSTR)szFileTitle;
		 ofn.nMaxFileTitle = _MAX_FNAME;
		 ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING| OFN_NOCHANGEDIR;
		 ofn.lpstrFilter = L"gif files(*.gif)\0*.gif\0All files (*.*)\0*.*\0\0";
		 ofn.hwndOwner = m_hWnd;
		if (::GetOpenFileNameW(&ofn))
		{
			if (DMSUCCEEDED(pGifWnd->LoadFromFile(szFileName)))
			{
				pGifWnd->Start();
			}
		}
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnBtnMenu()
{
	DUIMenu Menu;
	Menu.LoadMenu(L"LayOut",L"Dui_MENU");
	POINT pt;
	GetCursorPos(&pt);
	Menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnStartBtn()
{
	m_bStartTimer = true;
	if (0 == m_TimelineList.GetCount())// 没有就启动定时器
	{
		DM_SetTimer(TIMER_NEXTFRAME,10);
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnStopBtn()
{
	m_bStartTimer = false;
	if (0 == m_TimelineList.GetCount())
	{
		DM_KillTimer(TIMER_NEXTFRAME);
	}
	return DM_ECODE_OK;
}


void CMainWnd::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (3335 == nID)
	{
		OnClose();
	}

	if (33341 == nID)
	{
		On3DAnimateStart();
	}
}

//--------webkit
DMCode CMainWnd::OnBtnWebBack()
{
	if (m_pWebkit)
	{
		m_pWebkit->GetWebView()->goBack();
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnBtnWebRefresh()
{
	if (m_pWebkit)
	{
		m_pWebkit->GetWebView()->reload();
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnBtnWebfront()
{
	if (m_pWebkit)
	{
		m_pWebkit->GetWebView()->goForward();
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::RefreshWeb(DMEventArgs* pEvent)
{
	DUIRichEdit *pRichEdit = FindChildByNameT<DUIRichEdit>(L"web_url");
	if (pRichEdit&&m_pWebkit)
	{
		CStringW szUrl = pRichEdit->GetWindowText();
		//wchar_t *pBuf = szUrl.GetBufferSetLength(MAX_PATH);
		//pRichEdit->GetWindowText(szUrl,-1);
		if (!szUrl.IsEmpty())
		{
			m_pWebkit->GetWebView()->loadURL(szUrl);
		}
		szUrl.ReleaseBuffer();
	}

	return DM_ECODE_OK;
}

//---------ie
DMCode CMainWnd::OnBtnIeBack()
{
	if (m_pIE)
	{
		m_pIE->GoBack();
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnBtnIeRefresh()
{
	if (m_pIE)
	{
		m_pIE->Refresh();
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnBtnIefront()
{
	if (m_pIE)
	{
		m_pIE->GoForward();
	}
	return DM_ECODE_OK;
}

DM::DMCode CMainWnd::IERefreshWeb( DMEventArgs* pEvent )
{
	DUIRichEdit *pRichEdit = FindChildByNameT<DUIRichEdit>(L"ieurl");
	if (pRichEdit && m_pIE)
	{
		CStringW szUrl;
		wchar_t *pBuf = szUrl.GetBufferSetLength(1024);
		pRichEdit->GetWindowText(pBuf,-1);
		if (!szUrl.IsEmpty())
		{
			m_pIE->OpenUrl(szUrl);
		}
		szUrl.ReleaseBuffer();
	}

	return DM_ECODE_OK;
}

HRESULT CMainWnd::NavigateComplete2(DUIWND hWnd,DMIN IDispatch *pDisp,DMIN wchar_t *pUrl)
{
	DUIRichEdit *pRichEdit = FindChildByNameT<DUIRichEdit>(L"ieurl");
	if (pRichEdit&&m_pIE)
	{
		pRichEdit->SetWindowText(pUrl);
	}
	return S_OK;
}

DMCode CMainWnd::MyTimer()
{
	CStringW strSlider = L"";
	for (int i=0;i<4;i++)
	{
		strSlider.Format(L"sliderctrl%d",i+1);
		DUISliderCtrl* pSlider = FindChildByNameT<DUISliderCtrl>(strSlider);
		if (pSlider)
		{
			int iValue = pSlider->GetPos();
			int iMin;
			int iMax;
			pSlider->GetRange(iMin,iMax);
			iValue += 1;
			if (iValue>iMax)
			{
				iValue = iMin;
			}
			pSlider->SetPos(iValue);
		}
	}

	CStringW strProcess = L"";
	for (int i=0;i<2;i++)
	{
		strProcess.Format(L"processctrl%d",i+1);
		DUIProcessCtrl* pProcess = FindChildByNameT<DUIProcessCtrl>(strProcess);
		if (pProcess)
		{
			int iValue = pProcess->GetPos();
			int iMin;
			int iMax;
			pProcess->GetRange(iMin,iMax);
			iValue += 1;
			if (iValue>iMax)
			{
				iValue = iMin;
			}
			pProcess->SetPos(iValue);
		}
	}
	CStringW strScroll = L"";
	for (int i=0;i<2;i++)
	{
		strScroll.Format(L"scrollbar%d",i+1);
		DUIScrollBase* pScrollBar = FindChildByNameT<DUIScrollBase>(strScroll);
		if (pScrollBar)
		{
			pScrollBar->m_siHoz.nPos+=1;
			pScrollBar->m_siVer.nPos+=1;
			if (pScrollBar->m_siHoz.nPos>pScrollBar->m_siHoz.nMax-pScrollBar->m_siHoz.nMin-pScrollBar->m_siHoz.nPage)
			{
				pScrollBar->m_siHoz.nPos = pScrollBar->m_siHoz.nMin;
			}
			if (pScrollBar->m_siVer.nPos>pScrollBar->m_siVer.nMax-pScrollBar->m_siHoz.nMin-pScrollBar->m_siVer.nPage)
			{
				pScrollBar->m_siVer.nPos = pScrollBar->m_siVer.nMin;
			}
			pScrollBar->UpdateScrollBar();
		}
	}
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnTimeline()
{
	if (m_bStartTimer)
	{
		MyTimer();
	}
	return __super::OnTimeline();
}

void CMainWnd::OnTitleChanged(const DMClientHandler*, DMString title)
{
}

void CMainWnd::OnURLChanged(const DMClientHandler*handler, DMString url)
{
	CMainWnd *pWnd = (CMainWnd *)handler->pdata;
	DUIRichEdit *pRichEdit = pWnd->FindChildByNameT<DUIRichEdit>(L"web_url");
	DUIWebKit *pWebKit = pWnd->FindChildByNameT<DUIWebKit>(L"webobj");
	if (pRichEdit&&pWebKit)
	{
		pRichEdit->SetWindowText(pWebKit->GetWebView()->toStringW(url));
	}
}

// 动画---------------------------------------

DMCode CMainWnd::AnimateBegin_Callback(IDMAnimate*pAni,WPARAM wp, LPARAM lp)
{
	DM_SendMessage(WM_SHOWWINDOW,false,NormalShow);
	ShowWindow(SW_HIDE);
	return DM_ECODE_OK;
}

DMCode CMainWnd::AnimateMid_Callback(IDMAnimate*pAni, WPARAM wp, LPARAM lp)
{
	pAni->AnimateEnd(MAKELONG(10,10),0);
	return DM_ECODE_OK;
}

DMCode CMainWnd::AnimateEnd_Callback(IDMAnimate*pAni,WPARAM wp, LPARAM lp)
{	
	DM_SendMessage(WM_SHOWWINDOW,true,NormalShow);
	RedrawAll();
	ShowWindow(SW_SHOWNA);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnScreen4Start()
{
	do 
	{
		m_pAnimate.Release();
		g_pDMApp->CreateRegObj((void**)&m_pAnimate,L"DMScreenAnimate",DMREG_Animate);
		if (m_pAnimate.isNull())
		{
			break;
		}
		DUIWindow*pIE = FindChildByName(L"ie");
		if (pIE&&pIE->DM_IsVisible(true))
		{
			pIE->DM_SendMessage(WM_PAINT,(WPARAM)m_pShowCanvas.get(),0);
		}
		m_pAnimate->AnimateInit(this,m_pShowCanvas,NULL);
		m_pAnimate->AnimateBegin(0,MAKELONG(15,5));
	}while(false);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnScreenMeltStart()
{
	do 
	{
		m_pAnimate.Release();
		g_pDMApp->CreateRegObj((void**)&m_pAnimate,L"DMScreenAnimate",DMREG_Animate);
		if (m_pAnimate.isNull())
		{
			break;
		}
		DUIWindow*pIE = FindChildByName(L"ie");
		if (pIE&&pIE->DM_IsVisible(true))
		{
			pIE->DM_SendMessage(WM_PAINT,(WPARAM)m_pShowCanvas.get(),0);
		}
		m_pAnimate->AnimateInit(this,m_pShowCanvas,NULL);
		m_pAnimate->AnimateBegin(1,MAKELONG(15,5));
	}while(false);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnLineOpenAnimateStart()
{
	do 
	{
		m_pAnimate.Release();
		g_pDMApp->CreateRegObj((void**)&m_pAnimate,L"DMLineAnimate",DMREG_Animate);
		if (m_pAnimate.isNull())
		{
			break;
		}
		m_pAnimate->AnimateInit(this,NULL,NULL);
		m_pAnimate->AnimateBegin(1);
	}while(false);
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnLineCloseAnimateStart()
{
	do 
	{
		m_pAnimate.Release();
		g_pDMApp->CreateRegObj((void**)&m_pAnimate,L"DMLineAnimate",DMREG_Animate);
		if (m_pAnimate.isNull())
		{
			break;
		}
		m_pAnimate->AnimateInit(this,NULL,NULL);
		m_pAnimate->AnimateBegin();
	}while(false);
	return DM_ECODE_OK;
}

struct PARAM3D{int x;int y;	int z;int f;};
DMCode CMainWnd::On3DAnimateStart()
{
	do 
	{
		m_pAnimate.Release();
		g_pDMApp->CreateRegObj((void**)&m_pAnimate,L"DM3DAnimate",DMREG_Animate);
		if (m_pAnimate.isNull())
		{
			break;
		}

		bool bGlobal = true;
		DUIRadioButton *pR = FindChildByNameT<DUIRadioButton>(L"global_radio");
		if (pR&&!pR->DM_IsChecked())// 非全局
		{
			bGlobal = false;
		}

		PARAM3D data;int iCount = 0;
		DUISliderCtrl *px = FindChildByNameT<DUISliderCtrl>(L"3d_x");data.x = px->GetPos();
		DUISliderCtrl *py = FindChildByNameT<DUISliderCtrl>(L"3d_y");data.y = py->GetPos();
		DUISliderCtrl *pz = FindChildByNameT<DUISliderCtrl>(L"3d_z");data.z = pz->GetPos();
		DUISliderCtrl *pf = FindChildByNameT<DUISliderCtrl>(L"3d_offset");data.f = pf->GetPos();
		DUISliderCtrl *pc = FindChildByNameT<DUISliderCtrl>(L"3d_count");iCount = pc->GetPos();

		if (bGlobal)
		{
			DUIWindow*pIE = FindChildByName(L"ie");
			if (pIE&&pIE->DM_IsVisible(true))
			{
				pIE->DM_SendMessage(WM_PAINT,(WPARAM)m_pShowCanvas.get(),0);
			}
			
			m_pAnimate->AnimateInit(this,m_pShowCanvas,m_pShowCanvas,(WPARAM)&data,iCount<=0?10:iCount);
			m_pAnimate->AnimateBegin(MAKELONG(10,100),10);
		}
		else
		{
			return On3Dx(NULL);
		}
		
	} while (false);
	
	return DM_ECODE_OK;
}

DMCode CMainWnd::On3Dx(DMEventArgs* pEvent)
{
	do 
	{
		DUIRadioButton *pR = FindChildByNameT<DUIRadioButton>(L"global_radio");
		if (pR&&pR->DM_IsChecked())// 全局
		{
			break;
		}
		m_pAnimate.Release();
		g_pDMApp->CreateRegObj((void**)&m_pAnimate,L"DM3DAnimate",DMREG_Animate);
		if (m_pAnimate.isNull())
		{
			break;
		}
		PARAM3D data;
		DUISliderCtrl *px = FindChildByNameT<DUISliderCtrl>(L"3d_x");data.x = px->GetPos();
		DUISliderCtrl *py = FindChildByNameT<DUISliderCtrl>(L"3d_y");data.y = py->GetPos();
		DUISliderCtrl *pz = FindChildByNameT<DUISliderCtrl>(L"3d_z");data.z = pz->GetPos();
		DUISliderCtrl *pf = FindChildByNameT<DUISliderCtrl>(L"3d_offset");data.f = pf->GetPos();
		DUIWindow *pWnd = FindChildByName(L"anishow");
		if (pWnd)
		{
		
			DMSmartPtrT<IDMSkin> pSkin;
			pWnd->m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
			if (NULL == pSkin)
			{
				pWnd->m_pDUIXmlInfo->m_pStyle->SetAttribute(L"skin",L"1",false);
				pWnd->m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
			}
			if (pSkin)
			{
				// 先恢复
				wchar_t pWBuf[] = L"<imglist id=\"1\" src=\"png:1\" states=\"1\" />";
				int ulSize = 100;
				byte pBuf[100] = {0};
				UnicodeToUtf8(pWBuf,(PCHAR)pBuf,ulSize);
				DMXmlDocument doc;
				doc.LoadFromBuffer(pBuf, ulSize);
				DMXmlNode XmlNode = doc.Root();
				pSkin->InitDMData(XmlNode);

				DMSmartPtrT<IDMBitmap> pBitmap;
				pSkin->GetBitmap(&pBitmap);
				if (pBitmap)
				{
					m_pAnimate->AnitmateGet((WPARAM)&data,(LPARAM)pBitmap.get());
					pWnd->DM_Invalidate();
				}
			}
		}
	} while (false);
	
	return DM_ECODE_OK;
}

DMCode CMainWnd::On3Dy(DMEventArgs* pEvent)
{
	return On3Dx(pEvent);
}

DMCode CMainWnd::On3Dz(DMEventArgs* pEvent)
{
	return On3Dx(pEvent);
}

DMCode CMainWnd::On3Df(DMEventArgs* pEvent)
{
	return On3Dx(pEvent);
}