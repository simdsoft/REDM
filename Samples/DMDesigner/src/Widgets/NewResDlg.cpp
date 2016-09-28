#include "DMDesignerAfx.h"
#include "NewResDlg.h"
DUIRecentListBox::DUIRecentListBox()
{
	DMADDEVENT(DUIRecentListBoxDBLClickArgs::EventID);
}

void DUIRecentListBox::OnLButtonDbClick(UINT nFlags,CPoint pt)
{
	DUIListBox::OnLButtonDown(nFlags,pt);
	if (-1 != m_iSelItem)
	{
		CStringW strDir = m_DMArray[m_iSelItem]->strText;
		if (!IsDirectoryExist(strDir))
		{
			DM_MessageBox(L"该路径已无效!",MB_OK,L"MSG",GetContainer()->OnGetHWnd());
			DeleteString(m_iSelItem);
			g_pAttr->RemoveRecentResDir(strDir);
		}
		else
		{
			DUIRecentListBoxDBLClickArgs Evt(this);
			Evt.m_strDir = strDir;
			DV_FireEvent(Evt);
		}
	}
}

BEGIN_MSG_MAP(NewResDlg)
	MSG_WM_INITDIALOG(OnInitDialog) 
	CHAIN_MSG_MAP(DMHDialog)
END_MSG_MAP()
BEGIN_EVENT_MAP(NewResDlg)
	EVENT_NAME_COMMAND(L"ds_openrespath",OnOpenResPath)
	EVENT_NAME_HANDLER(L"ds_recentlist",DMEVT_DRLISTBOX_DBLCLICK,OnOpenRecentDir)
END_EVENT_MAP()

BOOL NewResDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_pResNameEdit = FindChildByNameT<DUIEdit>(L"ds_resnameedit");DMASSERT(m_pResNameEdit);
	m_pResPathEdit = FindChildByNameT<DUIEdit>(L"ds_respathedit");DMASSERT(m_pResPathEdit);
	m_pRecentList  = FindChildByNameT<DUIListBox>(L"ds_recentlist");DMASSERT(m_pRecentList);
	m_pResNameEdit->DV_SetFocusWnd();

	// 更新最近list列表
	DMXmlNode XmlRoot = g_pAttr->m_RecentDoc.Root();
	if (XmlRoot.IsValid())
	{
		DMXmlNode XmlNode = XmlRoot.FirstChild();
		while (XmlNode.IsValid())
		{
			CStringW strDir = XmlNode.Attribute(XML_PATH);
			if (!strDir.IsEmpty())
			{
				m_pRecentList->AddString(strDir);
			}
	
			XmlNode = XmlNode.NextSibling();
		}
	}

	return TRUE;
}

DMCode NewResDlg::OnOpenResPath()
{ 
	wchar_t path[MAX_PATH] = {0};
	BROWSEINFOW bi = {0};
	bi.ulFlags = BIF_STATUSTEXT| BIF_RETURNONLYFSDIRS|BIF_VALIDATE;
	bi.lpszTitle = L"请选择DM资源包的存放位置（目前仅支持文件夹方式）";
	bi.hwndOwner = m_hWnd;//设置拥有窗口
	bi.lpfn = NULL;	//指定回调函数地址
	ITEMIDLIST *pIDL = SHBrowseForFolderW(&bi);
	if (pIDL!=NULL)
	{
		if (SHGetPathFromIDListW(pIDL,path) == TRUE) //变量path中存储了经过用户选择后的目录的完整路径.
		{
			m_pResPathEdit->SetWindowText(path);
		}
		// free memory used   
		IMalloc * imalloc = 0;   
		if (SUCCEEDED(SHGetMalloc( &imalloc)))   
		{   
			imalloc->Free(pIDL);   
			imalloc->Release();   
		}   
	}
	return DM_ECODE_OK;
}

DMCode NewResDlg::OnOpenRecentDir(DMEventArgs *pEvt)
{
	DUIRecentListBoxDBLClickArgs* pEvent = (DUIRecentListBoxDBLClickArgs*)pEvt;
	CStringW strDir = pEvent->m_strDir;

	if (g_pMainWnd)
	{
		DUIStatic *pSta = g_pMainWnd->FindChildByNameT<DUIStatic>(L"ds_resdirsta");
		CStringW strCurDir = pSta->m_pDUIXmlInfo->m_strText;
		if (0 == strCurDir.CompareNoCase(strDir))
		{
			DM_MessageBox(L"当前Dir已在编辑中!",MB_OK,L"MSG",m_hWnd);
			return DM_ECODE_OK;
		}
		m_strResDir = strDir;
		EndDialog(IDOK);
	}

	return DM_ECODE_OK;
}

DMCode NewResDlg::OnOK()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		m_strResName = m_pResNameEdit->GetWindowText();
		m_strResPath = m_pResPathEdit->GetWindowText();
		if (m_strResName.IsEmpty())
		{
			DM_MessageBox(L"未设置资源名!",MB_OK,L"MSG",m_hWnd);
			break;
		}
		if (m_strResPath.IsEmpty())
		{
			DM_MessageBox(L"未设置资源路径!",MB_OK,L"MSG",m_hWnd);
			break;
		}

		m_strResDir = L"";
		if (m_strResPath.Right(1)!=L"\\")
		{
			m_strResPath += L"\\";
		}
		m_strResDir = m_strResPath + m_strResName;
		if (PathFileExists(m_strResDir))
		{
			DM_MessageBox(L"资源目录已存在,请重新选择!",MB_OK,L"MSG",m_hWnd);
			break;
		}

		if (!DMSUCCEEDED(CreateResDir()))
		{
			DM_MessageBox(L"资源目录创建失败,请重新选择!",MB_OK,L"MSG",m_hWnd);
			break;
		}
		iErr = DM_ECODE_OK;
		EndDialog(IDOK);
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		m_strResName = m_strResPath = m_strResDir = L"";
	}

	return iErr;
}

DMCode NewResDlg::CreateResDir()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		wchar_t szSrcPath[MAX_PATH] = {0};
		DM::GetRootFullPath(NEW_DIR,szSrcPath,MAX_PATH);
		if (false == CopyDirectory(szSrcPath,m_strResDir))
		{
			break;
		}
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}
