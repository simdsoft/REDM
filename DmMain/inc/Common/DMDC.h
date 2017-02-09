// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMDC.h
// File mark:   
// File summary:·â×°È«ÆÁDC
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-9-7
// ----------------------------------------------------------------
#pragma once
#include "DMRefNum.h"

namespace DM
{
	class DM_EXPORT DMAutoDC
	{
	public:
		DMAutoDC(HWND hWnd = NULL);
		~DMAutoDC();
		HDC GetDC(HWND hWnd);
		bool ReleaseDC();
		operator const HDC()const;
	private:
		HDC                m_hDC;
		HWND               m_hWnd;
	};

	class DM_EXPORT DMAutoMemDC
	{
	public:
		DMAutoMemDC(HDC hdc = NULL);
		DMAutoMemDC(DMAutoMemDC &hdc);
		~DMAutoMemDC();
		operator const HDC()const
		{
			return m_hMemDC;
		};

		bool SelectObject(HPEN hPen);
		bool SelectObject(HFONT hFont);
		bool SelectObject(HBRUSH hBrush);
		bool SelectObject(HBITMAP hBitmap);

		bool DeleteDC();
		
	private:
		HDC                m_hDC;
		HDC			       m_hMemDC;

		HPEN               m_hOldPen;
		HFONT              m_hOldFont;
		HBRUSH             m_hOldBrush;
		HBITMAP            m_hOldBitmap;
	};

}//namespace DM