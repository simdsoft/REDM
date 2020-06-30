#include "DMDesignerAfx.h"
#include "WidgetAttr.h"     

BEGIN_MSG_MAP(WidgetAttr)
	MSG_WM_LBUTTONDOWN(OnLButtonDown)
END_MSG_MAP()
BEGIN_EVENT_MAP(WidgetAttr)
	EVENT_NAME_COMMAND(L"ds_widget_expand", OnExpand)
END_EVENT_INBASE()

WidgetAttr*  WidgetAttr::ms_pthis = NULL;
WidgetAttr::WidgetAttr()
{ 
	m_pPanel = m_pObjXml->m_pRighXml->m_pList->FindChildByName(L"ds_attr_widget_panel",true);DMASSERT(m_pPanel);
	m_pExpandBtn = m_pObjXml->m_pRighXml->m_pList->FindChildByNameT<DUIButton>(L"ds_widget_expand",true);DMASSERT(m_pExpandBtn);
	m_pPropFrame = m_pObjXml->m_pRighXml->m_pList->FindChildByNameT<DUIPropFrame>(L"ds_widget_prop",true);DMASSERT(m_pPropFrame);
	m_pPropFrame->m_pPropList->m_EventMgr.SubscribeEvent(DM::PropValueChangedArgs::EventID, Subscriber(&WidgetAttr::OnPropValueChanged, this));
	m_pPropFrame->m_pPropList->m_EventMgr.SubscribeEvent(DM::PropDelingArgs::EventID, Subscriber(&WidgetAttr::OnPropDeling, this));
	m_pPropFrame->m_pPropList->m_EventMgr.SubscribeEvent(DM::PropDelArgs::EventID, Subscriber(&WidgetAttr::OnPropDel, this));
	ms_pthis = this; 
	m_pPropStyle = NULL;
	m_pCurDUI = NULL;
}

WidgetAttr::~WidgetAttr()
{
	ms_pthis = NULL; 
	m_pPropStyle = NULL;
	m_pCurDUI = NULL;
	if (m_pExpandWnd.isValid()&&m_pExpandWnd->IsWindow())
	{
		m_pExpandWnd->DestroyWindow();
		m_pExpandWnd.Release();
	}
	if (m_pPropFrame)
	{
		m_pPropFrame->m_pPropList->m_EventMgr.UnSubscribeEvent(DM::PropValueChangedArgs::EventID, Subscriber(&WidgetAttr::OnPropValueChanged, this));
		m_pPropFrame->m_pPropList->m_EventMgr.UnSubscribeEvent(DM::PropDelingArgs::EventID, Subscriber(&WidgetAttr::OnPropDeling, this));
		m_pPropFrame->m_pPropList->m_EventMgr.UnSubscribeEvent(DM::PropDelArgs::EventID, Subscriber(&WidgetAttr::OnPropDel, this));
	}

	ReleaseProp();
}

DMCode WidgetAttr::UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hSel);
		if (NULL == pData||NULL == pData->m_pDUIWnd)
		{
			break;
		}

		m_pPropStyle = NULL;
		if (hSel && 0 == m_pObjTree->GetParentItem(hSel))// 切换到最上层了，默认为DMHWnd窗口
		{
			m_pObjXml->m_pRighXml->m_pList->ExpandItem(WIGDET_ATTR,false);
			m_pPanel->DM_SetVisible(false,true);
			m_pCurDUI = NULL;
			break;
		}

		if (pData->m_pDUIWnd == pData->m_pRootWnd||m_pCurDUI != pData->m_pDUIWnd)// 不相等就切换
		{
			m_pCurDUI = pData->m_pDUIWnd;
			InitAttrArray();
		}

		m_pObjXml->m_pRighXml->m_pList->ExpandItem(WIGDET_ATTR,true);
		m_pPanel->DM_SetVisible(true,true);
		
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode WidgetAttr::OnExpand()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (m_pExpandWnd.isNull())
		{
			m_pExpandWnd.Attach(new AttrExpandWnd(this));
			m_pExpandWnd->DM_CreateWindowEx(L"ds_expandwnd",DM_DEF_WINDOW_NAME,WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0,g_pMainWnd->m_hWnd,NULL,false);
			m_pExpandWnd->SendMessage(WM_INITDIALOG);
		} 

		if (m_pExpandWnd->IsWindowVisible())
		{
			m_pExpandWnd->Hide();
		}
		else
		{
			CRect rcExpand = m_pExpandBtn->m_rcWindow;
			m_pExpandBtn->GetContainer()->OnClientToScreen(rcExpand);
			m_pExpandWnd->SetWindowPos(g_pMainWnd->m_hWnd, rcExpand.right,rcExpand.bottom-34,0, 0, SWP_NOSIZE);
			m_pExpandWnd->Show();
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode WidgetAttr::OnPropValueChanged(DMEventArgs *pEvt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == pEvt || NULL == m_hObjSel || NULL == m_pCurDUI)
		{
			break;
		}

		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (pData->m_pDUIWnd != m_pCurDUI)
		{
			break;
		}
		
		PropValueChangedArgs* pEvent = (PropValueChangedArgs*)pEvt;
		CStringW strName  = pEvent->m_pSel->GetName();
		CStringW strValue = pEvent->m_pSel->GetValue();

		if (0 == strName.CompareNoCase(DMAttr::DUIWindowAttr::bool_bvisible))// 针对eye关闭时特殊处理
		{
			DM::LPTVITEMEX pTV = m_pObjTree->GetItem(m_hObjSel);
			if (DMTVEXEye_UnEye == pTV->iEyeValue)
			{
				bool bVisible = 0;
				dm_parsebool(strValue,bVisible);
				pData->m_iOrgVisible = bVisible?DMVISIBLE_SHOW:DMVISIBLE_HIDE;
				iErr = DM_ECODE_OK;
			}
			else
			{
				iErr = pData->m_pDUIWnd->SetAttribute(strName,strValue);
			}
			m_pObjXml->m_pObjEditor->SelFrameInAllMode();
		}
		else if (0 == strName.CompareNoCase(DMAttr::DUIWindowAttr::STRING_style))// 针对style特殊处理
		{
			HDMTREEITEM hStyle = m_pObjXml->FindStyle(strValue);
			if (NULL == hStyle)
			{
				DM_MessageBox(L"未找到此样式池");
			}
			else
			{
				DMXmlNodePtr pStyleNode = (DMXmlNodePtr)m_pObjXml->m_pProjTree->GetItemData(hStyle);
				if (pStyleNode)// 如果外部已存在了样式中的属性(外部属性会覆盖),则此属性不再设置,否则设置
				{
					DMXmlAttribute XmlAttribute = pStyleNode->FirstAttribute();
					while (XmlAttribute.IsValid())
					{
						if (0 != _wcsicmp(XmlAttribute.GetName(),XML_ID))// 所有样式都带有ID，所以要排除(控件属性也存在ID)
						{
							DMXmlInitAttrPtr pInitAttr = FindAttrByName(XmlAttribute.GetName());
							if (NULL != pInitAttr&&false == pInitAttr->m_bUse)
							{
								pData->m_pDUIWnd->SetAttribute(XmlAttribute.GetName(),XmlAttribute.GetValue());
							}
						}
						XmlAttribute = XmlAttribute.NextAttribute();
					}
				}
				iErr = DM_ECODE_OK;
			}
		}
		else
		{
			iErr = pData->m_pDUIWnd->SetAttribute(strName,strValue);
		}

		if (!DMSUCCEEDED(iErr))
		{
			DM_MessageBox(L"设置属性失败");
		}
		else
		{
			pData->m_pXmlNode->SetAttribute(strName,strValue);
			pData->m_pDoc->m_bChange = true;
		}
	} while (false);
	// 通知XmlEditor更新
	if (g_pMainWnd)
	{
		DUIXmlUpdateArgs Evt(g_pMainWnd);
		Evt.m_UpdateType = DUIXmlUpdateArgs::XMLUPDATE_CHANGEPROP;
		g_pMainWnd->DV_FireEvent(Evt);
	}
	return iErr;
}

DMCode WidgetAttr::OnPropDeling(DMEventArgs *pEvt)
{
	PropDelingArgs* pEvent = (PropDelingArgs*)pEvt;
	if (0 == pEvent->m_pSel->GetData())
	{
		pEvent->m_bCancel = true;// 不删除第一层目录
	}

	return DM_ECODE_OK;
}

DMCode WidgetAttr::OnPropDel(DMEventArgs *pEvt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == pEvt || NULL == m_hObjSel || NULL == m_pCurDUI)
		{
			break;
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (pData->m_pDUIWnd != m_pCurDUI)
		{
			break;
		}
		PropDelArgs* pEvent = (PropDelArgs*)pEvt;
		DMXmlInitAttrPtr pInitAttr = (DMXmlInitAttrPtr)pEvent->m_pSel->GetData();
		if (NULL == pInitAttr)
		{
			break;
		}
		pInitAttr->m_bUse = false;///< 移除后设置为未使用状态

		if (pEvent->m_pSel == m_pPropStyle)
		{
			CStringW strValue = m_pPropStyle->GetValue();
			HDMTREEITEM hStyle = m_pObjXml->FindStyle(strValue);
			if (NULL != hStyle)
			{
				DMXmlNodePtr pStyleNode = (DMXmlNodePtr)m_pObjXml->m_pProjTree->GetItemData(hStyle);
				if (pStyleNode)// 如果外部已存在了样式中的属性(外部属性会覆盖),则此属性不再设置,否则设置回默认初始属性
				{
					DMXmlAttribute XmlAttribute = pStyleNode->FirstAttribute();
					while (XmlAttribute.IsValid())
					{
						if (0 != _wcsicmp(XmlAttribute.GetName(),XML_ID))// 所有样式都带有ID，所以要排除(控件属性也存在ID)
						{
							DMXmlInitAttrPtr pInitAttr = FindAttrByName(XmlAttribute.GetName());
							if (NULL != pInitAttr&&false == pInitAttr->m_bUse)
							{
								CStringW strType,strName,strValue,strDesc;
								g_pAttr->Parse(pInitAttr->m_pAttr,strType,strName,strValue,strDesc);
								InitSpecialValue(strValue,pInitAttr);
								pData->m_pDUIWnd->SetAttribute(strName,strValue);
							}
						}
						
						XmlAttribute = XmlAttribute.NextAttribute();
					}
				}
			}
			iErr = DM_ECODE_OK;
			//2.从xml中移除此结点,设置未保存状态
			pData->m_pXmlNode->RemoveAttribute(DMAttr::DUIWindowAttr::STRING_style);
			pData->m_pDoc->m_bChange = true;
			m_pPropStyle = NULL;
		}
		else
		{
			CStringW strType,strName,strValue,strDesc;
			g_pAttr->Parse(pInitAttr->m_pAttr,strType,strName,strValue,strDesc);
			InitSpecialValue(strValue,pInitAttr);
			iErr = pData->m_pDUIWnd->SetAttribute(strName,strValue);
			if (!DMSUCCEEDED(iErr))
			{
				DM_MessageBox(L"设置属性失败");
			}
			else
			{
				//2.从xml中移除此结点,设置未保存状态
				pData->m_pXmlNode->RemoveAttribute(strName);
				pData->m_pDoc->m_bChange = true;

				if (0 != strName.CompareNoCase(XML_ID)&&m_pPropStyle)
				{
					CStringW strValue = m_pPropStyle->GetValue();
					HDMTREEITEM hStyle = m_pObjXml->FindStyle(strValue);
					DMXmlNodePtr pStyleNode = (DMXmlNodePtr)m_pObjXml->m_pProjTree->GetItemData(hStyle);
					if (pStyleNode)
					{
						DMXmlAttribute XmlAttribute = pStyleNode->FirstAttribute();
						while (XmlAttribute.IsValid())
						{// 如果外部已存在了样式中的属性(外部属性会覆盖),当外部属性删除时，使用样式中的属性
							if (0 == strName.CompareNoCase(XmlAttribute.GetName()))
							{
								pData->m_pDUIWnd->SetAttribute(XmlAttribute.GetName(),XmlAttribute.GetValue());
								break;
							}
							XmlAttribute = XmlAttribute.NextAttribute();
						}
					}
				}
			}
		}
	} while (false);

	// 通知XmlEditor更新
	if (g_pMainWnd)
	{
		DUIXmlUpdateArgs Evt(g_pMainWnd);
		Evt.m_UpdateType = DUIXmlUpdateArgs::XMLUPDATE_DELPROP;
		g_pMainWnd->DV_FireEvent(Evt);
	}
	return iErr;
}

void WidgetAttr::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptScreen = point;
	::ClientToScreen(g_pMainWnd->m_hWnd,&ptScreen);
	CRect rcExpand = m_pExpandBtn->m_rcWindow;
	m_pExpandBtn->GetContainer()->OnClientToScreen(rcExpand);
	if (!rcExpand.PtInRect(ptScreen))
	{
		if (m_pExpandWnd.isValid()&&m_pExpandWnd->IsWindowVisible())
		{
			m_pExpandWnd->Hide();
		}
	}

	SetMsgHandled(FALSE);
}

DUIWindow* WidgetAttr::GetOwnerWindow()
{
	if (m_pPanel&&m_pPanel->DM_IsVisible())
	{
		return m_pPanel;
	}
	return NULL;
}

DMCode WidgetAttr::OnTreeInit(AttrTree* pTree)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		pTree->RemoveAllItems();
		DMXmlDocument doc;
		DMXmlNode NoDataNode = doc.Base();
		DMXmlNode DataNode = doc.Base();
		AttrExpandWnd::InitTreeNode(NoDataNode,false);
		AttrExpandWnd::InitTreeNode(DataNode,true);

		int nCount = (int)m_strList.GetCount();
		//1.插入根结点
		for (int i=0; i<nCount; i++)
		{
			NoDataNode.SetAttribute(XML_TEXT,m_strList[i]);
			HDMTREEITEM hTreeRoot =  pTree->InsertItem(NoDataNode);
			//2.插入子结点
			int iChildCount = (int)GetCount();
			for (int j=0;j<iChildCount;j++)
			{
				DMXmlInitAttrPtr pInitAttr = GetObj(j);
				if (false == pInitAttr->m_bUse&&0 == pInitAttr->m_strReserve.CompareNoCase(m_strList[i]))
				{
					DataNode.SetAttribute(XML_TEXT,pInitAttr->m_pAttr->GetName());
					HDMTREEITEM hRet = pTree->InsertItem(DataNode,hTreeRoot);
					pTree->SetItemData(hRet,(LPARAM)pInitAttr);	
				}
			}
			if (0 == i)// 仅展开首项
			{
				pTree->Expand(hTreeRoot);
			}	
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode WidgetAttr::OnTreeSel(AttrTree* pTree)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hObjSel)
		{
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (pData->m_pDUIWnd != m_pCurDUI)
		{
			break;
		}
		HDMTREEITEM hSel = pTree->GetSelectedItem();
		if (NULL == hSel)
		{
			break;
		}
		DMXmlInitAttrPtr pInitAttr = (DMXmlInitAttrPtr)pTree->GetItemData(hSel);
		if (NULL == pInitAttr)// 在折叠时，选中的是第一行，是无数据的
		{
			break;
		}
		pInitAttr->m_bUse = true;
		IPropPtr pProp = FindPropByName(pInitAttr->m_strReserve);
		CStringW strInitValue;// 默认为空
		InitSpecialValue(strInitValue,pInitAttr);
		IPropPtr pPropTemp = m_pPropFrame->AddInitAttrProperty(pInitAttr,pProp,strInitValue);

		//更新到XML中
		CStringW strType,strName,strValue,strDesc;
		g_pAttr->Parse(pInitAttr->m_pAttr,strType,strName,strValue,strDesc);
		if (!strInitValue.IsEmpty())
		{
			strValue = strInitValue;
		}
		pData->m_pDUIWnd->SetAttribute(strName,strValue);
		pData->m_pXmlNode->SetAttribute(strName,strValue);
		pData->m_pDoc->m_bChange = true;
		if (0 == strName.CompareNoCase(DMAttr::DUIWindowAttr::STRING_style))
		{
			m_pPropStyle = dynamic_cast<PropString*>(pPropTemp);
		}

		// 通知XmlEditor更新
		if (g_pMainWnd)
		{
			DUIXmlUpdateArgs Evt(g_pMainWnd);
			Evt.m_UpdateType = DUIXmlUpdateArgs::XMLUPDATE_ADDPROP;
			g_pMainWnd->DV_FireEvent(Evt);
		}
		iErr = DM_ECODE_OK;
	} while (false);

	return iErr;
}

DMCode WidgetAttr::OnTreeEnd(AttrTree* pTree)
{
	pTree->RemoveAllItems();
	return DM_ECODE_OK;
}

DMCode WidgetAttr::InitAttrArray()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ReleaseProp();
		if (NULL == m_pCurDUI)
		{
			break;
		} 

		//1.取得类名列表
		m_strList.RemoveAll();
		CStringW strClassName = m_pCurDUI->V_GetClassName();
		if (0 == strClassName.CompareNoCase(L"root"))
		{
			strClassName = L"window";
		}
		g_pAttr->Parse(strClassName,m_strList);
	

		//2.加入到第一列prop中
		int nCount = (int)m_strList.GetCount(); 
		if (nCount<=0)
		{
			break;
		}
		DMXmlDocument doc;
		DMXmlNode XmlBase = doc.Base();
		DMXmlNode XmlRoot = XmlBase.InsertChildNode(L"root");
		for (int i=0; i<nCount; i++)
		{
			DMXmlNode XmlNode = XmlRoot.InsertChildNode(L"propgrid");
			XmlNode.SetAttribute(XML_NAME,m_strList[i]);
			CStringW strDesc = m_strList[i] + L"属性集合";
			XmlNode.SetAttribute(L"desc",strDesc);
			XmlNode.SetAttribute(L"bgroup",L"1");
		}
		m_pPropFrame->m_pPropList->DV_CreateChildWnds(XmlRoot);

		//3.初始化DMArrayT
		for (int i=0; i<nCount; i++)
		{ 
			CStringW strTemp = m_strList[i];
			DMXmlNode XmlRoot = g_pAttr->GetAttrNode(strTemp);
			DMXmlAttribute XmlAttribute = XmlRoot.FirstAttribute();
			while (XmlAttribute.IsValid())
			{
				DMXmlInitAttrPtr pInitAttr = new DMXmlInitAttr(XmlAttribute);
				pInitAttr->m_strReserve = strTemp;
				AddObj(pInitAttr);
				XmlAttribute = XmlAttribute.NextAttribute();
			}
		}

		//4.尝试加入第二列prop
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		DMXmlAttribute XmlAttribute = pData->m_pXmlNode->FirstAttribute();
		while (XmlAttribute.IsValid())
		{
			CStringW strName = XmlAttribute.GetName();
			CStringW strValue = XmlAttribute.GetValue();
			
			DMXmlInitAttrPtr pInitAttr = FindAttrByName(strName);
			if (pInitAttr)
			{
				IPropPtr pParent = FindPropByName(pInitAttr->m_strReserve);
				if (pParent)
				{
					IPropPtr pPropTemp = m_pPropFrame->AddInitAttrProperty(pInitAttr,pParent,strValue);
					pInitAttr->m_bUse = true;// 初始化过的，不会在expand窗口中出现
					if (0 == strName.CompareNoCase(DMAttr::DUIWindowAttr::STRING_style))
					{
						m_pPropStyle = dynamic_cast<PropString*>(pPropTemp);
					}
				} 
			}

			XmlAttribute = XmlAttribute.NextAttribute();
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

IPropPtr WidgetAttr::FindPropByName(CStringW strName)
{
	IPropPtr pProp = NULL;
	do 
	{
		if (strName.IsEmpty())
		{
			break;
		}

		for (POSITION pos = m_pPropFrame->m_pPropList->m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pListProp = m_pPropFrame->m_pPropList->m_lstProps.GetNext(pos);
			if (0 == pListProp->GetName().CompareNoCase(strName))
			{
				pProp = pListProp;
				break;
			}
		}

	} while (false);
	return pProp;
}

DMXmlInitAttrPtr WidgetAttr::FindAttrByName(CStringW strName)
{
	DMXmlInitAttrPtr pInitAttr = NULL;
	do 
	{
		if (strName.IsEmpty())
		{
			break;
		}

		int iCount = (int)GetCount();
		for (int i=0;i<iCount;i++)
		{
			DMXmlInitAttrPtr ptr = GetObj(i);
			CStringW strTempType,strTempName;
			g_pAttr->ParseName(ptr->m_pAttr,strTempType,strTempName);
			if (0 == strName.CompareNoCase(strTempName))
			{
				pInitAttr = ptr;
				break;
			}
		}
	} while (false);
	return pInitAttr;
}

void WidgetAttr::PreArrayObjRemove(const DMXmlInitAttrPtr &obj)
{
	delete obj;
}

void WidgetAttr::ReleaseProp()
{
	if (m_pPropFrame)
	{
		m_pPropFrame->m_pPropList->SetCurSel(NULL);// 先设置为NULL,再移除prop list 不然如果prop desc刷新,而此时prop list 不存在了,可能会引发崩溃
		m_pPropFrame->m_pPropList->RemoveAllProp();
		RemoveAll();
	}
}

void WidgetAttr::InitSpecialValue(CStringW& strValue, DMXmlInitAttrPtr pInitAttr)
{
	CStringW strInitValue;// 默认为空
	if (0 == m_strList[0].CompareNoCase(L"DUIListBoxAttr")
		||0 == m_strList[0].CompareNoCase(L"DUIRadioButtonAttr")
		||0 == m_strList[0].CompareNoCase(L"DUISliderCtrlAttr")
		||0 == m_strList[0].CompareNoCase(L"DUIRichEditAttr")
		||0 == m_strList[0].CompareNoCase(L"DUIEditAttr")
		||0 == m_strList[0].CompareNoCase(L"DUIListBoxExAttr")
		||0 == m_strList[0].CompareNoCase(L"DUITabCtrlAttr")
		||0 == m_strList[0].CompareNoCase(L"DUIComboBoxBaseAttr")
		||0 == m_strList[0].CompareNoCase(L"DUIListCtrlExAttr")
		||0 == m_strList[0].CompareNoCase(L"DUITreeCtrlExAttr")
		)
	{ 
		if (0 == _wcsicmp(pInitAttr->m_pAttr->GetName(),L"bool_bfocusable"))
		{
			strInitValue = L"1"; 
		} 
	}
	if (0 == m_strList[0].CompareNoCase(L"DUIEditAttr") && 0 == _wcsicmp(pInitAttr->m_pAttr->GetName(),L"bool_brichtext"))
	{
		strInitValue = L"0";
	}
	if (0 == m_strList[0].CompareNoCase(L"DUILabelAttr") && 0 == _wcsicmp(pInitAttr->m_pAttr->GetName(),L"bool_bmsgnohandle"))
	{
		strInitValue = L"1";
	}   

	if (0 == _wcsicmp(pInitAttr->m_pAttr->GetName(),L"bool_bvisible"))
	{
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		DM::LPTVITEMEX pTV = m_pObjTree->GetItem(m_hObjSel);
		if (DMTVEXEye_UnEye == pTV->iEyeValue)// 当eye隐藏时,pData->m_iOrgVisible记录的才是真实的m_bvisible值
		{
			strInitValue = (DMVISIBLE_SHOW == pData->m_iOrgVisible)?L"1":L"0";
		}
		else
		{
			strInitValue = pData->m_pDUIWnd->DM_IsVisible(false)?L"1":L"0";
		}
	}

	if (!strInitValue.IsEmpty())// 特殊处理后才不会为空
	{
		strValue = strInitValue;
	}
}

DMCode WidgetAttr::s_DMHandleEvent(DM::DMEventArgs *pEvt)
{
	if (ms_pthis&&!ms_pthis->m_bMuted)
	{
		if (DMEventOfPanelArgs::EventID == pEvt->GetEventID())
		{
			DMEventOfPanelArgs* pEvent = (DMEventOfPanelArgs*)pEvt;
			return ms_pthis->DMHandleEvent(pEvent->m_pOrgEvt);
		}
		else  
		{
			return ms_pthis->DMHandleEvent(pEvt);// 转发
		}
	}
 
	return DM_ECODE_FAIL;
}




