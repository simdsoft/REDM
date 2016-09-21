#include "DmMainAfx.h"
#include "DUIEditHelper.h"
#include "DUIEdit.h"


namespace DM
{
	// 全局控制
	// 8d33f740-cf58-11ce-a89d-00aa006cadc5
	EXTERN_C const IID IID_ITextServices = {0x8d33f740,0xcf58,0x11ce,{0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}};
	// c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 
	EXTERN_C const IID IID_ITextHost ={0xc5bdd8d0,0xd26e,0x11ce,{0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}};

	#ifndef HIMETRIC_PER_INCH
	#define HIMETRIC_PER_INCH 2540
	#endif
	
	/// DUIEditHelper------------------------------------------
	DUIEditHelper::DUIEditHelper()
	{
		m_rich20 = LoadLibrary(_T("riched20.dll"));
		if (m_rich20) 
		{
			m_funCreateTextServices = (PCreateTextServices)GetProcAddress(m_rich20,"CreateTextServices");
		}
		else
		{
			DMASSERT_EXPR(0,L"加载riched20失败了！！");
		}
	}

	DUIEditHelper::~DUIEditHelper()
	{
		if (m_rich20)
		{
			::FreeLibrary(m_rich20);
		}

		m_funCreateTextServices = NULL;
	}

	HRESULT DUIEditHelper::CreateTextServices(IUnknown *punkOuter, ITextHost *pITextHost, IUnknown **ppUnk)
	{
		if (!m_funCreateTextServices) 
		{
			return E_NOTIMPL;
		}
		return m_funCreateTextServices(punkOuter,pITextHost,ppUnk);
	}
	///---------------------------
	DUICaret::DUICaret()
	{
		m_dwPreFrameTime = 0;
		m_bBlinking      = false;
		m_clr            = PBGRA(0,0,0,0xff);
		SetAniCount(6);
	}

	void DUICaret::SetAniCount(int iAniCount)
	{
		do 
		{
			if (m_iAniCount==iAniCount)
			{
				break;
			}
			m_iAniCount = iAniCount;
			if (m_iAniCount<=0)
			{
				m_iAniCount = 1;
			}

			m_iAniDelta = 255/m_iAniCount;
			m_dwDelta   = GetCaretBlinkTime()/m_iAniCount;
			m_iCurAlpha = 255-m_iAniDelta*m_iAniCount;
		} while (false);
	}

	void DUICaret::Show()
	{
		do 
		{
			if (DM_IsVisible(true))
			{
				DM_Invalidate();
				break;
			}

			m_bBlinking = true;
			m_iCurAlpha = 255;
		
			GetContainer()->OnRegisterTimeline(this);
			DM_SetVisible(true,true);
		} while (false);

	}

	void DUICaret::Hide()
	{
		do 
		{
			if (!DM_IsVisible(true))
			{
				break;
			}
			DM_SetVisible(false,true);
		} while (false);
	}

	void DUICaret::SetPos(CPoint point)
	{
		do 
		{
			if (point.x<0||point.y<0)
			{
				break;
			}
			if (m_pt == point)
			{
				break;
			}

			m_pt = point;
			CStringW strValue;
			strValue.Format(L"%d,%d,@%d,@%d",m_pt.x,m_pt.y,m_Size.cx,m_Size.cy);
			m_pLayout->SetAttribute(L"pos",strValue,false);
			DM_UpdateLayout(NULL);
			m_rcWindow;
		} while (false);
	}

	void DUICaret::SetSize(CSize size)
	{
		do 
		{
			if (size == m_Size)
			{
				break;
			}
			m_Size = size;
		} while (false);
	}

	bool DUICaret::IsBlinking()
	{
		return m_bBlinking;
	}

	void DUICaret::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			if (m_iCurAlpha>255)
			{
				m_iCurAlpha = 255;
			}
			if (m_iCurAlpha<0)
			{
				m_iCurAlpha = 0;
			}
			byte alpha = 0xff;
			m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
			alpha = alpha*m_iCurAlpha/255;
			DMColor clr = m_clr;
			clr.PreMultiply(alpha);
			pCanvas->FillSolidRect(&m_rcWindow,clr);
			
			if (GetTickCount()-m_dwPreFrameTime>m_dwDelta)
			{
				if (m_bBlinking)
				{
					m_iCurAlpha += m_iAniDelta;
					if (m_iCurAlpha>=255)
					{
						m_bBlinking = false;
					}
				}
				else
				{
					m_iCurAlpha -= m_iAniDelta;
					if (m_iCurAlpha<=0)
					{
						m_bBlinking = true;
					}
				}
				m_dwPreFrameTime = GetTickCount();
			}

		} while (false);
	}

	BOOL DUICaret::DM_OnEraseBkgnd(IDMCanvas* pCanvas)
	{
		return TRUE;
	}
	
	void DUICaret::OnDestroy()
	{
		GetContainer()->OnUnregisterTimeline(this);
		__super::OnDestroy();
	}

	DMCode DUICaret::OnTimeline()
	{
		do 
		{
			if (!DM_IsVisible(true))
			{
				break;
			}
			if (GetTickCount()-m_dwPreFrameTime<m_dwDelta)
			{
				break;
			}
			DM_Invalidate();
		} while (false);
		return DM_ECODE_OK;
	}

	/// DUIRicheditDropTarget
	DUIRicheditDropTarget::DUIRicheditDropTarget(ITextServices *pTxtSvr)
		:m_pServ(pTxtSvr)
	{
		Init();
	}

	DUIRicheditDropTarget::~DUIRicheditDropTarget()
	{
		UnInit();
	}

	void DUIRicheditDropTarget::Init()
	{
		m_nRef = 1;
		if (m_pServ)
		{
			m_pServ->AddRef();
		}
	}

	void DUIRicheditDropTarget::UnInit()
	{
		if (m_pServ)
		{
			m_pServ->Release();
		}
	}

	//IUnkown
	HRESULT STDMETHODCALLTYPE DUIRicheditDropTarget::QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		HRESULT hr = S_OK;// E_NOINTERFACE;
		do 
		{
			if (__uuidof(IUnknown) == riid)
			{
				*ppvObject=(IUnknown*) this;
				AddRef();
				break;
			}
			if(__uuidof(IDropTarget) == riid)
			{
				*ppvObject=(IDropTarget*)this;
				AddRef();
				break;
			}

			hr = E_NOINTERFACE;
		} while (false);
		return hr;
	}

	ULONG STDMETHODCALLTYPE DUIRicheditDropTarget::AddRef(void)
	{
		return ++m_nRef;
	}

	ULONG STDMETHODCALLTYPE DUIRicheditDropTarget::Release(void)
	{ 
		ULONG uRet = --m_nRef;
		if (0 == uRet)
		{
			delete this;
		}
		return uRet;
	}

	/// IDropTarget
	HRESULT STDMETHODCALLTYPE DUIRicheditDropTarget::DragEnter(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect)
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (NULL == m_pServ)
			{
				break;
			}

			IDropTarget *pDropTarget = NULL;
			hr = m_pServ->TxGetDropTarget(&pDropTarget);
			if (SUCCEEDED(hr))
			{
				hr = pDropTarget->DragEnter(pDataObj,grfKeyState,pt,pdwEffect);
				*pdwEffect = DROPEFFECT_COPY;
				pDropTarget->Release();
			}
		} while (false);

		return hr;
	}

	HRESULT STDMETHODCALLTYPE DUIRicheditDropTarget::DragOver(DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect)
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (NULL == m_pServ)
			{
				break;
			}
			IDropTarget* pDropTarget = NULL;
			hr = m_pServ->TxGetDropTarget(&pDropTarget);
			if (SUCCEEDED(hr))
			{
				hr = pDropTarget->DragOver(grfKeyState,pt,pdwEffect);
				*pdwEffect = DROPEFFECT_COPY;
				pDropTarget->Release();
			}

		} while (false);
		return hr;
	}

	HRESULT STDMETHODCALLTYPE DUIRicheditDropTarget::DragLeave(void)
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (NULL == m_pServ)
			{
				break;
			}

			IDropTarget* pDropTarget = NULL;
			hr = m_pServ->TxGetDropTarget(&pDropTarget);
			if (SUCCEEDED(hr))
			{
				hr = pDropTarget->DragLeave();
				pDropTarget->Release();
			}

		} while (false);

		return hr;
	}

	HRESULT STDMETHODCALLTYPE DUIRicheditDropTarget::Drop(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState, POINTL pt,__RPC__inout DWORD *pdwEffect)
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (NULL == m_pServ)
			{
				break;
			}
			if (DROPEFFECT_NONE == *pdwEffect) 
			{
				break;
			}

			IDropTarget* pDropTarget = NULL;
			hr = m_pServ->TxGetDropTarget(&pDropTarget);
			if (SUCCEEDED(hr))
			{
				hr = pDropTarget->Drop(pDataObj,grfKeyState,pt,pdwEffect);
				pDropTarget->Release();
			}

		} while (false);

		return hr;
	}

	/// DUITextHost
	DUITextHost::DUITextHost(void)
	{
		Init();
	}

	DUITextHost::~DUITextHost(void)
	{
		UnInit();
	}

	void DUITextHost::Init()
	{
		m_pRichEdit = NULL;
		m_uRefNum   = 0;
		m_bUiActive = false;
		m_pServ     = NULL;
	}

	void DUITextHost::UnInit()
	{
		if (m_pServ)
		{
			m_pServ->Release();
		}
	}

	bool DUITextHost::Init(DUIRichEdit* pRichEdit)
	{
		bool bRet = false;
		do 
		{
			m_pRichEdit = pRichEdit;
			IUnknown *pUnk = NULL;// 在RichEdit初始化之前初始化DUIEditHelper
			if (FAILED(DUIEditHelper::getSingleton().CreateTextServices(NULL, this, &pUnk))) 
			{
				break;
			}

			HRESULT hr = pUnk->QueryInterface(IID_ITextServices,(void **)&m_pServ);
			pUnk->Release();
			bRet = SUCCEEDED(hr);
		} while (false);
		return bRet;
	}

	ITextServices* DUITextHost::GetTextService()
	{
		return m_pServ;
	}

	POINT DUITextHost::GetCaretPos()
	{
		return m_ptCaret;
	}

	// IUnknown
	HRESULT _stdcall DUITextHost::QueryInterface(REFIID riid, void **ppvObject)
	{
		HRESULT hr = E_NOINTERFACE;
		do 
		{
			*ppvObject = NULL;
			if (IsEqualIID(riid, IID_IUnknown)
				|| IsEqualIID(riid, IID_ITextHost))
			{
				AddRef();
				*ppvObject = (ITextHost *) this;
				hr = S_OK;
			}
		} while (false);
		return hr;
	}

	ULONG _stdcall DUITextHost::AddRef( void )
	{
		return ++m_uRefNum;
	}

	ULONG _stdcall DUITextHost::Release( void )
	{
		ULONG uRefNum = --m_uRefNum;

		if (0 == uRefNum)
		{
			delete this;
		}

		return uRefNum;
	}

	// ITextHost
	HDC DUITextHost::TxGetDC()// hgytest pass
	{
		HDC hdc = NULL;
		if (m_pRichEdit)
		{
			hdc = ::GetDC(m_pRichEdit->GetContainer()->OnGetHWnd());
		}
		else
		{
			hdc = ::GetDC(NULL);
		}
		return hdc;
	}

	INT DUITextHost::TxReleaseDC(HDC hdc)// hgytest pass
	{
		HWND hWnd = NULL;
		if (m_pRichEdit)
		{
			hWnd = m_pRichEdit->GetContainer()->OnGetHWnd();
		}
		return ::ReleaseDC(hWnd,hdc);

	}

	BOOL DUITextHost::TxShowScrollBar(INT fnBar, BOOL fShow)// hgytest pass
	{
		int wBar = 0;
		switch (fnBar)
		{
		case SB_BOTH:
			{
				wBar = DMSB_BOTH;
			}
			break;

		case SB_VERT:
			{
				wBar = DMSB_VERT;
			}
			break;

		case SB_HORZ:
			{
				wBar = DMSB_HORZ;
			}
			break;
		}
		return m_pRichEdit->ShowScrollBar(wBar,TRUE==fShow);
	}

	BOOL DUITextHost::TxEnableScrollBar(INT fuSBFlags, INT fuArrowflags)// hgytest pass
	{
		int wBar = 0;
		switch (fuSBFlags)
		{
		case SB_BOTH:
			{
				wBar = DMSB_BOTH;
			}
			break;
		case SB_VERT:
			{
				wBar = DMSB_VERT;
			}
			break;

		case SB_HORZ:
			{
				wBar = DMSB_HORZ;
			}
			break;
		}
		return m_pRichEdit->EnableScrollBar(wBar,ESB_ENABLE_BOTH == fuArrowflags);
	}

	BOOL DUITextHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)// hgytest pass，当滚动范围变化时触发
	{
		return m_pRichEdit->SetScrollRange(fnBar!=SB_HORZ,nMinPos,nMaxPos,TRUE==fRedraw);
	}

	BOOL DUITextHost::TxSetScrollPos(INT fnBar, INT nPos, BOOL fRedraw)// hgytest pass，随字符变化自动触发设置滚动条滑块位置
	{
		BOOL bRet = FALSE;
		do 
		{
			if (m_pRichEdit->m_bScrollPending)
			{
				bRet = TRUE;
				break;
			}
			m_pRichEdit->m_bScrollPending = TRUE;
			bRet = m_pRichEdit->SetScrollPos(fnBar!=SB_HORZ,nPos,TRUE==fRedraw);
			m_pRichEdit->m_bScrollPending = FALSE;

		} while (false);
		return bRet;
	}

	void DUITextHost::TxInvalidateRect(LPCRECT lpRect, BOOL fMode)
	{
		if (lpRect && !fMode)
		{
			CRect rc = lpRect;
			m_pRichEdit->DM_InvalidateRect(rc);
		}
		else
		{ 
			m_pRichEdit->DM_InvalidateRect(m_pRichEdit->m_rcWindow);
		}
	}

	void DUITextHost::TxViewChange(BOOL fUpdate)
	{
		if (fUpdate)
		{
			m_pRichEdit->DM_InvalidateRect(m_pRichEdit->m_rcWindow);
		}
	}

	BOOL DUITextHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)// hgytest pass，插入符显示前自动触发
	{
		m_pRichEdit->InitCaret(xWidth,yHeight);
		return TRUE;
	}

	BOOL DUITextHost::TxShowCaret(BOOL fShow)// hgytest pass，设置焦点时自动触发
	{
		BOOL bRet = FALSE;
		do 
		{
			m_pRichEdit->ShowCaret(TRUE == fShow);
			bRet = TRUE;
		} while (false);
		return bRet;
	}

	BOOL DUITextHost::TxSetCaretPos(INT x, INT y)// hgytest pass 靠近边界时x.y均为-32000
	{
		m_pRichEdit->ResetCaret(CPoint(x,y));
		m_ptCaret.x = x;
		m_ptCaret.y = y;
		return TRUE;
	}

	BOOL DUITextHost::TxSetTimer(UINT idTimer, UINT uTimeout)// hgytest pass,在rtf或动画中调用
	{
		return DMSUCCEEDED(m_pRichEdit->DM_SetThreadTimer(idTimer,uTimeout));
	}

	void DUITextHost::TxKillTimer(UINT idTimer)// hgytest pass,在插入符改变时调用
	{
		if (!DMSUCCEEDED(m_pRichEdit->DM_KillThreadTimer(idTimer)))
		{
			HWND hWnd = m_pRichEdit->GetContainer()->OnGetHWnd();
			::KillTimer(hWnd,idTimer);
		}
	}

	void DUITextHost::TxScrollWindowEx(INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll)
	{// hgytest pass,暂未调用
		m_pRichEdit->DM_Invalidate();
	}

	void DUITextHost::TxSetCapture(BOOL fCapture)// hgytest pass
	{
		if (fCapture)
		{
			m_pRichEdit->DM_SetCapture();
		}
		else
		{
			m_pRichEdit->DM_ReleaseCapture();
		}
	}

	void DUITextHost::TxSetFocus()// hgytest pass
	{
		m_pRichEdit->DV_SetFocusWnd();
	}

	void DUITextHost::TxSetCursor(HCURSOR hcur, BOOL fText)// hgytest pass
	{
		::SetCursor(hcur);
	}

	BOOL DUITextHost::TxScreenToClient(LPPOINT lppt)// hgytest pass
	{
		return ::ScreenToClient(m_pRichEdit->GetContainer()->OnGetHWnd(),lppt);
	}

	BOOL DUITextHost::TxClientToScreen(LPPOINT lppt)// hgytest pass,暂未调用
	{
		return ::ClientToScreen(m_pRichEdit->GetContainer()->OnGetHWnd(),lppt);
	}

	HRESULT DUITextHost::TxActivate(LONG *plOldState)// hgytest pass
	{
		*plOldState = m_bUiActive;
		m_bUiActive = true;
		return S_OK;
	}

	HRESULT DUITextHost::TxDeactivate(LONG lNewState)// hgytest pass
	{
		m_bUiActive = false;
		return S_OK;
	}

	HRESULT DUITextHost::TxGetClientRect(LPRECT lpRect)// hgytest pass
	{
		m_pRichEdit->DV_GetClientRect(lpRect);
		return S_OK;
	}

	HRESULT DUITextHost::TxGetViewInset(LPRECT lpRect)// hgytest pass，设置标识文本所在区域离整个区域四边的边距
	{
		*lpRect = m_pRichEdit->m_HMErcInsetMargin;
		return S_OK;
	}

	HRESULT DUITextHost::TxGetCharFormat(const CHARFORMATW **ppCF)// hgytest pass
	{
		*ppCF = &m_pRichEdit->m_cfDef;
		return S_OK;
	}

	HRESULT DUITextHost::TxGetParaFormat(const PARAFORMAT **ppPF)// hgytest pass
	{
		*ppPF = &m_pRichEdit->m_pfDef;
		return S_OK;
	}

	COLORREF DUITextHost::TxGetSysColor(int nIndex)
	{
		return ::GetSysColor(nIndex);
	}

	HRESULT DUITextHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
	{
		*pstyle = TXTBACK_TRANSPARENT;
		return S_OK;
	}

	HRESULT DUITextHost::TxGetMaxLength(DWORD *plength)
	{
		*plength = m_pRichEdit->m_cchTextMost;
		return S_OK;
	}

	HRESULT DUITextHost::TxGetScrollBars(DWORD *pdwScrollBar)// hgytest pass，每次滚动条出现前，先通过它来获取滚动条具有的属性
	{// 重要测试函数，可通过它改变滚动条属性测试效果
		*pdwScrollBar = m_pRichEdit->m_dwEditStyle&(WS_VSCROLL|WS_HSCROLL|ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_DISABLENOSCROLL);
		return S_OK;
	}

	HRESULT DUITextHost::TxGetPasswordChar(TCHAR *pch)
	{
		*pch = m_pRichEdit->m_chPasswordChar;
		return S_OK;
	}

	HRESULT DUITextHost::TxGetAcceleratorPos(LONG *pcp)
	{
		*pcp = m_pRichEdit->m_lAccelPos;
		return S_OK;
	}

	HRESULT DUITextHost::TxGetExtent(LPSIZEL lpExtent)
	{
		*lpExtent = m_pRichEdit->m_HMEsizelExtent;
		return S_OK;
	}

	HRESULT DUITextHost::OnTxCharFormatChange(const CHARFORMATW * pcf)
	{
		return S_FALSE;
	}

	HRESULT DUITextHost::OnTxParaFormatChange(const PARAFORMAT * ppf)
	{
		return S_FALSE;
	}

	HRESULT DUITextHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits)
	{// 通过dwMask查询对应属性，重要函数
		DWORD dwProperties = 0;

		if (m_pRichEdit->m_bRichText)
		{
			dwProperties = TXTBIT_RICHTEXT;
		}

		if (m_pRichEdit->m_dwEditStyle & ES_MULTILINE)
		{
			dwProperties |= TXTBIT_MULTILINE;
		}

		if (m_pRichEdit->m_dwEditStyle & ES_READONLY)
		{
			dwProperties |= TXTBIT_READONLY;
		}

		if (m_pRichEdit->m_dwEditStyle & ES_PASSWORD)
		{
			dwProperties |= TXTBIT_USEPASSWORD;
		}

		if (!(m_pRichEdit->m_dwEditStyle & ES_NOHIDESEL))
		{
			dwProperties |= TXTBIT_HIDESELECTION;
		}

		if (m_pRichEdit->m_bAutoWordSel)
		{
			dwProperties |= TXTBIT_AUTOWORDSEL;
		}

		if (m_pRichEdit->m_bWordWrap)
		{
			dwProperties |= TXTBIT_WORDWRAP;
		}

		if (m_pRichEdit->m_bAllowBeep)
		{
			dwProperties |= TXTBIT_ALLOWBEEP;
		}

		if (m_pRichEdit->m_bSaveSelection)
		{
			dwProperties |= TXTBIT_SAVESELECTION;
		}

		*pdwBits = dwProperties & dwMask;
		return NOERROR;
	}

	HRESULT DUITextHost::TxNotify(DWORD iNotify, void *pv)
	{
		if (EN_REQUESTRESIZE == iNotify)
		{
			return S_OK;
		}
		return m_pRichEdit->OnTxNotify(iNotify,pv);
	}

	HIMC DUITextHost::TxImmGetContext()
	{
		return ImmGetContext(m_pRichEdit->GetContainer()->OnGetHWnd());
	}

	void DUITextHost::TxImmReleaseContext(HIMC himc)
	{
		ImmReleaseContext(m_pRichEdit->GetContainer()->OnGetHWnd(),himc);
	}

	HRESULT DUITextHost::TxGetSelectionBarWidth( LONG *plSelBarWidth )
	{
		*plSelBarWidth = 0;
		return S_OK;
	}

}//namespace DM