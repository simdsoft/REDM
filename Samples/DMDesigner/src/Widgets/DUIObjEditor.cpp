#include "DMDesignerAfx.h"
#include "DUIObjEditor.h"             

DUIObjEditor::DUIObjEditor()
{
	m_pShow		 = NULL;
	m_pHoverlFrame= NULL;
	m_pSelFrame  = NULL;
	m_pDragFrame = NULL;
	m_pObjXml	 = NULL;
	m_pTools	 = NULL;
	m_bInit      = false;

	m_DesignMod  = NoneMode;
}

DMCode DUIObjEditor::InitObjEditor()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{ 
		m_pObjXml      = g_pMainWnd->m_pDesignerXml;
		m_pHoverlFrame = g_pMainWnd->FindChildByNameT<DUIStatic>(L"ds_hoverframe");DMASSERT(m_pHoverlFrame);
		m_pSelFrame    = g_pMainWnd->FindChildByNameT<DUIStatic>(L"ds_selframe");DMASSERT(m_pSelFrame);
		m_pDragFrame   = g_pMainWnd->FindChildByNameT<DUIDragFrame>(L"ds_dragframe");DMASSERT(m_pDragFrame);
		m_pTools       = g_pMainWnd->FindChildByNameT<DUITabCtrl>(L"ds_tool");DMASSERT(m_pTools);
		m_pTools->m_EventMgr.SubscribeEvent(DMEventTabSelChangedArgs::EventID, Subscriber(&DUIObjEditor::OnDesignModeChanged, this));
		m_pCheckBtn    = g_pMainWnd->FindChildByNameT<DUICheckBox>(L"ds_w_widget");DMASSERT(m_pCheckBtn);
		m_pCheckBtn->DM_SetCheck(false);
		m_pCheckBtn->m_EventMgr.SubscribeEvent(DMEventCheckChangingCmdArgs::EventID, Subscriber(&DUIObjEditor::OnCheckChanging, this));
		m_pCheckBtn->m_EventMgr.SubscribeEvent(DMEventCheckChangedCmdArgs::EventID, Subscriber(&DUIObjEditor::OnCheckChanged, this));
		SetDesignMode((DesignMode)m_pTools->GetCurSel());
		m_bInit = true;
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
} 

DMCode DUIObjEditor::UnInitObjEditor()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (false == m_bInit)
		{
			break;
		}

		if (m_pTools)
		{
			m_pTools->m_EventMgr.UnSubscribeEvent(DMEventTabSelChangedArgs::EventID, Subscriber(&DUIObjEditor::OnDesignModeChanged, this));
		}
		if (m_pCheckBtn)
		{
			m_pCheckBtn->m_EventMgr.UnSubscribeEvent(DMEventCheckChangingCmdArgs::EventID, Subscriber(&DUIObjEditor::OnCheckChanging, this));
			m_pCheckBtn->m_EventMgr.UnSubscribeEvent(DMEventCheckChangedCmdArgs::EventID, Subscriber(&DUIObjEditor::OnCheckChanged, this));
		}
		RemoveAllDesignChild();
		m_pHoverlFrame->DM_SetVisible(false,true);
		m_pSelFrame->DM_SetVisible(false,true);
		m_pDragFrame->DM_SetVisible(false,true);
		SetRangeSize(CSize(0,0));
		m_pShow		 = NULL;
		m_pObjXml	 = NULL;
		m_pTools	 = NULL;
		m_bInit      = false;
		m_DesignMod  = NoneMode;
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode DUIObjEditor::SetDesignMode(DesignMode ds_mode)
{
	// 1.默认隐藏Hover Frame
	m_pHoverlFrame->DM_SetVisible(false,true);
	m_DesignMod = ds_mode;
	if (m_pShow)
	{
		DragFrameInSelMode();
		SelFrameInAllMode();
		m_pShow->SetDesignMode(m_DesignMod);
	}
	m_pCheckBtn->DM_SetCheck(AddMode == m_DesignMod);///< 当不是add模式时,自动取消选中
	return DM_ECODE_OK;
}

DUIRoot* DUIObjEditor::InitDesignChild(HDMTREEITEM hRootTree)
{
	DUIRoot* pChild = NULL;
	g_pDMApp->CreateRegObj((void**)&pChild, DUIRoot::GetClassName(),DMREG_Window);
	if (pChild)
	{
		DM_InsertChild(pChild);
		pChild->m_pParent = this;
		pChild->InitDesignEditor(hRootTree);
		pChild->DM_SetVisible(false,true);
		m_pHoverlFrame->DM_SetWndToTop();
		m_pSelFrame->DM_SetWndToTop();
		m_pDragFrame->DM_SetWndToTop();
	}
	return pChild;
}

DUIRoot* DUIObjEditor::GetShowDesignChild()
{
	DUIWindow *pChild = m_Node.m_pFirstChild;
	while (pChild)
	{
		if (pChild->DM_IsVisible()&&0 == _wcsicmp(pChild->V_GetClassName(),DUIRoot::GetClassName()))
		{
			break;
		}
		pChild = pChild->m_Node.m_pNextSibling;
	}
	return (DUIRoot*)pChild;
}

DMCode DUIObjEditor::ShowDesignChild(DUIRoot* pShow)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (pShow == m_pShow)
		{
			break;
		}
		if (m_pShow)
		{
			m_pShow->DM_SetVisible(false,true);
		}
		m_pShow = pShow;
		if (m_pShow)
		{
			m_pShow->DM_SetVisible(true,true);
		}

		UpdateScrollRangeSize();
		m_pShow->DM_UpdateLayout(NULL);
		SetDesignMode(m_DesignMod);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode DUIObjEditor::RemoveAllDesignChild()
{
	DUIWindow*pChild = DM_GetWindow(GDW_FIRSTCHILD);
	while (pChild)
	{
		DUIWindow* pTemp = pChild;
		pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
		if (0 == _wcsicmp(pTemp->V_GetClassName(),DUIRoot::GetClassName()))
		{
			DM_DestroyChildWnd(pTemp);
		}
	}
	m_pShow = NULL;
	return true;
}

void DUIObjEditor::UpdateScrollRangeSize()
{
	// 使用可见的ObjDesignWnd来设置滚动条
	DUIWindow *pChild = GetShowDesignChild();
	if (pChild)
	{
		CRect rcChildWnd = pChild->m_rcWindow;
		CRect rcParentWnd = m_rcWindow;
		// 拖动到左上角不出现滚动条
		if (rcChildWnd.top<rcParentWnd.top)
		{
			rcChildWnd.top = rcParentWnd.top;
		}
		if (rcChildWnd.left< rcParentWnd.left)
		{
			rcChildWnd.left = rcParentWnd.left;
		}
		int iHei = rcChildWnd.bottom -rcParentWnd.top;
		int iWid = rcChildWnd.right-rcParentWnd.left;;
		m_rcMeasure.UnionRect(rcChildWnd,rcParentWnd);
		CSize szView(iWid,iHei);
		SetRangeSize(szView);
	}
} 

void DUIObjEditor::OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew)
{
	DUIWindow *pChild = GetShowDesignChild();
	if (pChild)
	{
		CRect rcChild = pChild->m_rcWindow;
		rcChild.OffsetRect(ptOld-ptNew);
		pChild->DM_FloatLayout(rcChild);
		SetDesignMode(m_DesignMod);
	}
}

DMCode DUIObjEditor::DV_GetChildMeasureLayout(LPRECT lpRect)
{// 在DV_Arrange中，会通过父窗口的大小来限制子窗口
	*lpRect	= m_rcMeasure;
	return DM_ECODE_NOLOOP;
}

DMCode DUIObjEditor::OnDesignModeChanged(DMEventArgs* pEvt)
{ 
	DMEventTabSelChangedArgs* pEvent = (DMEventTabSelChangedArgs*)pEvt;
	SetDesignMode((DesignMode)pEvent->m_uNewSel);
	return DM_ECODE_OK;
}

DMCode DUIObjEditor::OnCheckChanging(DMEventArgs* pEvt)
{
	DMEventCheckChangingCmdArgs *pEvent = (DMEventCheckChangingCmdArgs*)pEvt;
	if (AddMode == m_DesignMod&&false == pEvent->m_bChecking)
	{
		pEvent->m_bCancel = true;// 在Add模式下不允许直接取消check
	}
	return DM_ECODE_OK;
}

DMCode DUIObjEditor::OnCheckChanged(DMEventArgs* pEvt)
{
	if (AddMode != m_DesignMod&&m_pCheckBtn->DM_IsChecked())
	{
		m_pTools->SetCurSel(2);
	}
	return DM_ECODE_OK;
}

DMCode DUIObjEditor::HoverInSelOrAddMode(DUIWindow* pDUIHover)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{ 
		if (MoveMode == m_DesignMod || NULL == pDUIHover)
		{
			break;
		}
		CRect rcFrame = pDUIHover->m_rcWindow;
		if (AddMode == m_DesignMod)
		{
			rcFrame.InflateRect(8,8,8,8);
			m_pHoverlFrame->SetAttribute(L"skin",L"ds_addframe");
		}
		else
		{
			rcFrame.InflateRect(3,3,3,3);
			m_pHoverlFrame->SetAttribute(L"skin",L"ds_hoverframe");
		}
		m_pHoverlFrame->DM_FloatLayout(rcFrame);
		m_pHoverlFrame->DM_SetVisible(true,true);
		DM_Invalidate();

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode DUIObjEditor::SelFrameInAllMode()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_pObjXml->m_hObjSel)
		{
			break;	
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjXml->m_pObjTree->GetItemData(m_pObjXml->m_hObjSel);
		if (!pData||!pData->IsValid())
		{
			break;
		}

		CRect rcFrame = pData->m_pDUIWnd->m_rcWindow;
		pData->m_pDUIWnd->GetContainer()->OnClientToScreen(rcFrame);
		g_pMainWnd->ScreenToClient(rcFrame);
		rcFrame.InflateRect(1,1,1,1);
		m_pSelFrame->DM_FloatLayout(rcFrame);
		m_pSelFrame->DM_SetVisible(pData->m_pDUIWnd->DM_IsVisible(true),true);// 隐藏的窗口就不框选了
		DM_Invalidate();
		iErr = DM_ECODE_OK;
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		m_pSelFrame->DM_SetVisible(false,true);
	}
	return iErr;
}

DMCode DUIObjEditor::DragFrameInSelMode()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (SelectMode != m_DesignMod||NULL == m_pObjXml->m_hObjSel)
		{
			break;	
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjXml->m_pObjTree->GetItemData(m_pObjXml->m_hObjSel);
		if (!pData||!pData->IsValid())
		{
			break;
		}
		CRect rcFrame;
		if (!DMSUCCEEDED(m_pDragFrame->InitDragFrame(pData,rcFrame)))
		{
			break;
		}
		m_pDragFrame->DM_FloatLayout(rcFrame);
		m_pDragFrame->DM_SetVisible(true,true);
		DM_Invalidate();
		iErr = DM_ECODE_OK;
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		m_pDragFrame->DM_SetVisible(false,true);
	}
	return iErr;
}

