// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	IDMDraw.h
// File mark:   
// File summary:指定绘制逻辑，每个DMHWnd可自定义一个自身的绘制逻辑
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2015-3-10
// ----------------------------------------------------------------
#pragma once

#pragma once
namespace DM
{
	/// <summary>
	///		绘制对外扩展接口,classtype=<see cref="DMREG_Draw"/>
	/// </summary>
	class IDMDraw:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMDraw,L"IDMDraw",DMREG_Draw);
	public:
		// DUI通用绘制
		virtual  DMCode DrawBackground(IDMCanvas* pCanvas,IDMRegion* lpRgn,DUIWindow* pWnd) = 0; 			///< 绘制背景
		virtual  DMCode DrawForeground(IDMCanvas* pCanvas,IDMRegion* lpRgn,DUIWindow* pWnd) = 0; 			///< 绘制前景
		virtual  DMCode Draw(IDMCanvas* pCanvas, IDMRegion* lpRgn,DUIWindow* pWnd) = 0; 					///< 绘制,绘制后一般无效区会被清空
		
		// DMHWnd无效区+自身绘制
		virtual  DMCode InvalidateRect(DUIWND hDUIWnd,LPCRECT lpRect,int fnCombineMode=RGN_AND) = 0;					    ///< 设置无效区
		virtual  int    GetInvalidateRect(DM::CArray<CRect>& Array) = 0;									///< 得到无效矩形列表,返回个数
		virtual  bool   IsInvalidate() = 0;																	///< 是否存在无效区
		virtual  DMCode Render(DUIWindow* pWnd) = 0;														///< 根据无效区列表进行绘制,pWnd一般为DMHWnd所在DUIWindow
		
		virtual  DMCode GetCanvas(IDMCanvas** ppCanvas) = 0;                                                ///< 取得绘制画布,内部会自动增加引用计数,请使用智能指针
		virtual  DMCode ResizeCanvas(SIZE size) = 0;														///< 重设绘制画布大小

	};

}//namespace DM