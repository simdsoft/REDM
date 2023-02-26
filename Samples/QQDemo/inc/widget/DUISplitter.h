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
	///		<see cref="DM::DUISplitter"/>��xml���Զ���
	/// </summary>
	class DUISplitterAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* bool_bvert;                                    ///< �Ƿ�����ֱ��,ʾ��:bvert="1"
		static wchar_t* bool_bfirstchange;                             ///< �������ⲿ����size�仯ʱ����һ�����ڱ仯,ʾ��:bfirstchange="1"                          
		static wchar_t* SKIN_sliderskin;                               ///< �м���϶����Ƥ��,ʾ��:sliderskin="sliderskin"
		static wchar_t* COLOR_clrslider;                               ///< �м���϶���ı���ɫ,ʾ��:clrslider="pbgra(ff,ff,ff,ff)"
		static wchar_t* INT_sliderwidth;                               ///< �м���϶���Ŀ��(ˮƽ)��߶�(��ֱ)��ʾ��:sliderwidth="10"
		static wchar_t* INT_firstchildwidth;                           ///< ��һ�����ڵĿ�ʼ���,ʾ��:firstchildwidth="100"
		
	};
	DMAttrValueInit(DUISplitterAttr,bool_bvert)DMAttrValueInit(DUISplitterAttr,bool_bfirstchange)DMAttrValueInit(DUISplitterAttr,INT_firstchildwidth)
	DMAttrValueInit(DUISplitterAttr,SKIN_sliderskin)DMAttrValueInit(DUISplitterAttr,COLOR_clrslider)DMAttrValueInit(DUISplitterAttr,INT_sliderwidth)
}

namespace DM
{
	/// <summary>
	///		 DUISplitter������ʵ�֣����ԣ�<see cref="DMAttr::DUISplitterAttr"/>
	/// ��:���������ڣ�A,B������,�������ⲿ����size�仯ʱ��ҪôA���ڱ仯��ҪôB���ڱ仯��2ѡһ
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
		DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
		DMCode DV_UpdateChildLayout();		
		DMCode DV_OnSetCursor(const CPoint &pt);

	public:// ����      
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
		bool                             m_bVert;                ///< �Ƿ���ֱ
		int                              m_iSliderWid;           ///< �м���϶���Ŀ��(ˮƽ)��߶�(��ֱ)
		DMSmartPtrT<IDMSkin>             m_pSliderSkin;
		DMColor                          m_ClrSlider;
		bool                             m_bFirstChange;
		int                              m_iFirstChildWidth;     ///< ��һ�����ڵĳ�ʼֵ
		int                              m_iFixWid;              ///< �̶��Ŀ�ȣ�Ϊ0��ʾδ��ʼ��,�����ʾsize�仯ʱ���̶�size���Ǹ�����
		bool                             m_bDrag;
		CPoint                           m_ptDrag;
		CStringW                         m_strPos;
	};


}//namespace DM