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
	class DMBitmapImpl:public IDMBitmap
	{
	public:
		DMBitmapImpl();
		~DMBitmapImpl();

	public://À©Õ¹½Ó¿Ú
		DMCode Init(int nWid,int nHei,const LPVOID pBits=NULL);
		DMCode InitFromFrame(IDMImgFrame *pImgFrame);
		DMCode LoadFromFile(LPCWSTR pszFileName,LPCWSTR pszType);
		DMCode LoadFromMemory(LPBYTE pBuf,size_t szLen,LPCWSTR pszType);

	    DMCode AdjustHSL32(int H, int S, int L,LPRECT lpRect=NULL);
		DMCode ResetHsl();
		DMCode SaveFile(LPCWSTR pszFileName);

		DMCode AdjustAlpha(byte byAlpha, LPRECT lpRect);
		DMCode AlphaBackup(LPRECT lpRect);
		DMCode AlphaRestore();

		int GetWidth();													
		int GetHeight();								
		int GetSize();												
		int GetBPS();														
		DMColor GetPixel(int x, int y);										
		bool SetPixel(int x, int y, DMColor clr);							
		PVOID GetPixelBits(); 
		HBITMAP GetBitmap();

	public:
		DMDIBHelper               m_DibHelper;
	};


}//namespace DM