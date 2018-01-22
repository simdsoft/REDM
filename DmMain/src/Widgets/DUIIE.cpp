#include "DmMainAfx.h"
#include "DUIIE.h"
#include <mshtmlc.h>


namespace DM
{
	DMIEEvtDispatch::DMIEEvtDispatch(IDMWebEvent* pEventHandler)
		:m_pEvtHandler(pEventHandler),
		m_nRef(1),
		m_hWnd(0),
		m_bCanGoForward(false),
		m_bCanGoBack(false)
	{
	}
	void DMIEEvtDispatch::SetWebBrowser(DUIIE  *pWebBrowser)
	{
		m_pWebBrowser = pWebBrowser;
		if (m_pWebBrowser)
		{
			m_hWnd = m_pWebBrowser->GetDUIWnd();
		}
	}

	void DMIEEvtDispatch::SetEvtHandler(IDMWebEvent* pEventHandler)
	{
		m_pEvtHandler = pEventHandler;
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

	STDMETHODIMP DMIEEvtDispatch::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams,  VARIANT *pVarResult,  EXCEPINFO *pExcepInfo,  UINT *puArgErr)
	{
		if ((riid != IID_NULL) || !m_pEvtHandler)
			return E_INVALIDARG;

		HRESULT hr = S_OK;
		switch(dispIdMember)
		{
		case DISPID_BEFORENAVIGATE2:
			{
				IDispatch *pDisp = pDispParams->rgvarg[6].pdispVal;
				wchar_t *pUrl	 = pDispParams->rgvarg[5].pvarVal->bstrVal; 
				int Flags		 = pDispParams->rgvarg[4].pvarVal->intVal; 
				wchar_t *TargetFrameName = pDispParams->rgvarg[3].pvarVal->bstrVal; 
				//wchar_t *PostData		 = pDispParams->rgvarg[2].pvarVal->bstrVal;
				wchar_t *Headers		 = pDispParams->rgvarg[1].pvarVal->bstrVal;
				VARIANT_BOOL *bCancel	 = pDispParams->rgvarg[0].pboolVal;
				hr = m_pEvtHandler->BeforeNavigate2(m_hWnd,pDisp,pUrl,Flags,TargetFrameName,pDispParams->rgvarg+2,Headers,bCancel);
			}
			break;

		case DISPID_CLIENTTOHOSTWINDOW:
			{
				long *pCx = pDispParams->rgvarg[1].plVal;
				long *pCy = pDispParams->rgvarg[0].plVal;
				hr = m_pEvtHandler->ClientToHostWindow(m_hWnd,pCx,pCy);
			}
			break;

		case DISPID_COMMANDSTATECHANGE:
			{
				long         Command = pDispParams->rgvarg[1].lVal;
				VARIANT_BOOL Enable  = pDispParams->rgvarg[0].boolVal;
				if (m_pWebBrowser)
				{
					if (CSC_NAVIGATEBACK == Command)
					{
						m_bCanGoBack = (VARIANT_TRUE == Enable);
					}
					else if (CSC_NAVIGATEFORWARD == Command)
					{
						m_bCanGoForward = (VARIANT_TRUE == Enable);
					}
				}
				
				hr = m_pEvtHandler->CommandStateChange(m_hWnd,Command,Enable);
			}
			break;

		case DISPID_DOCUMENTCOMPLETE:
			{
				IDispatch *pDispArg = pDispParams->rgvarg[1].pdispVal;
				wchar_t *pURL    = pDispParams->rgvarg[0].pvarVal->bstrVal;
				hr = m_pEvtHandler->DocumentComplete(m_hWnd,pDispArg,pURL);
			}
			break;
		
		case DISPID_DOWNLOADBEGIN:
			{
				hr = m_pEvtHandler->DownloadBegin(m_hWnd);
			}
			break;
		
		case DISPID_DOWNLOADCOMPLETE:
			{
				hr = m_pEvtHandler->DownloadComplete(m_hWnd);
			}
			break;
		
		case DISPID_FILEDOWNLOAD:
			{
				VARIANT_BOOL ActiveDocument = pDispParams->rgvarg[1].boolVal;
				VARIANT_BOOL *Cancel        = pDispParams->rgvarg[0].pboolVal;
				hr = m_pEvtHandler->FileDownload(m_hWnd,ActiveDocument,Cancel);
			}
			break;

		case DISPID_NAVIGATECOMPLETE2:
			{
				IDispatch *pDisp = pDispParams->rgvarg[1].pdispVal;
				wchar_t *pUrl    = pDispParams->rgvarg[0].pvarVal->bstrVal;
				hr = m_pEvtHandler->NavigateComplete2(m_hWnd,pDisp,pUrl);
			}
			break;

		case DISPID_NAVIGATEERROR:
			{
				IDispatch *pDisp = pDispParams->rgvarg[4].pdispVal;
				wchar_t *pUrl    = pDispParams->rgvarg[3].pvarVal->bstrVal;
				wchar_t *TargetFrameName = pDispParams->rgvarg[2].pvarVal->bstrVal;
				int StatusCode   = pDispParams->rgvarg[1].pvarVal->intVal;
				VARIANT_BOOL *Cancel = pDispParams->rgvarg[0].pvarVal->pboolVal;
				hr = m_pEvtHandler->NavigateError(m_hWnd,pDisp,pUrl,TargetFrameName,StatusCode,Cancel);
			}
			break;

		case DISPID_NEWWINDOW2:
			{
				IDispatch **ppDisp   = pDispParams->rgvarg[1].ppdispVal;
				VARIANT_BOOL *Cancel = pDispParams->rgvarg[0].pboolVal;
				hr = m_pEvtHandler->NewWindow2(m_hWnd,ppDisp,Cancel);
			}
			break;

		case DISPID_NEWWINDOW3:
			{
				IDispatch **ppDisp	 = pDispParams->rgvarg[4].ppdispVal;
				VARIANT_BOOL *Cancel = pDispParams->rgvarg[3].pboolVal;
				DWORD dwFlags        = pDispParams->rgvarg[2].intVal;
				wchar_t* bstrUrlContext = pDispParams->rgvarg[1].bstrVal;
				wchar_t* bstrUrl        = pDispParams->rgvarg[0].bstrVal;
				hr = m_pEvtHandler->NewWindow3(m_hWnd,ppDisp,Cancel,dwFlags,bstrUrlContext,bstrUrl);
			}
			break;

		case DISPID_ONFULLSCREEN:
			{
				VARIANT_BOOL FullScreen = pDispParams->rgvarg[0].boolVal;
				hr = m_pEvtHandler->OnFullScreen(m_hWnd,FullScreen);
			}
			break;
		
		case DISPID_ONMENUBAR:
			{
				VARIANT_BOOL MenuBar = pDispParams->rgvarg[0].boolVal;
				hr = m_pEvtHandler->OnMenuBar(m_hWnd,MenuBar);
			}
			break;

		case DISPID_ONQUIT:
			{
				hr = m_pEvtHandler->OnQuit(m_hWnd);
			}
			break;

		case DISPID_ONSTATUSBAR:
			{
				VARIANT_BOOL StatusBar = pDispParams->rgvarg[0].boolVal;
				hr = m_pEvtHandler->OnStatusBar(m_hWnd,StatusBar);
			}
			break;

		case DISPID_ONTHEATERMODE:
			{
				VARIANT_BOOL TheaterMode = pDispParams->rgvarg[0].boolVal;
				hr = m_pEvtHandler->OnTheaterMode(m_hWnd,TheaterMode);
			}
			break;

		case DISPID_ONTOOLBAR:
			{
				VARIANT_BOOL ToolBar = pDispParams->rgvarg[0].boolVal;
				hr = m_pEvtHandler->OnToolBar(m_hWnd,ToolBar);
			}
			break;

		case DISPID_ONVISIBLE:
			{
				VARIANT_BOOL Visible = pDispParams->rgvarg[0].boolVal;
				hr = m_pEvtHandler->OnVisible(m_hWnd,Visible);
			}
			break;
		
		case DISPID_PRINTTEMPLATEINSTANTIATION:
			{
				IDispatch *pDisp = pDispParams->rgvarg[0].pdispVal;
				hr = m_pEvtHandler->PrintTemplateInstantiation(m_hWnd,pDisp);
			}
			break;
		
		case DISPID_PRINTTEMPLATETEARDOWN:
			{
				IDispatch *pDisp = pDispParams->rgvarg[0].pdispVal;
				hr = m_pEvtHandler->PrintTemplateTeardown(m_hWnd,pDisp);
			}
			break;

		case DISPID_PRIVACYIMPACTEDSTATECHANGE:
			{
				VARIANT_BOOL PrivacyImpacted = pDispParams->rgvarg[0].boolVal;
				hr = m_pEvtHandler->PrivacyImpactedStateChange(m_hWnd,PrivacyImpacted);
			}
			break;

		case DISPID_PROGRESSCHANGE:
			{
				LONG Progress 	 =	pDispParams->rgvarg[1].lVal;
				LONG ProgressMax = pDispParams->rgvarg[0].lVal;
				hr = m_pEvtHandler->ProgressChange(m_hWnd,Progress,ProgressMax);
			}
			break;

		case DISPID_PROPERTYCHANGE:
			{
				wchar_t* szProperty = pDispParams->rgvarg[0].bstrVal;
				hr = m_pEvtHandler->PropertyChange(m_hWnd,szProperty);
			}
			break;

		case DISPID_SETPHISHINGFILTERSTATUS:
			{
				LONG PhishingFilterStatus = pDispParams->rgvarg[0].lVal;
				hr = m_pEvtHandler->SetPhishingFilterStatus(m_hWnd,PhishingFilterStatus);
			}
			break;

		case DISPID_SETSECURELOCKICON:
			{
				int SecureLockIcon = pDispParams->rgvarg[0].intVal;
				hr = m_pEvtHandler->SetSecureLockIcon(m_hWnd,SecureLockIcon);
			}
			break;

		case DISPID_STATUSTEXTCHANGE:
			{
				wchar_t* Text = pDispParams->rgvarg[0].bstrVal;
				hr = m_pEvtHandler->StatusTextChange(m_hWnd,Text);
			}
			break;

		case DISPID_TITLECHANGE:
			{
				wchar_t* Text = pDispParams->rgvarg[0].bstrVal;
				hr = m_pEvtHandler->TitleChange(m_hWnd,Text);
			}
			break;
		
		case DISPID_WINDOWCLOSING:
			{
				VARIANT_BOOL IsChildWindow = pDispParams->rgvarg[1].boolVal;
				VARIANT_BOOL *Cancel = pDispParams->rgvarg[0].pboolVal;
				hr = m_pEvtHandler->WindowClosing(m_hWnd,IsChildWindow,Cancel);
			}
			break;

		case DISPID_WINDOWSETHEIGHT:
			{
				LONG Height = pDispParams->rgvarg[0].lVal;
				hr = m_pEvtHandler->WindowSetHeight(m_hWnd,Height);
			}
			break;

		case DISPID_WINDOWSETLEFT:
			{
				LONG Left = pDispParams->rgvarg[0].lVal;
				hr = m_pEvtHandler->WindowSetLeft(m_hWnd,Left);
			}
			break;

		case DISPID_WINDOWSETRESIZABLE:
			{
				VARIANT_BOOL Resizable = pDispParams->rgvarg[0].boolVal;
				hr = m_pEvtHandler->WindowSetResizable(m_hWnd,Resizable);
			}
			break;

		case DISPID_WINDOWSETTOP:
			{
				LONG Top = pDispParams->rgvarg[0].lVal;
				hr = m_pEvtHandler->WindowSetTop(m_hWnd,Top);
			}
			break;

		case DISPID_WINDOWSETWIDTH:
			{
				LONG Width = pDispParams->rgvarg[0].lVal;
				hr = m_pEvtHandler->WindowSetWidth(m_hWnd,Width);
			}	
			break;

		case DISPID_WINDOWSTATECHANGED:
			{
				DWORD dwFlags = pDispParams->rgvarg[1].ulVal;
				DWORD dwValidFlagsMask = pDispParams->rgvarg[0].ulVal;
				hr = m_pEvtHandler->WindowStateChanged(m_hWnd,dwFlags,dwValidFlagsMask);
			}
			break;
		default:
			return DISP_E_MEMBERNOTFOUND;
		}
		return hr;
	}

	///DMIEDocHostUIHandler------------------------------------------------------------------------------------
	DMIEDocHostUIHandler::DMIEDocHostUIHandler()
		: m_nRef(0)
		, m_pWebBrowser(NULL)
		, m_bShowScrollBar(false)
		, m_bShowContextMenu(true)
	{
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch) || IsEqualIID(riid, IID_IDocHostUIHandler))
		{
			AddRef();
			*ppvObject=this;
			return S_OK;
		}
		else
		{
			ppvObject=NULL;
			return E_NOINTERFACE;
		}
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
	{
		if (!m_bShowContextMenu)
		{
			return S_OK;
		}

		return E_NOTIMPL;
	}


	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO *pInfo)
	{
		if (pInfo)
		{
			pInfo->cbSize = sizeof(DOCHOSTUIINFO);
			pInfo->dwFlags = DOCHOSTUIFLAG_DIV_BLOCKDEFAULT | DOCHOSTUIFLAG_CODEPAGELINKEDFONTS | DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_NO3DBORDER;
			if (m_bShowScrollBar)								//决定是否显示滚动条
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

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::GetExternal(IDispatch **ppDispatch)
	{
		if (m_pWebBrowser)
		{
			*ppDispatch = &m_pWebBrowser->m_External;
		}
		else
		{
			*ppDispatch = NULL;
			return S_FALSE;
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::TranslateUrl(DWORD dwTranslate, __in __nullterminated OLECHAR *pchURLIn,  __out OLECHAR **ppchURLOut)
	{
		*ppchURLOut = 0;
		return(S_FALSE);
	}

	HRESULT STDMETHODCALLTYPE DMIEDocHostUIHandler::FilterDataObject(IDataObject *pDO,  IDataObject **ppDORet)
	{
		*ppDORet = 0;
		return S_FALSE;
	}

	///DMIEOleClientSite------------------------------------------------------------------------------------
	DMIEOleClientSite::DMIEOleClientSite()
		: m_nRef(0)
	{
	}

	void DMIEOleClientSite::SetWebBrowser(DUIIE* pWebBrowser)
	{
		m_pWebBrowser = pWebBrowser;
	}

	STDMETHODIMP DMIEOleClientSite::QueryInterface(REFIID riid, /* [iid_is][out] */ VOID **ppvObject)
	{
		if(IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IOleClientSite))
		{
			AddRef();
			*ppvObject=this;
			return S_OK;
		}
		else if (IsEqualIID(riid, IID_IDocHostUIHandler))
		{
			m_pWebBrowser->m_DocHostUIHandler.AddRef();
			*ppvObject=&(m_pWebBrowser->m_DocHostUIHandler);
			return S_OK;
		}
		else
		{
			ppvObject=NULL;
			return E_NOINTERFACE;
		}
	}

	///DMIEExternal------------------------------------------------------------------------------------
	DMIEExternal::DMIEExternal()
		: m_nRef(0)
		, m_hDUIWnd(0)
		, m_pWebBrowser(NULL), m_pEvtHandler(NULL)
	{
	}

	void DMIEExternal::SetWebBrowser(DUIIE *pWebBrowser)
	{
		m_pWebBrowser = pWebBrowser;
		if (m_pWebBrowser)
		{
			m_hDUIWnd = m_pWebBrowser->GetDUIWnd();
		}
	}

	void DMIEExternal::SetEvtHandler(IDMWebEvent* pEventHandler)
	{
		m_pEvtHandler = pEventHandler;
	}

	HRESULT STDMETHODCALLTYPE DMIEExternal::QueryInterface(REFIID riid, /* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
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

	HRESULT STDMETHODCALLTYPE DMIEExternal::GetIDsOfNames(__RPC__in REFIID riid, /* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames, /* [range][in] */ UINT cNames, LCID lcid, /* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId)
	{
		if (m_pEvtHandler)
		{
			return m_pEvtHandler->GetIDsOfNames(m_hDUIWnd, riid, rgszNames, cNames, lcid, rgDispId);
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DMIEExternal::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams,  VARIANT *pVarResult,  EXCEPINFO *pExcepInfo,  UINT *puArgErr)
	{
		if (m_pEvtHandler)
		{
			return m_pEvtHandler->Invoke(m_hDUIWnd, dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
		}
		return S_OK;
	}

	///DUIIE------------------------------------------------------------------------------------
	DUIIE::DUIIE()
		:m_dwCookie(0)
		,m_EventDispatch(NULL)
		,m_bShowScroll(false)	
		,m_bShowContext(true)
		,m_bDisableScriptWarn(false)
		,m_Refreshkey(DUIAccel::TranslateAccelKey(L"f5"))
	{		
		m_ClsId = CLSID_WebBrowser;
	}

	void DUIIE::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			if (false == m_bInit || NULL == m_pAxContainer)
			{
				break;
			}
			if (DM_IsVisible(true))
			{
				SetActiveXVisible(true);
			}
			DMSmartPtrT<IDMCanvas> pTemp;
			g_pDMRender->CreateCanvas(m_rcWindow.Width(),m_rcWindow.Height(),&pTemp);
			HDC hdc = pTemp->GetDC();
			m_pAxContainer->ProcessPaint(hdc, m_rcWindow,true);
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

	void DUIIE::OnDestroy()
	{
		g_pDMApp->RemoveMessageFilter(this);
		SetEvtHandler(NULL);
		RegisterEvtHandler(FALSE);
		SetMsgHandled(FALSE);
	}

	void DUIIE::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		DUIWindow::OnShowWindow(bShow,nStatus);
		if (m_bDelayInit)
		{
			if (DM_IsVisible(true)&&false == m_bInit)// 窗口显示时才初始化
			{
				m_bInit = InitActiveX();
			}
		}
		else
		{
			if (false == m_bInit)
			{
				m_bInit = InitActiveX();
			}
		}
		
		SetActiveXVisible(false);// 在此处全部隐藏，在绘制处显示
	}

	DMCode DUIIE::DV_OnAxActivate(IUnknown *pUnknwn)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}

			RegisterEvtHandler(true);
			m_EventDispatch.SetWebBrowser(this);
			m_External.SetWebBrowser(this);
			m_ClientSite.SetWebBrowser(this);
			m_DocHostUIHandler.SetWebBrowser(this);

			DMComPtr<IOleObject> spOleObject;
			pWeb->QueryInterface(IID_IOleObject, (void**)&spOleObject);
			if (spOleObject)
			{
				spOleObject->SetClientSite(&m_ClientSite);
			}
			DisableScriptWarning(m_bDisableScriptWarn);
			pWeb->Navigate(bstr_t(m_strUrl),NULL,NULL,NULL,NULL);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	BOOL DUIIE::PreTranslateMessage(MSG* pMsg)
	{
		BOOL bRet = FALSE;
		do 
		{
			// 快捷键只考虑键盘消息
			if (NULL == pMsg
				||(WM_KEYDOWN != pMsg->message && WM_KEYUP != pMsg->message))
			{
				break;;
			}
			
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb || !DM_IsVisible(true))
			{
				break;
			}

			HWND hWnd = GetContainer()->OnGetHWnd();
			// todo.在一台测试机上运行时(版本:ie10)，IE以pop-realwnd存在,在主窗口上输入文字，会自动被ie捕获焦点，从而主窗口失焦,文字输入到了ie中
			// 解决方式:1.判断realwnd是否隐藏,隐藏时,不转发快捷键消息 2.判断同一进程中激活窗口是否为ie所在的realwnd，如果不是,不转发快捷键消息
			if (!::IsWindowVisible(hWnd))// 1
			{
				break;
			}

			if(pMsg->message == WM_KEYDOWN) //lzlong add
			{
				DUIAccel acc(m_Refreshkey);
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

			HWND hFocusWnd = ::GetFocus();
			HWND hFocusTopWnd = hFocusWnd;	
			while (hFocusTopWnd && hFocusTopWnd != hWnd)// 判断焦点窗口是否在hWnd的子窗口列中
			{
				hFocusTopWnd = ::GetParent(hFocusTopWnd);
			}
			if (hFocusTopWnd != hWnd // 焦点窗口不在hWnd的子窗口列中
				|| hFocusWnd == hWnd)// 焦点窗口在hWnd的子窗口列中,并且不是hWnd
			{
				break;
			}
			
			DMComQIPtr<IOleInPlaceActiveObject> spInPlaceActiveObject = pWeb;
			if (spInPlaceActiveObject)
			{			
				bRet =(S_OK == spInPlaceActiveObject->TranslateAccelerator(pMsg))?true:false;
			}
			
		} while (false);

		return bRet;
	}

	HRESULT DUIIE::SetEvtHandler(IDMWebEvent* pEventHandler)
	{
		m_EventDispatch.SetEvtHandler(pEventHandler);
		m_External.SetEvtHandler(pEventHandler);
		return S_OK;
	}

	HRESULT DUIIE::RegisterEvtHandler(bool inAdvise)
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComQIPtr<IConnectionPointContainer> pCPC = Ptr();
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
				hr = pCP->Advise(&m_EventDispatch, &m_dwCookie);
			}
			else
			{
				hr = pCP->Unadvise(m_dwCookie);
			}
		} while (false);
		return hr; 

	}

	HRESULT DUIIE::OpenUrl(LPCWSTR pszURL,int iFlags /*= 0*/,LPCWSTR pszTargetFrameName /*= NULL*/,LPCWSTR pszHeaders /*= NULL*/, LPCSTR pszPostData /*= NULL*/, int iPostDataLen /*=0*/)
	{
		HRESULT hr = E_FAIL;
		do 
		{
			if (!IsValidString(pszURL))
			{
				break;
			}
		
			m_strUrl = pszURL;// 如果IE延迟加载，保证在延迟加载时使用新的URL
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			_variant_t vUrl(pszURL);
			_variant_t vFlags((iFlags!=-1)?(long)iFlags:(long)navNoReadFromCache, VT_I4);
			_variant_t vTargetFrameName;
			_variant_t vPostData;
			_variant_t vHeader;
			_variant_t vNull;
			
			if (IsValidString(pszTargetFrameName)) 
			{
				vTargetFrameName = pszTargetFrameName;
			}
			if (IsValidString(pszHeaders))
			{
				vHeader = pszHeaders;
			}
			if (IsValidString(pszPostData) && iPostDataLen>0)
			{
				SAFEARRAYBOUND sab = {(ULONG)iPostDataLen,0};
				SAFEARRAY* pSF = ::SafeArrayCreate(VT_UI1, 1, &sab);
				for (ULONG lIndex = 0; lIndex < sab.cElements; lIndex++) 
				{
					::SafeArrayPutElement(pSF, (LONG*)&lIndex, (void*)(&pszPostData[lIndex]));
				}
				vPostData.vt = VT_ARRAY | VT_UI1;
				vPostData.parray = pSF;
				hr = pWeb->Navigate2(&vUrl, &vFlags, &vTargetFrameName, &vPostData, &vHeader);
				::SafeArrayDestroy(pSF);
			}
			else
			{
				hr = pWeb->Navigate2(&vUrl, &vNull, &vTargetFrameName, &vNull, &vHeader);
			}
		} while (false);
		return hr;
	}

	HRESULT DUIIE::GetUrl(LPWSTR pszUrl, int nMaxLen)
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			BSTR _bsURL = NULL;
			hr = pWeb->get_LocationURL(&_bsURL);
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

	CStringW DUIIE::GetUrl()
	{
		CStringW strUrl;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			BSTR _bsURL = NULL;
			HRESULT hr = pWeb->get_LocationURL(&_bsURL);
			if (!SUCCEEDED(hr) || _bsURL == NULL)
				break;
			CStringA strUrlA =_com_util::ConvertBSTRToString(_bsURL) ;
			strUrl = DMA2W(strUrlA, CP_UTF8);
		} while (false);
		return strUrl;
	}

	HRESULT DUIIE::SetWebFocus()
	{// 感谢Liufy大佬提供的代码
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}

			DMComQIPtr<IHTMLDocument2> spDoc;
			hr = pWeb->get_Document(reinterpret_cast<IDispatch**>(&spDoc));
			if (SUCCEEDED(hr) && spDoc)
			{
				DMComQIPtr<IHTMLWindow2> spWin;
				hr = spDoc->get_parentWindow(&spWin);
				if (SUCCEEDED(hr) && spWin)
				{
					try 
					{
						hr = spWin->focus();
					} 
					catch(...) 
					{
					}
				}
			}
		} while (false);
		return hr;
	}

	HWND DUIIE::GetOleWindow()
	{
		HWND hOleWnd = NULL;
		do 
		{
			if (NULL == m_pAxContainer)
			{
				break;
			}

			DMComPtr<IUnknown> pUnknown = m_pAxContainer->ActiveXControl();
			if (NULL == pUnknown)
			{
				break;
			}

			DMComQIPtr<IOleWindow> pOleWnd = pUnknown;
			if (NULL == pOleWnd)
			{
				break;
			}
			pOleWnd->GetWindow(&hOleWnd);//For windowless objects, this method should always fail and return E_FAIL：eg: IE [Shell Embedding] window
		} while (false);
		return hOleWnd;
	}

	HWND DUIIE::GetIESWindow()
	{
		HWND hIESWnd = NULL;
		do 
		{
			HWND hWnd = GetOleWindow();
			if (!hWnd)
			{
				break;
			}
			hWnd = ::FindWindowEx(hWnd, NULL, L"Shell DocObject View", NULL);
			if (NULL == hWnd)
			{
				break;
			}
			hIESWnd = ::FindWindowEx(hWnd, NULL, L"Internet Explorer_Server", NULL);
		} while (false);
		return hIESWnd;
	}

	DMComPtr<IWebBrowser2> DUIIE::Ptr()
	{
		DMComQIPtr<IWebBrowser2> ptr;
		if (m_pAxContainer)
		{
			ptr = m_pAxContainer->ActiveXControl();
		}
		return ptr;
	}

	HRESULT DUIIE::UpdateWebRect(LPRECT lpRect)
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (NULL == lpRect || !pWeb)
			{
				break;
			}
			m_rcWindow = lpRect;
			SetActiveXRect(m_rcWindow);
			hr = S_OK;
		} while (false);
		return hr;
	}

	bool DUIIE::IsBusy()
	{
		VARIANT_BOOL vBusy = VARIANT_FALSE;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			
			pWeb->get_Busy(&vBusy);
		} while (false);
		return VARIANT_TRUE == vBusy;
	}

	HRESULT DUIIE::Stop()
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			VARIANT_BOOL vBusy = VARIANT_FALSE;
			pWeb->get_Busy(&vBusy);
			// 只有浏览器繁忙时，才能停止浏览器。
			if (VARIANT_TRUE == vBusy)
				pWeb->Stop();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::Quit()
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			pWeb->Quit();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::Refresh()
	{
		HRESULT hr = E_FAIL;
		do 
		{
			if (m_strUrl.IsEmpty() || 0 == m_strUrl.CompareNoCase(L"about:blank"))
			{
				break;
			}

			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			pWeb->Refresh();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::Refresh2(UINT32 nLevel)
	{
		HRESULT hr = E_FAIL;
		do 
		{
			if (m_strUrl.IsEmpty() || 0 == m_strUrl.CompareNoCase(L"about:blank"))
			{
				break;
			}

			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			VARIANT _v;
			_v.vt = VT_I4;
			_v.lVal = nLevel;
			pWeb->Refresh2(&_v);
			hr = S_OK;
		} while (false);
		return hr;
	}

	bool DUIIE::CanGoBack()
	{
		return m_EventDispatch.m_bCanGoBack;
	}

	HRESULT DUIIE::GoBack()
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			pWeb->GoBack();
			hr = S_OK;
		} while (false);
		return hr;
	}

	bool DUIIE::CanGoForward()
	{
		return m_EventDispatch.m_bCanGoForward;
	}

	HRESULT DUIIE::GoForward()
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			pWeb->GoForward();
			hr = S_OK;
		} while (false);
		return hr;
	}

	HRESULT DUIIE::ExecuteScript(LPCWSTR pszScript)
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			DMComQIPtr<IHTMLDocument2> spDoc;
			hr = pWeb->get_Document(reinterpret_cast<IDispatch**>(&spDoc));
			if (SUCCEEDED(hr) && spDoc)
			{
				DMComQIPtr<IHTMLWindow2> spWin;
				hr = spDoc->get_parentWindow(&spWin);
				if (SUCCEEDED(hr) && spWin)
				{
					_variant_t vRetCode;
					try 
					{
						_bstr_t _bsScript(pszScript);
						hr = spWin->execScript(_bsScript, L"JavaScript", &vRetCode);
					} 
					catch(...) 
					{
					}
				}
			}
		} while (false);
		return hr;
	}

	HRESULT DUIIE::ExecuteScriptFuntion(LPCWSTR pszFun, const DM::CArray<LPCWSTR>& vecParams, LPWSTR strResult, int nMaxLen)
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			_variant_t _varErr;
			DMComQIPtr<IDispatch>  pDisp;
			hr = pWeb->get_Document(&pDisp);
			if (SUCCEEDED(hr) && pDisp)
			{
				DMComQIPtr<IHTMLDocument2> spDoc;
				hr = pDisp->QueryInterface(IID_IHTMLDocument2,(void**)&spDoc);
				if (SUCCEEDED(hr) && spDoc)
				{
					DMComQIPtr<IDispatch> spScript;
					hr = spDoc->get_Script(&spScript);
					if (SUCCEEDED(hr) && spScript) 
					{
						_bstr_t _bsMember(pszFun);
						DISPID _dispid = 0;
						hr = spScript->GetIDsOfNames(IID_NULL, &_bsMember.GetBSTR(), 1, LOCALE_SYSTEM_DEFAULT, &_dispid);
						if (SUCCEEDED(hr))
						{

							DISPPARAMS _dispparams = { 0 };
							_dispparams.cArgs = (UINT)vecParams.GetCount();
							_dispparams.cNamedArgs = 0;

							// 以前没有CComBSTR::CopyTo出来BSTR没有释放，这里换成_variant_t。by ZC. 2011-12-30.
							_variant_t* _vArrArg = new _variant_t[_dispparams.cArgs];
							for (size_t i = 0;i < _dispparams.cArgs;i++)
							{
								size_t indx = vecParams.GetCount() - i - 1;
								// 这里自动转化为BSTR。
								_vArrArg[i] = vecParams[indx];
							}
							_dispparams.rgvarg = _vArrArg;

							EXCEPINFO _excepinfo = {0};
							UINT _nArgErr = (UINT)-1;	//initialize to invalid arg

							hr = spScript->Invoke(_dispid,IID_NULL,0,DISPATCH_METHOD,&_dispparams, &_varErr, &_excepinfo, &_nArgErr);

							DM_DELETE_ARRAY(_vArrArg);
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

	HRESULT DUIIE::DisableScriptWarning(bool bDisable)
	{
		HRESULT hr = E_FAIL;
		do 
		{
			DMComPtr<IWebBrowser2> pWeb = Ptr();
			if (!pWeb)
			{
				break;
			}
			hr = pWeb->put_Silent(bDisable ? VARIANT_TRUE : VARIANT_FALSE);
		} while (false);
		return hr;
	}

	HRESULT DUIIE::SetScrollBarShow(bool bShow)
	{
		m_DocHostUIHandler.SetScrollBarShow(bShow);
		return S_OK;
	}

	HRESULT DUIIE::SetContextMenuShow(bool bShow)
	{
		m_DocHostUIHandler.SetContextMenuShow(bShow);
		return S_OK;
	}

	HRESULT DUIIE::WebSetAttribute(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml)
	{
		if (DMSUCCEEDED(SetAttribute(pszAttribute,pszValue,bLoadXml)))
		{
			return S_OK;
		}
		return S_FALSE;
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
		if (false == bLoadXml)
		{
			Refresh();
		}
		
		return DM_ECODE_OK;
	}

	DMCode DUIIE::OnAttributeShowContext(LPCWSTR pszValue, bool bLoadXml)
	{
		dm_parsebool(pszValue,m_bShowContext);
		SetContextMenuShow(m_bShowContext);
		if (false == bLoadXml)
		{
			Refresh();
		}

		return DM_ECODE_OK;
	}

	DMCode DUIIE::OnAttributeDisableScriptWarn(LPCWSTR pszValue, bool bLoadXml)
	{
		dm_parsebool(pszValue,m_bDisableScriptWarn);
		DisableScriptWarning(m_bDisableScriptWarn);
		return DM_ECODE_OK;
	}

	DMCode DUIIE::OnAttributeRefreshKey(LPCWSTR pszValue, bool bLoadXml)
	{
		CStringW strValue = pszValue;
		m_Refreshkey = DUIAccel::TranslateAccelKey(strValue);
		return DM_ECODE_OK;
	}

}//namespace DM