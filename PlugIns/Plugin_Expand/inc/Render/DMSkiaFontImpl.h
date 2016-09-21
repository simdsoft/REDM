//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSkiaFontImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-8-19	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class DMSkiaFontImpl:public IDMFont
	{
	public:
		DMSkiaFontImpl(LPLOGFONTW plf);
		~DMSkiaFontImpl();
	public:
		virtual const LOGFONTW* GetLogFont() const;
		virtual LPCWSTR GetFaceName();				// 字体名
		virtual bool IsUnderline();				    // 是否有下划线
		virtual bool IsItalic();					// 是否为斜体
		virtual bool IsStrikeOut();				    // 是否有删除线

	public:// 辅助
		SkPaint GetSkPaint();
		SkTypeface* GetSkTypeFace();

	protected:
		// SkTypeface代表指定的字体风格，可以用来测量和绘制文字，也就是说不仅可以用于绘制文本，还可以用来测量文本
		SkTypeface                          *m_SkFont;	 // 定义字体
		SkPaint                              m_SkPaint;  // 定义文字绘制属性
		LOGFONTW                             m_lf;
	};

}//namespace DM