//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUISliderCtrl.h 
// File Des: 内置单选按钮实现
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-18	1.0			
//--------------------------------------------------------
#pragma once
#include "DMWidgetDef.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUISliderCtrl"/>的xml属性定义
	/// </summary>
	class DUISliderCtrlAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* STRING_tipformat;                              ///< tip提示语的设置格式,示例:tipformat="this is%d"
		static wchar_t* SIZE_range;									   ///< 范围最小值,最大值,示例:range="0,100"
		static wchar_t* INT_value;									   ///< 范围当前值,示例:value="0"
		static wchar_t* INT_thumbwidth;								   ///< thumb宽度,示例:thumbwidth="10"
		static wchar_t* INT_channelhei;								   ///< channel高度,示例:channelhei="10"
		static wchar_t* bool_bvert;                                    ///< 是否是水平的,示例:bvert="1"
		static wchar_t* bool_bshowtip;                                 ///< 是否显示tip,示例:bshowtip="1"

		static wchar_t* SKIN_channelskin;                              ///< channel图,示例:channelskin="PNG:channel"
		static wchar_t* COLOR_clrchannel;                              ///< channel颜色,在channel图无效时使用,示例:clrchannel="pbgra(ff,ff,ff,ff)"
		static wchar_t* SKIN_thumbskin;                                ///< thumb图,示例:thumbskin="PNG:iconbtn"
		static wchar_t* COLOR_clrthumb;                                ///< thumb颜色,在thumb图无效时使用,示例:clrthumb="pbgra(ff,ff,ff,ff)"
	};
	DMAttrValueInit(DUISliderCtrlAttr,STRING_tipformat)DMAttrValueInit(DUISliderCtrlAttr,SIZE_range)DMAttrValueInit(DUISliderCtrlAttr,INT_value)
	DMAttrValueInit(DUISliderCtrlAttr,INT_thumbwidth)DMAttrValueInit(DUISliderCtrlAttr,INT_channelhei)DMAttrValueInit(DUISliderCtrlAttr,bool_bvert)
	DMAttrValueInit(DUISliderCtrlAttr,SKIN_channelskin)DMAttrValueInit(DUISliderCtrlAttr,COLOR_clrchannel)
	DMAttrValueInit(DUISliderCtrlAttr,SKIN_thumbskin)DMAttrValueInit(DUISliderCtrlAttr,COLOR_clrthumb)DMAttrValueInit(DUISliderCtrlAttr,bool_bshowtip)
}

namespace DM
{
	/// <summary>
	///		 DUISliderCtrl的内置实现，属性：<see cref="DMAttr::DUISliderCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUISliderCtrl:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUISliderCtrl,DUINAME_SliderCtrl,DMREG_Window);
		enum{DMSLIDER_OTHER,DMSLIDER_CHANNEL,DMSLIDER_THUMB};
	public:
		DUISliderCtrl();

	public:
		bool SetPos(int iValue);
		int GetPos();
		void GetRange(int& nMin,int& nMax) const;
		int GetRangeMax() const;
		int GetRangeMin() const;

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_KEYDOWN(OnKeyDown)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnMouseLeave();
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	public:
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);

	public:// 辅助
		CRect GetChannelRect();
		CRect GetThumbRect();
		int HitTest(CPoint &pt);
		int GetValue(CPoint &pt);
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTARROWS;};		

	public:  
		DM_BEGIN_ATTRIBUTES()
			DM_STRING_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::STRING_tipformat,m_strTipFormat,DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::SIZE_range,OnAttributeRange)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::INT_value,OnAttributeValue)
			DM_INT_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::INT_thumbwidth,m_iThumbWid,DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::INT_channelhei,m_iChannelHei,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::bool_bvert,m_bVert,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::bool_bshowtip,m_bShowTip,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::SKIN_channelskin,m_pChannelSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::SKIN_thumbskin,m_pThumbSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::COLOR_clrchannel,m_crChannel,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::COLOR_clrthumb,m_crThumb,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
		//------------------------------------------
		// 简化,在水平状态下，总是使thumb的高度和client高度相同，channel的宽度和wid相同
	public:
		DMCode OnAttributeRange(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeValue(LPCWSTR lpszValue, bool bLoadXml);
	public:
		CStringW                         m_strTipFormat;
		CSize                            m_szRange;              ///< x为最小值,y为最大值
		int                              m_iValue;               ///< 当前值

		int                              m_iThumbWid;            ///< thumb宽度
		int                              m_iChannelHei;			 ///< channel高度

		bool                             m_bVert;                ///< 是否水平
		bool                             m_bShowTip;             ///< 是否显示数字提示
		IDMSkinPtr                       m_pChannelSkin;
		IDMSkinPtr                       m_pThumbSkin;
		DMColor							 m_crChannel;
		DMColor                          m_crThumb;

		// 辅助
		bool                             m_bDrag;                ///< 拖动状态（鼠标按下时在滑块上）
		CPoint						   	 m_DragPt;			     ///< 拖动的位置（x表示水平，y表示竖直)
		int                              m_iDragValue;           ///< 拖动距离
		int                              m_iHitPre;              ///< 上次鼠标位置
	};

}//namespace DM