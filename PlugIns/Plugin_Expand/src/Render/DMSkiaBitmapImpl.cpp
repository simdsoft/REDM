#include "Plugin_ExpandAfx.h"
#include "DMSkiaBitmapImpl.h"

namespace DM
{
	DMSkiaBitmapImpl::DMSkiaBitmapImpl()
	{
		DM_Release();
	}

	DMSkiaBitmapImpl::~DMSkiaBitmapImpl()
	{
		DM_Release();
	}

	DMCode DMSkiaBitmapImpl::Init(int nWid,int nHei,const LPVOID pBits/*=NULL*/)
	{
		DM_Release();
		DMCode iErr = DM_ECODE_FAIL;
		HDC hdc = ::GetDC(NULL);
		do 
		{
			if (NULL == m_DibHelper.CreateDIBSection(hdc, nWid, nHei))
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
			m_SkBitmap.setInfo(SkImageInfo::Make(nWid,nHei,kN32_SkColorType,kPremul_SkAlphaType));
			m_SkBitmap.setPixels(m_DibHelper.m_pPixelBits);// SkBitmap关联像素阵列

			iErr = DM_ECODE_OK;
		} while (false);
		::ReleaseDC(NULL,hdc);
		return iErr;
	}

	DMCode DMSkiaBitmapImpl::InitFromFrame(IDMImgFrame *pImgFrame)
	{
		DM_Release();
		DMCode iErr = DM_ECODE_FAIL;
		HDC hdc = ::GetDC(NULL);
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

			if (NULL == m_DibHelper.CreateDIBSection(hdc, nWidth, nHeight))
			{
				break;
			}

			if (!DMSUCCEEDED(pImgFrame->CopyPixels(m_DibHelper.m_pPixelBits, m_DibHelper.m_nImageSize, m_DibHelper.m_nBPS)))
			{
				break;
			}
			m_SkBitmap.setInfo(SkImageInfo::Make(nWidth, nHeight, kN32_SkColorType,kPremul_SkAlphaType));
			m_SkBitmap.setPixels(m_DibHelper.m_pPixelBits);// SkBitmap关联像素阵列

			iErr = DM_ECODE_OK;
		} while (false);
		::ReleaseDC(NULL,hdc);
		return iErr;
	}

	DMCode DMSkiaBitmapImpl::LoadFromFile(LPCWSTR pszFileName,LPCWSTR pszType)
	{
		DM_Release();
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

	DMCode DMSkiaBitmapImpl::LoadFromMemory(LPBYTE pBuf,size_t szLen,LPCWSTR pszType)
	{
		DM_Release();
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

	DMCode DMSkiaBitmapImpl::AdjustHSL32(int H, int S, int L,LPRECT lpRect)
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

	DMCode DMSkiaBitmapImpl::ResetHsl()
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

	DMCode DMSkiaBitmapImpl::SaveFile(LPCWSTR pszFileName)
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

	DMCode DMSkiaBitmapImpl::AdjustAlpha(byte byAlpha, LPRECT lpRect)
	{
		m_DibHelper.AdjustAlphaRect(byAlpha,lpRect);
		return DM_ECODE_OK;
	}

	DMCode DMSkiaBitmapImpl::AlphaBackup(LPRECT lpRect)
	{
		m_DibHelper.AlphaBackup(lpRect);
		return DM_ECODE_OK;
	}

	DMCode DMSkiaBitmapImpl::AlphaRestore()
	{
		m_DibHelper.AlphaRestore();
		return DM_ECODE_OK;
	}

	int DMSkiaBitmapImpl::GetWidth()
	{
		return m_DibHelper.GetWidth();
	}

	int DMSkiaBitmapImpl::GetHeight()
	{
		return m_DibHelper.GetHeight();
	}

	int DMSkiaBitmapImpl::GetSize()
	{
		return m_DibHelper.GetSize();
	}

	int DMSkiaBitmapImpl::GetBPS()
	{
		return m_DibHelper.GetBPS();
	}

	DMColor DMSkiaBitmapImpl::GetPixel(int x, int y)
	{
		return m_DibHelper.GetPixel(x, y);
	}

	bool DMSkiaBitmapImpl::SetPixel(int x, int y, DMColor clr)
	{
		return m_DibHelper.SetPixel(x, y, clr);
	}

	PVOID DMSkiaBitmapImpl::GetPixelBits()
	{
		return m_DibHelper.GetPixelBits();
	}

	HBITMAP DMSkiaBitmapImpl::GetBitmap()
	{
		return m_DibHelper.m_hBitmap;
	}

	SkBitmap &DMSkiaBitmapImpl::GetSkBitmap()
	{
		return m_SkBitmap;
	}

	// 辅助--------------------------
	void DMSkiaBitmapImpl::DM_Release()
	{
		m_SkBitmap.reset();
		m_DibHelper.DIBRelease();
	}

}//namespace DM