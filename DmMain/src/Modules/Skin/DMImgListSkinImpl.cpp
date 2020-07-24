#include "DmMainAfx.h"
#include "DMImgListSkinImpl.h"

namespace DM
{
	DMImgListSkinImpl::DMImgListSkinImpl()
	{
		m_nStates	= 1;
		m_bTile		= false;
		m_bVert		= false;
	}

	DMCode DMImgListSkinImpl::GetID(char* lpszId, int iSize)
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
				DMFAIL_MSG("DMImgListSkinImpl id is empty!");
				break;
			}

			if (iSize<m_strID.GetLength())
			{
				iErr = DM_ECODE_SMALLBUF;
				break;
			}
			memset(lpszId, 0, iSize);
			sprintf_s(lpszId, iSize, "%s", (LPCSTR)m_strID);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMImgListSkinImpl::Draw(IDMCanvas *pCanvas, LPCRECT lpRectDraw, int iState,BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMASSERT_MSG(0 != m_nStates,"DMImgListSkinImpl m_nStates is zero"); // 状态图状态数肯定不能为0！
			if (NULL == pCanvas
				|| NULL == lpRectDraw
				|| NULL == m_pBitmap
				|| iState>=m_nStates)// 【0,m_nStates-1】
			{
				break;
			}

			SIZE size = {0};
			if (!DMSUCCEEDED(GetStateSize(size,iState)))
			{
				break;
			}

			RECT rcSrc = {0,0,size.cx,size.cy};
			if (m_bVert)
			{
				::OffsetRect(&rcSrc, 0, iState*size.cy);
			}
			else
			{
				::OffsetRect(&rcSrc, iState*size.cx, 0);
			}

			iErr = pCanvas->DrawBitamp(m_pBitmap,&rcSrc,lpRectDraw,alpha, m_bTile?DEM_TILE:DEM_STRETCH);
		} while (false);
		return iErr;
	}

	DMCode DMImgListSkinImpl::GetStateSize(SIZE &sz, int iState/*=0*/)
	{
		DMASSERT_MSG(0 != m_nStates,"DMImgListSkinImpl m_nStates is zero"); // 状态图状态数肯定不能为0！
		
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_nStates<=iState)
			{
				iState = 0;// 如果超了，默认取第一状态图
			}

			if (m_pBitmap.isNull())
			{
				break;
			}
			SIZE size = {0, 0};
			size.cx	  = m_pBitmap->GetWidth();
			size.cy   = m_pBitmap->GetHeight();

			if (m_bVert)
			{
				size.cy /= m_nStates;
			}
			else
			{
				size.cx /= m_nStates;
			}
			sz = size;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMImgListSkinImpl::GetStates(int &iStates)
	{
		iStates = m_nStates;
		return DM_ECODE_OK;
	}

	DMCode DMImgListSkinImpl::GetBitmap(IDMBitmap** ppBitmap)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pBitmap.isNull())
			{
				break;
			}
			if (NULL == ppBitmap)
			{
				break;
			}
			*ppBitmap = m_pBitmap;
			(*ppBitmap)->AddRef();

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMImgListSkinImpl::SetBitmap(LPBYTE pBuf,size_t szLen,LPCSTR pszType)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL==pBuf||0 == szLen)
			{
				break;
			}
			DMSmartPtrT<IDMRender> pRender;
			if (!DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&pRender, DMREG_Render)))
			{
				DMFAIL_MSG("Got default render fail");
				break;
			}
			DMSmartPtrT<IDMBitmap> pBitmap;
			pRender->CreateBitmap(&pBitmap);

			if (!DMSUCCEEDED(pBitmap->LoadFromMemory(pBuf, szLen,pszType)))
			{
				break;
			}

			if (pBitmap)
			{
				m_pBitmap.reset(pBitmap);
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMImgListSkinImpl::UpdateSkin(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			LPCSTR lpszPath = (LPCSTR)wp;
			if (NULL == lpszPath)
			{
				break;
			}
			if (0!=m_strRes.CompareNoCase(lpszPath))
			{
				break;
			}

			iErr = OnAttributeGetImage(lpszPath,true);
		} while (false);
		return iErr;
	}

	DMCode DMImgListSkinImpl::IsValid()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pBitmap.isNull())
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	///----------------------------
	DMCode DMImgListSkinImpl::OnAttributeGetImage(LPCSTR pszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CStringA  strValue = pszValue;
			CStringA strType;
			CStringA strResName;
			int iFind = strValue.ReverseFind((':'));
			if (-1 != iFind)// 可能是PNG:IDR_BTN
			{
				strType = strValue.Left(iFind);
				strResName = strValue.Right(strValue.GetLength()-iFind-1);
			}
			else// 也可能是IDR_BTN
			{	
				strType = "PNG";  // 默认为PNG
				strResName = strValue;
			}

			m_strRes = strType;m_strRes+=L':';m_strRes+= strResName;

			DMSmartPtrT<IDMRender> pRender;
			if (!DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&pRender, DMREG_Render)))
			{
				DMFAIL_MSG("Got default render fail");
				break;
			}

			DMSmartPtrT<IDMRes> pRes;
			if (!DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res)))
			{
				DMFAIL_MSG("Got default Res fail");
				break;
			}

			DMSmartPtrT<IDMBitmap> pBitmap;
			pRender->CreateBitmap(&pBitmap);

			// if (!DMSUCCEEDED(pRes->GetItemSize(strType,strResName,ulSize)))
			// {
			// 	CStringA szInfo = "DMImgListSkinImpl:OnAttributeGetImage获取";
			// 	szInfo += strType;szInfo+=strResName;szInfo += "失败";
			// 	DMASSERT_EXPR(0,szInfo);
			// 	break;
			// }
			// 
			// DMBufT<byte>pBuf;pBuf.Allocate(ulSize);
			// if (!DMSUCCEEDED(pRes->GetItemBuf(strType,strResName, pBuf, ulSize)))
			// {
			// 	break;
			// }
			DMBufT<byte> pBuf;
			unsigned long ulSize = 0;
			if(!DMSUCCEEDED(pRes->GetItemBuf(strType, strResName, pBuf, &ulSize)))
				break;

			if (!DMSUCCEEDED(pBitmap->LoadFromMemory(pBuf, ulSize,strType)))
			{
				break;
			}

			if (pBitmap)
			{
				m_pBitmap.reset(pBitmap);
				iErr = DM_ECODE_OK;
			}

		} while (false);

		return iErr;
	}
}//namespace DM