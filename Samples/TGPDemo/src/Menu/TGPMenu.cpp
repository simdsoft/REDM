#include "TGPDemoAfx.h"
#include "TGPMenu.h"

namespace DM
{
	void TGPMenuItem::OnRelease()
	{
		delete this;// 外部new的对象（TGPMenu::NewMenuItem()）,必须外部释放
	}

	void TGPMenuItem::DrawOwnerItem(IDMCanvas* pCanvas, CRect& rcItem, MENUITEMINFO &mii)
	{
		do 
		{
			__super::DrawOwnerItem(pCanvas,rcItem,mii);
			// TGP新加绘制,如果你想做更多绘制，可以直接把__super::DrawOwnerItem copy过来，在中间加入
			TGPMenuItemData *pdmmi = (TGPMenuItemData*)mii.dwItemData;
			if (pdmmi
				&&pdmmi->bHold
				&&pdmmi->pHoldSkin)
			{
				pdmmi->pHoldSkin->Draw(pCanvas,rcItem, pdmmi->bSel);
			}
		} while (false);
	}

	LRESULT TGPMenuItem::MenuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (NULL != m_hMenu && 
			(MN_BUTTONUP == uMsg || MN_DBLCLK == uMsg))
		{
			return On_MN_BUTTON(hWnd,uMsg,wParam,lParam);
		}
		return __super::MenuProc(hWnd,uMsg,wParam,lParam);
	}

	LRESULT TGPMenuItem::On_MN_BUTTON(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MENUITEMINFO mii;
		::ZeroMemory(&mii,sizeof(MENUITEMINFO));
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask  = MIIM_FTYPE | MIIM_DATA | MIIM_STATE | MIIM_STRING | MIIM_ID | MIIM_BITMAP | MIIM_SUBMENU | MIIM_BITMAP | MIIM_CHECKMARKS;
		if (0 == ::GetMenuItemInfo(m_hMenu, wParam, MF_BYPOSITION, &mii))
		{
			return ::CallWindowProc(m_pOldProc, hWnd, MN_BUTTONUP, wParam, lParam);
		}

		TGPMenuItemData *pdmmi = (TGPMenuItemData*)mii.dwItemData;
		if (pdmmi->bHold&&pdmmi->pHoldSkin)
		{
			pdmmi->bSel = !pdmmi->bSel;
			// 在这里发自定义消息到TGPMainWnd中
			if (m_hMenuOwner&&::IsWindow(m_hMenuOwner))
			{
				::PostMessage(m_hMenuOwner,WM_USER+1010,pdmmi->nID,NULL);
			}
			Render();//这里偷懒了，强制刷新整个菜单,你可以把菜单的指定项单独刷新
			return 0;//0表示不退出菜单
		}

		return ::CallWindowProc(m_pOldProc, hWnd, uMsg, wParam, lParam);;
	}

	// ------------------------TGPMenu----------------------------
	TGPMenu::~TGPMenu()
	{
		UnInstallMenuHook();
		DestroyMenu();/// 此处必须调用，才能走到TGPMenu::DeleteMenuItemData
	} 

	DMMenuItemData* TGPMenu::ParseItem(DMXmlNode& XmlItem)
	{
		TGPMenuItemData* pdmmi = (TGPMenuItemData*)__super::ParseItem(XmlItem);
		if (pdmmi)
		{
			// 解析扩展的MENU
			pdmmi->pHoldSkin = g_pDMApp->GetSkin(XmlItem.Attribute(DMAttr::TGPMenuAttr::ITEM_holdskin));
			pdmmi->bHold = pdmmi->bSel = false;
			DMAttributeDispatch::ParseBool(XmlItem.Attribute(DMAttr::TGPMenuAttr::ITEM_bhold),pdmmi->bHold);
			DMAttributeDispatch::ParseBool(XmlItem.Attribute(DMAttr::TGPMenuAttr::ITEM_bsel),pdmmi->bSel);
			return (DMMenuItemData*)pdmmi;
		}
		return NULL;
	}

	DMMenuItemData* TGPMenu::NewMenuItemData()
	{
		return new TGPMenuItemData();
	}

	void TGPMenu::DeleteMenuItemData(DMMenuItemData*pObj)
	{
		delete (TGPMenuItemData*)pObj;
	}

	DUIMenuItem* TGPMenu::NewMenuItem()
	{
		return new TGPMenuItem();
	}




}//namespace DM