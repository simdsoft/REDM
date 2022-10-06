//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIButton.h 
// File Des: ���ð�ťʵ��
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-9	1.0			
//--------------------------------------------------------
#pragma once
#include "Widgets/DMWidgetDef.h"


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIButton"/>��xml���Զ���
	/// </summary>
	class DUIButtonAttr:public DUIWindowAttr
	{
	public:
		static const char* bool_banimate;								    ///< �Ƿ����ý���ʾ����,ʾ��:banimate="1"
		static const char* INT_animatesteps;                               ///< �����ý���ʾ����ʱ,���÷ֶ��ٴ���ʾ(ÿ�μ��10ms��,ʾ��:animatesteps="1"
		static const char* bool_bmultilines;								///< �Ƿ�֧�ֶ���,ʾ��:bmultilines="1"
		static const char* INT_lineinter;                                  ///< �ڶ�����,�����м��,ʾ��:lineinter="1"
		static const char* SKIN_iconskin;                                  ///< iconͼ,��ʹ����iconͼ,��text���ٻ���,ʾ��:iconskin="PNG:iconbtn"
		static const char* ACCEL_accel;                                    ///< ���ټ�,btnʹ�ü��ټ����������Ϣ,ʾ��:accel="ctrl+enter"
	};
	DMAttrValueInit(DUIButtonAttr,bool_banimate)DMAttrValueInit(DUIButtonAttr,INT_animatesteps)
	DMAttrValueInit(DUIButtonAttr,bool_bmultilines)DMAttrValueInit(DUIButtonAttr,INT_lineinter)
	DMAttrValueInit(DUIButtonAttr,SKIN_iconskin)DMAttrValueInit(DUIButtonAttr,ACCEL_accel)
}

namespace DM
{
	/// <summary>
	///		 DUIButton������ʵ�֣����ԣ�<see cref="DMAttr::DUIButtonAttr"/>
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
			MSG_WM_LBUTTONDBLCLK(OnLButtonDown) //��˫����Ϣ����Ϊ����
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		BOOL DM_OnEraseBkgnd(IDMCanvas* pCanvas);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void DM_OnNcPaint(IDMCanvas* pCanvas);
		void OnDestroy();

	public:
		//---------------------------------------------------
		// Function Des: ����
		//---------------------------------------------------
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);
		DMCode DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState);
		DMCode OnTimeline();
		bool DV_IsStateChangeRedraw();   // ������״̬�仯ʱ���Ƿ��ػ棬����ͼ���ж���״̬��Ӧ��ˢ��
		bool OnAccelPressed(const DUIAccel& Accel);
	    UINT DV_OnGetDlgCode(){return DMDLGC_WANTCHARS;}

		//---------------------------------------------------
		// Function Des: ����
		//---------------------------------------------------
		DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);

	public:// ����
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
		DMCode OnAttributeAccel(LPCSTR pszValue, bool bLoadXml);

	public:
		bool							    m_bAnimate;				///< ����alpha���䶯��
		int									m_iAnimateSteps;		///< ���÷ֶ��ٴ���ʾ
		int                                 m_iAnimateDelta;		///< ����,��m_iAnimateSteps�͵�ǰalphaֵ(��ʼΪ1/5��ǰalpha)�����ÿ������
		byte                                m_byCurAlpha;           ///< ����,��ǰalpha����0-255����
	    DWORD                               m_dwPreFrameTime;       ///< ����,��һ֡��ʾ��ʱ�䣬��λms
		bool                                m_bMultiLines;			
		int                                 m_nLineInter;
		IDMSkin*                            m_pIconSkin;
		DWORD								m_accel;
	};

}// namespace DM