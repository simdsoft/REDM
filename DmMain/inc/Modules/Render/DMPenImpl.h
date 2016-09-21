//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMFontImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-26	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class DMPenImpl:public IDMPen
	{
	public:
		DMPenImpl(DMColor clr,int iStyle=PS_SOLID, int iWidth=1);
		~DMPenImpl();
	public:
		//---------------------------------------------------
		// Function Des: ÏµÁÐº¯Êý
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		virtual void SetWidth(int iWidth);
		virtual void SetColor(DMColor clr);
		virtual int GetWidth();
		virtual DMColor GetColor();
		virtual int GetStyle();
		virtual HPEN GetPen();

	protected:
		int							m_iStyle;
		int							m_iWidth;
		DMColor						m_Clr;
		HPEN						m_hPen;
	};
}