//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUISplitter.h 
// File Des:DUISplitter.h 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-30	1.0			
//-------------------------------------------------------
#pragma once
#include "DUIFlowLayout.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUISplitter"/>的xml属性定义
	/// </summary>
	class DUISplitterAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* bool_bvert;                                    ///< 是否是竖直的,示例:bvert="1"
		static wchar_t* bool_bfirstchange;                             ///< 当整个外部窗口size变化时，第一个窗口变化,示例:bfirstchange="1"                          
		static wchar_t* SKIN_sliderskin;                               ///< 中间可拖动块的皮肤,示例:sliderskin="sliderskin"
		static wchar_t* COLOR_clrslider;                               ///< 中间可拖动块的背景色,示例:clrslider="pbgra(ff,ff,ff,ff)"
		static wchar_t* INT_sliderwidth;                               ///< 中间可拖动块的宽度(水平)或高度(竖直)，示例:sliderwidth="10"
		static wchar_t* INT_firstchildwidth;                           ///< 第一个窗口的开始宽度,示例:firstchildwidth="100"
		
	};
	DMAttrValueInit(DUISplitterAttr,bool_bvert)DMAttrValueInit(DUISplitterAttr,bool_bfirstchange)DMAttrValueInit(DUISplitterAttr,INT_firstchildwidth)
	DMAttrValueInit(DUISplitterAttr,SKIN_sliderskin)DMAttrValueInit(DUISplitterAttr,COLOR_clrslider)DMAttrValueInit(DUISplitterAttr,INT_sliderwidth)
}

namespace DM
{
	/// <summary>
	///		 DUISplitter的内置实现，属性：<see cref="DMAttr::DUISplitterAttr"/>
	/// 简化:仅两个窗口（A,B）构成,当整个外部窗口size变化时，要么A窗口变化，要么B窗口变化，2选一
	/// </summary>
	class DUISplitter:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUISplitter, L"splitter",DMREG_FlowLayout)
	public:
		DUISplitter();
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
		DM_END_MSG_MAP()
	public:
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);

	public:
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		DMCode DV_UpdateChildLayout();		
		DMCode DV_OnSetCursor(const CPoint &pt);

	public:// 辅助      
		DMCode UpdateFixWidth();
		DMCode UpdateDrag(int iDiff);
		CRect GetSliderRect();
	public: 
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUISplitterAttr::bool_bvert,m_bVert,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUISplitterAttr::bool_bfirstchange,m_bFirstChange,DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUISplitterAttr::INT_sliderwidth,m_iSliderWid,DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUISplitterAttr::INT_firstchildwidth,m_iFirstChildWidth,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUISplitterAttr::SKIN_sliderskin,m_pSliderSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUISplitterAttr::COLOR_clrslider,m_ClrSlider,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()

	public:
		bool                             m_bVert;                ///< 是否竖直
		int                              m_iSliderWid;           ///< 中间可拖动块的宽度(水平)或高度(竖直)
		DMSmartPtrT<IDMSkin>             m_pSliderSkin;
		DMColor                          m_ClrSlider;
		bool                             m_bFirstChange;
		int                              m_iFirstChildWidth;     ///< 第一个窗口的初始值
		int                              m_iFixWid;              ///< 固定的宽度，为0表示未初始化,其余表示size变化时，固定size的那个窗口
		bool                             m_bDrag;
		CPoint                           m_ptDrag;
		CStringW                         m_strPos;
	};


}//namespace DM