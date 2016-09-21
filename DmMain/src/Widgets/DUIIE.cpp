#include "DmMainAfx.h"
#include "DUIIE.h"
#include <mshtmlc.h>


namespace DM
{
	DMIEEvtDispatch::DMIEEvtDispatch(IDMWebEvent* pEventHandler)
		:m_pEvtHandler(pEventHandler),
		m_nRef(1),
		m_hWnd(0)
	{
	}
	void DMIEEvtDispatch::SetWebBrowser(DUIIE  *pWebBrowser)
	{
		m_pWebBrowser = pWebBrowser;
	}

	void DMIEEvtDispatch::SetEvtHandler(IDMWebEvent* pEventHandler)
	{
		m_pEvtHandler = pEventHandler;
	}

	void DMIEEvtDispatch::SetDUIWnd( DUIWND hWnd )
	{
		m_hWnd = hWnd;
	}

	//IUnkown
	HRESULT STDMETHODCALLTYPE DMIEEvtDispatch::QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	ULONG STDMETHODCALLTYPE DMIEEvtDispatch::AddRef(void)
	{
		return ++m_nRef;
	}

	ULONG STDMETHODCALLTYPE DMIEEvtDispatch::Release(void)
	{ 
		return --m_nRef;
	}

	STDMETHODIMP DMIEEvtDispatch::Invoke( /* [in] */ DISPID dispIdMember, /* [in] */ REFIID riid, /* [in] */ LCID lcid, /* [in] */ WORD wFlags, /* [out][in] */ DISPPARAMS *pDispParams, /* [out] */ VARIANT *pVarResult, /* [out] */ EXCEPINFO *pExcepInfo, /* [out] */ UINT *puArgErr )
	{
		if ((riid != IID_NULL) || !m_pEvtHandler)
			return E_INVALIDARG;

		switch(dispIdMember)
		{
		case DISPID_BEFORENAVIGATE2:
			{
				IDispatch *pDisp = pDispParams->rgvarg[6].pdispVal;
				wchar_t *pUrl	 = pDispParams->rgvarg[5].pvarVal->bstrVal; 
				int Flags		 = pDispParams->rgvarg[4].pvarVal->intVal; 
				wchar_t *TargetFrameName = pDispParams->rgvarg[3].pvarVal->bstrVal; 
				wchar_t *PostData		 = pDispParams->rgvarg[2].pvarVal->bstrVal;
				wchar_t *Headers		 = pDispParams->rgvarg[1].pvarVal->bstrVal;
				VARIANT_BOOL *bCancel	 = pDispParams->rgvarg[0].pboolVal;
				m_pEvtHandler->BeforeNavigate2(m_hWnd,pDisp,pUrl,Flags,TargetFrameName,PostData,Headers,bCancel);
			}
			break;

		case DISPID_CLIENTTOHOSTWINDOW:
			{
				long *pCx = pDispParams->rgvarg[1].plVal;
				long *pCy = pDispParams->rgvarg[0].plVal;
				m_pEvtHandler->ClientToHostWindow(m_hWnd,pCx,pCy);
			}
			break;

		case DISPID_COMMANDSTATECHANGE:
			{
				long         Command = pDispParams->rgvarg[1].lVal;
				VARIANT_BOOL Enable  = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->CommandStateChange(m_hWnd,Command,Enable);
			}
			break;

		case DISPID_DOCUMENTCOMPLETE:
			{

// 				DMComQIPtr<IHTMLDocument2> pDoc;
// 				DMComQIPtr<ICustomDoc> pCustomDoc;
// 
// 				IDispatch  *pDisp=NULL;
// 				HRESULT hr=m_pWebBrowser->m_pIE->get_Document((IDispatch **)&pDisp);
// 				if(S_OK==hr)
// 				{
// 					hr=pDisp->QueryInterface(IID_IHTMLDocument2,(void**)&pDoc);
// 					if(S_OK!=hr) return S_FALSE;
// 					//----------------------------------------
// 
// 					hr=pDoc->QueryInterface(IID_ICustomDoc,(void**)&pCustomDoc);
// 					if(S_OK!=hr) return S_FALSE;
// 
// 					hr=pCustomDoc->SetUIHandler(&m_pWebBrowser->m_docHostUIHandler);
// 					if(S_OK!=hr) return S_FALSE;
// 
// 					//---------------------------------------
// 					IConnectionPointContainer *pCPContainer=NULL;
// 					hr=pDoc->QueryInterface(IID_IConnectionPointContainer,(void**)&pCPContainer);
// 					if(S_OK!=hr) return S_FALSE;
// 
// 					IConnectionPoint *m_pConnectionPoint=NULL;
// 					hr = pCPContainer->FindConnectionPoint(DIID_HTMLDocumentEvents2,&m_pConnectionPoint);
// 					if(S_OK!=hr) return S_FALSE;
// 
// 					DWORD m_dwCookie;
// 					hr = m_pConnectionPoint->Advise(&m_pWebBrowser->m_eventDispatch, &m_dwCookie);
// 					if(S_OK!=hr) return S_FALSE;
// 
// 					ULONG ulong=pCPContainer->Release();
// 					ulong=m_pConnectionPoint->Release();
// 
// 					pDisp->Release();
// 				}

				{
					IDispatch *pDispArg = pDispParams->rgvarg[1].pdispVal;
					wchar_t *pURL    = pDispParams->rgvarg[0].pvarVal->bstrVal;
					m_pEvtHandler->DocumentComplete(m_hWnd,pDispArg,pURL);
				}
			}
			break;
		
		case DISPID_DOWNLOADBEGIN:
			{
				m_pEvtHandler->DownloadBegin(m_hWnd);
			}
			break;
		
		case DISPID_DOWNLOADCOMPLETE:
			{
				m_pEvtHandler->DownloadComplete(m_hWnd);
			}
			break;
		
		case DISPID_FILEDOWNLOAD:
			{
				VARIANT_BOOL ActiveDocument = pDispParams->rgvarg[1].boolVal;
				VARIANT_BOOL *Cancel        = pDispParams->rgvarg[0].pboolVal;
				m_pEvtHandler->FileDownload(m_hWnd,ActiveDocument,Cancel);
			}
			break;

		case DISPID_NAVIGATECOMPLETE2:
			{
				IDispatch *pDisp = pDispParams->rgvarg[1].pdispVal;
				wchar_t *pUrl    = pDispParams->rgvarg[0].pvarVal->bstrVal;
				m_pEvtHandler->NavigateComplete2(m_hWnd,pDisp,pUrl);
			}
			break;

		case DISPID_NAVIGATEERROR:
			{
				IDispatch *pDisp = pDispParams->rgvarg[4].pdispVal;
				wchar_t *pUrl    = pDispParams->rgvarg[3].pvarVal->bstrVal;
				wchar_t *TargetFrameName = pDispParams->rgvarg[2].pvarVal->bstrVal;
				int StatusCode   = pDispParams->rgvarg[1].pvarVal->intVal;
				VARIANT_BOOL *Cancel = pDispParams->rgvarg[0].pvarVal->pboolVal;
				m_pEvtHandler->NavigateError(m_hWnd,pDisp,pUrl,TargetFrameName,StatusCode,Cancel);
			}
			break;

		case DISPID_NEWWINDOW2:
			{
				IDispatch **ppDisp   = pDispParams->rgvarg[1].ppdispVal;
				VARIANT_BOOL *Cancel = pDispParams->rgvarg[0].pboolVal;
				m_pEvtHandler->NewWindow2(m_hWnd,ppDisp,Cancel);
			}
			break;

		case DISPID_NEWWINDOW3:
			{
				IDispatch **ppDisp	 = pDispParams->rgvarg[4].ppdispVal;
				VARIANT_BOOL *Cancel = pDispParams->rgvarg[3].pboolVal;
				DWORD dwFlags        = pDispParams->rgvarg[2].intVal;
				wchar_t* bstrUrlContext = pDispParams->rgvarg[1].bstrVal;
				wchar_t* bstrUrl        = pDispParams->rgvarg[0].bstrVal;
				m_pEvtHandler->NewWindow3(m_hWnd,ppDisp,Cancel,dwFlags,bstrUrlContext,bstrUrl);
			}
			break;

		case DISPID_ONFULLSCREEN:
			{
				VARIANT_BOOL FullScreen = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->OnFullScreen(m_hWnd,FullScreen);
			}
			break;
		
		case DISPID_ONMENUBAR:
			{
				VARIANT_BOOL MenuBar = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->OnMenuBar(m_hWnd,MenuBar);
			}
			break;

		case DISPID_ONQUIT:
			{
				m_pEvtHandler->OnQuit(m_hWnd);
			}
			break;

		case DISPID_ONSTATUSBAR:
			{
				VARIANT_BOOL StatusBar = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->OnStatusBar(m_hWnd,StatusBar);
			}
			break;

		case DISPID_ONTHEATERMODE:
			{
				VARIANT_BOOL TheaterMode = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->OnTheaterMode(m_hWnd,TheaterMode);
			}
			break;

		case DISPID_ONTOOLBAR:
			{
				VARIANT_BOOL ToolBar = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->OnToolBar(m_hWnd,ToolBar);
			}
			break;

		case DISPID_ONVISIBLE:
			{
				VARIANT_BOOL Visible = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->OnVisible(m_hWnd,Visible);
			}
			break;
		
		case DISPID_PRINTTEMPLATEINSTANTIATION:
			{
				IDispatch *pDisp = pDispParams->rgvarg[0].pdispVal;
				m_pEvtHandler->PrintTemplateInstantiation(m_hWnd,pDisp);
			}
			break;
		
		case DISPID_PRINTTEMPLATETEARDOWN:
			{
				IDispatch *pDisp = pDispParams->rgvarg[0].pdispVal;
				m_pEvtHandler->PrintTemplateTeardown(m_hWnd,pDisp);
			}
			break;

		case DISPID_PRIVACYIMPACTEDSTATECHANGE:
			{
				VARIANT_BOOL PrivacyImpacted = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->PrivacyImpactedStateChange(m_hWnd,PrivacyImpacted);
			}
			break;

		case DISPID_PROGRESSCHANGE:
			{
				LONG Progress 	 =	pDispParams->rgvarg[1].lVal;
				LONG ProgressMax = pDispParams->rgvarg[0].lVal;
				m_pEvtHandler->ProgressChange(m_hWnd,Progress,ProgressMax);
			}
			break;

		case DISPID_PROPERTYCHANGE:
			{
				wchar_t* szProperty = pDispParams->rgvarg[0].bstrVal;
				m_pEvtHandler->PropertyChange(m_hWnd,szProperty);
			}
			break;

		case DISPID_SETPHISHINGFILTERSTATUS:
			{
				LONG PhishingFilterStatus = pDispParams->rgvarg[0].lVal;
				m_pEvtHandler->SetPhishingFilterStatus(m_hWnd,PhishingFilterStatus);
			}
			break;

		case DISPID_SETSECURELOCKICON:
			{
				int SecureLockIcon = pDispParams->rgvarg[0].intVal;
				m_pEvtHandler->SetSecureLockIcon(m_hWnd,SecureLockIcon);
			}
			break;

		case DISPID_STATUSTEXTCHANGE:
			{
				wchar_t* Text = pDispParams->rgvarg[0].bstrVal;
				m_pEvtHandler->StatusTextChange(m_hWnd,Text);
			}
			break;

		case DISPID_TITLECHANGE:
			{
				wchar_t* Text = pDispParams->rgvarg[0].bstrVal;
				m_pEvtHandler->TitleChange(m_hWnd,Text);
			}
			break;
		
		case DISPID_WINDOWCLOSING:
			{
				VARIANT_BOOL IsChildWindow = pDispParams->rgvarg[1].boolVal;
				VARIANT_BOOL *Cancel = pDispParams->rgvarg[0].pboolVal;
				m_pEvtHandler->WindowClosing(m_hWnd,IsChildWindow,Cancel);
			}
			break;

		case DISPID_WINDOWSETHEIGHT:
			{
				LONG Height = pDispParams->rgvarg[0].lVal;
				m_pEvtHandler->WindowSetHeight(m_hWnd,Height);
			}
			break;

		case DISPID_WINDOWSETLEFT:
			{
				LONG Left = pDispParams->rgvarg[0].lVal;
				m_pEvtHandler->WindowSetLeft(m_hWnd,Left);
			}
			break;

		case DISPID_WINDOWSETRESIZABLE:
			{
				VARIANT_BOOL Resizable = pDispParams->rgvarg[0].boolVal;
				m_pEvtHandler->WindowSetResizable(m_hWnd,Resizable);
			}
			break;

		case DISPID_WINDOWSETTOP:
			{
				LONG Top = pDispParams->rgvarg[0].lVal;
				m_pEvtHandler->WindowSetTop(m_hWnd,Top);
			}
			break;

		case DISPID_WINDOWSETWIDTH:
			{
				LONG Width = pDispParams->rgvarg[0].lVal;
				m_pEvtHandler->WindowSetWidth(m_hWnd,Width);
			}	
			break;

		case DISPID_WINDOWSTATECHANGED:
			{
				DWORD dwFlags = pDispParams->rgvarg[1].ulVal;
				DWORD dwValidFlagsMask = pDispParams->rgvarg[0].ulVal;
				m_pEvtHandler->WindowStateChanged(m_hWnd,dwFlags,dwValidFlagsMask);
			}
			break;
		default:
			return DISP_E_MEMBERNOTFOUND;
		}
		return S_OK;
	}

//////////////////////////////////////////////////////////////////////////
//DMIEDocHostUIHandler
	DMIEDocHostUIHandler::DMIEDocHostUIHandler()
		: m_refCount(0)
		, m_pWebBrowser(NULL)
		, m_bHasScrollBar_(false)
	{
	}

	void DMIEDocHostUIHandler::SetWebBrowser( DUIIE *pWebBrowser )
	{
		m_pWebBrowser=pWebBrowser;
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject )
	{
		if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch) || IsEqualIID(riid, IID_IDocHostUIHandler))
		{
			AddRef();
			*ppvObject=this;
			return S_OK;
		}else
		{
			ppvObject=NULL;
			return E_NOINTERFACE;
		}
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::GetHostInfo( /* [out][in] */ DOCHOSTUIINFO *pInfo )
	{
		if(pInfo)
		{
			pInfo->cbSize = sizeof(DOCHOSTUIINFO);
			pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_NO3DOUTERBORDER | DOCHOSTUIFLAG_DIALOG | DOCHOSTUIFLAG_DISABLE_HELP_MENU;
			if (m_bHasScrollBar_)								//决定是否显示滚动条
			{
				pInfo->dwFlags |= DOCHOSTUIFLAG_FLAT_SCROLLBAR;
			}
			else
			{
				pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
			}
			pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::GetExternal( /* [out] */ IDispatch **ppDispatch )
	{
		if(m_pWebBrowser)
		{
			*ppDispatch=&m_pWebBrowser->m_external;
		}else
		{
			*ppDispatch=NULL;
			return S_FALSE;
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::TranslateUrl( /* [in] */ DWORD dwTranslate, /* [in] */ __in __nullterminated OLECHAR *pchURLIn, /* [out] */ __out OLECHAR **ppchURLOut )
	{
		*ppchURLOut = 0;
		return(S_FALSE);
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::FilterDataObject( /* [in] */ IDataObject *pDO, /* [out] */ IDataObject **ppDORet )
	{
		*ppDORet = 0;
		return S_FALSE;
	}

//////////////////////////////////////////////////////////////////////////
//DMIEOleClientSite
	DMIEOleClientSite::DMIEOleClientSite()
		: m_refCount(0)
	{
	}

	void DMIEOleClientSite::SetWebBrowser( DUIIE* pWebBrowser )
	{
		m_pWebBrowser = pWebBrowser;
	}

	STDMETHODIMP DMIEOleClientSite::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ VOID **ppvObject )
	{
		if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleClientSite))
		{
			AddRef();
			*ppvObject=this;
			return S_OK;
		}
		else if (IsEqualIID(riid, IID_IDocHostUIHandler))
		{
			m_pWebBrowser->m_docHostUIHandler.AddRef();
			*ppvObject=&(m_pWebBrowser->m_docHostUIHandler);
			return S_OK;
		}
		else
		{
			ppvObject=NULL;
			return E_NOINTERFACE;
		}
	}

//////////////////////////////////////////////////////////////////////////
//DMIEExternal
	DMIEExternal::DMIEExternal()
		: m_refCount(0)
		, m_pWebBrowser(NULL), m_pEvtHandler(NULL)
	{
	}

	void DMIEExternal::SetWebBrowser( DUIIE *pWebBrowser )
	{
		m_pWebBrowser = pWebBrowser;
	}

	void DMIEExternal::SetEvtHandler( IDMWebEvent* pEventHandler )
	{
		m_pEvtHandler = pEventHandler;
	}

	void DMIEExternal::SetDUIWnd( DUIWND hWnd )
	{
		m_hWnd = hWnd;
	}

	HRESULT STDMETHODCALLTYPE DMIEExternal::QueryInterface( /* [in] */ REFIID riid, /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject )
	{
		if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch))
		{
			AddRef();
			*ppvObject=this;
			return S_OK;
		}else
		{
			ppvObject=NULL;
			return E_NOINTERFACE;
		}
	}

	HRESULT STDMETHODCALLTYPE DMIEExternal::GetIDsOfNames( /* [in] */ __RPC__in REFIID riid, /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames, /* [range][in] */ UINT cNames, /* [in] */ LCID lcid, /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId )
	{
		if (m_pEvtHandler)
		{
			return m_pEvtHandler->GetIDsOfNames(m_hWnd, riid, rgszNames, cNames, lcid, rgDispId);
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DMIEExternal::Invoke( /* [in] */ DISPID dispIdMember, /* [in] */ REFIID riid, /* [in] */ LCID lcid, /* [in] */ WORD wFlags, /* [out][in] */ DISPPARAMS *pDispParams, /* [out] */ VARIANT *pVarResult, /* [out] */ EXCEPINFO *pExcepInfo, /* [out] */ UINT *puArgErr )
	{
		if (m_pEvtHandler)
		{
			return m_pEvtHandler->Invoke(m_hWnd, dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
		}
		return S_OK;
	}

//////////////////////////////////////////////////////////////////////////
//DUIIE------------------------------------
	DUIIE::DUIIE()
		:m_dwCookie(0)
		,m_eventDispatch(NULL)
		,m_bShowScroll(false)	
		,m_refreshkey(DUIAccel::TranslateAccelKey(L"f5"))
	{		
		m_clsid = CLSID_WebBrowser;
	}

	DUIIE::~DUIIE()
	{
		RegisterEvtHandler(FALSE);
	}

	void DUIIE::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			if (false == m_bInit)
			{
				break;
			}
			if (DM_IsVisible())
			{
				SetActiveXVisible(TRUE);
			}
			DMSmartPtrT<IDMCanvas> pTemp;
			g_pDMRender->CreateCanvas(m_rcWindow.Width(),m_rcWindow.Height(),&pTemp);
			HDC hdc = pTemp->GetDC();
			m_axContainer->ProcessPaint(hdc, m_rcWindow,true);
			pTemp->ReleaseDC(hdc);
			pCanvas->BitBlt(pTemp,0,0,m_rcWindow);
		} while (false);

	}

	int DUIIE::OnCreate(LPVOID)
	{
		int nRet = 0;// __super::OnCreate(NULL);
		if(DMSUCCEEDED(GetContainer()->OnIsTranslucent()))
		{
			return 1;
		}
		g_pDMApp->AddMessageFilter(this);
		return nRet;
	}

	void DUIIE::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		DUIWindow::OnShowWindow(bShow,nStatus);
		if (m_bDelayInit)
		{
			if (bShow&&false == m_bInit)// 窗口显示时才初始化
			{
				InitActiveX();
				m_bInit = true;
			}
		}
		else
		{
			if (false == m_bInit)
			{
				InitActiveX();
				m_bInit = true;
			}
		}

		SetActiveXVisible(FALSE); // 在此处全部隐藏，在绘制处显示
	}

	void DUIIE::OnDestroy()
	{
		g_pDMApp->RemoveMessageFilter(this);
	}

	DMCode DUIIE::DV_OnAxActivate(IUnknown *pUnknwn)
	{
		m_pIE = pUnknwn;
		if (m_pIE)
		{
			RegisterEvtHandler(TRUE);
			m_eventDispatch.SetWebBrowser(this);
			m_external.SetWebBrowser(this);
			m_clientSite.SetWebBrowser(this);
			m_docHostUIHandler.SetWebBrowser(this);
			m_eventDispatch.SetDUIWnd(this->GetDUIWnd());
			m_external.SetDUIWnd(this->GetDUIWnd());

			DMComPtr<IOleObject> spOleObject;
			m_pIE->QueryInterface(IID_IOleObject, (void**)&spOleObject);
			if (spOleObject)
			{
				spOleObject->SetClientSite(&m_clientSite);
			}

// 			DMComPtr<IDispatch> spDisp;
// 			HRESULT hr = m_pIE->get_Document(&spDisp);
// 			
// 			if (SUCCEEDED(hr) && spDisp)
// 			{
// 				DMComPtr<IOleObject, &IID_IOleObject> spOleObject(spDisp);
// 				DMComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> spHTML(spDisp);
// 				if (spHTML)
// 				{
// 					if (spOleObject)
// 					{
// 						CComPtr<IOleClientSite> spClientSite;
// 						hr = spOleObject->GetClientSite(&spClientSite);
// 						if (SUCCEEDED(hr) && spClientSite)
// 						{
// 							m_spDefaultDocHostUIHandler = spClientSite;
// 							m_spDefaultOleCommandTarget = spClientSite;
// 						}
// 					}
// 
// 					DMComQIPtr<IOleClientSite, &IID_IOleClientSite> spOleClientSite(spDisp);
// 
// 					DMComQIPtr<ICustomDoc, &IID_ICustomDoc> spCustomDoc(spDisp);
// 					if (spCustomDoc) {
// 						spCustomDoc->SetUIHandler(&m_docHostUIHandler);
// 					}
// 				}
//			} 
			
			m_pIE->Navigate(bstr_t(m_strUrl),NULL,NULL,NULL,NULL);
		}
		return DM_ECODE_OK;
	}

	BOOL DUIIE::PreTranslateMessage(MSG* pMsg)
	{
		BOOL bRet = FALSE;
		do 
		{

			if (!m_pIE||!DM_IsVisible(true))
			{
				break;
			}

			HWND hWnd = GetContainer()->OnGetHWnd();
			// todo.在一台测试机上运行时(版本:ie10)，IE以pop-realwnd存在,在主窗口上输入文字，会自动被ie捕获焦点，从而主窗口失焦,文字输入到了ie中
			// 解决方式:1.判断realwnd是否隐藏,隐藏时,不转发快捷键消息 2.判断同一进程中焦点窗口是否为ie所在的realwnd，如果不是,不转发快捷键消息
			if (!::IsWindowVisible(hWnd))// 1
			{
				break;
			}

			if(pMsg->message == WM_KEYDOWN) //lzlong add
			{
				DUIAccel acc(m_refreshkey);
				if(pMsg->wParam == acc.GetKey())
				{
					if ((0 == acc.GetModifier()&&!PUSH_ALT&&!PUSH_CTRL&&!PUSH_SHIFT)// 未按下辅助键
						||(HOTKEYF_SHIFT == acc.GetModifier() &&PUSH_SHIFT)// Shift
						||(HOTKEYF_CONTROL == acc.GetModifier() &&PUSH_CTRL)// Ctrl
						||(HOTKEYF_ALT == acc.GetModifier() &&PUSH_ALT)//Alt
						)
					{
						Refresh();
					}
				}				
			}

			HWND hFocusWnd = ::GetFocus();// 2
			if (hFocusWnd != hWnd)
			{
				break;
			}

			DMComQIPtr<IOleInPlaceActiveObject> spInPlaceActiveObject(m_pIE);
			if (spInPlaceActiveObject)
			{
				spInPlaceActiveObject->TranslateAccelerator(pMsg);
			}
		} while (false);

		return bRet;
	}

	BOOL DUIIE::IsIEVisible()
	{
		BOOL bRet = FALSE;
		do 
		{
			if (!m_axContainer->ActiveXControl())
			{
				break;
			}
			DMComQIPtr<IOleWindow> ole_window = m_axContainer->ActiveXControl();
			if (!ole_window)
			{
				break;
			}
			HWND window = NULL;
			ole_window->GetWindow(&window);//For windowless objects, this method should always fail and return E_FAIL
			if(!window)
			{
				break;
			}
			bRet = ::IsWindowVisible(window);	
		} while (false);
		return bRet;
	}

	HRESULT DUIIE::SetEvtHandler( IDMWebEvent* pEventHandler )
	{
		m_eventDispatch.SetEvtHandler(pEventHandler);
		m_external.SetEvtHandler(pEventHandler);
		return S_OK;
	}

	HRESULT DUIIE::RegisterEvtHandler(BOOL inAdvise)
	{
		HRESULT hr = S_FALSE;
		do 
		{
			DMComQIPtr<IConnectionPointContainer> pCPC = m_pIE;
			if (!pCPC)
			{
				break;
			}
			DMComQIPtr<IConnectionPoint> pCP;
			pCPC->FindConnectionPoint(DIID_DWebBrowserEvents2,&pCP);
			if (!pCP) 
			{
				break;
			}
			if (inAdvise)
			{
				hr = pCP->Advise(&m_eventDispatch, &m_dwCookie);
			}
			else
			{
				hr = pCP->Unadvise(m_dwCookie);
			}
		} while (false);
		return hr; 

	}

	HRESULT DUIIE::OpenUrl( LPCWSTR pszURL )
	{
		HRESULT hr = S_FALSE;
		do 
		{
			m_strUrl = pszURL;// 如果IE延迟加载，保证在延迟加载时使用新的URL
			if (!m_pIE)
			{
				break;
			}
			hr = m_pIE->Navigate(bstr_t(pszURL),NULL,NULL,NULL,NULL);
		} while (false);
		return hr;
	}

	HRESULT DUIIE::GetUrl(LPWSTR pszUrl, int nMaxLen)
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			BSTR _bsURL = NULL;
			hr = m_pIE->get_LocationURL(&_bsURL);
			if (!SUCCEEDED(hr) || _bsURL == NULL)
				break;
			CStringA strUrlA =_com_util::ConvertBSTRToString(_bsURL) ;
			CStringW strUrlW = DMA2W(strUrlA, CP_UTF8);
			if (nMaxLen < strUrlW.GetLength())
			{
				break;
			}
			ZeroMemory(pszUrl, nMaxLen*sizeof(wchar_t));
			memcpy(pszUrl, strUrlW, strUrlW.GetLength()*sizeof(wchar_t));

			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::Stop()
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			VARIANT_BOOL vBusy = VARIANT_FALSE;
			m_pIE->get_Busy(&vBusy);
			// 只有浏览器繁忙时，才能停止浏览器。
			if (VARIANT_TRUE == vBusy)
				m_pIE->Stop();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::Quit()
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			m_pIE->Quit();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::Refresh()
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			m_pIE->Refresh();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::Refresh2( UINT32 nLevel )
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			VARIANT _v;
			_v.vt = VT_I4;
			_v.lVal = nLevel;
			m_pIE->Refresh2(&_v);
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::GoBack()
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			m_pIE->GoBack();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::GoForward()
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			m_pIE->GoForward();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::ExecuteScript( LPCWSTR pszScript )
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			DMComQIPtr<IHTMLDocument2> spDoc;
			hr = m_pIE->get_Document(reinterpret_cast<IDispatch**>(&spDoc));
			if (SUCCEEDED(hr) && spDoc) {
				DMComQIPtr<IHTMLWindow2> spWin;
				hr = spDoc->get_parentWindow(&spWin);
				if (SUCCEEDED(hr) && spWin) {
					_variant_t vRetCode;
					try {
						_bstr_t _bsScript(pszScript);
						hr = spWin->execScript(_bsScript, L"JavaScript", &vRetCode);
					} catch(...) {
					}
				}
			}
		} while (false);
		return hr;
	}

	HRESULT DUIIE::ExecuteScriptFuntion( LPCWSTR strFun, const DM::CArray<LPCWSTR>& vecParams, LPWSTR strResult, int nMaxLen )
	{
		HRESULT hr = E_FAIL;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			_variant_t _varErr;
			DMComQIPtr<IDispatch>  pDisp;
			hr = m_pIE->get_Document( &pDisp );
			if (SUCCEEDED(hr) && pDisp) {
				DMComQIPtr<IHTMLDocument2> spDoc;
				hr = pDisp->QueryInterface( IID_IHTMLDocument2,(void**)&spDoc );
				if (SUCCEEDED(hr) && spDoc) {
					DMComQIPtr<IDispatch> spScript;
					hr = spDoc->get_Script(&spScript);
					if (SUCCEEDED(hr) && spScript) {
						_bstr_t _bsMember(strFun);
						DISPID _dispid = 0;
						hr = spScript->GetIDsOfNames(IID_NULL, &_bsMember.GetBSTR(), 1, LOCALE_SYSTEM_DEFAULT, &_dispid);
						if (SUCCEEDED(hr)) {

							DISPPARAMS _dispparams = { 0 };
							_dispparams.cArgs = (UINT)vecParams.GetCount();
							_dispparams.cNamedArgs = 0;

							// 以前没有CComBSTR::CopyTo出来BSTR没有释放，这里换成_variant_t。by ZC. 2011-12-30.
							_variant_t* _vArrArg = new _variant_t[_dispparams.cArgs];
							for ( size_t i = 0;i < _dispparams.cArgs;i++ ) {
								size_t indx = vecParams.GetCount() - i - 1;
								// 这里自动转化为BSTR。
								_vArrArg[i] = vecParams[indx];
							}
							_dispparams.rgvarg = _vArrArg;

							EXCEPINFO _excepinfo = { 0 };
							UINT _nArgErr = (UINT)-1;	//initialize to invalid arg

							hr = spScript->Invoke(_dispid,IID_NULL,0,DISPATCH_METHOD,&_dispparams, &_varErr, &_excepinfo, &_nArgErr);

							if(_vArrArg != NULL)
							{
								delete []_vArrArg;
								_vArrArg = NULL;
							}
						}
					}
				}
			}

			if (strResult != NULL && _varErr.vt == VT_BSTR)
			{
				CStringA strResultA =_com_util::ConvertBSTRToString(_varErr.bstrVal) ;
				CStringW strResultW = DMA2W(strResultA, CP_UTF8);
				if (nMaxLen > strResultW.GetLength())
				{
					ZeroMemory(strResult, nMaxLen*sizeof(wchar_t));
					memcpy(strResult, strResultW, strResultW.GetLength()*sizeof(wchar_t));
				}
			}
		} while (false);
		return hr;
	}

	HRESULT DUIIE::DisableScriptWarning( bool bDisable )
	{
		HRESULT hr = E_FAIL;
		do 
		{
			if (!m_pIE)
			{
				break;
			}
			hr = m_pIE->put_Silent(bDisable ? VARIANT_TRUE : VARIANT_FALSE);
		} while (false);
		return hr;
	}

	void DUIIE::SetScrollBarShow( bool bShow )
	{
		m_docHostUIHandler.setScrollBar(bShow);
	}

	DMCode DUIIE::IESetAttribute(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml)
	{
		return SetAttribute(pszAttribute,pszValue,bLoadXml);
	}

	DMCode DUIIE::OnAttributeUrl(LPCWSTR pszValue, bool bLoadXml)
	{
		m_strUrl = pszValue;
		if (!bLoadXml)
		{
			OpenUrl(m_strUrl);
		}
		return DM_ECODE_OK;
	}

	DMCode DUIIE::OnAttributeShowScroll(LPCWSTR pszValue, bool bLoadXml)
	{
		dm_parsebool(pszValue,m_bShowScroll);
		SetScrollBarShow(m_bShowScroll);
		return DM_ECODE_OK;
	}

	DMCode DUIIE::OnAttributeRefreshKey(LPCWSTR pszValue, bool bLoadXml)
	{
		CStringW strValue = pszValue;
		m_refreshkey = DUIAccel::TranslateAccelKey(strValue);
		return DM_ECODE_OK;
	}

}//namespace DM