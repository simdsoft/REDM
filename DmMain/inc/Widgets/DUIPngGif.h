// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIPngGif.h
// File mark:   
// File summary:��������chenghui�ṩ��ά��
// Author:		elcondorpasa <834870885@qq.com>
// Edition:     1.0
// Create date: 2018-1-24
// ----------------------------------------------------------------
#pragma once
#include "Widgets/DUIGif.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIPngGif"/>��xml���Զ���
	/// </summary>
	class DUIPngGifAttr:public DUIGifAttr
	{
	public:
		static const char* SKIN_pnggifskin;                            ///< pnggif����Դ,ͨ��skinָ��,������gifskin,ʾ��:pnggifskin="png:4_png"                     
		static const char* INT_elapse;                                 ///< �ֲ���ʱ��������λ1/100�룩,Ĭ��Ϊ10,ʾ��:elapse="1"
	};
	DMAttrValueInit(DUIPngGifAttr,SKIN_pnggifskin)DMAttrValueInit(DUIPngGifAttr,INT_elapse)
}

namespace DM
{
	/// <summary>
	///		 pnggif������ʵ��
	/// </summary>
	class DUIPngGif:public DUIGif
	{
		DMDECLARE_CLASS_NAME(DUIPngGif, "pnggif",DMREG_Window)
	public:
		DUIPngGif();
		~DUIPngGif();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIPngGifAttr::INT_elapse, OnAttrElapse)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIPngGifAttr::SKIN_pnggifskin, OnAttrPngGif)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttrPngGif(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttrElapse(LPCSTR lpszValue, bool bLoadXml);

	public:  
		UINT                                         m_ulElapse;              ///< ��֡���		
	};
}