#include "DmMainAfx.h"
#include "DMDC.h"

namespace DM
{
	DMAutoDC g_DMDC;
	DMAutoDC::DMAutoDC(HWND hWnd)
	{
		m_hWnd = NULL;
		m_hDC  = NULL;
		GetDC(hWnd);
	}

	DMAutoDC::~DMAutoDC()
	{
		ReleaseDC();
	}

	HDC DMAutoDC::GetDC(HWND hWnd)
	{
		ReleaseDC();
		m_hWnd = hWnd;
		m_hDC  = ::GetDC(hWnd);
		if (NULL == m_hDC)
		{
			LOG_USER("获取DC失败\n");
			m_hDC = g_DMDC;
			m_hWnd = NULL;
		}
		return m_hDC;
	}

	bool DMAutoDC::ReleaseDC()
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_hDC
				||m_hDC == g_DMDC)
			{
				break;
			}
			bRet = (1 == ::ReleaseDC(m_hWnd,m_hDC));/// If the DC was released, the return value is 1.
		} while (false);
		m_hWnd = NULL;
		m_hDC = NULL;
		return bRet;
	}

	DMAutoDC::operator const HDC()const
	{
		if (NULL == m_hDC)
		{
			return g_DMDC;
		}
		return m_hDC;
	}

	// DMAutoMemDC -----------------------------------------------------------------
	DMAutoMemDC::DMAutoMemDC(HDC hdc /*= NULL*/)
		:m_hDC(hdc)
	{
		m_hOldPen    = NULL;
		m_hOldFont   = NULL;
		m_hOldBrush  = NULL;
		m_hOldBitmap = NULL;
		m_hMemDC = ::CreateCompatibleDC(hdc);
		DMASSERT(NULL != m_hMemDC);
	}

	DMAutoMemDC::DMAutoMemDC(DMAutoMemDC &hdc)
		:m_hDC(hdc)
	{
		m_hOldPen    = NULL;
		m_hOldFont   = NULL;
		m_hOldBrush  = NULL;
		m_hOldBitmap = NULL;
		m_hMemDC = ::CreateCompatibleDC(hdc);
		DMASSERT(NULL != m_hMemDC);
	}

	DMAutoMemDC::~DMAutoMemDC()
	{
		DeleteDC();
	}

	bool DMAutoMemDC::SelectObject(HPEN hPen)
	{
		bool bRet = false;
		do 
		{
			DMASSERT(NULL != m_hMemDC);
			if (NULL == hPen)
			{
				break;
			}
			if (NULL == m_hOldPen)
			{
				m_hOldPen = (HPEN)::SelectObject(m_hMemDC,hPen);
			}
			else
			{
				::SelectObject(m_hMemDC,hPen);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMAutoMemDC::SelectObject(HFONT hFont)
	{
		bool bRet = false;
		do 
		{
			DMASSERT(NULL != m_hMemDC);
			if (NULL == hFont)
			{
				break;
			}
			if (NULL == m_hOldFont)
			{
				m_hOldFont = (HFONT)::SelectObject(m_hMemDC,hFont);
			}
			else
			{
				::SelectObject(m_hMemDC,hFont);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMAutoMemDC::SelectObject(HBRUSH hBrush)
	{
		bool bRet = false;
		do 
		{
			DMASSERT(NULL != m_hMemDC);
			if (NULL == hBrush)
			{
				break;
			}
			if (NULL == m_hOldBrush)
			{
				m_hOldBrush = (HBRUSH)::SelectObject(m_hMemDC,hBrush);
			}
			else
			{
				::SelectObject(m_hMemDC,hBrush);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMAutoMemDC::SelectObject(HBITMAP hBitmap)
	{
		bool bRet = false;
		do 
		{
			DMASSERT(NULL != m_hMemDC);
			if (NULL == hBitmap)
			{
				break;
			}
			if (NULL == m_hOldBitmap)
			{
				m_hOldBitmap = (HBITMAP)::SelectObject(m_hMemDC,hBitmap);
			}
			else
			{
				::SelectObject(m_hMemDC,hBitmap);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMAutoMemDC::DeleteDC()
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_hMemDC) // 表示已调用过DeleteDC
			{
				break;
			}
			if (m_hOldPen)
			{
				::SelectObject(m_hMemDC,m_hOldPen);
			}
			if (m_hOldFont)
			{
				::SelectObject(m_hMemDC,m_hOldFont);
			}
			if (m_hOldBrush)
			{
				::SelectObject(m_hMemDC,m_hOldBrush);
			}
			if (m_hOldBitmap)
			{
				::SelectObject(m_hMemDC,m_hOldBitmap);
			}
			m_hOldPen    = NULL;
			m_hOldFont   = NULL;
			m_hOldBrush  = NULL;
			m_hOldBitmap = NULL;
		
			::DeleteDC(m_hMemDC);
			m_hMemDC = m_hDC = NULL;
			
			bRet = true;
		} while (false);
		return bRet;
	}
}//namespace DM