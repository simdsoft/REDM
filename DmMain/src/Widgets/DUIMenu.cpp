#include "DmMainAfx.h"
#include "DUIMenu.h"

namespace DM
{
	// ------------------------DUIMenu----------------------------
	BEGIN_MSG_MAP(DUIMenu)
		MSG_WM_INITMENU(OnInitMenu)
		MSG_WM_INITMENUPOPUP(OnInitMenuPopup)
		MSG_WM_MENUSELECT(OnMenuSelect)
		MSG_WM_MEASUREITEM(OnMeasureItem)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
	DUIMenu  *DUIMenu::m_pThis = NULL;
	DUIMenu::DUIMenu()
	{
		m_pDUIMenuXmlInfo.Attach(new DUIMenu_XmlInfo);
		m_hMenuHook		= NULL;
		m_hMenuOwner    = NULL;
		m_pParent		= NULL;
		m_hMenu			= 0;
		m_pThis			= this;
	}
	DUIMenu::~DUIMenu(void)
	{
		UnInstallMenuHook();
		DestroyMenu();
	}

	DMMenuItemData* DUIMenu::ParseItem(DMXmlNode& XmlItem)
	{
		DMMenuItemData *pdmmi = NewMenuItemData();
		pdmmi->itemInfo.iIcon = -1;
		DMAttributeDispatch::ParseInt(XmlItem.Attribute(DMAttr::DUIMenuAttr::ITEM_icon),pdmmi->itemInfo.iIcon);
		pdmmi->itemInfo.strText = XmlItem.Attribute(DMAttr::DUIMenuAttr::ITEM_text);
		pdmmi->itemInfo.pSkin = g_pDMApp->GetSkin(XmlItem.Attribute(DMAttr::DUIMenuAttr::ITEM_skin));
		pdmmi->itemInfo.nHeight = m_pDUIMenuXmlInfo->m_nItemHei;
		DMAttributeDispatch::ParseInt(XmlItem.Attribute(DMAttr::DUIMenuAttr::ITEM_height),pdmmi->itemInfo.nHeight);
		if (pdmmi->itemInfo.pSkin)
		{	
			CSize sz;
			pdmmi->itemInfo.pSkin->GetStateSize(sz);
			if (sz.cy>pdmmi->itemInfo.nHeight)
			{
				pdmmi->itemInfo.nHeight = sz.cy;
			}
		}
		return pdmmi;
	}

	DMMenuItemData* DUIMenu::NewMenuItemData()
	{
		return new DMMenuItemData;
	}

	void DUIMenu::DeleteMenuItemData(DMMenuItemData*pObj)
	{
		delete pObj;
	}

	DUIMenuItem* DUIMenu::NewMenuItem()
	{
		return new DUIMenuItem();
	}

	bool DUIMenu::LoadMenu(LPCWSTR lpszType,LPCWSTR lpszResName)
	{
		bool bRet = false;
		do 
		{
			if (::IsMenu(m_hMenu))
			{// Menu已存在
				break;
			}

			DMXmlDocument XmlDoc;
			if (!DMSUCCEEDED(g_pDMApp->InitDMXmlDocument(XmlDoc, lpszType,lpszResName)))
			{
				break;
			}

			DMXmlNode XmlNode = XmlDoc.Root();
			if (!XmlNode.IsValid())
			{
				break;
			}

			bRet = LoadMenu(XmlNode);
		} while (false);
		return bRet;
	}

	bool DUIMenu::LoadMenu(DMXmlNode &XmlNode)
	{
		bool bRet = false;
		do 
		{
			m_hMenu = ::CreatePopupMenu();
			if (NULL == m_hMenu)
			{
				break;
			}

			if (!DMSUCCEEDED(m_pDUIMenuXmlInfo->InitDMData(XmlNode)))
			{
				::DestroyMenu(m_hMenu);
				m_hMenu = NULL;
				break;
			}

			BuildMenu(m_hMenu,XmlNode);
			bRet = true;
		} while (false);
		return bRet;
	}

	int DUIMenu::TrackPopupMenu(UINT uFlags, int x, int y, HWND hWnd, LPCRECT lpRect)
	{
		int iRet = 0;
		do 
		{
			if (!::IsMenu(m_hMenu))
			{
				break;
			}
			m_hMenuOwner = hWnd;
			ATOM Atom = g_pDMApp->GetClassAtom(true);  // 是否使用阴影窗口类创建
			if (NULL == m_hWnd
				&&!CreateWindowEx((LPCWSTR)Atom,NULL, WS_POPUP, WS_EX_NOACTIVATE, 0, 0, 0, 0, NULL, NULL))
			{
				break;
			}
			UINT uNewFlags = uFlags|TPM_RETURNCMD;
			InstallMenuHook();
			iRet = ::TrackPopupMenu(m_hMenu,uNewFlags,x,y,0,m_hWnd,lpRect);
			UnInstallMenuHook();
			DestroyWindow();
			m_hMenuOwner = NULL;
			if (iRet&&!(uFlags&TPM_RETURNCMD)) 
			{
				::SendMessage(hWnd,WM_COMMAND,iRet,0);
			}
			m_hWnd = NULL;// 窗口释放时，必须清空
		} while (false);
		return iRet;
	}

	DUIMenu DUIMenu::GetSubMenu(int nPos)
	{
		HMENU hSubMenu = ::GetSubMenu(m_hMenu,nPos);
		DUIMenu ret;
		ret.m_pParent  = this;
		ret.m_hMenu    = hSubMenu;
		ret.m_pDUIMenuXmlInfo = m_pDUIMenuXmlInfo;
		return ret;
	}

	bool DUIMenu::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,LPCTSTR strText, int iIcon)
	{
		bool bRet = false;
		do 
		{
			nFlags |= MF_OWNERDRAW;
			if (nFlags&MF_SEPARATOR)
			{
				::InsertMenu(m_hMenu,nPosition,nFlags,(UINT_PTR)0,(LPCTSTR)NULL);
				bRet = true;
				break;// 跳出
			}

			DMMenuItemData* pMenuData	= new DMMenuItemData;
			pMenuData->hMenu			= m_hMenu;
			pMenuData->itemInfo.iIcon	= iIcon;
			pMenuData->itemInfo.nHeight = m_pDUIMenuXmlInfo->m_nItemHei;
			pMenuData->itemInfo.pSkin	= NULL;// 此功能暂不测试
			pMenuData->itemInfo.strText = strText;
	
			if (nFlags&MF_POPUP)
			{
				//插入子菜单，
				DUIMenu *pSubMenu = (DUIMenu*)(LPVOID)nIDNewItem;
				pMenuData->nID    = (UINT_PTR)pSubMenu->m_hMenu;
			}
			else
			{
				pMenuData->nID = nIDNewItem;
			}

			if (!::InsertMenu(m_hMenu,nPosition,nFlags,pMenuData->nID,(LPCTSTR)pMenuData))
			{
				delete pMenuData;
				break;
			}

			DUIMenu *pRootMenu = this;
			while (pRootMenu->m_pParent)
				pRootMenu = pRootMenu->m_pParent;

			// 将分配的内存放到根菜单的内存节点列表中
			pRootMenu->m_DmmiArray.Add(pMenuData);

			if (nFlags&MF_POPUP)
			{
				DUIMenu *pSubMenu = (DUIMenu*)(LPVOID)nIDNewItem;
				for (UINT i=0; i<pSubMenu->m_DmmiArray.GetCount(); i++)
				{
					pRootMenu->m_DmmiArray.Add(pSubMenu->m_DmmiArray[i]);
				}
				pSubMenu->m_DmmiArray.RemoveAll();
				pSubMenu->m_pParent = this;
				bRet = true;
			}
		} while (false);
		return bRet;
	}

	void DUIMenu::DestroyMenu()
	{
		if (!m_pParent)
		{
			if (m_hMenu) 
			{
				::DestroyMenu(m_hMenu);
			}
			for (UINT i=0; i<m_DmmiArray.GetCount(); i++) 
			{
				DeleteMenuItemData(m_DmmiArray[i]);
			}
			m_DmmiArray.RemoveAll();
		}
	}

	void DUIMenu::OnInitMenu(HMENU menu)
	{
		::SendMessage(m_hMenuOwner,WM_INITMENU,(WPARAM)menu,0);
	}

	void DUIMenu::OnInitMenuPopup(HMENU menuPopup, UINT nIndex, BOOL bSysMenu)
	{
		::SendMessage(m_hMenuOwner,WM_INITMENUPOPUP,(WPARAM)menuPopup,MAKELPARAM(nIndex,bSysMenu));
	}

	void DUIMenu::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU menu)
	{
		::SendMessage(m_hMenuOwner,WM_MENUSELECT,MAKEWPARAM(nItemID,nFlags),(LPARAM)menu);
	}

	void DUIMenu::OnMeasureItem(UINT /*wParam*/,LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
		do 
		{
			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			if (ODT_MENU != lpMeasureItemStruct->CtlType)
			{
				break;
			}

			DMMenuItemData *pdmmi = (DMMenuItemData*)lpMeasureItemStruct->itemData;
			if (pdmmi)
			{// Item
				lpMeasureItemStruct->itemHeight = pdmmi->itemInfo.nHeight;
				//
				lpMeasureItemStruct->itemWidth = m_pDUIMenuXmlInfo->m_szIcon.cx+m_pDUIMenuXmlInfo->m_IconOffset;

				int nSkinWidth = 0;
				if (pdmmi->itemInfo.pSkin)
				{
					CSize sz;
					pdmmi->itemInfo.pSkin->GetStateSize(sz);
					nSkinWidth = sz.cx;
				}

				int nTextWidth = 0;
				if (!pdmmi->itemInfo.strText.IsEmpty())
				{
					DMSmartPtrT<IDMCanvas> pCanvas;
					pRender->CreateCanvas(0,0,&pCanvas);
					DMSmartPtrT<IDMFont> pOldFont;
					pCanvas->SelectObject(m_pDUIMenuXmlInfo->m_hFont,(IDMMetaFile**)&pOldFont);
					SIZE szText;
					CStringW strTrans = g_pDMApp->GetTrans(pdmmi->itemInfo.strText);
					pCanvas->MeasureText(strTrans,strTrans.GetLength(),&szText);
					pCanvas->SelectObject(pOldFont);
					nTextWidth = szText.cx+m_pDUIMenuXmlInfo->m_TextOffset;
				}

				lpMeasureItemStruct->itemWidth += (nSkinWidth>nTextWidth?nSkinWidth:nTextWidth);
			}
			else
			{// Step
				CSize sz;
				if (m_pDUIMenuXmlInfo->m_pSepSkin)
				{
					m_pDUIMenuXmlInfo->m_pSepSkin->GetStateSize(sz);
				}
				lpMeasureItemStruct->itemHeight = m_pDUIMenuXmlInfo->m_pSepSkin?sz.cy:3;
				lpMeasureItemStruct->itemWidth  = 0;
			}

			if (!m_pDUIMenuXmlInfo->m_bAutoCalc)//如果是指定宽度，就使用指定宽度
			{
				lpMeasureItemStruct->itemWidth = m_pDUIMenuXmlInfo->m_MaxWidth;
			}

			if (m_pDUIMenuXmlInfo->m_bAutoCalc
				&&0!=lpMeasureItemStruct->itemWidth
				&&(int)lpMeasureItemStruct->itemWidth>m_pDUIMenuXmlInfo->m_MaxWidth)
			{
				m_pDUIMenuXmlInfo->m_MaxWidth = lpMeasureItemStruct->itemWidth;//如果不是指定宽度， 自动计算出最大宽度
			}

		} while (FALSE);
	}

	// 辅助
	void DUIMenu::BuildMenu(HMENU menuPopup,DMXmlNode xmlNode)
	{
		DMXmlNode XmlItem = xmlNode.FirstChild();
		while (XmlItem.IsValid())
		{
			if (0 == _wcsicmp(DMAttr::DUIMenuAttr::NODE_sep,XmlItem.GetName()))
			{
				AppendMenu(menuPopup,MF_SEPARATOR|MF_OWNERDRAW,(UINT_PTR)0,(LPCTSTR)NULL);
			}
			else if(0 == _wcsicmp(DMAttr::DUIMenuAttr::NODE_item,XmlItem.GetName()))
			{
				DMMenuItemData *pdmmi = ParseItem(XmlItem);
				if (NULL == pdmmi)
				{
					DMASSERT_EXPR(FALSE,L"解析Menu的xml失败！");
					XmlItem = XmlItem.NextSibling();
					continue;
				}

				pdmmi->hMenu = menuPopup;

				bool bCheck = false,bRadio = false,bDisable = false;
				DMAttributeDispatch::ParseBool(XmlItem.Attribute(DMAttr::DUIMenuAttr::ITEM_bcheck),bCheck);
				DMAttributeDispatch::ParseBool(XmlItem.Attribute(DMAttr::DUIMenuAttr::ITEM_bradio),bRadio);
				DMAttributeDispatch::ParseBool(XmlItem.Attribute(DMAttr::DUIMenuAttr::ITEM_bdisable),bDisable);

				int nId = 0;
				DMAttributeDispatch::ParseInt(XmlItem.Attribute(DMAttr::DUIMenuAttr::ITEM_id),nId);
				DMXmlNode XmlChild = XmlItem.FirstChild();
				if (!XmlChild.IsValid())
				{
					pdmmi->nID = nId;
					UINT uFlag = MF_OWNERDRAW;// 自绘
					if (bCheck)
					{
						uFlag |= MF_CHECKED;
					}
					if (bDisable)
					{
						uFlag |= MF_GRAYED;
					}
					if (bRadio)
					{
						uFlag |= MFT_RADIOCHECK|MF_CHECKED;
					}
					// 具体看MSDN
					::AppendMenu(menuPopup,uFlag,(UINT_PTR)pdmmi->nID,(LPCTSTR)pdmmi);
				}
				else
				{
					HMENU hSubMenu = ::CreatePopupMenu();
					pdmmi->nID = (UINT_PTR)hSubMenu;
					UINT uFlag = MF_OWNERDRAW|MF_POPUP;
					if (bDisable)
					{
						uFlag |= MF_GRAYED;
					}
					AppendMenu(menuPopup,uFlag,(UINT_PTR)hSubMenu,(LPCTSTR)pdmmi);
					BuildMenu(hSubMenu,XmlItem);// 轮询子控件
				}
				m_DmmiArray.Add(pdmmi);
			}

			XmlItem = XmlItem.NextSibling();
		}
	}

	void DUIMenu::InstallMenuHook()
	{
		m_pThis = this;// DUIMenu对象如果不是临时变量，那么在每次弹出时，先要把m_pThis重新赋值,以防m_pThis的值改变
		if (NULL == m_hMenuHook)
		{
			m_hMenuHook = ::SetWindowsHookEx(WH_CALLWNDPROC, MenuHookProc, GetModuleHandle(NULL), ::GetCurrentThreadId());
		}
	}

	void DUIMenu::UnInstallMenuHook()
	{
		if (m_hMenuHook)
		{
			::UnhookWindowsHookEx(m_hMenuHook);
			m_hMenuHook = NULL;
		}
		m_pThis = NULL;
	}

	LRESULT CALLBACK DUIMenu::MenuHookProc(int iCode, WPARAM wParam, LPARAM lParam)
	{
		do 
		{
			if( HC_ACTION != iCode)
			{
				break;
			}
			CWPSTRUCT *pwt = (CWPSTRUCT*)lParam;
			if (pwt && 
				WM_CREATE == pwt->message
				&& (iCode >= 0))
			{
				TCHAR szClsName[64];
				::GetClassName(pwt->hwnd, szClsName, 64);
				if (m_pThis
					&&0 == lstrcmpi(szClsName,_T("#32768")))
				{
					if (!::GetProp(pwt->hwnd, DMMENU_PROP_OBJ))
					{
						DUIMenuItem *pObj = m_pThis->NewMenuItem();
						pObj->m_hMenuOwner = m_pThis->m_hMenuOwner;
						pObj->m_pDUIMenuXmlInfo = m_pThis->m_pDUIMenuXmlInfo;
						pObj->AddHook(pwt->hwnd,DUIMenuItem::SubMenuProc);
					}
				}
			}
		} while (false);
		//Windows NT/XP/2003: Ignored.
		return CallNextHookEx(NULL, iCode, wParam, lParam);
	}

}//namespace DM