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
			LOG_USER("ªÒ»°DC ß∞‹\n");
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
}//namespace DM