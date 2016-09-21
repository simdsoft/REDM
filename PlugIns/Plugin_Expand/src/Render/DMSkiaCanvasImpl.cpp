#include "Plugin_ExpandAfx.h"
#include "DMSkiaCanvasImpl.h"

namespace DM
{
	DMSkiaCanvasImpl::DMSkiaCanvasImpl(IDMRender* pRender, int nWid, int nHei)
	{
		Canv_Init(pRender, nWid, nHei);
	}

	DMSkiaCanvasImpl::~DMSkiaCanvasImpl()
	{
		Canv_Release();
	}

	void DMSkiaCanvasImpl::Canv_Init(IDMRender* pRender, int nWid, int nHei)
	{
		m_pSkCanvas  = new SkCanvas();
		m_pRender	 = pRender;
		m_hGetDC	 = NULL;
		m_iSaveState = - 1;
		m_ptOrg.fX = m_ptOrg.fY = 0.0f;

		// 初始化被选入DC的当前图元 -----------------
		DMSmartPtrT<IDMPen> pPen;
		m_pRender->CreatePen(PBGRA(0, 0, 0,0xFF),PS_SOLID,1, &pPen);
		SelectObject(pPen);

		DMSmartPtrT<IDMBrush> pBrush;
		m_pRender->CreateSolidColorBrush(PBGRA(0, 0, 0,0xFF),&pBrush);
		SelectObject(pBrush);

		// 这部分代码需在XML解析后获取指定默认的font
		DMSmartPtrT<IDMFont> pFont;
		LOGFONTW lf = {0};
		lf.lfQuality = CLEARTYPE_QUALITY;
		lf.lfHeight	=  20;
		wcscpy_s(lf.lfFaceName,L"宋体");
		m_pRender->CreateFont(&lf,&pFont);
		SelectObject(pFont);

		DMSmartPtrT<IDMBitmap> pBmp;
		m_pRender->CreateBitmap(&pBmp);
		pBmp->Init(nWid,nHei);
		SelectObject(pBmp);
	}

	void DMSkiaCanvasImpl::Canv_Release()
	{
		//SkGraphics::Term();// 缓存很重要，skia崩了
		DM_DELETE(m_pSkCanvas);
	}

	///----------------------------------------
	DMCode DMSkiaCanvasImpl::SelectObject(IDMMetaFile* pObj,IDMMetaFile** ppOldObj /*= NULL*/)
	{
		if (NULL == pObj)
		{
			return DM_ECODE_FAIL;
		}

		DMCode iErr = DM_ECODE_OK;
		// 临时变量，如果ppOldObj为空,它在析构时会释放原始的对象
		DMSmartPtrT<IDMMetaFile> pPreObj;
		switch (pObj->GetType())
		{
		case DMF_PEN:
			{
				pPreObj	  = m_pCurPen;
				m_pCurPen = (DMSkiaPenImpl*)pObj;
			}
			break;

		case DMF_BRUSH:
			{
				pPreObj     = m_pCurBrush;
				m_pCurBrush = (DMSkiaBrushImpl*)pObj;
			}
			break;

		case DMF_BITMAP:
			{
				pPreObj      =  m_pCurBitmap;
				m_pCurBitmap = (DMSkiaBitmapImpl*)pObj;
				DM_DELETE(m_pSkCanvas);
				m_pSkCanvas = new SkCanvas(m_pCurBitmap->GetSkBitmap());
			}
			break;

		case DMF_FONT:
			{
				pPreObj    = m_pCurFont;
				m_pCurFont = (DMSkiaFontImpl*)pObj;
			}
			break;

		default:
			{
				iErr = DM_ECODE_FAIL;
			}
			break;
		}

		if (pPreObj&&NULL != ppOldObj)
		{
			pPreObj->AddRef();// 由获取者调用Release来释放
			*ppOldObj = pPreObj;
		}
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::GetObject(IDMMetaFile** ppObj, DMFTYPE DmfType)
	{
		if (NULL == ppObj)
		{
			return DM_ECODE_FAIL;
		}
		DMCode iErr = DM_ECODE_OK;
		switch (DmfType)
		{
		case DMF_PEN:
			{
				*ppObj = m_pCurPen;
			}
			break;

		case DMF_BRUSH:
			{
				*ppObj = m_pCurBrush;
			}
			break;

		case DMF_BITMAP:
			{
				*ppObj = m_pCurBitmap;
			}
			break;

		case DMF_FONT:
			{
				*ppObj = m_pCurFont;
			}
			break;

		default:
			{
				iErr = DM_ECODE_FAIL;
			}
			break;
		}

		if (DMSUCCEEDED(iErr)&&(*ppObj))
		{
			(*ppObj)->AddRef();// 由获取者调用Release来释放
		}
		return iErr;
	}

	///----------------------------------------
	DMCode DMSkiaCanvasImpl::Resize(SIZE size)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pCurBitmap
				||size.cx<0||size.cy<0)
			{
				break;
			}

			if (!DMSUCCEEDED(m_pCurBitmap->Init(size.cx, size.cy)))
			{
				break;
			}
			DM_DELETE(m_pSkCanvas);
			m_pSkCanvas = new SkCanvas(m_pCurBitmap->GetSkBitmap());
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::Resize(int nWid,int nHei)
	{
		CSize sz(nWid,nHei);
		return Resize(sz);
	}

	DMCode DMSkiaCanvasImpl::GetSize(SIZE &size)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pCurBitmap)
			{
				break;
			}
			size.cx = m_pCurBitmap->GetWidth();
			size.cy = m_pCurBitmap->GetHeight();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	///----------------------------------------
	HDC DMSkiaCanvasImpl::GetDC()
	{
		if (m_hGetDC)
		{
			return m_hGetDC;
		}

		HDC hdc = ::GetDC(NULL);
		m_hGetDC = ::CreateCompatibleDC(hdc);
		::ReleaseDC(NULL, hdc);
		HBITMAP hBitmap = m_pCurBitmap->GetBitmap();
		::SelectObject(m_hGetDC, hBitmap);
		if (m_pSkCanvas->isClipEmpty())// 什么都没画
		{
			::IntersectClipRect(m_hGetDC,0,0,0,0); // 创建一个新的空的剪切区域
		}
		else if(m_pSkCanvas->isClipRect())// 只绘了一个矩形区
		{
			SkRect rcClip;
			m_pSkCanvas->getClipBounds(&rcClip);
			RECT rc = {(int)rcClip.left(),(int)rcClip.top(),(int)rcClip.right(),(int)rcClip.bottom()};
			::InflateRect(&rc,-1,-1);// 注意需要向内缩小一个象素
			::IntersectClipRect(m_hGetDC,rc.left,rc.top,rc.right,rc.bottom);
		}
		else// 此部分请参考资料http://hgy413.com/1845.html
		{
			SkRegion rgn = m_pSkCanvas->internal_private_getTotalClip();
			SkRegion::Iterator it(rgn);
			int nCount=0;
			for(;!it.done();it.next())
			{
				nCount++;
			}
			it.rewind();

			int nSize = sizeof(RGNDATAHEADER)+nCount*sizeof(RECT);
			DMBufT<RGNDATA>pRgnData; pRgnData.AllocateBytes(nSize);
			pRgnData->rdh.dwSize			= sizeof(RGNDATAHEADER);
			pRgnData->rdh.iType				= RDH_RECTANGLES;
			pRgnData->rdh.nCount			= nCount;
			pRgnData->rdh.rcBound.right		= m_pCurBitmap->GetWidth();
			pRgnData->rdh.rcBound.bottom	= m_pCurBitmap->GetHeight();

			nCount = 0;
			LPRECT pRc = (LPRECT)pRgnData->Buffer;
			for(;!it.done();it.next())
			{
				SkIRect skRc = it.rect();
				RECT rc = {skRc.fLeft,skRc.fTop,skRc.fRight,skRc.fBottom};
				pRc[nCount++]= rc;
			}

			HRGN hRgn = ::ExtCreateRegion(NULL,nSize,pRgnData);
			::SelectClipRgn(m_hGetDC,hRgn);// 设置裁剪区，区外的不显示出来
			DM_DELETE_OBJECT(hRgn);
		}

		// 坐标变换可参考http://hgy413.com/1847.html
		::SetViewportOrgEx(m_hGetDC,(int)m_ptOrg.fX,(int)m_ptOrg.fY,NULL);

		return m_hGetDC;
	}

	DMCode DMSkiaCanvasImpl::ReleaseDC(HDC hdc)
	{
		if (hdc == m_hGetDC)
		{
			DM_DELETE_OBJECT(m_hGetDC);
		}
		return DM_ECODE_OK;
	}

	DMCode DMSkiaCanvasImpl::SaveCanvas(int *pnState/*=NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			m_iSaveState = m_pSkCanvas->save();
			if (pnState)
			{
				*pnState = m_iSaveState;
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::RestoreCanvas(int nState/*=-1*/)
	{
		DMASSERT(-1 != m_iSaveState);
		if (-1 == nState)
		{
			m_pSkCanvas->restoreToCount(m_iSaveState);
		}
		else
		{
			m_pSkCanvas->restoreToCount(nState);
		}
		return DM_ECODE_OK;
	}

	///----------------------------------------
	DMCode DMSkiaCanvasImpl::AdjustHSL32(int H, int S, int L,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!m_pCurBitmap)
			{
				break;
			}
			iErr = m_pCurBitmap->AdjustHSL32(H,S,L,lpRect);
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::ResetHsl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!m_pCurBitmap)
			{
				break;
			}
			iErr = m_pCurBitmap->ResetHsl();
		} while (false);
		return iErr;
	}

	PVOID DMSkiaCanvasImpl::GetPixelBits(int* pSize)
	{
		PVOID pPixs = NULL;
		do 
		{
			if (!m_pCurBitmap)
			{
				break;
			}
			pPixs = m_pCurBitmap->GetPixelBits();
			if (pSize)
			{
				*pSize = m_pCurBitmap->GetSize();
			}
		} while (false);
		return pPixs;
	}
	

	///----------------------------------------
	DMCode DMSkiaCanvasImpl::BitBlt(IDMCanvas*pCanvasSrc, int xSrc, int ySrc, LPCRECT lpRectDest, DWORD dwRop/* = SRCCOPY*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pCanvasSrc||NULL == lpRectDest)
			{
				break;
			}

			SkPaint skPaint;
			skPaint.setStyle(SkPaint::kFill_Style);
			dwRop = dwRop & 0x7fffffff;
			switch(dwRop)
			{
			case SRCCOPY:
				skPaint.setXfermodeMode(SkXfermode::kSrc_Mode);
				break;
			case DSTINVERT:
				skPaint.setXfermode(new ProcXfermode(ProcXfermode::Rop2_Invert));
				break;
			case SRCINVERT:
				skPaint.setXfermode(new ProcXfermode(ProcXfermode::Rop2_Xor));
				break;
			case SRCAND:
				skPaint.setXfermode(new ProcXfermode(ProcXfermode::Rop2_And));
				break;
			default:
				{
					DMASSERT_EXPR(0,L"BitBlt-dwRop err");
				}
				break;
			}

			DMSkiaCanvasImpl* pCanvasSkia = (DMSkiaCanvasImpl*)pCanvasSrc;
			SkBitmap  bmpSrc = pCanvasSkia->m_pCurBitmap->GetSkBitmap();
			POINT ptSourViewport;
			pCanvasSkia->GetViewportOrg(&ptSourViewport);
			xSrc += ptSourViewport.x;
			ySrc += ptSourViewport.y;

			SkIRect iSrc={xSrc,ySrc,xSrc + lpRectDest->right-lpRectDest->left,ySrc+lpRectDest->bottom-lpRectDest->top};
			SkRect skRc;
			Rect2SkRect(lpRectDest, skRc);
			skRc.offset(m_ptOrg);
			m_pSkCanvas->drawBitmapRect(bmpSrc,&iSrc,skRc,&skPaint);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::AlphaBlend(IDMCanvas*pCanvasSrc, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pCanvasSrc||NULL == lpRectSrc||NULL == lpRectDest)
			{
				break;
			}
			DMSmartPtrT<IDMBitmap> pBmp;
			pCanvasSrc->GetObject((IDMMetaFile**)&pBmp, DMF_BITMAP);
			if (NULL == pBmp)
			{
				break;
			}
			RECT rcSrc = *lpRectSrc;
			POINT ptSrcOrg;
			pCanvasSrc->GetViewportOrg(&ptSrcOrg);
			::OffsetRect(&rcSrc,ptSrcOrg.x,ptSrcOrg.y);
			iErr = DrawBitamp(pBmp,&rcSrc,lpRectDest, alpha, DEM_STRETCH);
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::DrawBitamp(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha/*=0xFF*/, DMEXPEND_MODE ExpandMode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pBitamp)
			{
				break;
			}

			DMSmartPtrT<DMSkiaBitmapImpl> pSkiaBitmap = (DMSkiaBitmapImpl*)pBitamp;
			SkBitmap skBmp = pSkiaBitmap->GetSkBitmap();
			RECT rcSour = {0,0,skBmp.width(),skBmp.height()};
			if (NULL == lpRectSrc) 
			{
				lpRectSrc = &rcSour;
			}

			if (NULL == lpRectDest)
			{
				lpRectDest = &rcSour;
			}

			SkRect skRcSrc;
			SkRect skRcDest;
			Rect2SkRect(lpRectSrc, skRcSrc);
			Rect2SkRect(lpRectDest, skRcDest);
			skRcDest.offset(m_ptOrg);

			SkPaint skPaint;
			skPaint.setAntiAlias(true);//反锯齿
			if (0xFF != alpha) 
			{
				skPaint.setAlpha(alpha);
			}

			if (DEM_STRETCH == ExpandMode)
			{
				m_pSkCanvas->drawBitmapRectToRect(skBmp, &skRcSrc, skRcDest,&skPaint);
			}
			else /*(DEM_TILE == ExpandMode)*/ // 平铺
			{
#if 0
				SkBitmap skBmpsub;
				SkIRect skiSubset;
				skiSubset.setLTRB(lpRectSrc->left, lpRectSrc->top, lpRectSrc->right, lpRectSrc->bottom);
				if (false == skBmp.extractSubset(&skBmpsub, skiSubset))
				{
					break;
				}
				skPaint.setShader(SkShader::CreateBitmapShader(skBmpsub,SkShader::kRepeat_TileMode,SkShader::kRepeat_TileMode))->unref();
				m_pSkCanvas->drawRect(skRcDest,skPaint);
#endif 
				PushClip(lpRectDest,RGN_AND);
				SkIRect skIRcSrc = {lpRectSrc->left,lpRectSrc->top,lpRectSrc->right,lpRectSrc->bottom};
				SkRect skSubDest;
				skSubDest.setLTRB(0.0f,0.0f,(float)skIRcSrc.width(),(float)skIRcSrc.height());
				for (float y = skRcDest.fTop;y<skRcDest.fBottom;y += skIRcSrc.height())
				{
					skSubDest.offsetTo(skRcDest.fLeft,y);               
					for(float x=skRcDest.fLeft;x<skRcDest.fRight;x += skIRcSrc.width())
					{
						m_pSkCanvas->drawBitmapRect(skBmp,&skIRcSrc,skSubDest,&skPaint);
						skSubDest.offset((float)skIRcSrc.width(),0.0f);
					}
				}

				PopClip();
			}
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::DrawBitmapNine(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpSrcMargin, LPCRECT lpRectDest, BYTE alpha/*=0xFF*/, DMEXPEND_MODE ExpandMode/*=DEM_STRETCH*/)
	{
		// 可以通过SkNinePatch::DrawNine实现简单的9宫格
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pBitamp)
			{
				break;
			}

			// 9宫格，xDest、xSrc对应以左上角为原点，9宫格划分的4个横坐标，yDest、ySrc以左上角为原点，9宫格划分的4个纵坐标
			int xDest[4] = {lpRectDest->left,lpRectDest->left+lpSrcMargin->left,lpRectDest->right-lpSrcMargin->right,lpRectDest->right};
			int yDest[4] = {lpRectDest->top,lpRectDest->top+lpSrcMargin->top,lpRectDest->bottom-lpSrcMargin->bottom,lpRectDest->bottom};
			int xSrc[4]  = {lpRectSrc->left,lpRectSrc->left+lpSrcMargin->left,lpRectSrc->right-lpSrcMargin->right,lpRectSrc->right};
			int ySrc[4]  = {lpRectSrc->top,lpRectSrc->top+lpSrcMargin->top,lpRectSrc->bottom-lpSrcMargin->bottom,lpRectSrc->bottom};
			if (!(xSrc[0] <= xSrc[1] && xSrc[1] <= xSrc[2] && xSrc[2] <= xSrc[3])) 
			{
				break;
			}
			if (!(ySrc[0] <= ySrc[1] && ySrc[1] <= ySrc[2] && ySrc[2] <= ySrc[3]))
			{
				break;
			}

			//调整目标位置
			int iDestWidth = lpRectDest->right-lpRectDest->left;
			int iDestHeight = lpRectDest->bottom-lpRectDest->top;

			if((lpSrcMargin->left+lpSrcMargin->right) > iDestWidth)// 边缘宽度大于目标宽度的处理
			{
				if (lpSrcMargin->left >= iDestWidth)// 只绘制左边部分
				{
					xSrc[1] = xSrc[2] = xSrc[3] = xSrc[0]+iDestWidth;
					xDest[1] = xDest[2] = xDest[3] = xDest[0]+iDestWidth;
				}
				else if(lpSrcMargin->right >= iDestWidth)// 只绘制右边部分
				{
					xSrc[0] = xSrc[1] = xSrc[2] = xSrc[3]-iDestWidth;
					xDest[0] = xDest[1] = xDest[2] = xDest[3]-iDestWidth;
				}
				else//先绘制左边部分，剩余的用右边填充
				{
					int nRemain=xDest[3]-xDest[1];
					xSrc[2] = xSrc[3]-nRemain;
					xDest[2] = xDest[3]-nRemain;
				}
			}

			if (lpSrcMargin->top + lpSrcMargin->bottom > iDestHeight)
			{
				if (lpSrcMargin->top >= iDestHeight)// 只绘制上边部分
				{
					ySrc[1] = ySrc[2] = ySrc[3] = ySrc[0]+iDestHeight;
					yDest[1] = yDest[2] = yDest[3] = yDest[0]+iDestHeight;
				}
				else if(lpSrcMargin->bottom >= iDestHeight)// 只绘制下边部分
				{
					ySrc[0] = ySrc[1] = ySrc[2] = ySrc[3]-iDestHeight;
					yDest[0] = yDest[1] = yDest[2] = yDest[3]-iDestHeight;
				}
				else// 先绘制左边部分，剩余的用右边填充
				{
					int nRemain=yDest[3]-yDest[1];
					ySrc[2] = ySrc[3]-nRemain;
					yDest[2] = yDest[3]-nRemain;
				}
			}

			//定义绘制模式
			DMEXPEND_MODE mode[3][3]={//四个边角从上面可以看出源和目标的边角矩形大小总是一致
				{DEM_STRETCH,ExpandMode,DEM_STRETCH},
				{ExpandMode,ExpandMode,ExpandMode},
				{DEM_STRETCH,ExpandMode,DEM_STRETCH}
			};

			for (int y=0;y<3;y++)
			{
				if (ySrc[y] == ySrc[y+1]) // 空矩形
				{
					continue;
				}
				for (int x=0;x<3;x++)
				{
					if (xSrc[x] == xSrc[x+1]) // 空矩形
					{
						continue;
					}

					RECT rcSrc = {xSrc[x],ySrc[y],xSrc[x+1],ySrc[y+1]};
					RECT rcDest ={xDest[x],yDest[y],xDest[x+1],yDest[y+1]};
					iErr = DrawBitamp(pBitamp, &rcSrc, &rcDest, alpha, mode[y][x]);
				}
			}
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::DrawRectangle(LPCRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			SkPaint skPaint;
			SkColor skClr = m_pCurPen->GetColor().ToBGRA();
			skPaint.setColor(skClr);
			DMGetLineDashEffect skDash(m_pCurPen->GetStyle());
			skPaint.setPathEffect(skDash.Get());
			skPaint.setStrokeWidth((SkScalar)m_pCurPen->GetWidth()-0.5f);
			skPaint.setStyle(SkPaint::kStroke_Style);
			skPaint.setAntiAlias(true);

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);
			InflateSkRect(&skRc,-0.5f,-0.5f);
			m_pSkCanvas->drawRect(skRc,skPaint);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::FillRectangle(LPCRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			SkPaint skPaint;
			if (m_pCurBrush->IsBitmap())
			{
				skPaint.setFilterBitmap(true);
				skPaint.setShader(SkShader::CreateBitmapShader(m_pCurBrush->GetBitmap(),SkShader::kRepeat_TileMode,SkShader::kRepeat_TileMode))->unref();
			}
			else
			{
				skPaint.setFilterBitmap(false);
				SkColor skClr = m_pCurBrush->GetColor().ToBGRA();
				skPaint.setColor(skClr);
			}
			skPaint.setStyle(SkPaint::kFill_Style);
			//skPaint.setAntiAlias(true);

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);
			InflateSkRect(&skRc,-0.5f,-0.5f);
			m_pSkCanvas->drawRect(skRc,skPaint);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::FillSolidRect(LPCRECT lpRect,DMColor clr)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcDest = lpRect;
			if (NULL == lpRect||rcDest.IsRectEmpty())
			{
				break;
			}

			SkPaint skPaint;
			skPaint.setStyle(SkPaint::kFill_Style);
			skPaint.setColor(clr.ToBGRA());
			skPaint.setXfermodeMode(SkXfermode::kSrcOver_Mode);
			//skPaint.setAntiAlias(true);

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);
			m_pSkCanvas->drawRect(skRc,skPaint);   
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::ClearRect(LPCRECT lpRect,DMColor clr)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}
			SkPaint skPaint;
			skPaint.setStyle(SkPaint::kFill_Style);
			skPaint.setColor(clr.ToBGRA());
			skPaint.setXfermodeMode(SkXfermode::kSrc_Mode);

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);
			m_pSkCanvas->drawRect(skRc,skPaint);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::DrawRoundRect(LPCRECT lpRect,POINT &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			SkPaint skPaint;
			SkColor skClr = m_pCurPen->GetColor().ToBGRA();
			skPaint.setColor(skClr);

			DMGetLineDashEffect skDash(m_pCurPen->GetStyle());
			skPaint.setPathEffect(skDash.Get());
			skPaint.setStrokeWidth((SkScalar)m_pCurPen->GetWidth()-0.5f);
			skPaint.setStyle(SkPaint::kStroke_Style);
			skPaint.setAntiAlias(true);

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);
			InflateSkRect(&skRc,-0.5f,-0.5f);//要缩小0.5显示效果才和GDI一致。
			m_pSkCanvas->drawRoundRect(skRc,(SkScalar)pt.x,(SkScalar)pt.y,skPaint);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::FillRoundRect(LPCRECT lpRect,POINT &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			SkPaint skPaint;
			skPaint.setAntiAlias(true);

			if (m_pCurBrush->IsBitmap())
			{
				skPaint.setFilterBitmap(true);
				skPaint.setShader(SkShader::CreateBitmapShader(m_pCurBrush->GetBitmap(),SkShader::kRepeat_TileMode,SkShader::kRepeat_TileMode))->unref();
			}
			else
			{
				skPaint.setFilterBitmap(false);
				SkColor skClr = m_pCurBrush->GetColor().ToBGRA();
				skPaint.setColor(skClr);
			}
			skPaint.setStyle(SkPaint::kFill_Style);

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			InflateSkRect(&skRc,-0.5f,-0.5f);//要缩小0.5显示效果才和GDI一致。
			skRc.offset(m_ptOrg);

			m_pSkCanvas->drawRoundRect(skRc,(SkScalar)pt.x,(SkScalar)pt.y,skPaint);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::DrawEllipse(LPCRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			SkPaint skPaint;
			SkColor skClr = m_pCurPen->GetColor().ToBGRA();
			skPaint.setColor(skClr);

			DMGetLineDashEffect skDash(m_pCurPen->GetStyle());
			skPaint.setPathEffect(skDash.Get());
			skPaint.setStrokeWidth((SkScalar)m_pCurPen->GetWidth()-0.5f);
			skPaint.setStyle(SkPaint::kStroke_Style);
			//skPaint.setAntiAlias(true);

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);
			m_pSkCanvas->drawOval(skRc,skPaint);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::FillEllipse(LPCRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			SkPaint skPaint;
			if (m_pCurBrush->IsBitmap())
			{
				skPaint.setFilterBitmap(true);
				skPaint.setShader(SkShader::CreateBitmapShader(m_pCurBrush->GetBitmap(),SkShader::kRepeat_TileMode,SkShader::kRepeat_TileMode))->unref();
			}
			else
			{
				skPaint.setFilterBitmap(false);
				SkColor skClr = m_pCurBrush->GetColor().ToBGRA();
				skPaint.setColor(skClr);
			}
			skPaint.setStyle(SkPaint::kFill_Style);

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);;
			skRc.offset(m_ptOrg);
			m_pSkCanvas->drawOval(skRc,skPaint);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::Polyline(LPPOINT lpPt,int cPoints)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMBufT<SkPoint> pts; pts.Allocate(cPoints);
			for (int i=0; i<cPoints; i++)
			{
				pts[i].fX = (SkScalar)lpPt[i].x;
				pts[i].fY = (SkScalar)lpPt[i].y;
			}
			SkPoint::Offset(pts,cPoints,m_ptOrg);

			SkPaint skPaint;
			skPaint.setAntiAlias(true);
			SkColor skClr = m_pCurPen->GetColor().ToBGRA();
			skPaint.setColor(skClr);
			DMGetLineDashEffect skDash(m_pCurPen->GetStyle());
			skPaint.setPathEffect(skDash.Get());
			skPaint.setStrokeWidth((SkScalar)m_pCurPen->GetWidth()-0.5f);
			skPaint.setStyle(SkPaint::kStroke_Style);
			m_pSkCanvas->drawPoints(SkCanvas::kPolygon_PointMode,cPoints,pts,skPaint);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;

	}

	DMCode DMSkiaCanvasImpl::GradientFill(DMColor clrBegin,DMColor clrEnd,LPCRECT lpRect,BOOL bVert,BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);

			SkPoint pts[2];
			pts[0].set(skRc.left(),skRc.top());

			if (bVert)
			{
				pts[1].set(skRc.left(),skRc.bottom());
			}
			else
			{
				pts[1].set(skRc.right(),skRc.top());
			}

			DMColor clr1 = clrBegin;
			clr1.PreMultiPly(clrBegin,alpha);
			DMColor clr2 = clrEnd;
			clr2.PreMultiPly(clrEnd,alpha);

			const SkColor colors[2] = {clr1.ToBGRA(),clr2.ToBGRA()};
			SkShader *pShader = SkGradientShader::CreateLinear(pts, colors, NULL,2,SkShader::kMirror_TileMode);
			SkPaint skPaint;
			skPaint.setShader(pShader);
			pShader->unref();

			m_pSkCanvas->drawRect(skRc,skPaint);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	///----------------------------------
	DMColor DMSkiaCanvasImpl::SetTextColor(DMColor clr)
	{
		DMColor ClrOld = m_CurTextColor;
		m_CurTextColor = clr;
		return ClrOld;
	}

	DMColor DMSkiaCanvasImpl::GetTextColor()
	{
		return m_CurTextColor;
	}

	DMCode DMSkiaCanvasImpl::MeasureText(LPCWSTR lpString, int nCount, OUT SIZE *pSize)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpString
				||NULL == pSize)
			{
				break;
			}

			if (nCount<0)
			{
				nCount = (int)wcslen(lpString);
			}

			SkPaint txtPaint = m_pCurFont->GetSkPaint();
			txtPaint.setTypeface(m_pCurFont->GetSkTypeFace());
			pSize->cx = (LONG)txtPaint.measureText(lpString, nCount*sizeof(wchar_t));

			SkPaint::FontMetrics metrics;
			txtPaint.getFontMetrics(&metrics);
			pSize->cy = (int)(metrics.fBottom-metrics.fTop);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::DrawText(LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpString||0==wcslen(lpString))
			{
				break;
			}

			if (nCount<0)
			{
				nCount = (int)wcslen(lpString);
			}

			if (0 == nCount) // 要绘制的文字还是空
			{
				if (uFormat & DT_CALCRECT)// 需取得大小，绘制区设置为0
				{
					lpRect->right = lpRect->left;
					lpRect->bottom = lpRect->top;
				}
				iErr = DM_ECODE_OK;
				break;
			}
			
			SkPaint  txtPaint = m_pCurFont->GetSkPaint();
			txtPaint.setColor(m_CurTextColor.ToBGRA());
			txtPaint.setTypeface(m_pCurFont->GetSkTypeFace());
			if (0xff!=alpha)
			{
				txtPaint.setAlpha(alpha);
			}
			if (uFormat&DT_CENTER)
			{
				txtPaint.setTextAlign(SkPaint::kCenter_Align);
			}
			else if(uFormat&DT_RIGHT)
			{
				txtPaint.setTextAlign(SkPaint::kRight_Align);
			}

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);
			skRc = DrawText_Skia(m_pSkCanvas,lpString,nCount,skRc,txtPaint,uFormat);
			if (uFormat & DT_CALCRECT)
			{
				lpRect->left	= (int)skRc.fLeft;
				lpRect->top		= (int)skRc.fTop;
				lpRect->right	= (int)skRc.fRight;
				lpRect->bottom  = (int)skRc.fBottom;
				::OffsetRect(lpRect, (int)-m_ptOrg.fX,(int)-m_ptOrg.fY);// 防止负数出现
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::TextOut(LPCWSTR lpString, int nCount, int x, int  y, BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpString||0==wcslen(lpString))
			{
				break;
			}

			if (nCount<0)
			{
				nCount = (int)wcslen(lpString);
			}

			SkPaint txtPaint = m_pCurFont->GetSkPaint();
			SkPaint::FontMetrics metrics;
			txtPaint.getFontMetrics(&metrics);
			SkScalar fx = m_ptOrg.fX + x;
			SkScalar fy = m_ptOrg.fY + y;
			fy -= metrics.fAscent;

			txtPaint.setColor(m_CurTextColor.ToBGRA());
			txtPaint.setTypeface(m_pCurFont->GetSkTypeFace());
			if (0xff!=alpha)
			{
				txtPaint.setAlpha(alpha);
			}
			m_pSkCanvas->drawText(lpString, nCount*sizeof(wchar_t),fx,fy,txtPaint);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	///----------------------------------
	DMCode DMSkiaCanvasImpl::OffsetViewportOrg(int dx, int dy, OUT LPPOINT lpPoint /*= NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lpPoint)
			{
				lpPoint->x = (LONG) m_ptOrg.fX;
				lpPoint->y = (LONG) m_ptOrg.fY;
			}
			m_ptOrg.offset(SkIntToScalar(dx), SkIntToScalar(dy));
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::SetViewportOrgEx(int dx, int dy,  OUT LPPOINT lpPoint /*= NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lpPoint)
			{
				lpPoint->x = (LONG) m_ptOrg.fX;
				lpPoint->y = (LONG) m_ptOrg.fY;
			}

			m_ptOrg.fX = SkIntToScalar(dx);
			m_ptOrg.fY = SkIntToScalar(dy);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::GetViewportOrg(LPPOINT lpPoint)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lpPoint)
			{
				lpPoint->x = (LONG) m_ptOrg.fX;
				lpPoint->y = (LONG) m_ptOrg.fY;
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	///----------------------------------
	DMCode DMSkiaCanvasImpl::PushClip(IDMRegion* pRegion,int fnCombineMode/*=RGN_AND*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pRegion)
			{
				break;
			}

			DMSkiaRegionImpl* pSkiaRegion = (DMSkiaRegionImpl*)pRegion;

			SkRegion rgn = pSkiaRegion->GetRegion();
			rgn.translate((int)m_ptOrg.fX,(int)m_ptOrg.fY);

			m_pSkCanvas->save();// 在PopClip中做恢复
			m_pSkCanvas->clipRegion(rgn,DMSkiaRegionImpl::GdiRgnOp2SkRgnOp(fnCombineMode));
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::PushClip(LPCRECT lpRect,int fnCombineMode/*=RGN_AND*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			SkRect skRc;
			Rect2SkRect(lpRect, skRc);
			skRc.offset(m_ptOrg);

			m_pSkCanvas->save();// 在PopClip中做恢复
			m_pSkCanvas->clipRect(skRc,DMSkiaRegionImpl::GdiRgnOp2SkRgnOp(fnCombineMode));
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMSkiaCanvasImpl::PopClip()
	{
		m_pSkCanvas->restore();
		return DM_ECODE_OK;
	}

	DMCode DMSkiaCanvasImpl::GetClipBox(LPRECT lpRect)
	{
		SkRect skRc;
		m_pSkCanvas->getClipBounds(&skRc);
		// 需要将rect的viewOrg还原
		skRc.offset(-m_ptOrg);

		lpRect->left	= (LONG)skRc.fLeft;
		lpRect->top		= (LONG)skRc.fTop;
		lpRect->right	= (LONG)skRc.fRight;
		lpRect->bottom	= (LONG)skRc.fBottom;
		// 需要4周缩小一个单位才是和GDI相同的剪裁区
		::InflateRect(lpRect,-1,-1);
		return DM_ECODE_OK;
	}

	DMCode DMSkiaCanvasImpl::GetClipRegion(IDMRegion **ppRegion)
	{
		DMSkiaRegionImpl *pRgn = new DMSkiaRegionImpl();
		SkRegion skRgn = m_pSkCanvas->internal_private_getTotalClip();
		//需要将rect的viewOrg还原
		skRgn.translate((int)-m_ptOrg.fX,(int)-m_ptOrg.fY);
		pRgn->SetRegion(skRgn);
		*ppRegion = pRgn;
		return DM_ECODE_OK;
	}

	///----------------------------------
	DMCode DMSkiaCanvasImpl::ExcludeClipRect(LPCRECT lpRect)
	{
		SkRect skRc;
		Rect2SkRect(lpRect, skRc);
		skRc.offset(m_ptOrg);
		m_pSkCanvas->clipRect(skRc,SkRegion::kDifference_Op);
		return DM_ECODE_OK;
	}

	DMCode DMSkiaCanvasImpl::IntersectClipRect(LPCRECT lpRect)
	{
		SkRect skRc;
		Rect2SkRect(lpRect, skRc);
		skRc.offset(m_ptOrg);
		m_pSkCanvas->clipRect(skRc,SkRegion::kIntersect_Op);
		return DM_ECODE_OK;
	}

	DMCode DMSkiaCanvasImpl::SaveBmpToFile(LPCWSTR pszFileName)
	{
		if (m_pCurBitmap)
		{
			return m_pCurBitmap->SaveFile(pszFileName);
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMSkiaCanvasImpl::AdjustAlpha(byte byAlpha, LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcBackup = lpRect;
			rcBackup.OffsetRect((int)m_ptOrg.fX,(int)m_ptOrg.fY);
			iErr = m_pCurBitmap->AdjustAlpha(byAlpha,rcBackup);
		} while (false);
		return iErr;
	}

	//辅助
	bool DMSkiaCanvasImpl::Rect2SkRect(LPCRECT lpRect,SkRect &skiRc)
	{
		bool bRet = false;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			skiRc.setLTRB((float)lpRect->left,(float)lpRect->top,(float)lpRect->right,(float)lpRect->bottom);
			bRet = true;
		} while (false);

		return bRet;
	}

	void DMSkiaCanvasImpl::InflateSkRect(SkRect *lpRect,SkScalar dx,SkScalar dy)
	{
		lpRect->fLeft	-= dx;
		lpRect->fRight	+= dx;
		lpRect->fTop	-= dy;
		lpRect->fBottom += dy;
	}

}//namespace DM