#include "DmMainAfx.h"
#include "DUIPngGif.h"

namespace DM
{
	DUIPngGif::DUIPngGif()
	{
		m_ulElapse  = 10;
	}

	DUIPngGif::~DUIPngGif()
	{
		Clear();
	}

	// 把序列帧解析，填入
	DMCode DUIPngGif::OnAttrPngGif(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			DM::IDMSkin* pPngGifSkin = g_pDMApp->GetSkin(lpszValue);
			if (!pPngGifSkin)
				break;

			DMSmartPtrT<IDMBitmap> pAllBitMap = NULL;
			pPngGifSkin->GetBitmap(&pAllBitMap);
			if (!pAllBitMap)
				break;

			Clear();
			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender, DMREG_Render);	

			SIZE szDst = {0};
			pPngGifSkin->GetStateSize(szDst);

			int nDstX = szDst.cx;
			int nDstY = szDst.cy;
			int nSrcX = pAllBitMap->GetWidth();
			int nSrcY = pAllBitMap->GetHeight();

			UINT ulFrameCount = 0;
			pPngGifSkin->GetStates((int&)ulFrameCount);	

			BYTE* pSrcBits = (BYTE*)pAllBitMap->GetPixelBits();
			DMBufT<BYTE> pDstBits;pDstBits.AllocateBytes(nDstX*nDstY*4);
			for (UINT uSeq = 0; uSeq < ulFrameCount; uSeq++)
			{
				DMSmartPtrT<IDMBitmap> pSeqBitmap;
				pRender->CreateBitmap(&pSeqBitmap);
				memset(pDstBits,0,nDstX*nDstY*4);
				for (int j = 0 ; j < nDstY; j++)
				{
					memcpy(pDstBits+j*nDstX*4 ,pSrcBits+(nSrcX*j + uSeq*nDstX)*4, nDstX*4);
				}
				pSeqBitmap->Init(szDst.cx, szDst.cy, pDstBits);

				DMAnimateFrame* pAnimateFrame = new DMAnimateFrame;
				memset(pAnimateFrame,0,sizeof(DMAnimateFrame));
				pAnimateFrame->pBitmap = pSeqBitmap;
				pAnimateFrame->gifFrame.ctrlExt.delayTime = m_ulElapse;
				pAnimateFrame->gifFrame.imageWidth  = nDstX;
				pAnimateFrame->gifFrame.imageHeight = nDstY;
				pAnimateFrame->gifFrame.ctrlExt.disposalMethod = DM_BACKGROUND;
				AddObj(pAnimateFrame);
			}

			m_ulFrameCount = (UINT)ulFrameCount;
			pRender->CreateCanvas(nDstX,nDstY,&m_pMemCanvas);
			m_rcGif.SetRect(0,0,nDstX,nDstY);
			m_ulTotalLoopCount = -1;

			Start(); // 播放
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	// 填充动画间隔时间
	DMCode DUIPngGif::OnAttrElapse(LPCWSTR lpszValue, bool bLoadXml)
	{
		int temp = 0;
		dm_parseint(lpszValue,temp);
		if (temp)
		{
			m_ulElapse = (int)temp;
			size_t nCount = GetCount();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++)
			{
				GetObj(nIndex)->gifFrame.ctrlExt.delayTime = m_ulElapse;
			}
		}
		return DM_ECODE_OK;
	}

}//namespace DM