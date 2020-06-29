#include "DMDesignerAfx.h"
#include "ObjXml.h"

DesignMenu g_ObjMenuItem[] = \
{
	{OBJMENU_OPENDIR,			L"     打开控件所在xml"},//0 
	{OBJMENU_PASTE,				L"     粘贴"},//1 
	{OBJMENU_COPY,				L"     复制"},//2
	{OBJMENU_LOCKALLCHILD,		L"     锁定所有子控件(一层)"},//3
	{OBJMENU_UNLOCKALLCHILD,	L"     解锁所有子控件(一层)"},//4
	{OBJMENU_DEL,				L"     删除控件"},//5 
};

ObjXml::ObjXml()
{ 
	m_bInitObjTree = false;
	m_pObjEditor  = g_pMainWnd->FindChildByNameT<DUIObjEditor>(L"ds_objeditor");DMASSERT(m_pObjEditor);
	m_pObjTree    = g_pMainWnd->FindChildByNameT<ObjTree>(L"ds_objtree");DMASSERT(m_pObjTree); 
	m_pbPanelCheck= g_pMainWnd->FindChildByNameT<DUICheckBox>(L"ds_w_bpanel");DMASSERT(m_pbPanelCheck); 
	m_pObjTree->m_EventMgr.SubscribeEvent(DMEventTCSelChangedArgs::EventID, Subscriber(&ObjXml::OnObjTreeChanged, this));
	m_hObjSel = NULL;
	m_pRighXml = NULL;
	m_pAddXml  = NULL;
}

ObjXml::~ObjXml()
{
	m_pObjTree->m_EventMgr.UnSubscribeEvent(DMEventTCSelChangedArgs::EventID, Subscriber(&ObjXml::OnObjTreeChanged, this));//必须反注册,不然第二次打开对象视图时，注册会失败
	ReleaseObjTree();
}

DMCode ObjXml::InitObjTree()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		//1.资源包未加载，说明工程视图都未加载
		if (m_pRes.isNull())
		{
			break;
		}

		//2.初始化右边属性区
		m_pRighXml = new RightXml;
		m_pRighXml->InitRightXml();

		//3.更换资源包
		g_pRes->RemoveObj(1);
		g_pRes->AddObj(m_pRes);m_pRes->AddRef();

		g_pDMApp->SetSubXmlDocCallBack(GetExistXmlDoc);
		g_pDMApp->InitGlobal(m_strGlobalName);
		g_pDMApp->SetSubXmlDocCallBack(NULL);

		//4.初始化objeditor
		m_pObjEditor->InitObjEditor();
		
		//5.解析出所有的DM开始的XML,并生成初始树形控件根列表
		InitObjRootTree();

		//6.遍历初始树形控件，创建相对子树和子窗口
		HDMTREEITEM hDMTreeItem = m_pObjTree->GetRootItem();
		while (NULL != hDMTreeItem)
		{
			ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hDMTreeItem);
			DUIRoot* pEditor = m_pObjEditor->InitDesignChild(hDMTreeItem);
			DUIWindowPtr pDUI = pEditor;
			DMXmlNode XmlNode = pData->m_pDoc->m_pXmlDoc->Root().FirstChild(L"root");
			BindObjTreeData(pData->m_pDoc,pEditor,pDUI,XmlNode,hDMTreeItem);// 更新
			// 解析生成子树
			InitObjChildTree(hDMTreeItem);
			m_pObjTree->SetLockState(hDMTreeItem,false);// 默认初始全不锁
			hDMTreeItem = m_pObjTree->GetNextSiblingItem(hDMTreeItem);
		}

		//7.初始化变量
		m_bInitObjTree = true;

		//8.把projtree的私有样式皮肤列表子项全更新
		InitSubXmlList();
  
		//9.初始化Add属性区
		m_pAddXml = new AddXml;
		m_pAddXml->InitAddXml();

		//10.禁用面板选择
		m_pbPanelCheck->DM_EnableWindow(FALSE,true);

		m_pObjTree->SelectItem(m_pObjTree->GetRootItem());
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ObjXml::ReleaseObjTree()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		//0.干掉Add属性区
		if (m_pAddXml)
		{
			m_pAddXml->UnInitAddXml();
			DM_DELETE(m_pAddXml);
		}

		//1.干掉右边属性区
		if (m_pRighXml)
		{
			m_pRighXml->UnInitRightXml();
			DM_DELETE(m_pRighXml);
		}

		//2.干掉预览窗口
		if (m_pPreWnd&&m_pPreWnd->IsWindow())
		{
			m_pPreWnd->DestroyWindow();
		}
		m_pPreWnd.Release();

		//3.反初始化objeditor,同时移除它所有的子项
		if (m_pObjEditor)
		{
			m_pObjEditor->UnInitObjEditor();
		}

		//4.移除全局资源,ds_dmdesigner3432C589-C750-4064-B2A6-B2215B74D221为设计器自身的skin池名
		g_pDMApp->RemoveAllSkinPoolItemExcept(L"ds_dmdesigner3432C589-C750-4064-B2A6-B2215B74D221");
		g_pDMApp->RemoveAllStylePoolItem();
		g_pRes->RemoveObj(1);

		//5. 释放自身资源
		m_pObjTree->RemoveAllItems();
		DMMapT<HDMTREEITEM,ObjTreeDataPtr>::RemoveAll();

		//6.初始化变量
		m_bInitObjTree = false;

		//7.恢复面板选择
		m_pbPanelCheck->DM_EnableWindow(TRUE,true);
		m_hObjSel = NULL;
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ObjXml::ShowPreview()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (false == m_bInitObjTree||NULL == m_hObjSel)
		{
			break;
		}

		HDMTREEITEM hRoot = m_pObjTree->GetRootItem(m_hObjSel);
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hRoot);
		if (pData&&pData->m_pDoc&&pData->m_pDoc->IsValid())
		{
			if (m_pPreWnd&&m_pPreWnd->IsWindow())
			{
				m_pPreWnd->DestroyWindow();
			}
			m_pPreWnd.Release();
			m_pPreWnd.Attach(new PreHWnd());
			m_pPreWnd->ObjCreateWindow(*pData->m_pDoc->m_pXmlDoc);
			m_pPreWnd->CenterWindow();
			m_pPreWnd->ShowWindow(SW_SHOW);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ObjXml::PopObjTreeMenu(HDMTREEITEM hSel)
{
	DMCode iErr = DM_ECODE_FAIL;
	do   
	{ 
		m_hProjSel = hSel;

		DMXmlDocument Doc;
		g_pDMApp->InitDMXmlDocument(Doc, XML_LAYOUT,L"ds_menu_proj");
		DMXmlNode XmlNode = Doc.Root();
		InitCopyObjMenu(XmlNode);
		InitCustomObjMenu(XmlNode);
		

		DUIMenu Menu;
		Menu.LoadMenu(XmlNode); 
		POINT pt;
		GetCursorPos(&pt);
		Menu.TrackPopupMenu(0,pt.x,pt.y,g_pMainWnd->m_hWnd);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ObjXml::HandleObjTreeMenu(int nID)
{
	switch (nID)
	{
	case OBJMENU_OPENDIR:
		{ 
			ObjMenu_OpenDir();
		}
		break;
	case OBJMENU_PASTE:
		{
			ObjMenu_Paste();
		}
		break;
	case OBJMENU_COPY:
		{
			ObjMenu_Copy();
		}
		break;
	case OBJMENU_LOCKALLCHILD:
		{
			ObjMenu_LockAllChild(true);
		}
		break;
	case OBJMENU_UNLOCKALLCHILD:
		{
			ObjMenu_LockAllChild(false);
		}
		break;
	case OBJMENU_DEL:
		{
			ObjMenu_Del();
		}
		break;
	default:
		break;
	}
	return DM_ECODE_OK;
}

DMCode ObjXml::OnObjTreeChanged(DMEventArgs* pEvt)
{
	DMEventTCSelChangedArgs* pEvent = (DMEventTCSelChangedArgs*)pEvt;
	if (m_hObjSel != pEvent->m_hNewSel)
	{
		//干掉预览窗口
		if (m_pPreWnd&&m_pPreWnd->IsWindow())
		{
			m_pPreWnd->DestroyWindow();
		}
		m_pPreWnd.Release();
		m_hObjSel = pEvent->m_hNewSel;
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (pData&&pData->IsValid())
		{
			m_pObjEditor->ShowDesignChild(pData->m_pRootWnd);
			m_pObjEditor->SelFrameInAllMode();
			m_pObjEditor->DragFrameInSelMode();
		}
	}
	return DM_ECODE_OK;
}

DMCode ObjXml::InitObjTreeNode(DUIWindow*pWnd, DMXmlNode& TreeNode,bool IsDMXml)
{
	TreeNode = TreeNode.InsertChildNode(XML_TREEITEM);
	TreeNode.SetAttribute(XML_CHILDOFFSET,L"0");
	DMXmlNode StaticNode = TreeNode.InsertChildNode(XML_STATIC);
	StaticNode.SetAttribute(XML_POS,L"3,0,-36,-0");
	StaticNode.SetAttribute(XML_SKIN,IsDMXml? XML_DEFSKIN:XML_DATASKIN);
	StaticNode.SetAttribute(XML_CLRTEXT,L"pbgra(ff,ff,ff,ff)");

	if (!IsDMXml && !IsDeletable(pWnd))//规避主窗口和panel窗口
	{
		StaticNode.SetAttribute(XML_SKIN,XML_CUSTOMSKIN);
	}

	Init_Debug_XmlBuf(TreeNode);

	return DM_ECODE_OK;
}

DMCode ObjXml::BindObjTreeData(DocDataPtr pDoc,DUIRoot* pEditor,DUIWindowPtr pDUI,DMXmlNode& Node,HDMTREEITEM hTreeItem,bool bPanel)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (!m_pObjTree->ItemIsValid(hTreeItem))
		{
			break;
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hTreeItem);
		if (pData)// 更新数据
		{
			pData->SetData(pDoc,pEditor,pDUI,Node,bPanel);
		}
		else// 新增绑定
		{
			pData = new ObjTreeData(pDoc,pEditor,pDUI,Node,bPanel);
			if (!DMMapT<HDMTREEITEM,ObjTreeDataPtr>::AddKey(hTreeItem,pData))
			{
				DM_MessageBox(L"此data已加入过!");
				break;
			}
			m_pObjTree->SetItemData(hTreeItem,(LPARAM)pData);	
		}
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}  

HDMTREEITEM ObjXml::InsertObjTreeItem(DMXmlNode& TreeNode,CStringW strText,HDMTREEITEM hParent /*=DMTVI_ROOT*/)
{
	DMXmlNode StaticNode = TreeNode.FirstChild(XML_STATIC);
	DMASSERT(StaticNode.IsValid());
	StaticNode.SetAttribute(XML_TEXT,strText);
	return m_pObjTree->InsertItem(TreeNode,hParent);
}

HDMTREEITEM ObjXml::FindStyle(CStringW strStyle)
{
	HDMTREEITEM hRet = NULL;
	do 
	{
		if (strStyle.IsEmpty())
		{
			break;
		}
		//1.拆分style
		CStringWList strList;
		CStringW strName;
		CStringW strKey;
		int nCount = (int)SplitStringT(strStyle,L':',strList);
		if (1==nCount)
		{
			strKey = strStyle;
		}
		else if (2 == nCount)
		{
			strName = strList[0];
			strKey  = strList[1];
		}
		else
		{
			CStringW strInfo;
			strInfo.Format(L"style-%s设置错误",strStyle);
			DM_MessageBox(strInfo);
			break;
		}

		//2.从全局的global.xml中查找
		if (m_hProjGlobalStyles)
		{
			hRet = _FindStyle(m_hProjGlobalStyles,strName,strKey);
			if (hRet)
			{
				break;
			}
		}
		
		//3.从自身所在的根结点的dmxml中查找
		if (NULL == m_pObjEditor->m_pShow)
		{
			break;
		}
		ObjTreeDataPtr pRootData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_pObjEditor->m_pShow->m_hRoot);
		CStringW strRootXmlPath = pRootData->m_pDoc->m_strXmlPath;
		HDMTREEITEM hDM = m_pProjTree->GetChildItem(m_hProjPrivStyleSkins);
		while (hDM)
		{
			DMXmlNodePtr pDMNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hDM);
			CStringW strDMPath = m_strResDir + pDMNode->Attribute(XML_PATH);
			if (0 == strRootXmlPath.CompareNoCase(strDMPath))
			{
				HDMTREEITEM hChild = m_pProjTree->GetChildItem(hDM);
				while (hChild)
				{
					CStringW strText = GetProjTreeItemText(hChild);
					if (0 == strText.CompareNoCase(PROJTREE_PRIVSTYLES))
					{// 找到PROJTREE_PRIVSTYLES所在的节点了
						break;
					}
					hChild = m_pProjTree->GetNextSiblingItem(hChild);
				}
				if (hChild)
				{
					hRet = _FindStyle(hChild,strName,strKey);
					if (hRet)
					{
						break;
					}
				}
			}
			hDM = m_pProjTree->GetNextSiblingItem(hDM);
		}
	} while (false);
	return hRet;
}


DMCode ObjXml::InitCopyObjMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMXmlNode XmlPaste = XmlNode.InsertChildNode(XML_ITEM);
		XmlPaste.SetAttribute(XML_ID,IntToString(g_ObjMenuItem[OBJMENU_PASTE-OBJMENU_BASE].id));XmlPaste.SetAttribute(XML_TEXT,g_ObjMenuItem[OBJMENU_PASTE-OBJMENU_BASE].text);
		XmlPaste.SetAttributeInt(XML_BDISABLE,g_pAttr->m_CopyNode.IsValid()?0:1);

		DMXmlNode XmlCopy = XmlNode.InsertChildNode(XML_ITEM);
		XmlCopy.SetAttribute(XML_ID,IntToString(g_ObjMenuItem[OBJMENU_COPY-OBJMENU_BASE].id));XmlCopy.SetAttribute(XML_TEXT,g_ObjMenuItem[OBJMENU_COPY-OBJMENU_BASE].text);
		XmlNode.InsertChildNode(XML_SEP);

		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (NULL == pData||!pData->IsValid()||0 == m_pObjTree->GetParentItem(m_hObjSel))
		{// DM根结点不使用复制,粘贴菜单
			XmlPaste.SetAttributeInt(XML_BDISABLE,1);
			XmlCopy.SetAttributeInt(XML_BDISABLE,1);
			break;
		}
		if (MoveMode == m_pObjEditor->m_DesignMod)
		{// move模式下不支持
			XmlPaste.SetAttributeInt(XML_BDISABLE,1);
			XmlCopy.SetAttributeInt(XML_BDISABLE,1);
			break;
		}
		if (pData->m_bPanel)
		{// panel下不支持
			XmlPaste.SetAttributeInt(XML_BDISABLE,1);
			XmlCopy.SetAttributeInt(XML_BDISABLE,1);
			break;
		}

		if (pData->m_pRootWnd == pData->m_pDUIWnd)
		{// root层也不使用复制菜单,但可以粘贴
			XmlCopy.SetAttributeInt(XML_BDISABLE,1);
			break;
		}
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ObjXml::InitCustomObjMenu(DMXmlNode& XmlNode)
{
	DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
	XmlItem.SetAttribute(XML_ID,IntToString(g_ObjMenuItem[OBJMENU_OPENDIR-OBJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ObjMenuItem[OBJMENU_OPENDIR-OBJMENU_BASE].text);

	XmlNode.InsertChildNode(XML_SEP);
	XmlItem = XmlNode.InsertChildNode(XML_ITEM);
	XmlItem.SetAttribute(XML_ID,IntToString(g_ObjMenuItem[OBJMENU_LOCKALLCHILD-OBJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ObjMenuItem[OBJMENU_LOCKALLCHILD-OBJMENU_BASE].text);
	XmlItem = XmlNode.InsertChildNode(XML_ITEM);
	XmlItem.SetAttribute(XML_ID,IntToString(g_ObjMenuItem[OBJMENU_UNLOCKALLCHILD-OBJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ObjMenuItem[OBJMENU_UNLOCKALLCHILD-OBJMENU_BASE].text);
	
	if (NULL != m_hObjSel)
	{
		XmlNode.InsertChildNode(XML_SEP);
		XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_ObjMenuItem[OBJMENU_DEL-OBJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ObjMenuItem[OBJMENU_DEL-OBJMENU_BASE].text);
		DM::LPTVITEMEX pTviData = m_pObjTree->GetItem(m_hObjSel);
		if (DMTVEXLock_UnLocked == pTviData->iLockValue &&0 != m_pObjTree->GetParentItem(m_hObjSel)&&0 != m_pObjTree->GetParentItem(m_pObjTree->GetParentItem(m_hObjSel)))
		{
			XmlItem.SetAttributeInt(XML_BDISABLE,0);
		}
		else
		{
			XmlItem.SetAttributeInt(XML_BDISABLE,1);
		}

		// 特殊子控件不允许被删除
		ObjTreeDataPtr pData = (ObjTreeDataPtr)pTviData->lParam;
		if (NULL != pData&&pData->IsValid()&&NULL != pData->m_pDUIWnd)
		{
			if (!IsDeletable(pData->m_pDUIWnd))
			{
				XmlItem.SetAttributeInt(XML_BDISABLE,1);
			}
		}
	}
	
	Init_Debug_XmlBuf(XmlNode);

	return DM_ECODE_OK;
}

DMCode ObjXml::ObjMenu_OpenDir()
{
	CStringW strPath		  = m_strResDir;
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_pObjTree||NULL == m_hObjSel)
		{
			break;
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (NULL == pData||!pData->IsValid()||NULL == pData->m_pDUIWnd)
		{
			break;
		}
		strPath = pData->m_pDoc->m_strXmlPath;
		iErr = DM_ECODE_OK;
	} while (false);
	OpenFolderAndSelectFile(strPath);
	return iErr;
}

DMCode ObjXml::ObjMenu_Paste()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_pObjTree||NULL == m_hObjSel)
		{
			break;
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (NULL == pData||!pData->IsValid()||0 == m_pObjTree->GetParentItem(m_hObjSel)||!g_pAttr->m_CopyNode.IsValid())
		{
			break;
		}
		DM::LPTVITEMEX pTviData = m_pObjTree->GetItem(m_hObjSel);
		if (DMTVEXLock_UnLocked != pTviData->iLockValue)
		{
			SetLogInfo(L"不能在锁定状态下粘贴");
			break;
		}

		DUIWindow* pParentWnd = pData->m_pDUIWnd;
		//2. 判断窗口是否支持插入子窗口
		CStringW strReg = g_pAttr->m_CopyNode.GetName();
		if (!DUIRoot::IsSupportAddChild(pParentWnd,strReg))
		{ 
			break;
		}
		//3. 创建新窗口
		DUIWindow* pChild = DUIRoot::CreateAddChild(pParentWnd,strReg);
		if (NULL == pChild)
		{
			break;
		}

		//4. 初始化通用属性
		POINT pt;
		GetCursorPos(&pt);
		::ScreenToClient(g_pMainWnd->m_hWnd,&pt);
		CRect rcParent;
		pParentWnd->DV_GetChildMeasureLayout(rcParent);
		CPoint ptFirst = rcParent.TopLeft();
		if (rcParent.PtInRect(pt))// 鼠标在窗口内，说明是窗口内右键粘贴
		{
			ptFirst = pt;
		}
		CRect rcDrag(ptFirst,rcParent.BottomRight());
		if (rcDrag.Width()>g_pAttr->m_rcCopy.Width())
		{
			rcDrag.right = rcDrag.left + g_pAttr->m_rcCopy.Width();
		}
		if (rcDrag.Height()>g_pAttr->m_rcCopy.Height())
		{
			rcDrag.bottom = rcDrag.top + g_pAttr->m_rcCopy.Height();
		}
		if (!DUIRoot::InitAddChild(pData,pChild,rcDrag,true))
		{
			break;
		}

		//5.枚举加入
		EnumChildTreeItem(pData->m_pRootWnd,pChild,m_hObjSel);

		//6.选中最后一个子项,即新加入的子项
		pParentWnd->DV_UpdateChildLayout();
		m_pObjTree->SelectItem(m_pObjTree->GetChildItem(m_hObjSel,false));
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ObjXml::ObjMenu_Copy()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_pObjTree||NULL == m_hObjSel)
		{
			break;
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (NULL == pData||!pData->IsValid()||NULL == pData->m_pDUIWnd||!pData->m_pXmlNode->IsValid())
		{
			break;
		}
		g_pAttr->SetCopyNode(pData->m_pXmlNode,pData->m_pDUIWnd->m_rcWindow);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ObjXml::ObjMenu_LockAllChild(bool bLock)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_pObjTree||NULL == m_hObjSel)
		{
			break;
		}

		HDMTREEITEM hChild = m_pObjTree->GetChildItem(m_hObjSel);
		while (hChild)
		{
			m_pObjTree->SetLockState(hChild,bLock);
			hChild = m_pObjTree->GetNextSiblingItem(hChild);
		}	

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}


DMCode ObjXml::ObjMenu_Del()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_pObjTree||NULL == m_hObjSel)
		{
			break;
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (NULL == pData||!pData->IsValid()||NULL == pData->m_pDUIWnd)
		{
			break;
		}

		CStringW strInfo;
		strInfo.Format(L"你确认删除[%s][%s]控件?此操作不可恢复",pData->m_pDUIWnd->V_GetClassName(),pData->m_pDUIWnd->GetName());
		if (IDOK != DM_MessageBox(strInfo,MB_OKCANCEL))
		{
			break;
		}

		// 1.找到父控件
		DUIWindowPtr pParent = NULL;
		if (pData->m_bPanel)
		{
			pParent = pData->m_pDUIWnd->DM_GetWindow(GDW_PANELPARENT);
		}
		else
		{
			pParent = pData->m_pDUIWnd->DM_GetWindow(GDW_PARENT);
		}
		if (NULL == pParent)
		{
			break;
		}

		// 2.从父控件中移除子控件,特殊情况特殊处理
		if (0 == _wcsicmp(pParent->V_GetClassName(),DUITabCtrl::GetClassName()))
		{
			DUITabCtrl* pTabCtrl = dynamic_cast<DUITabCtrl*>(pParent);
			pTabCtrl->RemoveItem(pData->m_pDUIWnd);
		}
		else if (pData->m_bPanel)
		{
			DUIItemPanel* pPanel = dynamic_cast<DUIItemPanel*>(pData->m_pDUIWnd);
			if (0 == _wcsicmp(pParent->V_GetClassName(),DUIListBoxEx::GetClassName()))
			{
				DUIListBoxEx* pListBoxEx = dynamic_cast<DUIListBoxEx*>(pParent);
				pListBoxEx->DeleteItem((int)pPanel->m_dwItemId);
			}
			if (0 == _wcsicmp(pParent->V_GetClassName(),DUIListCtrlEx::GetClassName()))
			{
				DUIListCtrlEx* pListCtrlEx = dynamic_cast<DUIListCtrlEx*>(pParent);
				pListCtrlEx->DeleteItem((int)pPanel->m_dwItemId);
			}
			else
			{
				SetLogInfo(L"不支持删除");
				break;// 其余的不支持
			}
		}
		else
		{
			pParent->DM_DestroyChildWnd(pData->m_pDUIWnd);
		}

		// 3.从xml中移除此node
		HDMTREEITEM hTreeParent = m_pObjTree->GetParentItem(m_hObjSel);
		ObjTreeDataPtr pParentData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hTreeParent);
		if (pParentData->m_pDoc != pData->m_pDoc)
		{// 子控件为sub列表的最上层控件，直接从sub中移除即可
			pData->m_pDoc->m_pXmlDoc->Root().RemoveChildNode(pData->m_pXmlNode);
		}
		else
		{
			pParentData->m_pXmlNode->RemoveChildNode(pData->m_pXmlNode);
		}
		pData->m_pDoc->m_bChange = true;

		// 4.从树形控件中移除此tree,并移除map
		RemoveObjTreeItemMap(m_hObjSel);
		m_pObjTree->RemoveItem(m_hObjSel);

		// 选中父窗口
		m_pObjTree->SelectItem(hTreeParent);
		
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

// private
DMCode ObjXml::InitObjRootTree()
{
	DMCode iErr = DM_ECODE_FAIL;
	
	DMSmartPtrT<ResFolderItem> pLayout = m_pRes->m_pLayout;
	int nCount = (int)pLayout->GetCount();
	for (int i=0; i<nCount; i++)
	{
		//1.查找以dm为根结点的xml（只有以dm为根结点的才能创建窗口）
		ResItemPtr pItem = pLayout->GetObj(i);
		unsigned long dwSize = GetFileSizeW(pItem->m_szPath);
		if (dwSize<=0)
		{
			continue;
		}
		DMBufT<byte>pBuf;pBuf.Allocate(dwSize);
		DWORD dwRead = 0;
		if (false == GetFileBufW(pItem->m_szPath,(void**)&pBuf,dwSize,dwRead))
		{
			continue;
		}

		DMXmlDocument doc;
		if (false == doc.LoadFromBuffer(pBuf, dwSize))
		{
			continue;
		}
		DMXmlNode XmlNode = doc.Root(XML_DM);
		if (!XmlNode.IsValid())
		{
			continue;
		}

		//2.从projxml中尝试获取xml，如果没有，就加入
		DocDataPtr pDoc = FindDocData(pItem->m_szPath);
		if (NULL == pDoc)
		{
			pDoc = new DocData(pItem->m_szPath);
			if (!pDoc->IsValid())
			{
				DM_DELETE(pDoc);
			}
			AddObj(pDoc);
		}
		if (NULL == pDoc)
		{
			continue;
		}

		//3.尝试插入到树形控件中
		DMXmlDocument TreeDoc;
		DMXmlNode TreeNode = TreeDoc.Base();
		InitObjTreeNode(NULL,TreeNode,true);
		HDMTREEITEM hDMTree  = InsertObjTreeItem(TreeNode,pItem->m_szName);

		//4.绑定data
		DMXmlNode TempNode;
		BindObjTreeData(pDoc,NULL, NULL,TempNode,hDMTree);
		iErr = DM_ECODE_OK;
	}
	return iErr;
}

DMCode ObjXml::InitObjChildTree(HDMTREEITEM hRootItem)
{
	ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hRootItem);
	if (pData&&pData->IsValid()&&pData->m_pRootWnd)
	{
		EnumChildTreeItem(pData->m_pRootWnd,pData->m_pDUIWnd,hRootItem);
	}
	return DM_ECODE_OK;
}

DMCode ObjXml::EnumChildTreeItem(DUIRoot*pMainWnd, DUIWindow* pWnd, HDMTREEITEM hTreeItem,bool bPanel)
{ 
	do     
	{ 
		if (NULL == pWnd)
		{
			break;
		}

		//1.获得XmlNode位于哪个pDoc中
		DocDataPtr pDoc = FindDocData(&pWnd->m_XmlNode);
		if (NULL == pDoc)//过滤掉那些insert进来的child，但调用了InitDMData，如DUIPAddressCtrl的子项IPEdit
		{
			break;
		}

		//2. 插入子结点并绑定数据
		DMXmlDocument TreeDoc;
		DMXmlNode TreeNode = TreeDoc.Base();
		InitObjTreeNode(pWnd,TreeNode,bPanel);
		CStringW strText = pWnd->V_GetClassName();
		if (bPanel)
		{
			strText = L"ItemPanel";
		}
		CStringW strName = pWnd->GetName();
		if (!strName.IsEmpty())
		{
			strText.Format(L"%s[%s]",strText,strName);
		}
		HDMTREEITEM hChild = InsertObjTreeItem(TreeNode,strText,hTreeItem);
		BindObjTreeData(pDoc,pMainWnd,pWnd,pWnd->m_XmlNode,hChild,bPanel);

		//3. 子结点递归
		DUIWindow* pChild = pWnd->DM_GetWindow(GDW_FIRSTCHILD);
		while (pChild)
		{
			if (pChild->m_XmlNode.IsValid())//1. 过滤掉那些insert进来的child，并且不调用InitDMData的,规避在insert进来的child之后有xml创建的窗口
			{
				EnumChildTreeItem(pMainWnd,pChild,hChild,false);// 进入下一层
			}
			pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
		}
		if (m_pbPanelCheck->DM_IsChecked())
		{
			EnumPanelTreeItem(pMainWnd,pWnd,hChild);
		}
	} while (false);
	return DM_ECODE_OK;
}

DMCode ObjXml::EnumPanelTreeItem(DUIRoot*pMainWnd, DUIWindow* pWnd, HDMTREEITEM hTreeItem)
{
	do 
	{
		if (NULL == pWnd)
		{
			break;
		}

		//1.获得XmlNode位于哪个pDoc中
		DocDataPtr pDoc = FindDocData(&pWnd->m_XmlNode);
		if (NULL == pDoc)//过滤掉那些insert进来的child，但调用了InitDMData，如DUIPAddressCtrl的子项IPEdit
		{
			break;
		}

		int count = (int)pWnd->m_ChildPanelArray.GetCount();
		for (int i=0; i<count; i++)
		{
			DUIWindowPtr pPanel = pWnd->m_ChildPanelArray[i];
			if (pPanel)
			{
				EnumChildTreeItem(pMainWnd,pPanel,hTreeItem,true);
			}
		}

	} while (false);
	return DM_ECODE_OK;
}

DMCode ObjXml::RemoveObjTreeItemMap(HDMTREEITEM hTreeItem)
{
	if (NULL != hTreeItem)
	{
		DMMapT<HDMTREEITEM,ObjTreeDataPtr>::RemoveKey(hTreeItem);
		HDMTREEITEM hChild = m_pObjTree->GetChildItem(hTreeItem);
		while (hChild)
		{
			RemoveObjTreeItemMap(hChild);
			hChild = m_pObjTree->GetNextSiblingItem(hChild);
		}
	}
	return DM_ECODE_OK;
}

bool ObjXml::IsDeletable(DUIWindow* pWnd)
{
	bool bRet = false;
	do 
	{
		if (NULL == pWnd)
		{
			break;
		}

		CStringW strClassName = pWnd->V_GetClassName();
		bool bPanel = (0 == _wcsicmp(strClassName, L"ItemPanel"));

		// 1.找到父控件
		DUIWindowPtr pParent = bPanel? pWnd->DM_GetWindow(GDW_PANELPARENT):pWnd->DM_GetWindow(GDW_PARENT);
		if (NULL == pParent)
		{
			break;
		}
		
		CStringW strParentClassName = pParent->V_GetClassName();

		if (0 == _wcsicmp(strClassName,DUIHeaderCtrl::GetClassName())
			&& (0 == _wcsicmp(strParentClassName,DUIListCtrlEx::GetClassName())))
		{
			SetLogInfo(L"ListCtrl的DUIHeaderCtrl不能被删除");
			break;
		}

		if (0 == _wcsicmp(strParentClassName,DUISplitLayout::GetClassName()))
		{
			SetLogInfo(L"DUISplitLayout的子控件不能被删除");
			break;
		}

		if (0 == _wcsicmp(strClassName,DUIScrollFL::GetClassName())
			&& (0 == _wcsicmp(strParentClassName,DUIScrollWnd::GetClassName())))
		{
			SetLogInfo(L"ScrollWnd的ScrollFL不能被删除");
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}

HDMTREEITEM ObjXml::_FindStyle(HDMTREEITEM hStylePoolParent,CStringW strName,CStringW strKey)
{
	HDMTREEITEM hRet = NULL;
	HDMTREEITEM hStylePool = m_pProjTree->GetChildItem(hStylePoolParent);
	while (hStylePool)
	{
		DMXmlNodePtr pStylePoolNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hStylePool);
		if (pStylePoolNode)
		{
			if (strName.IsEmpty()||0 == strName.CompareNoCase(pStylePoolNode->Attribute(XML_NAME)))
			{// 未指定style池名或style池名匹配
				HDMTREEITEM hStyle =  m_pProjTree->GetChildItem(hStylePool);
				while (hStyle)
				{
					DMXmlNodePtr pStyleNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hStyle);
					if (pStyleNode && 0 == strKey.CompareNoCase(pStyleNode->Attribute(XML_ID)))
					{
						hRet = hStyle;
						break;// 仅跳出内层while
					}
					hStyle = m_pProjTree->GetNextSiblingItem(hStyle);
				}
			}
			if (hRet)// 再次跳出while
			{
				break;
			}
		}
		hStylePool = m_pProjTree->GetNextSiblingItem(hStylePool);
	}
	return hRet;
}

void ObjXml::PreMapKeyRemove(const ObjTreeDataPtr &obj)
{
	delete obj;
}






