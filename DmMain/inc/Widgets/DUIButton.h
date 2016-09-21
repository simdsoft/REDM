//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIButton.h 
// File Des: 内置按钮实现
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-9	1.0			
//--------------------------------------------------------
#pragma once
#include "DMWidgetDef.h"


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIButton"/>的xml属性定义
	/// </summary>
	class DUIButtonAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* bool_banimate;								    ///< 是否启用渐显示动画,示例:banimate="1"
		static wchar_t* INT_animatesteps;                               ///< 在启用渐显示动画时,设置分多少次显示(每次间隔10ms）,示例:animatesteps="1"
		static wchar_t* bool_bmultilines;								///< 是否支持多行,示例:bmultilines="1"
		static wchar_t* INT_lineinter;                                  ///< 在多行下,设置行间距,示例:lineinter="1"
		static wchar_t* SKIN_iconskin;                                  ///< icon图,如使用了icon图,则text不再绘制,示例:iconskin="PNG:iconbtn"
		static wchar_t* ACCEL_accel;                                    ///< 加速键,btn使用加速键触发点击消息,示例:accel="ctrl+enter"
	};
	DMAttrValueInit(DUIButtonAttr,bool_banimate)DMAttrValueInit(DUIButtonAttr,INT_animatesteps)
	DMAttrValueInit(DUIButtonAttr,bool_bmultilines)DMAttrValueInit(DUIButtonAttr,INT_lineinter)
	DMAttrValueInit(DUIButtonAttr,SKIN_iconskin)DMAttrValueInit(DUIButtonAttr,ACCEL_accel)
}

namespace DM
{
	/// <summary>
	///		 DUIButton的内置实现，属性：<see cref="DMAttr::DUIButtonAttr"/>
	/// </summary>
	class DM_EXPORT DUIButton:public DUIWindow, public IDMTimeline,public IDMAccelHandler
	{
		DMDECLARE_CLASS_NAME(DUIButton,DUINAME_Button,DMREG_Window);
	public:
		DUIButton();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			DM_MSG_WM_NCPAINT(DM_OnNcPaint)
			DM_MSG_WM_ERASEBKGND(DM_OnEraseBkgnd)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDown) //将双击消息处理为单击
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		BOOL DM_OnEraseBkgnd(IDMCanvas* pCanvas);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void DM_OnNcPaint(IDMCanvas* pCanvas);
		void OnDestroy();

	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);
		DMCode DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState);
		DMCode OnTimeline();
		bool DV_IsStateChangeRedraw();   // 当窗口状态变化时，是否重绘，背景图如有多种状态，应该刷新
		bool OnAccelPressed(const DUIAccel& Accel);
	    UINT DV_OnGetDlgCode(){return DMDLGC_WANTCHARS;}

		//---------------------------------------------------
		// Function Des: 换肤
		//---------------------------------------------------
		DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);

	public:// 辅助
		void StopCurAnimate();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUIButtonAttr::bool_banimate, m_bAnimate, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIButtonAttr::bool_bmultilines, m_bMultiLines, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIButtonAttr::INT_animatesteps,  m_iAnimateSteps,  DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIButtonAttr::INT_lineinter,     m_nLineInter,  DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIButtonAttr::SKIN_iconskin, m_pIconSkin,   DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIButtonAttr::ACCEL_accel, OnAttributeAccel)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeAccel(LPCWSTR pszValue, bool bLoadXml);

	public:
		bool							    m_bAnimate;				///< 启用alpha渐变动画
		int									m_iAnimateSteps;		///< 设置分多少次显示
		int                                 m_iAnimateDelta;		///< 辅助,由m_iAnimateSteps和当前alpha值(起始为1/5当前alpha)算出的每次增量
		byte                                m_byCurAlpha;           ///< 辅助,当前alpha，以0-255计算
	    DWORD                               m_dwPreFrameTime;       ///< 辅助,上一帧显示的时间，单位ms
		bool                                m_bMultiLines;			
		int                                 m_nLineInter;
		IDMSkin*                            m_pIconSkin;
		DWORD								m_accel;
	};

}// namespace DM