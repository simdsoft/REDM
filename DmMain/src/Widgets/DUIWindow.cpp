#include "DmMainAfx.h"
#include "DUIWindow.h"

namespace DM
{
	typedef DMXmlDocument* (*fun_cbGetSubXmlDoc)(LPCWSTR,LPCWSTR);
	extern 	fun_cbGetSubXmlDoc  g_pGetSubXmlDoc;
	DUIWindow::DUIWindow()
	{
		m_rcWindow.SetRect(POS_INIT,POS_INIT,POS_INIT,POS_INIT); // δ��ɲ���ʱ��ʼֵ
		m_pDUIXmlInfo.Attach(new DUIWindow_XmlInfo(this));
		g_pDMApp->CreateRegObj((void**)&m_pLayout,NULL,DMREG_Layout);
		DMASSERT(NULL != m_pLayout);
		m_pLayout->SetOwner(this);
		m_dwDUIState  = DUIWNDSTATE_Normal;
		m_pContainer   = NULL;
		m_hDUIWnd      = g_pDMDWndPool->NewDUIWnd(this);
		m_bFloatLayout = false;

		// ע��Event
		// �����׼�¼�
		DMADDEVENT(DMEventCmdArgs::EventID);//
		DMADDEVENT(DMEventHoverCmdArgs::EventID);
		DMADDEVENT(DMEventLeaveCmdArgs::EventID);
		DMADDEVENT(DMEventSizeChangedCmdArgs::EventID);
		DMADDEVENT(DMEventCheckChangingCmdArgs::EventID);
		DMADDEVENT(DMEventCheckChangedCmdArgs::EventID);
		DMADDEVENT(DMEventChildLayoutFinishedArgs::EventID);

		// focus
		DMADDEVENT(DMEventSetFocusArgs::EventID);
		DMADDEVENT(DMEventKillFocusArgs::EventID);
	}

	DUIWindow::~DUIWindow()
	{
		g_pDMDWndPool->DestoryDUIWnd(m_hDUIWnd);

		// �Ƴ�ע��Event
		m_EventMgr.RemoveAllEvent();
	}

	//---------------------------------------------------
	// Function Des:��������
	LPCWSTR DUIWindow::GetName()
	{
		return m_pDUIXmlInfo->m_strName;
	}

	int DUIWindow::GetID()
	{
		return m_pDUIXmlInfo->m_iID;
	}

	DMCode DUIWindow::SetID(int id)
	{
		m_pDUIXmlInfo->m_iID = id;
		return DM_ECODE_OK;
	}

	DUIWindow* DUIWindow::FindChildByName(LPCWSTR lpszName,bool bPanelFind/* = false*/)
	{
		DUIWindow* pFindWnd = NULL;
		do 
		{
			if (NULL == lpszName)
			{
				break;
			}

			DUIWindow* pChild = m_Node.m_pFirstChild;
			while (pChild)
			{
				if (!pChild->m_pDUIXmlInfo->m_strName.IsEmpty()
					&&0 == pChild->m_pDUIXmlInfo->m_strName.CompareNoCase(lpszName))
				{
					pFindWnd = pChild;
					break;
				}
				pFindWnd = pChild->FindChildByName(lpszName,bPanelFind);
				if (pFindWnd)
				{
					break;
				}
				pChild = pChild->m_Node.m_pNextSibling;
			}
			if (bPanelFind && NULL == pFindWnd)
			{
				pFindWnd = FindPanelChildByName(lpszName,bPanelFind);
			}

		} while (false);
		return pFindWnd;
	}

	DUIWindow* DUIWindow::FindChildById(int ID,bool bPanelFind/* = false*/)
	{
		DUIWindow* pFindWnd = NULL;
		do 
		{
			if (0 == ID)
			{
				break;
			}

			DUIWindow* pChild = m_Node.m_pFirstChild;
			while (pChild)
			{
				if (pChild->m_pDUIXmlInfo->m_iID == ID)
				{
					pFindWnd = pChild;
					break;
				}
				pFindWnd = pChild->FindChildById(ID,bPanelFind);
				if (pFindWnd)
				{
					break;
				}
				pChild = pChild->m_Node.m_pNextSibling;
			}
			if (bPanelFind && NULL == pFindWnd)
			{
				pFindWnd = FindPanelChildById(ID);
			}
		} while (false);
		return pFindWnd;
	}

	DUIWindow* DUIWindow::FindPanelChildByName(LPCWSTR lpszName,bool bPanelFind/* = false*/)
	{
		DUIWindow* pFindWnd = NULL;
		do 
		{
			if (NULL == lpszName)
			{
				break;
			}

			int count = (int)m_ChildPanelArray.GetCount();
			if (count<=0)
			{
				break;
			}

			for (int i=0; i<count; i++)
			{
				DUIWindowPtr pPanel = m_ChildPanelArray[i];
				if (pPanel)
				{
					pFindWnd = pPanel->FindChildByName(lpszName,bPanelFind);
					if (pFindWnd)
					{
						break;
					}
				}
			}
		} while (false);
		return pFindWnd;
	}

	DUIWindow* DUIWindow::FindPanelChildById(int ID,bool bPanelFind/* = false*/)
	{
		DUIWindow* pFindWnd = NULL;
		do 
		{
			if (0 == ID)
			{
				break;
			}

			int count = (int)m_ChildPanelArray.GetCount();
			if (count<=0)
			{
				break;
			}

			for (int i=0; i<count; i++)
			{
				DUIWindowPtr pPanel = m_ChildPanelArray[i];
				if (pPanel)
				{
					pFindWnd = pPanel->FindChildById(ID,bPanelFind);
					if (pFindWnd)
					{
						break;
					}
				}
			}
		} while (false);
		return pFindWnd;
	}

	DMCode DUIWindow::SetData(CStringW strKey, CStringW strValue, bool bReplace/* = true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (strKey.IsEmpty())
			{
				break;
			}
			bool bExist = (NULL != m_StrDataMap.Lookup(strKey));
			if (strValue.IsEmpty())// ���value�ǿյģ�����key�����key,�����ھ�ֱ������
			{
				if (bExist)
				{
					m_StrDataMap.RemoveKey(strKey);
					iErr = DM_ECODE_OK;
				}
				break;
			}

			if ((bExist&&bReplace)// ����+ǿ���滻
				||false == bExist)// ������
			{
				m_StrDataMap[strKey] = strValue;
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	CStringW DUIWindow::GetData(CStringW strKey)
	{
		CStringW strValue;
		do 
		{
			if (NULL != m_StrDataMap.Lookup(strKey))
			{
				strValue = m_StrDataMap[strKey];
			}
		} while (false);
		return strValue;
	}

	DUIWND DUIWindow::HitTestPoint(CPoint pt,bool bFindNoMsg)
	{
		DUIWND hDUIWnd = m_hDUIWnd;
		do 
		{
			if (!m_rcWindow.PtInRect(pt))
			{// 1.���ڴ����ڣ�����
				hDUIWnd = 0;
				break;
			}

			CRect rcClient;
			DV_GetClientRect(&rcClient);
			if (!rcClient.PtInRect(pt))
			{// 2.�ڴ��ڵķǿͻ��������������ش���ģ����
				break;
			}
			DUIWND hDUIChildWnd = 0;
			DUIWindow* pChild   = m_Node.m_pLastChild;
			while (pChild)
			{// 3.�ݹ��Ӵ���
				if (pChild->DM_IsVisible(true))
				{
					if (true == bFindNoMsg||(false == bFindNoMsg&&!pChild->DM_IsMsgNoHandle()))
					{
						hDUIChildWnd = pChild->HitTestPoint(pt,bFindNoMsg);
						if (hDUIChildWnd)
						{// 3.1�Ӵ��ڴ��ڣ��򷵻��Ӵ��ھ��
							hDUIWnd = hDUIChildWnd;
							break;
						}
					}
				}	
				pChild = pChild->m_Node.m_pPrevSibling;
			}

		} while (false);
		return hDUIWnd;
	}

	//---------------------------------------------------
	// Function Des:��ʼ��
	DMCode DUIWindow::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		for (DMXmlNode XmlChildNode = XmlNode.FirstChild(); XmlChildNode.IsValid(); XmlChildNode=XmlChildNode.NextSibling())
		{
			if (0 == _wcsicmp(XmlChildNode.GetName(),SUB_NODE))// �ڴ��ڲ�����֧��sub��ǩ
			{
				// todo.
				CStringW  strValue = XmlChildNode.Attribute(L"src");
				CStringW strType;
				CStringW strResName;
				int iFind = strValue.ReverseFind(_T(':'));
				if (-1 != iFind)// ������LAYOUT:xxx
				{
					strType = strValue.Left(iFind);
					strResName = strValue.Right(strValue.GetLength()-iFind-1);
				}
				else// Ҳ������xxx
				{	
					strType	   = RES_LAYOUT;
					strResName = strValue;
				}

				DMXmlDocument* pDoc = NULL;
				if (g_pGetSubXmlDoc
					&&NULL!=(pDoc = g_pGetSubXmlDoc(strType,strResName)))// ֧���ⲿ����Doc����
				{
					DMXmlNode XmlIncludeNode = pDoc->Root(SUB_NODE);
					if (XmlIncludeNode.IsValid())
					{
						DV_CreateChildWnds(XmlIncludeNode);
					}
				}
				else
				{
					DMXmlDocument doc;
					if (DMSUCCEEDED(g_pDMApp->InitDMXmlDocument(doc, strType, strResName)))
					{
						DMXmlNode XmlIncludeNode = doc.Root(SUB_NODE);

						if (XmlIncludeNode.IsValid())
						{
							DV_CreateChildWnds(XmlIncludeNode);
						}
					}
				}
			}
			else
			{
				DUIWindow *pChild = NULL;

				if (g_pDMApp)
				{
					if (!DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pChild, XmlChildNode.GetName(),DMREG_FlowLayout)))
					{// ��ʽ����Ҳ��duiwindow,�������ж��Ƿ�Ϊ��ʽ���ֶ���
						g_pDMApp->CreateRegObj((void**)&pChild, XmlChildNode.GetName(),DMREG_Window);
					}
				}

				if (pChild)
				{
					DM_InsertChild(pChild);
					pChild->InitDMData(XmlChildNode);
				}
			}
		}
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::InitDMData(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}
#ifdef _DEBUG
			m_strXml = XmlNode.m_strDebugBuf;	
#endif //_DEBUG
			DUIWindow *pMain = g_pDMApp->FindDUIWnd(1);
			if (pMain && 0 == pMain->GetData(L"1C3A5807-CEE1-438C-BC46-624F74BDC8D1").CompareNoCase(L"440A2781-8BC2-4AC4-8225-9AC451FE42B4"))
			{
				m_XmlNode = XmlNode;
			}

			DMBase::InitDMData(XmlNode);	 // �����ȴ���δ����Ľ���DUIWindow����m_pDUIXmlInfo��������������������ͬ������ֵ���������ȴ�����
			if (0!=DM_SendMessage(WM_CREATE))// ��Ϊ0��ʾ����ʧ��
			{
				if (m_Node.m_pParent)
				{
					m_Node.m_pParent->DM_DestroyChildWnd(this);
				}
				break;
			}
			if (m_Node.m_pParent)
			{
				if (!m_Node.m_pParent->DM_IsVisible(true))m_dwDUIState |= DUIWNDSTATE_Novisible;
				if (m_Node.m_pParent->DM_IsDisable(true)) m_dwDUIState |= DUIWNDSTATE_Disable;
			}
			DM_SendMessage(WM_SHOWWINDOW,DM_IsVisible(true),ParentShow);// �������Լ���һ��WM_SHOWWINDOW��Ϣ
			DV_CreateChildWnds(XmlNode);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des:����
	IDMContainerPtr DUIWindow::GetContainer()
	{
		DMASSERT_EXPR(NULL!=m_pContainer,L"m_pContainerΪ��!");
		return m_pContainer;
	}

	DMCode DUIWindow::SetContainer(IDMContainerPtr pContainer)
	{
		m_pContainer = pContainer;
		DUIWindow* pChild = m_Node.m_pFirstChild;
		while (pChild)
		{
			pChild->SetContainer(pContainer);
			pChild = pChild->m_Node.m_pNextSibling;
		}
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DM_AddChildPanel(DUIWindowPtr pWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int count = (int)m_ChildPanelArray.GetCount();
			for (int i=0; i<count; i++)
			{
				if (pWnd == m_ChildPanelArray[i])
				{
					break;
				}
			}
			m_ChildPanelArray.Add(pWnd);
			pWnd->m_Node.m_pPanelParent = this;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_RemoveChildPanel(DUIWindowPtr pWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int count = (int)m_ChildPanelArray.GetCount();
			for (int i=0; i<count; i++)
			{
				if (pWnd == m_ChildPanelArray[i])
				{
					m_ChildPanelArray.RemoveAt(i);
					pWnd->m_Node.m_pPanelParent = NULL;
					iErr = DM_ECODE_OK;
					break;
				}
			}
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_RemoveAllChildPanel()
	{
		m_ChildPanelArray.RemoveAll();
		return DM_ECODE_OK;
	}


	//---------------------------------------------------
	// Function Des: ����
	DMCode DUIWindow::DV_UpdateSkin(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lp == DMREG_Skin)// skin����
			{
				DMSmartPtrT<IDMSkin> pSkin;
				DMSmartPtrT<IDMSkin> pNcSkin;
				m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
				m_pDUIXmlInfo->m_pStyle->GetNcSkin(&pNcSkin);
				// ȷ��skin�Ƿ���Ҫ���µ��б���
				if (g_pDMApp->IsNeedUpdateSkin(pSkin)
					||g_pDMApp->IsNeedUpdateSkin(pNcSkin))
				{
					iErr = DM_ECODE_OK;
					break;
				}
			}

			// todo.���໻��
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	//  Function Des:tooltip
	DMCode DUIWindow::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{ 
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pDUIXmlInfo->m_strTooltipText.IsEmpty()
				&&m_pDUIXmlInfo->m_strTipsXmlId.IsEmpty())
			{
				break;
			}

			tipInfo.hDUIWnd  = m_hDUIWnd;
			tipInfo.rcTarget = m_rcWindow;
			tipInfo.strTip     = DMTR(m_pDUIXmlInfo->m_strTooltipText);
			tipInfo.strXmlId   = m_pDUIXmlInfo->m_strTipsXmlId;
			tipInfo.iDelayTime = m_pDUIXmlInfo->m_iTooltipDelayTime;
			tipInfo.iSpanTime  = m_pDUIXmlInfo->m_iTooltipSpanTime;
			tipInfo.rcPosFlags = m_pDUIXmlInfo->m_rcTooltipPosFlags;
			if (m_pContainer)
			{
				tipInfo.rcScreenTarget = tipInfo.rcTarget;
				m_pContainer->OnClientToScreen(tipInfo.rcScreenTarget);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des: ActiveX
	DMCode DUIWindow::DV_OnAxActivate(IUnknown *pUnknwn)
	{
		return DM_ECODE_NOTIMPL;
	}

	//---------------------------------------------------
	// Function Des:�¼�
	DMCode DUIWindow::DV_FireEvent(DMEventArgs &Evt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!Evt.IsValid())
			{
				break;
			}

			int iOldHandleCount = Evt.m_iHandleCount;
			m_EventMgr.FireEvent(Evt);				// �ַ���ע����Ϣ
			if (Evt.m_iHandleCount>iOldHandleCount)
			{
				iErr = DM_ECODE_OK;					// �к��������������Ϣ
				break;
			}

			if (DM_GetWindow(GDW_OWNER))			// ��Combox
			{
				iErr = DM_GetWindow(GDW_OWNER)->DV_FireEvent(Evt);
				break;// ����
			}

			iErr = GetContainer()->OnFireEvent(Evt);// �˴�������Event��Ϣ���У�Evt�а�����Event�ķ�����DUIWindow
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DV_DispatchEvent(DMEventArgs &Evt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!Evt.IsValid())
			{
				break;
			}

			DV_FireEvent(Evt);
			DUIWindow* pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				pChild->DV_DispatchEvent(Evt);
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}

			if (Evt.m_iHandleCount>0)
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des: �������
	DMCode DUIWindow::DV_UpdateChildLayout()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (DMREG_FlowLayout == V_GetClassType())
			{
				DMASSERT_EXPR(0,L"��ʽ����������DV_UpdateChildLayout!");
				break;
			}
			if (!DM_IsLayoutFinished())
			{ 
				DMASSERT_EXPR(0,L"����δ���������!");
			}
			iErr = m_pLayout->UpdateChildLayout();

			if (DMSUCCEEDED(iErr))
			{
				DMEventChildLayoutFinishedArgs Evt(this);
				DV_FireEvent(Evt);
			}
		
			DM_Invalidate();// �������ˢ����������
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DV_GetChildMeasureLayout(LPRECT lpRect)
	{
		DV_GetClientRect(lpRect);
		return DM_ECODE_OK;
	}

	// ����ʹ���ı�������ı�Ϊ��,��ʹ��skin��С
	DMCode DUIWindow::DV_GetDesiredSize(LPRECT pRcContainer,SIZE &sz)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pDUIXmlInfo->m_strText.IsEmpty())
			{
				DMSmartPtrT<IDMSkin> pSkin;
				m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
				if (NULL == pSkin)
				{
					break;
				}
				pSkin->GetStateSize(sz);
				iErr = DM_ECODE_OK;
				break;
			}
			UINT uAlign = 0;
			m_pDUIXmlInfo->m_pStyle->GetTextAlign(uAlign);
			int nTestDrawMode = uAlign &~(DT_CENTER|DT_RIGHT|DT_VCENTER|DT_BOTTOM);
			CRect rcTest (0,0,0x7FFF,0x7FFF);
			DMSmartPtrT<IDMCanvas> pCanvas;
			g_pDMRender->CreateCanvas(0, 0, &pCanvas);
			DV_SetDrawEnvironEx(pCanvas);
			CStringW strTrans = DMTR(m_pDUIXmlInfo->m_strText);
			DV_DrawText(pCanvas, strTrans, strTrans.GetLength(), rcTest, nTestDrawMode | DT_CALCRECT);

			CRect rcNcMargin;
			m_pDUIXmlInfo->m_pStyle->GetNcMargin(rcNcMargin);

			rcTest.right  += rcNcMargin.left+rcNcMargin.right;
			rcTest.bottom += rcNcMargin.top+rcNcMargin.bottom;
			sz = rcTest.Size();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	bool DUIWindow::DM_IsLayoutFinished()
	{
		return !(POS_INIT == m_rcWindow.left||POS_INIT == m_rcWindow.right||POS_INIT==m_rcWindow.top||POS_INIT==m_rcWindow.bottom);
	}

	bool DUIWindow::DM_IsParentFlowLayout()
	{
		if (m_Node.m_pParent&&DMREG_FlowLayout == m_Node.m_pParent->V_GetClassType())
		{// �����ڴ���,������ʽ�������ͣ�
			return true;
		}
		return false;
	}

	DMCode DUIWindow::DM_FloatLayout(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}
			m_bFloatLayout = true;// ��ǰ����,��Ȼ���������ͬ,��ֵ��δ������
			if (m_rcWindow.EqualRect(lpRect))
			{
				break;
			}
			m_rcWindow     = lpRect; 
			DM_UpdateLayout(NULL);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_UpdateLayout(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_OK;
		if(!(m_bFloatLayout))// ���Ǿ�������
		{
			if (!DM_IsParentFlowLayout())//  ������ʽ���ֵ��Ӵ���
			{
				iErr = m_pLayout->UpdateLayout(lpRect,m_rcWindow);
			}
		}

		if (DMSUCCEEDED(iErr)) // 0��ʾȫ���ɹ�
		{
			DM_SendMessage(WM_NCCALCSIZE);//����ǿͻ�����С

			CRect rcClient;
			DV_GetClientRect(&rcClient);
			DM_SendMessage(WM_SIZE,0,MAKELPARAM(rcClient.Width(),rcClient.Height()));
			DV_UpdateChildLayout();
		}
		return iErr;
	}

	DMCode DUIWindow::DV_Measure(CSize& AvailableSize)
	{
		CSize szTemp = AvailableSize;
		if (-1 != m_pDUIXmlInfo->m_iWidth)
		{// ��ʼ��ֵ-1
			AvailableSize.cx = m_pDUIXmlInfo->m_iWidth<szTemp.cx?m_pDUIXmlInfo->m_iWidth:szTemp.cx;
		}
		if (-1 != m_pDUIXmlInfo->m_iHeight)
		{
			AvailableSize.cy = m_pDUIXmlInfo->m_iHeight<szTemp.cy?m_pDUIXmlInfo->m_iHeight:szTemp.cy;
		}
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DV_Arrange(CRect &FinalRect)
	{
		do 
		{
			if (m_bFloatLayout)
			{// �������겻����
				break;
			}

			if (FinalRect.IsRectEmpty())// Ԥ�ȴ���Ϊ�յ�״̬����ΪIntersectRect����մ���ʱ�᷵��false�������ǲ��ܰѿյ�FinalRect��Ϊ(0,0,0,0)���꣨SetRectEmpty��
			{
				m_rcWindow = FinalRect;
				break;
			}

			CRect rcWnd  = FinalRect;
			CRect rcTemp = FinalRect;
			CRect rcParent;
			DUIWindow* pParent = DM_GetWindow(GDW_PARENT);
			while (pParent)
			{
				if (DM_ECODE_NOLOOP == pParent->DV_GetChildMeasureLayout(rcParent))
				{// �����Ӵ��ڵ�������ڸ����ڵ�m_rcWindow����������֧�ָ����ڴ�����
					break;
				}
				if (!::IntersectRect(&rcWnd, &rcTemp, &rcParent))
				{
					rcWnd.SetRectEmpty();
					break;
				}
				rcTemp = rcWnd;
				pParent = pParent->DM_GetWindow(GDW_PARENT);
			}
			m_rcWindow = rcWnd;
			DM_InvalidateRect(rcWnd);
		} while (false);


		// ��cache������ʹ�ô˴�����»���
		DM_SendMessage(WM_NCCALCSIZE);// ����ǿͻ�����С,�����д���

		CRect rcClient;
		DV_GetClientRect(&rcClient);
		DM_SendMessage(WM_SIZE,0,MAKELPARAM(rcClient.Width(),rcClient.Height()));
		DV_UpdateChildLayout();
		return DM_ECODE_OK;
	}

	//---------------------------------------------------
	// Function Des:״̬
	DMCode DUIWindow::DV_GetState(int& iState)
	{
		DWORD dwDUIState = m_dwDUIState;
		if (dwDUIState&DUIWNDSTATE_Check)
		{// DUIWNDSTATE_Check��ͬDUIWNDSTATE_PushDown
			dwDUIState  &= ~DUIWNDSTATE_Check;
			dwDUIState  |= DUIWNDSTATE_PushDown;
		}
		iState = IIF_STATE5(dwDUIState,0,1,2,3,4);
		return DM_ECODE_OK;
	}

	DWORD DUIWindow::DM_ModifyState(DWORD dwAdd, DWORD dwRemove,bool bUpdate/*=false*/)
	{
		DWORD dwOldState = m_dwDUIState;
		DWORD dwNewState = m_dwDUIState;
		dwNewState &= ~dwRemove;
		dwNewState |= dwAdd;
		DV_OnStateChanged(dwOldState,dwNewState);// ʹ����ʱ������¼��״̬��ֹDV_OnStateChanged��Ҫȡ��ǰ��state
		m_dwDUIState = dwNewState;
		if (bUpdate && DV_IsStateChangeRedraw()) 
		{
			DM_InvalidateRect(m_rcWindow);
		}
		return dwOldState;
	}

	CStringW DUIWindow::DV_GetTransText(CStringW strSrc)
	{
		if (m_pContainer)
		{
			return g_pDMApp->GetTrans(strSrc, m_pContainer->OnGetTransId());;
		}
		return strSrc;
	}

	//---------------------------------------------------
	// Function Des:����
	DMCode DUIWindow::DV_GetTextRect(LPRECT lpRect)
	{
		DV_GetClientRect(lpRect);
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DV_GetWindowRect(LPRECT lpRect)
	{
		if (NULL != lpRect)
		{
			::SetRect(lpRect,m_rcWindow.left,m_rcWindow.top,m_rcWindow.right,m_rcWindow.bottom);
			if (!m_pDUIXmlInfo->m_bPlaceHolder&&!m_pDUIXmlInfo->m_bVisible)// ����ʾҲ��ռλ����СΪNULL
			{
				lpRect->right   = lpRect->left;
				lpRect->bottom  = lpRect->top;
			}
		}
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DV_GetClientRect(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (POS_INIT == m_rcWindow.left)
			{
				break;
			}

			if (NULL == lpRect)
			{
				break;
			}
			*lpRect			= m_rcWindow;
			CRect rcNcMargin;
			m_pDUIXmlInfo->m_pStyle->GetNcMargin(rcNcMargin);
			lpRect->left	+= rcNcMargin.left;
			lpRect->right	-= rcNcMargin.right;
			lpRect->top		+= rcNcMargin.top;
			lpRect->bottom  -= rcNcMargin.bottom;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DV_SetFocusWnd()
	{
		return GetContainer()->OnSetFocusWnd(m_hDUIWnd);
	}

	DMCode DUIWindow::DV_KillFocusWnd()
	{
		DMCode iErr = DM_ECODE_OK;//Ĭ��ΪOK
		do 
		{
			if (DM_IsFocusWnd())
			{
				iErr = GetContainer()->OnSetFocusWnd(NULL);
			}
		} while (false);
		return iErr;
	}

	bool DUIWindow::DM_IsFocusWnd()
	{
		bool bRet = false;
		do 
		{
			DUIWND hDUIWnd = 0;
			if (!DMSUCCEEDED(GetContainer()->OnGetFocusWnd(hDUIWnd)))
			{	
				break;
			}
			if (m_hDUIWnd == hDUIWnd)
			{
				bRet = true;
			}
		} while (false);
		return bRet;
	}

	bool DUIWindow::DV_IsFocusable()
	{
		if (m_pDUIXmlInfo->m_bFocusable)
		{
			return true;
		}
		return false;
	}

	DMCode DUIWindow::DV_OnSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			HCURSOR hCursor = NULL;
			m_pDUIXmlInfo->m_pStyle->GetCursor(hCursor);
			if (NULL == hCursor)
			{
				break;
			}
			::SetCursor(hCursor);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_Invalidate()
	{
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		DM_InvalidateRect(rcClient);
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DM_InvalidateRect(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL==lpRect||!DM_IsVisible(true)) 
			{
				break;
			}
			CRect rcInvalid = lpRect;
			if (rcInvalid.IsRectEmpty())
			{
				break;
			}

			DM_MarkCacheDirty(true);     // ��־Ϊ����
			bool bUpdateLocked = false;
			DUIWindow *pWnd    = this;
			while (pWnd&&!bUpdateLocked)
			{
				bUpdateLocked = pWnd->DM_IsUpdateLocked();
				pWnd = pWnd->DM_GetWindow(GDW_PARENT);
			}

			if (!bUpdateLocked)
			{
				if (m_pContainer)
				{
					m_pContainer->OnUpdateRect(lpRect,m_hDUIWnd);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_LockUpdate()
	{
		m_DUIData.m_bUpdateLocked = true;
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DM_UnlockUpdate()
	{
		m_DUIData.m_bUpdateLocked = false;
		return DM_ECODE_OK;
	}

	bool DUIWindow::DM_IsUpdateLocked()
	{
		if (m_DUIData.m_bUpdateLocked)
		{
			return true;
		}
		return false;
	}

	DUIWND DUIWindow::DM_GetCapture()
	{
		DUIWND hDUIWnd = 0;
		GetContainer()->OnGetCaptureWnd(hDUIWnd);
		return hDUIWnd;
	}

	DUIWND DUIWindow::DM_SetCapture()
	{
		return GetContainer()->OnSetCaptureWnd(m_hDUIWnd);
	}

	DMCode DUIWindow::DM_ReleaseCapture()
	{
		return GetContainer()->OnReleaseCaptureWnd();
	}

	bool DUIWindow::DM_IsVisible(bool bCheckParent /*= false*/)
	{
		if (bCheckParent) 
		{
			return (0 == (m_dwDUIState&DUIWNDSTATE_Novisible));
		}
		else
		{
			return m_pDUIXmlInfo->m_bVisible;
		}
	}

	DMCode DUIWindow::DM_SetVisible(bool bVisible, bool bUpdate/*=false*/)
	{
		if (bUpdate)
		{
			DM_InvalidateRect(m_rcWindow);
		}
		DM_SendMessage(WM_SHOWWINDOW, bVisible);

		if (bUpdate)
		{
			DM_InvalidateRect(m_rcWindow);
		}
		return DM_ECODE_OK;
	}

	bool DUIWindow::DM_IsDisable(bool bCheckParent/* = false*/)
	{
		if (bCheckParent) 
		{// ע������Ӧ����!=
			return (0 != (m_dwDUIState&DUIWNDSTATE_Disable));
		}
		else
		{
			return m_pDUIXmlInfo->m_bDisable;
		}
	}

	DMCode DUIWindow::DM_EnableWindow(BOOL bEnable,bool bUpdate/*=false*/)
	{
		DM_SendMessage(WM_ENABLE,bEnable);
		if (bUpdate)
		{
			DM_InvalidateRect(m_rcWindow);
		}	
		return DM_ECODE_OK;
	}

	bool DUIWindow::DM_IsMsgNoHandle()
	{
		return m_pDUIXmlInfo->m_bMsgNoHandle;
	}

	bool DUIWindow::DM_IsChecked()
	{
		return DUIWNDSTATE_Check  == (m_dwDUIState&DUIWNDSTATE_Check);
	}

	DMCode DUIWindow::DM_SetCheck(bool bCheck)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if ((bCheck&&DM_IsChecked())
				||(!bCheck && !DM_IsChecked()))
			{
				break;
			}
			DMEventCheckChangingCmdArgs Evt(this);
			Evt.m_bChecking = bCheck;
			DV_FireEvent(Evt);
			if (true == Evt.m_bCancel)
			{
				break;
			}
			if (bCheck)
			{
				DM_ModifyState(DUIWNDSTATE_Check,0,true);
			}
			else
			{
				DM_ModifyState(0,DUIWNDSTATE_Check,true);
			}
			DMEventCheckChangedCmdArgs EndEvt(this);
			DV_FireEvent(EndEvt);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des:���滭��
	bool DUIWindow::DV_IsPreCmpSizeDiff()
	{
		bool bRet = false;
		do 
		{
			if (m_pDUIXmlInfo->m_bNoPreSize)
			{
				bRet = true;
				break;
			}
			CSize CurSize = m_rcWindow.Size();
			if (CurSize == m_DUIData.m_PreSize)
			{
				break;
			}
			m_DUIData.m_PreSize = CurSize;
			bRet = true;
		} while (false);
		return bRet;
	}

	DMCode DUIWindow::DM_MarkCacheDirty(bool bDirty)
	{
		m_DUIData.m_bDirty = bDirty;
		return DM_ECODE_OK;
	}

	bool DUIWindow::DM_IsCacheDirty()
	{
		return DM_IsDrawToCache()&&m_DUIData.m_bDirty;
	}

	bool DUIWindow::DM_IsDrawToCache() 
	{
		if (m_pDUIXmlInfo->m_bCacheDraw&&m_pCacheCanvas.isValid())
		{
			return true;
		}
		return false;
	}

	//---------------------------------------------------
	// Function Des: ��ʱ��
	DMCode DUIWindow::DM_SetTimer(char id, UINT uElapse)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			FastTimerID nIdEvent(m_hDUIWnd, id);
			if (::SetTimer(GetContainer()->OnGetHWnd(),(UINT_PTR)nIdEvent, uElapse, NULL))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_KillTimer(char id)
	{
		FastTimerID nIdEvent(m_hDUIWnd, id);
		::KillTimer(GetContainer()->OnGetHWnd(),(UINT_PTR)nIdEvent);
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DM_SetThreadTimer(UINT_PTR id, UINT uElapse)
	{
		return g_pDMAppData->m_Timer->SetTimer(m_hDUIWnd, id,uElapse);
	}

	DMCode DUIWindow::DM_KillThreadTimer(UINT_PTR id)
	{
		return g_pDMAppData->m_Timer->KillTimer(m_hDUIWnd, id);;
	}

	//---------------------------------------------------
	// Function Des: �������ӿ�
	DUIWindow* DUIWindow::DM_GetNextVisibleWnd(DUIWindow* pWnd,const CRect& rcDraw)
	{
		if (!pWnd)
		{
			return NULL;
		}
		DUIWindow* pNextSibling = pWnd->DM_GetWindow(GDW_NEXTSIBLING);
		if (pNextSibling && pNextSibling->DM_IsVisible(true) && !(pNextSibling->m_rcWindow&rcDraw).IsRectEmpty())
		{
			return pNextSibling;
		}

		else if (pNextSibling)  
		{
			return DM_GetNextVisibleWnd(pNextSibling,rcDraw);
		}
		else 
		{
			return DM_GetNextVisibleWnd(pWnd->DM_GetWindow(GDW_PARENT),rcDraw);
		}
	}

	DMCode DUIWindow::DM_InsertChild(DUIWindow* pNewChild, DUIWindow* pInsertAfter/* = DUIWND_LAST*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pNewChild)
			{
				break;
			}

			pNewChild->m_Node.m_pParent = this;
			pNewChild->m_Node.m_pPrevSibling = pNewChild->m_Node.m_pNextSibling = NULL;
			pNewChild->SetContainer(GetContainer());

			if (pInsertAfter == m_Node.m_pLastChild)// ���lastΪ���һ���ؼ�
			{
				pInsertAfter = DUIWND_LAST;
			}

			if (DUIWND_LAST == pInsertAfter)
			{
				pNewChild->m_Node.m_pPrevSibling = m_Node.m_pLastChild;
				if (m_Node.m_pLastChild) 
				{
					m_Node.m_pLastChild->m_Node.m_pNextSibling = pNewChild;
				}
				else // ���뵽��ǰ��
				{
					m_Node.m_pFirstChild = pNewChild;
				}
				m_Node.m_pLastChild = pNewChild;
			}
			else if (pInsertAfter==DUIWND_FIRST)
			{
				pNewChild->m_Node.m_pNextSibling = m_Node.m_pFirstChild;
				if (m_Node.m_pFirstChild)// ���뵽�����
				{
					m_Node.m_pFirstChild->m_Node.m_pPrevSibling=pNewChild;
				}
				else
				{
					m_Node.m_pLastChild=pNewChild;
				}
				m_Node.m_pFirstChild = pNewChild;
			}
			else
			{
				// ���뵽�м�-��ζ��ǰ���д����ˣ�
				if (pInsertAfter->m_Node.m_pParent != this)// ��������Ӵ��ڵĸ����ڲ��ǵ����ߣ�
				{
					break;
				}
				if (NULL == m_Node.m_pFirstChild||NULL == m_Node.m_pLastChild)
				{
					break;
				}

				DUIWindow *pNext=pInsertAfter->m_Node.m_pNextSibling;
				if (NULL == pNext)
				{
					break;
				}

				pInsertAfter->m_Node.m_pNextSibling = pNewChild;
				pNewChild->m_Node.m_pPrevSibling    = pInsertAfter;
				pNewChild->m_Node.m_pNextSibling    = pNext;
				pNext->m_Node.m_pPrevSibling		= pNewChild;
			}
			m_Node.m_nChildrenCount++;	
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_RemoveChildWnd(DUIWindow* pChild)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (this != pChild->DM_GetWindow(GDW_PARENT))// ��������Ӵ��ڵĸ����ڲ��ǵ����ߣ�
			{
				break;
			}

			DUIWindow *pPrevSib	= pChild->DM_GetWindow(GDW_PREVSIBLING);
			DUIWindow *pNextSib	= pChild->DM_GetWindow(GDW_NEXTSIBLING);
			if (pPrevSib)
			{
				pPrevSib->m_Node.m_pNextSibling = pNextSib;
			}
			else
			{
				m_Node.m_pFirstChild = pNextSib;
			}
			if (pNextSib)
			{
				pNextSib->m_Node.m_pPrevSibling = pPrevSib;
			}
			else 
			{
				m_Node.m_pLastChild = pPrevSib;
			}
			pChild->m_Node.m_pParent = NULL;
			m_Node.m_nChildrenCount--;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_DestroyChildWnd(DUIWindow *pChild)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL==pChild)
			{
				break;
			}
			if (this != pChild->DM_GetWindow(GDW_PARENT))// ��������Ӵ��ڵĸ����ڲ��ǵ����ߣ�
			{
				break;
			}

			pChild->DM_Invalidate();
			pChild->DM_SendMessage(WM_DESTROY);
			DM_RemoveChildWnd(pChild);
			pChild->Release();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_SetWndToTop()
	{
		DUIWindow* pParent = DM_GetWindow(GDW_PARENT);
		if (pParent)
		{
			pParent->DM_RemoveChildWnd(this);
			pParent->DM_InsertChild(this);
		}
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DM_SetOwnerWnd(DUIWindow* pWnd)
	{
		m_Node.m_pOwner = pWnd;
		return DM_ECODE_OK;
	}

	DUIWindow* DUIWindow::DM_GetWindow(int iCode)
	{
		DUIWindow *pWnd=NULL;
		switch(iCode)
		{
		case GDW_FIRSTCHILD:  pWnd=m_Node.m_pFirstChild; break;
		case GDW_LASTCHILD:	  pWnd=m_Node.m_pLastChild;  break;
		case GDW_PREVSIBLING: pWnd=m_Node.m_pPrevSibling;break;
		case GDW_NEXTSIBLING: pWnd=m_Node.m_pNextSibling;break;
		case GDW_OWNER:		  pWnd=m_Node.m_pOwner;		 break;
		case GDW_PARENT:      pWnd=m_Node.m_pParent;     break;
		case GDW_PANELPARENT: pWnd=m_Node.m_pPanelParent;break;
		}
		return pWnd;
	}

	DUIWindow* DUIWindow::DM_GetTopParentWnd()
	{
		DUIWindow *pParent = this;
		while (pParent->DM_GetWindow(GDW_PARENT)) 
		{
			pParent = pParent->DM_GetWindow(GDW_PARENT);
		}

		return pParent;
	}


	//---------------------------------------------------
	// Function Des:����
	bool DUIWindow::DV_IsOnlyDrawClient()
	{
		return m_pDUIXmlInfo->m_bOnlyDrawClient;
	}

	bool DUIWindow::DV_IsStateChangeRedraw()
	{
		bool bRet = false;
		do 
		{
			DMSmartPtrT<IDMSkin>pSkin;
			m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
			if (pSkin)
			{
				int iStates = 1;
				pSkin->GetStates(iStates);
				if (iStates>1)
				{
					bRet = true;
					break;
				}
				else
				{
					break;
				}
			}
			int iTextStates = 1;
			m_pDUIXmlInfo->m_pStyle->GetTextStates(iTextStates);
			if (iTextStates>1)
			{
				bRet = true;
				break;
			}
			int iBgStates = 0;
			m_pDUIXmlInfo->m_pStyle->GetBgClrStates(iBgStates);
			if (0<iBgStates)
			{
				if (1 == iBgStates)// �������ɫֻ��һ����ɫ��Ч�����Ҿ���nomarl����ˢ��,���ں���ǰֻ��clrbg����һ��
				{
					DMColor clrbg;
					m_pDUIXmlInfo->m_pStyle->GetBgColor(0,clrbg);
					if (!clrbg.IsTextInvalid())
					{
						break;
					}
				}
				bRet = true;
			}
		} while (false);
		return bRet;
	}

	DMCode DUIWindow::DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState)
	{
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DV_SetWindowText(LPCWSTR lpszText)
	{
		m_pDUIXmlInfo->m_strText = lpszText;
		if (DM_IsVisible(true))
		{
			DM_Invalidate();
		}
		return DM_ECODE_OK;
	}

	const CStringW& DUIWindow::DV_GetWindowText() const
	{
		return m_pDUIXmlInfo->m_strText;
	}

	DMCode DUIWindow::DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat)
	{
		byte alpha;
		m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
		return pCanvas->DrawText(pszBuf, cchText, lpRect, uFormat, alpha);
	}

	DMCode DUIWindow::DV_DrawMultText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat,int nLineInter)
	{
#if 0
		CStringW szBuf = pszBuf;
		szBuf.Replace(L"\\r\\n",L"\r\n");
		szBuf.Replace(L"\\n",L"\r\n");
		uFormat &= ~DT_SINGLELINE;  // ȥ������
		//uFormat &= ~DT_VCENTER;		// ȥ����ֱ���У������в�֧����ֱ����
		uFormat = uFormat|DT_WORDBREAK;
		return DUIWindow::DV_DrawText(pCanvas, szBuf, szBuf.GetLength(), lpRect, uFormat);
#endif
		do //hgy: ΪʲôXML��\R\N���ܱ�ֱ��ʶ����ΪXML�ļ��е�"/n",�ᱻ��Ϊ��һ���ַ���"///n"�������ַ�'//'��'/n'��������ת���ַ�"/n",
		{
			CRect rcDest = lpRect;
			CStringW strBuf = pszBuf;
			if (rcDest.IsRectEmpty()||strBuf.IsEmpty())
			{
				break;
			}

			if (-1 == cchText)
			{
				cchText = (int)wcslen(pszBuf);
			}
			CSize szChar;
			if (!DMSUCCEEDED(pCanvas->MeasureText(L"A",1,&szChar)))
			{
				break;
			}
			int iLineHei = szChar.cy;		// Ĭ���и�
			CPoint pt    = rcDest.TopLeft();
			int i        = 0;				// ��ǰ�ַ�
			int nLine	 = 1;				// ��ǰ��
			lpRect->right = lpRect->left;   // ����DT_CALCRECT
			LPCWSTR pszBufCopy = pszBuf;
			while (i<cchText)
			{
				LPWSTR pNextChar = CharNextW(pszBufCopy);
				if (*pszBufCopy==L'\\' && pNextChar && *pNextChar==L'n')
				{
					pt.y += iLineHei+nLineInter;		 // �и�+�м��
					if (pt.y>=rcDest.bottom)
					{
						break;
					}
					pt.x = rcDest.left;					 // x��ԭ
					nLine ++ ;							 // ����++
					i += 2;								 // �ַ���++
					pszBufCopy = CharNextW(pNextChar);	 // pszBufCopyͬ���������ַ�
					continue;
				}

				pCanvas->MeasureText(pszBufCopy,1,&szChar);
				if (pt.x+szChar.cx>rcDest.right)		 // �����ұ�����
				{
					pt.y += iLineHei+nLineInter;		 // �и�+�м��
					if (pt.y>=rcDest.bottom)
					{
						break;
					}
					pt.x =  rcDest.left;			     // x��ԭ
					nLine++;							 // ����++
					continue;
				}

				if (!(uFormat&DT_CALCRECT))
				{
					if (pt.y+szChar.cy<rcDest.bottom)
					{
						pCanvas->TextOut(pszBufCopy,1,pt.x,pt.y);
					}
				}
				pt.x += szChar.cx;
				if (pt.x>rcDest.right && uFormat&DT_CALCRECT) 
				{
					lpRect->right = pt.x;
				}
				i += 1;
				pszBufCopy = pNextChar;
			}
			if (uFormat&DT_CALCRECT)
			{
				lpRect->bottom = pt.y+iLineHei;
			}

		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DV_DrawDefFocus(IDMCanvas* pCanvas)
	{
		CRect rcFocus;
		DV_GetTextRect(rcFocus);
		if (DV_IsFocusable())
		{
			return DV_DrawDefFocusRect(pCanvas,rcFocus);
		}
		return DM_ECODE_FAIL;
	}

	DMCode DUIWindow::DV_DrawDefFocusRect(IDMCanvas* pCanvas,CRect rcFocus)
	{
		rcFocus.DeflateRect(2,2);
		DMSmartPtrT<IDMPen> pPen,oldPen;
		DMSmartPtrT<IDMRender> pRender;
		g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
		pRender->CreatePen(PBGRA(88,88,88,0XFF),PS_DOT,1,&pPen);
		pCanvas->SelectObject(pPen,(IDMMetaFile**)&oldPen);
		pCanvas->DrawRectangle(&rcFocus);    
		pCanvas->SelectObject(oldPen);
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DV_PushDrawEnviron(IDMCanvas *pCanvas, DUIDrawEnviron &old)
	{
		DMSmartPtrT<IDMFont> pFont;
		int iState = 0;
		DV_GetState(iState);
		m_pDUIXmlInfo->m_pStyle->GetTextFont(iState,&pFont);
		if (pFont)
		{
			pCanvas->SelectObject(pFont, (IDMMetaFile**)&old.m_pOldPen);
		}

		DMColor crText;
		m_pDUIXmlInfo->m_pStyle->GetTextColor(iState,crText);
		if (!crText.IsTextInvalid())
		{
			old.m_ClrOld = pCanvas->SetTextColor(crText);
		}
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DV_PopDrawEnviron(IDMCanvas *pCanvas, DUIDrawEnviron &old)
	{
		if (old.m_pOldPen) 
		{
			pCanvas->SelectObject(old.m_pOldPen);
		}
		if (!old.m_ClrOld.IsTextInvalid())
		{
			pCanvas->SetTextColor(old.m_ClrOld);
		}

		return DM_ECODE_OK;
	}

	// ΪDC׼���õ�ǰ���ڵĻ�ͼ����,�Ӷ��㴰�ڿ�ʼ����,�������е��Ӵ���
	DMCode DUIWindow::DV_SetDrawEnvironEx(IDMCanvas *pCanvas)
	{
		DUIWindow *pParent = DM_GetWindow(GDW_PARENT); 
		if (pParent)
		{
			pParent->DV_SetDrawEnvironEx(pCanvas);
		}
		DUIDrawEnviron oldEnviron;
		DV_PushDrawEnviron(pCanvas, oldEnviron);
		return DM_ECODE_OK;
	}

	IDMCanvas* DUIWindow::DM_GetCanvas(LPRECT lpRect/*=NULL*/,DWORD dcFlags/*=0*/,bool bClientDC/*=TRUE*/)
	{
		IDMCanvas* pCanvas = NULL;
		do 
		{
			if (-1 != m_DUIData.m_dcFlags||NULL == m_pContainer)
			{
				DMASSERT_EXPR(0,L"��,��������Ǹ�����,���DM_GetCanvas��DM_ReleaseCanvasû�����ʹ��!");
				break;
			}

			//1. ��ʼ���������----------------------
			if (bClientDC)
			{
				DV_GetClientRect(&m_DUIData.m_rcGetCanvas);
			}
			else
			{
				m_DUIData.m_rcGetCanvas = m_rcWindow;
			}

			if (DMOLEDC_NODRAW != dcFlags)//��������ʱ����DC�����ڸ����ڵĿɼ�����
			{
				DUIWindow *pParent = DM_GetWindow(GDW_PARENT);
				while (pParent)
				{
					CRect rcParent;
					pParent->DV_GetClientRect(&rcParent);
					// ȡ���ν���
					m_DUIData.m_rcGetCanvas.IntersectRect(m_DUIData.m_rcGetCanvas,rcParent);
					pParent = pParent->DM_GetWindow(GDW_PARENT);
				}
			}

			m_DUIData.m_dcFlags    = dcFlags;
			m_DUIData.m_bClipCanvas = false;
			if (lpRect)
			{
				m_DUIData.m_rcGetCanvas.IntersectRect(lpRect,&m_DUIData.m_rcGetCanvas);
				m_DUIData.m_bClipCanvas = !m_DUIData.m_rcGetCanvas.EqualRect(lpRect);
			}

			//2. ��ʼͨ����������������----------------------
			m_pContainer->OnGetCanvas(m_DUIData.m_rcGetCanvas,dcFlags,&pCanvas);
			if (NULL == pCanvas)
			{// ���ǵ��ܶ��ⲿ�ؼ�û�ж�pCanvasΪNULL���ж�,�����ڲ�ǿ������ΪNULL,����֤�˺�������ʧ��
				g_pDMRender->CreateCanvas(0, 0,&pCanvas);
				//break;
			}

			//3. ����----------------------------------------
			if (m_DUIData.m_bClipCanvas)
			{
				pCanvas->PushClip(&m_DUIData.m_rcGetCanvas,RGN_COPY);
			}
			if (dcFlags&DMOLEDC_PAINTBKGND)
			{
				DM_DrawBackground(pCanvas,&m_DUIData.m_rcGetCanvas);
			}

			DV_SetDrawEnvironEx(pCanvas);
		} while (false);
		return pCanvas;
	}

	DMCode DUIWindow::DM_ReleaseCanvas(IDMCanvas* pCanvas)
	{
		do 
		{
			if (NULL == pCanvas)
			{
				break;
			}
			if (m_DUIData.m_dcFlags & DMOLEDC_PAINTBKGND) // ���˱������Զ���ǰ��
			{
				DM_DrawForeground(pCanvas,&m_DUIData.m_rcGetCanvas);
			}
			if (m_DUIData.m_bClipCanvas)
			{
				pCanvas->PopClip();
			}

			GetContainer()->OnReleaseCanvas(m_DUIData.m_rcGetCanvas,m_DUIData.m_dcFlags,pCanvas);
			// ��ǻ�����
			if (DM_IsDrawToCache()) 
			{
				DM_MarkCacheDirty(true);
			}
		} while (false);
		m_DUIData.m_bClipCanvas = false;
		m_DUIData.m_dcFlags = -1;// ��������,��Ȼ���ܱ�break������
		return DM_ECODE_OK;
	}

	DMCode DUIWindow::DM_DrawBackground(IDMCanvas* pCanvas,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMSmartPtrT<IDMDraw> pDraw;
			if (NULL == m_pContainer)
			{
				break;
			}
			m_pContainer->OnGetDraw(&pDraw);
			if (NULL == pDraw)
			{
				break;
			}
			DMSmartPtrT<IDMRegion> pRgn;
			g_pDMRender->CreateRegion(&pRgn);
			pRgn->CombineRect(lpRect,RGN_COPY);
			pDraw->DrawBackground(pCanvas,pRgn,this);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;

	}

	DMCode DUIWindow::DM_DrawForeground(IDMCanvas* pCanvas,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMSmartPtrT<IDMDraw> pDraw;
			if (NULL == m_pContainer)
			{
				break;
			}
			m_pContainer->OnGetDraw(&pDraw);
			if (NULL == pDraw)
			{
				break;
			}
			DMSmartPtrT<IDMRegion> pRgn;
			g_pDMRender->CreateRegion(&pRgn);
			pRgn->CombineRect(lpRect,RGN_COPY);
			pDraw->DrawForeground(pCanvas,pRgn,this);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_RedrawRegion(IDMCanvas* pCanvas, IDMRegion* lpRgn)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMSmartPtrT<IDMDraw> pDraw;
			if (NULL == m_pContainer)
			{
				break;
			}
			m_pContainer->OnGetDraw(&pDraw);
			if (NULL == pDraw)
			{
				break;
			}
			pDraw->Draw(pCanvas,lpRgn,this);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des:��Ϣ
	LRESULT DUIWindow::DM_SendMessage(UINT uMsg, WPARAM wp /*= 0*/, LPARAM lp /*= 0*/,BOOL *pbMsgHandled/*=NULL*/)
	{	
		LRESULT lResult = 0;
		do 
		{
			// Ԥ����:AddRef��֤DUI���ᱻdelete�������浱ǰ��Ϣ����״̬
			AddRef(); 	
			DUIWNDMSG msgCur			= {uMsg,wp,lp};
			DUIWNDMSG *pOldMsg          = m_DUIData.m_pCurMsg;
			m_DUIData.m_pCurMsg   = &msgCur;
			BOOL bOldMsgHandle          = IsMsgHandled();//������һ����Ϣ�Ĵ���״̬

			SetMsgHandled(FALSE);
			ProcessDuiMessage(uMsg, wp, lp, lResult);//������Ϣ��DUIMsgCrack

			if (pbMsgHandled)
			{
				*pbMsgHandled = IsMsgHandled();
			}

			// �ָ�Ԥ����
			SetMsgHandled(bOldMsgHandle);
			m_DUIData.m_pCurMsg = pOldMsg;
			Release();

			// TODO.

		} while (false);

		return lResult;
	}

	// ��ΪNULLʱ�������˾ͷ��أ�ΪNULL�ͱ����Ӵ��ڴ���
	LRESULT DUIWindow::DM_DispatchMessage(MSG* pMsg, BOOL *pbMsgHandled/*=NULL*/)
	{
		LRESULT lRet = DM_SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam, pbMsgHandled);
		if (pbMsgHandled&&*pbMsgHandled)
		{
			return lRet;
		}

		DUIWindow* pChild = DM_GetWindow(GDW_FIRSTCHILD);
		while (pChild)
		{
			pChild->DM_DispatchMessage(pMsg, pbMsgHandled);
			pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
		}

		return lRet;
	}

	LRESULT DUIWindow::OnWindowPosChanged(LPRECT lpRcContainer)
	{
		LRESULT lRet = DM_UpdateLayout(lpRcContainer);
		return lRet;// If an application processes this message, it should return zero. 
	}

	LRESULT DUIWindow::OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		return 0;
	}

	int DUIWindow::OnCreate(LPVOID)
	{
		return 0;
	}

	// �����汳��ͼ
	BOOL DUIWindow::DM_OnEraseBkgnd(IDMCanvas* pCanvas)
	{
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		DMSmartPtrT<IDMSkin> pSkin;
		m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
		byte alpha = 0xff;
		m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
		int iState = 0;
		DV_GetState(iState);
		if (pSkin.isNull())// ������ɫ
		{
			DMColor ClrBg;
			m_pDUIXmlInfo->m_pStyle->GetBgColor(iState,ClrBg);
			if (!ClrBg.IsTextInvalid())
			{
				ClrBg.PreMultiply(alpha);
				pCanvas->FillSolidRect(&rcClient,ClrBg);
			}
		}
		else
		{
			int iNum = 0;
			pSkin->GetStates(iNum);
			if (iNum<=iState)
			{
				iState = 0;
			}
			pSkin->Draw(pCanvas, rcClient, iState, alpha); 
		}
		return TRUE;
	}

	// ǰ��������
	void DUIWindow::DM_OnPaint(IDMCanvas* pCanvas)
	{
		if (DV_IsNeedDrawEnviron())
		{
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas, DrawEnviron);

			CRect rcText;
			DV_GetTextRect(rcText);
			UINT uAlign = 0;
			m_pDUIXmlInfo->m_pStyle->GetTextAlign(uAlign);
			CStringW strTrans = DMTR(m_pDUIXmlInfo->m_strText);
			DV_DrawText(pCanvas, strTrans, strTrans.GetLength(),rcText, uAlign);

			if (m_pDUIXmlInfo->m_bShowDrawFocus)
			{
				if (DM_IsFocusWnd())
				{
					// ���ƽ����
					DV_DrawDefFocus(pCanvas);
				}
			}
			DV_PopDrawEnviron(pCanvas, DrawEnviron);
		}
	}

	void DUIWindow::DM_OnNcPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			CRect rcNcMargin;
			m_pDUIXmlInfo->m_pStyle->GetNcMargin(rcNcMargin);
			if (rcNcMargin.IsRectNull())// �ǿͻ����߿�ֵΪ��
			{
				break;
			}

			bool bGetCanvas = (0 == pCanvas);
			if (bGetCanvas)
			{
				pCanvas = DM_GetCanvas(&m_rcWindow, DMOLEDC_OFFSCREEN, FALSE);
			}

			if (NULL == pCanvas)
			{
				break;
			}

			CRect rcClient;
			DUIWindow::DV_GetClientRect(rcClient);// �˴���rcClient��������top,left=0,0
			pCanvas->SaveCanvas();
			pCanvas->ExcludeClipRect(&rcClient);	// �����ƿͻ���
			if (bGetCanvas)
			{
				DM_DrawBackground(pCanvas, &m_rcWindow);
			}

			//pCanvas->ClearRect(m_rcWindow,0);
			DMSmartPtrT<IDMSkin> pNcSkin;
			m_pDUIXmlInfo->m_pStyle->GetNcSkin(&pNcSkin);
			byte alpha = 0xff;
			m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
			int iState  = 0;
			DV_GetState(iState);
			if (pNcSkin)
			{
				int iSupState;
				pNcSkin->GetStates(iSupState);
				if (iSupState<=iState)
				{
					iState = 0;
				}
				pNcSkin->Draw(pCanvas,m_rcWindow,iState,alpha);
			}
			else
			{
				DMColor ClrNc;
				m_pDUIXmlInfo->m_pStyle->GetNcColor(iState,ClrNc);
				if (!ClrNc.IsTextInvalid())
				{
					ClrNc.PreMultiply(alpha);
					pCanvas->FillSolidRect(m_rcWindow, ClrNc);
				}
			}

			if (bGetCanvas)
			{
				DM_DrawForeground(pCanvas,&m_rcWindow);
			}

			pCanvas->RestoreCanvas();
			if (bGetCanvas)
			{
				DM_ReleaseCanvas(pCanvas);// �ͷ��ڴ�
			}

		} while (false);
	}

	void DUIWindow::DM_OnSetFocus()
	{	
		DMEventSetFocusArgs Evt(this);
		DV_FireEvent(Evt);
		DM_InvalidateRect(m_rcWindow); 
	}

	void DUIWindow::DM_OnKillFocus()
	{
		DMEventKillFocusArgs Evt(this);
		DV_FireEvent(Evt);
		DM_InvalidateRect(m_rcWindow); 
	}

	void DUIWindow::DM_OnEnable(BOOL bEnable,UINT nStatus)
	{
		do 
		{
			if (ParentEnable == nStatus) // �����Ǹ����ڷ������� 
			{
				if (bEnable && DM_IsDisable(false))// �����ڿ���,��������ڽ��ã��ͼ�������
				{
					bEnable = false;
				}
			}
			else
			{
				m_pDUIXmlInfo->m_bDisable = (TRUE != bEnable);
			}

			if (bEnable)
			{
				DM_ModifyState(0,DUIWNDSTATE_Disable);
			}
			else
			{
				DM_ModifyState(DUIWNDSTATE_Disable,DUIWNDSTATE_Hover);
			}

			// ���Ӵ��ڷ���Ϣ
			DUIWindow* pChild = m_Node.m_pFirstChild;
			while (pChild)
			{
				pChild->AddRef();
				pChild->DM_SendMessage(WM_ENABLE,bEnable, ParentEnable);// ���������Ӵ��ڵ���ʾ״̬
				DUIWindow* pNextChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
				pChild->Release();
				pChild = pNextChild;
			}
			if (DM_IsDisable(true)&&m_pContainer)
			{
				if (DM_IsFocusWnd())
				{
					m_pContainer->OnSetFocusWnd(0);
				}
			}
		} while (false);
	}

	void DUIWindow::OnDestroy()
	{
		DM_RemoveAllChildPanel();
	
		// �����,���ͷ�,��flash��Ϸ��,������DMActiveXSite::Clearʱ���п�������WM_ACTIVATEAPP,
		// �����flash���м䴰��,����m_pFirstChild�ѱ��ͷ�(����Whileѭ��,δ��NULL),������쳣
		DUIWindowNode TempNode = m_Node;
		m_Node.m_pFirstChild = m_Node.m_pLastChild=NULL;
		m_Node.m_nChildrenCount = 0;

		DUIWindow *pChild = TempNode.m_pFirstChild;
		while (pChild)
		{
			DUIWindow *pNextChild = pChild->m_Node.m_pNextSibling;
			pChild->DM_SendMessage(WM_DESTROY);// ���������Ӵ���
			pChild->Release();
			pChild = pNextChild;
		}
	}

	void DUIWindow::OnSize(UINT nType, CSize size)
	{// �޸ģ������Ӵ��ڸı��Сʱ���»������˹��ܽ���Cache�º�m_bNoPreSizeΪfalseʱ��Ч��
		if (true == m_pDUIXmlInfo->m_bCacheDraw)
		{
			if(!m_pCacheCanvas)
			{
				g_pDMRender->CreateCanvas(m_rcWindow.Width(),m_rcWindow.Height(),&m_pCacheCanvas);
				m_pCacheCanvas->SelectObject(g_pDMFontPool->GetFont(L""));
				DM_MarkCacheDirty(true);
			}
			else
			{
				if (DV_IsPreCmpSizeDiff())
				{
					m_pCacheCanvas->Resize(m_rcWindow.Size());
					DM_MarkCacheDirty(true);
				}
			}
		}
	}

	void DUIWindow::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		do 
		{
			if (ParentShow == nStatus)// �����Ǹ����ڷ�������
			{
				if (bShow&& false == DM_IsVisible(false))//��������ʾ,��������ڲ���ʾ���Ͳ���ʾ
				{ 
					bShow = false;
				}
			}
			else
			{
				m_pDUIXmlInfo->m_bVisible = (TRUE==bShow);  // �����������±����ڵ���ʾ��־
			}

			if (bShow && m_Node.m_pParent)
			{
				bShow = m_Node.m_pParent->DM_IsVisible(true);
			}
			
			if (bShow)// ���±�������ʾ״̬
			{
				DM_ModifyState(0, DUIWNDSTATE_Novisible);
			}
			else
			{
				DM_ModifyState(DUIWNDSTATE_Novisible, 0);
			}

			DUIWindow* pChild = m_Node.m_pFirstChild;
			while (pChild)
			{
				pChild->AddRef();
				pChild->DM_SendMessage(WM_SHOWWINDOW,bShow, ParentShow);// ���������Ӵ��ڵ���ʾ״̬
				DUIWindow* pNextChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
				pChild->Release();
				pChild = pNextChild;
			}

			size_t count = (int)m_ChildPanelArray.GetCount();
			for (size_t i = 0; i < count; i++)
			{
				m_ChildPanelArray[i]->AddRef();
				m_ChildPanelArray[i]->DM_SendMessage(WM_SHOWWINDOW, bShow, ParentShow);// ��������panel�Ӵ��ڵ���ʾ״̬
				m_ChildPanelArray[i]->Release();
			}

			if (!DM_IsVisible(true))
			{	
				if (DM_IsFocusWnd()) 
				{
					GetContainer()->OnSetFocusWnd(NULL);   
				}
				if (DM_GetCapture()==m_hDUIWnd)
				{
					DM_ReleaseCapture();
				}
			}

			if (!m_pDUIXmlInfo->m_bPlaceHolder && DM_IsLayoutFinished())// ����Ӵ��ڲ�ռλ������Ҫ���²���
			{
				DUIWindow* pParent = DM_GetWindow(GDW_PARENT);
				if (pParent)
				{
					pParent->DV_UpdateChildLayout();
				}
			}

		} while (false);
	}

	void DUIWindow::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		do 
		{
			if (m_pDUIXmlInfo->m_bFocusable)
			{
				DV_SetFocusWnd();
			}

			if (m_pDUIXmlInfo->m_bDrag)
			{
				HWND hWnd = GetContainer()->OnGetHWnd();
				::SendMessage(hWnd,WM_SYSCOMMAND, SC_MOVE | HTCAPTION,0);
				break;// HTCAPTION���ú�,ͨ����ӡ��Ϣ,ȷ��hook���յ�WM_LBUTTONUP��Ϣ
			}

			DM_SetCapture();
			DM_ModifyState(DUIWNDSTATE_PushDown, 0, true);

		} while (false);
	}

	void DUIWindow::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		DM_ReleaseCapture();
		DM_ModifyState(0, DUIWNDSTATE_PushDown,true);
		if (!m_rcWindow.PtInRect(pt))
		{
			return;
		}
		if (0!=m_pDUIXmlInfo->m_iID
			||!m_pDUIXmlInfo->m_strName.IsEmpty())
		{
			DMEventCmdArgs Evt(this);
			DV_FireEvent(Evt);
		}
		if (m_pDUIXmlInfo->m_bDrag)
		{
			HWND hWnd = GetContainer()->OnGetHWnd();
			::SendMessage(hWnd,WM_SYSCOMMAND, 0,0);
		}
	}

	void DUIWindow::OnMouseMove(UINT nFlags,CPoint pt)
	{
	}

	void DUIWindow::OnMouseHover(WPARAM wParam,CPoint ptPos)
	{
		if (m_hDUIWnd == DM_GetCapture())
		{
			DM_ModifyState(DUIWNDSTATE_PushDown, 0, false);
		}

		DM_ModifyState(DUIWNDSTATE_Hover, 0, true);
	}

	void DUIWindow::OnMouseLeave()
	{
		if (m_hDUIWnd == DM_GetCapture())
		{
			DM_ModifyState(0, DUIWNDSTATE_PushDown, false);
		}
		DM_ModifyState(0, DUIWNDSTATE_Hover, true);
	}

	BOOL DUIWindow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		BOOL bRet = FALSE;
		if (m_Node.m_pParent)
		{
			bRet = (BOOL)m_Node.m_pParent->DM_SendMessage(WM_MOUSEWHEEL,MAKEWPARAM(nFlags,zDelta),MAKELPARAM(pt.x,pt.y));
		}
		return bRet;
	}

	///
	BOOL DUIWindow::DV_WndProc(UINT uMsg,WPARAM wParam,LPARAM lParam, LRESULT& lResult)
	{
		return FALSE;
	}

	DMCode DUIWindow::OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr)
	{
		do 
		{
			if (DM_ECODE_NOXMLLOADREFRESH == iErr)
			{
				if (false == bLoadXml)
				{
					DM_InvalidateRect(m_rcWindow);// ����ǿͻ���
				}
				iErr = DM_ECODE_OK;/// �Ѵ���
				break;
			}

			if (DM_ECODE_NOXMLLOADPARENTREFRESH == iErr)
			{
				if (false == bLoadXml)
				{
					DUIWindow* pParent = DM_GetWindow(GDW_PARENT);
					if (pParent)
					{
						pParent->DM_InvalidateRect(pParent->m_rcWindow);// ����ǿͻ���
					}
				}
				iErr = DM_ECODE_OK;/// �Ѵ���
				break;
			}

			if (DM_ECODE_NOXMLRELAYOUT== iErr)
			{
				if (false == bLoadXml)
				{
					DUIWindow* pParent = DM_GetWindow(GDW_PARENT);
					if (pParent)
					{
						pParent->DV_UpdateChildLayout();
						pParent->DM_Invalidate();
					}
				}
				iErr = DM_ECODE_OK;/// �Ѵ���
				break;
			}

		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DM_Layout(LPCWSTR lpszAttribute,LPCWSTR lpszValue,bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (DM_IsParentFlowLayout())
			{
				// TODO.
				break;
			}
			if (m_bFloatLayout)
			{
				// TODO.
				break;
			}

			iErr = m_pLayout->SetAttribute(lpszAttribute,lpszValue,bLoadXml);		 // ê�㲼���Լ�����
			if (!bLoadXml&&m_Node.m_pParent)
			{
				m_Node.m_pParent->DV_UpdateChildLayout();
			}
		} while (false);
		return iErr;
	}
}//namespace DM