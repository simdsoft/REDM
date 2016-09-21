//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMScrollBarSkinImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-28	1.0			
//-------------------------------------------------------
#pragma once
#include "DMImgListSkinImpl.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DMImgListSkinImpl"/>的xml属性定义
	/// </summary>
	class DMScrollBarSkinImplAttr:public DMImgListSkinImplAttr
	{
	public:
		static wchar_t* RECT_margin;                            ///< scroll为9宫格绘制,指定绘制的四边框,margin="1,1,1,1"

	};
	DMAttrValueInit(DMScrollBarSkinImplAttr,RECT_margin);
}

namespace DM
{
	/// <summary>
	///		scroll连图内置实现，属性：<see cref="DMAttr::DMScrollBarSkinImplAttr"/>
	/// </summary>
	class DMScrollBarSkinImpl:public DMImgListSkinImpl
	{
		DMDECLARE_CLASS_NAME(DMScrollBarSkinImpl,L"scrollbar",DMREG_Skin);
	public:
		DMCode Draw(IDMCanvas *pCanvas, LPCRECT lpRectDraw, int iState,BYTE alpha=0xFF); 

	public:
		//--------------------------------------------------
		// 控件:上箭头、下箭头、坚直滚动条、竖直滑槽、左箭头、右箭头、水平滚动条、水平滑槽、小边角
		// 状态:正常、停留、点击、禁用、非Active
		// 共列9*行5=45种状态 nSbCode+bVert选择列 nState为行
		//--------------------------------------------------
		CRect GetPartRect(int nSbCode, int nState,bool bVert);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_RECT_ATTRIBUTE(DMAttr::DMScrollBarSkinImplAttr::RECT_margin ,m_rcMargin,DM_ECODE_OK)
		DM_END_ATTRIBUTES()

	public:
		CRect								m_rcMargin;
		
	};

}//namespace DM