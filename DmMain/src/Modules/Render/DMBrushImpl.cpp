#include "DmMainAfx.h"
#include "DMBrushImpl.h"

namespace DM
{
	DMBrushImpl *DMBrushImpl::CreateSolidBrush(DMColor clr)
	{
		return new DMBrushImpl(clr);
	}

	DMBrushImpl *DMBrushImpl::CreateBitmapBrush(HBITMAP hBmp)
	{
		return new DMBrushImpl(hBmp);
	}

	DMBrushImpl::DMBrushImpl(DMColor clr)
	{
		m_Clr  = clr;
		m_bBmp = 0;
		m_hBrush = ::CreateSolidBrush(clr.ToCOLORREF());
	}

	DMBrushImpl::DMBrushImpl(HBITMAP hBmp)
	{
		m_bBmp = 1;
		m_hBrush = ::CreatePatternBrush(hBmp);
	}

	DMBrushImpl::~DMBrushImpl()
	{
		DM_DELETE_OBJECT(m_hBrush);
	}

	bool DMBrushImpl::IsBitmap()
	{
		return m_bBmp;
	}

	HBRUSH DMBrushImpl::GetBrush()
	{
		return m_hBrush;
	}

	DMColor DMBrushImpl::GetColor()
	{
		return m_Clr;
	}
}//namespace DM