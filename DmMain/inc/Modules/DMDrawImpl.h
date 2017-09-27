// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DMDrawImpl.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-23
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	typedef enum _DRAWSTATE
	{
		DRAW_START=0,    //开始
		DRAW_DRAWING,    //绘制中
		DRAW_END         //结吉
	}DMDRAW_STATE;

	/// <summary>
	///		绘制逻辑内置实现
	/// </summary>
	class DMDrawImpl:public IDMDraw
	{
		DMDECLARE_CLASS_NAME(DMDrawImpl,L"DMDrawImpl",DMREG_Draw);
	public:
		DMDrawImpl();

	public:

		DMCode DrawBackground(IDMCanvas* pCanvas,IDMRegion* lpRgn,DUIWindow* pWnd);
		DMCode DrawForeground(IDMCanvas* pCanvas,IDMRegion* lpRgn,DUIWindow* pWnd);
		DMCode Draw(IDMCanvas* pCanvas, IDMRegion* lpRgn,DUIWindow* pWnd);

		DMCode InvalidateRect(DUIWND hDUIWnd,LPCRECT lpRect,int fnCombineMode=RGN_AND);
		int    GetInvalidateRect(DM::CArray<CRect>& Array);
		bool   IsInvalidate(){return m_bNeedRepaint;};
		DMCode Render(DUIWindow* pWnd);

		DMCode GetCanvas(IDMCanvas** ppCanvas);
		DMCode ResizeCanvas(SIZE size);

	private:// 辅助
		DMCode _OnRender(IDMCanvas* pCanvas, IDMRegion* pRgn,DUIWindow* pWndCur,DUIWindow* pStart,DUIWindow *pEnd, DMDRAW_STATE& Draw_State);

	public:

		DMSmartPtrT<IDMRender>                  m_pRender;
		DMSmartPtrT<IDMRegion>					m_pInvalidRegion; 
		DMSmartPtrT<IDMCanvas>					m_pMemCanvas;

		bool								    m_bNeedRepaint;
		bool                                    m_bOnlyOneRectRepaint;			///< 仅只有一个无效矩形区
		CRect                                   m_rcOnlyOne;                   
		CRect                                   m_rcCanvas;
	};

}//namespace DM