//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSkiaBitmapImpl.h 
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
	class DMSkiaRegionImpl:public IDMRegion
	{
	public:
		DMSkiaRegionImpl();
		~DMSkiaRegionImpl();
	public:
		virtual bool CombineRect(LPCRECT lpRect,int fnCombineMode );
		virtual bool PtInRegion(const POINT &pt);
		virtual bool RectInRegion(LPCRECT lpRect);
		virtual void GetRgnBox(LPRECT lpRect);
		virtual bool IsEmpty();
		virtual void Offset(const POINT &pt);
		virtual void Clear();
		virtual DMCode GetRectNum(int &iNum){return DM_ECODE_FAIL;};
		virtual DMCode GetRectArray(RECT* pRect,int iNum){return DM_ECODE_FAIL;};;

	public:// 辅助
		SkRegion GetRegion() const;
		void SetRegion(const SkRegion& skRgn);

		// SkIRect和SkRect只是对Rect的一个封装，一个是整型，一个是float,资料看这里：http://hgy413.com/1838.html
		bool Rect2SkIRect(LPCRECT lpRect,SkIRect &skiRc);
		static SkRegion::Op GdiRgnOp2SkRgnOp(int fnCombineMode);// 

	protected:
		// SkRegion的资料看这里：http://hgy413.com/1834.html或http://hgy413.com/1843.html
		SkRegion                      m_SkRegion;
	};


}//namespace DM