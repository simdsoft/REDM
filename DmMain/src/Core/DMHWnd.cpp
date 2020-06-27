#include "DmMainAfx.h"
#include "DMHWnd.h"
#include <dwmapi.h>

#define  RESIZE_OFFSET								5							///< Resizeʱ���ü�ͷ����
namespace DM
{
	BEGIN_MSG_MAP(DMHWnd)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_PRINT(OnPrint)
		MSG_WM_SIZE(OnSize)
		//MSG_WM_SIZING(OnSizing)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_ACTIVATE(OnActivate)
		MSG_WM_NCACTIVATE(OnNcActivate)
		MSG_WM_SETFOCUS(OnSetFocus)
		MSG_WM_KILLFOCUS(OnKillFocus)
		MSG_WM_TIMER(OnTimer)

		MSG_WM_MOVE(OnMove)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SETCURSOR(OnSetCursor)

		MSG_WM_CREATE(OnCreate)
		MSG_WM_NCCREATE(OnNcCreate)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)   // һ��Ҫ�������Ϣ����Ȼ͸������+resize�£��治������
		MSG_WM_NCHITTEST(OnNcHitTest)    // resize�´����ͷ״̬

		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseEvent)
		MESSAGE_RANGE_HANDLER_EX(WM_KEYFIRST, WM_KEYLAST, OnKeyEvent)
		MESSAGE_RANGE_HANDLER_EX(WM_IME_STARTCOMPOSITION,WM_IME_KEYLAST,OnKeyEvent)
		MESSAGE_HANDLER_EX(WM_IME_CHAR, OnKeyEvent)
		MESSAGE_HANDLER_EX(WM_ACTIVATEAPP,OnHostMsg)

		MESSAGE_HANDLER_EX(WM_DMSPY,OnSpy)
	END_MSG_MAP()

	BEGIN_EVENT_MAP(DMHWnd)
	END_EVENT_INBASE()// ����

	DMHWnd::DMHWnd()
		:DMContainerImpl(this)
		,m_dummyWnd(this)
	{
		SetContainer(this);
		m_pHWndXmlInfo.Attach(new DMHWnd_XmlInfo(this));
		g_pDMDWndPool->AddMainDUIWnd(GetDUIWnd());			///< ���������ڵ�DUI�б�,���ڻ�������
		m_bSizeChanging = false;
		g_pDMApp->CreateRegObj((void**)&m_pDraw,NULL,DMREG_Draw);/// �����ڴ˴�����Ĭ�ϵ�draw������Ϊ�������е�DMHWnd���඼�����SetAttribute��,��DMDropWnd
		m_pCurMsgLoop = g_pDMMsgLoopTool->GetMessageLoop();
		if (!m_pCurMsgLoop)
		{
			m_pCurMsgLoop.Attach(new DMMsgLoop);
			g_pDMMsgLoopTool->AddMessageLoop(m_pCurMsgLoop.get());
		}
	}

	/// @brief ��������
	/// @param shadowStyle ��Ӱ���
	/// @return  ������Դ���.
	HWND DMHWnd::DM_CreateWindow(LPCWSTR lpszXmlId, int x/*=0*/, int y/*=0*/, int nWidth/*=0*/, int nHeight/*=0*/, HWND hWndParent/*=NULL*/, int shadowStyle/*=NWSDS_NULL*/)
	{
		LOG_INFO("[start]lpszXmlId:%s\n",lpszXmlId);
		if (m_hWnd)
		{
			LOG_ERR("[mid]m_hWnd:0x%08x is exist\n", m_hWnd);
			return m_hWnd;
		}
		DM_CreateWindowEx(lpszXmlId, DM_DEF_WINDOW_NAME, DM_DEF_STYLE, 0, x, y, nWidth, nHeight, hWndParent, NULL, shadowStyle);
		LOG_INFO("[end]-m_hWnd:0x%08x\n", m_hWnd);
		return m_hWnd;
	}

	HWND DMHWnd::DM_CreateWindowEx(LPCWSTR lpszXmlId, LPCWSTR lpWindowName,DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, PVOID lpParam, int shadowStyle/*=NWSDS_NULL*/)
	{
		LOG_INFO("[start]lpszXmlId:%s\n",lpszXmlId);
		do
		{
			// �����Ѵ���
			if (m_hWnd)
			{
				LOG_ERR("[mid]m_hWnd:0x%08x is exist\n", m_hWnd);
				break;
			}

			if (NULL == lpszXmlId)
			{
				LOG_ERR("[mid]lpszXmlIdi is NULL!!!!\n");
				break;
			}
			m_HWndData.m_lpszXmlId = lpszXmlId;

			ATOM Atom = (shadowStyle == NWSDS_DROPSHADOW)?g_pDMAppData->m_shadowAtom:g_pDMAppData->m_Atom;  // �Ƿ�ʹ����Ӱ�����ഴ��

			// ����ʱ���ȵ��õ�OnNcCreate������XML��Դ����OnNcCreate����ʧ��,�򴴽�ʧ�ܣ�m_hWndΪNULL
			DMCWnd::CreateWindowEx((LPCWSTR)Atom,lpWindowName, dwStyle, dwExStyle, x, y, nWidth, nHeight, hWndParent, lpParam);
			if (NULL == m_hWnd)
			{
				LOG_ERR("[mid]CreateWnd fail\n");
				break;
			}

			// DWM��ӰЧ��
			if (shadowStyle == NWSDS_DWMSHADOW)
				DM_EnableShadowEffect();

			// ����xml
			if (DM_ECODE_OK != LoadDMData(lpszXmlId))
			{
				LOG_ERR("[mid]LoadDMData fail\n");
				break;
			}

		} while (false);
		if (m_hWnd)
		{
			// ��ʼ����
			InitFromDMData();

			// ע����϶�
			::RegisterDragDrop(m_hWnd, &m_DropTarget);

			// ���ó�ʼ�����״̬
			g_pDMPluginTool->SetInit();

			OnAfterCreated();
		}
		
		LOG_INFO("[end]m_hWnd:0x%08x\n",m_hWnd);
		return m_hWnd;
	}

	HWND DMHWnd::DM_CreateWindowEx(void *pXmlBuf, size_t bufLen, LPCWSTR lpWindowName,DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, PVOID lpParam, int shadowStyle/*=NWSDS_NULL*/)
	{
		do
		{
			// �����Ѵ���
			if (m_hWnd)
			{
				LOG_ERR("[mid]m_hWnd:0x%08x is exist\n", m_hWnd);
				break;
			}

			if (NULL == pXmlBuf||0 == bufLen)
			{
				LOG_ERR("[mid]lpszXmlIdi is NULL!!!!\n");
				break;
			}

			ATOM Atom = (shadowStyle == NWSDS_DROPSHADOW)?g_pDMAppData->m_shadowAtom:g_pDMAppData->m_Atom;  // �Ƿ�ʹ����Ӱ�����ഴ��

			// ����ʱ���ȵ��õ�OnNcCreate������XML��Դ����OnNcCreate����ʧ��,�򴴽�ʧ�ܣ�m_hWndΪNULL
			DMCWnd::CreateWindowEx((LPCWSTR)Atom,lpWindowName, dwStyle, dwExStyle, x, y, nWidth, nHeight, hWndParent, lpParam);
			if (NULL == m_hWnd)
			{
				LOG_ERR("[mid]CreateWnd fail\n");
				break;
			}

			// DWM��ӰЧ��
			if (shadowStyle == NWSDS_DWMSHADOW)
				DM_EnableShadowEffect();

			// ����xml
			if (DM_ECODE_OK != LoadDMData(pXmlBuf, bufLen))
			{
				LOG_ERR("[mid]LoadDMData fail\n");
				break;
			}

		} while (false);
		if (m_hWnd)
		{
			// ��ʼ����
			InitFromDMData();

			// ע����϶�
			::RegisterDragDrop(m_hWnd, &m_DropTarget);

			// ���ó�ʼ�����״̬
			g_pDMPluginTool->SetInit();

			OnAfterCreated();
		}
	
		LOG_INFO("[end]m_hWnd:0x%08x\n",m_hWnd);
		return m_hWnd;
	}

	void DMHWnd::UpdateHWnd(HDC hdc,const CRect &rcInvalid,bool bUpdate)
	{
		do
		{
			if (NULL == m_pDraw)
			{
				break;
			}
			CRect rcClient;
			GetClientRect(rcClient);
			if (rcClient.IsRectEmpty())
			{
				break;
			}
			CRect rcDest = rcClient&rcInvalid;
			if (!DMSUCCEEDED(DM_UpdateShowCanvas(rcDest)))
			{
				break;
			}
			HDC dcMem = m_pShowCanvas->GetDC();
			if (m_pHWndXmlInfo->m_bTranslucent)
			{
				DM_UpdateLayeredWindow(m_pShowCanvas,m_pHWndXmlInfo->m_byAlpha,rcDest);
			}
			else
			{
				if (bUpdate)
				{
					::BitBlt(hdc, rcInvalid.left, rcInvalid.top, rcInvalid.Width(), rcInvalid.Height(), dcMem, rcInvalid.left, rcInvalid.top, SRCCOPY);
				}
				else
				{
					::BitBlt(hdc, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), dcMem, rcClient.left, rcClient.top, SRCCOPY);
				}
			}
			m_pShowCanvas->ReleaseDC(dcMem);
		} while (false);
	}

	// ������WM_PAINT��Ϣ֮����Invalidate����ʹ֮ʧЧ������ͻᱻ�ۼ�������Ȼ����һ��WM_PAINT��Ϣ��һ�εõ����£�
	// �����ܱ������ظ��ظ���ͬһ����Ҳ�Ż���Ӧ�õĸ��²���
	void DMHWnd::RedrawAll()
	{
		do
		{
			if (NULL == m_pDraw)
			{
				break;
			}
			m_pDraw->InvalidateRect(NULL,&m_rcWindow,RGN_COPY);// �����Ч��

			if (false == m_pHWndXmlInfo->m_bTranslucent)
			{
				Invalidate(FALSE);// ����ձ���
			}
			else if (m_dummyWnd.IsWindow())
			{
				m_dummyWnd.Invalidate(FALSE);
			}
		} while (false);
	}

	//---------------------------------------------------
	// Function Des: XML
	//---------------------------------------------------
	DMCode DMHWnd::LoadDMData(LPCWSTR lpszXmlId)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			DMXmlDocument XmlDoc;
			if (!DMSUCCEEDED(g_pDMAppData->InitDMXmlDocument(XmlDoc,RES_LAYOUT,lpszXmlId)))
			{
				break;
			}
			DMXmlNode XmlNode = XmlDoc.Root(MAINWND_NODE);
			if (!XmlNode.IsValid())
			{
				break;
			}

			m_pHWndXmlInfo->ResetXmlInfo();                // ������XML����

			// ����˽��Skin�ڵ�,�ⲿ�����ͷ���---------------
			DMXmlNode XmlSkin = XmlNode.FirstChild(L"skin");
			while (XmlSkin.IsValid())
			{
				g_pDMSkinPool->AddSkinPoolItem(XmlSkin);
				XmlSkin = XmlSkin.NextSibling(L"skin");
			}

			// ����˽��Style�ڵ�,�ⲿ�����ͷ���--------------
			DMXmlNode XmlStyle = XmlNode.FirstChild(L"style");
			while (XmlStyle.IsValid())
			{
				g_pDMStylePool->AddStylePoolItem(XmlStyle);
				XmlStyle = XmlStyle.NextSibling(L"style");
			}

			// ���������XML-Attribute-----------------------
			iErr = DMBase::InitDMData(XmlNode);
			if (iErr != DM_ECODE_OK)
			{
				break;
			}

			// ѭ������DUI��XML -----------------------------
			// root�ڵ㱾��Ҳ��һ��DUIWindow���ڶ��󣬵��������������"root"����ʶ��������ڵ��п�����DUIWindow�ĸ������ԣ����ǺͲ���λ����ص������Զ���Ч����Ϊ�ô������ǳ��������������ڡ�
			iErr = DUIWindow::InitDMData(XmlNode.FirstChild(DUIROOT_NODE));
		} while (false);
		return iErr;
	}

	DMCode DMHWnd::LoadDMData(void *pXmlBuf, size_t bufLen)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			DMXmlDocument XmlDoc;
			if (!XmlDoc.LoadFromBuffer(pXmlBuf, bufLen))
			{
				break;
			}
			DMXmlNode XmlNode = XmlDoc.Root(MAINWND_NODE);
			if (!XmlNode.IsValid())
			{
				break;
			}

			m_pHWndXmlInfo->ResetXmlInfo();                // ������XML����

			// ����˽��Skin�ڵ�,�ⲿ�����ͷ���---------------
			DMXmlNode XmlSkin = XmlNode.FirstChild(L"skin");
			while (XmlSkin.IsValid())
			{
				g_pDMSkinPool->AddSkinPoolItem(XmlSkin);
				XmlSkin = XmlSkin.NextSibling(L"skin");
			}

			// ����˽��Style�ڵ�,�ⲿ�����ͷ���--------------
			DMXmlNode XmlStyle = XmlNode.FirstChild(L"style");
			while (XmlStyle.IsValid())
			{
				g_pDMStylePool->AddStylePoolItem(XmlStyle);
				XmlStyle = XmlStyle.NextSibling(L"style");
			}

			// ���������XML-Attribute-----------------------
			iErr = DMBase::InitDMData(XmlNode);
			if (iErr != DM_ECODE_OK)
			{
				break;
			}

			// ѭ������DUI��XML -----------------------------
			// root�ڵ㱾��Ҳ��һ��DUIWindow���ڶ��󣬵��������������"root"����ʶ��������ڵ��п�����DUIWindow�ĸ������ԣ����ǺͲ���λ����ص������Զ���Ч����Ϊ�ô������ǳ��������������ڡ�
			iErr = DUIWindow::InitDMData(XmlNode.FirstChild(DUIROOT_NODE));
		} while (false);
		return iErr;
	}

	DMCode DMHWnd::InitFromDMData()
	{
		DMCode iErr = DM_ECODE_OK;
		do
		{
			// ע��tip
			m_pToolTip.Release();
			if (!DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&m_pToolTip,m_pHWndXmlInfo->m_strRegTip,DMREG_ToolTip)))
			{
				CStringW szInfo = m_pHWndXmlInfo->m_strRegTip;
				szInfo += L"ע��tooltipʧ��";
				DMASSERT_EXPR(0,szInfo);
			}
			m_pCurMsgLoop->AddMessageFilter(m_pToolTip.get());

			// ���ô������Կ��϶� ----------------------
			DWORD dwStyle = WS_MAXIMIZEBOX| WS_THICKFRAME;
			if (m_pHWndXmlInfo->m_bResizable)
			{
				ModifyStyle(0,dwStyle);
			}
			else
			{
				ModifyStyle(dwStyle,0);
			}

			DWORD dwExStyle = WS_EX_LAYERED;
			if (m_pHWndXmlInfo->m_bTranslucent)
			{
				ModifyStyleEx(0,dwExStyle);
			}

			// ���ô��ڱ��� ----------------------------
			SetWindowText(m_pHWndXmlInfo->m_strTitle);

			// ����͸�������ɰ�--�����߲����UpdateLayeredWindow�Ա�SetLayeredWindowAttributes���ɲο����ϣ�http://hgy413.com/1865.html
			if (m_pHWndXmlInfo->m_bTranslucent)// �ڴ����ɰ�ʱ��alphaֵֻ����alpha���ʱʹ�ü���,��ʱʹ��UpdateLayeredWindow����
			{
				SetWindowLongPtr(GWL_EXSTYLE, GetWindowLongPtr(GWL_EXSTYLE)|WS_EX_LAYERED);
				m_dummyWnd.CreateWindowEx(LPCWSTR(g_pDMAppData->m_Atom),L"DM_DUMMY_WND",WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE,0,0,10,10,m_hWnd,NULL);
				m_dummyWnd.SetWindowLongPtr(GWL_EXSTYLE,m_dummyWnd.GetWindowLongPtr(GWL_EXSTYLE) | WS_EX_LAYERED);
				::SetLayeredWindowAttributes(m_dummyWnd.m_hWnd,0,0,LWA_ALPHA);
				m_dummyWnd.ShowWindow(SW_SHOWNOACTIVATE);
			}
			else if(0xFF != m_pHWndXmlInfo->m_byAlpha)// ע����else if��û���ɰ�ʱ���͵��Լ����÷ֲ����ԣ�SetLayeredWindowAttributes������ͳһ��͸����
			{
				ModifyStyleEx(0,WS_EX_LAYERED);
				::SetLayeredWindowAttributes(m_hWnd,0,m_pHWndXmlInfo->m_byAlpha,LWA_ALPHA);
			}

			// ���ÿ�� ------------------------------
			CRect rcClient;
			GetClientRect(rcClient);
			if (rcClient.IsRectEmpty())
			{
				// �ڴ���WM_SIZEʱ�����ò���
				SetWindowPos(NULL,0,0,m_pHWndXmlInfo->m_szInit.cx,m_pHWndXmlInfo->m_szInit.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
			}

			else
			{
				DM_FloatLayout(rcClient);
				DV_UpdateChildLayout();// ������ô˴�����DUIλ�ã���Ȼ�ⲿ���ô���û�в���
			}

			// ������
			RedrawAll();
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des: ��Ϣ�ַ�ϵ�к���
	//---------------------------------------------------
	void DMHWnd::OnPaint(HDC hdc)
	{
		if (m_pDraw&&false == m_pDraw->IsInvalidate()&&false == m_pHWndXmlInfo->m_bTranslucent)
		{/*hgy: һ��˴�Ϊ����window��סDUI����������WM_PAINT��Ϣ�����ڲ�������������
		 ��ʱû��������Ч��,��������������Ҳ�ܼ򵥣�ֱ�Ӱѻ��滭��������Ч���ͺ���*/
			CRect rcUp;
			::GetClipBox(hdc, rcUp);// ���Ա����˴����Ǻ�PAINTSTRUCT.rcPaintһ��
			UpdateHWnd(hdc, rcUp);
		}
		else
		{
			OnPrint(hdc, NOMAL_PAINT);
		}
	}

	void DMHWnd::OnPrint(HDC hdc, UINT uFlags)
	{
		if (m_pDraw)
		{
			m_bSizeChanging = false;
			DM::CArray<CRect> Array;
			int iNum = m_pDraw->GetInvalidateRect(Array);
			m_pDraw->Render(this);
			for (int i=0; i<iNum; i++)
			{
				UpdateHWnd(hdc, Array[i]);
			}
		}
	}

	void DMHWnd::OnSize(UINT nType, CSize size)
	{
		do
		{
			if (IsIconic())
			{
				break;
			}

			if (0 == size.cx||0 == size.cy)
			{
				break;
			}

			if (m_pDraw)
			{
				m_pDraw->ResizeCanvas(size);
			}
			m_bSizeChanging = true;
			DM_FloatLayout(CRect(0,0,size.cx,size.cy));
			RedrawAll();
		} while (false);
	}

	void DMHWnd::OnSizing(UINT nSide, LPRECT lpRect)
	{
		if (NULL!=lpRect)
		{
			CRect Rect = lpRect;
			m_bSizeChanging=true;
			ScreenToClient(Rect);
			DM_FloatLayout(Rect);
			RedrawAll();
		}
	}

	void DMHWnd::OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
	{// http://hgy413.com/1870.html
		do
		{
			HMONITOR hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONULL);
			if (NULL == hMonitor)
			{
				break;
			}
			MONITORINFO mi = {sizeof(MONITORINFO)};
			::GetMonitorInfo(hMonitor, &mi);
			CRect rcWork = mi.rcWork;
			CRect rcMonitor = mi.rcMonitor;
			// ���ʱλ�ã���С
			lpMMI->ptMaxPosition.x = abs(rcWork.left - rcMonitor.left) - 1 + m_pHWndXmlInfo->m_rcMaxInset.left;
			lpMMI->ptMaxPosition.y = abs(rcWork.top - rcMonitor.top) - 1 + m_pHWndXmlInfo->m_rcMaxInset.top;
			lpMMI->ptMaxSize.x = abs(rcWork.Width()) + 2 - m_pHWndXmlInfo->m_rcMaxInset.left - m_pHWndXmlInfo->m_rcMaxInset.right;
			lpMMI->ptMaxSize.y = abs(rcWork.Height()) + 2 - m_pHWndXmlInfo->m_rcMaxInset.top - m_pHWndXmlInfo->m_rcMaxInset.bottom;

			lpMMI->ptMinTrackSize = CPoint(m_pHWndXmlInfo->m_szMin.cx, m_pHWndXmlInfo->m_szMin.cy);

			// ����϶��ı䴰�ڴ�Сʱ�������Сsize
			lpMMI->ptMaxTrackSize.x = abs(rcWork.Width()) + 2 - m_pHWndXmlInfo->m_rcMaxInset.left - m_pHWndXmlInfo->m_rcMaxInset.right;
			lpMMI->ptMaxTrackSize.y = abs(rcWork.Height()) + 2 - m_pHWndXmlInfo->m_rcMaxInset.top - m_pHWndXmlInfo->m_rcMaxInset.bottom;
			if (-1!=m_pHWndXmlInfo->m_szMax.cx)
			{
				lpMMI->ptMaxTrackSize.x = m_pHWndXmlInfo->m_szMax.cx;
			}
			if (-1!=m_pHWndXmlInfo->m_szMax.cy)
			{
				lpMMI->ptMaxTrackSize.y = m_pHWndXmlInfo->m_szMax.cy;
			}
		} while (false);
	}

	void DMHWnd::OnDestroy()
	{
		DUIWindow::DM_SendMessage(WM_DESTROY);    // ����DUI����
		if (m_pDraw)
		{
			m_pDraw.Release();
		}
		if (m_pToolTip)
		{
			if (m_pCurMsgLoop)
			{
				m_pCurMsgLoop->RemoveMessageFilter(m_pToolTip.get());
			}
			m_pToolTip->Clear();
		}

		if (m_dummyWnd.IsWindow())
		{
			m_dummyWnd.DestroyWindow();
		}
		::RevokeDragDrop(m_hWnd);						    // ���ؿ��϶�
		g_pDMDWndPool->RemoveMainDUIWnd(GetDUIWnd());		///< ���������ڵ�DUI�б�,���ڻ�������
		OnAfterClosed();
	}

	void DMHWnd::OnActivate(UINT nState, BOOL bMinimized, HWND wndOther)
	{
		if (nState!=WA_INACTIVE)
		{
			::SetFocus(m_hWnd);// �������ý��㣬��Ȼ������Ӧ��������Ϣ
		}
		else
		{
			::SetFocus(NULL);
		}
	}

	void DMHWnd::OnSetFocus(HWND wndOld)
	{
		OnFrameEvent(WM_SETFOCUS,0,0);
	}

	void DMHWnd::OnKillFocus(HWND wndFocus)
	{
		OnFrameEvent(WM_KILLFOCUS,0,0);
	}

	void DMHWnd::OnTimer(UINT_PTR idEvent)
	{
		FastTimerID nIdEvent((DWORD)idEvent);
		if (nIdEvent.m_Flag)
		{
			DUIWindow *pWnd = g_pDMDWndPool->FindDUIWnd(nIdEvent.m_hDUIWnd);
			if (pWnd)
			{
				if (this == pWnd)
				{
					OnDUITimer(nIdEvent.m_dwTimerID);// ��HWnd�в�����DUI����Ϣӳ���
				}
				else
				{
					pWnd->DM_SendMessage(WM_TIMER,nIdEvent.m_dwTimerID,NULL);
				}
			}
			else
			{
				::KillTimer(m_hWnd, idEvent);
			}
		}
		else
		{
			SetMsgHandled(FALSE);
		}
	}

	void DMHWnd::OnDUITimer(char id)
	{
		if (TIMER_NEXTFRAME == id)
		{
			 OnTimeline();
		}
	}

	void DMHWnd::OnMove(CPoint pt)
	{
		do
		{
			int iCount = (int)g_pDMDWndPool->m_RealDUIWndArray.GetCount();
			if (iCount<=0)
			{
				break;
			}
			DUIWND hDUIWnd = 0;
			for (int i=0;i<iCount;i++)
			{
				hDUIWnd = g_pDMDWndPool->m_RealDUIWndArray[i];
				DUIWindowPtr pWnd = g_pDMDWndPool->FindDUIWnd(hDUIWnd);
				if (pWnd)
				{
					IDUIRealWnd* pRealWnd = (IDUIRealWnd*)pWnd;
					pRealWnd->OnMove(pt);
				}
			}
		} while (false);
		SetMsgHandled(FALSE);
	}

	void DMHWnd::OnMouseMove(UINT nFlags, CPoint point)
	{
		if (!m_HWndData.m_bTrackFlag)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize	    = sizeof(tme);
			tme.hwndTrack   = m_hWnd;
			tme.dwFlags     = TME_LEAVE;
			tme.dwHoverTime = 0;
			m_HWndData.m_bTrackFlag = (TRUE == TrackMouseEvent(&tme));
		}

		// ֱ�ӵ���OnMouseEvent,��ʵWM_MOUSEMOVE����WM_MOUSEFIRST
		OnMouseEvent(WM_MOUSEMOVE,nFlags,MAKELPARAM(point.x,point.y));
	}

	void DMHWnd::OnMouseLeave()
	{
		m_HWndData.m_bTrackFlag = false;
		OnFrameEvent(WM_MOUSELEAVE,0,0);
	}

	BOOL DMHWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		if (IsWindowEnabled())
		{// win10
			ScreenToClient(&pt);
			return 0!=OnFrameEvent(WM_MOUSEWHEEL,MAKEWPARAM(nFlags,zDelta),MAKELPARAM(pt.x,pt.y));
		}
		return FALSE;
	}

	BOOL DMHWnd::OnNcActivate(BOOL bActive)
	{
		return TRUE;
	}

	BOOL DMHWnd::OnEraseBkgnd(HDC hdc)
	{
		return TRUE;
	}

	BOOL DMHWnd::OnSetCursor(HWND hwnd, UINT nHitTest, UINT message)
	{
		if (hwnd != m_hWnd)
		{
			return FALSE;
		}

		if (HTCLIENT == nHitTest)
		{
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			return OnFrameEvent(WM_SETCURSOR,0,MAKELPARAM(pt.x,pt.y))!=0;
		}

		return 0 != DefWindowProc();
	}

	UINT DMHWnd::OnNcHitTest(CPoint point)
	{
		if (m_pHWndXmlInfo->m_bResizable && !::IsZoomed(m_hWnd))
		{
			ScreenToClient(&point);
			if (point.x > m_rcWindow.right - RESIZE_OFFSET)
			{
				if (point.y > m_rcWindow.bottom - RESIZE_OFFSET)
				{
					return HTBOTTOMRIGHT;
				}
				else if (point.y < RESIZE_OFFSET)
				{
					return HTTOPRIGHT;
				}
				return HTRIGHT;
			}
			else if (point.x < RESIZE_OFFSET)
			{
				if (point.y > m_rcWindow.bottom - RESIZE_OFFSET)
				{
					return HTBOTTOMLEFT;
				}
				else if (point.y < RESIZE_OFFSET)
				{
					return HTTOPLEFT;
				}
				return HTLEFT;
			}
			else if (point.y > m_rcWindow.bottom - RESIZE_OFFSET)
			{
				return HTBOTTOM;
			}
			else if (point.y < RESIZE_OFFSET)
			{
				return HTTOP;
			}
		}

		return HTCLIENT;   //��Ĭ�Ϸ��طǿͻ���
	}

	int DMHWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		DUIWindow::SetContainer(this);
		return 0;
	}


	LRESULT DMHWnd::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
	{
		return TRUE;
	}

	LRESULT DMHWnd::OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		// ���Ͽɲο�http://blog.csdn.net/hgy413/article/details/6863924
		do
		{
			if (!bCalcValidRects)
			{
				break;
			}

			if (!(GetStyle()&WS_POPUP))
			{
				break;
			}

			LPNCCALCSIZE_PARAMS pParam = (LPNCCALCSIZE_PARAMS)lParam;
			if (SWP_NOSIZE == (SWP_NOSIZE & pParam->lppos->flags))
			{
				break;
			}

			CRect rcClient;
			GetClientRect(rcClient);
			ClientToScreen(rcClient);

			if (0 == (SWP_NOMOVE & pParam->lppos->flags))
			{
				rcClient.left = pParam->lppos->x;
				rcClient.top = pParam->lppos->y;
			}
			rcClient.right  = rcClient.left + pParam->lppos->cx;
			rcClient.bottom = rcClient.top + pParam->lppos->cy;
			pParam->rgrc[0] = rcClient;
		} while (false);
		return 0;
	}

	LRESULT DMHWnd::OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		do
		{

			OnFrameEvent(uMsg,wParam,lParam);    //�������Ϣת����DUIWindow����
			if (m_pToolTip)
			{
				if (m_dwState & DMSTATE_DESTROYED)
				{
					break;
				}
				DUIWindow *pHoverWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);
				if(!pHoverWnd || pHoverWnd->DM_IsDisable(true))
				{
					m_pToolTip->Hide();
				}
				else
				{
					CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					DMToolTipInfo tipInfo;
					if(DMSUCCEEDED(pHoverWnd->DV_OnUpdateToolTip(pt,tipInfo)))
					{
						m_pToolTip->Update(&tipInfo);
					}
					else
					{// hide tooltip
						m_pToolTip->Hide();
					}
				}
			}
		} while (false);
		return 0;
	}

	LRESULT DMHWnd::OnKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (WM_SYSKEYDOWN == uMsg
			||WM_SYSKEYUP == uMsg)
		{
			DUIWindow *pFocusWnd = g_pDMDWndPool->FindDUIWnd(m_FocusMgr.GetFocusedWnd());
			if(!pFocusWnd||!(pFocusWnd->DV_OnGetDlgCode()&DMDLGC_WANTSYSKEY))
			{
				SetMsgHandled(FALSE);
				return 0;
			}
		}
		LRESULT lRet = OnFrameEvent(uMsg,wParam,lParam);
		SetMsgHandled(DUIWindow::IsMsgHandled());
		return lRet;
	}

	LRESULT DMHWnd::OnHostMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		if (IsWindow())
		{
			return OnFrameEvent(uMsg,wParam,lParam);
		}
		return 0;
	}

	DMCode DMHWnd::DM_AnimateWindow(DWORD dwTime,DWORD dwFlags)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			if (NULL == m_pDraw)
			{
				break;
			}
			if (false == m_pHWndXmlInfo->m_bTranslucent)
			{
				if (dwFlags&AW_HIDE)
				{
					g_pDMDWndPool->HideAllRealDUIWnd(m_hDUIWnd);// ������realwnd����ʵ����
				}

				if (TRUE == ::AnimateWindow(m_hWnd,dwTime,dwFlags))
				{
					iErr = DM_ECODE_OK;
				}
				break;
			}

			CRect rcClient;
			GetClientRect(&rcClient);
			CRect rcShow(rcClient);
			DMSmartPtrT<IDMCanvas> pCanvas;
			g_pDMRender->CreateCanvas(rcShow.Width(),rcShow.Height(),&pCanvas);
			RedrawAll();
			m_pDraw->Render(this);
			if (!DMSUCCEEDED(DM_UpdateShowCanvas(rcClient)))
			{
				break;
			}
			int nSteps = dwTime/10;
			if (nSteps<=1)
			{
				break;
			}
			if (dwFlags&AW_HIDE)
			{
				g_pDMDWndPool->HideAllRealDUIWnd(m_hDUIWnd);// ������realwnd����ʵ����
				if (dwFlags& AW_SLIDE)
				{
					LONG  x1 = rcShow.left;LONG  x2 = rcShow.left;LONG  y1 = rcShow.top;LONG  y2 = rcShow.top;
					LONG * x = &rcShow.left;LONG * y = &rcShow.top;

					if (dwFlags&AW_HOR_POSITIVE)// ������
					{
						x1 = rcShow.left,x2 = rcShow.right;x = &rcShow.left;
					}
					else if (dwFlags&AW_HOR_NEGATIVE)// ���ҵ���
					{
						x1 = rcShow.right,x2 = rcShow.left;x=&rcShow.right;
					}

					if (dwFlags&AW_VER_POSITIVE)// ���ϵ���
					{
						y1 = rcShow.top,y2 = rcShow.bottom;y = &rcShow.top;
					}
					else if (dwFlags & AW_VER_NEGATIVE)// ���µ���
					{
						y1 = rcShow.bottom,y2 = rcShow.top;y = &rcShow.bottom;
					}
					LONG xStepLen = (x2-x1)/nSteps;LONG yStepLen = (y2-y1)/nSteps;
					for (int i=0;i<nSteps;i++)
					{
						*x += xStepLen;*y += yStepLen;
						pCanvas->ClearRect(rcClient,0);
						CPoint ptAnchor;
						if (dwFlags&AW_VER_NEGATIVE)ptAnchor.y = rcClient.bottom-rcShow.Height();
						if (dwFlags&AW_HOR_NEGATIVE)ptAnchor.x = rcClient.right-rcShow.Width();
						pCanvas->BitBlt(m_pShowCanvas,ptAnchor.x,ptAnchor.y,rcShow);
						DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
						Sleep(10);
					}
					ShowWindow(SW_HIDE);
					iErr = DM_ECODE_OK;
					break;
				}
				if (dwFlags&AW_CENTER)
				{
					int xStep = rcShow.Width()/(2*nSteps);int yStep = rcShow.Height()/(2*nSteps);
					for (int i=0;i<nSteps;i++)
					{
						rcShow.DeflateRect(xStep,yStep);
						pCanvas->ClearRect(rcClient,0);
						pCanvas->BitBlt(m_pShowCanvas,rcShow.top,rcShow.left,rcShow);
						DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
						Sleep(10);
					}
					ShowWindow(SW_HIDE);
					iErr = DM_ECODE_OK;
					break;
				}
				if (dwFlags&AW_BLEND)
				{
					BYTE byAlpha = 255;
					for (int i=0;i<nSteps;i++)
					{
						byAlpha-=255/nSteps;
						DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
						Sleep(10);
					}
					ShowWindow(SW_HIDE);
					iErr = DM_ECODE_OK;
					break;
				}
				break;
			}
			//==============
			if (!IsWindowVisible())
			{
				SetWindowPos(0,0,0,0,0,SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER|SWP_NOSIZE);
			}
			SetWindowPos(HWND_TOP,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			if (dwFlags& AW_SLIDE)
			{
				LONG  x1 = rcShow.left;LONG  x2 = rcShow.left;LONG  y1 = rcShow.top;LONG  y2 = rcShow.top;
				LONG * x =&rcShow.left;LONG * y =&rcShow.top;
				if (dwFlags&AW_HOR_POSITIVE)
				{
					x1=rcShow.left,x2=rcShow.right;rcShow.right=rcShow.left,x=&rcShow.right;
				}
				else if(dwFlags&AW_HOR_NEGATIVE)
				{
					x1=rcShow.right,x2=rcShow.left;rcShow.left=rcShow.right,x=&rcShow.left;
				}
				if (dwFlags&AW_VER_POSITIVE)
				{
					y1=rcShow.top,y2=rcShow.bottom;rcShow.bottom=rcShow.top,y=&rcShow.bottom;
				}
				else if(dwFlags & AW_VER_NEGATIVE)
				{
					y1=rcShow.bottom,y2=rcShow.top;rcShow.top=rcShow.bottom,y=&rcShow.top;
				}
				LONG xStepLen = (x2-x1)/nSteps;LONG yStepLen = (y2-y1)/nSteps;
				for (int i=0;i<nSteps;i++)
				{
					*x += xStepLen;*y += yStepLen;
					pCanvas->ClearRect(rcClient,0);
					CPoint ptAnchor;
					if (dwFlags&AW_VER_POSITIVE)ptAnchor.y=rcClient.bottom-rcShow.Height();
					if (dwFlags & AW_HOR_POSITIVE)ptAnchor.x=rcClient.right-rcShow.Width();
					pCanvas->BitBlt(m_pShowCanvas,ptAnchor.x,ptAnchor.y,rcShow);
					DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
					Sleep(10);
				}
				DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
				iErr = DM_ECODE_OK;
				break;
			}
			if(dwFlags&AW_CENTER)
			{
				int xStep=rcShow.Width()/(2*nSteps);
				int yStep=rcShow.Height()/(2*nSteps);
				rcShow.left=rcShow.right=(rcShow.left+rcShow.right)/2;
				rcShow.top=rcShow.bottom=(rcShow.top+rcShow.bottom)/2;
				for(int i=0;i<nSteps;i++)
				{
					rcShow.InflateRect(xStep,yStep);
					pCanvas->ClearRect(rcClient,0);
					pCanvas->BitBlt(m_pShowCanvas,rcShow.top,rcShow.left,rcShow);
					DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
					Sleep(10);
				}
				DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
				iErr = DM_ECODE_OK;
				break;
			}
			if(dwFlags&AW_BLEND)
			{
				BYTE byAlpha=0;
				for(int i=0;i<nSteps;i++)
				{
					byAlpha+=255/nSteps;
					DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
					Sleep(10);
				}
				DM_UpdateLayeredWindow(pCanvas,m_pHWndXmlInfo->m_byAlpha,NULL);
				iErr = DM_ECODE_OK;
				break;
			}
		} while (false);
		return iErr;
	}

	DUIWindow* DMHWnd::GetAnimateOwnerWnd()
	{
		return this;
	}

	//---------------------------------------------------
	// Function Des: ��������
	DMCode DMHWnd::OnSetCaptureWnd(DUIWND DUIWnd,DUIWNDPtr pOldDUIWnd)
	{
		SetCapture();
		return DMContainerImpl::OnSetCaptureWnd(DUIWnd,pOldDUIWnd);
	}
	DMCode DMHWnd::OnReleaseCaptureWnd()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			iErr = DMContainerImpl::OnReleaseCaptureWnd();
			::ReleaseCapture();
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			PostMessage(WM_MOUSEMOVE,0,MAKELPARAM(pt.x,pt.y));
		} while (false);
		return iErr;
	}

	DMCode DMHWnd::OnFireEvent(DMEventArgs &Evt)
	{
		return DMHandleEvent(&Evt);
	}

	DMCode DMHWnd::OnGetDraw(IDMDraw** ppObj)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			if (NULL == ppObj||NULL == m_pDraw)
			{
				break;
			}

			*ppObj = m_pDraw;
			m_pDraw->AddRef();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd::OnGetCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas**ppCanvas)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			if (NULL == ppCanvas||NULL == lpRect||NULL == m_pDraw)
			{
				break;
			}
			CRect rc = lpRect;
			g_pDMRender->CreateCanvas(rc.Width(), rc.Height(),ppCanvas);
			(*ppCanvas)->OffsetViewportOrg(-rc.left,-rc.top);
			(*ppCanvas)->SelectObject(g_pDMFontPool->GetFont(L""));
			(*ppCanvas)->SetTextColor(PBGRA(0,0,0,0xFF));

			if(!(dcFlags & DMOLEDC_NODRAW))
			{
				DMSmartPtrT<IDMCanvas> pMemCanvas;
				m_pDraw->GetCanvas(&pMemCanvas);
				if (pMemCanvas)
				{
					(*ppCanvas)->BitBlt(pMemCanvas,rc.left,rc.top,&rc,SRCCOPY);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd::OnReleaseCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas*pCanvas)
	{
		do
		{
			if (NULL == lpRect||NULL == m_pDraw)
			{
				break;
			}
			if (dcFlags&DMOLEDC_NODRAW)
			{
				break;
			}
			if (true == m_bSizeChanging)
			{
				break;
			}

			CRect rc = lpRect;
			DMSmartPtrT<IDMCanvas> pMemCanvas;
			m_pDraw->GetCanvas(&pMemCanvas);
			if (pMemCanvas)
			{
				pMemCanvas->BitBlt(pCanvas,rc.left,rc.top,&rc,SRCCOPY);
			}
			DMAutoDC dc(m_hWnd);// �˴���Ҫ����ĻDC,��Ϊ�Ƿֲ㴰��ʹ����bitbltֱ�ӻ���,δ������ת��
			UpdateHWnd(dc,lpRect,true);
		} while (false);
		pCanvas->Release();
		return DM_ECODE_OK;
	}

	HWND   DMHWnd::OnGetHWnd()
	{
		return m_hWnd;
	}

	DMCode DMHWnd::OnGetContainerRect(LPRECT lpRect)
	{
		*lpRect = m_rcWindow;///hgy413 note: ����ֱ��д��lpRect = m_rcWindow������ֻ��ȡָ�룬���Ǹ�ֵ
		return DM_ECODE_OK;
	}

	DMCode DMHWnd::OnIsTranslucent()
	{
		if (m_pHWndXmlInfo->m_bTranslucent)
		{
			return DM_ECODE_OK;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMHWnd::OnUpdateWindow()
	{
		if (m_dummyWnd.IsWindow())
		{
			::UpdateWindow(m_dummyWnd.m_hWnd);
		}
		else
		{
			::UpdateWindow(m_hWnd);
		}
		return DM_ECODE_OK;
	}

	DMCode DMHWnd::OnForceUpdateWindow()
	{
		DMAutoDC dc(m_hWnd);
		OnPrint(dc,NOMAL_PAINT);
		return DM_ECODE_OK;
	}

	DMCode DMHWnd::OnUpdateRect(LPCRECT lpRect,DUIWND hDUIWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			if (NULL == m_pDraw
				||NULL == lpRect)
			{
				break;
			}

			m_pDraw->InvalidateRect(hDUIWnd,lpRect,RGN_OR);// �����Ч��
			if (m_dummyWnd.IsWindow())
			{
				m_dummyWnd.Invalidate(FALSE);
			}
			else
			{
				Invalidate(FALSE);// ����ձ���
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMHWnd::OnClientToScreen(LPRECT lpRect)
	{
		ClientToScreen(lpRect);
		return DM_ECODE_OK;
	}

	DMCode DMHWnd::OnIsSizChanging()
	{
		if (m_bSizeChanging)
		{
			return DM_ECODE_OK;
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMHWnd::OnRegisterTimeline(IDMTimeline *pHandler)
	{
		DMCode iErr = DMContainerImpl::OnRegisterTimeline(pHandler);
		if (DMSUCCEEDED(iErr))
		{
			if (1 == m_TimelineList.GetCount())// ��һ�������
			{
				DM_SetTimer(TIMER_NEXTFRAME,10);
			}
		}
		return iErr;
	}

	DMCode DMHWnd::OnUnregisterTimeline(IDMTimeline *pHandler)
	{
		DMCode iErr = DMContainerImpl::OnUnregisterTimeline(pHandler);
		if (DMSUCCEEDED(iErr))
		{
			if (0 == m_TimelineList.GetCount())
			{
				DM_KillTimer(TIMER_NEXTFRAME);
			}
		}
		return iErr;
	}

	CStringW  DMHWnd::OnGetTransId()
	{
		return m_pHWndXmlInfo->m_strTransId;
	}

	//---------------------------------------------------
	// Function Des: spy++����
	LRESULT DMHWnd::OnSpy(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (DMSPY_INIT == wParam)
		{
			g_pDMSpyTool->m_hMainDUIWnd = GetDUIWnd();
		}
		return g_pDMSpyTool->OnSpy(uMsg, wParam, lParam);
	}

	DMCode DMHWnd::DM_UpdateLayeredWindow(IDMCanvas* pCanvas,BYTE byAlpha, LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			CRect rcWindow;
			GetWindowRect(rcWindow);
			if (rcWindow.IsRectEmpty())
			{
				break;
			}
			CRect rcInvalid;
			if (NULL == lpRect)
			{
				rcInvalid = rcWindow;
				rcInvalid.OffsetRect(-rcWindow.TopLeft());
			}
			else
			{
				rcInvalid = lpRect;
			}

			BOOL bRet = FALSE;
			HDC dcMem = pCanvas->GetDC();
			BLENDFUNCTION bf = {AC_SRC_OVER,0,byAlpha,AC_SRC_ALPHA};
			if (g_pDMAppData->m_fun_UpdateLayeredWindowIndirect)
			{
				CPoint ptDst = rcWindow.TopLeft();
				CSize  szDst = rcWindow.Size();
				CPoint ptSrc(0,0);
				DMUPDATELAYEREDWINDOWINFO ulinfo = {sizeof(ulinfo),NULL,&ptDst,&szDst,dcMem,&ptSrc,0,&bf,ULW_ALPHA,&rcInvalid};
				bRet = g_pDMAppData->m_fun_UpdateLayeredWindowIndirect(m_hWnd,&ulinfo);
			}
			if (FALSE == bRet)
			{
				DMAutoDC hdcDst;
				bRet = ::UpdateLayeredWindow(m_hWnd,hdcDst,&rcWindow.TopLeft(),&rcWindow.Size(),dcMem,&CPoint(0,0),0,&bf,ULW_ALPHA);
			}
			pCanvas->ReleaseDC(dcMem);
			if (bRet)
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMHWnd::DM_UpdateShowCanvas(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do
		{
			if (NULL == m_pDraw)
			{
				break;
			}
			//1 ��hsl�仯ʱ��Ĭ��Ϊ����ͬһ����
			DMSmartPtrT<IDMCanvas> pMemCanvas;
			m_pDraw->GetCanvas(&pMemCanvas);
			if (NULL == pMemCanvas)
			{
				break;
			}
			if (0 == m_pHWndXmlInfo->m_H&&100==m_pHWndXmlInfo->m_S&&100==m_pHWndXmlInfo->m_L)
			{
				if (m_pShowCanvas!=pMemCanvas)
				{
					m_pShowCanvas.Release();
					m_pShowCanvas = pMemCanvas;
				}
				iErr = DM_ECODE_OK;
				break;// ����
			}
			//2.��hsl�仯
			CSize size;
			pMemCanvas->GetSize(size);
			CRect rcCanvas(0,0,size.cx,size.cy);
			CRect rcDest = (NULL==lpRect)?rcCanvas:lpRect;
			IntersectRect(rcDest,rcDest,rcCanvas);
			if (rcDest.IsRectEmpty())
			{
				break;
			}
			//2.1.δ��ʼ���ȳ�ʼ��
			if (m_pShowCanvas == pMemCanvas)
			{
				m_pShowCanvas.Release();// ���ͷ����ü���
			}
			if (m_pShowCanvas.isNull())
			{
				g_pDMRender->CreateCanvas(size.cx,size.cy,&m_pShowCanvas);
				m_pShowCanvas->BitBlt(pMemCanvas,0,0,rcCanvas);//�´�����ֱ�Ӹ���
				m_pShowCanvas->AdjustHSL32(m_pHWndXmlInfo->m_H,m_pHWndXmlInfo->m_S, m_pHWndXmlInfo->m_L,rcCanvas);
			}
			else
			{
				CSize showsize;
				m_pShowCanvas->GetSize(showsize);
				if (showsize!=size)	//2.2.��ʼ���ˣ���������С��һ��
				{
					m_pShowCanvas->Resize(size);
					m_pShowCanvas->BitBlt(pMemCanvas,0,0,rcCanvas);//�ص��������������,ֱ�Ӹ���
					m_pShowCanvas->AdjustHSL32(m_pHWndXmlInfo->m_H,m_pHWndXmlInfo->m_S, m_pHWndXmlInfo->m_L,rcCanvas);
				}
				else//2.3.��ʼ���ˣ�������Сһ��,������Ч��
				{
					m_pShowCanvas->BitBlt(pMemCanvas,rcDest.left,rcDest.top,rcDest);
					m_pShowCanvas->AdjustHSL32(m_pHWndXmlInfo->m_H,m_pHWndXmlInfo->m_S, m_pHWndXmlInfo->m_L,rcDest);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	BOOL DMHWnd::DM_EnableShadowEffect(BOOL bEnabled/* = TRUE*/, int margin/* = 1 */)
	{
		DMAppData* thisApp = g_pDMAppData;
		if (thisApp->m_fun_DwmIsCompositionEnabled) {
			BOOL bSystemEnabled = FALSE;
			thisApp->m_fun_DwmIsCompositionEnabled(&bSystemEnabled);

			if (bSystemEnabled) {
				// apply shadow
				int v = bEnabled ? DWMNCRP_ENABLED : DWMNCRP_USEWINDOWSTYLE;
				HRESULT hr = thisApp->m_fun_DwmSetWindowAttribute(m_hWnd, DWMWA_NCRENDERING_POLICY, &v, sizeof(v));

				if (bEnabled && SUCCEEDED(hr)) {
					MARGINS margins = { margin };
					hr = thisApp->m_fun_DwmExtendFrameIntoClientArea(m_hWnd, &margins);
					return SUCCEEDED(hr);
				}
			}
		}

		return FALSE;
	}

	void DMHWnd::OnAfterCreated()
	{

	}

	void DMHWnd::OnAfterClosed()
	{
		  if (DMSUCCEEDED(g_pDMApp->IsRun(m_hWnd)))
		  {
			  ::PostQuitMessage(1);
		  }
	}


}//namespace DM
