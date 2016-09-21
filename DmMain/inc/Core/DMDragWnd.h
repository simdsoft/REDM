//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMDragWnd.h 
// File Des: 简单模拟拖动
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-13	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		拖动真实窗口
	/// </summary>
	class DM_EXPORT DMDragWnd : public DMCWnd
	{
	public:
		DMDragWnd(void);
		virtual~DMDragWnd(void);

		//---------------------------------------------------
		// Function Des: 接口
		//---------------------------------------------------
		static bool DragBegin(IDMCanvas* pCanvas,POINT ptHot,COLORREF crKey, BYTE byAlpha,DWORD dwFlags);
		static void DragMove(POINT pt);
		static void DragEnd();
	
	public:
		CPoint								m_ptHot;					///< 相对于拖动画布左上角的偏移
		static DMDragWnd*					ms_pCurDragWnd;
	};
}//namespace DM