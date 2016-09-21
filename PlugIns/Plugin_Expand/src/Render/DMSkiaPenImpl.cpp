#include "Plugin_ExpandAfx.h"
#include "DMSkiaPenImpl.h"

namespace DM
{
	DMSkiaPenImpl::DMSkiaPenImpl(DMColor clr,int iStyle/*=PS_SOLID*/, int iWidth/*=1*/)
	{
		m_iStyle = iStyle;
		m_iWidth = iWidth;
		m_Clr	 =  clr;
	}

	DMSkiaPenImpl::~DMSkiaPenImpl()
	{
	}

	void DMSkiaPenImpl::SetWidth(int iWidth)
	{
		m_iWidth = iWidth;
	}

	void DMSkiaPenImpl::SetColor(DMColor clr)
	{
		m_Clr = clr;
	}

	void DMSkiaPenImpl::SetStyle(int iStyle)
	{
		m_iStyle = iStyle;
	}

	int DMSkiaPenImpl::GetWidth()
	{
		return m_iWidth;
	}

	DMColor DMSkiaPenImpl::GetColor()
	{
		return m_Clr;
	}

	int DMSkiaPenImpl::GetStyle()
	{
		return m_iStyle;
	}
}//namespace DM