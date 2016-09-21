#include "Plugin_ExpandAfx.h"
#include "DMSkiaFontImpl.h"

namespace DM
{

	DMSkiaFontImpl::DMSkiaFontImpl(LPLOGFONTW plf)
		:m_SkFont(NULL)
	{
		if (plf)
		{
			// SkTypeface的文档可参考https://sites.google.com/site/skiadocs/developer-documentation/testing/using-gm-golden-master-baseline-tools-to-validate-results/gm-and-fonts
			memcpy(&m_lf, plf, sizeof(LOGFONTW));
#if 0
			char szFaceName[LF_FACESIZE*2] = {0};
			DM::UnicodeToUtf8(plf->lfFaceName,szFaceName, LF_FACESIZE*2);


			byte style = SkTypeface::kNormal;
			if (plf->lfItalic) 
			{
				style |= SkTypeface::kItalic;
			}

			if (FW_BOLD == plf->lfWeight)
			{
				style |= SkTypeface::kBold;
			}

			m_SkFont = SkTypeface::CreateFromName(szFaceName,(SkTypeface::Style)style);
#endif 
			m_SkFont = SkCreateTypefaceFromLOGFONT((const LOGFONTW)m_lf);
			//
			m_SkPaint.setTextSize(SkIntToScalar(abs(plf->lfHeight)));// 设置字体尺寸。 
			m_SkPaint.setUnderlineText(!!plf->lfUnderline);
			m_SkPaint.setStrikeThruText(!!plf->lfStrikeOut);

			m_SkPaint.setTextEncoding(SkPaint::kUTF16_TextEncoding);
			m_SkPaint.setAntiAlias(true);// 去锯齿
			m_SkPaint.setLCDRenderText(true);
		}
	}

	DMSkiaFontImpl::~DMSkiaFontImpl()
	{
		if (m_SkFont)// 释放，不然有内存泄露
		{
			m_SkFont->unref();
			m_SkFont = NULL;
		}
	}

	const LOGFONTW* DMSkiaFontImpl::GetLogFont() const
	{
		return &m_lf;
	}

	LPCWSTR DMSkiaFontImpl::GetFaceName()
	{
		return m_lf.lfFaceName;
	}

	bool DMSkiaFontImpl::IsUnderline()		
	{
		return !!m_lf.lfUnderline;
	}

	bool DMSkiaFontImpl::IsItalic()			
	{
		return !!m_lf.lfItalic;
	}

	bool DMSkiaFontImpl::IsStrikeOut()		
	{
		return !!m_lf.lfStrikeOut;
	}

	SkPaint DMSkiaFontImpl::GetSkPaint()
	{
		return m_SkPaint;
	}

	SkTypeface* DMSkiaFontImpl::GetSkTypeFace()
	{
		return m_SkFont;
	}

}//namespace DM