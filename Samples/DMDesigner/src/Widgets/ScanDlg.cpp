#include "DMDesignerAfx.h"
#include "ScanDlg.h"

BEGIN_MSG_MAP(ScanDlg)    
	MSG_WM_SIZE(OnSize) 
	CHAIN_MSG_MAP(DMHDialog)
END_MSG_MAP() 
void ScanDlg::OnSize(UINT nType, CSize size)
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

DMCode ScanDlg::OnOK()
{
	DUITabCtrl *pTab = FindChildByNameT<DUITabCtrl>(L"ds_scantabctrl");
	int iCurSel = pTab->GetCurSel();
	if (0 == iCurSel)//0.点击 开始体检按钮
	{
		pTab->SetCurSel(2);
		SetWindowPos(NULL,0,0,500,200,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);  
		
		Scan();
		if (false == m_bExistErr)//1.提示无错误
		{
			pTab->SetCurSel(1);
			SetWindowPos(NULL,0,0,400,100,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);  
		}
	}
	else if (2 == iCurSel)//2.点击 立即修复按钮
	{
		Scan(true);
		pTab->SetCurSel(3);
	}
	else if (3 == iCurSel||1==iCurSel)// 退出
	{
		EndDialog(IDOK); 
	}
	return DM_ECODE_OK;
}

DMCode ScanDlg::Scan(bool bRepair)
{
	m_bExistErr = false;
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ObjXml* pXml		 = g_pMainWnd->m_pDesignerXml;
		CStringW strResDir	 = pXml->m_strResDir;
		DMSmartPtrT<ResFolder>pRes = pXml->m_pRes;
		ProjTree* pProjTree  = g_pMainWnd->FindChildByNameT<ProjTree>(L"ds_projtree");
		DUIListBox* pListBox = FindChildByNameT<DUIListBox>(bRepair?L"ds_repairlist":L"ds_scanlist");
		pListBox->DeleteAll();

		DMXmlNodePtr pData = NULL;
		CStringW strPath,strErrText;
		//1.扫描布局文件列表，判断它们的ID是否有效
		HDMTREEITEM hLayoutFiles = pXml->m_hProjLayoutFiles;
		HDMTREEITEM hGlobal		 = pXml->m_hProjGlobal;

		if (hGlobal)
		{
			pData		  = (DMXmlNodePtr) pProjTree->GetItemData(hGlobal);
			strPath		  = strResDir + pData->Attribute(XML_PATH);
			
			//1.1.0.判断global文件是否存在
			if (!CheckFileExistW((wchar_t*)(const wchar_t*)strPath))
			{
				strErrText.Format(L"[global]文件未找到:%s",strPath);
				m_bExistErr = true;
				if (bRepair)
				{
					//1.1.1.从父xml节点移除global节点
					HDMTREEITEM hParent = pProjTree->GetParentItem(hGlobal);
					DMXmlNodePtr pParentData = (DMXmlNodePtr) pProjTree->GetItemData(hParent);
					pParentData->RemoveChildNode(pData);

					//1.1.2.设置xml为未保存状态
					strPath = strResDir + XML_LAYOUT_DMINDEX;
					pXml->SetDocUnSave(strPath);

					//1.1.3.从Res解析器中把global移除
					pRes->RemoveResItem(XML_GLOBAL,pData->Attribute(XML_NAME),L"");
	
					strErrText += L"[已移除]";

					//1.1.4.从map中移除
					pXml->DMMapT<HDMTREEITEM,DMXmlNodePtr>::RemoveKey(hGlobal);

					//1.1.5.从tree中移除此global结点，并把global成员变量清0
					pProjTree->RemoveItem(hGlobal);
					pXml->m_hProjGlobal = NULL;		
				}
				pListBox->AddString(strErrText);
			}
		}

		//1.2.0.判断layout文件是否存在
		HDMTREEITEM hLayout = pProjTree->GetChildItem(hLayoutFiles,false);
		if (hLayout)
		{
			HDMTREEITEM hChild = pProjTree->GetChildItem(hLayout);
			while (hChild)
			{
				pData = (DMXmlNodePtr) pProjTree->GetItemData(hChild);
				HDMTREEITEM hTemp = pProjTree->GetNextSiblingItem(hChild);

				strPath = strResDir + pData->Attribute(XML_PATH);

				//1.2.0.判断layout文件是否存在
				if (!CheckFileExistW((wchar_t*)(const wchar_t*)strPath))
				{
					strErrText.Format(L"[layout]文件未找到:%s",strPath);
					m_bExistErr = true;
					if (bRepair)
					{
						//1.2.1.从父xml节点移除layout节点
						DMXmlNodePtr pParentData = (DMXmlNodePtr) pProjTree->GetItemData(hLayout);
						pParentData->RemoveChildNode(pData);

						//1.2.2.设置xml为未保存状态
						strPath = strResDir + XML_LAYOUT_DMINDEX;
						pXml->SetDocUnSave(strPath);

						//1.2.3.从Res解析器中把layout移除
						pRes->RemoveResItem(XML_LAYOUT,pData->Attribute(XML_NAME),L"");

						strErrText += L"[已移除]";

						//1.2.4.从map中移除
						pXml->DMMapT<HDMTREEITEM,DMXmlNodePtr>::RemoveKey(hChild);

						//1.2.5.从tree中移除此layout结点
						pProjTree->RemoveItem(hChild);
					}
					pListBox->AddString(strErrText);
				}

				hChild = hTemp;
			}
		}
		
		//2.扫描主题包列表中的主题列表，判断它们的dmindex.xml是否存在
		HDMTREEITEM hThemes = pXml->m_hProjThemes;
		HDMTREEITEM hTheme = pProjTree->GetChildItem(hThemes);
		while (hTheme)
		{
			HDMTREEITEM hTemp = pProjTree->GetNextSiblingItem(hTheme);
			pData = (DMXmlNodePtr) pProjTree->GetItemData(hTheme);
			strPath = strResDir + pData->Attribute(XML_PATH);
			//2.1.某个主题的dmindex.xml不存在，或解析失败
			if (!pXml->FindDocData(strPath))
			{
				strErrText.Format(L"[theme]dmindex.xml解析失败:%s",strPath);
				m_bExistErr = true;
				if (bRepair)
				{
					//2.2.从父xml节点移除theme节点
					DMXmlNodePtr pParentData = (DMXmlNodePtr) pProjTree->GetItemData(hThemes);
					pParentData->RemoveChildNode(pData);
					
					//2.3.设置xml为未保存状态
					strPath = strResDir + XML_THEMES_DMINDEX;
					pXml->SetDocUnSave(strPath);
					
					strErrText += L"[已移除]";

					//2.4.从map中移除
					pXml->DMMapT<HDMTREEITEM,DMXmlNodePtr>::RemoveKey(hTheme);

					// 2.5.从tree中移除此theme结点
					pProjTree->RemoveItem(hTheme);
				}
				pListBox->AddString(strErrText);
			}
			hTheme = hTemp;
		}

		//3.扫描各主题包中的文件列表，判断它们的ID是否有效
		hTheme = pProjTree->GetChildItem(hThemes);
		while (hTheme)
		{
			HDMTREEITEM hType = pProjTree->GetChildItem(hTheme);
			while (hType)
			{
				HDMTREEITEM hFile = pProjTree->GetChildItem(hType);
				while (hFile)
				{
					HDMTREEITEM hTemp = pProjTree->GetNextSiblingItem(hFile);
					pData = (DMXmlNodePtr) pProjTree->GetItemData(hFile);
					strPath = strResDir + pData->Attribute(XML_PATH);
					//3.0.判断file文件是否存在
					if (!CheckFileExistW((wchar_t*)(const wchar_t*)strPath))
					{
						strErrText.Format(L"[file]未找到:%s",strPath);
						m_bExistErr = true;
						if (bRepair)
						{
							//3.1.从父xml节点移除file节点
							DMXmlNodePtr pParentData = (DMXmlNodePtr) pProjTree->GetItemData(hType);
							pParentData->RemoveChildNode(pData);
							
							DMXmlNodePtr pThemeData = (DMXmlNodePtr) pProjTree->GetItemData(hTheme);
							//3.2.设置xml为未保存状态
							strPath = strResDir + pThemeData->Attribute(XML_PATH);
							pXml->SetDocUnSave(strPath);

							//3.3.从Res解析器中把file移除
							pRes->RemoveResItem(pParentData->GetName(),pData->Attribute(XML_NAME),pThemeData->Attribute(XML_NAME));

							strErrText += L"[已移除]";

							//3.4.从map中移除
							pXml->DMMapT<HDMTREEITEM,DMXmlNodePtr>::RemoveKey(hFile);

							//3.5.从tree中移除此file结点
							pProjTree->RemoveItem(hFile);
						}

						pListBox->AddString(strErrText);
					}

					hFile = hTemp;
				}
				hType = pProjTree->GetNextSiblingItem(hType);
			}

			hTheme = pProjTree->GetNextSiblingItem(hTheme);
		}

		//4.扫描全局皮肤列表，判断它们的ID是否有效
		HDMTREEITEM hGlobalSkins = pXml->m_hProjGlobalSkins;
		if (hGlobalSkins)
		{
			HDMTREEITEM hGlobalSkin = pProjTree->GetChildItem(hGlobalSkins);
			while (hGlobalSkin)
			{
				HDMTREEITEM hGlobalSkinId = pProjTree->GetChildItem(hGlobalSkin);
				while (hGlobalSkinId)
				{
					HDMTREEITEM hTemp = pProjTree->GetNextSiblingItem(hGlobalSkinId);

					//4.1.取得皮肤id
					pData = (DMXmlNodePtr) pProjTree->GetItemData(hGlobalSkinId);
					CStringW strSrc = pData->Attribute(XML_SRC);
					CStringWList strList;
					CStringW strName;
					CStringW strKey;
					int nCount = SplitStringT(strSrc,L':',strList);
					if (1==nCount)
					{
						strKey  = L"PNG";
						strName = strSrc;
					}
					else if (2 == nCount)
					{
						strKey  = strList[0];
						strName = strList[1];
					}
					//4.2.从Res解析器中循环查找此id是否存在
					if (!DMSUCCEEDED(pXml->m_pRes->IsItemExists(strKey,strName)))//循环查找
					{
						strErrText.Format(L"[global-skins]id:%s-src:%s未找到对应皮肤",pData->Attribute(XML_ID),strSrc);
						m_bExistErr = true;
						if (bRepair)
						{
							//4.3.从父xml节点移除皮肤结点
							DMXmlNodePtr pParentData = (DMXmlNodePtr) pProjTree->GetItemData(hGlobalSkin);
							pParentData->RemoveChildNode(pData);

							//4.4.设置xml为未保存状态
							DMXmlNodePtr pGlobal = (DMXmlNodePtr) pProjTree->GetItemData(pXml->m_hProjGlobal);
							strPath = strResDir + pGlobal->Attribute(XML_PATH);
							pXml->SetDocUnSave(strPath);
							// 因为Res不存在，所以不需要移除Res
							
							strErrText += L"[已移除]";
							//4.5.从map中移除
							pXml->DMMapT<HDMTREEITEM,DMXmlNodePtr>::RemoveKey(hGlobalSkinId);

							//4.6.从tree中移除此skin结点
							pProjTree->RemoveItem(hGlobalSkinId);
						}

						pListBox->AddString(strErrText);
					}

					hGlobalSkinId = hTemp;
				}
				hGlobalSkin = pProjTree->GetNextSiblingItem(hGlobalSkin);
			}
		}

		//5.扫描私有皮肤列表，判断它们的ID是否有效
		HDMTREEITEM hPrivStyleSkins = pXml->m_hProjPrivStyleSkins;
		if (hPrivStyleSkins)
		{
			HDMTREEITEM hDM = pProjTree->GetChildItem(hPrivStyleSkins);
			while (hDM)
			{
				HDMTREEITEM hPrivSkins = pProjTree->GetChildItem(hDM,false);// 最后一个是skin
				if (hPrivSkins)
				{
					HDMTREEITEM hPrivSkin = pProjTree->GetChildItem(hPrivSkins);
					while (hPrivSkin)
					{
						HDMTREEITEM hPrivSkinId = pProjTree->GetChildItem(hPrivSkin);
						while (hPrivSkinId)
						{
							HDMTREEITEM hTemp = pProjTree->GetNextSiblingItem(hPrivSkinId);
							pData = (DMXmlNodePtr) pProjTree->GetItemData(hPrivSkinId);
							CStringW strSrc = pData->Attribute(XML_SRC);
							CStringWList strList;
							CStringW strName;
							CStringW strKey;
							int nCount = SplitStringT(strSrc,L':',strList);
							if (1==nCount)
							{
								strKey = L"PNG";
								strName = strSrc;
							}
							else if (2 == nCount)
							{
								strKey = strList[0];
								strName  = strList[1];
							}
							if (!DMSUCCEEDED(pXml->m_pRes->IsItemExists(strKey,strName)))//循环查找
							{
								strErrText.Format(L"[private-skins]id:%s-src:%s未找到对应皮肤",pData->Attribute(XML_ID),strSrc);
								m_bExistErr = true;
								if (bRepair)
								{
									DMXmlNodePtr pParentData = (DMXmlNodePtr) pProjTree->GetItemData(hPrivSkin);
									pParentData->RemoveChildNode(pData);

									DMXmlNodePtr pDM = (DMXmlNodePtr) pProjTree->GetItemData(hDM);
									strPath = strResDir + pDM->Attribute(XML_PATH);
									pXml->SetDocUnSave(strPath);
									// 因为Res不存在，所以不需要移除Res

									strErrText += L"[已移除]";
									pXml->DMMapT<HDMTREEITEM,DMXmlNodePtr>::RemoveKey(hPrivSkinId);
									pProjTree->RemoveItem(hPrivSkinId);

								}
								pListBox->AddString(strErrText);
							}

							hPrivSkinId = hTemp;
						}

						hPrivSkin = pProjTree->GetNextSiblingItem(hPrivSkin);
					}
				}

				hDM = pProjTree->GetNextSiblingItem(hDM);
			}
		}
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}
