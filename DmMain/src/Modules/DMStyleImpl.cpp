#include "DmMainAfx.h"
#include "DMStyleImpl.h"

namespace DM
{


	DMStyleImpl::DMStyleImpl()
	{
		m_uAlign		 = Align_Center;
		m_uVAlign		 = VAlign_Middle;
		m_nTextAlign	 = 0;
		m_bDotted        = false;
		m_strCursor      = L"arrow";
		m_hCursor        = ::LoadCursor(NULL,IDC_ARROW);
		m_bBmpCursor	 = false;
		m_byAlpha        = 0xFF;
		
		for (int i=0;i<5;i++)
		{
			m_ftText[i] = NULL;
			m_crText[i].SetTextInvalid();
			m_crBg[i].SetTextInvalid();
			m_crNc[i].SetTextInvalid();
		}

		// 保证normal是有值的.
		m_ftText[0] = g_pDMFontPool->GetFont(L"");
		m_crText[0].SetRGBA(0,0,0,0xff);
	}

	DMStyleImpl::~DMStyleImpl()
	{
		if (m_bBmpCursor &&NULL != m_hCursor)
		{// 图片创建的Icon,一定要删除掉
			::DestroyIcon(m_hCursor);
			m_hCursor = NULL;
		}
	}

	DMCode DMStyleImpl::CopyData(IDMStyle* pStyle)// 仅复制默认值部分，因为pStyle可以自身先定制
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pStyle)
			{
				break;
			}

			DMStyleImpl*pCopy = (DMStyleImpl*)pStyle;
			for (int i=0;i<5;i++)
			{
				if (pCopy->m_crBg[i].IsTextInvalid())
				{
					pCopy->m_crBg[i] = m_crBg[i];
				}
				if (pCopy->m_crNc[i].IsTextInvalid())
				{
					pCopy->m_crNc[i] = m_crNc[i];
				}
			}
			if (pCopy->m_rcNcMargin.IsRectNull())
			{
				pCopy->m_rcNcMargin = m_rcNcMargin;
			}
			
			for (int i=0;i<5;i++)
			{
				if (0 == i)// 0被赋了默认值
				{
					if (pCopy->m_ftText[0] == g_pDMFontPool->GetFont(L""))
					{
						pCopy->m_ftText[0] = m_ftText[0];
					}

					if (pCopy->m_crText[0].IsDefault())
					{
						pCopy->m_crText[0] = m_crText[0];
					}
				}
				else
				{
					if (pCopy->m_ftText[i].isNull())
					{
						pCopy->m_ftText[i] = m_ftText[i];
					}	
					if (pCopy->m_crText[i].IsTextInvalid())
					{
						pCopy->m_crText[i] = m_crText[i];
					}
				}
			}

			if (pCopy->m_strSkinName.IsEmpty())
			{
				pCopy->m_strSkinName   = m_strSkinName;
			}

			if (pCopy->m_strNcSkinName.IsEmpty())
			{
				pCopy->m_strNcSkinName = m_strNcSkinName;
			}

			if (0 == pCopy->m_strCursor.CompareNoCase(L"arrow"))
			{
				pCopy->m_strCursor	   = m_strCursor;
				pCopy->m_hCursor       = m_hCursor;		///< 由图片创建的icon可以DestroyIcon多次，后面会直接返回无效光标的错误，但不会报错
				pCopy->m_bBmpCursor    = m_bBmpCursor;
			}
			if (0xff == pCopy->m_byAlpha)
			{
				pCopy->m_byAlpha	   = m_byAlpha;
			}

			if (0 == pCopy->m_nTextAlign)
			{
				pCopy->m_nTextAlign	   = m_nTextAlign;
			}

			if (Align_Center == pCopy->m_uAlign)
			{
				pCopy->m_uAlign		   = m_uAlign;
			}

			if (VAlign_Middle == pCopy->m_uVAlign)
			{
				pCopy->m_uVAlign  = m_uVAlign;
			}

			if (false == pCopy->m_bDotted)
			{
				pCopy->m_bDotted = m_bDotted;
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::GetID(wchar_t* lpszId, int iSize)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszId||0 >= iSize)
			{
				break;
			}
			if (m_strID.IsEmpty())
			{
				DMASSERT_EXPR(0,L"DMImgListSkinImpl对象的ID竟然为空！");
				break;
			}

			if (iSize<m_strID.GetLength())
			{
				iErr = DM_ECODE_SMALLBUF;
				break;
			}
			memset(lpszId, 0, iSize);
			swprintf_s(lpszId, iSize, L"%s", m_strID);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des: 文本相关
	//---------------------------------------------------
	DMCode DMStyleImpl::GetTextAlign(UINT &ulAlign)
	{
		do 
		{
			if (m_nTextAlign)
			{
				ulAlign = m_nTextAlign;
				break;
			}

			UINT uRet = 0;
			switch (m_uAlign)
			{
			case Align_Center: uRet |= DT_CENTER;break;
			case Align_Right:  uRet |= DT_RIGHT; break;
			}
			switch (m_uVAlign)
			{
			case VAlign_Middle:uRet |= DT_VCENTER|DT_SINGLELINE;break;
			case VAlign_Bottom:uRet |= DT_BOTTOM|DT_SINGLELINE; break;
			}
			if (m_bDotted) 
			{
				uRet|=DT_END_ELLIPSIS;
			}
			ulAlign = uRet;
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DMStyleImpl::GetTextColor(int iState,DMColor &Clr)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (iState>=countof(m_crText)||m_crText[iState].IsTextInvalid())
			{
				iState = 0;
			}
			Clr = m_crText[iState];
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::GetTextFont(int iState, IDMFont** ppFont) 
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppFont)
			{
				break;
			}

			if (iState>=countof(m_ftText)||!m_ftText[iState])
			{
				iState = 0;
			}
			if (m_ftText[iState])
			{
				*ppFont = m_ftText[iState];
				(*ppFont)->AddRef();
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::GetTextStates(int &iState)
	{
		int f = 1,c = 1;
		for (int i=1;i<5;i++)
		{
			if (m_ftText[i]) f++;
			if (!m_crText[i].IsTextInvalid())c++;
		}
		iState = max(f,c);
		return DM_ECODE_OK;
	}

	//---------------------------------------------------
	// Function Des: 光标相关
	//---------------------------------------------------
	DMCode DMStyleImpl::GetCursor(HCURSOR &hCursor)
	{
		DMCode iErr = DM_ECODE_FAIL;
		hCursor = m_hCursor;
		if (hCursor)
		{
			iErr = DM_ECODE_OK;
		}
		return iErr;
	}

	//---------------------------------------------------
	// Function Des: 窗口相关
	//---------------------------------------------------
	DMCode DMStyleImpl::GetBgSkin(IDMSkin** ppSkin)	
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppSkin||m_strSkinName.IsEmpty())
			{
				break;
			}

			m_strSkinName.Remove(VK_SPACE);
			DMSmartPtrT<IDMSkin> pSkin;
			pSkin = g_pDMApp->GetSkin(m_strSkinName);
			if (NULL == pSkin)
			{
				CStringW szInfo = m_strSkinName;
				szInfo += L"(skin类型)获取失败!";
				DMASSERT_EXPR(0, szInfo);
				break;
			}
	
			*ppSkin = pSkin;
			(*ppSkin)->AddRef();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::GetBgColor(int iState, DMColor &Clr)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (iState>=countof(m_crBg)||m_crBg[iState].IsTextInvalid())
			{
				iState = 0;
			}
			Clr = m_crBg[iState];
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::GetBgClrStates(int &iState)
	{
		int b = 1,c = 1;
		for (int i=1;i<5;i++)
		{
			if (!m_crBg[i].IsTextInvalid()) b++;
			if (!m_crNc[i].IsTextInvalid())c++;
		}
		iState = max(b,c);
		return DM_ECODE_OK;
	}

	DMCode DMStyleImpl::GetNcMargin(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}
			*lpRect = m_rcNcMargin;///hgy413 note: 不能直接写成lpRect = m_rcNcMargin，这样只是取指针，不是赋值
			iErr   = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::GetNcColor(int iState, DMColor &Clr)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (iState>=countof(m_crNc)||m_crNc[iState].IsTextInvalid())
			{
				iState = 0;
			}
			Clr = m_crNc[iState];
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::GetNcSkin(IDMSkin** ppSkin)	
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppSkin||m_strNcSkinName.IsEmpty())
			{
				break;
			}
			m_strNcSkinName.Remove(VK_SPACE);
			DMSmartPtrT<IDMSkin> pSkin;
			pSkin = g_pDMApp->GetSkin(m_strNcSkinName);
			if (NULL == pSkin)
			{
				CStringW szInfo = m_strNcSkinName;
				szInfo += L"(skin类型)获取失败!";
				DMASSERT_EXPR(0, szInfo);
				break;
			}
			*ppSkin = pSkin;
			(*ppSkin)->AddRef();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::GetAlpha(byte &alpha)
	{
		alpha = m_byAlpha;
		return DM_ECODE_OK;
	}

	//
	DMCode DMStyleImpl::OnAttributeSkin(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			m_strSkinName = lpszValue;
			if (false == bLoadXml)
			{
				iErr = DM_ECODE_NOXMLLOADREFRESH;
				break;
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::OnAttributeNcSkin(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			m_strNcSkinName = lpszValue;
			if (false == bLoadXml)
			{
				iErr = DM_ECODE_NOXMLLOADREFRESH;
				break;
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::OnAttributeNcMargin(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			dm_parserect(lpszValue,m_rcNcMargin);
			if (false == bLoadXml)
			{
				iErr = DM_ECODE_NOXMLLOADREFRESH;
				break;
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMStyleImpl::OnAttributeCursor(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CStringW strCursor = lpszValue;
			if (0 == m_strCursor.CompareNoCase(strCursor))
			{
				iErr = DM_ECODE_OK;
				break;
			}
			m_strCursor = strCursor;
			if (false == bLoadXml)
			{
				if (m_bBmpCursor &&NULL != m_hCursor)
				{// 图片创建的Icon,一定要删除掉
					::DestroyIcon(m_hCursor);
					m_hCursor = NULL;
				}
			}
			m_bBmpCursor = false;
			m_strCursor.Remove(VK_SPACE);
			if (0 == m_strCursor.CompareNoCase(L"arrow"))	   {m_hCursor = ::LoadCursor(NULL,IDC_ARROW);   break;}
			if (0 == m_strCursor.CompareNoCase(L"ibeam"))	   {m_hCursor = ::LoadCursor(NULL,IDC_IBEAM);   break;}
			if (0 == m_strCursor.CompareNoCase(L"wait"))	   {m_hCursor = ::LoadCursor(NULL,IDC_WAIT);    break;}
			if (0 == m_strCursor.CompareNoCase(L"cross"))	   {m_hCursor = ::LoadCursor(NULL,IDC_CROSS);   break;}
			if (0 == m_strCursor.CompareNoCase(L"uparrow"))	   {m_hCursor = ::LoadCursor(NULL,IDC_UPARROW); break;}
			if (0 == m_strCursor.CompareNoCase(L"size"))	   {m_hCursor = ::LoadCursor(NULL,IDC_SIZE);    break;}
			if (0 == m_strCursor.CompareNoCase(L"sizenwse"))   {m_hCursor = ::LoadCursor(NULL,IDC_SIZENWSE);break;}
			if (0 == m_strCursor.CompareNoCase(L"sizenesw"))   {m_hCursor = ::LoadCursor(NULL,IDC_SIZENESW);break;}
			if (0 == m_strCursor.CompareNoCase(L"sizewe"))     {m_hCursor = ::LoadCursor(NULL,IDC_SIZEWE);  break;}
			if (0 == m_strCursor.CompareNoCase(L"sizens"))     {m_hCursor = ::LoadCursor(NULL,IDC_SIZENS);  break;}
			if (0 == m_strCursor.CompareNoCase(L"sizeall"))    {m_hCursor = ::LoadCursor(NULL,IDC_SIZEALL); break;}
			if (0 == m_strCursor.CompareNoCase(L"no"))	       {m_hCursor = ::LoadCursor(NULL,IDC_NO);      break;}
			if (0 == m_strCursor.CompareNoCase(L"hand"))	   {m_hCursor = ::LoadCursor(NULL,IDC_HAND);    break;}
			if (0 == m_strCursor.CompareNoCase(L"help"))	   {m_hCursor = ::LoadCursor(NULL,IDC_HELP);    break;}
			// 支持图片生成HCURSOR
			DMSmartPtrT<IDMSkin> pCursorSkin = g_pDMApp->GetSkin(m_strCursor);
			if (pCursorSkin.isValid())
			{
				DMSmartPtrT<IDMBitmap> pBitmap;
				pCursorSkin->GetBitmap(&pBitmap);
				if (pBitmap.isValid())
				{
					HBITMAP hBitmap = pBitmap->GetBitmap();
					CSize sz(pBitmap->GetWidth(),pBitmap->GetHeight());
					m_hCursor = CreateCursorFromBitmap(hBitmap,true,0,sz.cx/2,sz.cy/2);
					if (m_hCursor)
					{
						m_bBmpCursor = true;
					}
					break;
				}
			}
			m_hCursor = NULL;
		} while (false);
		if (m_hCursor)
		{
			iErr = DM_ECODE_OK;
		}
		return iErr;
	}

}//namespace DM