//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMFontImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-23	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class DMFontImpl:public IDMFont
	{
	public:
		DMFontImpl(LPLOGFONTW plf);
		~DMFontImpl();
	public:
		virtual const LOGFONTW* GetLogFont() const;
		virtual LPCWSTR GetFaceName();				// 字体名
		virtual bool IsUnderline();				    // 是否有下划线
		virtual bool IsItalic();					// 是否为斜体
		virtual bool IsStrikeOut();				    // 是否有删除线


	public:// 辅助
		HFONT  GetFont();

	protected:
		HFONT                                m_hFont;
		LOGFONTW                             m_lf;
	};



}//namespace DM