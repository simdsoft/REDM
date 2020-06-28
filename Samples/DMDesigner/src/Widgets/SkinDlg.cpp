#include "DMDesignerAfx.h"
#include "SkinDlg.h"

BEGIN_MSG_MAP(SkinDlg)
	MSG_WM_INITDIALOG(OnInitDialog) 
	MSG_WM_SIZE(OnSize)
	MSG_WM_DESTROY(OnDestroy)
	CHAIN_MSG_MAP(DMHDialog)
END_MSG_MAP()
BEGIN_EVENT_MAP(SkinDlg)
	EVENT_NAME_HANDLER(L"ds_skin_bvert",DMEVT_CHECKCHANGED,OnCheckChange)
	EVENT_NAME_HANDLER(L"ds_skin_btitle",DMEVT_CHECKCHANGED,OnCheckChange)
	EVENT_NAME_HANDLER(L"ds_skin_typecbx",DMEVT_CB_SELCHANGE,OnTypeCbxChange)
	EVENT_NAME_HANDLER(L"ds_skin_themecbx",DMEVT_CB_SELCHANGE,OnThemeCbxChange)
	EVENT_NAME_HANDLER(L"ds_skintree",DMEVT_TC_SELCHANGED,OnTreeChange)
	EVENT_NAME_HANDLER(L"ds_skin_id",DMEVT_RENOTIFY,OnEditChange)
	EVENT_NAME_HANDLER(L"ds_skin_state",DMEVT_RENOTIFY,OnEditChange)
	EVENT_NAME_HANDLER(L"ds_skin_margin",DMEVT_DSRECT_CHANGED,OnDSRectChange)
END_EVENT_MAP()
SkinDlg::SkinDlg(bool bEditMode)
{
	m_bEditMode = bEditMode;
}  

SkinDlg::~SkinDlg()
{
	
}
 
BOOL SkinDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	DragAcceptFiles(TRUE);
	m_pTypeCbx	 = FindChildByNameT<DUIComboBox>(L"ds_skin_typecbx");
	m_pThemeCbx  = FindChildByNameT<DUIComboBox>(L"ds_skin_themecbx");
	m_pImg9		 = FindChildByName(L"ds_skin_img9");
	m_pRect		 = FindChildByNameT<DUIRect>(L"ds_skin_margin");
	m_pImgEditor = FindChildByNameT<DUImgEditor>(L"ds_skin_editor");
	m_pVert		 = FindChildByNameT<DUICheckBox>(L"ds_skin_bvert");
	m_pTitle	 = FindChildByNameT<DUICheckBox>(L"ds_skin_btitle");
	m_pId        = FindChildByNameT<DUIEdit>(L"ds_skin_id");
	m_pState     = FindChildByNameT<DUIEdit>(L"ds_skin_state");
	m_pSkinTree  = FindChildByNameT<ProjTree>(L"ds_skintree");
	m_pObjXml    = g_pMainWnd->m_pDesignerXml;
	m_pProjTree  = g_pMainWnd->FindChildByNameT<ProjTree>(L"ds_projtree");
	if (!DMSUCCEEDED(InitThemeCbx()))
	{
		EndDialog(IDCANCEL);
	}

	m_pId->SetEventMask(ENM_CHANGE|m_pId->GetEventMask());
	m_pRect->DM_SendMessage(EM_SETEVENTMASK,0,ENM_CHANGE);
	m_pState->SetEventMask(ENM_CHANGE|m_pState->GetEventMask());

	if (m_bEditMode)// 编辑模式，初始化部分变量
	{
	
		DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_pObjXml->m_hProjSel);
		CStringW strName = pNode->GetName();
		if (0 == strName.CompareNoCase(L"img9"))
		{
			m_pTypeCbx->SetCurSel(1);
		}
		else if (0 == strName.CompareNoCase(L"scrollbar"))
		{
			m_pTypeCbx->SetCurSel(2);
		}
		else 
		{
			m_pTypeCbx->SetCurSel(0);
		}
		// id
		m_pId->SetWindowText(pNode->Attribute(XML_ID));
		//m_pId->SetAttribute(L"breadonly",L"1");// 编辑模式下不允许设置ID

		// bvert
		CStringW strVert = pNode->Attribute(L"bvert");
		bool bVert = false;
		DMAttributeDispatch::ParseBool(strVert,bVert);
		m_pVert->DM_SetCheck(bVert);

		// btitle
		CStringW strTitle = pNode->Attribute(L"btitle");
		bool bTitle = false;
		DMAttributeDispatch::ParseBool(strTitle,bTitle);
		m_pTitle->DM_SetCheck(bTitle);

		// states
		CStringW strStates = pNode->Attribute(L"states");
		m_pState->SetWindowText(strStates);

		// margin
		CStringW strMargin = pNode->Attribute(L"margin");
		CRect rcMargin;
		DMAttributeDispatch::ParseRect(strMargin,rcMargin);
		m_pRect->SetAddress(rcMargin);
	}

	CStringW strName = m_pTypeCbx->GetLBText(m_pTypeCbx->GetCurSel());
	DMXmlNode XmlBase = m_SkinDoc.Base();
	m_SkinNode = XmlBase.InsertChildNode(strName);
	UpdateXml();

	return TRUE;
}

void SkinDlg::OnSize(UINT nType, CSize size)
{
	if (!IsIconic()) 
	{
		CRect rcWnd;
		::GetWindowRect(m_hWnd, &rcWnd);
		::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);  
		HRGN hWndRgn = ::CreateRoundRectRgn(0, 0, rcWnd.right, rcWnd.bottom,4,4);
		SetWindowRgn(hWndRgn, TRUE);
		::DeleteObject(hWndRgn); 
	}               
	SetMsgHandled(FALSE);
}

void SkinDlg::OnDestroy()
{
	ReleaseSkinTree();
	SetMsgHandled(FALSE);
}

DMCode SkinDlg::OnTypeCbxChange(DMEventArgs *pEvt)
{
	DMEventCBSelChangeArgs* pEvent = (DMEventCBSelChangeArgs*)pEvt;
	if (0 == pEvent->m_nCurSel)//imglist
	{
		m_pImg9->SetAttribute(L"bvisible",L"0");
	}
	else
	{
		m_pImg9->SetAttribute(L"bvisible",L"1");
	}  
	UpdateXml();
	return DM_ECODE_OK;
} 


DMCode SkinDlg::OnThemeCbxChange(DMEventArgs *pEvt)
{
	DMCode iErr = DM_ECODE_FAIL;
	DMEventCBSelChangeArgs* pEvent = (DMEventCBSelChangeArgs*)pEvt;
	do 
	{
		ReleaseSkinTree();

		HDMTREEITEM hTheme = m_pThemeCbx->GetItemData(pEvent->m_nCurSel);
		if (NULL == hTheme)
		{
			break;
		}

		CStringW strType, strName;
		if (m_bEditMode) // 编辑模式下，只显示当前图片
		{
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_pObjXml->m_hProjSel);
			CStringW strSrc = pNode->Attribute(XML_SRC);
			CStringWList strList;
			int nCount = (int)SplitStringT(strSrc,L':',strList);
			if (2 == nCount)
			{
				strType = strList[0];
				strName = strList[1];
			}
			else
			{
				strType = L"PNG";
				strName = strList[0];
			}
		}

		DMXmlDocument doc;
		DMXmlNode DataNode = doc.Base();
		m_pObjXml->InitProjTreeNode(DataNode,true);

		DMXmlNodePtr pThemeNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hTheme);
		CStringW strThemeIndexPath = m_pObjXml->m_strResDir + pThemeNode->Attribute(XML_PATH);
		DocDataPtr pDoc = m_pObjXml->FindDocData(strThemeIndexPath);
		if (NULL == pDoc)
		{
			break;
		}

		// 插入xml
		CStringW strText;
		DMXmlNode XmlImg = pDoc->m_XmlRoot.FirstChild();
		while (XmlImg.IsValid())
		{
			strText = XmlImg.GetName();
			HDMTREEITEM hRet0 = InsertSkinTreeItem(DataNode,strText);
			BindSkinTreeData(XmlImg,hRet0);

			// 10.遍历img文件
			DMXmlNode XmlFile = XmlImg.FirstChild(XML_FILE);
			while (XmlFile.IsValid())
			{
				strText = XmlFile.Attribute(XML_PATH);
				strText = strText.Right(strText.GetLength()-strText.ReverseFind(L'\\')-1);
				CStringW strAfx = strText.Right(strText.GetLength()-strText.ReverseFind(L'.'));
				if (0 == strAfx.CompareNoCase(L".bmp")
					||0 == strAfx.CompareNoCase(L".png")
					||0 == strAfx.CompareNoCase(L".jpg")
					||0 == strAfx.CompareNoCase(L".jpeg")
					||0 == strAfx.CompareNoCase(L".gif")
					)
				{
					if (m_bEditMode)// 编辑模式下，只允许插入相匹配的img
					{
						if (0 == strType.CompareNoCase(XmlImg.GetName())
							&&0 == strName.CompareNoCase(XmlFile.Attribute(XML_NAME))
							)
						{
							HDMTREEITEM hRet1 = InsertSkinTreeItem(DataNode,strText,hRet0);
							BindSkinTreeData(XmlFile,hRet1);
							m_pSkinTree->SelectItem(hRet1);
						}
					}
					else
					{
						HDMTREEITEM hRet1 = InsertSkinTreeItem(DataNode,strText,hRet0);
						BindSkinTreeData(XmlFile,hRet1);
					}
				}

				XmlFile = XmlFile.NextSibling(XML_FILE);
			}

			XmlImg = XmlImg.NextSibling();
		}

		iErr = DM_ECODE_OK;
		UpdateXml();
	} while (false);
	return iErr;
}

DMCode SkinDlg::OnTreeChange(DMEventArgs *pEvt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do   
	{
		DMEventTCSelChangedArgs* pEvent = (DMEventTCSelChangedArgs*)pEvt;
		if (NULL == pEvent->m_hNewSel)
		{
			break;
		}
		DMXmlNodePtr pNode = (DMXmlNodePtr)m_pSkinTree->GetItemData(pEvent->m_hNewSel);
		m_pImgEditor->Clear();
		CStringW strImgPath = m_pObjXml->m_strResDir + pNode->Attribute(XML_PATH);
		m_pImgEditor->AddImg(strImgPath);
		UpdateXml();
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode SkinDlg::OnEditChange(DMEventArgs *pEvt)
{
	DMEventRENotifyArgs *pEvent = (DMEventRENotifyArgs*)(pEvt);
	if (EN_CHANGE == pEvent->m_iNotify)
	{
		UpdateXml();
	}

	return DM_ECODE_OK;
}

DMCode SkinDlg::OnCheckChange(DMEventArgs *pEvt)
{
	UpdateXml();
	return DM_ECODE_OK;
} 

DMCode SkinDlg::OnDSRectChange(DMEventArgs *pEvt)
{
	UpdateXml();
	return DM_ECODE_OK;
}

DMCode SkinDlg::ReleaseSkinTree()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		m_pSkinTree->RemoveAllItems();
		DMMapT<HDMTREEITEM,DMXmlNodePtr>::RemoveAll();
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode SkinDlg::OnOK()
{
	DMCode iErr = DM_ECODE_FAIL;
	HDMTREEITEM hAdd = NULL;
	do 
	{
		// 1.判断是否选择皮肤的img
		CStringW strImgSrc = m_SkinNode.Attribute(XML_SRC);
		CStringWList strList;
		int nCount = (int)SplitStringT(strImgSrc,L':',strList);
		if (strImgSrc.IsEmpty()
			||2!=nCount)
		{
			DM_MessageBox(L"请先从skin树形控件中选择皮肤要使用的img!",MB_OK,L"MSG",m_hWnd);
			break;
		}

		// 2.判断是否设置皮肤的id
		CStringW strId = m_pId->GetWindowText();
		strId.Trim();
		if (strId.IsEmpty())
		{
			DM_MessageBox(L"未设置id!",MB_OK,L"MSG",m_hWnd);
			m_pId->DV_SetFocusWnd();
			break;
		}

		// 3.判断是否设置皮肤的状态数
		CStringW strState = m_pState->GetWindowText();
		strState.Trim();
		if (strState.IsEmpty())
		{
			DM_MessageBox(L"未设置状态数!",MB_OK,L"MSG",m_hWnd);
			m_pState->DV_SetFocusWnd();
			break;
		}

		if (m_bEditMode)// 编辑模式
		{
			//4.更新xml结点数据
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_pObjXml->m_hProjSel);

			//5. 判断id是否已存在
			if (0 != strId.CompareNoCase(pNode->Attribute(XML_ID)))
			{
				bool bFind = false;
				HDMTREEITEM hParentItem = m_pProjTree->GetParentItem(m_pObjXml->m_hProjSel);
				if (hParentItem)
				{
					HDMTREEITEM hChildItem = m_pProjTree->GetChildItem(hParentItem);
					while (hChildItem)
					{
						if (hChildItem != m_pObjXml->m_hProjSel)
						{// 跳过编辑自身
							DMXmlNodePtr pChildNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hChildItem);
							if (0 == strId.CompareNoCase(pChildNode->Attribute(XML_ID)))
							{
								bFind = true;
								break;
							}
						}
						hChildItem = m_pProjTree->GetNextSiblingItem(hChildItem);
					}
				}
				if (bFind)
				{
					DM_MessageBox(L"id已存在,请重新设置!",MB_OK,L"MSG",m_hWnd);
					m_pId->DV_SetFocusWnd();
					break;
				}
			}

			CStringW strType = m_SkinNode.GetName();
			CStringW strOldType = pNode->GetName();
			CStringW strOldId = pNode->Attribute(XML_ID);
			if (0 == strState.CompareNoCase(pNode->Attribute(L"states"))
				&& 0 == strType.CompareNoCase(strOldType)
				&& m_SkinNode.AttributeInt(L"bvert")==pNode->AttributeInt(L"bvert")
				&& m_SkinNode.AttributeInt(L"btitle")==pNode->AttributeInt(L"btitle")
				&& 0 == _wcsicmp(m_SkinNode.Attribute(L"margin"),pNode->Attribute(L"margin"))
				&& 0 == strId.CompareNoCase(strOldId)
				)
			{
				// todo.
			}
			else
			{
				pNode->SetAttribute(XML_ID,strId);
				pNode->SetAttribute(L"states",m_SkinNode.Attribute(L"states"));
				pNode->SetName(strType);
				pNode->SetAttribute(L"bvert",m_SkinNode.Attribute(L"bvert"));
				pNode->SetAttribute(L"btitle",m_SkinNode.Attribute(L"btitle"));
				pNode->SetAttribute(L"margin",m_SkinNode.Attribute(L"margin"));
				m_pObjXml->SetDocUnSave(pNode);

				//6. 更新tree结点显示
				DM::LPTVITEMEX pData = m_pProjTree->GetItem(m_pObjXml->m_hProjSel);
				pData->pPanel->m_pDUIXmlInfo->m_strText = pNode->Attribute(XML_ID);
				m_pProjTree->UpdateItemRect(m_pObjXml->m_hProjSel);

				// 对象视图处理模式
				if (m_pObjXml->m_bInitObjTree)
				{
					if (0 == strType.CompareNoCase(strOldType)&&0 == strId.CompareNoCase(strOldId))// 未改变类型,ID
					{
						IDMSkinPtr pSkin = g_pDMApp->GetSkin(pNode->Attribute(XML_SRC));
						if (pSkin)
						{
							pSkin->InitDMData(m_SkinNode);
							m_pObjXml->m_pObjEditor->DM_Invalidate();
						}
					}
					else
					{
						DM_MessageBox(L"skin类型被改变,你必须重新打开工程视图以保证资源正常加载",MB_OK,L"MSG",m_hWnd);
					}
				}
			}
			iErr = DM_ECODE_OK;
			EndDialog(IDOK);
			break;
		}


		//4. 判断id是否已存在
		DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_pObjXml->m_hProjSel);
		DMXmlNode XmlNode = pNode->FirstChild();
		bool bFind = false;
		while (XmlNode.IsValid())
		{
			if (0 == strId.CompareNoCase(XmlNode.Attribute(XML_ID)))
			{
				bFind = true;
				break;
			}
			XmlNode = XmlNode.NextSibling();
		}
		if (bFind)
		{
			DM_MessageBox(L"id已存在,请重新设置!",MB_OK,L"MSG",m_hWnd);
			m_pId->DV_SetFocusWnd();
			break;
		}
	
		//5. 把xml结点插入到xml中
		DMXmlNodePtr pSkinTypeNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_pObjXml->m_hProjSel);
		DMXmlNode XmlSkinNode = pSkinTypeNode->InsertCopyChildNode(&m_SkinNode);
		
		//6. 在tree中加入结点
		DMXmlDocument doc;
		DMXmlNode DataNode = doc.Base();
		m_pObjXml->InitProjTreeNode(DataNode,true);
		hAdd = m_pObjXml->InsertProjTreeItem(DataNode,strId,m_pObjXml->m_hProjSel);
		m_pObjXml->BindProjTreeData(XmlSkinNode,hAdd);

		//7. 设置xml为未保存状态
		m_pObjXml->SetDocUnSave(pSkinTypeNode);

		
		//处理对象视图
		if (hAdd&&m_pObjXml->m_bInitObjTree)// 
		{
			//1. 增加到skin中
			DMXmlDocument Doc;
			DMXmlNode XmlBase = Doc.Base();
			DMXmlNode XmlSkin = XmlBase.InsertChildNode(XML_SKIN);
			XmlSkin.SetAttribute(XML_NAME,pSkinTypeNode->Attribute(XML_NAME));// 父结点name即为所属skin池name
			XmlSkin.InsertCopyChildNode(&XmlSkinNode);
			g_pDMApp->AddSkinPoolItem(XmlSkin);
		}

		if (hAdd)
		{
			m_pProjTree->SelectItem(hAdd);
		}
		EndDialog(IDOK);
		
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
} 
     
DMCode SkinDlg::InitThemeCbx()
{    
	DMCode iErr = DM_ECODE_FAIL;
	do   
	{ 
		if (NULL == m_pObjXml 
			||NULL == m_pObjXml->m_hProjThemes
			||NULL == m_pProjTree
			)
		{
			DM_MessageBox(L"不完整的Res包",MB_OK,L"Error",m_hWnd);
			break;
		}  

		int iChildCount = m_pProjTree->GetChildrenCount(m_pObjXml->m_hProjThemes);
		if (0 == iChildCount)
		{ 
			DM_MessageBox(L"没有主题包\r\n请先创建主题包",MB_OK,L"Error",m_hWnd);
			break;
		}
		DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_pObjXml->m_hProjThemes);
		CStringW strDefThemeName = pNode->Attribute(XML_NAME);
		bool bMatch = false;
		HDMTREEITEM hTheme = m_pProjTree->GetChildItem(m_pObjXml->m_hProjThemes);
		while (hTheme)
		{
			pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hTheme);
			CStringW strThemeName = pNode->Attribute(XML_NAME);
			int iIndex = m_pThemeCbx->InsertItem(-1,strThemeName,-1,-1,(LPARAM)hTheme);
			if (!strDefThemeName.IsEmpty()&&0==strDefThemeName.CompareNoCase(strThemeName))
			{
				m_pThemeCbx->SetCurSel(iIndex);
			} 
			hTheme = m_pProjTree->GetNextSiblingItem(hTheme);
		} 
		if (-1 == m_pThemeCbx->GetCurSel())
		{
			m_pThemeCbx->SetCurSel(0);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr; 
}

DMCode SkinDlg::BindSkinTreeData(DMXmlNode& DataNode,HDMTREEITEM hTreeItem)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (!DataNode.IsValid()||!m_pSkinTree->ItemIsValid(hTreeItem))
		{
			break;
		}
		DMXmlNodePtr pNode = new DMXmlNode;
		if (!AddKey(hTreeItem,pNode))
		{
			break;
		}
		*pNode = DataNode;
		m_pSkinTree->SetItemData(hTreeItem,(LPARAM)pNode);	
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

HDMTREEITEM SkinDlg::InsertSkinTreeItem(DMXmlNode& TreeNode,CStringW strText,HDMTREEITEM hParent /*=DMTVI_ROOT*/)
{
	TreeNode.SetAttribute(XML_TEXT,strText);
	return m_pSkinTree->InsertItem(TreeNode,hParent);
}
 
DMCode SkinDlg::UpdateXml()
{
	DMCode iErr = DM_ECODE_FAIL;
	do  
	{
		CStringW strLBName = m_pTypeCbx->GetLBText(m_pTypeCbx->GetCurSel());
		m_SkinNode.SetName(strLBName);
		CStringW strId = m_pId->GetWindowText();
		m_SkinNode.SetAttribute(XML_ID,strId);

		// 取得src和img text
		CStringW ImgText = L"[未选中img或img无效]";
		m_SkinNode.SetAttribute(XML_SRC,L"");
		HDMTREEITEM hSel = m_pSkinTree->GetSelectedItem();
		if (NULL != hSel && 1 == m_pImgEditor->GetImgCount())// img可以被解析出来
		{
			HDMTREEITEM hParent = m_pSkinTree->GetParentItem(hSel);
			DMXmlNodePtr pParentNode = (DMXmlNodePtr)m_pSkinTree->GetItemData(hParent);
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pSkinTree->GetItemData(hSel);
			CStringW strSrc;
			strSrc.Format(L"%s:%s",pParentNode->GetName(),pNode->Attribute(XML_NAME));
			m_SkinNode.SetAttribute(XML_SRC,strSrc);

			//     
			pNode->GetXmlContent(ImgText);
		}  

		CStringW strStates = m_pState->GetWindowText();
		m_SkinNode.SetAttribute(L"states",strStates);

		CStringW strVert;
		strVert.Format(L"%d",m_pVert->DM_IsChecked());
		m_SkinNode.SetAttribute(L"bvert",strVert);
		CStringW strTitle;
		strTitle.Format(L"%d",m_pTitle->DM_IsChecked());
		m_SkinNode.SetAttribute(L"btitle",strTitle);

		if (m_pImg9->DM_IsVisible())
		{ 
			CRect rc;
			m_pRect->GetAddress(rc);
			CStringW strRc;
			strRc.Format(L"%d,%d,%d,%d",rc.left,rc.top,rc.right,rc.bottom);
			m_SkinNode.SetAttribute(L"margin",strRc);
		}
		else  
		{
			m_SkinNode.RemoveAttribute(L"margin");
		}

		CStringW strXml;
		m_SkinNode.GetXmlContent(strXml);
		FindChildByNameT<DUIEdit>(L"ds_skin_xml")->SetWindowText(strXml);
		FindChildByNameT<DUIEdit>(L"ds_skin_imgpath")->SetWindowText(ImgText);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}


void SkinDlg::PreMapKeyRemove(const DMXmlNodePtr &obj)
{
	delete obj;
}
