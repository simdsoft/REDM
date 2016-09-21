#include "DmMainAfx.h"
#include "DMImg9SkinImpl.h"

namespace DM
{
	DMCode DMImg9SkinImpl::Draw(IDMCanvas *pCanvas, LPCRECT lpRectDraw, int iState,BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMASSERT_EXPR(0 != m_nStates,L"DMImgListSkinImpl m_nStates is zero"); // 状态图状态数肯定不能为0！
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

			iErr = pCanvas->DrawBitmapNine(m_pBitmap,&rcSrc,m_rcMargin, lpRectDraw,alpha, m_bTile?DEM_TILE:DEM_STRETCH);
		} while (false);
		return iErr;
	}

	DMCode DMImg9SkinImpl::SendExpandInfo(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (0 == wp)
			{
				break;
			}
			CRect *pRect = (CRect *)wp;
			*pRect = m_rcMargin;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

}//namespace DM
