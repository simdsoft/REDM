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
		DMSkiaBrushImpl(IDMBitmap* pBmp);
		~DMSkiaBrushImpl();

		bool IsBitmap();
		SkBitmap GetBitmap();
		HBRUSH	GetBrush();
		DMColor GetColor();

	protected:
		bool                           m_bBmp;	
		DMColor						   m_Clr;
		SkBitmap					   m_SkBmp;			// Î»Í¼»­Ë¢
		HBRUSH                         m_hBrush;
	};

}//namespace DM