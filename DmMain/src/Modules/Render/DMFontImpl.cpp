#include "DmMainAfx.h"
#include "DMFontImpl.h"

namespace DM
{

	DMFontImpl::DMFontImpl(LPLOGFONTW plf)
		:m_hFont(NULL)
	{
		if (plf)
		{
			memcpy(&m_lf, plf, sizeof(LOGFONTW));
			m_lf.lfQuality = CLEARTYPE_QUALITY;
			m_hFont = ::CreateFontIndirectW(&m_lf);
		}
	}

	DMFontImpl::~DMFontImpl()
	{
		DM_DELETE_OBJECT(m_hFont);
	}

	const LOGFONTW* DMFontImpl::GetLogFont() const
	{
		return &m_lf;
	}

	LPCWSTR DMFontImpl::GetFaceName()
	{
		return m_lf.lfFaceName;
	}

	bool DMFontImpl::IsUnderline()		
	{
		return !!m_lf.lfUnderline;
	}

	bool DMFontImpl::IsItalic()			
	{
		return !!m_lf.lfItalic;
	}

	bool DMFontImpl::IsStrikeOut()		
	{
		return !!m_lf.lfStrikeOut;
	}

	HFONT DMFontImpl::GetFont()
	{
		return m_hFont;
	}

}//namespace DM