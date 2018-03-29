#include "Plugin_ExpandAfx.h"
#include "DMSkiaBrushImpl.h"
#include "DMSkiaBitmapImpl.h"

namespace DM
{
	DMSkiaBrushImpl::DMSkiaBrushImpl(DMColor clr)
	{
		m_Clr  = clr;
		m_bBmp = 0;
		m_hBrush = ::CreateSolidBrush(clr.ToCOLORREF());
	}

	DMSkiaBrushImpl::DMSkiaBrushImpl(IDMBitmap* pBmp)
	{
		m_bBmp = 1;
		DMSkiaBitmapImpl *pSkiaBmp = (DMSkiaBitmapImpl*)pBmp;
		m_SkBmp = pSkiaBmp->GetSkBitmap(); 
		m_hBrush = ::CreatePatternBrush(pSkiaBmp->GetBitmap());
	}

	DMSkiaBrushImpl::~DMSkiaBrushImpl()
	{
		DM_DELETE_OBJECT(m_hBrush);
	}

	bool DMSkiaBrushImpl::IsBitmap()
	{
		return m_bBmp;
	}

	SkBitmap DMSkiaBrushImpl::GetBitmap()
	{
		return m_SkBmp;
	}

	HBRUSH DMSkiaBrushImpl::GetBrush()
	{
		return m_hBrush;
	}

	DMColor DMSkiaBrushImpl::GetColor()
	{
		return m_Clr;
	}
}//namespace DM