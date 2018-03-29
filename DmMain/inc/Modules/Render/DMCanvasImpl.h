//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMRegionImpl.h 
// File Des: alpha混合后续全使用mmx混合,已完成测试demo,稳定后移入
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-23	1.0			
//-------------------------------------------------------
#pragma once
#include "DMPenImpl.h"
#include "DMFontImpl.h"
#include "DMBrushImpl.h"
#include "DMBitmapImpl.h"
#include "DMRegionImpl.h"

namespace DM
{
	class DMCanvasImpl:public IDMCanvas
	{
	public:

		DMCanvasImpl(IDMRender* pRender, int nWid, int nHei);
		~DMCanvasImpl();
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
		DMCode AlphaBackup(LPRECT lpRect);
		DMCode AlphaRestore();

	public:// 辅助
	
		DMAutoMemDC AlphaBlendBackup(DMAutoMemDC& dcMem,LPCRECT lpRect,bool bInherit=false,bool bCopy=false);
		bool AlphaBlendRestore(DMAutoMemDC& dcMem,BYTE alpha=0xFF);
		CRect GetRealClipRect(LPCRECT lpRect);                  // 真正绘制的区域
		BOOL GradientFillRect(HDC hdc, const RECT *rcFill, DMColor clrBegin,DMColor clrEnd,BOOL bVert);

	public:
		// 1
		DMAutoMemDC									 m_hdc;

		// 2被选入DC的当前图元
		DMColor									     m_CurTextColor;
		DMSmartPtrT<DMBitmapImpl>					 m_pCurBitmap;
		DMSmartPtrT<DMPenImpl>						 m_pCurPen;
		DMSmartPtrT<DMBrushImpl>					 m_pCurBrush;
		DMSmartPtrT<DMFontImpl>						 m_pCurFont;

		// 3
		POINT                                        m_ptOrg;

		// 类厂
		IDMRender									 *m_pRender;

		// 辅助混合使用------------------
		DMDIBHelper                                  m_DIBTemp;
		CRect                                        m_RcTemp;
		bool                                         m_bCopyTemp;
	};





}//namespace DM