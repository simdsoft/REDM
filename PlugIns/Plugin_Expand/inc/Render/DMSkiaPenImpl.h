//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSkiaFontImpl.h 
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
	class DMSkiaPenImpl:public IDMPen
	{
	public:
		DMSkiaPenImpl(DMColor clr,int iStyle=PS_SOLID, int iWidth=1);
		~DMSkiaPenImpl();
	public:
		//---------------------------------------------------
		// Function Des: ÏµÁÐº¯Êý
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		virtual void SetWidth(int iWidth);
		virtual void SetColor(DMColor clr);
		virtual void SetStyle(int iStyle);
		virtual int GetWidth();
		virtual DMColor GetColor();
		virtual int GetStyle();

	protected:
		int							m_iStyle;
		int							m_iWidth;
		DMColor						m_Clr;
	};
}