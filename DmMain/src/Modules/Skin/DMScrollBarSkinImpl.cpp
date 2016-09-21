#include "DmMainAfx.h"
#include "DMScrollBarSkinImpl.h"

namespace DM
{

	DMCode DMScrollBarSkinImpl::Draw(IDMCanvas *pCanvas, LPCRECT lpRectDraw, int iState,BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pBitmap)
			{
				break;
			}
			int nSbCode = LOWORD(iState);
			int nState	= LOBYTE(HIWORD(iState));
			bool bVert	= !!HIBYTE(HIWORD(iState));

			CRect rcMargin(0,0,0,0);
			if (bVert)
			{
				rcMargin.top    = m_rcMargin.top;
				rcMargin.bottom = m_rcMargin.bottom;
			}
			else
			{
				rcMargin.left   = m_rcMargin.left;
				rcMargin.right  = m_rcMargin.right;
			}

			CRect rcSor = GetPartRect(nSbCode,nState,bVert);
			iErr = pCanvas->DrawBitmapNine(m_pBitmap,&rcSor,&rcMargin,lpRectDraw,alpha,m_bTile?DEM_TILE:DEM_STRETCH);
		} while (false);
		return iErr;
	}

	//--------------------------------------------------
	// 控件:上箭头、下箭头、坚直滚动条、竖直滑槽、左箭头、右箭头、水平滚动条、水平滑槽、小边角
	// 状态:正常、停留、点击、禁用、非Active
	// 共列9*行5=45种状态 nSbCode+bVert选择列 nState为行
	//--------------------------------------------------
	CRect DMScrollBarSkinImpl::GetPartRect(int nSbCode, int nState,bool bVert)
	{
		CRect rcDest;
		do 
		{
			if (m_pBitmap.isNull())
			{
				break;
			}
			int nWid = m_pBitmap->GetWidth();
			int nHei = m_pBitmap->GetHeight();

			CSize szItem(nWid/9,nWid/9);//16*16

			int iPart = -1;
			switch (nSbCode)
			{
			case SB_LINEUP:			iPart = 0;break;
			case SB_LINEDOWN:		iPart = 1;break;
			case SB_THUMBTRACK:		iPart = 2;break;
			case SB_PAGEUP:
			case SB_PAGEDOWN:		iPart = 3;break;
			case SB_CORNOR:         iPart = 8;break;
			}
			if (!bVert&&nSbCode!=SB_CORNOR) // 边角只有一份
			{
				iPart += 4;
			}

			if (3 == nHei/szItem.cy) // 仅前三行,也就是没有禁用、非Active状态
			{
				if (DMSBST_DISABLE == nState||DMSBST_NOACTIVE==nState)
				{
					nState = DMSBST_NORMAL;
				}
			}

			if (4 == nHei/szItem.cy) // 仅前四行,也就是没有非Active状态
			{
				if (DMSBST_NOACTIVE == nState)
				{
					nState = DMSBST_NORMAL;
				}
			}

			if (DMSBST_NOACTIVE == nState)
			{
				if (SB_THUMBTRACK == nSbCode)
				{
					nState = DMSBST_NORMAL;
				}
			}

			rcDest = CRect(CPoint(szItem.cx*iPart,szItem.cy*nState),szItem);
		} while (false);
		return rcDest;
	}

}//namespace DM