#include "DmMainAfx.h"
#include "DMBitmapImpl.h"

namespace DM
{
	DMBitmapImpl::DMBitmapImpl()
	{
		m_DibHelper.DIBRelease();
	}

	DMBitmapImpl::~DMBitmapImpl()
	{
		m_DibHelper.DIBRelease();
	}

	DMCode DMBitmapImpl::Init(int nWid,int nHei,const LPVOID pBits/*=NULL*/)
	{
		m_DibHelper.DIBRelease();
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_DibHelper.CreateDIBSection(NULL, nWid, nHei))
			{
				break;
			}

			if (pBits)
			{
				memcpy(m_DibHelper.m_pPixelBits, pBits, m_DibHelper.m_nImageSize);
			}
			else
			{
				memset(m_DibHelper.m_pPixelBits, 0, m_DibHelper.m_nImageSize);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMBitmapImpl::InitFromFrame(IDMImgFrame *pImgFrame)
	{
		m_DibHelper.DIBRelease();
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pImgFrame)
			{
				break;
			}

			UINT nWidth  = 0;
			UINT nHeight = 0;
			if (!DMSUCCEEDED(pImgFrame->GetSize(nWidth, nHeight)))
			{
				break;
			}

			if (NULL == m_DibHelper.CreateDIBSection(NULL, nWidth, nHeight))
			{
				break;
			}

			if (!DMSUCCEEDED(pImgFrame->CopyPixels(m_DibHelper.m_pPixelBits, m_DibHelper.m_nImageSize, m_DibHelper.m_nBPS)))
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMBitmapImpl::LoadFromFile(LPCWSTR pszFileName,LPCWSTR pszType)
	{
		m_DibHelper.DIBRelease();
		DMCode iErr = DM_ECODE_FAIL;
		DMSmartPtrT<IDMImgDecoder> pImgDecoder;
		do 
		{
			if (!DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,NULL,DMREG_ImgDecoder)))
			{
				break;
			}

			if (!DMSUCCEEDED(pImgDecoder->LoadFromFile(pszFileName)))
			{
				break;
			}

			DMSmartPtrT<IDMImgFrame> pFrame;
			if (!DMSUCCEEDED(pImgDecoder->GetFrame(0, &pFrame)))
			{
				break;
			}

			if (!DMSUCCEEDED(InitFromFrame(pFrame)))
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMBitmapImpl::LoadFromMemory(LPBYTE pBuf,size_t szLen,LPCWSTR pszType)
	{
		m_DibHelper.DIBRelease();
		DMCode iErr = DM_ECODE_FAIL;
		DMSmartPtrT<IDMImgDecoder> pImgDecoder;
		do 
		{
			if (!DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,NULL,DMREG_ImgDecoder)))
			{
				break;
			}

			if (!DMSUCCEEDED(pImgDecoder->LoadFromMemory(pBuf, szLen)))
			{
				break;
			}

			DMSmartPtrT<IDMImgFrame> pFrame;
			if (!DMSUCCEEDED(pImgDecoder->GetFrame(0, &pFrame)))
			{
				break;
			}

			if (!DMSUCCEEDED(InitFromFrame(pFrame)))
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMBitmapImpl::AdjustHSL32(int H, int S, int L,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (false == m_DibHelper.AdjustHSL32Rect(H, S, L,lpRect))
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMBitmapImpl::ResetHsl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (false == m_DibHelper.ResetHSL32())
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMBitmapImpl::SaveFile(LPCWSTR pszFileName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (false == m_DibHelper.SaveFile(pszFileName))
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMBitmapImpl::AdjustAlpha(byte byAlpha, LPRECT lpRect)
	{
		m_DibHelper.AdjustAlphaRect(byAlpha,lpRect);
		return DM_ECODE_OK;
	}

	DMCode DMBitmapImpl::AlphaBackup(LPRECT lpRect)
	{
		m_DibHelper.AlphaBackup(lpRect);
		return DM_ECODE_OK;
	}

	DMCode DMBitmapImpl::AlphaRestore()
	{
		m_DibHelper.AlphaRestore();
		return DM_ECODE_OK;
	}

	int DMBitmapImpl::GetWidth()
	{
		return m_DibHelper.GetWidth();
	}

	int DMBitmapImpl::GetHeight()
	{
		return m_DibHelper.GetHeight();
	}

	int DMBitmapImpl::GetSize()
	{
		return m_DibHelper.GetSize();
	}

	int DMBitmapImpl::GetBPS()
	{
		return m_DibHelper.GetBPS();
	}

	DMColor DMBitmapImpl::GetPixel(int x, int y)
	{
		return m_DibHelper.GetPixel(x, y);
	}

	bool DMBitmapImpl::SetPixel(int x, int y, DMColor clr)
	{
		return m_DibHelper.SetPixel(x, y, clr);
	}

	PVOID DMBitmapImpl::GetPixelBits()
	{
		return m_DibHelper.GetPixelBits();
	}

	HBITMAP DMBitmapImpl::GetBitmap()
	{
		return m_DibHelper.m_hBitmap;
	}

}//namespace DM