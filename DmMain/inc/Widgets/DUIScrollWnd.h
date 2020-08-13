//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: DUIScrollWnd只允许包含一个绝对布局子控件DUIScrollFL,DUIScrollFL的大小总是包含它的所有子窗口区域
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2019-6-19	1.0			
//--------------------------------------------------------
#pragma once
#include "Widgets/DMWidgetDef.h"
#include "Widgets/DUIScrollBase.h"

namespace DM
{
	/// <summary>
	///		 DUIScrollWnd的辅助,用于绝对布局,包含它的所有子窗口区域,当子窗口区域变化时，它会同步变化
	/// </summary>
	class DM_EXPORT DUIScrollFL : public DUIWindow
	{
	public:
		DMDECLARE_CLASS_NAME(DUIScrollFL, DUINAME_ScrollFL, DMREG_Window);

	public:
		CRect GetMeasureRect(bool bUseFLCoordinate = false);				   ///< bUseFLCoordinate为true时,表示以ScrollFL的左上角为坐标原点,否则以主窗口为坐标原点

	public:
		virtual DMCode DV_GetChildMeasureLayout(LPRECT lpRect);
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		DMCode OnChildLayoutFinishedEvent(DMEventArgs *pEvt);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void OnDestroy();

	public:
		CRect                                          m_rcMeasure;             ///< 以ScrollFL的左上角为坐标原点,包含所有子控件的最小区域(不包含ScrollFL非客户区)
	};


	/// <summary>
	///		 DUIScrollWnd的内置实现
	/// </summary>
	class DM_EXPORT DUIScrollWnd :public DUIScrollBase
	{
	public:
		DMDECLARE_CLASS_NAME(DUIScrollWnd, DUINAME_ScrollWnd, DMREG_Window);
	public:
		DUIScrollWnd();

	public:
		//---------------------------------------------------
		// Function Des: 对外接口
		//---------------------------------------------------
		virtual void UpdateScrollRange();									    ///< 更新滚动范围，如果子控件的大小改变了,需要主动调用此来触发

		//---------------------------------------------------
		// Function Des: DV methods
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual void OnRangeCurPosChanged(CPoint ptOld, CPoint ptNew);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_SIZE(OnSize)
		DM_END_MSG_MAP()

	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void OnSize(UINT nType, CSize size);

	public:
		DUIScrollFL*                                          m_pFLChild;
	};



}//namespace DM