#include "DmMainAfx.h"
#include "DUIMenuHelper.h"

namespace DM
{
	DUIMenu_XmlInfo::DUIMenu_XmlInfo()
	{
		m_pBgSkin = m_pItemSelBgSkin = m_pIconSkin = m_pSepSkin = m_pCheckSkin = NULL;
		m_nItemHei	  = 16;
		m_IconOffset  = 0;// icon的偏移会在OnMeasureItem计算的
		m_TextOffset  = 5;
		m_StepOffset  = 7;
		m_szIcon.SetSize(0,0);// icon的大小会在OnMeasureItem计算的
		m_hFont       = NULL;
		m_byAlpha     = 0xff;
		m_StepHei     = 1;
		m_MaxWidth	  = -1;
		m_hFont		  = g_pDMApp->GetFont(L"");
		m_bShadow     = false;
		m_bAutoCalc	  = true;

		m_crBg         = GetSysColor(COLOR_MENU)|0xff000000;
		m_crSelBg      = GetSysColor(COLOR_HIGHLIGHT)|0xff000000;
		m_crStep	   = GetSysColor(COLOR_GRAYTEXT)|0xff000000;
		m_crTextNormal = GetSysColor(COLOR_MENUTEXT)|0xff000000;
		m_crTextSel    = GetSysColor(COLOR_HIGHLIGHTTEXT)|0xff000000;
		m_crTextGray   = GetSysColor(COLOR_GRAYTEXT)|0xff000000;
	}

	DMCode DUIMenu_XmlInfo::OnAttrMaxWidth(LPCWSTR pszValue, bool bLoadXml)
	{
		DMAttributeDispatch::ParseInt(pszValue,m_MaxWidth);
		if (m_MaxWidth>0)
		{
			m_bAutoCalc = false;
		}
		else
		{
			m_bAutoCalc = true;
		}
		return DM_ECODE_OK;
	}

	// DUIMenuItem-----------------------------------------------------
	DUIMenuItem::DUIMenuItem()
	{
		m_hWnd			= NULL;
		m_hMenuOwner	= NULL;
		m_hMenu			= NULL;
		m_pOldProc      = NULL;
		m_pNowProc      = NULL;
		m_bAlpha        = false;
		m_dwCurSel      = 0xFFFFFFFF;
	}

	DUIMenuItem::~DUIMenuItem()
	{
		if (IsMenuWindowExist())
		{
			SetWindowLong(m_hWnd,GWL_WNDPROC, (LONG)m_pOldProc);
			::RemoveProp(m_hWnd, DMMENU_PROP_OBJ);
			m_hWnd			= NULL;
			m_pOldProc		= NULL;
			m_pNowProc      = NULL;
		}
	}

	void DUIMenuItem::OnRelease()
	{
		delete  this;
	}

	LRESULT DUIMenuItem::MenuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
			return On_WM_CREATE(wParam, lParam);
			break;

		case MN_SIZEWINDOW:
			return On_MN_SIZEWINDOW(wParam,lParam);
			break;

		case WM_NCDESTROY:
			return On_WM_NCDESTROY(wParam, lParam);
			break;
		}

		if( NULL != m_hMenu)
		{
			switch(uMsg)
			{
			case MN_SELECTITEM:
				return On_MN_SELECTITEM(wParam,lParam);
				break;

			case WM_KEYFIRST:
				return On_WM_KEYFIRST(wParam, lParam);
				break;

			case WM_ERASEBKGND:
				return TRUE;
				break;

			case WM_NCPAINT:
				return On_WM_NCPAINT(wParam,lParam); 
				break;

			case WM_PAINT:
				return On_WM_PAINT(wParam,lParam);
				break;

			case WM_PRINT:
				return On_WM_PRINT(wParam,lParam);
				break;

			case WM_SIZE:  
				return On_WM_SIZE(wParam,lParam);
				break;		
			}
		}
		return ::CallWindowProc(m_pOldProc, hWnd, uMsg, wParam, lParam);
	}

	void DUIMenuItem::Render(HDC hdc)
	{
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}

			bool bGetDC = false;
			if(NULL == hdc)
			{
				hdc = ::GetWindowDC(m_hWnd);
				bGetDC = true;
			}

			CRect rcClt, rcWnd;
			::GetWindowRect(m_hWnd,&rcWnd);
			::GetClientRect(m_hWnd,&rcClt);
			::ClientToScreen(m_hWnd,(LPPOINT)&rcClt);
			::ClientToScreen(m_hWnd,(LPPOINT)&rcClt+1);

			CRect rcDraw = rcWnd;
			rcDraw.OffsetRect(-rcDraw.TopLeft());

			DMSmartPtrT<IDMCanvas> pCanvas;// 创建临时画布，双缓冲
			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			pRender->CreateCanvas(rcDraw.Width(),rcDraw.Height(),&pCanvas);

			//1. 绘制整个背景
			if (m_pDUIMenuXmlInfo->m_pBgSkin)
			{
				m_pDUIMenuXmlInfo->m_pBgSkin->Draw(pCanvas,rcDraw,0);
			}
			else if(!m_pDUIMenuXmlInfo->m_crBg.IsTextInvalid())
			{
				pCanvas->FillSolidRect(rcDraw,m_pDUIMenuXmlInfo->m_crBg);
			}

			// 2.为了菜单的滚动需要测试第一项的序号,简版就不开定时器绘制滚动了！
			int	  nFirstOffs;
			POINT ptClt = {0, 0};
			::ClientToScreen(m_hWnd,&ptClt);
			int nFirstPos = ::MenuItemFromPoint(NULL, m_hMenu, ptClt);
			if (nFirstPos > 0)
			{
				RECT rcItem = {0};
				::GetMenuItemRect(m_hWnd, m_hMenu, 0, &rcItem);
				nFirstOffs = rcItem.top;
				::GetMenuItemRect(m_hWnd, m_hMenu, nFirstPos, &rcItem);
				nFirstOffs -= rcItem.top;
			}
			else
			{
				nFirstPos  = 0;
				nFirstOffs = 0;
			}
			int nSum = ::GetMenuItemCount(m_hMenu);
			for (int i = nFirstPos; i < nSum ; i++ )
			{
				CRect rcItem;
				::GetMenuItemRect(m_hWnd, m_hMenu, i, &rcItem);
				::OffsetRect(&rcItem, -rcWnd.left, nFirstOffs - rcWnd.top);
				if (rcItem.bottom > (rcWnd.bottom - rcWnd.top))
				{
					break;
				}

				MENUITEMINFO mii;
				::ZeroMemory(&mii,sizeof(MENUITEMINFO));
				mii.cbSize = sizeof(MENUITEMINFO);
				mii.fMask  = MIIM_FTYPE | MIIM_DATA | MIIM_STATE | MIIM_STRING | MIIM_ID | MIIM_BITMAP | MIIM_SUBMENU | MIIM_BITMAP | MIIM_CHECKMARKS;
				if (0 == ::GetMenuItemInfo(m_hMenu, i, MF_BYPOSITION, &mii))
				{
					continue;
				}

				if (mii.fType & MFT_SEPARATOR)
				{
					DrawSeparator(pCanvas,rcItem);
				}
				else
				{
					DrawOwnerItem(pCanvas, rcItem, mii);
				}
			}

			HDC hmemdc = pCanvas->GetDC();
			BitBlt(hdc,0,0,rcWnd.Width(),rcWnd.Height(),hmemdc,0,0,SRCCOPY);
			pCanvas->ReleaseDC(hmemdc);
			if (bGetDC)
			{
				::ReleaseDC(m_hWnd, hdc);
			}
		} while (false);
	}

	void DUIMenuItem::DrawSeparator(IDMCanvas* pCanvas, CRect& rcItem)
	{
		CRect rcDraw = rcItem;
		rcDraw.left += m_pDUIMenuXmlInfo->m_StepOffset;
		int nTop = rcItem.top + (rcItem.bottom - rcItem.top)/2;
		rcDraw.top  = nTop;
		rcDraw.bottom  = rcDraw.top+m_pDUIMenuXmlInfo->m_StepHei;
		if (m_pDUIMenuXmlInfo->m_pSepSkin)
		{
			m_pDUIMenuXmlInfo->m_pSepSkin->Draw(pCanvas,rcDraw,0);
		}
		else
		{
			pCanvas->FillSolidRect(rcDraw,m_pDUIMenuXmlInfo->m_crStep);
		}
	}

	void DUIMenuItem::DrawOwnerItem(IDMCanvas* pCanvas, CRect& rcItem, MENUITEMINFO &mii)
	{
		do 
		{
			DMMenuItemData *pdmmi = (DMMenuItemData*)mii.dwItemData;
			if (!pdmmi)
			{
				break;
			}

			bool bDisabled = false;
			bool bSelected = false;
			bool bChecked  = false;
			bool bRadio    = false;
			if (mii.fState&MFS_GRAYED)
			{
				bDisabled = true;
			}
			else if(mii.fState&MFS_HILITE)
			{
				bSelected = true;
			}
			if (mii.fState&MFS_CHECKED)
			{
				bChecked = true;
			}
			if (mii.fType&MFT_RADIOCHECK)// 这是type
			{
				bRadio = true;
			}

			// 绘制选中ITEM项的背景框
			if (bSelected)
			{
				if (m_pDUIMenuXmlInfo->m_pItemSelBgSkin)
				{
					m_pDUIMenuXmlInfo->m_pItemSelBgSkin->Draw(pCanvas, rcItem,0);
				}
				else if (!m_pDUIMenuXmlInfo->m_crSelBg.IsTextInvalid())
				{
					pCanvas->FillSolidRect(rcItem,m_pDUIMenuXmlInfo->m_crSelBg);
				}
			}

			// 绘制ITEM项的ICON
			CRect rcIcon;
			rcIcon.left		= rcItem.left+m_pDUIMenuXmlInfo->m_IconOffset;
			rcIcon.right	= rcIcon.left+m_pDUIMenuXmlInfo->m_szIcon.cx;
			rcIcon.top		= rcItem.top+(rcItem.Height()-m_pDUIMenuXmlInfo->m_szIcon.cy)/2;
			rcIcon.bottom   = rcIcon.top+m_pDUIMenuXmlInfo->m_szIcon.cy;
			if (bChecked)
			{
				if (m_pDUIMenuXmlInfo->m_pCheckSkin)
				{
					m_pDUIMenuXmlInfo->m_pCheckSkin->Draw(pCanvas,rcIcon,bRadio?1:0);
				}
			}
			else if (pdmmi->itemInfo.iIcon!=-1 && m_pDUIMenuXmlInfo->m_pIconSkin)
			{
				m_pDUIMenuXmlInfo->m_pIconSkin->Draw(pCanvas,rcIcon,pdmmi->itemInfo.iIcon);
			}
			rcItem.left = rcIcon.right;// 转换成文字区域
			// 绘制Skin
			if (pdmmi->itemInfo.pSkin)
			{
				CRect rcSkin = rcItem;
				int iState = 0;
				if (bDisabled)
				{
					iState = 2;
				}
				else if (bSelected)
				{
					iState = 1;
				}
				
				pdmmi->itemInfo.pSkin->Draw(pCanvas, rcSkin, iState);
			}

			// 绘制文字
			if (pdmmi->itemInfo.strText.IsEmpty())
			{
				break;// 不绘制
			}
			CRect rcText = rcItem;
			rcText.left = rcText.left+m_pDUIMenuXmlInfo->m_TextOffset;
			DMColor crOld = 0xFFFFFFFF;
			if (bDisabled)
			{
				crOld = pCanvas->SetTextColor(m_pDUIMenuXmlInfo->m_crTextGray);
			}
			else if (bSelected)
			{
				crOld = pCanvas->SetTextColor(m_pDUIMenuXmlInfo->m_crTextSel);
			}
			else
			{
				crOld = pCanvas->SetTextColor(m_pDUIMenuXmlInfo->m_crTextNormal);
			}
			DMSmartPtrT<IDMFont> pOldFont;
			pCanvas->SelectObject(m_pDUIMenuXmlInfo->m_hFont,(IDMMetaFile**)&pOldFont);
			pCanvas->DrawText(pdmmi->itemInfo.strText,pdmmi->itemInfo.strText.GetLength(),&rcText,DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			pCanvas->SelectObject((IDMMetaFile*)pOldFont);
			pCanvas->SetTextColor(crOld);

			if(NULL != mii.hSubMenu)
			{
				DrawIconSub(pCanvas, rcItem, mii);
			}

		} while (false);
	}

	void DUIMenuItem::DrawIconSub(IDMCanvas* pCanvas, RECT& rcItem, MENUITEMINFO& mii)
	{
		// todo.
	}

	LRESULT DUIMenuItem::On_WM_NCDESTROY(WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}

			if (m_bAlpha)
			{
				BOOL bSet = TRUE;
				::SystemParametersInfo(0x1003, 0, &bSet,0);
			}
			lRet = ::CallWindowProc(m_pOldProc,m_hWnd,WM_NCDESTROY, wParam, lParam);	
			OnRelease();
		} while (false);
		return lRet;
	}

	LRESULT DUIMenuItem::On_WM_NCPAINT(WPARAM wParam,LPARAM lParam)
	{
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}
			Render();
		} while (false);
		return 0;
	}

	LRESULT DUIMenuItem::On_WM_CREATE(WPARAM wParam, LPARAM lParam)
	{
		if (0xff!=m_pDUIMenuXmlInfo->m_byAlpha)
		{

			BOOL bEnabled = FALSE;

			//SPI_GETMENUANIMATION
			SystemParametersInfo(0x1002  , 0, &bEnabled, 0);

			if (bEnabled)
			{
				//SPI_SETMENUANIMATION
				SystemParametersInfo(0x1003, 0, 0,0);
				m_bAlpha= TRUE;
			}
			else
			{
				m_bAlpha = false;
			}

			::SetWindowLong(m_hWnd, GWL_EXSTYLE, ::GetWindowLong(m_hWnd, GWL_EXSTYLE) | 0x80000);
			::SetLayeredWindowAttributes(m_hWnd, 0, m_pDUIMenuXmlInfo->m_byAlpha, LWA_ALPHA);
		}
		else
		{
			m_bAlpha = false;
		}	
		return ::CallWindowProc(m_pOldProc,m_hWnd,WM_CREATE,wParam,lParam);
	}

	LRESULT DUIMenuItem::On_WM_KEYFIRST(WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}
			lRet = ::CallWindowProc(m_pOldProc, m_hWnd, WM_KEYFIRST, wParam, lParam);
			if (!IsMenuWindowExist())// CallWindowProc在win10下可能内部发送WM_NCDESTROY销毁窗口,而Render是虚函数,从而可能引发异常,必须在此再次判断
			{
				break;
			}
			Render();

		} while (false);
		return lRet;
	}

	LRESULT DUIMenuItem::On_MN_SELECTITEM(WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}

			if(wParam > 0x0000FFFF)
			{
				wParam = 0xFFFFFFFF;
			}

			lRet = ::CallWindowProc(m_pOldProc, m_hWnd, MN_SELECTITEM, wParam, lParam);

			if (!IsMenuWindowExist())
			{
				break;
			}
			if(wParam != m_dwCurSel)
			{
				Render();
			}
			m_dwCurSel = wParam;
		} while (false);
		return lRet;
	}

	LRESULT DUIMenuItem::On_WM_PAINT(WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}

			PAINTSTRUCT ps;
			::BeginPaint(m_hWnd,&ps);		
			::EndPaint(m_hWnd,&ps);
			Render();
		} while (false);
		return lRet;
	}

	LRESULT DUIMenuItem::On_WM_PRINT(WPARAM wParam, LPARAM lParam)
	{
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}

			::CallWindowProc(m_pOldProc, m_hWnd, WM_PRINT, wParam,lParam);

			if (!IsMenuWindowExist())
			{
				break;
			}
			Render((HDC)wParam);
		} while (false);
		return 0;
	}

	LRESULT DUIMenuItem::On_MN_SIZEWINDOW(WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}
			m_hMenu = (HMENU) ::SendMessage(m_hWnd, MN_GETHMENU, 0, 0);
			lRet = ::CallWindowProc(m_pOldProc,m_hWnd,MN_SIZEWINDOW,wParam,lParam);
		} while (false);
		return lRet;
	}

	LRESULT DUIMenuItem::On_WM_SIZE(WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			if (!IsMenuWindowExist())
			{
				break;
			}
			lRet = ::CallWindowProc(m_pOldProc, m_hWnd, WM_SIZE, wParam, lParam);
			if (0 == m_pDUIMenuXmlInfo->m_szRound.cx && 0 == m_pDUIMenuXmlInfo->m_szRound.cy)
			{
				break;
			}
			RECT rcWnd;
			::GetWindowRect(m_hWnd, &rcWnd);
			::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);

			HRGN hWndRgn = ::CreateRoundRectRgn(0, 0, rcWnd.right-rcWnd.left, rcWnd.bottom-rcWnd.top,m_pDUIMenuXmlInfo->m_szRound.cx,m_pDUIMenuXmlInfo->m_szRound.cy);
			::SetWindowRgn(m_hWnd, hWndRgn, TRUE);
		} while (false);
		return lRet;
	}


	void DUIMenuItem::AddHook(HWND hWnd, WNDPROC WndProc)
	{
		DWORD dwClsStyle = ::GetClassLong(hWnd, GCL_STYLE);
		if (false == m_pDUIMenuXmlInfo->m_bShadow)
		{
			dwClsStyle &= ~CS_DROPSHADOW;
		}
		else
		{
			dwClsStyle |= CS_DROPSHADOW;
		}
		
		SetClassLong(hWnd, GCL_STYLE,dwClsStyle);
		m_hWnd = hWnd;
		::SetProp(m_hWnd, DMMENU_PROP_OBJ, (HANDLE)this);
		m_pNowProc = WndProc;
		m_pOldProc = (WNDPROC)::SetWindowLong(m_hWnd, GWL_WNDPROC, (DWORD)WndProc);
	}

	LRESULT CALLBACK DUIMenuItem::SubMenuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DUIMenuItem* pWnd = (DUIMenuItem *)::GetProp(hWnd,DMMENU_PROP_OBJ);
		if(NULL == pWnd
			||NULL == pWnd->m_hWnd
			||false == ::IsWindow(pWnd->m_hWnd))
		{
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		return pWnd->MenuProc(hWnd, uMsg, wParam, lParam);
	}


}//namespace DM