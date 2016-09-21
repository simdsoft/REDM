//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSkiaRegionImpl.h 
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
	class DMSkiaBrushImpl:public IDMBrush
	{
	public:
		DMSkiaBrushImpl(DMColor clr);
		DMSkiaBrushImpl(SkBitmap SkBmp);
		~DMSkiaBrushImpl();

		bool IsBitmap();
		SkBitmap GetBitmap();
		DMColor GetColor();

	protected:
		bool                           m_bBmp;	
		DMColor						   m_Clr;
		SkBitmap					   m_SkBmp;			// Î»Í¼»­Ë¢
	};

}//namespace DM