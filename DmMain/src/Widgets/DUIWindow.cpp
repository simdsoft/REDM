#include "DmMainAfx.h"
#include "DUIWindow.h"

namespace DM
{
	typedef DMXmlDocument* (*fun_cbGetSubXmlDoc)(LPCWSTR,LPCWSTR);
	extern 	fun_cbGetSubXmlDoc  g_pGetSubXmlDoc;
	DUIWindow::DUIWindow()
	{
		m_rcWindow.SetRect(POS_INIT,POS_INIT,POS_INIT,POS_INIT); // 未完成布局时初始值
		m_pDUIXmlInfo.Attach(new DUIWindow_XmlInfo(this));
		g_pDMApp->CreateRegObj((void**)&m_pLayout,NULL,DMREG_Layout);
		DMASSERT(NULL != m_pLayout);
		m_pLayout->SetOwner(this);
		m_dwDUIState  = DUIWNDSTATE_Normal;
		m_pContainer   = NULL;
		m_hDUIWnd      = g_pDMDWndPool->NewDUIWnd(this);
		m_bFloatLayout = false;

		// 注册Event
		// 加入标准事件
		DMADDEVENT(DMEventCmdArgs::EventID);//
		DMADDEVENT(DMEventHoverCmdArgs::EventID);
		DMADDEVENT(DMEventLeaveCmdArgs::EventID);
		DMADDEVENT(DMEventSizeChangedCmdArgs::EventID);
		DMADDEVENT(DMEventCheckChangingCmdArgs::EventID);
		DMADDEVENT(DMEventCheckChangedCmdArgs::EventID);

		// focus
		DMADDEVENT(DMEventSetFocusArgs::EventID);
		DMADDEVENT(DMEventKillFocusArgs::EventID);
	}

	DUIWindow::~DUIWindow()
	{
		g_pDMDWndPool->DestoryDUIWnd(m_hDUIWnd);

		// 移除注册Event
		m_EventMgr.RemoveAllEvent();
	}

	//---------------------------------------------------
	// Function Des:基础属性
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
			if (strValue.IsEmpty())// 如果value是空的，存在key就清空key,不存在就直接跳出
			{
				if (bExist)
				{
					m_StrDataMap.RemoveKey(strKey);
					iErr = DM_ECODE_OK;
				}
				break;
			}

			if ((bExist&&bReplace)// 存在+强制替换
				||false == bExist)// 不存在
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
			{// 1.不在窗口内，跳出
				hDUIWnd = 0;
				break;
			}

			CRect rcClient;
			DV_GetClientRect(&rcClient);
			if (!rcClient.PtInRect(pt))
			{// 2.在窗口的非客户区，跳出，返回窗口模拟句柄
				break;
			}
			DUIWND hDUIChildWnd = 0;
			DUIWindow* pChild   = m_Node.m_pLastChild;
			while (pChild)
			{// 3.递归子窗口
				if (pChild->DM_IsVisible(true))
				{
					if (true == bFindNoMsg||(false == bFindNoMsg&&!pChild->DM_IsMsgNoHandle()))
					{
						hDUIChildWnd = pChild->HitTestPoint(pt,bFindNoMsg);
						if (hDUIChildWnd)
						{// 3.1子窗口存在，则返回子窗口句柄
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
	// Function Des:初始化
	DMCode DUIWindow::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		for (DMXmlNode XmlChildNode = XmlNode.FirstChild(); XmlChildNode.IsValid(); XmlChildNode=XmlChildNode.NextSibling())
		{
			if (0 == _wcsicmp(XmlChildNode.GetName(),SUB_NODE))// 在窗口布局中支持sub标签
			{
				// todo.
				CStringW  strValue = XmlChildNode.Attribute(L"src");
				CStringW strType;
				CStringW strResName;
				int iFind = strValue.ReverseFind(_T(':'));
				if (-1 != iFind)// 可能是LAYOUT:xxx
				{
					strType = strValue.Left(iFind);
					strResName = strValue.Right(strValue.GetLength()-iFind-1);
				}
				else// 也可能是xxx
				{	
					strType	   = RES_LAYOUT;
					strResName = strValue;
				}

				DMXmlDocument* pDoc = NULL;
				if (g_pGetSubXmlDoc
					&&NULL!=(pDoc = g_pGetSubXmlDoc(strType,strResName)))// 支持外部生成Doc传入
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
					{// 流式布局也是duiwindow,所以先判断是否为流式布局对象
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
#ifdef _DMDesigner_
			m_XmlNode = XmlNode;
#endif// _DMDesigner_

			DMBase::InitDMData(XmlNode);	 // 子类先处理，未处理的交由DUIWindow自身m_pDUIXmlInfo处理，这样子类如设置相同的属性值，子类优先处理完
			if (0!=DM_SendMessage(WM_CREATE))// 不为0表示创建失败
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
			DM_SendMessage(WM_SHOWWINDOW,DM_IsVisible(true),ParentShow);// 仅仅给自己发一个WM_SHOWWINDOW消息
			DV_CreateChildWnds(XmlNode);
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des:容器
	IDMContainerPtr DUIWindow::GetContainer()
	{
		DMASSERT_EXPR(NULL!=m_pContainer,L"m_pContainer为空!");
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
	// Function Des: 换肤
	DMCode DUIWindow::DV_UpdateSkin(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lp == DMREG_Skin)// skin换肤
			{
				DMSmartPtrT<IDMSkin> pSkin;
				DMSmartPtrT<IDMSkin> pNcSkin;
				m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
				m_pDUIXmlInfo->m_pStyle->GetNcSkin(&pNcSkin);
				// 确认skin是否在要更新的列表中
				if (g_pDMApp->IsNeedUpdateSkin(pSkin)
					||g_pDMApp->IsNeedUpdateSkin(pNcSkin))
				{
					iErr = DM_ECODE_OK;
					break;
				}
			}

			// todo.其余换肤
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
	// Function Des:事件
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
			m_EventMgr.FireEvent(Evt);				// 分发给注册消息
			if (Evt.m_iHandleCount>iOldHandleCount)
			{
				iErr = DM_ECODE_OK;					// 有函数处理了这个消息
				break;
			}

			if (DM_GetWindow(GDW_OWNER))			// 如Combox
			{
				iErr = DM_GetWindow(GDW_OWNER)->DV_FireEvent(Evt);
				break;// 跳出
			}

			iErr = GetContainer()->OnFireEvent(Evt);// 此处进入了Event消息队列，Evt中包含了Event的发出者DUIWindow
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
	// Function Des: 布局相关
	DMCode DUIWindow::DV_UpdateChildLayout()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (DMREG_FlowLayout == V_GetClassType())
			{
				DMASSERT_EXPR(0,L"流式布局请重载DV_UpdateChildLayout!");
				break;
			}
			if (!DM_IsLayoutFinished())
			{ 
				DMASSERT_EXPR(0,L"窗口未完成自身布局!");
			}
			iErr = m_pLayout->UpdateChildLayout();
			DM_Invalidate();// 更新完后刷新整个区域
		} while (false);
		return iErr;
	}

	DMCode DUIWindow::DV_GetChildMeasureLayout(LPRECT lpRect)
	{
		DV_GetClientRect(lpRect);
		return DM_ECODE_OK;
	}

	// 通过文本来计算大小
	DMCode DUIWindow::DV_GetDesiredSize(LPRECT pRcContainer,SIZE &sz)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pDUIXmlInfo->m_strText.IsEmpty())
			{
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
		{// 父窗口存在,且是流式布局类型！
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
			m_bFloatLayout = true;// 提前设置,不然如果区域相同,此值将未被设置
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
		if(!(m_bFloatLayout))// 不是绝对坐标
		{
			if (!DM_IsParentFlowLayout())//  不是流式布局的子窗口
			{
				iErr = m_pLayout->UpdateLayout(lpRect,m_rcWindow);
			}
		}

		if (DMSUCCEEDED(iErr)) // 0表示全部成功
		{
			DM_SendMessage(WM_NCCALCSIZE);//计算非客户区大小

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
		{// 初始赋值-1
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
			{// 绝对坐标不处理
				break;
			}

			if (FinalRect.IsRectEmpty())// 预先处理为空的状态，因为IntersectRect处理空窗口时会返回false，而我们不能把空的FinalRect变为(0,0,0,0)坐标（SetRectEmpty）
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
				{// 允许子窗口的区域大于父窗口的m_rcWindow，这样才能支持父窗口带滚动
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


		// 在cache画布下使用此代码更新画布
		DM_SendMessage(WM_NCCALCSIZE);// 计算非客户区大小,子类中处理

		CRect rcClient;
		DV_GetClientRect(&rcClient);
		DM_SendMessage(WM_SIZE,0,MAKELPARAM(rcClient.Width(),rcClient.Height()));
		DV_UpdateChildLayout();
		return DM_ECODE_OK;
	}

	//---------------------------------------------------
	// Function Des:状态
	DMCode DUIWindow::DV_GetState(int& iState)
	{
		DWORD dwDUIState = m_dwDUIState;
		if (dwDUIState&DUIWNDSTATE_Check)
		{// DUIWNDSTATE_Check视同DUIWNDSTATE_PushDown
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
		DV_OnStateChanged(dwOldState,dwNewState);// 使用临时变量记录新状态防止DV_OnStateChanged需要取当前的state
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
	// Function Des:属性
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
			if (!m_pDUIXmlInfo->m_bPlaceHolder&&!m_pDUIXmlInfo->m_bVisible)// 不显示也不占位，大小为NULL
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
		DMCode iErr = DM_ECODE_OK;//默认为OK
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

			DM_MarkCacheDirty(true);     // 标志为脏区
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
		{// 注意这里应该是!=
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
	// Function Des:缓存画布
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
	// Function Des: 定时器
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
	// Function Des: 窗口树接口
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

			if (pInsertAfter == m_Node.m_pLastChild)// 如果last为最后一个控件
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
				else // 插入到最前面
				{
					m_Node.m_pFirstChild = pNewChild;
				}
				m_Node.m_pLastChild = pNewChild;
			}
			else if (pInsertAfter==DUIWND_FIRST)
			{
				pNewChild->m_Node.m_pNextSibling = m_Node.m_pFirstChild;
				if (m_Node.m_pFirstChild)// 插入到最后面
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
				// 插入到中间-意味着前后都有窗口了！
				if (pInsertAfter->m_Node.m_pParent != this)// 被插入的子窗口的父窗口不是调用者！
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
			if (this != pChild->DM_GetWindow(GDW_PARENT))// 被插入的子窗口的父窗口不是调用者！
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
			if (this != pChild->DM_GetWindow(GDW_PARENT))// 被插入的子窗口的父窗口不是调用者！
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
	// Function Des:绘制
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
				if (1 == iBgStates)// 如果背景色只有一种颜色有效，而且就是nomarl，则不刷新,用于和以前只有clrbg保持一致
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
		uFormat &= ~DT_SINGLELINE;  // 去掉单行
		//uFormat &= ~DT_VCENTER;		// 去掉竖直居中，多行中不支持竖直居中
		uFormat = uFormat|DT_WORDBREAK;
		return DUIWindow::DV_DrawText(pCanvas, szBuf, szBuf.GetLength(), lpRect, uFormat);
#endif
		do //hgy: 为什么XML中\R\N不能被直接识别，因为XML文件中的"/n",会被认为是一个字符串"///n"是两个字符'//'和'/n'，而不是转义字符"/n",
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
			int iLineHei = szChar.cy;		// 默认行高
			CPoint pt    = rcDest.TopLeft();
			int i        = 0;				// 当前字符
			int nLine	 = 1;				// 当前行
			lpRect->right = lpRect->left;   // 用于DT_CALCRECT
			LPCWSTR pszBufCopy = pszBuf;
			while (i<cchText)
			{
				LPWSTR pNextChar = CharNextW(pszBufCopy);
				if (*pszBufCopy==L'\\' && pNextChar && *pNextChar==L'n')
				{
					pt.y += iLineHei+nLineInter;		 // 行高+行间距
					if (pt.y>=rcDest.bottom)
					{
						break;
					}
					pt.x = rcDest.left;					 // x还原
					nLine ++ ;							 // 行数++
					i += 2;								 // 字符数++
					pszBufCopy = CharNextW(pNextChar);	 // pszBufCopy同样跳两个字符
					continue;
				}

				pCanvas->MeasureText(pszBufCopy,1,&szChar);
				if (pt.x+szChar.cx>rcDest.right)		 // 大于右边限制
				{
					pt.y += iLineHei+nLineInter;		 // 行高+行间距
					if (pt.y>=rcDest.bottom)
					{
						break;
					}
					pt.x =  rcDest.left;			     // x还原
					nLine++;							 // 行数++
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

	// 为DC准备好当前窗口的绘图环境,从顶层窗口开始设置,设置所有的子窗口
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
				DMASSERT_EXPR(0,L"亲,弹这个框是告诉你,你的DM_GetCanvas和DM_ReleaseCanvas没有配对使用!");
				break;
			}

			//1. 开始计算绘制区----------------------
			if (bClientDC)
			{
				DV_GetClientRect(&m_DUIData.m_rcGetCanvas);
			}
			else
			{
				m_DUIData.m_rcGetCanvas = m_rcWindow;
			}

			if (DMOLEDC_NODRAW != dcFlags)//需做绘制时，将DC限制在父窗口的可见区域
			{
				DUIWindow *pParent = DM_GetWindow(GDW_PARENT);
				while (pParent)
				{
					CRect rcParent;
					pParent->DV_GetClientRect(&rcParent);
					// 取矩形交集
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

			//2. 开始通过绘制区创建画布----------------------
			m_pContainer->OnGetCanvas(m_DUIData.m_rcGetCanvas,dcFlags,&pCanvas);
			if (NULL == pCanvas)
			{// 考虑到很多外部控件没有对pCanvas为NULL做判断,所以内部强制它不为NULL,即保证此函数不会失败
				g_pDMRender->CreateCanvas(0, 0,&pCanvas);
				//break;
			}

			//3. 绘制----------------------------------------
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
			if (m_DUIData.m_dcFlags & DMOLEDC_PAINTBKGND) // 画了背景，自动画前景
			{
				DM_DrawForeground(pCanvas,&m_DUIData.m_rcGetCanvas);
			}
			if (m_DUIData.m_bClipCanvas)
			{
				pCanvas->PopClip();
			}

			GetContainer()->OnReleaseCanvas(m_DUIData.m_rcGetCanvas,m_DUIData.m_dcFlags,pCanvas);
			// 标记缓存脏
			if (DM_IsDrawToCache()) 
			{
				DM_MarkCacheDirty(true);
			}
		} while (false);
		m_DUIData.m_bClipCanvas = false;
		m_DUIData.m_dcFlags = -1;// 放在外面,不然可能被break跳过了
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
	// Function Des:消息
	LRESULT DUIWindow::DM_SendMessage(UINT uMsg, WPARAM wp /*= 0*/, LPARAM lp /*= 0*/,BOOL *pbMsgHandled/*=NULL*/)
	{	
		LRESULT lResult = 0;
		do 
		{
			// 预处理:AddRef保证DUI不会被delete掉，保存当前消息处理状态
			AddRef(); 	
			DUIWNDMSG msgCur			= {uMsg,wp,lp};
			DUIWNDMSG *pOldMsg          = m_DUIData.m_pCurMsg;
			m_DUIData.m_pCurMsg   = &msgCur;
			BOOL bOldMsgHandle          = IsMsgHandled();//备分上一个消息的处理状态

			SetMsgHandled(FALSE);
			ProcessDuiMessage(uMsg, wp, lp, lResult);//触发消息宏DUIMsgCrack

			if (pbMsgHandled)
			{
				*pbMsgHandled = IsMsgHandled();
			}

			// 恢复预处理
			SetMsgHandled(bOldMsgHandle);
			m_DUIData.m_pCurMsg = pOldMsg;
			Release();

			// TODO.

		} while (false);

		return lResult;
	}

	// 不为NULL时，处理了就返回，为NULL就遍历子窗口传递
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

	// 背景绘背景图
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
		if (pSkin.isNull())// 绘制颜色
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

	// 前景绘文字
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
					// 绘制焦点框
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
			if (rcNcMargin.IsRectNull())// 非客户区边框值为空
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
			DUIWindow::DV_GetClientRect(rcClient);// 此处的rcClient基本不会top,left=0,0
			pCanvas->SaveCanvas();
			pCanvas->ExcludeClipRect(&rcClient);	// 不绘制客户区
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
				DM_ReleaseCanvas(pCanvas);// 释放内存
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
			if (ParentEnable == nStatus) // 命令是父窗口发过来的 
			{
				if (bEnable && DM_IsDisable(false))// 父窗口可用,如果本窗口禁用，就继续禁用
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

			// 给子窗口发消息
			DUIWindow* pChild = m_Node.m_pFirstChild;
			while (pChild)
			{
				pChild->AddRef();
				pChild->DM_SendMessage(WM_ENABLE,bEnable, ParentEnable);// 更新所有子窗口的显示状态
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
	
		// 先清空,再释放,在flash游戏中,它调用DMActiveXSite::Clear时，有可能引起WM_ACTIVATEAPP,
		// 而如果flash是中间窗口,由于m_pFirstChild已被释放(还在While循环,未置NULL),则可能异常
		DUIWindowNode TempNode = m_Node;
		m_Node.m_pFirstChild = m_Node.m_pLastChild=NULL;
		m_Node.m_nChildrenCount = 0;

		DUIWindow *pChild = TempNode.m_pFirstChild;
		while (pChild)
		{
			DUIWindow *pNextChild = pChild->m_Node.m_pNextSibling;
			pChild->DM_SendMessage(WM_DESTROY);// 销毁自身子窗口
			pChild->Release();
			pChild = pNextChild;
		}
	}

	void DUIWindow::OnSize(UINT nType, CSize size)
	{// 修改：仅在子窗口改变大小时更新画布（此功能仅在Cache下和m_bNoPreSize为false时起效）
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
			if (ParentShow == nStatus)// 命令是父窗口发过来的
			{
				if (bShow&& false == DM_IsVisible(false))//父窗口显示,如果本窗口不显示，就不显示
				{ 
					bShow = false;
				}
			}
			else
			{
				m_pDUIXmlInfo->m_bVisible = (TRUE==bShow);  // 正常命令，则更新本窗口的显示标志
			}

			if (bShow && m_Node.m_pParent)
			{
				bShow = m_Node.m_pParent->DM_IsVisible(true);
			}
			
			if (bShow)// 更新本窗口显示状态
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
				pChild->DM_SendMessage(WM_SHOWWINDOW,bShow, ParentShow);// 更新所有子窗口的显示状态
				DUIWindow* pNextChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
				pChild->Release();
				pChild = pNextChild;
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

			if (!m_pDUIXmlInfo->m_bPlaceHolder && DM_IsLayoutFinished())// 如果子窗口不占位，则需要重新布局
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
				break;// HTCAPTION设置后,通过打印消息,确认hook中收到WM_LBUTTONUP消息
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
					DM_InvalidateRect(m_rcWindow);// 处理非客户区
				}
				iErr = DM_ECODE_OK;/// 已处理
				break;
			}

			if (DM_ECODE_NOXMLLOADPARENTREFRESH == iErr)
			{
				if (false == bLoadXml)
				{
					DUIWindow* pParent = DM_GetWindow(GDW_PARENT);
					if (pParent)
					{
						pParent->DM_InvalidateRect(pParent->m_rcWindow);// 处理非客户区
					}
				}
				iErr = DM_ECODE_OK;/// 已处理
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
				iErr = DM_ECODE_OK;/// 已处理
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

			iErr = m_pLayout->SetAttribute(lpszAttribute,lpszValue,bLoadXml);		 // 锚点布局自己解析
			if (!bLoadXml&&m_Node.m_pParent)
			{
				m_Node.m_pParent->DV_UpdateChildLayout();
			}
		} while (false);
		return iErr;
	}
}//namespace DM