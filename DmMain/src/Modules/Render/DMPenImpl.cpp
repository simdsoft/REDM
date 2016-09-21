#include "DmMainAfx.h"
#include "DMPenImpl.h"

namespace DM
{
	DMPenImpl::DMPenImpl(DMColor clr,int iStyle/*=PS_SOLID*/, int iWidth/*=1*/)
	{
		m_iStyle = iStyle;
		m_iWidth = iWidth;
		m_Clr	 =  clr;
		m_hPen	 = ::CreatePen(m_iStyle,m_iWidth,m_Clr.ToCOLORREF());
	}

	DMPenImpl::~DMPenImpl()
	{
		DM_DELETE_OBJECT(m_hPen);
	}

	void DMPenImpl::SetWidth(int iWidth)
	{
		m_iWidth = iWidth;
	}

	void DMPenImpl::SetColor(DMColor clr)
	{
		m_Clr = clr;
	}

	int DMPenImpl::GetWidth()
	{
		return m_iWidth;
	}

	DMColor DMPenImpl::GetColor()
	{
		return m_Clr;
	}

	int DMPenImpl::GetStyle()
	{
		return m_iStyle;
	}

	HPEN DMPenImpl::GetPen()
	{
		return m_hPen;
	}

}//namespace DM