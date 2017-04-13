#include "DmMainAfx.h"
#include "DUIEdit.h"

namespace DM
{
	/// def 抄自wndless
	const LONG g_InitTextMaxLen = (32*1024) - 1;
	#define FValidCF(_pcf) ((_pcf)->cbSize == sizeof(CHARFORMAT2W))
	#define FValidPF(_ppf) ((_ppf)->cbSize == sizeof(PARAFORMAT2))
	#define TIMER_INVALIDATE    6
	#ifndef LY_PER_INCH
	#define LY_PER_INCH 1440
	#endif

	//--------------------------------------DUIRichEdit
	DUIRichEdit::DUIRichEdit()
	{
		Init();
	}

	DUIRichEdit::~DUIRichEdit()
	{
		UnInit();
	}

	void DUIRichEdit::Init()
	{
		m_pTxtHost			 = NULL;
		m_bRichText          = 1;
		m_bSaveSelection     = true;
		m_bWordWrap          = false;
		m_bAllowBeep         = false;
		m_bAutoWordSel		 = true;
		m_bWantTab           = false;
		m_bSingleLineVCenter = true;
		m_bScrollPending     = false;
		m_bEnableDragDrop    = true;
		m_bAutoSel           = false;
		m_cchTextMost        = g_InitTextMaxLen;
		m_chPasswordChar     = _T('*');
		m_lAccelPos          = -1;
		m_dwEditStyle        = ES_LEFT|ES_AUTOHSCROLL|ES_AUTOVSCROLL;
		m_rcInsetMargin.SetRect(2,2,2,2);
		m_HMEsizelExtent.cx = m_HMEsizelExtent.cy = 0;

		//
		m_pDUIXmlInfo->m_bFocusable = true;
		m_bUseRange          = false;

		//
		m_pCaret			 = NULL;
		m_clrCaret           = PBGRA(0,0,0,0xff);
		m_iCaretAniCount     = 6;

		// edit
		DMADDEVENT(DMEventRENotifyArgs::EventID);
		DMADDEVENT(DMEventREWantReturnArgs::EventID);
		DMADDEVENT(DMEventRButtonMenuCmdArgs::EventID);//
	}

	void DUIRichEdit::UnInit()
	{
	}

	//---------------------------------------------------
	// Function Des: 对外接口
	CStringW DUIRichEdit::GetWindowText()
	{
		CStringW strRet;
		int nLen = (int)DM_SendMessage(WM_GETTEXTLENGTH);
		wchar_t *pBuf = strRet.GetBufferSetLength(nLen+1);
		DM_SendMessage(WM_GETTEXT,(WPARAM)nLen+1,(LPARAM)pBuf);
		strRet.ReleaseBuffer();
		return strRet;
	}

	int DUIRichEdit::GetWindowText(LPWSTR lpString,int nMaxCount)
	{
		int iNum = -1;
		do 
		{
			int nLen = (int)DM_SendMessage(WM_GETTEXTLENGTH);
			if (nMaxCount!=-1&&nMaxCount<nLen)
			{
				break;
			}
			DM_SendMessage(WM_GETTEXT,(WPARAM)nLen+1,(LPARAM)lpString);
			iNum = nLen;
		} while (false);
		return iNum;
	}

	int DUIRichEdit::GetWindowTextLength()
	{
		return (int)DM_SendMessage(WM_GETTEXTLENGTH);
	}

	void DUIRichEdit::SetWindowText(LPCWSTR lpszText)
	{
		DM_SendMessage(WM_SETTEXT,0,(LPARAM)lpszText);
	}

	void DUIRichEdit::SetSel(DWORD dwSelection, BOOL bNoScroll)
	{
		DM_SendMessage(EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
		if (!bNoScroll)
		{
			DM_SendMessage(EM_SCROLLCARET, 0, 0L);
		}
	}

	void DUIRichEdit::ReplaceSel(LPWSTR pszText,BOOL bCanUndo)
	{
		DM_SendMessage(EM_REPLACESEL,(WPARAM)bCanUndo,(LPARAM)pszText);
	}

	void DUIRichEdit::SetWordWrap(bool bWordWrap)
	{
		m_bWordWrap = bWordWrap;
		m_pTxtHost->GetTextService()->OnTxPropertyBitsChange(TXTBIT_WORDWRAP, bWordWrap?TXTBIT_WORDWRAP:0);
	}

	bool DUIRichEdit::GetWordWrap(void)
	{
		return m_bWordWrap;
	}

	BOOL DUIRichEdit::GetReadOnly()
	{
		return 0!=(m_dwEditStyle&ES_READONLY);
	}

	BOOL DUIRichEdit::SetReadOnly(bool bReadOnly)
	{
		return 0!=DM_SendMessage(EM_SETREADONLY, bReadOnly);
	}

	LONG DUIRichEdit::GetLimitText()
	{
		return m_cchTextMost;
	}

	BOOL DUIRichEdit::SetLimitText(int nLength)
	{
		return 0!=DM_SendMessage(EM_EXLIMITTEXT, nLength);
	}

	WORD DUIRichEdit::GetDefaultAlign()
	{
		return m_pfDef.wAlignment;
	}

	void DUIRichEdit::SetDefaultAlign(WORD wNewAlign)
	{
		m_pfDef.wAlignment = wNewAlign;
		// Notify control of property change
		m_pTxtHost->GetTextService()->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
	}

	bool DUIRichEdit::GetRichTextFlag()
	{
		return m_bRichText;
	}

	void DUIRichEdit::SetRichTextFlag(bool bRich)
	{
		m_bRichText = bRich;
		m_pTxtHost->GetTextService()->OnTxPropertyBitsChange(TXTBIT_RICHTEXT,bRich?TXTBIT_RICHTEXT:0);
	}


	LONG DUIRichEdit::GetDefaultLeftIndent()
	{
		return m_pfDef.dxOffset;
	}

	void DUIRichEdit::SetDefaultLeftIndent(LONG lNewIndent)
	{
		m_pfDef.dxOffset = lNewIndent;
		m_pTxtHost->GetTextService()->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
	}


	bool DUIRichEdit::SetSaveSelection(bool bSaveSelection)
	{
		bool bResult = bSaveSelection;
		m_bSaveSelection = bSaveSelection;

		// notify text services of property change
		m_pTxtHost->GetTextService()->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION,m_bSaveSelection?TXTBIT_SAVESELECTION:0);

		return bResult;
	}

	DMColor DUIRichEdit::SetDefaultTextColor(DMColor Clr)
	{ 
		COLORREF crOld = m_cfDef.crTextColor;
		m_cfDef.crTextColor = Clr.ToCOLORREF();
		m_pTxtHost->GetTextService()->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
		return crOld;
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
	int DUIRichEdit::OnCreate(LPVOID)
	{
		int iRet = 0;
		do 
		{
			if (0 != (iRet=__super::OnCreate(NULL)))
			{
				break;
			}

			InitDefaultCharFormat(&m_cfDef,NULL);
			InitDefaultParaFormat(&m_pfDef);

			m_pTxtHost = new DUITextHost;
			m_pTxtHost->AddRef();
			if (!m_pTxtHost->Init(this))
			{
				m_pTxtHost->Release();// 引用计数为0，自动删除
				m_pTxtHost = NULL;
				iRet = 1;
				break;
			}
			
			//inplace activate
			m_pTxtHost->GetTextService()->OnTxInPlaceActivate(NULL);
			//默认没有焦点
			m_pTxtHost->m_bUiActive=FALSE;
			m_pTxtHost->GetTextService()->OnTxUIDeactivate();
			m_pTxtHost->GetTextService()->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);

			// set IME
			// 获取IME和远东语言支持选项
			LRESULT lr = DM_SendMessage(EM_GETLANGOPTIONS);
			lr |= IMF_AUTOKEYBOARD | IMF_DUALFONT;
			lr &= ~IMF_AUTOFONT;
			DM_SendMessage(EM_SETLANGOPTIONS, 0, lr);

			// 设置文字
			SetWindowText(m_pDUIXmlInfo->m_strText);

			OnEnableDragDrop(!(m_dwEditStyle&ES_READONLY)&m_bEnableDragDrop);
		} while (false);
		return iRet;
	}

	void DUIRichEdit::OnDestroy()
	{
		OnEnableDragDrop(false);
		__super::OnDestroy();

		if (m_pTxtHost)
		{
			m_pTxtHost->GetTextService()->OnTxInPlaceDeactivate();
			m_pTxtHost->Release();
			m_pTxtHost = NULL;
		}
	}

	void DUIRichEdit::DM_OnPaint(IDMCanvas* pCanvas)
	{

		do 
		{
			if (NULL == pCanvas)
			{
				break;
			}
			
			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(&rcClient);
			CRect rcInvalid;
			pCanvas->GetClipBox(rcInvalid);
			rcInvalid.IntersectRect(rcClient,rcInvalid);
			pCanvas->PushClip(rcInvalid);
			HDC hdc = pCanvas->GetDC();
		
			if (DMSUCCEEDED(GetContainer()->OnIsTranslucent()))
			{
				//pCanvas->AlphaBackup(rcClient);
			}

			LONG lViewId = TXTVIEW_ACTIVE;
			if (!m_pTxtHost->m_bUiActive)
			{
				lViewId = TXTVIEW_INACTIVE;
			}
			RECTL rcL = {rcClient.left,rcClient.top,rcClient.right,rcClient.bottom};
			pServ->TxDraw(
				DVASPECT_CONTENT,          // Draw Aspect
				/*-1*/0,                   // Lindex
				NULL,                      // Info for drawing optimazation
				NULL,                      // target device information
				hdc,					   // Draw device HDC
				NULL,                      // Target device HDC
				&rcL,						 // Bounding client rectangle
				NULL,						 // Clipping rectangle for metafiles
				&rcInvalid,				 // Update rectangle
				NULL,                      // Call back function
				NULL,                      // Call back parameter
				TXTVIEW_ACTIVE);

			if(DMSUCCEEDED(GetContainer()->OnIsTranslucent()))
			{
				//pCanvas->AlphaRestore();
				rcClient.DeflateRect(m_rcInsetMargin);// 不在文字范围内的不做填充
				pCanvas->AdjustAlpha(0xff,rcClient);
			}

			pCanvas->ReleaseDC(hdc);
			pCanvas->PopClip();
		} while (false);
	}

	void DUIRichEdit::DM_OnSetFocus()
	{
		do 
		{
			if (false == DV_IsFocusable())
			{
				break;
			}

			__super::DM_OnSetFocus();
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			m_pTxtHost->m_bUiActive = true;
			pServ->OnTxUIActivate();
			pServ->TxSendMessage(WM_SETFOCUS,0,0,NULL);

			if (m_bAutoSel) 
			{
				SetSel(MAKELONG(0,-1),TRUE);
			}

		} while (false);
	}

	void DUIRichEdit::DM_OnKillFocus()
	{
		do 
		{
			if (false == DV_IsFocusable())
			{
				break;
			}

			__super::DM_OnKillFocus();

			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			m_pTxtHost->m_bUiActive = false;
			pServ->OnTxUIDeactivate();
			pServ->TxSendMessage(WM_KILLFOCUS,0,0,NULL);
			m_pTxtHost->TxShowCaret(FALSE);

		} while (false);
	}

	void DUIRichEdit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			if (!DM_IsFocusWnd())
			{
				DV_SetFocusWnd();
				if (!m_bAutoSel)
				{// 改变光标位置
					pServ->TxSendMessage(m_DUIData.m_pCurMsg->uMsg,m_DUIData.m_pCurMsg->wp,m_DUIData.m_pCurMsg->lp,NULL);
				}
			}
			else
			{
				pServ->TxSendMessage(m_DUIData.m_pCurMsg->uMsg,m_DUIData.m_pCurMsg->wp,m_DUIData.m_pCurMsg->lp,NULL);
			}

		} while (false);
	}

	void DUIRichEdit::OnRButtonDown(UINT nFlags, CPoint point)
	{
		DMEventRButtonMenuCmdArgs Evt(this);
		Evt.m_pt = point;
		DV_FireEvent(Evt); 
	}

	void DUIRichEdit::OnMouseMove(UINT nFlags, CPoint point)
	{
		OnTxSendCurMessage();
	}

	void DUIRichEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// 在设置了wantReturn状态时，而且为单行时，发此消息，用于配合网页显示控件
		if (VK_RETURN==nChar  // 按下Enter
			&&((m_dwEditStyle&ES_WANTRETURN)&&!PUSH_CTRL)// wantReturn=1，并且没有同时按下CTRL
			&&!(m_dwEditStyle&ES_MULTILINE)// 不是多行
			)
		{
			DMEventREWantReturnArgs Evt(this);
			DV_FireEvent(Evt);
		}
		if (VK_RETURN==nChar&&!(m_dwEditStyle&ES_WANTRETURN)&&!PUSH_CTRL)
		{
			SetMsgHandled(FALSE);
		}
		else
		{
			OnTxSendCurMessage();
		}
	}

#define CTRL(_ch) (_ch - 'A' + 1)
	void DUIRichEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		bool bSendMsg = true;
		switch (nChar)
		{
			// Ctrl-Return generates Ctrl-J (LF), treat it as an ordinary return
		case CTRL('J'):
		case VK_RETURN:
			{
				if (!PUSH_CTRL
					&&!(m_dwEditStyle & ES_WANTRETURN))
				{
					bSendMsg = false;
				}
			}
			break;

		case VK_TAB:
			{
				if (!m_bWantTab
					&&!PUSH_CTRL)
				{
					bSendMsg = false;
				}
			}
			break;

		default:
			{
				if (m_dwEditStyle&ES_NUMBER 
					&&!isdigit(nChar) // 非数字
					&&nChar!='-' && nChar!='.' && nChar!=',')
				{
					bSendMsg = false;
				}
			}
			break;
		}
		if (bSendMsg)
		{
			OnTxSendCurMessage();
		}
	}

	void DUIRichEdit::OnSetFont(IDMFont *pFont, BOOL bRedraw)
	{
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			if (!SUCCEEDED(InitDefaultCharFormat(&m_cfDef, pFont)))
			{
				break;
			}
			//TXTBIT_CHARFORMATCHANGE: the character format changed
			pServ->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, TXTBIT_CHARFORMATCHANGE);
		}while(false);
	}

	LRESULT DUIRichEdit::OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{//TODO
		__super::OnNcCalcSize(bCalcValidRects,lParam);
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			// 计算
			CRect rcInsetMargin = m_rcInsetMargin;
			if(!m_bRichText && m_bSingleLineVCenter && !(m_dwEditStyle&ES_MULTILINE))
			{
				rcInsetMargin.top    =
				rcInsetMargin.bottom = (m_rcsbClient.Height()-m_nFontHeight)/2;
			}

			m_siHoz.nPage = m_rcsbClient.Width()-rcInsetMargin.left-rcInsetMargin.right;
			m_siVer.nPage = m_rcsbClient.Height()-rcInsetMargin.top-rcInsetMargin.bottom;

			SIZEL pxsize = {m_rcsbClient.Width(),m_rcsbClient.Height()};
			SIZEL hmsize = {0};
			DMPixelToHiMetric(&pxsize, &hmsize,GetContainer()->OnGetHWnd());
			m_HMEsizelExtent.cx = hmsize.cx;
			m_HMEsizelExtent.cy = hmsize.cy;

			pxsize.cx = rcInsetMargin.left;
			pxsize.cy = rcInsetMargin.right;
			DMPixelToHiMetric(&pxsize, &hmsize,GetContainer()->OnGetHWnd());
			m_HMErcInsetMargin.left  = hmsize.cx;
			m_HMErcInsetMargin.right = hmsize.cy;

			pxsize.cx = rcInsetMargin.top;
			pxsize.cy = rcInsetMargin.bottom;
			DMPixelToHiMetric(&pxsize, &hmsize,GetContainer()->OnGetHWnd());
			m_HMErcInsetMargin.top	  = hmsize.cx;
			m_HMErcInsetMargin.bottom = hmsize.cy;


			bool bFocus = DM_IsFocusWnd();
			if (bFocus)
			{
				m_pTxtHost->m_bUiActive = false;
				pServ->OnTxUIDeactivate();
				pServ->TxSendMessage(WM_KILLFOCUS,0,0,NULL);
				m_pTxtHost->TxShowCaret(FALSE);
			}
			// TXTBIT_CHARFORMATCHANGE:the character format changed.
			// TXTBIT_CLIENTRECTCHANGE:the client rectangle changed
			pServ->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE|TXTBIT_CLIENTRECTCHANGE, TXTBIT_EXTENTCHANGE|TXTBIT_CLIENTRECTCHANGE);
			if (bFocus)
			{
				m_pTxtHost->m_bUiActive = true;
				pServ->OnTxUIActivate();
				pServ->TxSendMessage(WM_SETFOCUS,0,0,NULL);
				m_pTxtHost->TxSetFocus();
			}
#if 0
			m_siHoz.nPage = m_rcsbClient.Width()-m_rcInsetMargin.left-m_rcInsetMargin.right;
			m_siVer.nPage = m_rcsbClient.Height()-m_rcInsetMargin.top-m_rcInsetMargin.bottom;

			HDC hdc = ::GetDC(GetContainer()->OnGetHWnd());
			LONG xPerInch = ::GetDeviceCaps(hdc, LOGPIXELSX); // 沿屏幕宽度每逻辑英寸的像素数
			LONG yPerInch = ::GetDeviceCaps(hdc, LOGPIXELSY); // 沿屏幕高度每逻辑英寸的像素数
			::ReleaseDC(GetContainer()->OnGetHWnd(), hdc);

			m_HMEsizelExtent.cx = DUIEditHelper::DtoHimetric(m_rcsbClient.Width(), xPerInch);
			m_HMEsizelExtent.cy = DUIEditHelper::DtoHimetric(m_rcsbClient.Height(), yPerInch);

			m_HMErcInsetMargin.left  = DUIEditHelper::DtoHimetric(m_rcInsetMargin.left,xPerInch);
			m_HMErcInsetMargin.right = DUIEditHelper::DtoHimetric(m_rcInsetMargin.right,xPerInch);

			if (!m_bRichText && m_bSingleLineVCenter && !(m_dwEditStyle&ES_MULTILINE))
			{// 单行居中状态，此时只留一个文字高
				m_HMErcInsetMargin.top	  = DUIEditHelper::DtoHimetric(m_rcsbClient.Height()-m_nFontHeight,yPerInch)/2;
				m_HMErcInsetMargin.bottom = m_HMErcInsetMargin.top;
			}
			else
			{
				m_HMErcInsetMargin.top	  = DUIEditHelper::DtoHimetric(m_rcInsetMargin.top,yPerInch);
				m_HMErcInsetMargin.bottom = DUIEditHelper::DtoHimetric(m_rcInsetMargin.bottom,yPerInch);
			}
			
			/*窗口有焦点时，需要更新光标位置：先使edit失活用来关闭光标，再激活edit来显示光标。
			此处不应该直接用setfocus和killfocus，因为这两个消息可能会被外面响应。导致逻辑错误*/

			bool bFocus = DM_IsFocusWnd();
			if (bFocus)
			{
				m_pTxtHost->m_bUiActive = false;
				pServ->OnTxUIDeactivate();
				pServ->TxSendMessage(WM_KILLFOCUS,0,0,NULL);
				m_pTxtHost->TxShowCaret(FALSE);
			}
			// TXTBIT_CHARFORMATCHANGE:the character format changed.
			// TXTBIT_CLIENTRECTCHANGE:the client rectangle changed
			pServ->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE|TXTBIT_CLIENTRECTCHANGE, TXTBIT_EXTENTCHANGE|TXTBIT_CLIENTRECTCHANGE);
			if (bFocus)
			{
				CRect rcClient;
				DV_GetClientRect(&rcClient);
				if (DM_GetWindow(GDW_PARENT))// 设置光标显示区
				{
					DM_GetWindow(GDW_PARENT)->DV_OnShowCaretRect(&rcClient);
				}

				m_pTxtHost->m_bUiActive = true;
				pServ->OnTxUIActivate();
				pServ->TxSendMessage(WM_SETFOCUS,0,0,NULL);
			}
#endif 
		} while (false);
		return 0;
	}

	LRESULT DUIRichEdit::OnSetText(UINT uMsg,WPARAM wparam,LPARAM lparam)
	{
		// For RichEdit 1.0, the max text length would be reset by a settext so we follow pattern here as well.
		bool bRet = false;
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}
			if (!SUCCEEDED(pServ->TxSendMessage(uMsg, wparam, lparam, 0)))
			{
				break;
			}

			DWORD cchTextMost = 0;
			if (lparam)
			{
				cchTextMost = (DWORD)_tcslen((LPCTSTR)lparam);
			}

			if (cchTextMost>m_cchTextMost)
			{// If the new text is greater than the max set the max to the new text length.
				m_cchTextMost = cchTextMost;
			}
			bRet = true;
		} while (false);
		return (HRESULT)bRet;
	}


	LRESULT DUIRichEdit::OnSetCharFormat(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{// TODO
		bool bRet = false;
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			if (!FValidCF((CHARFORMAT2W*)lParam))
			{
				break;
			}

			if (wParam&SCF_SELECTION)
			{
				pServ->TxSendMessage(uMsg,wParam,lParam,NULL);
			}
			else
			{
				m_cfDef = *(CHARFORMAT2W *)lParam;
				pServ->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE,TXTBIT_CHARFORMATCHANGE);
			}
			bRet = true;
		} while (false);
		return (HRESULT)bRet;
	}

	LRESULT DUIRichEdit::OnSetParaFormat( UINT uMsg, WPARAM wparam, LPARAM lparam )
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			if (!FValidPF((PARAFORMAT2*)lparam))
			{
				break;
			}

			// check to see if we're setting the default.
			// either SCF_DEFAULT will be specified *or* there is no
			// no text in the document (richedit1.0 behaviour).
			if (!(wparam&SCF_DEFAULT))
			{
				if (S_OK == pServ->TxSendMessage(WM_GETTEXTLENGTH, 0, 0, 0))
				{
					wparam |= SCF_DEFAULT;
				}
			}

			if (wparam & SCF_DEFAULT)
			{
				m_pfDef = *(PARAFORMAT2*)lparam;
				pServ->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE,TXTBIT_PARAFORMATCHANGE);
			}
			else
			{
				pServ->TxSendMessage(uMsg,wparam,lparam,NULL);    // Change selection format
			}
			bRet = true;
		} while (false);
		return (HRESULT)bRet;
	}

	LRESULT DUIRichEdit::OnSetReadOnly(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return (HRESULT)(SetAttribute(DMAttr::DUIRichEditAttr::bool_breadonly,wParam?L"1":L"0"));
	}

	LRESULT DUIRichEdit::OnSetLimitText(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			if (0 == wParam)
			{
				m_cchTextMost = g_InitTextMaxLen;
			}
			else
			{
				m_cchTextMost = (DWORD)wParam;
			}
			pServ->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
			bRet = true;
		} while (false);
		return (HRESULT)bRet;
	}


	LRESULT DUIRichEdit::OnButtonClick(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}
			pServ->TxSendMessage(uMsg,wParam,lParam,NULL);
			bRet = true;
		} while (false);
		return (HRESULT)bRet;
	}


	LRESULT DUIRichEdit::OnImeChar(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// For a Unicode window, this message is the same as WM_CHAR.
		if (m_dwEditStyle&ES_NUMBER) // 数字
		{
		}
		else
		{
			SetMsgHandled(FALSE);
		}
		return 0;
	}

	void DUIRichEdit::DM_OnTimer(UINT_PTR idEvent)
	{
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			pServ->TxSendMessage(WM_TIMER,idEvent,0,NULL);
		} while (false);
	}

	DMCode DUIRichEdit::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = __super::DV_CreateChildWnds(XmlNode);
		if (NULL == m_pCaret)
		{
			m_pCaret = new DUICaret();
			m_pCaret->m_clr = m_clrCaret;
			m_pCaret->SetAniCount(m_iCaretAniCount);
		}
		DM_InsertChild(m_pCaret);
		CStringW strValue = L"0,0,@0,@0";
		m_pCaret->SetAttribute(L"pos",strValue,true);
		m_pCaret->DM_SetVisible(false,false);
		return iErr;
	}

	UINT DUIRichEdit::DV_OnGetDlgCode()                                                   
	{
		UINT uRet = DMDLGC_WANTCHARS|DMDLGC_WANTARROWS;
		if (m_bWantTab) 
		{
			uRet |= DMDLGC_WANTTAB;
		}
		if (m_dwEditStyle&ES_WANTRETURN)
		{
			uRet |= DMDLGC_WANTRETURN;
		}
		return uRet;
	}

	DMCode DUIRichEdit::DV_OnSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(&rcClient);
			if (!rcClient.PtInRect(pt))
			{
				break;
			}

			DMAutoDC hdc(GetContainer()->OnGetHWnd());
			pServ->OnTxSetCursor(
				DVASPECT_CONTENT,
				-1,
				NULL,
				NULL,
				hdc,
				NULL,
				&rcClient,
				pt.x,
				pt.y);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	bool DUIRichEdit::OnScroll(bool bVert,UINT uCode,int nPos)
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}

			if (m_bScrollPending)
			{
				break;
			}
			m_bScrollPending = true;
			DUIScrollBase::OnScroll(bVert,uCode,nPos);
			LRESULT lresult = -1;
			pServ->TxSendMessage(bVert?WM_VSCROLL:WM_HSCROLL,MAKEWPARAM(uCode,nPos),0,&lresult);
			LONG lPos = 0;
			if (bVert)
			{
				pServ->TxGetVScroll(NULL,NULL,&lPos,NULL,NULL);
			}
			else
			{
				pServ->TxGetHScroll(NULL,NULL,&lPos,NULL,NULL);
			}


			if (lPos != GetScrollPos(bVert))
			{
				SetScrollPos(bVert,lPos,FALSE);
			}

			m_bScrollPending = false;
			if (SB_THUMBTRACK == uCode)
			{
				ScrollUpdateWindow();
			}
			bRet = (0 == lresult);

		} while (false);
		return bRet;
	}

	// 辅助
	HRESULT DUIRichEdit::InitDefaultCharFormat(CHARFORMAT2W* pcf,IDMFont *pFt)
	{
		HRESULT hr = S_FALSE;
		do 
		{
			DMSmartPtrT<IDMFont> pFont = pFt;
			if (NULL == pFont)
			{
				m_pDUIXmlInfo->m_pStyle->GetTextFont(0,&pFont);
				if (NULL == pFont)
				{
					pFont = g_pDMApp->GetFont(L"");
				}
			}
			m_nFontHeight = DMABS(pFont->GetLogFont()->lfHeight);

			memset(pcf, 0, sizeof(CHARFORMAT2W));
			pcf->cbSize = sizeof(CHARFORMAT2W);
			pcf->dwMask = CFM_SIZE|CFM_OFFSET|CFM_FACE|CFM_CHARSET|CFM_COLOR|CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE;

			DMColor crText;
			m_pDUIXmlInfo->m_pStyle->GetTextColor(0,crText);
			pcf->crTextColor = crText.ToCOLORREF();

			DMAutoDC hdc;
			LONG yPixPerInch = GetDeviceCaps(hdc, LOGPIXELSY);
			hdc.ReleaseDC();

			const LOGFONTW *plf = pFont->GetLogFont();
			if (yPixPerInch)
			{
				pcf->yHeight	 = -DMABS(plf->lfHeight*LY_PER_INCH/yPixPerInch);
			}
			pcf->yOffset	 = 0;
			pcf->dwEffects   = 0;
			
			if (pFont->GetLogFont()->lfWeight>=FW_BOLD)
			{
				pcf->dwEffects |= CFE_BOLD;
			}
			if (pFont->IsItalic())
			{
				pcf->dwEffects |= CFE_ITALIC;
			}
			if (pFont->IsUnderline())
			{
				pcf->dwEffects |= CFE_UNDERLINE;
			}

			pcf->bCharSet = plf->lfCharSet;
			pcf->bPitchAndFamily = plf->lfPitchAndFamily;
			wcscpy_s(pcf->szFaceName, plf->lfFaceName);

			hr = S_OK;
		} while (false);

		return hr;
	}

	HRESULT DUIRichEdit::InitDefaultParaFormat(PARAFORMAT2* ppf)
	{
		memset(ppf,0,sizeof(PARAFORMAT2));
		ppf->cbSize		= sizeof(PARAFORMAT2);
		ppf->dwMask		= PFM_ALL;
		ppf->cTabCount  = 1;
		ppf->rgxTabs[0] = lDefaultTab;

		if (m_dwEditStyle&ES_CENTER)
		{
			ppf->wAlignment = PFA_CENTER;
		}
		else if (m_dwEditStyle&ES_RIGHT)
		{
			ppf->wAlignment = PFA_RIGHT;
		}
		else
		{
			ppf->wAlignment = PFA_LEFT;
		}

		return S_OK;
	}

	void DUIRichEdit::OnEnableDragDrop(bool bEnable)
	{
		if (bEnable)
		{
			DUIRicheditDropTarget *pDropTarget = new DUIRicheditDropTarget(m_pTxtHost->GetTextService());
			GetContainer()->OnRegisterDragDrop(m_hDUIWnd,pDropTarget);
			pDropTarget->Release();
		}
		else
		{
			GetContainer()->OnUnRegisterDragDrap(m_hDUIWnd);
		}
	}

	HRESULT DUIRichEdit::OnTxSendCurMessage()
	{
		HRESULT hr = S_FALSE;
		do 
		{
			if (NULL == m_pTxtHost)
			{
				break;
			}

			ITextServices* pServ = m_pTxtHost->GetTextService();
			if (NULL == pServ)
			{
				break;
			}
			hr = pServ->TxSendMessage(m_DUIData.m_pCurMsg->uMsg,m_DUIData.m_pCurMsg->wp,m_DUIData.m_pCurMsg->lp,NULL);
		} while (false);
		return hr;
	}

	void DUIRichEdit::ShowCaret(bool bShow)
	{
		do 
		{
			if (NULL == m_pCaret)
			{
				break;
			}
			// 此消息可能由setfocus消息触发，此时还没有设置完成焦点窗口，所以使用标志判断
			if (bShow&&m_pTxtHost->m_bUiActive)
			{
				m_pCaret->Show();
			}
			else
			{
				m_pCaret->Hide();
			}
		} while (false);
	}

	void DUIRichEdit::InitCaret(int w, int h)
	{
		if (m_pCaret)
		{
			m_pCaret->SetSize(CSize(w,h));
		}
	}

	void DUIRichEdit::ResetCaret(CPoint point)
	{
		CRect rcClient;
		DV_GetClientRect(rcClient);
		CPoint pt = rcClient.TopLeft();
		point -= pt;
		if (m_pCaret)
		{
			m_pCaret->SetPos(point);
		}
	}


	BOOL DUIRichEdit::DV_WndProc( UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT & lResult )
	{
		do 
		{
			if (NULL==m_pCaret||NULL == m_pTxtHost->GetTextService())
			{
				break;
			}
			if (WM_KEYDOWN == uMsg)
			{
				if (VK_RETURN==(TCHAR)wParam&&!(m_dwEditStyle&ES_WANTRETURN)&&!PUSH_CTRL)
				{
					break;
				}
			}
			if (EM_GETRECT == uMsg)
			{
				SetMsgHandled(TRUE);
				DV_GetClientRect((LPRECT)lParam);
			}

			if (S_OK == m_pTxtHost->GetTextService()->TxSendMessage(uMsg,wParam,lParam,&lResult))
			{
				SetMsgHandled(TRUE);
			}
		} while (false);
		if (!IsMsgHandled())
		{
			return __super::DV_WndProc(uMsg,wParam,lParam,lResult);;
		}	
		return TRUE;
	}


	HRESULT DUIRichEdit::OnTxNotify( DWORD iNotify,LPVOID pv )
	{
		// 示例取自sharpui--------------
		switch (iNotify)
		{
		case EN_MAXTEXT:// 达到最大长度
			break;
		case EN_SELCHANGE:// 选择变化，需要事件掩码ENM_SELCHANGE
			{
				SELCHANGE* s = (SELCHANGE*)pv;            
			}
			break;
		case EN_ERRSPACE:// 内存溢出
			break;
		case EN_CHANGE:	// 输入变化，需要事件ENM_CHANGE掩码
			{
				CHANGENOTIFY* c = (CHANGENOTIFY*)pv;
			}
			break;
		case EN_DROPFILES:// 文件拖动，需要事件ENM_DROPFILES掩码
			{
				ENDROPFILES* e = (ENDROPFILES*)pv;
			}
			break;
		case EN_LINK:// 超链接，dwEffects需要CFE_LINK
			{
				ENLINK* enlink = (ENLINK*)pv;
				int i = 0;
			}
			break;
		case EN_REQUESTRESIZE: // 需要事件掩码ENM_REQUESTRESIZE
			{
				break;
			}
		case EN_OLEOPFAILED:// ole对象操作失败
			break;
		case EN_UPDATE:// 准备绘制
			break;
		case EN_KILLFOCUS:// 失去焦点
			break;
		case EN_PROTECTED:// 数据保护, CFE_PROTECTED
			{
				ENPROTECTED* enp = (ENPROTECTED*)pv;
			}
			break;
		case EN_MSGFILTER:// 消息过滤
			{
				MSGFILTER* mf = (MSGFILTER*)pv;
			}
			break;
		case EN_SAVECLIPBOARD:// 存储剪贴板
			{
				ENSAVECLIPBOARD* ens = (ENSAVECLIPBOARD*)pv;
			}
			break;
		case EN_DRAGDROPDONE: // 拖动完成，需要事件掩码ENM_DRAGDROPDONE
			break;
		}

		DMEventRENotifyArgs evt(this);
		evt.m_iNotify = iNotify;
		evt.m_pv	  = pv;
		evt.m_hr      = S_OK;
		DV_FireEvent(evt);
		return evt.m_hr;
	}

	DMCode DUIRichEdit::OnAttrCustomEx(LPCWSTR lpszAttribute, LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_OK;
		DWORD dwBit  = 0;
		DWORD dwMask = 0;
		bool bObj    = false;
		do 
		{
			if (NULL == lpszAttribute||NULL == lpszValue)
			{
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute, DMAttr::DUIRichEditAttr::INT_editstyle))
			{// 强制属性
				int iStyle = 0;
				DMAttributeDispatch::ParseInt(lpszValue,iStyle);
				m_dwEditStyle = iStyle;
				if (!bLoadXml)
				{
					InitTxPropertyBits(dwBit,dwMask);
				}	
				break;
			}

			if (0 ==  dm_wcsicmp(lpszAttribute, DMAttr::DUIRichEditAttr::INT_maxbuf))
			{
				int iMaxLen = g_InitTextMaxLen;
				DMAttributeDispatch::ParseInt(lpszValue,iMaxLen);
				m_cchTextMost = iMaxLen;
				dwBit |= TXTBIT_MAXLENGTHCHANGE;
				dwMask|= TXTBIT_MAXLENGTHCHANGE;
				break;
			}
			
			if (0 ==  dm_wcsicmp(lpszAttribute, DMAttr::DUIRichEditAttr::bool_brichtext))
			{
				bObj  = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				m_bRichText = bObj;
	
				if (bObj)
				{
					dwBit  |= TXTBIT_RICHTEXT;
				}
				else
				{
					dwBit  &= ~TXTBIT_RICHTEXT;
				}
				dwMask |= TXTBIT_RICHTEXT;
				break;
			}

			if (0 ==  dm_wcsicmp(lpszAttribute, DMAttr::DUIRichEditAttr::bool_bwordwrap))
			{// wordwrap
				bObj  = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				m_bWordWrap = bObj;
				if (bObj)
				{
					dwBit  |= TXTBIT_WORDWRAP;
				}
				else
				{
					dwBit  &= ~TXTBIT_WORDWRAP;
				}
				dwMask |= TXTBIT_WORDWRAP;
				break;
			}

			if (0 ==  dm_wcsicmp(lpszAttribute, DMAttr::DUIRichEditAttr::bool_ballowbeep))
			{// allowbeep
				bObj  = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				m_bAllowBeep = bObj;
				if (bObj)
				{
					dwBit  |= TXTBIT_ALLOWBEEP;
				}
				else
				{
					dwBit  &= ~TXTBIT_ALLOWBEEP;
				}
				dwMask |= TXTBIT_ALLOWBEEP;
				break;
			}

			if (0 ==  dm_wcsicmp(lpszAttribute, DMAttr::DUIRichEditAttr::bool_bautowordsel))
			{// autowordsel
				bObj  = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				m_bAutoWordSel = bObj;
				if (bObj)
				{
					dwBit  |= TXTBIT_AUTOWORDSEL;
				}
				else
				{
					dwBit  &= ~TXTBIT_AUTOWORDSEL;
				}
				dwMask |= TXTBIT_AUTOWORDSEL;
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_bmultilines))
			{// multilines
				bObj  = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=ES_MULTILINE;
					dwBit |= TXTBIT_MULTILINE;
				}
				else
				{
					m_dwEditStyle&=~ES_MULTILINE;
				}
				dwMask |= TXTBIT_MULTILINE;
				if (!bLoadXml)
				{
					DM_SendMessage(WM_NCCALCSIZE);// 重新计算宿主客户区大小
					DM_InvalidateRect(m_rcWindow);
				}
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_bhscroll))
			{// hscrollbar
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=WS_HSCROLL;
				}
				else
				{
					m_dwEditStyle&=~WS_HSCROLL;
				}
				dwBit  |= TXTBIT_SCROLLBARCHANGE;
				dwMask |= TXTBIT_SCROLLBARCHANGE;
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_bvscroll))
			{// vscrollBar
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=WS_VSCROLL;
				}
				else
				{
					m_dwEditStyle&=~WS_VSCROLL;
				}
				dwBit  |= TXTBIT_SCROLLBARCHANGE;
				dwMask |= TXTBIT_SCROLLBARCHANGE;
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_bautohscroll))
			{// auto hscroll
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=ES_AUTOHSCROLL;
				}
				else
				{
					m_dwEditStyle&=~ES_AUTOHSCROLL;
				}
				dwBit  |= TXTBIT_SCROLLBARCHANGE;
				dwMask |= TXTBIT_SCROLLBARCHANGE;
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_bautovscroll))
			{// auto vscroll
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=ES_AUTOVSCROLL;
				}
				else
				{
					m_dwEditStyle&=~ES_AUTOVSCROLL;
				}
				dwBit  |= TXTBIT_SCROLLBARCHANGE;
				dwMask |= TXTBIT_SCROLLBARCHANGE;
				break;
			}

			// readonly
			if(0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_breadonly))
			{
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=ES_READONLY;
					dwBit |= TXTBIT_READONLY;
				}
				else
				{
					m_dwEditStyle&=~ES_READONLY;
				}
				dwMask |= TXTBIT_READONLY;
				break;
			}

			// want return
			if(0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_bwantreturn))
			{
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=ES_WANTRETURN;
				}
				else
				{
					m_dwEditStyle&=~ES_WANTRETURN;
				}
				break;
			}


			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_bpassword))
			{// password
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=ES_PASSWORD;
					dwBit  |= TXTBIT_USEPASSWORD;
				}
				else
				{
					m_dwEditStyle&=~ES_PASSWORD;
				}
				dwMask |= TXTBIT_USEPASSWORD;
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::CHAR_passwordchar))
			{
				m_chPasswordChar = lpszValue[0];
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_bnumber))
			{// number
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				if (bObj)
				{
					m_dwEditStyle|=ES_NUMBER;
				}
				else
				{
					m_dwEditStyle&=~ES_NUMBER;
				}
				break;
			}

			if (0 == dm_wcsicmp(lpszAttribute,DMAttr::DUIRichEditAttr::bool_benabledrag))
			{// enabledragdrop
				bObj = false;
				DMAttributeDispatch::ParseBool(lpszValue,bObj);
				m_bEnableDragDrop = bObj;
				if(!bLoadXml)
				{
					OnEnableDragDrop(!(m_dwEditStyle&ES_READONLY) & m_bEnableDragDrop);
				}
				break;
			}
			iErr = DM_ECODE_FAIL;// 什么都没处理到！
		} while (false);

		if (!bLoadXml&&DMSUCCEEDED(iErr))
		{
			m_pTxtHost->GetTextService()->OnTxPropertyBitsChange(dwMask,dwBit);
			DM_InvalidateRect(m_rcWindow);
		}
		return iErr;
	}

	DMCode DUIRichEdit::OnAttrCuretClr(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			DMAttributeDispatch::ParseColor(pszValue,m_clrCaret);
			if (NULL == m_pCaret)// 初化时光标是在create后
			{
				break;	
			}
			m_pCaret->m_clr = m_clrCaret;
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUIRichEdit::OnAttrCuretAnimateCount(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			DMAttributeDispatch::ParseInt(pszValue,m_iCaretAniCount);
			if (NULL == m_pCaret)// 初化时光标是在create后
			{
				break;	
			}
			m_pCaret->SetAniCount(m_iCaretAniCount);
		} while (false);
		return DM_ECODE_OK;
	}

	void DUIRichEdit::InitTxPropertyBits(DWORD &dwBit,DWORD &dwMask)
	{
		do 
		{
			if (m_dwEditStyle & ES_MULTILINE)
			{// multilines
				dwBit  |= TXTBIT_MULTILINE;
				dwMask |= TXTBIT_MULTILINE;
			}

			if (m_dwEditStyle & WS_HSCROLL||m_dwEditStyle & WS_VSCROLL
				||m_dwEditStyle & ES_AUTOHSCROLL||m_dwEditStyle & ES_AUTOVSCROLL)
			{
				dwBit  |= TXTBIT_SCROLLBARCHANGE;
				dwMask |= TXTBIT_SCROLLBARCHANGE;
			}

			if (m_dwEditStyle & ES_READONLY)
			{// multilines
				dwBit  |= TXTBIT_READONLY;
				dwMask |= TXTBIT_READONLY;
			}

			if (m_dwEditStyle & ES_PASSWORD)
			{// multilines
				dwBit  |= TXTBIT_USEPASSWORD;
				dwMask |= TXTBIT_USEPASSWORD;
			}

		} while (false);
	}

	DMCode DUIRichEdit::OnAttrTextColor(LPCWSTR pszValue, bool bLoadXml)
	{
		DMColor Clr;
		Clr.SetTextInvalid();
		DMAttributeDispatch::ParseColor(pszValue,Clr);
		if (!Clr.IsTextInvalid())
		{
			m_pDUIXmlInfo->m_pStyle->SetAttribute(L"clrtext",pszValue,bLoadXml);
		}

		if (!bLoadXml)
		{
			SetDefaultTextColor(Clr);
		}
		return DM_ECODE_OK;
	}

	DMCode DUIRichEdit::OnAttrTextFont(LPCWSTR pszValue, bool bLoadXml)
	{
		m_pDUIXmlInfo->m_pStyle->SetAttribute(L"font",pszValue,bLoadXml);
		if (!bLoadXml)
		{
			DMSmartPtrT<IDMFont> pFont;
			m_pDUIXmlInfo->m_pStyle->GetTextFont(0,&pFont);
			OnSetFont(pFont,TRUE);
		}
		return DM_ECODE_OK;
	}

	DWORD CALLBACK EditStreamCallback_FILE(
		DWORD_PTR dwCookie,
		LPBYTE pbBuff,
		LONG cb,
		LONG * pcb 
		)
	{
		FILE *f=(FILE*)dwCookie;
		LONG nReaded = (LONG)fread(pbBuff,1,cb,f);
		if(pcb) *pcb = nReaded;
		return 0;
	}

	struct MemBlock
	{
		LPCBYTE  pBuf;
		LONG     nRemains;
	};

	DWORD CALLBACK EditStreamCallback_MemBlock(
		DWORD_PTR dwCookie,
		LPBYTE pbBuff,
		LONG cb,
		LONG * pcb 
		)
	{
		MemBlock *pmb=(MemBlock*)dwCookie;
		if (pmb->nRemains>=cb)
		{
			memcpy(pbBuff,pmb->pBuf,cb);
			pmb->pBuf+=cb;
			pmb->nRemains-=cb;
			if (pcb) *pcb = cb;
			return 0;
		}
		else
		{
			memcpy(pbBuff,pmb->pBuf,pmb->nRemains);
			pmb->pBuf+=pmb->nRemains;
			if(pcb) *pcb = pmb->nRemains;
			pmb->nRemains =0;
			return 0;
		}
	}


	DMCode DUIRichEdit::OnAttrAlign(LPCWSTR pszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pszValue||0 == wcslen(pszValue))
			{
				break;
			}
			m_dwEditStyle &= ~ES_CENTER;
			m_dwEditStyle &= ~ES_RIGHT;
			if (0 == _wcsicmp(pszValue,L"center"))
			{
				m_dwEditStyle|=ES_CENTER;
			}
			else if (0 == _wcsicmp(pszValue,L"right"))
			{
				m_dwEditStyle|=ES_RIGHT;
			}

			if (!bLoadXml)// 直接更新
			{
				if (m_dwEditStyle&ES_CENTER)
				{
					m_pfDef.wAlignment = PFA_CENTER;
				}
				else if (m_dwEditStyle&ES_RIGHT)
				{
					m_pfDef.wAlignment = PFA_RIGHT;
				}
				else
				{
					m_pfDef.wAlignment = PFA_LEFT;
				}
				m_pTxtHost->GetTextService()->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIRichEdit::OnAttrText(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			if (bLoadXml)
			{
				m_pDUIXmlInfo->m_strText = pszValue;
				break;
			}
			SetWindowText(pszValue);
		} while (false);
		return DM_ECODE_OK;
	}

	//------------------------------------DUIEdit
	DUIEdit::DUIEdit()
	{
		m_SurfaceTextClr       = PBGRA(0xcc,0xcc,0xcc,0xff);
		m_bRichText	   = 0;
		m_bAutoSel     = false;
		m_uSurfaceAlign= EditAlign_Left;
	}

	int DUIEdit::GetSurfaceText(LPTSTR lpString,int nMaxCount)
	{
		int iNum = -1;
		do 
		{
			int nLen = m_strSurfaceText.GetLength();
			if (nMaxCount!=-1&&nMaxCount<nLen)
			{
				break;
			}
			memcpy(lpString,m_strSurfaceText.GetBuffer(-1),nLen);
			m_strSurfaceText.ReleaseBuffer();
			iNum = nLen;
		} while (false);
		return iNum;
	}

	int DUIEdit::GetSurfaceTextLength()
	{
		return m_strSurfaceText.GetLength();
	}

	void DUIEdit::DM_OnKillFocus()
	{
		DUIRichEdit::DM_OnKillFocus();
		if (!m_strSurfaceText.IsEmpty() && 0==GetWindowTextLength()) 
		{
			DM_Invalidate();
		}
	}

	void DUIEdit::DM_OnSetFocus()
	{
		DUIRichEdit::DM_OnSetFocus();
		if (!m_strSurfaceText.IsEmpty() && 0==GetWindowTextLength()) 
		{
			DM_Invalidate();
		}
	}

	BOOL DUIEdit::DM_OnEraseBkgnd(IDMCanvas* pCanvas)
	{
		do 
		{
			DUIWND hDUIWnd = 0;
			GetContainer()->OnGetFocusWnd(hDUIWnd);
			if ((!m_strSurfaceText.IsEmpty() || m_pSurfaceSkin)
				&& 0==GetWindowTextLength() && hDUIWnd!=m_hDUIWnd)
			{
				break;// 有蒙层时不绘制
			}
			SetMsgHandled(FALSE);
		} while (false);
		return TRUE;
	}

	void DUIEdit::DM_OnPaint(IDMCanvas * pCanvas)
	{
		DUIRichEdit::DM_OnPaint(pCanvas);
		if ((!m_strSurfaceText.IsEmpty() || m_pSurfaceSkin)
			&& 0==GetWindowTextLength() && !DM_IsFocusWnd())
		{	
			CRect rcClient;
			DV_GetClientRect(&rcClient);

			if(DMSUCCEEDED(GetContainer()->OnIsTranslucent())) //lzlon add
			{
				//pCanvas->AlphaRestore();
				rcClient.DeflateRect(m_rcInsetMargin);// 不在文字范围内的不做填充
				pCanvas->AdjustAlpha(0xff,rcClient);
			}

			if (m_pSurfaceSkin)
			{
				m_pSurfaceSkin->Draw(pCanvas,rcClient,0,0xff);
			}

			DUIDrawEnviron DrawEnviron;
			bool bNeed = false;
			if (m_pSurfaceFont)
			{
				pCanvas->SelectObject((m_pSurfaceFont));
			}
			else
			{
				DV_PushDrawEnviron(pCanvas,DrawEnviron);
				bNeed = true;
			}
			if (!m_strSurfaceText.IsEmpty())
			{
				DMColor ClrOld = pCanvas->SetTextColor(m_SurfaceTextClr);
				CStringW strTrans = DMTR(m_strSurfaceText);
				UINT uFormat = DT_SINGLELINE|DT_VCENTER;
				if (EditAlign_Center == m_uSurfaceAlign)
				{
					uFormat |= DT_CENTER;
				}
				else if (EditAlign_Right == m_uSurfaceAlign)
				{
					uFormat |= DT_RIGHT;
				}
				pCanvas->DrawText(strTrans,strTrans.GetLength(),&rcClient,uFormat);
				pCanvas->SetTextColor(ClrOld);
			}
			
			if (bNeed)
			{
				DV_PopDrawEnviron(pCanvas,DrawEnviron);
			}
		}
	}

}//namespace DM