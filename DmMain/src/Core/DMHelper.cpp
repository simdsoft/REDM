#include "DmMainAfx.h"
#include "DMHelper.h"

namespace DM
{
	bool GetRootPathW(wchar_t *pszPath, DWORD dwSize)
	{
		bool bRet = false;
		do 
		{
			HMODULE hMod = GetModuleHandleW(NULL);
			if (NULL == hMod)
			{
				LOG_ERR("[mid]hMod is NULL\n");
				break;
			}

			wchar_t szPath[MAX_PATH] = {0};
			if (NULL == GetModuleFileNameW(hMod, szPath, MAX_PATH))
			{
				LOG_ERR("[mid]GetModuleFileNameW fail\n");
				break;
			}

			if (wcslen(szPath)>=dwSize)
			{
				LOG_ERR("[mid]dwSize too small\n");
				break;
			}

			// 据说StringCchPrintf比swprintf_s效率低了点，好吧！
			swprintf_s(pszPath, dwSize, L"%s", szPath);
			bRet = true;
		} while (FALSE);
		return bRet;
	}

	bool GetRootDirW(wchar_t *pszPath, DWORD dwSize)
	{
		bool bRet = false;
		do 
		{
			HMODULE hMod = GetModuleHandleW(NULL);
			if (NULL == hMod)
			{
				LOG_ERR("[mid]hMod is NULL\n");
				break;
			}

			wchar_t szPath[MAX_PATH] = {0};
			if (NULL == GetModuleFileNameW(hMod, szPath, MAX_PATH))
			{
				LOG_ERR("[mid]GetModuleFileNameW fail\n");
				break;
			}
			wchar_t *p = wcsrchr(szPath,L'\\');
			if (NULL == p)
			{
				LOG_ERR("[mid]wcsrchr fail\n");
				break;
			}
			*(++p)='\0';

			if (wcslen(szPath)>=dwSize)
			{
				LOG_ERR("[mid]dwSize too small\n");
				break;
			}

			// 据说StringCchPrintf比swprintf_s效率低了点，好吧！
			swprintf_s(pszPath, dwSize, L"%s", szPath);
			bRet = true;
		} while (false);
		return bRet;
	}

	bool GetRootFullPath(const wchar_t *pszSrcPath, wchar_t *pszDestPath, DWORD dwSize)
	{
		bool bRet = false;
		do 
		{
			if (NULL == pszSrcPath||NULL == pszDestPath ||0 >= dwSize)
			{
				break;
			}
			wchar_t szExeDir[MAX_PATH] = {0};
			if (!GetRootDirW(szExeDir, MAX_PATH))
			{
				LOG_ERR("[mid]-GetRootDirW fail\n");
				break;
			}
			wmemset(pszDestPath, 0, dwSize);
			if (0 == PathCombineW(pszDestPath, szExeDir, pszSrcPath))
			{
				LOG_ERR("[mid]-PathCombineW fail\n");
				break;
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	bool CheckFileExistW(const wchar_t *pszFilePath)
	{
		bool bRet = false;
		do 
		{
			if (NULL == pszFilePath
				|| 0 == wcslen(pszFilePath))
			{
				break;
			}

			FILE *fp = NULL;
			_wfopen_s(&fp, pszFilePath, L"rb");
			if (!fp)
			{
				break;
			}

			fclose(fp);
			bRet = true;
		} while (FALSE);
		return bRet;
	}

	DWORD GetFileSizeW(const wchar_t *pszFilePath)
	{
		DWORD dwSize = 0;
		do 
		{
			if (NULL == pszFilePath
				||0 == wcslen(pszFilePath))
			{
				break;
			}

			FILE *fp = NULL;
			_wfopen_s(&fp, pszFilePath, L"rb");
			if (!fp)
			{
				break;
			}

			DWORD pos = ftell(fp);
			fseek(fp, 0, SEEK_END);
			dwSize    = ftell(fp);
			fseek(fp, pos, SEEK_SET);

			fclose(fp);
		} while (FALSE);
		return dwSize;
	}

	bool GetFileBufW(const wchar_t *pszFilePath, void **ppBuf, DWORD dwSize, DWORD &dwReadSize)
	{
		bool bRet =  false;
		do 
		{
			if (NULL == pszFilePath
				||0 == wcslen(pszFilePath)
				||NULL == *ppBuf)
			{
				break;
			}

			FILE *fp = NULL;
			_wfopen_s(&fp, pszFilePath, L"rb");
			if (!fp)
			{
				break;
			}
			dwReadSize = (DWORD)fread(*ppBuf, 1, dwSize, fp);
			fclose(fp);

			bRet = true;
		} while (FALSE);
		return bRet;
	}

	bool UnicodeToAscii(const wchar_t * pwszSrc, PCHAR pszDst, DWORD dwLen)
	{
		bool bRet = false;
		do 
		{
			int nSize = ::WideCharToMultiByte(CP_ACP, 0, pwszSrc, wcslen(pwszSrc)+1, NULL, 0, NULL, NULL);
			if (0 == nSize)// not success
			{
				break;
			}

			if ((int)dwLen<nSize
				|| NULL == pszDst)
			{
				break;
			}

			if (!::WideCharToMultiByte(CP_ACP, 0, pwszSrc, wcslen(pwszSrc)+1, pszDst, nSize, NULL, NULL))
			{
				break;
			}

			pszDst[nSize-1] = 0;
			bRet = true;
		} while (false);
		return bRet;
	}

	bool UnicodeToUtf8(const wchar_t * pwszSrc, PCHAR pszDst, DWORD dwLen)
	{
		bool bRet = false;
		do 
		{
			int nSize = ::WideCharToMultiByte(CP_UTF8, 0, pwszSrc, wcslen(pwszSrc)+1, NULL, 0, NULL, NULL);
			if (0 == nSize)// not success
			{
				break;
			}

			if ((int)dwLen<nSize
				|| NULL == pszDst)
			{
				break;
			}

			if (!::WideCharToMultiByte(CP_UTF8, 0, pwszSrc, wcslen(pwszSrc)+1, pszDst, nSize, NULL, NULL))
			{
				break;
			}

			pszDst[nSize-1] = 0;
			bRet = true;
		} while (false);
		return bRet;
	}

	bool MeetRect(LPRECT lpRect, SIZE szPic)
	{
		bool bRet = false;
		do 
		{
			if (!lpRect)
			{
				break;
			}
			int x, y, w, h;
			int nWidth = lpRect->right-lpRect->left;
			int nHeight = lpRect->bottom-lpRect->top;
			if (nWidth > 0
				&& szPic.cx > 0)
			{
				// Get Meet Rect
				if (szPic.cx > nWidth 
					|| szPic.cy > nHeight)
				{
					const float fRatio = (float)nHeight / nWidth;
					const float fPicRatio = (float)szPic.cy / szPic.cx;

					if(fPicRatio > fRatio)
					{
						w = (int)(((float)nHeight) / fPicRatio);
						h = nHeight;
						x = lpRect->left + (nWidth - w) / 2;
						y = lpRect->top;			
					}
					else
					{
						w = nWidth;
						h = (int)(((float)nWidth )* fPicRatio);
						x = lpRect->left;
						y = lpRect->top + (nHeight - h) / 2;
					}
				}
				else
				{
					w = szPic.cx;
					h = szPic.cy;
					x = lpRect->left + (nWidth - w) / 2;
					y = lpRect->top + (nHeight - h) / 2;
				}
				::SetRect(lpRect,x, y, x + w, y + h);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	void DMHiMetricToPixel(const SIZEL* lpSizeInHiMetric, LPSIZEL lpSizeInPix,HWND hWnd)
	{
		do 
		{
			if (NULL == lpSizeInHiMetric||NULL == lpSizeInPix)
			{
				break;
			}
			int nPixelsPerInchX;    // Pixels per logical inch along width
			int nPixelsPerInchY;    // Pixels per logical inch along height

			DMAutoDC hDCScreen(hWnd);
			nPixelsPerInchX = GetDeviceCaps(hDCScreen, LOGPIXELSX);
			nPixelsPerInchY = GetDeviceCaps(hDCScreen, LOGPIXELSY);


			lpSizeInPix->cx = DMMAP_LOGHIM_TO_PIX(lpSizeInHiMetric->cx, nPixelsPerInchX);
			lpSizeInPix->cy = DMMAP_LOGHIM_TO_PIX(lpSizeInHiMetric->cy, nPixelsPerInchY);

		} while (false);
	}

	void DMPixelToHiMetric(const SIZEL* lpSizeInPix, LPSIZEL lpSizeInHiMetric, HWND hWnd)
	{// 像素转换成毫米公式：(宽度像素/水平DPI)*25.4;，所以此处返回为0.01毫米
		do 
		{
			if (NULL == lpSizeInHiMetric||NULL == lpSizeInPix)
			{
				break;
			}

			int nPixelsPerInchX;    // Pixels per logical inch along width
			int nPixelsPerInchY;    // Pixels per logical inch along height

			DMAutoDC hDCScreen(hWnd);
			nPixelsPerInchX = GetDeviceCaps(hDCScreen, LOGPIXELSX);
			nPixelsPerInchY = GetDeviceCaps(hDCScreen, LOGPIXELSY);

			lpSizeInHiMetric->cx = (LONG)DMMAP_PIX_TO_LOGHIM(lpSizeInPix->cx, nPixelsPerInchX);
			lpSizeInHiMetric->cy = (LONG)DMMAP_PIX_TO_LOGHIM(lpSizeInPix->cy, nPixelsPerInchY);
		} while (false);
	}

	void GetMaskBitmaps(HBITMAP hSourceBitmap,COLORREF clrTransparent,HBITMAP &hAndMaskBitmap,HBITMAP &hXorMaskBitmap)  
	{  
		DMAutoDC hDC;
		HDC hMainDC             = ::CreateCompatibleDC(hDC);   
		HDC hAndMaskDC          = ::CreateCompatibleDC(hDC);   
		HDC hXorMaskDC          = ::CreateCompatibleDC(hDC);   

		//Get the dimensions of the source bitmap  
		BITMAP bm;  
		::GetObject(hSourceBitmap,sizeof(BITMAP),&bm);  

		hAndMaskBitmap  = ::CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);  
		hXorMaskBitmap  = ::CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);  

		//Select the bitmaps to DC  
		HBITMAP hOldMainBitmap = (HBITMAP)::SelectObject(hMainDC,hSourceBitmap);  
		HBITMAP hOldAndMaskBitmap   = (HBITMAP)::SelectObject(hAndMaskDC,hAndMaskBitmap);  
		HBITMAP hOldXorMaskBitmap   = (HBITMAP)::SelectObject(hXorMaskDC,hXorMaskBitmap);  

		//Scan each pixel of the souce bitmap and create the masks  
		COLORREF MainBitPixel;  
		for(int x=0;x<bm.bmWidth;++x)  
		{  
			for(int y=0;y<bm.bmHeight;++y)  
			{  
				MainBitPixel = ::GetPixel(hMainDC,x,y);  
				if(MainBitPixel == clrTransparent)  
				{  
					::SetPixel(hAndMaskDC,x,y,RGB(255,255,255));  
					::SetPixel(hXorMaskDC,x,y,RGB(0,0,0));  
				}  
				else  
				{  
					::SetPixel(hAndMaskDC,x,y,RGB(0,0,0));  
					::SetPixel(hXorMaskDC,x,y,MainBitPixel);  
				}  
			}  
		}  

		::SelectObject(hMainDC,hOldMainBitmap);  
		::SelectObject(hAndMaskDC,hOldAndMaskBitmap);  
		::SelectObject(hXorMaskDC,hOldXorMaskBitmap);  

		::DeleteDC(hXorMaskDC);  
		::DeleteDC(hAndMaskDC);  
		::DeleteDC(hMainDC);  
	}  

	HCURSOR CreateCursorFromBitmap(HBITMAP hSourceBitmap,bool bSimple,COLORREF clrTransparent, DWORD xHotspot,DWORD yHotspot)
	{
		HCURSOR hRetCursor = NULL;  

		do  
		{  
			if (NULL == hSourceBitmap)  
			{  
				break;  
			}  

			if (bSimple)
			{
				ICONINFO iconinfo = {0};  
				iconinfo.fIcon      = FALSE;  
				iconinfo.xHotspot   = xHotspot;  
				iconinfo.yHotspot   = yHotspot;  
				iconinfo.hbmMask    = hSourceBitmap;  
				iconinfo.hbmColor   = hSourceBitmap;  

				hRetCursor = ::CreateIconIndirect(&iconinfo);  
				break;
			}

			//Create the AND and XOR masks for the bitmap  
			HBITMAP hAndMask = NULL;  
			HBITMAP hXorMask = NULL;  
			GetMaskBitmaps(hSourceBitmap,clrTransparent,hAndMask,hXorMask);  
			if(NULL == hAndMask || NULL == hXorMask)  
			{  
				break;  
			}  

			//Create the cursor using the masks and the hotspot values provided  
			ICONINFO iconinfo = {0};  
			iconinfo.fIcon      = FALSE;  
			iconinfo.xHotspot   = xHotspot;  
			iconinfo.yHotspot   = yHotspot;  
			iconinfo.hbmMask    = hAndMask;  
			iconinfo.hbmColor   = hXorMask;  

			hRetCursor = ::CreateIconIndirect(&iconinfo);  
			DeleteObject(hAndMask);
			DeleteObject(hXorMask);

		}while(false);  
		
		return hRetCursor;  
	}



}//namespace DM