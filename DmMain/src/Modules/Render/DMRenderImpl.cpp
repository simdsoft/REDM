#include "DmMainAfx.h"
#include "DMRenderImpl.h"
#include "DMPenImpl.h"
#include "DMFontImpl.h"
#include "DMBrushImpl.h"
#include "DMBitmapImpl.h"
#include "DMRegionImpl.h"
#include "DMCanvasImpl.h"

namespace DM
{
	DMCode DMRenderImpl::CreateBitmap(IDMBitmap** ppBitmap)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppBitmap)
			{
				break;
			}
			*ppBitmap = new DMBitmapImpl();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMRenderImpl::CreateFont(const LPLOGFONTW plf,IDMFont** ppFont)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppFont)
			{
				break;
			}

			*ppFont = new DMFontImpl(plf);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMRenderImpl::CreateRegion(IDMRegion** ppRgn)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppRgn)
			{
				break;
			}

			*ppRgn = new DMRegionImpl();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMRenderImpl::CreatePen(DMColor clr,int iStyle,int iWidth,IDMPen** ppPen)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppPen)
			{
				break;
			}

			*ppPen = new DMPenImpl(clr,iStyle,iWidth);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMRenderImpl::CreateSolidColorBrush(DMColor clr,IDMBrush** ppBrush)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppBrush)
			{
				break;
			}

			*ppBrush = new DMBrushImpl(clr);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMRenderImpl::CreateBitmapBrush(IDMBitmap*pBmp,IDMBrush** ppBrush)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppBrush||NULL == pBmp)
			{
				break;
			}

			DMBitmapImpl *pGdiBmp = (DMBitmapImpl*)pBmp;
			*ppBrush = new DMBrushImpl(pGdiBmp->GetBitmap());
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMRenderImpl::CreateCanvas(int nWid,int nHei,IDMCanvas** ppCanvas)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppCanvas)
			{
				break;
			}

			*ppCanvas = new DMCanvasImpl(this, nWid, nHei);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}
}//namespace DM