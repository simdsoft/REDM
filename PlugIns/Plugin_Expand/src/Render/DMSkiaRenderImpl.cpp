#include "Plugin_ExpandAfx.h"

#include "DMSkiaRenderImpl.h"
#include "DMSkiaPenImpl.h"
#include "DMSkiaFontImpl.h"
#include "DMSkiaBrushImpl.h"
#include "DMSkiaBitmapImpl.h"
#include "DMSkiaRegionImpl.h"
#include "DMSkiaCanvasImpl.h"

namespace DM
{
	DMCode DMSkiaRenderImpl::CreateBitmap(IDMBitmap** ppBitmap)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppBitmap)
			{
				break;
			}
			*ppBitmap = new DMSkiaBitmapImpl();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaRenderImpl::CreateFont(const LPLOGFONTW plf,IDMFont** ppFont)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppFont)
			{
				break;
			}

			*ppFont = new DMSkiaFontImpl(plf);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaRenderImpl::CreateRegion(IDMRegion** ppRgn)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppRgn)
			{
				break;
			}

			*ppRgn = new DMSkiaRegionImpl();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaRenderImpl::CreatePen(DMColor clr,int iStyle,int iWidth,IDMPen** ppPen)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppPen)
			{
				break;
			}

			*ppPen = new DMSkiaPenImpl(clr,iStyle,iWidth);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaRenderImpl::CreateSolidColorBrush(DMColor clr,IDMBrush** ppBrush)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppBrush)
			{
				break;
			}

			*ppBrush = new DMSkiaBrushImpl(clr);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaRenderImpl::CreateBitmapBrush(IDMBitmap*pBmp,IDMBrush** ppBrush)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppBrush||NULL == pBmp)
			{
				break;
			}

			DMSkiaBitmapImpl *pSkiaBmp = (DMSkiaBitmapImpl*)pBmp;
			*ppBrush = new DMSkiaBrushImpl(pSkiaBmp->GetSkBitmap());
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaRenderImpl::CreateCanvas(int nWid,int nHei,IDMCanvas** ppCanvas)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppCanvas)
			{
				break;
			}

			*ppCanvas = new DMSkiaCanvasImpl(this, nWid, nHei);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}
}//namespace DM