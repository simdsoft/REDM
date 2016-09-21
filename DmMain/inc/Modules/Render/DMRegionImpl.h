//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMBitmapImpl.h 
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
	class DMRegionImpl:public IDMRegion
	{
	public:
		DMRegionImpl();
		~DMRegionImpl();
	public:
		virtual bool CombineRect(LPCRECT lpRect,int fnCombineMode );
		virtual bool PtInRegion(const POINT &pt);
		virtual bool RectInRegion(LPCRECT lpRect);
		virtual void GetRgnBox(LPRECT lpRect);
		virtual bool IsEmpty();
		virtual void Offset(const POINT &pt);
		virtual void Clear();
		virtual DMCode GetRectNum(int &iNum);
		virtual DMCode GetRectArray(RECT* pRect,int iNum);

	public:// ¸¨Öú
		HRGN GetRegion() const;
		void SetRegion(const HRGN rgn);

	protected:
		HRGN                    m_hRgn;

	};


}//namespace DM