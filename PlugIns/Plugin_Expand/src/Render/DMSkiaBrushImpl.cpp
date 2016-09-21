#include "Plugin_ExpandAfx.h"
#include "DMSkiaBrushImpl.h"

namespace DM
{
	DMSkiaBrushImpl::DMSkiaBrushImpl(DMColor clr)
	{
		m_Clr  = clr;
		m_bBmp = 0;
	}

	DMSkiaBrushImpl::DMSkiaBrushImpl(SkBitmap SkBmp)
	{
		m_bBmp = 1;
		m_SkBmp = SkBmp; 
	}

	DMSkiaBrushImpl::~DMSkiaBrushImpl()
	{
	}

	bool DMSkiaBrushImpl::IsBitmap()
	{
		return m_bBmp;
	}

	SkBitmap DMSkiaBrushImpl::GetBitmap()
	{
		return m_SkBmp;
	}

	DMColor DMSkiaBrushImpl::GetColor()
	{
		return m_Clr;
	}
}//namespace DM