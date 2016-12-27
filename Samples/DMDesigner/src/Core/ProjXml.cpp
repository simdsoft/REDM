#include "DMDesignerAfx.h"
#include "ProjXml.h"

DesignMenu g_ProjMenuItem[] = \
{
	{PROJMENU_OPENDIR,			L"     打开目录"},//0 
	{PROJMENU_SCAN,				L"     扫描错误"},//1 
	{PROJMENU_ADDSKINPOOL,		L"     增加皮肤池"},//2 
	{PROJMENU_DELSKINPOOL,		L"     删除皮肤池"},//3 
	{PROJMENU_EDITSKINPOOL,		L"     编辑皮肤池"},//4 

	{PROJMENU_ADDSTYLEPOOL,		L"     增加样式池"},//5 
	{PROJMENU_DELSTYLEPOOL,		L"     删除样式池"},//6 
	{PROJMENU_EDITSTYLEPOOL,	L"     编辑样式池"},//7 

	{PROJMENU_ADDSKIN,			L"     增加皮肤"},//8 
	{PROJMENU_DELSKIN,			L"     删除皮肤"},//9 
	{PROJMENU_EDITSKIN,			L"     编辑皮肤"},//10 

	{PROJMENU_ADDSTYLE,			L"     增加样式"},//11
	{PROJMENU_DELSTYLE,			L"     删除样式"},//12
	{PROJMENU_EDITSTYLE,		L"     编辑样式"},//13 

	{PROJMENU_ADDIMGTYPE,		L"     增加img类型"},//14 
	{PROJMENU_DELIMGTYPE,		L"     删除img类型"},//15 
	{PROJMENU_EDITIMGTYPE,		L"     编辑img类型"},//16 

	{PROJMENU_ADDTHEME,			L"     增加主题包"},//17 
	{PROJMENU_DELTHEME,			L"     删除主题包"},//18 
	{PROJMENU_EDITTHEME,		L"     编辑主题包"},//19
	{PROJMENU_SETDEFTHEME,		L"     设置默认主题包"},//20

	{PROJMENU_ADDIMG,			L"     增加IMG"},//21 
	{PROJMENU_DELIMG,			L"     删除IMG"},//22
	{PROJMENU_EDITIMG,			L"     编辑IMG"},//23

	{PROJMENU_ADDLAYOUT,		L"     增加LAYOUT"},//24
	{PROJMENU_DELLAYOUT,		L"     删除LAYOUT"},//25
	{PROJMENU_EDITLAYOUT,		L"     编辑LAYOUT"},//26
};

ProjXml::ProjXml()
{
	m_pProjTree = g_pMainWnd->FindChildByNameT<ProjTree>(L"ds_projtree");DMASSERT(m_pProjTree);
	m_hProjLayouts = m_hProjThemes = m_hProjLayoutFiles = m_hProjGlobal = 
	m_hProjGlobalStyleSkins = m_hProjPrivStyleSkins = m_hProjGlobalStyles = m_hProjGlobalSkins = m_hProjSel = m_hProjDefTheme = NULL;
	m_bSubInit  = false;
}

ProjXml::~ProjXml()
{
	ReleaseProjTree();
}

DMCode ProjXml::ParseRes(CStringW strResDir)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		m_pRes.Release();
		m_pRes.Attach(new ResFolder);
		if (!DMSUCCEEDED(m_pRes->LoadResPack((WPARAM)(const wchar_t*)strResDir,NULL)))
		{
			break;
		}
		m_strResDir = strResDir;
		if (m_strResDir.Right(1)!=L"\\")
		{
			m_strResDir += L"\\";
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

bool  ProjXml::IsNeedSave()
{
	bool bRet = false;
	do 
	{
		int iCount = (int)m_DMArray.GetCount();
		for (int i=0;i<iCount;i++)
		{
			DocDataPtr &pDoc = m_DMArray[i];
			if (pDoc&&pDoc->IsValid()&&pDoc->m_bChange)
			{
				bRet = true;
				break;
			}
		}
	} while (false);
	return bRet;
}

DMCode ProjXml::SaveRes(bool bCheck)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (bCheck &&false == IsNeedSave())
		{
			DM_MessageBox(L"文件不需要保存");
			break;
		}
		if (IDOK == DM_MessageBox(L"确认保存修改到资源包中?\r\n",MB_OKCANCEL))
		{
			int iCount = (int)m_DMArray.GetCount();
			for (int i=0;i<iCount;i++)
			{
				DocDataPtr &pDoc = m_DMArray[i];
				if (pDoc&&pDoc->IsValid()&&pDoc->m_bChange)
				{
					pDoc->m_pXmlDoc->SaveXml(pDoc->m_strXmlPath);
					pDoc->m_bChange = false;
				}
			}
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::InitProjTree()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		// 1.初始化树形控件结点NODE
		DMXmlDocument doc;
		DMXmlNode NoDataNode = doc.Base();
		DMXmlNode DataNode = doc.Base();
		InitProjTreeNode(NoDataNode,false);
		InitProjTreeNode(DataNode,true);

		DMXmlNode XmlNode,XmlFile,XmlImg,XmlStyle,XmlSkin;
		CStringW strText,strPath,strName,strInfo;
		HDMTREEITEM hRet0,hRet1,hRet2,hRet3;

		// 3.插入两个副标题:样式-皮肤-布局文件与主题包列表
		m_hProjLayouts = InsertProjTreeItem(NoDataNode,L"样式-皮肤-文件列表");
		m_hProjThemes = InsertProjTreeItem(DataNode,PROJTREE_THEMES);

		// 4.解析布局文件列表
		strPath = m_strResDir + XML_LAYOUT_DMINDEX;
		DocDataPtr pDoc = new DocData(strPath);
		if (!pDoc->IsValid())
		{
			DM_DELETE(pDoc);
			break;
		}
		AddObj(pDoc);

		// 5.插入标题:布局文件列表
		m_hProjLayoutFiles = InsertProjTreeItem(DataNode,PROJTREE_LAYOUTFILES,m_hProjLayouts);
		BindProjTreeData(pDoc->m_XmlRoot,m_hProjLayoutFiles);

		// 6.插入布局文件列表
		XmlNode = pDoc->m_XmlRoot.FirstChild();
		while (XmlNode.IsValid())
		{
			strName = XmlNode.GetName();
			hRet0 = InsertProjTreeItem(DataNode,strName,m_hProjLayoutFiles);
			BindProjTreeData(XmlNode,hRet0);
	
			XmlFile = XmlNode.FirstChild(XML_FILE);// 子项
			while (XmlFile.IsValid())
			{
				strText = XmlFile.Attribute(XML_PATH);
				strText = strText.Right(strText.GetLength()-strText.ReverseFind(L'\\')-1);
				if (m_strGlobalName.IsEmpty() && 0 == strName.CompareNoCase(XML_GLOBAL))// 绑定global,如果有多份global，则只绑定第一份global
				{
					m_strGlobalName = XmlFile.Attribute(XML_NAME);
					m_hProjGlobal = InsertProjTreeItem(DataNode,strText,hRet0);
					BindProjTreeData(XmlFile,m_hProjGlobal);
				}
				else// 绑定layout
				{
					hRet1 = InsertProjTreeItem(DataNode,strText,hRet0);
					BindProjTreeData(XmlFile,hRet1);
				}
				XmlFile = XmlFile.NextSibling(XML_FILE);
			}
			XmlNode = XmlNode.NextSibling();
		}
		if (m_hProjGlobal)
		{
			hRet0 = m_pProjTree->GetParentItem(m_hProjGlobal);
			if (m_pProjTree->GetChildrenCount(hRet0)>1)
			{
				DM_MessageBox(L"检测到多份global.xml,请合并后再打开工程视图");
			}
		}

		// 7.主题列表绑定数据
		strPath = m_strResDir + XML_THEMES_DMINDEX;
		pDoc = new DocData(strPath);
		if (!pDoc->IsValid())
		{
			DM_DELETE(pDoc);
			break;
		}
		AddObj(pDoc);

		XmlNode = pDoc->m_XmlRoot.FirstChild(XML_THEMES);
		BindProjTreeData(XmlNode,m_hProjThemes);
		CStringW strDefThemeName = XmlNode.Attribute(XML_NAME);

		DMXmlNode ThemeNode = XmlNode.FirstChild(XML_FILE);
		while (ThemeNode.IsValid())// 遍历主题列表
		{
			// 8.遍历插入主题包
			strText = ThemeNode.Attribute(XML_NAME);
			strPath = m_strResDir + ThemeNode.Attribute(XML_PATH);

			if (0 == strText.CompareNoCase(strDefThemeName))// 默认主题包
			{
				DataNode.SetAttribute(XML_SKIN,XML_DEFSKIN);
			}
			else
			{
				DataNode.SetAttribute(XML_SKIN,XML_DATASKIN);
			}

			hRet0 = InsertProjTreeItem(DataNode,strText,m_hProjThemes);
			if (0 == strText.CompareNoCase(strDefThemeName))// 默认主题包
			{
				m_hProjDefTheme = hRet0;
			}
			BindProjTreeData(ThemeNode,hRet0);
			pDoc = new DocData(strPath);
			if (!pDoc->IsValid())
			{
				DM_DELETE(pDoc);
				ThemeNode = ThemeNode.NextSibling(XML_FILE);
				continue;
			}
			AddObj(pDoc);

			// 9.遍历img类型
			DataNode.SetAttribute(XML_SKIN,XML_DATASKIN);
			XmlImg = pDoc->m_XmlRoot.FirstChild();
			while (XmlImg.IsValid())
			{
				strText = XmlImg.GetName();
				hRet1 = InsertProjTreeItem(DataNode,strText,hRet0);
				BindProjTreeData(XmlImg,hRet1);

				// 10.遍历img文件
			    XmlFile = XmlImg.FirstChild(XML_FILE);
				while (XmlFile.IsValid())
				{
					strText = XmlFile.Attribute(XML_PATH);
					strText = strText.Right(strText.GetLength()-strText.ReverseFind(L'\\')-1);
					hRet2 = InsertProjTreeItem(DataNode,strText,hRet1);
					BindProjTreeData(XmlFile,hRet2);
					XmlFile = XmlFile.NextSibling(XML_FILE);
				}

				XmlImg = XmlImg.NextSibling();
			}
			ThemeNode = ThemeNode.NextSibling(XML_FILE);
		}   
		DataNode.SetAttribute(XML_SKIN,XML_DATASKIN);
	
		// 11.插入结点:全局样式皮肤列表,私有样式皮肤列表
		m_hProjGlobalStyleSkins = InsertProjTreeItem(DataNode,PROJTREE_GLOBALSTYLESKINS,m_hProjLayouts);
		m_hProjPrivStyleSkins = InsertProjTreeItem(NoDataNode,PROJTREE_PRIVSTYLESKINS,m_hProjLayouts);
		m_hProjGlobalStyles = InsertProjTreeItem(DataNode,PROJTREE_GLOBALSTYLS,m_hProjGlobalStyleSkins);
		m_hProjGlobalSkins = InsertProjTreeItem(DataNode,PROJTREE_GLOBALSKINS,m_hProjGlobalStyleSkins);

		// 12.读取全局xml的数据
		DMXmlNodePtr pData = GetObjByKey(m_hProjGlobal);
		BindProjTreeData(*pData,m_hProjGlobalStyleSkins);
		strPath = m_strResDir + pData->Attribute(XML_PATH);
		pDoc = new DocData(strPath);
		if (!pDoc->IsValid())
		{
			DM_DELETE(pDoc);
			break;
		}
		AddObj(pDoc);
		BindProjTreeData(pDoc->m_XmlRoot,m_hProjGlobalStyles);
		BindProjTreeData(pDoc->m_XmlRoot,m_hProjGlobalSkins);

		// 13.插入结点:全局样式列表
		XmlNode = pDoc->m_XmlRoot.FirstChild(XML_STYLE);
		while (XmlNode.IsValid())
		{
			strText = XmlNode.Attribute(XML_NAME);// 可为空
			if (strText.IsEmpty())
			{
				strText = XML_STYLE;
			}
			hRet0 = InsertProjTreeItem(DataNode,strText,m_hProjGlobalStyles);
			BindProjTreeData(XmlNode,hRet0);
			
			// 读取style列表数据
			XmlStyle = XmlNode.FirstChild(XML_STYLE);
			while (XmlStyle.IsValid())
			{
				strText = XmlStyle.Attribute(XML_ID);
				hRet1 = InsertProjTreeItem(DataNode,strText,hRet0);
				BindProjTreeData(XmlStyle,hRet1);

				XmlStyle = XmlStyle.NextSibling(XML_STYLE);
			} 
			
			XmlNode = XmlNode.NextSibling(XML_STYLE);
		}

		// 14.插入结点:全局皮肤列表
		XmlNode = pDoc->m_XmlRoot.FirstChild(XML_SKIN);
		while (XmlNode.IsValid())
		{
			strText = XmlNode.Attribute(XML_NAME);// 可为空
			if (strText.IsEmpty())
			{
				strText = XML_SKIN;
			}
			hRet0 = InsertProjTreeItem(DataNode,strText,m_hProjGlobalSkins);
			BindProjTreeData(XmlNode,hRet0);

			// 读取skin列表数据
			XmlSkin = XmlNode.FirstChild();
			while (XmlSkin.IsValid())
			{
				strText = XmlSkin.Attribute(XML_ID);
				hRet1 = InsertProjTreeItem(DataNode,strText,hRet0);
				BindProjTreeData(XmlSkin,hRet1);

				XmlSkin = XmlSkin.NextSibling();
			}

			XmlNode = XmlNode.NextSibling(XML_SKIN);
		}

		// 15.插入结点:DM Xml列表
		DMSmartPtrT<ResFolderItem> pLayout = m_pRes->m_pLayout;
		int nCount = (int)pLayout->GetCount();
		for (int i=0; i<nCount; i++)
		{
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
			DMXmlNode XmlNode = doc.Root(XML_DM);// 只有以dm为根结点的才能创建窗口
			if (!XmlNode.IsValid())
			{
				continue;
			} 

			// 前面判断完成后，开始加入列表
			pDoc = new DocData(pItem->m_szPath);
			if (!AddObj(pDoc))
			{
				DM_DELETE(pDoc);
			}

			// 插入DM窗口id,并绑定到layouts
			strText = pItem->m_szName;
			strPath = m_strResDir + XML_LAYOUT_DMINDEX;
			DocDataPtr pLayoutDoc = FindDocData(strPath);
			bool bFind = false;
			XmlFile = pLayoutDoc->m_XmlRoot.FirstChild(XML_LAYOUT).FirstChild(XML_FILE);
			while (XmlFile.IsValid())
			{
				strName = XmlFile.Attribute(XML_NAME);
				if (0 == strName.CompareNoCase(strText))
				{
					bFind = true;
					break;
				}
				XmlFile = XmlFile.NextSibling(XML_FILE);
			}
			if (false == bFind)
			{
				continue;
			}
			hRet0 = InsertProjTreeItem(DataNode,strText,m_hProjPrivStyleSkins);
			BindProjTreeData(XmlFile,hRet0);
			//16.插入结点:sub窗口xml列表
			InsertProjTreeItem(NoDataNode,L"sub窗口xml列表",hRet0);

			//17.插入结点:私有样式列表
			hRet1 =	InsertProjTreeItem(DataNode,PROJTREE_PRIVSTYLES,hRet0);
			BindProjTreeData(pDoc->m_XmlRoot,hRet1);

			//18.插入结点:私有样式类型
			XmlNode = pDoc->m_XmlRoot.FirstChild(XML_STYLE);
			while (XmlNode.IsValid())
			{
				strText = XmlNode.Attribute(XML_NAME);// 可为空
				if (strText.IsEmpty())
				{
					strText = XML_STYLE;
				}
				hRet2 = InsertProjTreeItem(DataNode,strText,hRet1);
				BindProjTreeData(XmlNode,hRet2);

				// 读取style列表数据
				XmlStyle = XmlNode.FirstChild(XML_STYLE);
				while (XmlStyle.IsValid())
				{
					strText = XmlStyle.Attribute(XML_ID);
					hRet3 = InsertProjTreeItem(DataNode,strText,hRet2);
					BindProjTreeData(XmlStyle,hRet3);

					XmlStyle = XmlStyle.NextSibling(XML_STYLE);
				} 

				XmlNode = XmlNode.NextSibling(XML_STYLE);
			}

			// 19.插入结点:私有皮肤列表
			hRet1 =	InsertProjTreeItem(DataNode,PROJTREE_PRIVSKINS,hRet0);
			BindProjTreeData(pDoc->m_XmlRoot,hRet1);
			// 20.插入结点:私有skin类型
			XmlNode = pDoc->m_XmlRoot.FirstChild(XML_SKIN);
			while (XmlNode.IsValid())
			{
				strText = XmlNode.Attribute(XML_NAME);// 可为空
				if (strText.IsEmpty())
				{
					strText = XML_SKIN;
				}
				hRet2 = InsertProjTreeItem(DataNode,strText,hRet1);
				BindProjTreeData(XmlNode,hRet2);

				// 读取skin列表数据
				XmlSkin = XmlNode.FirstChild();
				while (XmlSkin.IsValid())
				{
					strText = XmlSkin.Attribute(XML_ID);
					hRet3 = InsertProjTreeItem(DataNode,strText,hRet2);
					BindProjTreeData(XmlSkin,hRet3);

					XmlSkin = XmlSkin.NextSibling();
				}

				XmlNode = XmlNode.NextSibling(XML_SKIN);
			}
		}
	
		m_pProjTree->Expand(m_hProjLayouts,DMTVEX_COLLAPSE);
		m_pProjTree->Expand(m_hProjThemes,DMTVEX_COLLAPSE);

		// 扫描
		if (IDOK == DM_MessageBox(L"是否进行完整性体检?",MB_OKCANCEL))
		{
			ProjMenu_Scan();
		}	

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
 
}

DMCode ProjXml::ReleaseProjTree()
{ 
	DMCode iErr = DM_ECODE_FAIL;
	do  
	{
		m_pProjTree->RemoveAllItems();
		DMArrayT<DocDataPtr>::RemoveAll();
		DMMapT<HDMTREEITEM,DMXmlNodePtr>::RemoveAll();
		m_strGlobalName = L"";
		m_bSubInit = false;
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
} 

DMCode ProjXml::PopProjTreeMenu(HDMTREEITEM hSel)
{
	DMCode iErr = DM_ECODE_FAIL;
	do   
	{ 
		m_hProjSel = hSel;

		DMXmlDocument Doc;
		g_pDMApp->InitDMXmlDocument(Doc, XML_LAYOUT,L"ds_menu_proj");
		DMXmlNode XmlNode = Doc.Root();
		InitCustomProjMenu(XmlNode);
		InitSkinPoolsProjMenu(XmlNode);
		InitStylePoolsProjMenu(XmlNode);
		InitSkinProjMenu(XmlNode);
		InitStyleProjMenu(XmlNode);
		InitImgTypeMenu(XmlNode);
		InitThemeMenu(XmlNode);
		InitImgMenu(XmlNode);
		InitLayoutMenu(XmlNode);

		DUIMenu Menu;
		Menu.LoadMenu(XmlNode); 
		POINT pt;
		GetCursorPos(&pt);
		Menu.TrackPopupMenu(0,pt.x,pt.y,g_pMainWnd->m_hWnd);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::HandleProjTreeMenu(int nID)
{
	switch (nID)
	{
	case PROJMENU_OPENDIR:
		{ 
			ProjMenu_OpenDir();
		}
		break;

	case PROJMENU_SCAN:
		{ 
			ProjMenu_Scan();
		}
		break;

	case PROJMENU_ADDSKINPOOL:
		{
			ProjMenu_AddSkinPool();
		}
		break;

	case PROJMENU_DELSKINPOOL:
		{
			ProjMenu_DelSkinPool();
		}
		break;

	case PROJMENU_EDITSKINPOOL:
		{
			ProjMenu_EditSkinPool();
		}
		break;

	case PROJMENU_ADDSTYLEPOOL:
		{
			ProjMenu_AddStylePool();
		}
		break;

	case PROJMENU_DELSTYLEPOOL:
		{
			ProjMenu_DelStylePool();
		}
		break;

	case PROJMENU_EDITSTYLEPOOL:
		{
			ProjMenu_EditStylePool();
		}
		break;

	case PROJMENU_ADDSKIN:
		{
			ProjMenu_AddSkin();
		}
		break;

	case PROJMENU_DELSKIN:
		{
			ProjMenu_DelSkin();
		}
		break;

	case PROJMENU_EDITSKIN:
		{
			ProjMenu_EditSkin();
		}
		break;

	case PROJMENU_ADDSTYLE:
		{
			ProjMenu_AddStyle();
		}
		break;

	case PROJMENU_DELSTYLE:
		{
			ProjMenu_DelStyle();
		}
		break;

	case PROJMENU_EDITSTYLE:
		{
			ProjMenu_EditStyle();
		}
		break;

	case PROJMENU_ADDIMGTYPE:
		{
			ProjMenu_AddImgType();
		}
		break;

	case PROJMENU_DELIMGTYPE:
		{
			ProjMenu_DelImgType();
		}
		break;

	case PROJMENU_EDITIMGTYPE:
		{
			ProjMenu_EditImgType();
		}
		break;

	case PROJMENU_ADDTHEME:
		{
			ProjMenu_AddTheme();
		}
		break;

	case PROJMENU_DELTHEME:
		{
			ProjMenu_DelTheme();
		}
		break;

	case PROJMENU_EDITTHEME:
		{
			ProjMenu_EditTheme();
		}
		break;

	case PROJMENU_SETDEFTHEME:
		{
			ProjMenu_SetDefTheme();
		}
		break;

	case PROJMENU_ADDIMG:
		{
			ProjMenu_AddImg();
		}
		break;

	case PROJMENU_DELIMG:
		{
			ProjMenu_DelImg();
		}
		break;

	case PROJMENU_EDITIMG:
		{
			ProjMenu_EditImg();
		}
		break;

	case PROJMENU_ADDLAYOUT:
		{
			ProjMenu_AddLayout();
		}
		break;

	case PROJMENU_DELLAYOUT:
		{
			ProjMenu_DelLayout();
		}
		break;

	case PROJMENU_EDITLAYOUT:
		{
			ProjMenu_EditLayout();
		}
		break;

	default:
		break;
	}
	return DM_ECODE_OK;
}

DMCode ProjXml::InitSubXmlList()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (m_bSubInit)
		{
			break;
		}
		HDMTREEITEM hDMTreeItem = m_pProjTree->GetChildItem(m_hProjPrivStyleSkins);
		while (hDMTreeItem)
		{
			DMXmlNodePtr pData = (DMXmlNodePtr)m_pProjTree->GetItemData(hDMTreeItem);
			if (pData)
			{
				CStringW strPath = m_strResDir + pData->Attribute(XML_PATH);
				DocDataPtr pDoc = FindDocData(strPath);
				if (pDoc)
				{
					int nCount = (int)pDoc->GetCount();
					for (int i=0; i<nCount; i++)
					{
						DocDataPtr pSub = pDoc->GetObj(i);
						DMXmlDocument doc;
						DMXmlNode DataNode = doc.Base();
						InitProjTreeNode(DataNode,true);
						CStringW strSubPath = pSub->m_strXmlPath;
						// 查找到此path对应的xml id
						HDMTREEITEM hLayout = m_pProjTree->GetChildItem(m_hProjLayoutFiles,false);
						HDMTREEITEM hFile = m_pProjTree->GetChildItem(hLayout);
						DMXmlNodePtr pFileData = NULL;
						bool bFind = false;
						while (hFile)
						{
							pFileData = (DMXmlNodePtr)m_pProjTree->GetItemData(hFile);
							CStringW strTempPath = m_strResDir + pFileData->Attribute(XML_PATH);
							if (0 == strTempPath.CompareNoCase(strSubPath))
							{
								bFind = true;
								break;
							}

							hFile = m_pProjTree->GetNextSiblingItem(hFile);
						}
						if (bFind&&pFileData->IsValid())
						{
							HDMTREEITEM hRet0 = InsertProjTreeItem(DataNode,pFileData->Attribute(XML_NAME),m_pProjTree->GetChildItem(hDMTreeItem));
							BindProjTreeData(*pFileData,hRet0);
						}
					}
				}
			}
			hDMTreeItem = m_pProjTree->GetNextSiblingItem(hDMTreeItem);
		}
		m_bSubInit = true;
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::SetDocUnSave(CStringW strPath)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DocDataPtr pDoc = FindDocData(strPath);
		if (!pDoc)
		{
			break;
		}
		pDoc->m_bChange = true;
		Init_Debug_XmlBuf(pDoc->m_XmlRoot);	

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::SetDocUnSave(DMXmlNodePtr pNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == pNode)
		{
			break;
		}
		DocDataPtr pDoc = FindDocData(pNode);
		if (!pDoc)
		{
			break;
		}
		pDoc->m_bChange = true;
		Init_Debug_XmlBuf(pDoc->m_XmlRoot);	

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::InitProjTreeNode(DMXmlNode& TreeNode,bool IsData)
{
	TreeNode = TreeNode.InsertChildNode(XML_TREEITEM);
	TreeNode.SetAttribute(XML_SKIN,IsData? XML_DATASKIN:XML_CUSTOMSKIN);
	TreeNode.SetAttribute(XML_CLRTEXT,L"pbgra(ff,ff,ff,ff)");
	TreeNode.SetAttribute(XML_FONT,L"face:新宋体,size:12,weight:100");
	TreeNode.SetAttribute(XML_CHILDOFFSET,L"0");
	return DM_ECODE_OK;
}

DMCode ProjXml::BindProjTreeData(DMXmlNode& DataNode,HDMTREEITEM hTreeItem)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (!DataNode.IsValid()||!m_pProjTree->ItemIsValid(hTreeItem))
		{
			break;
		}
		DMXmlNodePtr pNode = new DMXmlNode;
		if (!DMMapT<HDMTREEITEM,DMXmlNodePtr>::AddKey(hTreeItem,pNode))
		{
			break;
		}
		*pNode = DataNode;
		m_pProjTree->SetItemData(hTreeItem,(LPARAM)pNode);	
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

HDMTREEITEM ProjXml::InsertProjTreeItem(DMXmlNode& TreeNode,CStringW strText,HDMTREEITEM hParent/*=DMTVI_ROOT*/)
{
	TreeNode.SetAttribute(XML_TEXT,strText);
	return m_pProjTree->InsertItem(TreeNode,hParent);
}

DocDataPtr ProjXml::FindDocData(CStringW strPath)
{
	DocDataPtr pDoc = NULL;
	DocData TempDoc;TempDoc.m_strXmlPath = strPath;
	int iFind = FindObj(&TempDoc);
	if (DM_INVALID_VALUE == iFind)
	{
		return pDoc;
	}
	else
	{
		return GetObj(iFind);
	}
}

DocDataPtr ProjXml::FindDocData(DMXmlNodePtr pNode)
{
	int iCount = (int)m_DMArray.GetCount();
	for (int i=0;i<iCount;i++)
	{
		DocDataPtr &pDoc = m_DMArray[i];
		if (pDoc->m_XmlRoot.IsEqualRoot(*pNode))
		{
			return pDoc;
		}
	}

	return NULL;
}

CStringW ProjXml::GetProjTreeItemText(HDMTREEITEM hSel)
{
	CStringW strText;
	do 
	{
		if (NULL == hSel)
		{
			break;
		}
		DM::LPTVITEMEX pData = m_pProjTree->GetItem(hSel);
		if (NULL == pData||NULL == pData->pPanel)
		{
			break;
		}
		strText = pData->pPanel->m_pDUIXmlInfo->m_strText;
	} while (false);
	return strText;
}

DMCode ProjXml::ProjMenu_OpenDir()
{
	CStringW strPath		   = m_strResDir;
	int iChildNum			   = m_pProjTree->GetChildrenCount(m_hProjSel);
	HDMTREEITEM hPTreeItem	   = m_pProjTree->GetParentItem(m_hProjSel);
	HDMTREEITEM hP_PTreeItem   = NULL;
	HDMTREEITEM hP_P_PTreeItem = NULL;
	if (hPTreeItem)
	{
		hP_PTreeItem = m_pProjTree->GetParentItem(hPTreeItem);
	}

	if (hP_PTreeItem)
	{
		hP_P_PTreeItem = m_pProjTree->GetParentItem(hP_PTreeItem);
	}

	do 
	{
		
		if (m_hProjLayouts == m_hProjSel)
		{
			break;
		}
		
		//1.布局文件列表或它的子项(仅一层)，打开layout文件夹下的dmindex.xml
		if (m_hProjLayoutFiles == m_hProjSel
			||m_hProjLayoutFiles == hPTreeItem)
		{
			strPath += XML_LAYOUT_DMINDEX;
			break;
		}

		//2.父结点的父结点是布局文件列表，打开自身标识的文件
		if (m_hProjLayoutFiles == hP_PTreeItem)
		{
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			CStringW strRelativePath = pNode->Attribute(XML_PATH);
			strPath += strRelativePath;
			break;
		}

		//3.样式-皮肤(全局)列表及其所有子项，打开加载的全局global.xml文件
		if (m_pProjTree->IsAncestor(m_hProjGlobalStyleSkins,m_hProjSel))
		{
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjGlobalStyleSkins);
			CStringW strRelativePath = pNode->Attribute(XML_PATH);
			strPath += strRelativePath;
			break;
		}

		if (m_hProjPrivStyleSkins!=m_hProjSel && m_pProjTree->IsAncestor(m_hProjPrivStyleSkins,m_hProjSel))//4. 样式-皮肤(私有)列表的子结点
		{
			HDMTREEITEM hRet = NULL;
			if (m_hProjPrivStyleSkins == hPTreeItem)//4.1 DM 窗口及它的私有样式，私有皮肤，sub窗口列表，打开对应的布局xml文件
			{
				hRet = m_hProjSel;
			}
			else if (m_hProjPrivStyleSkins == hP_PTreeItem)//4.2 sub 窗口列表子项,打开它自身的sub xml文件
			{
				hRet = hPTreeItem;
			}
			else if (m_hProjPrivStyleSkins == hP_P_PTreeItem)
			{
				hRet = hP_PTreeItem;
				if (NULL == m_pProjTree->GetPrevSiblingItem(hPTreeItem))
				{
					hRet = m_hProjSel;
				}
			}
			else// 子列的子列
			{
				hRet = hP_P_PTreeItem;
			}

			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hRet);
			CStringW strRelativePath = pNode->Attribute(XML_PATH);
			strPath += strRelativePath;
			break;
		}

		//5.主题包列表，打开themes文件夹下的dmindex.xml
		if (m_hProjThemes == m_hProjSel)
		{
			strPath += XML_THEMES_DMINDEX;
			break;
		}

		if (m_pProjTree->IsAncestor(m_hProjThemes,m_hProjSel))// 前面过滤了主题包列表
		{
			HDMTREEITEM hRet = NULL;
			if (m_hProjThemes == hPTreeItem)//6.各主题包及它的子项类型,打开自己文件夹下的dmindex.xml
			{
				hRet = m_hProjSel;
			}
			else if (m_hProjThemes == hP_PTreeItem)
			{
				hRet = hPTreeItem;
			}
			else if (m_hProjThemes == hP_P_PTreeItem)//7. 各主题包的文件(图片),打开自身文件
			{
				DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
				CStringW strRelativePath = pNode->Attribute(XML_PATH);
				strPath += strRelativePath;
				break;
			}

			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hRet);
			CStringW strRelativePath = pNode->Attribute(XML_PATH);
			strPath += strRelativePath;
			break;
		}
	} while (false);
	//8. 其余全部默认打开根文件夹目录
	OpenFolderAndSelectFile(strPath);
	return DM_ECODE_OK;
}

DMCode ProjXml::ProjMenu_Scan()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{     
		DMSmartPtrT<ScanDlg> pScanDlg;pScanDlg.Attach(new ScanDlg);
		pScanDlg->DoModal(L"ds_scandlg",g_pMainWnd->m_hWnd,true);
		    
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;  
}

DMCode ProjXml::ProjMenu_AddSkinPool()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{  
		DMSmartPtrT<PoolDlg> pDlg;pDlg.Attach(new PoolDlg);
		pDlg->DoModal(L"ds_pooldlg",g_pMainWnd->m_hWnd,true);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_DelSkinPool()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{  
		if (IDOK == DM_MessageBox(L"删除皮肤池是有风险的\r\n请确认此皮肤池中皮肤未被使用？",MB_OKCANCEL))
		{
			//1.取得此皮肤池父结点
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
			DMXmlNodePtr pParentNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hParent);
	
			//2. 移除自身的全部子结点的map
			HDMTREEITEM hChild = m_pProjTree->GetChildItem(m_hProjSel);
			while (hChild)
			{
				RemoveKey(hChild);
				hChild = m_pProjTree->GetNextSiblingItem(hChild);
			}
			
			//3.从父xml节点移除皮肤池结点
			pParentNode->RemoveChildNode(pNode);

			//4.将对应xml设置为未保存
			SetDocUnSave(pParentNode);

			//5.移除自身的map+tree
			RemoveKey(m_hProjSel);
			m_pProjTree->RemoveItem(m_hProjSel);
		}
		
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_EditSkinPool()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
		DMSmartPtrT<PoolDlg> pDlg;pDlg.Attach(new PoolDlg(true,true,pNode->Attribute(XML_NAME)));
		pDlg->DoModal(L"ds_pooldlg",g_pMainWnd->m_hWnd,true);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_AddStylePool()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<PoolDlg> pDlg;pDlg.Attach(new PoolDlg(false));
		pDlg->DoModal(L"ds_pooldlg",g_pMainWnd->m_hWnd,true);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_DelStylePool()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"删除样式池是有风险的\r\n请确认此样式池中样式未被使用？",MB_OKCANCEL))
		{
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
			DMXmlNodePtr pParentNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hParent);

			HDMTREEITEM hChild = m_pProjTree->GetChildItem(m_hProjSel);
			while (hChild)
			{
				RemoveKey(hChild);
				hChild = m_pProjTree->GetNextSiblingItem(hChild);
			}

			pParentNode->RemoveChildNode(pNode);
			SetDocUnSave(pParentNode);

			RemoveKey(m_hProjSel);
			m_pProjTree->RemoveItem(m_hProjSel);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_EditStylePool()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
		DMSmartPtrT<PoolDlg> pDlg;pDlg.Attach(new PoolDlg(false,true,pNode->Attribute(XML_NAME)));
		pDlg->DoModal(L"ds_pooldlg",g_pMainWnd->m_hWnd,true);
		iErr = DM_ECODE_OK;

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_AddSkin()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<SkinDlg> pSkinDlg;pSkinDlg.Attach(new SkinDlg());
		pSkinDlg->DoModal(L"ds_skindlg",g_pMainWnd->m_hWnd,true);
			
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_DelSkin()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"删除皮肤是有风险的\r\n请确认此皮肤未被使用？",MB_OKCANCEL))
		{
			//1. 从所属皮肤池中移除xml结点
			HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
			DMXmlNodePtr pParentNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hParent);
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			pParentNode->RemoveChildNode(pNode);
			
			//2. 从父tree中删除此皮肤结点
			RemoveKey(m_hProjSel);
			m_pProjTree->RemoveItem(m_hProjSel);

			//3. 设置xml为未保存状态
			SetDocUnSave(pParentNode);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}
  
DMCode ProjXml::ProjMenu_EditSkin()
{  
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{ 
		DMSmartPtrT<SkinDlg> pSkinDlg;pSkinDlg.Attach(new SkinDlg(true));
		pSkinDlg->DoModal(L"ds_skindlg",g_pMainWnd->m_hWnd,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_AddStyle()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<StyleDlg> pStyleDlg;pStyleDlg.Attach(new StyleDlg(false));
		pStyleDlg->DoModal(L"ds_styledlg",g_pMainWnd->m_hWnd,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_DelStyle()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"删除样式是有风险的\r\n请确认此样式未被使用？",MB_OKCANCEL))
		{
			//1. 从所属样式池中移除xml节点
			HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
			DMXmlNodePtr pParentNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hParent);
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			pParentNode->RemoveChildNode(pNode);

			//2. 从父tree中删除此结点
			RemoveKey(m_hProjSel);
			m_pProjTree->RemoveItem(m_hProjSel);

			//3. 设置xml为未保存状态
			SetDocUnSave(pParentNode);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_EditStyle()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<StyleDlg> pStyleDlg;pStyleDlg.Attach(new StyleDlg(true));
		pStyleDlg->DoModal(L"ds_styledlg",g_pMainWnd->m_hWnd,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_AddImgType()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<ImgTypeDlg> pDlg;pDlg.Attach(new ImgTypeDlg());
		pDlg->DoModal(L"ds_pooldlg",g_pMainWnd->m_hWnd,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_DelImgType()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"删除Img类型是有风险的\r\n请确认此Img类型中Img未被使用？",MB_OKCANCEL))
		{
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
			DMXmlNodePtr pParentNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hParent);
			CStringW strPath = m_strResDir + pParentNode->Attribute(XML_PATH);
			DocDataPtr pDoc  = FindDocData(strPath);
			CStringW strThemeName = pParentNode->Attribute(XML_NAME);
			if (NULL == pDoc)
			{
				DM_MessageBox(L"此主题包为空,请先扫描错误",MB_OK);
				break;
			}
			//1.移除自身的全部子结点的map,同时移除Res中的项
			CStringW strType = pNode->GetName();
			HDMTREEITEM hChild = m_pProjTree->GetChildItem(m_hProjSel);
			while (hChild)
			{
				DMXmlNodePtr pChildNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hChild);
				m_pRes->RemoveResItem(strType,pChildNode->Attribute(XML_NAME),strThemeName);
				RemoveKey(hChild);
				hChild = m_pProjTree->GetNextSiblingItem(hChild);
			}

			//2.移除自身xml节点，并设置xml为未保存状态
			pDoc->m_XmlRoot.RemoveChildNode(pNode);
			SetDocUnSave(strPath);

			//3.移除自身的map和tree结点
			RemoveKey(m_hProjSel);
			m_pProjTree->RemoveItem(m_hProjSel);

			//4. 自动体检
			DMSmartPtrT<ScanDlg> pScanDlg;pScanDlg.Attach(new ScanDlg);
			pScanDlg->DoModal(L"ds_scandlg",g_pMainWnd->m_hWnd,true);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_EditImgType()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"改变Img类型是极端危险的!!\r\n请确认它的所有子项从未被任何skin使用过!!\r\n修改保存后请重新加载Res",MB_OKCANCEL))
		{
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			DMSmartPtrT<ImgTypeDlg> pDlg;pDlg.Attach(new ImgTypeDlg(true,pNode->GetName()));
			pDlg->DoModal(L"ds_pooldlg",g_pMainWnd->m_hWnd,true);
		} 

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_AddTheme()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<ThemeDlg> pDlg;pDlg.Attach(new ThemeDlg());
		pDlg->DoModal(L"ds_themedlg",g_pMainWnd->m_hWnd,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_DelTheme()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"删除主题包是有风险的\r\n请确认此主题包中图片未被skin使用？",MB_OKCANCEL))
		{
			//1.取得父结点,从父结点的xml中移除此主题包结点
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
			DMXmlNodePtr pParentNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hParent);
			pParentNode->RemoveChildNode(pNode);

			//2.如果删除的是是默认主题包,则把父结点的相关属性清空
			CStringW strThemeName = pNode->Attribute(XML_NAME);
			if (m_hProjDefTheme == m_hProjSel)
			{
				pParentNode->RemoveAttribute(XML_NAME);
				m_hProjDefTheme = NULL;
			}
		

			//3.移除自身的全部子结点的map,同时移除Res中的项
			HDMTREEITEM hType = m_pProjTree->GetChildItem(m_hProjSel);
			while (hType)
			{
				DMXmlNodePtr pTypeNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hType);
				CStringW strType = pTypeNode->GetName();
				HDMTREEITEM hImg = m_pProjTree->GetChildItem(hType);
				while (hImg)
				{
					DMXmlNodePtr pChildNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hImg);
					m_pRes->RemoveResItem(strType,pChildNode->Attribute(XML_NAME),strThemeName);
					RemoveKey(hImg);
					hImg = m_pProjTree->GetNextSiblingItem(hImg);
				}
				RemoveKey(hType);
				hType = m_pProjTree->GetNextSiblingItem(hType);
			}

			//4.设置node所在xml未保存状态
			SetDocUnSave(pParentNode);

			//5.移除自身的map+tree
			RemoveKey(m_hProjSel);
			m_pProjTree->RemoveItem(m_hProjSel);

			//6.自动体检
			DMSmartPtrT<ScanDlg> pScanDlg;pScanDlg.Attach(new ScanDlg);
			pScanDlg->DoModal(L"ds_scandlg",g_pMainWnd->m_hWnd,true);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_EditTheme()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
		CStringW strThemeName = pNode->Attribute(XML_NAME);
		CStringW strDir = pNode->Attribute(XML_PATH);
		strDir = strDir.Left(strDir.ReverseFind(L'\\'));
		strDir = strDir.Right(strDir.GetLength()-strDir.ReverseFind(L'\\')-1);
		DMSmartPtrT<ThemeDlg> pDlg;pDlg.Attach(new ThemeDlg(true,strThemeName,strDir));
		pDlg->DoModal(L"ds_themedlg",g_pMainWnd->m_hWnd,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_SetDefTheme()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		//1. 改变tree上的默认主题显示皮肤
		DMXmlNodePtr pThemesNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjThemes);
		CStringW strDefThemeName = pThemesNode->Attribute(XML_NAME);
		if (NULL != m_hProjDefTheme)
		{
			DM::LPTVITEMEX pDefData = m_pProjTree->GetItem(m_hProjDefTheme);
			pDefData->pPanel->SetAttribute(XML_SKIN,XML_DATASKIN);
		}

		DM::LPTVITEMEX pData = m_pProjTree->GetItem(m_hProjSel);
		pData->pPanel->SetAttribute(XML_SKIN,XML_DEFSKIN);

		// 2.更新父xml结点默认主题包名称
		DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
		pThemesNode->SetAttribute(XML_NAME,pNode->Attribute(XML_NAME));

		m_hProjDefTheme = m_hProjSel;

		// 3.将themes的dmindex.xml设置为未保存状态
		SetDocUnSave(pThemesNode);

		// 4.把Res解析器中默认主题的名字换掉
		m_pRes->SetCurTheme(pNode->Attribute(XML_NAME));

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_AddImg()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{

		DMSmartPtrT<ImgDlg> pDlg;pDlg.Attach(new ImgDlg());
		pDlg->DoModal(L"ds_imgdlg",g_pMainWnd->m_hWnd,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_DelImg()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"删除Img是有风险的\r\n请确认此Img未被skin使用？",MB_OKCANCEL))
		{
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			HDMTREEITEM hType = m_pProjTree->GetParentItem(m_hProjSel);
			DMXmlNodePtr pTypeNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hType);
			HDMTREEITEM hTheme = m_pProjTree->GetParentItem(hType);
			DMXmlNodePtr pThemeNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hTheme);

			//1.从父xml中移除自身,并设置成未保存状态
			pTypeNode->RemoveChildNode(pNode);
			SetDocUnSave(pTypeNode);

			//2.从Res解析器中移除
			CStringW strType = pTypeNode->GetName();
			CStringW strThemeName = pThemeNode->Attribute(XML_NAME);
			m_pRes->RemoveResItem(strType,pNode->Attribute(XML_NAME),strThemeName);

			//3.移除自身map+tree
			RemoveKey(m_hProjSel);
			m_pProjTree->RemoveItem(m_hProjSel);

			//4.自动体检
			DMSmartPtrT<ScanDlg> pScanDlg;pScanDlg.Attach(new ScanDlg);
			pScanDlg->DoModal(L"ds_scandlg",g_pMainWnd->m_hWnd,true);
		}
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_EditImg()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"改变Img是危险的!!\r\n请确认它从未被任何skin使用过!!",MB_OKCANCEL))
		{
			DMSmartPtrT<ImgDlg> pDlg;pDlg.Attach(new ImgDlg(true));
			pDlg->DoModal(L"ds_imgdlg",g_pMainWnd->m_hWnd,true);
		} 

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_AddLayout()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<LayoutDlg> pDlg;pDlg.Attach(new LayoutDlg());
		pDlg->DoModal(L"ds_layoutdlg",g_pMainWnd->m_hWnd,true);
	
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_DelLayout()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (IDOK == DM_MessageBox(L"删除LAYOUT是有风险的\r\n请确认此LAYOUT未被使用？",MB_OKCANCEL))
		{
			DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
			HDMTREEITEM hType = m_pProjTree->GetParentItem(m_hProjSel);
			DMXmlNodePtr pTypeNode = (DMXmlNodePtr)m_pProjTree->GetItemData(hType);

			//1.从父xml中移除自身,并设置成未保存状态
			pTypeNode->RemoveChildNode(pNode);
			SetDocUnSave(pTypeNode);

			//2.从Res解析器中移除
			CStringW strType = pTypeNode->GetName();
			m_pRes->RemoveResItem(strType,pNode->Attribute(XML_NAME),NULL);

			//3.移除自身map+tree
			RemoveKey(m_hProjSel);
			m_pProjTree->RemoveItem(m_hProjSel);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::ProjMenu_EditLayout()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMSmartPtrT<LayoutDlg> pDlg;pDlg.Attach(new LayoutDlg(true));
		pDlg->DoModal(L"ds_layoutdlg",g_pMainWnd->m_hWnd,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::InitCustomProjMenu(DMXmlNode& XmlNode)
{
	DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
	XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_OPENDIR-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_OPENDIR-PROJMENU_BASE].text);
	XmlItem = XmlNode.InsertChildNode(XML_ITEM);
	XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_SCAN-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_SCAN-PROJMENU_BASE].text);
	Init_Debug_XmlBuf(XmlNode);

	return DM_ECODE_OK;
}  

DMCode ProjXml::InitSkinPoolsProjMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hProjSel)
		{
			break;
		}
		
		CStringW strText = GetProjTreeItemText(m_hProjSel);
		if (0 == strText.CompareNoCase(PROJTREE_GLOBALSKINS) ||0 == strText.CompareNoCase(PROJTREE_PRIVSKINS))
		{
			XmlNode.InsertChildNode(XML_SEP);
			DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
			XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_ADDSKINPOOL-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_ADDSKINPOOL-PROJMENU_BASE].text);
			Init_Debug_XmlBuf(XmlNode);
			iErr = DM_ECODE_OK;
			break;
		}

		HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
		if (hParent)
		{
			strText = GetProjTreeItemText(hParent);
			if (0 == strText.CompareNoCase(PROJTREE_GLOBALSKINS) ||0 == strText.CompareNoCase(PROJTREE_PRIVSKINS))
			{
				XmlNode.InsertChildNode(XML_SEP);
				DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_DELSKINPOOL-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_DELSKINPOOL-PROJMENU_BASE].text);
				XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_EDITSKINPOOL-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_EDITSKINPOOL-PROJMENU_BASE].text);;
				Init_Debug_XmlBuf(XmlNode);
				iErr = DM_ECODE_OK;
				break;
			}
		}
	} while (false);
	return iErr;
	
	return DM_ECODE_OK;
}

DMCode ProjXml::InitStylePoolsProjMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hProjSel)
		{
			break;
		}

		CStringW strText = GetProjTreeItemText(m_hProjSel);
		if (0 == strText.CompareNoCase(PROJTREE_GLOBALSTYLS) ||0 == strText.CompareNoCase(PROJTREE_PRIVSTYLES))
		{
			XmlNode.InsertChildNode(XML_SEP);
			DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
			XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_ADDSTYLEPOOL-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_ADDSTYLEPOOL-PROJMENU_BASE].text);
			Init_Debug_XmlBuf(XmlNode);
			iErr = DM_ECODE_OK;
			break;
		}

		HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
		if (hParent)
		{
			strText = GetProjTreeItemText(hParent);
			if (0 == strText.CompareNoCase(PROJTREE_GLOBALSTYLS) ||0 == strText.CompareNoCase(PROJTREE_PRIVSTYLES))
			{
				XmlNode.InsertChildNode(XML_SEP);
				DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_DELSTYLEPOOL-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_DELSTYLEPOOL-PROJMENU_BASE].text);
				XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_EDITSTYLEPOOL-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_EDITSTYLEPOOL-PROJMENU_BASE].text);;
				Init_Debug_XmlBuf(XmlNode);
				iErr = DM_ECODE_OK;
				break;
			}
		}

	} while (false);
	return iErr;

	return DM_ECODE_OK;
}

DMCode ProjXml::InitSkinProjMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hProjSel)
		{
			break;
		}
		HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
		if (hParent)
		{
			CStringW strText = GetProjTreeItemText(hParent);
			if (0 == strText.CompareNoCase(PROJTREE_GLOBALSKINS) ||0 == strText.CompareNoCase(PROJTREE_PRIVSKINS))
			{
				XmlNode.InsertChildNode(XML_SEP);
				DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_ADDSKIN-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_ADDSKIN-PROJMENU_BASE].text);
				Init_Debug_XmlBuf(XmlNode);
				iErr = DM_ECODE_OK;
				break;
			}

			HDMTREEITEM hParentParent = m_pProjTree->GetParentItem(hParent);
			if (hParentParent)
			{
				strText = GetProjTreeItemText(hParentParent);
				if (0 == strText.CompareNoCase(PROJTREE_GLOBALSKINS) ||0 == strText.CompareNoCase(PROJTREE_PRIVSKINS))
				{
					XmlNode.InsertChildNode(XML_SEP);
					DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
					XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_DELSKIN-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_DELSKIN-PROJMENU_BASE].text);
					XmlItem = XmlNode.InsertChildNode(XML_ITEM);
					XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_EDITSKIN-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_EDITSKIN-PROJMENU_BASE].text);
					Init_Debug_XmlBuf(XmlNode);
					iErr = DM_ECODE_OK;
					break;
				}
			}
		}
	} while (false);
	return iErr;
}

DMCode ProjXml::InitStyleProjMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hProjSel)
		{
			break;
		}
		HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
		if (hParent)
		{
			CStringW strText = GetProjTreeItemText(hParent);
			if (0 == strText.CompareNoCase(PROJTREE_GLOBALSTYLS) ||0 == strText.CompareNoCase(PROJTREE_PRIVSTYLES))
			{
				XmlNode.InsertChildNode(XML_SEP);
				DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_ADDSTYLE-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_ADDSTYLE-PROJMENU_BASE].text);
				Init_Debug_XmlBuf(XmlNode);
				iErr = DM_ECODE_OK;
				break;
			}

			HDMTREEITEM hParentParent = m_pProjTree->GetParentItem(hParent);
			if (hParentParent)
			{
				strText = GetProjTreeItemText(hParentParent);
				if (0 == strText.CompareNoCase(PROJTREE_GLOBALSTYLS) ||0 == strText.CompareNoCase(PROJTREE_PRIVSTYLES))
				{
					XmlNode.InsertChildNode(XML_SEP);
					DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
					XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_DELSTYLE-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_DELSTYLE-PROJMENU_BASE].text);
					XmlItem = XmlNode.InsertChildNode(XML_ITEM);
					XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_EDITSTYLE-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_EDITSTYLE-PROJMENU_BASE].text);
					Init_Debug_XmlBuf(XmlNode);
					iErr = DM_ECODE_OK;
					break;
				}
			}
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::InitImgTypeMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hProjSel)
		{
			break;
		}
		HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
		if (hParent)
		{
			CStringW strText = GetProjTreeItemText(hParent);
			if (0 == strText.CompareNoCase(PROJTREE_THEMES))
			{
				XmlNode.InsertChildNode(XML_SEP);
				DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_ADDIMGTYPE-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_ADDIMGTYPE-PROJMENU_BASE].text);
				Init_Debug_XmlBuf(XmlNode);
				iErr = DM_ECODE_OK;
				break;
			}

			HDMTREEITEM hParentParent = m_pProjTree->GetParentItem(hParent);
			if (hParentParent)
			{
				strText = GetProjTreeItemText(hParentParent);
				if (0 == strText.CompareNoCase(PROJTREE_THEMES))
				{
					XmlNode.InsertChildNode(XML_SEP);
					DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
					XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_DELIMGTYPE-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_DELIMGTYPE-PROJMENU_BASE].text);
					XmlItem = XmlNode.InsertChildNode(XML_ITEM);
					XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_EDITIMGTYPE-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_EDITIMGTYPE-PROJMENU_BASE].text);
					Init_Debug_XmlBuf(XmlNode);
					iErr = DM_ECODE_OK;
					break;
				}
			}
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::InitThemeMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hProjSel)
		{
			break;
		}

		CStringW strText = GetProjTreeItemText(m_hProjSel);
		if (0 == strText.CompareNoCase(PROJTREE_THEMES))
		{
			XmlNode.InsertChildNode(XML_SEP);
			DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
			XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_ADDTHEME-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_ADDTHEME-PROJMENU_BASE].text);
			Init_Debug_XmlBuf(XmlNode);
			iErr = DM_ECODE_OK;
			break;
		}

		HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
		if (hParent)
		{	
			strText = GetProjTreeItemText(hParent);
			if (0 == strText.CompareNoCase(PROJTREE_THEMES))
			{
				XmlNode.InsertChildNode(XML_SEP);
				DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_DELTHEME-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_DELTHEME-PROJMENU_BASE].text);
				XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_EDITTHEME-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_EDITTHEME-PROJMENU_BASE].text);
				
				DMXmlNodePtr pThemesNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjThemes);
				CStringW strDefThemeName = pThemesNode->Attribute(XML_NAME);
				DMXmlNodePtr pNode = (DMXmlNodePtr)m_pProjTree->GetItemData(m_hProjSel);
				CStringW strThemeName = pNode->Attribute(XML_NAME);
				XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_SETDEFTHEME-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_SETDEFTHEME-PROJMENU_BASE].text);
				if (0 == strDefThemeName.CompareNoCase(strThemeName))
				{
					XmlItem.SetAttribute(L"bdisable",L"1");
				}

				Init_Debug_XmlBuf(XmlNode);
				iErr = DM_ECODE_OK;
				break;
			}
		}
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::InitImgMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hProjSel)
		{
			break;
		}

		HDMTREEITEM h = m_pProjTree->GetParentItem(m_hProjSel);
		HDMTREEITEM h_h = NULL;
		HDMTREEITEM h_h_h = NULL;
		if (h)
		{
			h_h = m_pProjTree->GetParentItem(h);
			if (h_h)
			{
				h_h_h = m_pProjTree->GetParentItem(h_h);
			}
		}

		if (h_h == m_hProjThemes)
		{
			XmlNode.InsertChildNode(XML_SEP);
			DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
			XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_ADDIMG-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_ADDIMG-PROJMENU_BASE].text);
			Init_Debug_XmlBuf(XmlNode);
			iErr = DM_ECODE_OK;
			break;
		}

		if (h_h_h == m_hProjThemes)
		{
			XmlNode.InsertChildNode(XML_SEP);
			DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
			XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_DELIMG-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_DELIMG-PROJMENU_BASE].text);
			XmlItem = XmlNode.InsertChildNode(XML_ITEM);
			XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_EDITIMG-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_EDITIMG-PROJMENU_BASE].text);
			Init_Debug_XmlBuf(XmlNode);
			iErr = DM_ECODE_OK;
			break;
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode ProjXml::InitLayoutMenu(DMXmlNode& XmlNode)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_hProjSel)
		{
			break;
		}

		HDMTREEITEM hParent = m_pProjTree->GetParentItem(m_hProjSel);
		HDMTREEITEM hParentParent = NULL;
		if (hParent)
		{
			CStringW strText = GetProjTreeItemText(hParent);
			if (0 == strText.CompareNoCase(PROJTREE_LAYOUTFILES))// 父窗口为布局文件列表
			{
				strText = GetProjTreeItemText(m_hProjSel);
				if (0 == strText.CompareNoCase(XML_LAYOUT))// 自身窗口为LAYOUT
				{
					XmlNode.InsertChildNode(XML_SEP);
					DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
					XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_ADDLAYOUT-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_ADDLAYOUT-PROJMENU_BASE].text);
					Init_Debug_XmlBuf(XmlNode);
					iErr = DM_ECODE_OK;
					break;
				}
			}
			hParentParent = m_pProjTree->GetParentItem(hParent);
		}

		if (hParentParent)
		{
			CStringW strText = GetProjTreeItemText(hParentParent);
			if (0 == strText.CompareNoCase(PROJTREE_LAYOUTFILES))// 父窗口的父窗口为布局文件列表
			{
				strText = GetProjTreeItemText(hParent);
				XmlNode.InsertChildNode(XML_SEP);
				DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
				XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_EDITLAYOUT-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_EDITLAYOUT-PROJMENU_BASE].text);
				Init_Debug_XmlBuf(XmlNode);
				if (0 == strText.CompareNoCase(XML_LAYOUT))// 自身窗口为LAYOUT的子窗口
				{
					XmlItem = XmlNode.InsertChildNode(XML_ITEM);
					XmlItem.SetAttribute(XML_ID,IntToString(g_ProjMenuItem[PROJMENU_DELLAYOUT-PROJMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_ProjMenuItem[PROJMENU_DELLAYOUT-PROJMENU_BASE].text);
					Init_Debug_XmlBuf(XmlNode);
				}
				iErr = DM_ECODE_OK;
				break;
			}
		}
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

// private
void ProjXml::PreArrayObjRemove(const DocDataPtr &obj)
{
	delete obj;
}

void ProjXml::PreMapKeyRemove(const DMXmlNodePtr &obj)
{
	delete obj;
}

bool ProjXml::EqualArrayObj(const DocDataPtr &objsrc, const DocDataPtr &objdest)
{
	if (objdest && objsrc && 0 == objdest->m_strXmlPath.CompareNoCase(objsrc->m_strXmlPath))
	{
		return true;
	}
	return false;
}

