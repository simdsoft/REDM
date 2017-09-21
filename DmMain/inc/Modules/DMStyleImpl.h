//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMStyleImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-29	1.0			
//-------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIWindow"/>的style xml属性定义
	/// </summary>
	class DUIStyleAttr
	{
	public:
		static wchar_t* STRING_id;											///< styleID,相当于窗口的name，唯一标识,示例:id="style"
		static wchar_t* SKIN_skin;											///< 窗口的style背景图,示例:skin="checkbox"
		static wchar_t* SKIN_ncskin;										///< 窗口的styleNC背景图,仅在设置了ncmargin时有效,示例:ncskin="dui_frame"
		static wchar_t* RECT_ncmargin;                                      ///< 窗口的style非客户区四边距,示例:ncmargin="2,2,2,2"
		static wchar_t* STRING_cursor;                                      ///< 窗口的style默认光标,示例:cursor="hand"
		static wchar_t* COLOR_clrbg;                                        ///< 窗口的style默认背景色,在背景图为空时使用,示例:clrbg="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrbghover;                                   ///< 窗口的style默认hover背景色,在背景图为空时使用,示例:clrbghover="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrbgpush;                                    ///< 窗口的style默认push背景色,在背景图为空时使用,示例:clrbgpush="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrbgdisable;                                 ///< 窗口的style默认disable背景色,在背景图为空时使用,示例:clrbgdisable="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrbgnoactive;                                ///< 窗口的style默认noactive背景色,在背景图为空时使用,示例:clrbgnoactive="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrnc;                                        ///< 窗口的style默认NC背景色,在NC背景图为空时使用,示例:clrnc="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrnchover;                                   ///< 窗口的style默认NC hover背景色,在NC背景图为空时使用,示例:clrnchover="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrncpush;                                    ///< 窗口的style默认NC push背景色,在NC背景图为空时使用,示例:clrncpush="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrncdisable;                                 ///< 窗口的style默认NC disable背景色,在NC背景图为空时使用,示例:clrncdisable="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrncnoactive;                                ///< 窗口的style默认NC noactive背景色,在NC背景图为空时使用,示例:clrncnoactive="pbgra(ff,00,ff,ff)"
		
		static wchar_t* BYTE_alpha;                                         ///< 窗口的style透明度,会在背景图或背景色的alpha值再次预乘,示例:alpha="0x80"
		static wchar_t* COLOR_clrtext;                                      ///< 窗口的style默认normarl文本色,示例:clrtext="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrtexthover;                                 ///< 窗口的style默认hover文本色,示例:clrtexthover="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrtextpush;									///< 窗口的style默认push文本色,示例:clrtextpush="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrtextdisable;                               ///< 窗口的style默认disable文本色,示例:clrtextdisable="pbgra(ff,00,ff,ff)"
		static wchar_t* COLOR_clrtextnoactive;                              ///< 窗口的style默认noactive文本色,示例:clrtextnoactive="pbgra(ff,00,ff,ff)"

		static wchar_t* FONT_font;											///< 窗口的style默认normarl字体,示例:font="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		static wchar_t* FONT_fonthover;										///< 窗口的style默认hover字体,示例:fonthover="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		static wchar_t* FONT_fontpush;										///< 窗口的style默认push字体,示例:fontpush="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		static wchar_t* FONT_fontdisable;									///< 窗口的style默认disable字体,示例:fontdisable="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		static wchar_t* FONT_fontnoactive;									///< 窗口的style默认noactive字体,示例:fontnoactive="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位,允许空格!内部做了空格去除操作
		
		static wchar_t* INT_textmode;                                       ///< 窗口的style默认textalign,使用数字表示,示例textmode="0x100"
		static wchar_t* OPTION_align;                                       ///< 窗口的style默认水平align,left对应Align_Left,center对应Align_Center,right对应Align_Right,示例align="left"
		static wchar_t*	OPTION_valign;                                      ///< 窗口的style默认竖直align,top对应VAlign_Top,middle对应VAlign_Middle,bottom对应VAlign_Bottom,示例valign="top"
		static wchar_t* bool_bdot;									    	///< 窗口的style默认文本过长时使用...,示例bdot="1"
	};
	DMAttrValueInit(DUIStyleAttr,STRING_id)
	DMAttrValueInit(DUIStyleAttr,SKIN_skin)DMAttrValueInit(DUIStyleAttr,SKIN_ncskin)DMAttrValueInit(DUIStyleAttr,RECT_ncmargin)
	DMAttrValueInit(DUIStyleAttr,STRING_cursor)
	DMAttrValueInit(DUIStyleAttr,COLOR_clrbg)DMAttrValueInit(DUIStyleAttr,COLOR_clrbghover)DMAttrValueInit(DUIStyleAttr,COLOR_clrbgpush)DMAttrValueInit(DUIStyleAttr,COLOR_clrbgdisable)DMAttrValueInit(DUIStyleAttr,COLOR_clrbgnoactive)
	DMAttrValueInit(DUIStyleAttr,COLOR_clrnc)DMAttrValueInit(DUIStyleAttr,COLOR_clrnchover)DMAttrValueInit(DUIStyleAttr,COLOR_clrncpush)DMAttrValueInit(DUIStyleAttr,COLOR_clrncdisable)DMAttrValueInit(DUIStyleAttr,COLOR_clrncnoactive)
	DMAttrValueInit(DUIStyleAttr,BYTE_alpha)
	DMAttrValueInit(DUIStyleAttr,COLOR_clrtext)DMAttrValueInit(DUIStyleAttr,COLOR_clrtexthover)DMAttrValueInit(DUIStyleAttr,COLOR_clrtextpush)DMAttrValueInit(DUIStyleAttr,COLOR_clrtextdisable)DMAttrValueInit(DUIStyleAttr,COLOR_clrtextnoactive)
	DMAttrValueInit(DUIStyleAttr,FONT_font)DMAttrValueInit(DUIStyleAttr,FONT_fonthover)DMAttrValueInit(DUIStyleAttr,FONT_fontpush)
	DMAttrValueInit(DUIStyleAttr,FONT_fontdisable)DMAttrValueInit(DUIStyleAttr,FONT_fontnoactive)
	DMAttrValueInit(DUIStyleAttr,INT_textmode)DMAttrValueInit(DUIStyleAttr,OPTION_align)DMAttrValueInit(DUIStyleAttr,OPTION_valign)DMAttrValueInit(DUIStyleAttr,bool_bdot)
}

namespace DM
{
	enum
	{
		Align_Left               = 0x000UL, // align = left
		Align_Center             = 0x100UL, // align = center
		Align_Right              = 0x200UL, // align = right

		VAlign_Top               = 0x0000UL, // valign = top
		VAlign_Middle            = 0x1000UL, // valign = middle
		VAlign_Bottom            = 0x2000UL, // valign = bottom
	};
	class DMStyleImpl:public IDMStyle
	{
		DMDECLARE_CLASS_NAME(DMStyleImpl,L"style",DMREG_Style);
	public:
		DMStyleImpl();
		~DMStyleImpl();

	public:
		DMCode GetID(wchar_t* lpszId, int iSize);	
		DMCode CopyData(IDMStyle* pStyle);

		//---------------------------------------------------
		// Function Des: 文本相关
		//---------------------------------------------------
		DMCode GetTextAlign(UINT &ulAlign);   
		DMCode GetTextColor(int iState,DMColor &Clr);   
		DMCode GetTextFont(int iState, IDMFont** ppFont); 
		DMCode GetTextStates(int &iState);

		//---------------------------------------------------
		// Function Des: 光标相关
		//---------------------------------------------------
		DMCode GetCursor(HCURSOR &hCursor);

		//---------------------------------------------------
		// Function Des: 窗口相关
		//---------------------------------------------------
		DMCode GetBgSkin(IDMSkin** ppSkin);	
		DMCode GetBgColor(int iState, DMColor &Clr);
		DMCode GetBgClrStates(int &iState);

		DMCode GetNcMargin(LPRECT lpRect);
		DMCode GetNcColor(int iState, DMColor &Clr);
		DMCode GetNcSkin(IDMSkin** ppSkin);	

		DMCode GetAlpha(byte &alpha);
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_STRING_ATTRIBUTE(DMAttr::DUIStyleAttr::STRING_id, m_strID, DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIStyleAttr::SKIN_skin,OnAttributeSkin)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIStyleAttr::SKIN_ncskin,OnAttributeNcSkin)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIStyleAttr::RECT_ncmargin,OnAttributeNcMargin)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIStyleAttr::STRING_cursor,OnAttributeCursor)

			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrbg, m_crBg[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrbghover, m_crBg[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrbgpush, m_crBg[2], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrbgdisable, m_crBg[3], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrbgnoactive, m_crBg[4], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrnc, m_crNc[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrnchover, m_crNc[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrncpush, m_crNc[2], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrncdisable, m_crNc[3], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrncnoactive, m_crNc[04], DM_ECODE_NOXMLLOADREFRESH)

			DM_UINT_ATTRIBUTE(DMAttr::DUIStyleAttr::BYTE_alpha, m_byAlpha,DM_ECODE_NOXMLLOADREFRESH)

			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrtext, m_crText[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrtexthover, m_crText[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrtextpush, m_crText[2], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrtextdisable, m_crText[3], DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIStyleAttr::COLOR_clrtextnoactive, m_crText[4], DM_ECODE_NOXMLLOADREFRESH)

			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIStyleAttr::FONT_font, m_ftText[0], DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIStyleAttr::FONT_fonthover, m_ftText[1], DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIStyleAttr::FONT_fontpush, m_ftText[2], DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIStyleAttr::FONT_fontdisable, m_ftText[3], DM_ECODE_NOXMLLOADREFRESH)
			DM_FONTPTR_ATTRIBUTE(DMAttr::DUIStyleAttr::FONT_fontnoactive, m_ftText[3], DM_ECODE_NOXMLLOADREFRESH)

			//--------------------------
			DM_UINT_ATTRIBUTE(DMAttr::DUIStyleAttr::INT_textmode, m_nTextAlign, DM_ECODE_NOXMLLOADREFRESH)
			DM_ENUM_BEGIN(DMAttr::DUIStyleAttr::OPTION_align, UINT, DM_ECODE_NOXMLLOADREFRESH)
				DM_ENUM_VALUE(L"left", Align_Left)
				DM_ENUM_VALUE(L"center", Align_Center)
				DM_ENUM_VALUE(L"right", Align_Right)
			DM_ENUM_END(m_uAlign)

			DM_ENUM_BEGIN(DMAttr::DUIStyleAttr::OPTION_valign, UINT, DM_ECODE_NOXMLLOADREFRESH)
				DM_ENUM_VALUE(L"top", VAlign_Top)
				DM_ENUM_VALUE(L"middle", VAlign_Middle)
				DM_ENUM_VALUE(L"bottom", VAlign_Bottom)
			DM_ENUM_END(m_uVAlign)
			DM_bool_ATTRIBUTE(DMAttr::DUIStyleAttr::bool_bdot,m_bDotted,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_BREAK()

	public:
		DMCode OnAttributeSkin(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeNcSkin(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeNcMargin(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeCursor(LPCWSTR lpszValue, bool bLoadXml);

	public:
		CStringW                                    m_strID;        ///< 唯一标识

		DMColor										m_crBg[5];      ///< 背景5种状态下的颜色
		DMColor                                     m_crNc[5];		///< 边框5种状态下的颜色 
		CRect										m_rcNcMargin;   ///< 非客户区的四边

		DMColor                                     m_crText[5];    ///< 文字5种状态下的颜色
		DMSmartPtrT<IDMFont>						m_ftText[5];    ///< 文字5种状态下的字体

		CStringW                                    m_strSkinName;  ///< SKIN NAME,用NAME的原因是，在style加载时，skin不一定加载了！
		CStringW									m_strNcSkinName;///< 非客户区SKIN NAME
		
	    CStringW                                    m_strCursor;    ///< 光标NAME
		HCURSOR                                     m_hCursor;	    ///< 当前光标值		
		bool                                        m_bBmpCursor;   ///< 当前光标是否由图片创建
		BYTE										m_byAlpha;      ///< 窗口透明度,只进行配置，支持依赖于控件

		//-----------------------------
		// 文本对齐的几种方式
		UINT                                        m_nTextAlign;   ///< 文本对齐
		UINT									    m_uAlign;		///< 水平对齐
		UINT										m_uVAlign;		///< 垂直对齐
		bool                                        m_bDotted;      ///< 支持省略号显示文本	
	};
}//namespace DM