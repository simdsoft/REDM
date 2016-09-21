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
#include "DMSkiaPenImpl.h"
#include "DMSkiaFontImpl.h"
#include "DMSkiaBrushImpl.h"
#include "DMSkiaBitmapImpl.h"
#include "DMSkiaRegionImpl.h"
#include "DMSkiaHelper.h"

namespace DM
{;
	class DMSkiaCanvasImpl:public IDMCanvas
	{
	public:

		DMSkiaCanvasImpl(IDMRender* pRender, int nWid, int nHei);
		~DMSkiaCanvasImpl();
		void Canv_Init(IDMRender* pRender, int nWid, int nHei);
		void Canv_Release();

		//---------------------------------------------------
		// Function Des:基础
		//---------------------------------------------------
		DMCode SelectObject(IDMMetaFile* pObj,IDMMetaFile** ppOldObj = NULL);
		DMCode GetObject(IDMMetaFile** ppObj, DMFTYPE DmfType);

		DMCode Resize(SIZE size);
		DMCode Resize(int nWid,int nHei);
		DMCode GetSize(SIZE &size);

		HDC GetDC();
		DMCode ReleaseDC(HDC hdc);
		DMCode SaveCanvas(int *pnState=NULL);
		DMCode RestoreCanvas(int nState=-1);

		//---------------------------------------------------
		// Function Des:HSL变换
		//---------------------------------------------------
		DMCode AdjustHSL32(int H, int S, int L,LPRECT lpRect=NULL);
		DMCode ResetHsl();
		PVOID GetPixelBits(int* pSize);

		//---------------------------------------------------
		// Function Des:绘制
		//---------------------------------------------------
		DMCode BitBlt(IDMCanvas*pCanvasSrc, int xSrc, int ySrc, LPCRECT lpRectDest, DWORD dwRop = SRCCOPY);
		DMCode AlphaBlend(IDMCanvas*pCanvasSrc, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha=0xFF);

		DMCode DrawBitamp(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha=0xFF, DMEXPEND_MODE ExpandMode=DEM_STRETCH);
		DMCode DrawBitmapNine(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpSrcMargin, LPCRECT lpRectDest, BYTE alpha=0xFF, DMEXPEND_MODE ExpandMode=DEM_STRETCH);
		
		DMCode DrawRectangle(LPCRECT lpRect);
		DMCode FillRectangle(LPCRECT lpRect);
		DMCode FillSolidRect(LPCRECT lpRect,DMColor clr);
		DMCode ClearRect(LPCRECT lpRect,DMColor clr);

		DMCode DrawRoundRect(LPCRECT lpRect,POINT &pt);
		DMCode FillRoundRect(LPCRECT lpRect,POINT &pt);

		DMCode DrawEllipse(LPCRECT lpRect);
		DMCode FillEllipse(LPCRECT lpRect);
	
		DMCode Polyline(LPPOINT lpPt,int cPoints);

		DMCode GradientFill(DMColor clrBegin,DMColor clrEnd,LPCRECT lpRect,BOOL bVert,BYTE alpha=0xFF);

		//---------------------------------------------------
		// Function Des:文本
		//---------------------------------------------------
		DMColor SetTextColor(DMColor clr);
		DMColor GetTextColor();

		DMCode MeasureText(LPCWSTR lpString, int nCount, OUT SIZE *pSize);
		DMCode DrawText(LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha=0xFF);
		DMCode TextOut(LPCWSTR lpString, int nCount, int x, int  y, BYTE alpha=0xFF);

		//---------------------------------------------------
		// Function Des:坐标变换
		//---------------------------------------------------
		DMCode OffsetViewportOrg(int dx, int dy, OUT LPPOINT lpPoint = NULL);
		DMCode SetViewportOrgEx(int dx, int dy,  OUT LPPOINT lpPoint = NULL);
		DMCode GetViewportOrg(LPPOINT lpPoint);

		//---------------------------------------------------
		// Function Des:绘制区域裁剪
		//---------------------------------------------------
		DMCode PushClip(IDMRegion *pRegion,int fnCombineMode=RGN_AND);
		DMCode PushClip(LPCRECT lpRect,int fnCombineMode=RGN_AND);
		DMCode PopClip();
		DMCode GetClipBox(LPRECT lpRect);
		DMCode GetClipRegion(IDMRegion **ppRegion);

		DMCode ExcludeClipRect(LPCRECT lpRect);
		DMCode IntersectClipRect(LPCRECT lpRect);

		//---------------------------------------------------
		// Function Des:辅助调试使用
		//---------------------------------------------------
		DMCode SaveBmpToFile(LPCWSTR pszFileName);

		DMCode AdjustAlpha(byte byAlpha, LPRECT lpRect);
		DMCode AlphaBackup(LPRECT lpRect){return DM_ECODE_OK;};
		DMCode AlphaRestore(){return DM_ECODE_OK;};

	public:// 辅助
		bool Rect2SkRect(LPCRECT lpRect,SkRect &skiRc);
		void InflateSkRect(SkRect *lpRect,SkScalar dx,SkScalar dy);
	
	public:
		// 1
		SkCanvas									 *m_pSkCanvas;

		// 2被选入Canvas的当前图元
		DMColor									     m_CurTextColor;
		DMSmartPtrT<DMSkiaBitmapImpl>				 m_pCurBitmap;
		DMSmartPtrT<DMSkiaPenImpl>					 m_pCurPen;
		DMSmartPtrT<DMSkiaBrushImpl>				 m_pCurBrush;
		DMSmartPtrT<DMSkiaFontImpl>					 m_pCurFont;

		// 3
		SkPoint                                      m_ptOrg;
		HDC											 m_hGetDC;

		// 类厂
		IDMRender									 *m_pRender;

		// 辅助混合使用------------------
		int                                           m_iSaveState; //skia 的savestate和hdc不同，不是-1返回上一次~！
	};





}//namespace DM