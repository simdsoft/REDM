#include "DmMainAfx.h"
#include "DMCanvasImpl.h"
#include <math.h>

namespace DM
{
	DMCanvasImpl::DMCanvasImpl(IDMRender* pRender, int nWid, int nHei)
	{
		Canv_Init(pRender, nWid, nHei);
	}

	DMCanvasImpl::~DMCanvasImpl()
	{
		Canv_Release();
	}

	void DMCanvasImpl::Canv_Init(IDMRender* pRender, int nWid, int nHei)
	{
		m_pRender	 = pRender;
		m_ptOrg.x    = m_ptOrg.y = 0;

		::SetBkMode(m_hdc,TRANSPARENT);    // ����͸������

		// ��ʼ����ѡ��DC�ĵ�ǰͼԪ -----------------
		DMSmartPtrT<IDMPen> pPen;
		m_pRender->CreatePen( PBGRA(0, 0, 0,0xFF), PS_SOLID, 1, &pPen);
		SelectObject(pPen);

		DMSmartPtrT<IDMBrush> pBrush;
		m_pRender->CreateSolidColorBrush(PBGRA(0, 0, 0,0xFF),&pBrush);
		SelectObject(pBrush);

		// �ⲿ�ִ�������XML�������ȡָ��Ĭ�ϵ�font
		DMSmartPtrT<IDMFont> pFont;
		LOGFONTW lf = {0};
		lf.lfQuality = CLEARTYPE_QUALITY;
		lf.lfHeight	=  20;
		wcscpy_s(lf.lfFaceName,L"����");
		m_pRender->CreateFont(&lf,&pFont);
		SelectObject(pFont);

		DMSmartPtrT<IDMBitmap> pBmp;
		m_pRender->CreateBitmap(&pBmp);
		pBmp->Init(nWid,nHei);
		SelectObject(pBmp);
	}

	void DMCanvasImpl::Canv_Release()
	{
		m_hdc.DeleteDC();
	}

	///----------------------------------------
	DMCode DMCanvasImpl::SelectObject(IDMMetaFile* pObj,IDMMetaFile** ppOldObj /*= NULL*/)
	{
		if (NULL == pObj)
		{
			return DM_ECODE_FAIL;
		}

		DMCode iErr = DM_ECODE_OK;
		// ��ʱ���������ppOldObjΪ��,��������ʱ���ͷ�ԭʼ�Ķ���
		DMSmartPtrT<IDMMetaFile> pPreObj;
		switch (pObj->GetType())
		{
		case DMF_PEN:
			{
				pPreObj	  = m_pCurPen;
				m_pCurPen = (DMPenImpl*)pObj;
				m_hdc.SelectObject(m_pCurPen->GetPen());
			}
			break;

		case DMF_BRUSH:
			{
				pPreObj     = m_pCurBrush;
				m_pCurBrush = (DMBrushImpl*)pObj;
				m_hdc.SelectObject(m_pCurBrush->GetBrush());
			}
			break;

		case DMF_BITMAP:
			{
				pPreObj      =  m_pCurBitmap;
				m_pCurBitmap = (DMBitmapImpl*)pObj;
				m_hdc.SelectObject(m_pCurBitmap->GetBitmap());
			}
			break;

		case DMF_FONT:
			{
				pPreObj    = m_pCurFont;
				m_pCurFont = (DMFontImpl*)pObj;
				m_hdc.SelectObject(m_pCurFont->GetFont());
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
			pPreObj->AddRef();// �ɻ�ȡ�ߵ���Release���ͷ�
			*ppOldObj = pPreObj;
		}
		return iErr;
	}

	DMCode DMCanvasImpl::GetObject(IDMMetaFile** ppObj, DMFTYPE DmfType)
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
			(*ppObj)->AddRef();// �ɻ�ȡ�ߵ���Release���ͷ�
		}
		return iErr;
	}

	///----------------------------------------
	DMCode DMCanvasImpl::Resize(SIZE size)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pCurBitmap
				||size.cx<0||size.cy<0)
			{
				break;
			}

			HBITMAP hBmp = ::CreateCompatibleBitmap(m_hdc,0,0);
			m_hdc.SelectObject(hBmp);// ��ʱ���󣬷�ֹ��Init�е�HBITMAP������
			if (!DMSUCCEEDED(m_pCurBitmap->Init(size.cx, size.cy)))
			{
				DM_DELETE_OBJECT(hBmp);
				break;
			}
			m_hdc.SelectObject(m_pCurBitmap->GetBitmap());
			DM_DELETE_OBJECT(hBmp);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::Resize(int nWid,int nHei)
	{
		CSize sz(nWid,nHei);
		return Resize(sz);
	}

	DMCode DMCanvasImpl::GetSize(SIZE &size)
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
	HDC DMCanvasImpl::GetDC()
	{
		return m_hdc;
	}

	DMCode DMCanvasImpl::ReleaseDC(HDC hdc)
	{
		return DM_ECODE_NOTIMPL;
	}

	DMCode DMCanvasImpl::SaveCanvas(int *pnState/*=NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int nState = ::SaveDC(m_hdc);
			if (pnState)
			{
				*pnState = nState;
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::RestoreCanvas(int nState/*=-1*/)
	{
		::RestoreDC(m_hdc,nState);
		return DM_ECODE_OK;
	}

	///----------------------------------------
	DMCode DMCanvasImpl::AdjustHSL32(int H, int S, int L,LPRECT lpRect)
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

	DMCode DMCanvasImpl::ResetHsl()
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

	PVOID DMCanvasImpl::GetPixelBits(int* pSize)
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
	DMCode DMCanvasImpl::BitBlt(IDMCanvas*pCanvasSrc, int xSrc, int ySrc, LPCRECT lpRectDest, DWORD dwRop/* = SRCCOPY*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pCanvasSrc||NULL == lpRectDest)
			{
				break;
			}

			DMCanvasImpl *pGdiCanvasSrc = (DMCanvasImpl*)pCanvasSrc;
			::BitBlt(m_hdc,lpRectDest->left,lpRectDest->top,lpRectDest->right-lpRectDest->left,lpRectDest->bottom-lpRectDest->top,
				pGdiCanvasSrc->m_hdc,xSrc,ySrc,dwRop);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::AlphaBlend(IDMCanvas*pCanvasSrc, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pCanvasSrc||NULL == lpRectSrc||NULL == lpRectDest)
			{
				break;
			}
			BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA};
			DMCanvasImpl *pGdiCanvasSrc=(DMCanvasImpl*)pCanvasSrc;
			if (::AlphaBlend(m_hdc,lpRectDest->left,lpRectDest->top,lpRectDest->right-lpRectDest->left,lpRectDest->bottom-lpRectDest->top,
					pGdiCanvasSrc->m_hdc,lpRectSrc->left,lpRectSrc->top,lpRectSrc->right-lpRectSrc->left,lpRectSrc->bottom-lpRectSrc->top, bf))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::DrawBitamp(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha/*=0xFF*/, DMEXPEND_MODE ExpandMode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		int iSaveState = ::SaveDC(m_hdc);
		do 
		{
			if (NULL == pBitamp)
			{
				break;
			}
			DMEXPEND_MODE lowExpandMode = (DMEXPEND_MODE)LOWORD(ExpandMode);
			DMSmartPtrT<DMBitmapImpl> pGdiBitmap = (DMBitmapImpl*)pBitamp;
			RECT rcSour = {0,0,pGdiBitmap->GetWidth(),pGdiBitmap->GetHeight()};
			if (NULL == lpRectSrc) 
			{
				lpRectSrc = &rcSour;
			}

			if (NULL == lpRectDest)
			{
				lpRectDest = &rcSour;
			}

			DMAutoMemDC dcMem(m_hdc);
			dcMem.SelectObject(pGdiBitmap->GetBitmap());
			BLENDFUNCTION bf = {AC_SRC_OVER,0,alpha,AC_SRC_ALPHA};
			int nWidth		 = 0;
			int nHeight	     = 0;
			if (DEM_TILE == lowExpandMode) // ƽ��
			{
				::IntersectClipRect(m_hdc, lpRectDest->left,lpRectDest->top,lpRectDest->right,lpRectDest->bottom);
				nWidth	= lpRectSrc->right - lpRectSrc->left;
				nHeight = lpRectSrc->bottom - lpRectSrc->top;
				for(int y=lpRectDest->top ;y<lpRectDest->bottom;y+=nHeight)
				{
					for(int x=lpRectDest->left; x<lpRectDest->right; x+=nWidth)
					{
						if (::AlphaBlend(m_hdc,x,y,nWidth,nHeight,dcMem,lpRectSrc->left,lpRectSrc->top,nWidth,nHeight,bf))
						{
							iErr = DM_ECODE_OK; 
						}
					}
				}
			}
			else if (DEM_STRETCH == lowExpandMode)
			{
				if (::AlphaBlend(m_hdc,lpRectDest->left,lpRectDest->top,lpRectDest->right-lpRectDest->left,lpRectDest->bottom-lpRectDest->top,
					dcMem,lpRectSrc->left,lpRectSrc->top,lpRectSrc->right-lpRectSrc->left,lpRectSrc->bottom-lpRectSrc->top,bf))
				{
					m_pCurBitmap;
					iErr = DM_ECODE_OK; 
				}
			}

			dcMem.DeleteDC(); //����������ͷţ��´�AlphaBlend�ͻ�ʧ�ܣ�gtest���Խ��
		} while (false);
		::RestoreDC(m_hdc, iSaveState);
		return iErr;
	}

	DMCode DMCanvasImpl::DrawBitmapNine(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpSrcMargin, LPCRECT lpRectDest, BYTE alpha/*=0xFF*/, DMEXPEND_MODE ExpandMode/*=DEM_STRETCH*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pBitamp)
			{
				break;
			}
			WORD hiExpandMode = HIWORD(ExpandMode);
			// 9����xDest��xSrc��Ӧ�����Ͻ�Ϊԭ�㣬9���񻮷ֵ�4�������꣬yDest��ySrc�����Ͻ�Ϊԭ�㣬9���񻮷ֵ�4��������
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

			//����Ŀ��λ��
			int iDestWidth = lpRectDest->right-lpRectDest->left;
			int iDestHeight = lpRectDest->bottom-lpRectDest->top;

			if((lpSrcMargin->left+lpSrcMargin->right) > iDestWidth)// ��Ե��ȴ���Ŀ���ȵĴ���
			{
				if (lpSrcMargin->left >= iDestWidth)// ֻ������߲���
				{
					xSrc[1] = xSrc[2] = xSrc[3] = xSrc[0]+iDestWidth;
					xDest[1] = xDest[2] = xDest[3] = xDest[0]+iDestWidth;
				}
				else if(lpSrcMargin->right >= iDestWidth)// ֻ�����ұ߲���
				{
					xSrc[0] = xSrc[1] = xSrc[2] = xSrc[3]-iDestWidth;
					xDest[0] = xDest[1] = xDest[2] = xDest[3]-iDestWidth;
				}
				else//�Ȼ�����߲��֣�ʣ������ұ����
				{
					int nRemain=xDest[3]-xDest[1];
					xSrc[2] = xSrc[3]-nRemain;
					xDest[2] = xDest[3]-nRemain;
				}
			}

			if (lpSrcMargin->top + lpSrcMargin->bottom > iDestHeight)
			{
				if (lpSrcMargin->top >= iDestHeight)// ֻ�����ϱ߲���
				{
					ySrc[1] = ySrc[2] = ySrc[3] = ySrc[0]+iDestHeight;
					yDest[1] = yDest[2] = yDest[3] = yDest[0]+iDestHeight;
				}
				else if(lpSrcMargin->bottom >= iDestHeight)// ֻ�����±߲���
				{
					ySrc[0] = ySrc[1] = ySrc[2] = ySrc[3]-iDestHeight;
					yDest[0] = yDest[1] = yDest[2] = yDest[3]-iDestHeight;
				}
				else// �Ȼ�����߲��֣�ʣ������ұ����
				{
					int nRemain=yDest[3]-yDest[1];
					ySrc[2] = ySrc[3]-nRemain;
					yDest[2] = yDest[3]-nRemain;
				}
			}

			//�������ģʽ
			DMEXPEND_MODE mode[3][3]={//�ĸ��߽Ǵ�������Կ���Դ��Ŀ��ı߽Ǿ��δ�С����һ��
				{(DMEXPEND_MODE)MAKELONG(DEM_STRETCH,hiExpandMode),ExpandMode,(DMEXPEND_MODE)MAKELONG(DEM_STRETCH,hiExpandMode) },
				{ExpandMode,ExpandMode,ExpandMode},
				{(DMEXPEND_MODE)MAKELONG(DEM_STRETCH,hiExpandMode),ExpandMode,(DMEXPEND_MODE)MAKELONG(DEM_STRETCH,hiExpandMode) }
			};

			for (int y=0;y<3;y++)
			{
				if (ySrc[y] == ySrc[y+1]) // �վ���
				{
					continue;
				}
				for (int x=0;x<3;x++)
				{
					if (xSrc[x] == xSrc[x+1]) // �վ���
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

	DMCode DMCanvasImpl::DrawRectangle(LPCRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,lpRect);/// ע�⣬����ʹ�õ���lpRect
			dcMem.SelectObject(m_pCurPen->GetPen());
			::Rectangle(dcMem,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);/// ע�⣬����ʹ�õ���lpRect

			BYTE alpha = m_pCurPen->GetColor().a;
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::FillRectangle(LPCRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,rcDest,false);
			dcMem.SelectObject(m_pCurBrush->GetBrush());
			::Rectangle(dcMem,rcDest.left,rcDest.top,rcDest.right,rcDest.bottom);
		
			BYTE alpha = 0xFF;
			if (!m_pCurBrush->IsBitmap())
			{
				alpha = m_pCurBrush->GetColor().a;
			}
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::FillSolidRect(LPCRECT lpRect,DMColor clr)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcDest = lpRect;
			if (NULL == lpRect||rcDest.IsRectEmpty())
			{
				break;
			}
#if 1
	
			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,lpRect);
			HBRUSH hBrush = ::CreateSolidBrush(clr.ToCOLORREF());
			::FillRect(dcMem,lpRect,hBrush);
			AlphaBlendRestore(dcMem,clr.a);
			DM_DELETE_OBJECT(hBrush);
#else
			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			rcDest.OffsetRect(m_ptOrg);// �ص�0,0λ��
			if (rcDest.left<0||rcDest.top<0)
			{
				break;
			}

			LPBYTE lpPixStart = m_pCurBitmap->m_DibHelper.m_pPixelBits+rcDest.top*m_pCurBitmap->m_DibHelper.m_nBPS+rcDest.left*4;
			int iRow = 0;int iCol=0;int cx = rcDest.Width();int cy = rcDest.Height();
			UINT* pCopy = new UINT[cx];
			for (int i=0;i<cx;i++)
			{
				memcpy(pCopy+i,&clr,4);
			}
			BYTE* pBit = NULL;
			for (iRow=0;iRow<cy;iRow++)
			{
				pBit = lpPixStart + iRow*m_pCurBitmap->m_DibHelper.m_nBPS;
				memcpy(pBit,pCopy,cx*4);
			}
			DM_DELETE_ARRAY(pCopy);
#endif 
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::ClearRect(LPCRECT lpRect,DMColor clr)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}
			int iWid = lpRect->right-lpRect->left;
			int iHei = lpRect->bottom-lpRect->top;

			if (0>=iWid||0>=iHei)
			{
				iErr = DM_ECODE_OK;
				break;
			}
			LPDWORD pBits = NULL;
			HBITMAP hBmp  = DMDIBHelper::CreateDIBSection(m_hdc,iWid,iHei,(void**)&pBits);
			if (NULL == pBits)
			{
				break;
			}
			DMAutoMemDC dcMem(m_hdc);
			dcMem.SelectObject(hBmp);
			DWORD dwColor = clr.ToBGRA();
			LPDWORD p     = pBits;
			int nCount	  = iWid*iHei;
			for(int i=0;i<nCount;i++)
			{
				*pBits++ = dwColor;
			}
			::BitBlt(m_hdc,lpRect->left,lpRect->top,iWid,iHei,dcMem,0,0,SRCCOPY);
			dcMem.DeleteDC();
			::DeleteObject(hBmp);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::DrawRoundRect(LPCRECT lpRect, const POINT &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,lpRect);/// ע�⣬����ʹ�õ���lpRect
			dcMem.SelectObject(m_pCurPen->GetPen());
			::RoundRect(dcMem,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,pt.x,pt.y);/// ע�⣬����ʹ�õ���lpRect

			BYTE alpha = m_pCurPen->GetColor().a;
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::FillRoundRect(LPCRECT lpRect,const POINT &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,lpRect);/// ע�⣬����ʹ�õ���lpRect
			dcMem.SelectObject(m_pCurBrush->GetBrush());
			::RoundRect(dcMem,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,pt.x,pt.y);/// ע�⣬����ʹ�õ���lpRect

			BYTE alpha = 0xFF;
			if (!m_pCurBrush->IsBitmap())
			{
				alpha = m_pCurBrush->GetColor().a;
			}
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::DrawEllipse(LPCRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,lpRect);/// ע�⣬����ʹ�õ���lpRect
			dcMem.SelectObject(m_pCurPen->GetPen());
			::Ellipse(dcMem,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);/// ע�⣬����ʹ�õ���lpRect

			BYTE alpha = m_pCurPen->GetColor().a;
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::FillEllipse(LPCRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,lpRect);/// ע�⣬����ʹ�õ���lpRect
			dcMem.SelectObject(m_pCurBrush->GetBrush());
			::Ellipse(dcMem,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);/// ע�⣬����ʹ�õ���lpRect

			BYTE alpha = 0xFF;
			if (!m_pCurBrush->IsBitmap())
			{
				alpha = m_pCurBrush->GetColor().a;
			}
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::Polyline(LPPOINT lpPt,int cPoints)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcSrc;
			for (int i=0;i<cPoints;i++)
			{
				rcSrc.left = min(lpPt[i].x,rcSrc.left);
				rcSrc.top = min(lpPt[i].y,rcSrc.top);
				rcSrc.right = max(lpPt[i].x,rcSrc.right);
				rcSrc.bottom = max(lpPt[i].y,rcSrc.bottom);
			}
			
			int nPenWid  = m_pCurPen->GetWidth();
			rcSrc.left	 -= nPenWid;
			rcSrc.top	 -= nPenWid;
			rcSrc.bottom += nPenWid;
			rcSrc.right  += nPenWid;
	
			CRect rcDest = GetRealClipRect(rcSrc);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,rcDest);
			dcMem.SelectObject(m_pCurPen->GetPen());
			::Polyline(dcMem,lpPt, cPoints);
			BYTE alpha = m_pCurPen->GetColor().a;
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;

	}

	DMCode DMCanvasImpl::GradientFill(DMColor clrBegin,DMColor clrEnd,LPCRECT lpRect,BOOL bVert,BYTE alpha/*=0xFF*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			if (0xFF != alpha)
			{
				int nWid = lpRect->right-lpRect->left;
				int nHei = lpRect->bottom-lpRect->top;
				RECT rc  = {0,0,nWid,nHei};
				HBITMAP hBmpMem = ::CreateCompatibleBitmap(m_hdc,nWid,nHei);
				DMAutoMemDC dcMem(m_hdc);
				dcMem.SelectObject(hBmpMem);
				GradientFillRect(dcMem,&rc,clrBegin,clrEnd,bVert);
				BLENDFUNCTION bf={AC_SRC_OVER,0,alpha,AC_SRC_ALPHA};
				 ::AlphaBlend(m_hdc,lpRect->left,lpRect->top,nWid,nHei,dcMem,0,0,nWid,nHei,bf);
				dcMem.DeleteDC();
				::DeleteObject(hBmpMem);
				iErr = DM_ECODE_OK;
			}
			else
			{
				if (GradientFillRect(m_hdc,lpRect,clrBegin,clrEnd,bVert))
				{
					iErr = DM_ECODE_OK;
				}
			}

		} while (false);
		return iErr;
	}

	///----------------------------------
	DMColor DMCanvasImpl::SetTextColor(DMColor clr)
	{
		DMColor ClrOld = m_CurTextColor;
		m_CurTextColor = clr;
		::SetTextColor(m_hdc,clr.ToCOLORREF());
		return ClrOld;
	}

	DMColor DMCanvasImpl::GetTextColor()
	{
		return m_CurTextColor;
	}

	DMCode DMCanvasImpl::MeasureText(LPCWSTR lpString, int nCount, OUT SIZE *pSize)
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

			if (TRUE == ::GetTextExtentPoint32W(m_hdc,lpString,nCount,pSize))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::DrawText(LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha/*=0xFF*/)
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

			if (uFormat & DT_CALCRECT)
			{
				::DrawText(m_hdc,lpString,nCount,lpRect,uFormat);
				iErr = DM_ECODE_OK;
				break;
			}
			
			// �ȼ�����ʵ���������С
			CRect rcMeasure = *lpRect;// ȡ���Ͻ�����
			::DrawText(m_hdc,lpString,nCount,rcMeasure,uFormat|DT_CALCRECT);
			CRect rcAll = lpRect;
			int nMeasureWid = rcMeasure.Width();int nMeasureHei = rcMeasure.Height();
			int nWid = lpRect->right-lpRect->left;int nHei = lpRect->bottom-lpRect->top;
			if (uFormat&DT_CENTER &&nWid>nMeasureWid)
			{
				rcMeasure.left  = lpRect->left+(nWid-nMeasureWid)/2;
				rcMeasure.right = rcMeasure.left+nMeasureWid;
			}
			if (uFormat&DT_VCENTER&&nHei>nMeasureHei)
			{
				rcMeasure.top    = lpRect->top+(nHei-nMeasureHei)/2;
				rcMeasure.bottom = rcMeasure.top+nMeasureHei;
			}
			if (uFormat&DT_RIGHT)
			{
				rcMeasure.right = rcAll.right;
				rcMeasure.left  = rcMeasure.right-nMeasureWid;
			}
			if (uFormat&DT_BOTTOM)
			{
				rcMeasure.bottom = rcAll.bottom;
				rcMeasure.top    = rcMeasure.bottom-nMeasureHei;
			}
			
			//CRect rcDest = GetRealClipRect(rcMeasure);//�Ѽ������С���Ͳ��òü���
			CRect rcDest = rcMeasure&rcAll;
			if (rcDest.IsRectEmpty())
			{
				break;
			}
			if (rcDest.Width()*rcDest.Height()>2000*2000)// ��ֹ�ⲿ�Ƿ����ν�����
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,rcDest,true,true);
			DMColor TextClr = m_CurTextColor;
			TextClr.PreMultiply(alpha);
			::SetTextColor(dcMem, TextClr.ToCOLORREF());
			::DrawTextW(dcMem,lpString,nCount,rcDest,uFormat);
			AlphaBlendRestore(dcMem,TextClr.a);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::TextOut(LPCWSTR lpString, int nCount, int x, int  y, BYTE alpha/*=0xFF*/)
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

			SIZE size = {0};
			MeasureText(lpString,nCount,&size);
			CRect rcSrc(x,y,x+size.cx,y+size.cy);
			CRect rcDest = GetRealClipRect(rcSrc);
			if (rcDest.IsRectEmpty())
			{
				break;
			}
			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,rcDest,true,true);
			DMColor TextClr = m_CurTextColor;
			TextClr.PreMultiply(alpha);
			::SetTextColor(dcMem, TextClr.ToCOLORREF());
			::TextOutW(dcMem,x,y,lpString,nCount);
			AlphaBlendRestore(dcMem,TextClr.a);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	///----------------------------------
	DMCode DMCanvasImpl::OffsetViewportOrg(int dx, int dy, OUT LPPOINT lpPoint /*= NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lpPoint)
			{
				*lpPoint = m_ptOrg;
			}

			m_ptOrg.x += dx;
			m_ptOrg.y += dy;
			if (::SetViewportOrgEx(m_hdc, m_ptOrg.x, m_ptOrg.y, NULL))
			{
				iErr = DM_ECODE_OK;
			}

		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::SetViewportOrgEx(int dx, int dy,  OUT LPPOINT lpPoint /*= NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lpPoint)
			{
				*lpPoint = m_ptOrg;
			}

			m_ptOrg.x = dx;
			m_ptOrg.y = dy;
			if (::SetViewportOrgEx(m_hdc, m_ptOrg.x, m_ptOrg.y, NULL))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::GetViewportOrg(LPPOINT lpPoint)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lpPoint)
			{
				*lpPoint = m_ptOrg;
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	///----------------------------------
	DMCode DMCanvasImpl::PushClip(IDMRegion* pRegion,int fnCombineMode/*=RGN_AND*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pRegion)
			{
				break;
			}

			DMRegionImpl* pGdiRgn = (DMRegionImpl*)pRegion;
			HRGN hRgn = ::CreateRectRgn(0,0,0,0);
			::CombineRgn(hRgn,pGdiRgn->GetRegion(),NULL,RGN_COPY);
			::OffsetRgn(hRgn,m_ptOrg.x,m_ptOrg.y);
			::SaveDC(m_hdc);							// PopClip�лָ�DC
			::ExtSelectClipRgn(m_hdc,hRgn,fnCombineMode);
			::DeleteObject(hRgn);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::PushClip(LPCRECT lpRect,int fnCombineMode/*=RGN_AND*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			RECT rc = *lpRect;
			::OffsetRect(&rc,m_ptOrg.x,m_ptOrg.y);
			HRGN hRgn = ::CreateRectRgnIndirect(&rc);
			::SaveDC(m_hdc);// PopClip�лָ�DC
			::ExtSelectClipRgn(m_hdc,hRgn,fnCombineMode);
			::DeleteObject(hRgn);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::PopClip()
	{
		::RestoreDC(m_hdc,-1);
		return DM_ECODE_OK;
	}

	DMCode DMCanvasImpl::GetClipBox(LPRECT lpRect)
	{
		::GetClipBox(m_hdc,lpRect);
		return DM_ECODE_OK;
	}

	DMCode DMCanvasImpl::GetClipRegion(IDMRegion **ppRegion)
	{
		DMRegionImpl *pRgn  = new DMRegionImpl();
		::GetClipRgn(m_hdc,pRgn->GetRegion());
		POINT pt = {-m_ptOrg.x,-m_ptOrg.y};
		pRgn->Offset(pt);
		*ppRegion = pRgn;
		return DM_ECODE_OK;
	}

	///----------------------------------
	DMCode DMCanvasImpl::ExcludeClipRect(LPCRECT lpRect)
	{
		::ExcludeClipRect(m_hdc,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);
		return DM_ECODE_OK;
	}

	DMCode DMCanvasImpl::IntersectClipRect(LPCRECT lpRect)
	{
		::IntersectClipRect(m_hdc,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);
		return DM_ECODE_OK;
	}

	DMCode DMCanvasImpl::SaveBmpToFile(LPCWSTR pszFileName)
	{
		if (m_pCurBitmap)
		{
			return m_pCurBitmap->SaveFile(pszFileName);
		}
		return DM_ECODE_FAIL;
	}

	DMCode DMCanvasImpl::AdjustAlpha(byte byAlpha, LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcBackup = lpRect;
			rcBackup.OffsetRect(m_ptOrg);
			iErr = m_pCurBitmap->AdjustAlpha(byAlpha,rcBackup);
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::AlphaBackup(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect||m_pCurBitmap.isNull())
			{
				break;
			}
			CRect rcBackup = lpRect;
			rcBackup.OffsetRect(m_ptOrg);
			iErr = m_pCurBitmap->AlphaBackup(rcBackup);
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::AlphaRestore()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pCurBitmap.isNull())
			{
				break;
			}
			iErr = m_pCurBitmap->AlphaRestore();
		} while (false);
		return iErr;
	}

	DMAutoMemDC DMCanvasImpl::AlphaBlendBackup(DMAutoMemDC& dcMem,LPCRECT lpRect,bool bInherit,bool bCopy)
	{
		SaveCanvas();
		m_RcTemp = lpRect;
		int nWid = m_RcTemp.Width();
		int nHei = m_RcTemp.Height();
		m_DIBTemp.CreateDIBSection(m_hdc,nWid,nHei);
		
		::SetBkMode(dcMem,TRANSPARENT);
		dcMem.SelectObject(m_DIBTemp.m_hBitmap);
		::SetViewportOrgEx(dcMem,-lpRect->left,-lpRect->top,NULL);

		// �������޻���------------------------------------
		if (bInherit)
		{
			dcMem.SelectObject(m_pCurPen->GetPen());
			dcMem.SelectObject(m_pCurBrush->GetBrush());
			dcMem.SelectObject(m_pCurFont->GetFont());
			::SetTextColor(dcMem, m_CurTextColor.ToCOLORREF());
		}
		else
		{
			dcMem.SelectObject((HPEN)GetStockObject(NULL_PEN));
			dcMem.SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));
			dcMem.SelectObject((HFONT)GetStockObject(DEFAULT_GUI_FONT));
			::SetTextColor(dcMem,RGBA(0,0,0,0));
		}
		m_bCopyTemp = bCopy;
		if (bCopy)
		{
			::BitBlt(dcMem,lpRect->left, lpRect->top, nWid, nHei,m_hdc,lpRect->left, lpRect->top, SRCCOPY);
		}

		if (m_DIBTemp.m_pPixelBits)
		{
			BYTE* p = m_DIBTemp.m_pPixelBits+3;
			for (int i=0; i<m_DIBTemp.m_nImageSize; i+=4,p+=4)
			{
				*p = 1;
			}
		}
		else
		{
			LOG_ERR("CreateDIBSectionʧ����!\n");
		}

		return dcMem;
	}

	bool DMCanvasImpl::AlphaBlendRestore(DMAutoMemDC& dcMem,BYTE alpha)
	{
		int nWid = m_RcTemp.Width();
		int nHei = m_RcTemp.Height();
		if (m_DIBTemp.m_pPixelBits)
		{
			BYTE* p = m_DIBTemp.m_pPixelBits+3;
			if (m_bCopyTemp)
			{
				for (int i=0; i<m_DIBTemp.m_nImageSize; i+=4,p+=4)
				{
					*p -= 1;
					if (0 == *p)
					{
						memset(p-3,0,3);// ��xp,win7-32����Ҫʹ�ô˷�ʽ
					}
					//if(*p==0) *p=0xff;
					//else memset(p-3,0,4);
				}
			}
			else
			{
				for (int i=0; i<m_DIBTemp.m_nImageSize; i+=4,p+=4)
				{
					*p -= 1;
				}
			}
		}//if (m_DIBTemp.m_pPixelBits)

		BLENDFUNCTION bf = {AC_SRC_OVER,0,alpha,AC_SRC_ALPHA};
		BOOL bRet = ::AlphaBlend(m_hdc,m_RcTemp.left,m_RcTemp.top, nWid, nHei,
			dcMem,m_RcTemp.left,m_RcTemp.top,nWid, nHei, bf);

		dcMem.DeleteDC();
		m_DIBTemp.DIBRelease();
		RestoreCanvas();
		return TRUE == bRet;
	}

	CRect DMCanvasImpl::GetRealClipRect(LPCRECT lpRect)
	{
		CRect rcSrc  = lpRect;
		CRect rcClip;
		::GetClipBox(m_hdc,rcClip);
		CRect rcDest = rcClip&rcSrc;
		return rcDest;
	}

	BOOL DMCanvasImpl::GradientFillRect(HDC hdc, const RECT *rcFill, DMColor clrBegin,DMColor clrEnd,BOOL bVert)
	{// �˺��������Լ�ʹ�����Բ��ʵ�֣���ˮƽ��ֻ��õ�һ�м��ɣ�Ȼ��memcpy��������
		TRIVERTEX        vert[2] ;
		vert [0] .x      = rcFill->left;
		vert [0] .y      = rcFill->top;
		vert [0] .Red    = (COLOR16)clrBegin.r<<8;
		vert [0] .Green  = (COLOR16)clrBegin.g<<8;
		vert [0] .Blue   = (COLOR16)clrBegin.b<<8;
		vert [0] .Alpha  = (COLOR16)clrBegin.a<<8;

		vert [1] .x      = rcFill->right;
		vert [1] .y      = rcFill->bottom; 
		vert [1] .Red    = (COLOR16)clrEnd.r<<8;
		vert [1] .Green  = (COLOR16)clrEnd.g<<8;
		vert [1] .Blue   = (COLOR16)clrEnd.b<<8;
		vert [1] .Alpha  = (COLOR16)clrEnd.a<<8;

		GRADIENT_RECT  gRect = {0,1};
		return ::GradientFill(hdc,vert,2,&gRect,1,bVert?GRADIENT_FILL_RECT_V:GRADIENT_FILL_RECT_H);
	}

	DMCode DMCanvasImpl::DrawArc(LPCRECT lpRect,float startAngle, float sweepAngle)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}
			CRect rcArc = *lpRect;
			int iPenWid = (m_pCurPen->GetWidth()+1)/2;
			rcArc.InflateRect(iPenWid,iPenWid,iPenWid,iPenWid);
			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,rcArc);/// ע�⣬����ʹ�õ���lpRect
			dcMem.SelectObject(m_pCurPen->GetPen());

			CPoint ptCenter((lpRect->left+lpRect->right)/2,(lpRect->top+lpRect->bottom)/2);
			int xWid = lpRect->right - ptCenter.x;
			int yHei = ptCenter.y - lpRect->top;
			float startAngleMath = startAngle*3.1415926f/180.f;// ת����ʵ�ɼ���ĽǶ�
			float sweepAngleMath = (startAngle+sweepAngle)*3.1415926f/180.f;// ת����ʵ�ɼ���ĽǶ�
			int nXStartArc = ptCenter.x + (int)(xWid*cos(startAngleMath));
			int nYStartArc = ptCenter.y + (int)(yHei*sin(startAngleMath));
			int nXEndArc   = ptCenter.x + (int)(xWid*cos(sweepAngleMath));
			int nYEndArc   = ptCenter.y + (int)(yHei*sin(sweepAngleMath));
			::Arc(dcMem,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,nXEndArc,nYEndArc,nXStartArc,nYStartArc);// start�ź���,������gdi+������һ��,����˳ʱ��
			
			BYTE alpha = m_pCurPen->GetColor().a;
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMCanvasImpl::FillPie(LPCRECT lpRect,float startAngle, float sweepAngle)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			CRect rcDest = GetRealClipRect(lpRect);
			if (rcDest.IsRectEmpty())
			{
				break;
			}

			DMAutoMemDC dcMem(m_hdc);AlphaBlendBackup(dcMem,rcDest,false);
			dcMem.SelectObject(m_pCurBrush->GetBrush());
			
			CPoint ptCenter((lpRect->left+lpRect->right)/2,(lpRect->top+lpRect->bottom)/2);
			int xWid = lpRect->right - ptCenter.x;
			int yHei = ptCenter.y - lpRect->top;
			float startAngleMath = startAngle*3.1415926f/180.f;// ת����ʵ�ɼ���ĽǶ�
			float sweepAngleMath = (startAngle+sweepAngle)*3.1415926f/180.f;// ת����ʵ�ɼ���ĽǶ�
			int nXStartArc = ptCenter.x + (int)(xWid*cos(startAngleMath));
			int nYStartArc = ptCenter.y + (int)(yHei*sin(startAngleMath));
			int nXEndArc   = ptCenter.x + (int)(xWid*cos(sweepAngleMath));
			int nYEndArc   = ptCenter.y + (int)(yHei*sin(sweepAngleMath));
			::Pie(dcMem,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,nXEndArc,nYEndArc,nXStartArc,nYStartArc);// start�ź���,������gdi+������һ��,����˳ʱ��

			BYTE alpha = 0xFF;
			if (!m_pCurBrush->IsBitmap())
			{
				alpha = m_pCurBrush->GetColor().a;
			}
			AlphaBlendRestore(dcMem,alpha);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}
}//namespace DM