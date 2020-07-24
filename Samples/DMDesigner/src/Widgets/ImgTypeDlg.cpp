#include "DMDesignerAfx.h"
#include "ImgTypeDlg.h"

BEGIN_MSG_MAP(ImgTypeDlg)   
	MSG_WM_INITDIALOG(OnInitDialog) 
	MSG_WM_SIZE(OnSize) 
	CHAIN_MSG_MAP(DMHDialog)
END_MSG_MAP() 
ImgTypeDlg::ImgTypeDlg(bool bEditMode,CStringW strName)
{ 
	m_bEditMode = bEditMode;
	m_strName   = strName;
}  
 
BOOL ImgTypeDlg::OnInitDialog( HWND wndFocus, LPARAM lInitParam )
{
	DUIStatic* pTitle = FindChildByNameT<DUIStatic>("ds_title");
	CStringA strTitle;
	if (false == m_bEditMode)
	{
		strTitle = "[";
		ObjXml* pXml = g_pMainWnd->m_pDesignerXml;
		ProjTree* pProjTree = g_pMainWnd->FindChildByNameT<ProjTree>("ds_projtree");
		DMXmlNodePtr pThemeData = (DMXmlNodePtr)pProjTree->GetItemData(pXml->m_hProjSel);
		strTitle += pThemeData->Attribute(XML_NAME);
		strTitle += "]";
	}
	
	if (false == m_bEditMode)
	{
		strTitle += "增加Img类型";
	}
	else
	{
		strTitle += "编辑Img类型";
	}
	pTitle->SetTextA(strTitle);
	DUIEdit* pEdit = FindChildByNameT<DUIEdit>("ds_name");
	if (m_bEditMode)
	{
		pEdit->SetWindowText(m_strName);
	}
	pEdit->DV_SetFocusWnd();
	return TRUE;
}

void ImgTypeDlg::OnSize( UINT nType, CSize size )
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

DMCode ImgTypeDlg::OnOK()
{
	DMCode iErr = DM_ECODE_FAIL;
	HDMTREEITEM hAdd = NULL;
	do 
	{
		ObjXml* pXml = g_pMainWnd->m_pDesignerXml;
		ProjTree* pProjTree  = g_pMainWnd->FindChildByNameT<ProjTree>("ds_projtree");
		if (NULL == pXml||NULL == pProjTree)
		{
			break;
		}
		DUIEdit* pEdit = FindChildByNameT<DUIEdit>("ds_name");
		CStringA strName = pEdit->GetTextA();
		strName.Trim();
		HDMTREEITEM hSel = pXml->m_hProjSel;
		DMXmlNodePtr pNode = (DMXmlNodePtr)pProjTree->GetItemData(hSel);
		if (NULL == pNode)
		{
			break;
		}
		if (strName.IsEmpty())
		{
			DM_MessageBox(L"名字不能为空",MB_OK,L"Error",m_hWnd);
			pEdit->DV_SetFocusWnd();
			break;
		}

		// 编辑模式 ------------------------------
		if (m_bEditMode)
		{
			CStringA strOldName = pNode->GetName();
			if (strOldName != strName)
			{
				// 修改Node的Name
				pNode->SetName(strName);
				DM::LPTVITEMEX pData = pProjTree->GetItem(hSel);
				pData->pPanel->m_pDUIXmlInfo->m_strText = DMCA2W(strName);
				pProjTree->UpdateItemRect(hSel);

				// 将对应xml设置为未保存
				pXml->SetDocUnSave(pNode);
			}

			iErr = DM_ECODE_OK;
			EndDialog(IDOK);
			break;
		}

		// 新建模式 ------------------------------
		CStringW strPath = pXml->m_strResDir + DMCA2W(pNode->Attribute(XML_PATH));
		DocDataPtr pDoc = pXml->FindDocData(strPath);
		if (NULL == pDoc)
		{
			DM_MessageBox(L"此主题包为空,请先扫描错误",MB_OK,L"Error",m_hWnd);
			EndDialog(IDCANCEL);
			break;
		}

		//1.插入xml结点
		DMXmlNode XmlNode = pDoc->m_XmlRoot.InsertChildNode(strName);

		//2.插入tree结点,并绑定数据
		DMXmlDocument doc;
		DMXmlNode DataNode = doc.Base();
		pXml->InitProjTreeNode(DataNode,true);
		hAdd = pXml->InsertProjTreeItem(DataNode,strName,hSel);
		pXml->BindProjTreeData(XmlNode,hAdd);

		//3.将对应xml设置为未保存状态
		pXml->SetDocUnSave(strPath);

		iErr = DM_ECODE_OK;
		EndDialog(IDOK);
		if (hAdd)
		{
			pProjTree->SelectItem(hAdd);
		}
	} while (false);
	return iErr;
}

