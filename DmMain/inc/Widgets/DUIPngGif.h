// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIPngGif.h
// File mark:   
// File summary:本代码由chenghui提供及维护
// Author:		elcondorpasa <834870885@qq.com>
// Edition:     1.0
// Create date: 2018-1-24
// ----------------------------------------------------------------
#pragma once
#include "DUIGif.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIPngGif"/>的xml属性定义
	/// </summary>
	class DUIPngGifAttr:public DUIGifAttr
	{
	public:
		static wchar_t* SKIN_pnggifskin;                            ///< pnggif的资源,通过skin指定,区分于gifskin,示例:pnggifskin="png:4_png"                     
		static wchar_t* INT_elapse;                                 ///< 轮播的时间间隔（单位1/100秒）,默认为10,示例:elapse="1"
	};
	DMAttrValueInit(DUIPngGifAttr,SKIN_pnggifskin)DMAttrValueInit(DUIPngGifAttr,INT_elapse)
}

namespace DM
{
	/// <summary>
	///		 pnggif的内置实现
	/// </summary>
	class DUIPngGif:public DUIGif
	{
		DMDECLARE_CLASS_NAME(DUIPngGif, L"pnggif",DMREG_Window)
	public:
		DUIPngGif();
		~DUIPngGif();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIPngGifAttr::INT_elapse, OnAttrElapse)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIPngGifAttr::SKIN_pnggifskin, OnAttrPngGif)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttrPngGif(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttrElapse(LPCWSTR lpszValue, bool bLoadXml);

	public:  
		UINT                                         m_ulElapse;              ///< 换帧间隔		
	};
}