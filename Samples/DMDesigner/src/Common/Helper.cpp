#include "DMDesignerAfx.h"
#include "Helper.h"
DMLazyT<DMCtrlXml> g_pAttr;

bool IsDirectoryExist(CStringW strDir)
{
	bool bRet = false;
	do 
	{
		DWORD dwAttr = GetFileAttributesW(strDir);

		if (INVALID_FILE_ATTRIBUTES != dwAttr
			&& FILE_ATTRIBUTE_DIRECTORY == (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		{
			bRet = true;
		}
	} while (false);
	return bRet;
}

bool OpenFolderAndSelectFile(CStringW strFilePath)
{
	bool bRet = false;
	::CoInitialize(NULL);
	LPSHELLFOLDER pDesktopFolder = NULL;
	do 
	{
		if (FAILED(SHGetDesktopFolder(&pDesktopFolder)))
		{
			break;
		}

		LPITEMIDLIST pidl;
		ULONG chEaten;
		wchar_t wfilePath[MAX_PATH+1] = {0};
		// IShellFolder::ParseDisplayName要传入宽字节
		wcscpy_s(wfilePath,MAX_PATH+1,strFilePath);

		LPWSTR lpWStr = wfilePath;
		HRESULT hr = pDesktopFolder->ParseDisplayName(NULL, 0, lpWStr, &chEaten, &pidl, NULL);
		if (FAILED(hr))
		{
			break;
		}

		LPCITEMIDLIST cpidl = pidl;
		// SHOpenFolderAndSelectItems是非公开的API函数，需要从shell32.dll获取
		HMODULE hShell32DLL = ::LoadLibraryW(L"shell32.dll");
		if (hShell32DLL)
		{
			typedef HRESULT (WINAPI *pSelFun)( LPCITEMIDLIST pidlFolder, UINT cidl, LPCITEMIDLIST *apidl, DWORD dwFlags );
			pSelFun pFun = (pSelFun)::GetProcAddress(hShell32DLL, "SHOpenFolderAndSelectItems");
			::FreeLibrary(hShell32DLL);
			if(pFun != NULL)
			{ // 第二个参数cidl置为0，表示是选中文件
				bRet = SUCCEEDED(pFun(cpidl, 0, NULL, 0));
			}
		}
	} while (false);
	if (pDesktopFolder)
	{
		pDesktopFolder->Release();
	}
	::CoUninitialize();
	return bRet;
}

bool CopyDirectory(CStringW strSrcDir,CStringW strDestDir)
{
	bool bRet = false;
	do 
	{
		if (strSrcDir.IsEmpty()||strDestDir.IsEmpty())		
		{
			break;
		}
		wchar_t szSrcFolder[MAX_PATH+1] = {0};
		wcscpy_s(szSrcFolder,strSrcDir.GetBuffer());strSrcDir.ReleaseBuffer();
		szSrcFolder[strSrcDir.GetLength()] =  L'\0';
		szSrcFolder[strSrcDir.GetLength()+1] = L'\0';

		wchar_t szDestFolder[MAX_PATH+1] = {0};
		wcscpy_s(szDestFolder,strDestDir.GetBuffer());strDestDir.ReleaseBuffer();
		szDestFolder[strDestDir.GetLength()] =  L'\0';
		szDestFolder[strDestDir.GetLength()+1] = L'\0';

		SHFILEOPSTRUCTW FileOp; 
		ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT)); 
		FileOp.fFlags |= FOF_SILENT;        /*不显示进度*/
		FileOp.fFlags |= FOF_NOERRORUI ;    /*不报告错误信息*/
		FileOp.fFlags |= FOF_NOCONFIRMATION;/*不进行确认*/
		FileOp.hNameMappings = NULL;
		FileOp.hwnd = NULL;
	    FileOp.lpszProgressTitle = NULL;
		FileOp.wFunc = FO_COPY;
		FileOp.pFrom = szSrcFolder;        /*源目录，必须以2个\0结尾*/
		FileOp.pTo = szDestFolder;          /*目的目录，必须以2个\0结尾*/    
		if (0!= SHFileOperationW(&FileOp))
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool SetLogInfo(CStringW strInfo)
{
	if (g_pMainWnd)
	{
		g_pMainWnd->FindChildByNameT<DUIStatic>(L"ds_tips")->SetAttribute(XML_CLRTEXT,L"pbgra(0,0,ff,ff)");
		g_pMainWnd->FindChildByNameT<DUIStatic>(L"ds_tips")->DV_SetWindowText(strInfo);
		return true;
	}
	return false;
}

DMXmlDocument* GetExistXmlDoc(LPCWSTR lpszType,LPCWSTR lpszResName)
{
	DMXmlDocument* pXmlDoc = NULL;
	do 
	{
		if (NULL==lpszType||NULL==lpszResName||NULL == g_pMainWnd||NULL == g_pMainWnd->m_pDesignerXml)
		{
			break;
		}

		DMSmartPtrT<ResFolder>pRes = g_pMainWnd->m_pDesignerXml->m_pRes;
		CStringW strPath = pRes->GetItemPath(lpszType,lpszResName,L"");
		if (!PathFileExists(strPath))
		{
			break;
		}
		DocDataPtr pSubData = g_pMainWnd->m_pDesignerXml->FindDocData(strPath);
		if (pSubData&&pSubData->IsValid())// 原来已解析过了
		{
			pXmlDoc = pSubData->m_pXmlDoc;
			break;
		}
	} while (false);
	return pXmlDoc;
}

int DM_MessageBox(LPCWSTR lpText, UINT uType, LPCWSTR lpCaption,HWND hWnd)
{
	DMSmartPtrT<MsgBox> pBox;pBox.Attach(new  MsgBox());
	if (NULL == hWnd)
	{
		hWnd = g_pMainWnd->m_hWnd;
	}
	int iRet = pBox->MessageBox(hWnd,lpText,lpCaption,uType);
	if (g_pMainWnd && g_pMainWnd->IsWindow())
	{
		g_pMainWnd->SetActiveWindow();
	}
	return iRet;
}

int StringToInt(CStringW str)
{
	int iRet = 0;
	dm_parseint(str,iRet);
	return iRet;
}

CStringW IntToString(int id)
{
	CStringW str;
	str.Format(L"%d",id);
	return str;
}

bool IsUseDgSkin()
{
	bool bRet = false;
	do 
	{
		if (NULL == g_pMainWnd
			||NULL == g_pMainWnd->m_pDesignerXml
			||g_pMainWnd->m_pDesignerXml->m_strResDir.IsEmpty())
		{
			break;
		}

		CStringW strDgSkinDir = g_pMainWnd->m_pDesignerXml->m_strResDir+L"themes\\theme0\\designer";
		bRet = IsDirectoryExist(strDgSkinDir);
	} while (false);
	return bRet;
}

DMCode AutoDrawText(IDMCanvas*pCanvas,CStringW strFont,DMColor TextClr,LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha/*=0xFF*/)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<IDMRender> pRender;
		g_pDMApp->CreateRegObj((void**)&pRender,NULL, DMREG_Render);
		DMColor ClrOld = pCanvas->SetTextColor(TextClr);

		IDMFont* pFont = g_pDMApp->GetFont(strFont);
		DMSmartPtrT<IDMFont> pOldFont;
		pCanvas->SelectObject(pFont,(IDMMetaFile**)&pOldFont);

		iErr = pCanvas->DrawText(lpString,nCount,lpRect,uFormat,alpha);
		
		pCanvas->SetTextColor(ClrOld);
		pCanvas->SelectObject(pOldFont);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode AutoDrawRoundRect(IDMCanvas*pCanvas,DMColor TextClr,int iStyle,int iWidth,LPCRECT lpRect,POINT &pt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<IDMRender> pRender;
		g_pDMApp->CreateRegObj((void**)&pRender,NULL, DMREG_Render);
		DMSmartPtrT<IDMPen> pPen;
		pRender->CreatePen(TextClr,iStyle,iWidth, &pPen);

		DMSmartPtrT<IDMPen> pOldPen;
		pCanvas->SelectObject(pPen,(IDMMetaFile**)&pOldPen);;

		pCanvas->DrawRoundRect(lpRect,pt);
		
		pCanvas->SelectObject(pOldPen);	
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode AutoFillRoundRect(IDMCanvas*pCanvas,DMColor BrushClr,LPCRECT lpRect,POINT &pt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<IDMRender> pRender;
		g_pDMApp->CreateRegObj((void**)&pRender,NULL, DMREG_Render);
		DMSmartPtrT<IDMBrush> pBrsh;
		pRender->CreateSolidColorBrush(BrushClr, &pBrsh);

		DMSmartPtrT<IDMBrush> pOldBrush;
		pCanvas->SelectObject(pBrsh,(IDMMetaFile**)&pOldBrush);;

		pCanvas->FillRoundRect(lpRect,pt);

		pCanvas->SelectObject(pOldBrush);	
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

// DMHook
DMHook::DMHook()
{
	wchar_t szDll[MAX_PATH] = {0};
	GetRootFullPath(L".\\DesignerRes\\DMH.dll",szDll,MAX_PATH);
	m_hDll = LoadLibraryW(szDll);
	if (m_hDll)// 加载DM hook皮肤
	{
		typedef int (WINAPI *fun_DMMain_Attach)();
		fun_DMMain_Attach DMMain_Attach = (fun_DMMain_Attach)GetProcAddress(m_hDll, "_DMMain_Attach@0");
		if (DMMain_Attach)
		{
			DMMain_Attach();
		}
	}
}

DMHook::~DMHook()
{
	if (m_hDll)
	{
		typedef int (WINAPI *fun_DMMain_Detach)();
		fun_DMMain_Detach DMMain_Detach = (fun_DMMain_Detach)GetProcAddress(m_hDll, "_DMMain_Detach@0");
		if (DMMain_Detach)
		{
			DMMain_Detach();
		}
		FreeLibrary(m_hDll);
	}
}

// DMXmlInitAttr
DMXmlInitAttr::DMXmlInitAttr(DMXmlAttribute& XmlAttr)
{
	m_pAttr  = new DMXmlAttribute;
	*m_pAttr = XmlAttr;
	m_bUse   = false;
}

DMXmlInitAttr::~DMXmlInitAttr()
{
	DM_DELETE(m_pAttr);
	m_bUse = false;
}

// DMAttrXml
DMCtrlXml::DMCtrlXml()
{
	// 初始化DMAttr.xml
	wchar_t szPath[MAX_PATH] = {0};
	DM::GetRootFullPath(ATTR_FILE,szPath,MAX_PATH);
	if (!PathFileExists(szPath))
	{
		DMASSERT(L"未找到DMAttr.xml");
	}
	m_AttrDoc.LoadFromFile(szPath);

	// 初始化DMCls.xml
	DM::GetRootFullPath(CLS_FILE,szPath,MAX_PATH);
	if (!PathFileExists(szPath))
	{
		DMASSERT(L"未找到DMCls.xml");
	}
	m_ClsDoc.LoadFromFile(szPath);

	// 初始化DMAdd.xml
	DM::GetRootFullPath(ADD_FILE,szPath,MAX_PATH);
	if (!PathFileExists(szPath))
	{
		DMASSERT(L"未找到DMAdd.xml");
	}
	m_AddDoc.LoadFromFile(szPath);

	// 初始化DMAdd_dg.xml
	DM::GetRootFullPath(ADD_DG_FILE,szPath,MAX_PATH);
	if (!PathFileExists(szPath))
	{
		DMASSERT(L"未找到DMAdd_dg.xml");
	}
	m_AddDgDoc.LoadFromFile(szPath);

	// 初始化Copy Node，临时保存的xml
	DMXmlNode BaseNode = m_CopyDoc.Base();
	BaseNode.InsertChildNode(L"root");

	// 初始化DMDesigner.xml
	DM::GetRootFullPath(REC_FILE,szPath,MAX_PATH);
	if (!PathFileExists(szPath))
	{
		DMXmlNode NewNode = m_RecentDoc.Base();
		NewNode.InsertChildNode(L"recently");
		m_RecentDoc.SaveXml(szPath);
	}
	else
	{
		m_RecentDoc.LoadFromFile(szPath);
	}
}

DMCtrlXml::~DMCtrlXml()
{
	wchar_t szPath[MAX_PATH] = {0};
	DM::GetRootFullPath(REC_FILE,szPath,MAX_PATH);
	m_RecentDoc.SaveXml(szPath);
}

bool DMCtrlXml::Parse(DMXmlAttributePtr ptr,CStringW &strType,CStringW &strName,CStringW& strValue,CStringW &strDesc)
{
	bool bRet = false;
	do 
	{
		if (NULL == ptr||!ptr->IsValid())
		{
			break;
		}
		CStringW AttrName = ptr->GetName();
		CStringWList strList;
		int nCount = (int)SplitStringT(AttrName,L'_',strList);
		DMASSERT(2==nCount);

		//0.指定desc
		strType = strList[0];
		strName = strList[1];
		strDesc = ptr->GetValue();

		//1.初始化type
		CStringW tempDesc;
		nCount = countof(g_stAttrDesc);
		for (int i=0; i<nCount; i++)
		{
			if (0 == strList[0].CompareNoCase(g_stAttrDesc[i].strOne))
			{
				tempDesc = g_stAttrDesc[i].strTwo;
				break;
			}
		}
		if (tempDesc.IsEmpty())
		{
			tempDesc = L"未知描述";
		}
	
		// 4种状态:1.{value}desc 2.desc 3.{value} 4.空
		if (strDesc.IsEmpty())//4
		{
			strDesc = tempDesc;
		}
		else
		{
			CStringWList strValueList;
			nCount = (int)SplitStringT(strDesc,L'}',strValueList);
			if (strDesc.Left(1) == L'{'&& 2 == nCount)//1
			{
				strValue = strValueList[0].Mid(1,strValueList[0].GetLength()-1);
				strDesc = strValueList[1];
			}
			else if (strDesc.Left(1) == L'{'&& 1 == nCount)//3
			{
				strValue = strValueList[0].Mid(1,strValueList[0].GetLength()-1);
				strDesc = tempDesc;
			}
			else 
			{
				strDesc = strValueList[0];
			}
		}

		bRet = true;
	} while (false);
	return bRet;
}

bool DMCtrlXml::ParseName(DMXmlAttributePtr ptr,CStringW &strType,CStringW &strName)
{
	bool bRet = false;
	do 
	{
		if (NULL == ptr)
		{
			break;
		}
		CStringW AttrName = ptr->GetName();
		CStringWList strList;
		int nCount = (int)SplitStringT(AttrName,L'_',strList);
		DMASSERT(2==nCount);
	
		strType = strList[0];
		strName = strList[1];

		bRet = true;
	} while (false);
	return bRet;
}

bool DMCtrlXml::Parse(CStringW strClsName,CStringWList &strList)
{
	bool bRet = false;
	do 
	{
		DMXmlNode XmlRoot = m_ClsDoc.Root();
		if (!XmlRoot.IsValid())
		{
			break;
		}

		DMXmlNode XmlNode = XmlRoot.FirstChild(strClsName);
		if (!XmlNode.IsValid())
		{
			break;
		}

		DMXmlNode XmlChild = XmlNode.FirstChild();
		while (XmlChild.IsValid())
		{
			strList.Add(XmlChild.GetName());

			XmlChild = XmlChild.NextSibling();
		}
		bRet = true;
	} while (false);
	return bRet;
}

DMXmlNode DMCtrlXml::Parse(CStringW strReg)
{
	DMXmlNode XmlNode;
	do 
	{
		if (strReg.IsEmpty())
		{
			break;
		}

		DMXmlNode RootNode = IsUseDgSkin()?m_AddDgDoc.Root():m_AddDoc.Root();
		if (!RootNode.IsValid())
		{
			break;
		}
		XmlNode = RootNode.FirstChild(strReg);
		if (!XmlNode.IsValid())
		{// 不存在就自建一个默认的
			XmlNode = RootNode.InsertChildNode(strReg);
			XmlNode.SetAttribute(L"ncmargin",L"1,1,1,1");
			XmlNode.SetAttribute(L"clrnc",L"pbgra(00,00,00,ff)");
			XmlNode.SetAttribute(L"clrbg",L"pbgra(dd,dd,dd,ff)");
			XmlNode.SetAttribute(L"text",strReg);
		}
	} while (false);
	return XmlNode;
}

DMXmlNode DMCtrlXml::GetAttrNode(CStringW strNode)
{
	DMXmlNode XmlNode;
	do 
	{
		if (strNode.IsEmpty())
		{
			break;
		}
		DMXmlNode RootNode = m_AttrDoc.Root();
		if (!RootNode.IsValid())
		{
			break;
		}

		XmlNode = RootNode.FirstChild(strNode);
	} while (false);
	return XmlNode;
}

bool DMCtrlXml::SetCopyNode(DMXmlNodePtr pNode, CRect rcWnd)
{
	if (pNode->IsValid())
	{
		DMXmlNode RootNode = m_CopyDoc.Root();
		if (m_CopyNode.IsValid())
		{
			RootNode.RemoveChildNode(&m_CopyNode);
			m_rcCopy.SetRectEmpty();
		}
		m_CopyNode = RootNode.InsertCopyChildNode(pNode);
		// 移除掉名字、id、坐标
		m_CopyNode.RemoveAttribute(XML_NAME);
		m_CopyNode.RemoveAttribute(XML_POS);
		m_CopyNode.RemoveAttribute(XML_ID);
		m_rcCopy = rcWnd;
		Init_Debug_XmlBuf(RootNode);
		return true;
	}
	return false;
}

bool DMCtrlXml::AddRecentResDir(CStringW strNewDir)
{
	bool bRet = false;
	do 
	{
		//1.合法性判断
		if (strNewDir.IsEmpty()||!IsDirectoryExist(strNewDir))
		{
			break;
		}
		if (strNewDir.Right(1)!=L"\\")
		{// 避免出现E:\MyRes和E:\MyRes\\并存的现象
			strNewDir += L"\\";
		}

		//2.循环查找是否已存在此dir记录
		DMXmlNode XmlRoot = m_RecentDoc.Root();
		DMXmlNode XmlExist,XmlLast;
		int iNum = 0;
		DMXmlNode XmlNode = XmlRoot.FirstChild();
		while (XmlNode.IsValid())
		{
			CStringW strDir = XmlNode.Attribute(XML_PATH);
			if (strDir.Right(1)!=L"\\")
			{
				strDir += L"\\";
			}

			if (0 == strNewDir.CompareNoCase(strDir))
			{// 已存在
				XmlExist = XmlNode;
			}
			XmlLast = XmlNode;
			XmlNode = XmlNode.NextSibling();
			iNum ++;
		}
		//3.限制iNum不能大于5条,否则移除最后一条,如果已存在此dir记录，iNum是不会变化的，所以判断限制为5，如果不存在，iNum将要+1,所以判断限制为4
		if (XmlExist.IsValid()&&iNum>5
			||!XmlExist.IsValid()&&iNum>4
			)
		{
			XmlRoot.RemoveChildNode(&XmlLast);
		}

		//4.在最前面插入新记录
		if (XmlExist.IsValid())// 原来就存在了,先移除
		{
			XmlRoot.RemoveChildNode(&XmlExist);
		}

		DMXmlNode XmlFirst = XmlRoot.FirstChild();
		DMXmlNode XmlNewNode = XmlRoot.InsertChildNode(L"item",&XmlFirst,false);
		XmlNewNode.SetAttribute(XML_PATH,strNewDir);
		bRet = true;
	} while (false);
	return bRet;
}

bool DMCtrlXml::RemoveRecentResDir(CStringW strNewDir)
{
	bool bRet = false;
	do 
	{
		DMXmlNode XmlRoot = m_RecentDoc.Root();
		DMXmlNode XmlNode = XmlRoot.FirstChild();
		while (XmlNode.IsValid())
		{
			CStringW strDir = XmlNode.Attribute(XML_PATH);
			if (0 == strNewDir.CompareNoCase(strDir))
			{
				XmlRoot.RemoveChildNode(&XmlNode);
				break;
			}
	
			XmlNode = XmlNode.NextSibling();
		}
		bRet = true;
	} while (false);
	return bRet;
}
