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

}//namespace DM