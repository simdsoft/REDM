//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSkiaRenderImpl.h 
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
	class DMSkiaRenderImpl:public IDMRender
	{
		DMDECLARE_CLASS_NAME(DMSkiaRenderImpl,L"DMSkiaRenderImpl",DMREG_Render);
	public:
		DMCode CreateBitmap(IDMBitmap** ppBitmap); 
		DMCode CreateFont(const LPLOGFONTW plf,IDMFont** ppFont); 
		DMCode CreateRegion(IDMRegion** ppRgn); 
		DMCode CreatePen(DMColor clr,int iStyle,int iWidth,IDMPen** ppPen); 
		DMCode CreateSolidColorBrush(DMColor clr,IDMBrush** ppBrush);  
		DMCode CreateBitmapBrush(IDMBitmap*pBmp,IDMBrush** ppBrush); 
		DMCode CreateCanvas(int nWid,int nHei,IDMCanvas** ppCanvas);
	};

}//namespace DM