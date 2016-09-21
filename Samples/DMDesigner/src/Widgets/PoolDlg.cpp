#include "DMDesignerAfx.h"
#include "PoolDlg.h"

BEGIN_MSG_MAP(PoolDlg)   
	MSG_WM_INITDIALOG(OnInitDialog) 
	MSG_WM_SIZE(OnSize) 
	CHAIN_MSG_MAP(DMHDialog)
END_MSG_MAP() 

PoolDlg::PoolDlg(bool bSkinPool,bool bEditMode,CStringW strName)
{
	m_bSkinPool = bSkinPool;
	m_bEditMode = bEditMode;
	m_strName   = strName;
}

BOOL PoolDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam )
{
	DUIStatic* pTitle = FindChildByNameT<DUIStatic>(L"ds_title");
	CStringW strTitle;
	if (false == m_bEditMode)
	{
		strTitle = L"增加";
	}
	else
	{
		strTitle = L"编辑";
	}

	if (false == m_bSkinPool)
	{
		strTitle += L"样式池";
	}
	else
	{
		strTitle += L"皮肤池";
	}
	pTitle->DV_SetWindowText(strTitle);
	DUIEdit* pEdit = FindChildByNameT<DUIEdit>(L"ds_name");
	if (m_bEditMode)
	{
		pEdit->SetWindowText(m_strName);
	}
	pEdit->DV_SetFocusWnd();
	return TRUE;
}

void PoolDlg::OnSize(UINT nType, CSize size)
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

DMCode PoolDlg::OnOK()
{
	DMCode iErr = DM_ECODE_FAIL;
	HDMTREEITEM hAdd = NULL;
	do 
	{
		ObjXml* pXml = g_pMainWnd->m_pDesignerXml;
		ProjTree* pProjTree  = g_pMainWnd->FindChildByNameT<ProjTree>(L"ds_projtree");
		if (NULL == pXml||NULL == pProjTree)
		{
			break;
		}
		DUIEdit* pEdit = FindChildByNameT<DUIEdit>(L"ds_name");
		CStringW strName = pEdit->GetWindowText();
		strName.Trim();
		HDMTREEITEM hSel = pXml->m_hProjSel;
		DMXmlNodePtr pNode = (DMXmlNodePtr)pProjTree->GetItemData(hSel);
		if (NULL == pNode)
		{
			break;
		}
		// 编辑模式 ------------------------------
		if (m_bEditMode)
		{
			CStringW strOldName = pNode->Attribute(XML_NAME);
			//1. 取得原始名字,并与当前名字比较
			if (strOldName != strName)
			{
				//2. 修改Node的Name
				pNode->SetAttribute(XML_NAME,strName);

				//3. 修改tree的显示Name
				if (strName.IsEmpty())
				{
					strName = m_bSkinPool? XML_SKIN:XML_STYLE;
				}
				DM::LPTVITEMEX pData = pProjTree->GetItem(hSel);
				pData->pPanel->m_pDUIXmlInfo->m_strText = strName;
				pProjTree->UpdateItemRect(hSel);

				//4. 将对应xml设置为未保存状态
				pXml->SetDocUnSave(pNode);
			}

			iErr = DM_ECODE_OK;
			EndDialog(IDOK);
			break;
		}


		// 新建模式 ------------------------------
		if (strName.IsEmpty())
		{
			if (IDCANCEL == DM_MessageBox(L"名字为空,是否继续？",MB_OKCANCEL,L"MSG",m_hWnd))
			{
				break;
			}
		}
	
		// 1.插入XML结点
		DMXmlNode XmlNode = pNode->InsertChildNode(m_bSkinPool?L"skin":L"style");
		if (!strName.IsEmpty())
		{
			XmlNode.SetAttribute(XML_NAME,strName);
		}
		
		// 2.插入tree结点
		DMXmlDocument doc;
		DMXmlNode DataNode = doc.Base();
		pXml->InitProjTreeNode(DataNode,true);
		if (strName.IsEmpty())
		{
			strName = m_bSkinPool? XML_SKIN:XML_STYLE;
		}
		hAdd = pXml->InsertProjTreeItem(DataNode,strName,hSel);
		pXml->BindProjTreeData(XmlNode,hAdd);

		// 3.将对应xml设置为未保存
		pXml->SetDocUnSave(pNode);

		iErr = DM_ECODE_OK;
		EndDialog(IDOK);
		if (hAdd)
		{
			pProjTree->SelectItem(hAdd);
		}
	} while (false);
	return iErr;
}
