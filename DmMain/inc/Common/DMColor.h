//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMColor.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
#define GetAValue(rgb)		 (LOBYTE((rgb)>>24))
#define RGBA(r,g,b,a)		 (RGB(r,g,b)|(a<<24))
#define	DMCLR_INVALID		  0x00FFFFFF				  ///< 用于屏蔽Alpha，不然GDI无法显示
#define BGR(b,g,r)           ((DWORD)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))
#define PBGRA(b,g,r,a)       ((DWORD)(BGR(b,g,r)|(a<<24)))///<请注意是PBGRA,请先预乘alpha，亲！
	/// <summary>
	///		图形解码器或其他DIB默认从低到高位是BGRA
	///     而RGBA(COLORREF)默认从低到高位是RGBA
	///     DM内部默认使用BGRA
	///     统一使用PBGRA
	/// </summary>
	class DM_EXPORT DMColor
	{
	public:
		DMColor();
		DMColor(BYTE r,BYTE g,BYTE b,BYTE a=0xFF);
		DMColor(DWORD clr);///<使用DWORD构造函数，方便直接使用PBGRA赋值

		const COLORREF ToCOLORREF() const;							 ///< 转换成COLORREF,并把alpha置为0
		const DWORD	   ToBGRA() const;		

		void SetRGBA(BYTE R,BYTE G,BYTE B,BYTE A);
		void SetbyBGRA(DWORD Clr);									 ///< clr为BGRA格式

		static DMColor PreMultiPly(DMColor clr,BYTE alpha);          ///< 全局预乘
		void PreMultiply(BYTE alpha);								 ///< 预乘
		bool IsDefault(){return 0==b&&0==g&&0==r&&0xff==a;}          ///< 是否默认色     
		bool IsTranslucent(){return 0==a;}                           ///< 是否透明
		bool IsTextInvalid(){return 0==a&&0xff==b&&0xff==g&&0xff==r;}///< 纯白透明度为0的认为是无效字体色
		void SetTextInvalid(){a=0;g=b=r=0xff;}						 ///< 设置无效字体色

		bool operator==(DMColor& obj) const throw()
		{
			return b == obj.b&&g == obj.g&&r == obj.r&&a == obj.a;
		}

		bool operator!=(DMColor& obj) const throw()
		{
			return b != obj.b|| g != obj.g||r != obj.r||a != obj.a;
		}

	public:
		DWORD						b:8;
		DWORD						g:8;
		DWORD						r:8;
		DWORD						a:8;
	};

}//namespace DM