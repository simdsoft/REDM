//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMRegionImpl.h 
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
	class DMBrushImpl:public IDMBrush
	{
	public:
		static DMBrushImpl *CreateSolidBrush(DMColor clr);
		static DMBrushImpl *CreateBitmapBrush(HBITMAP hBmp);
	public:
		DMBrushImpl(DMColor clr);
		DMBrushImpl(HBITMAP hBmp);
		~DMBrushImpl();

		bool    IsBitmap();
		HBRUSH	GetBrush();
		DMColor GetColor();

	protected:
		HBRUSH                         m_hBrush;
		bool                           m_bBmp;	
		DMColor					   m_Clr;
	};

}//namespace DM