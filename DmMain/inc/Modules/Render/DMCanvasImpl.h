//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMRegionImpl.h 
// File Des: alpha��Ϻ���ȫʹ��mmx���,����ɲ���demo,�ȶ�������
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-23	1.0			
//-------------------------------------------------------
#pragma once
#include "Modules/Render/DMPenImpl.h"
#include "Modules/Render/DMFontImpl.h"
#include "Modules/Render/DMBrushImpl.h"
#include "Modules/Render/DMBitmapImpl.h"
#include "Modules/Render/DMRegionImpl.h"

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
		// Function Des:����
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
		// Function Des:HSL�任
		//---------------------------------------------------
		DMCode AdjustHSL32(int H, int S, int L,LPRECT lpRect=NULL);
		DMCode ResetHsl();
		PVOID GetPixelBits(int* pSize);

		//---------------------------------------------------
		// Function Des:����
		//---------------------------------------------------
		DMCode BitBlt(IDMCanvas*pCanvasSrc, int xSrc, int ySrc, LPCRECT lpRectDest, DWORD dwRop = SRCCOPY);
		DMCode AlphaBlend(IDMCanvas*pCanvasSrc, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha=0xFF);

		DMCode DrawBitamp(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha=0xFF, DMEXPEND_MODE ExpandMode=DEM_STRETCH);
		DMCode DrawBitmapNine(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpSrcMargin, LPCRECT lpRectDest, BYTE alpha=0xFF, DMEXPEND_MODE ExpandMode=DEM_STRETCH);
		
		DMCode DrawRectangle(LPCRECT lpRect);
		DMCode FillRectangle(LPCRECT lpRect);
		DMCode FillSolidRect(LPCRECT lpRect,DMColor clr);
		DMCode ClearRect(LPCRECT lpRect,DMColor clr);

		DMCode DrawRoundRect(LPCRECT lpRect,const POINT &pt);
		DMCode FillRoundRect(LPCRECT lpRect,POINT &pt);

		DMCode DrawEllipse(LPCRECT lpRect);
		DMCode FillEllipse(LPCRECT lpRect);
	
		DMCode Polyline(LPPOINT lpPt,int cPoints);

		DMCode GradientFill(DMColor clrBegin,DMColor clrEnd,LPCRECT lpRect,BOOL bVert,BYTE alpha=0xFF);

		//---------------------------------------------------
		// Function Des:�ı�
		//---------------------------------------------------
		DMColor SetTextColor(DMColor clr);
		DMColor GetTextColor();

		DMCode MeasureText(LPCWSTR lpString, int nCount, OUT SIZE *pSize);
		DMCode DrawText(LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha=0xFF);
		DMCode TextOut(LPCWSTR lpString, int nCount, int x, int  y, BYTE alpha=0xFF);

		//---------------------------------------------------
		// Function Des:����任
		//---------------------------------------------------
		DMCode OffsetViewportOrg(int dx, int dy, OUT LPPOINT lpPoint = NULL);
		DMCode SetViewportOrgEx(int dx, int dy,  OUT LPPOINT lpPoint = NULL);
		DMCode GetViewportOrg(LPPOINT lpPoint);

		//---------------------------------------------------
		// Function Des:��������ü�
		//---------------------------------------------------
		DMCode PushClip(IDMRegion *pRegion,int fnCombineMode=RGN_AND);
		DMCode PushClip(LPCRECT lpRect,int fnCombineMode=RGN_AND);
		DMCode PopClip();
		DMCode GetClipBox(LPRECT lpRect);
		DMCode GetClipRegion(IDMRegion **ppRegion);

		DMCode ExcludeClipRect(LPCRECT lpRect);
		DMCode IntersectClipRect(LPCRECT lpRect);

		//---------------------------------------------------
		// Function Des:��������ʹ��
		//---------------------------------------------------
		DMCode SaveBmpToFile(LPCWSTR pszFileName);

		DMCode AdjustAlpha(byte byAlpha, LPRECT lpRect);
		DMCode AlphaBackup(LPRECT lpRect);
		DMCode AlphaRestore();

	public:// ����
		DMAutoMemDC AlphaBlendBackup(DMAutoMemDC& dcMem,LPCRECT lpRect,bool bInherit=false,bool bCopy=false);
		bool AlphaBlendRestore(DMAutoMemDC& dcMem,BYTE alpha=0xFF);
		CRect GetRealClipRect(LPCRECT lpRect);                  // �������Ƶ�����
		BOOL GradientFillRect(HDC hdc, const RECT *rcFill, DMColor clrBegin,DMColor clrEnd,BOOL bVert);

	public:
		//---------------------------------------------------
		// Function Des:���������������ӵ���չ����,�������,�Ա�֤���ϼ���
		//---------------------------------------------------
		DMCode DrawArc(LPCRECT lpRect,float startAngle, float sweepAngle);
		DMCode FillPie(LPCRECT lpRect,float startAngle, float sweepAngle);

	public:
		// 1
		DMAutoMemDC									 m_hdc;

		// 2��ѡ��DC�ĵ�ǰͼԪ
		DMColor									     m_CurTextColor;
		DMSmartPtrT<DMBitmapImpl>					 m_pCurBitmap;
		DMSmartPtrT<DMPenImpl>						 m_pCurPen;
		DMSmartPtrT<DMBrushImpl>					 m_pCurBrush;
		DMSmartPtrT<DMFontImpl>						 m_pCurFont;

		// 3
		POINT                                        m_ptOrg;

		// �೧
		IDMRender									 *m_pRender;

		// �������ʹ��------------------
		DMDIBHelper                                  m_DIBTemp;
		CRect                                        m_RcTemp;
		bool                                         m_bCopyTemp;
	};





}//namespace DM