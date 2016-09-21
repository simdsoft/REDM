//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIProcessCtrl.h 
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
	///		<see cref="DM::DUIProcessCtrl"/>的xml属性定义
	/// </summary>
	class DUIProcessCtrlAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* SIZE_range;									   ///< 范围最小值,最大值,示例:range="0,100"
		static wchar_t* INT_value;									   ///< 范围当前值,示例:value="0"
		static wchar_t* bool_bvert;                                    ///< 是否是水平的,示例:bvert="1"
		static wchar_t* bool_bshowtip;                                 ///< 是否显示tip

		static wchar_t* SKIN_channelskin;                              ///< channel图,示例:channelskin="PNG:channel"
		static wchar_t* COLOR_clrchannel;							   ///< channel颜色,在channel图无效时使用,示例:clrchannel="pbgra(ff,ff,ff,ff)"
		static wchar_t* SKIN_selskin;                                  ///< 选中部分图,示例:selskin="PNG:iconbtn"
		static wchar_t* COLOR_clrsel;                                  ///< 选中颜色,在选中图无效时使用,示例:clrsel="pbgra(ff,ff,ff,ff)"
		static wchar_t* STRING_textformat;                             ///< 需要显示的进度文字format,示例:textformat="进度:%d"
	};
	DMAttrValueInit(DUIProcessCtrlAttr,SIZE_range)DMAttrValueInit(DUIProcessCtrlAttr,INT_value)
	DMAttrValueInit(DUIProcessCtrlAttr,bool_bvert)DMAttrValueInit(DUIProcessCtrlAttr,SKIN_channelskin)DMAttrValueInit(DUIProcessCtrlAttr,COLOR_clrchannel)
	DMAttrValueInit(DUIProcessCtrlAttr,SKIN_selskin)DMAttrValueInit(DUIProcessCtrlAttr,COLOR_clrsel)DMAttrValueInit(DUIProcessCtrlAttr,bool_bshowtip)DMAttrValueInit(DUIProcessCtrlAttr,STRING_textformat)
}

namespace DM
{
	/// <summary>
	///		 DUIProcessCtrl的内置实现，属性：<see cref="DMAttr::DUIProcessCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUIProcessCtrl:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIProcessCtrl,DUINAME_ProcessCtrl,DMREG_Window);
		enum{DMSLIDER_OTHER,DMSLIDER_CHANNEL,DMSLIDER_THUMB};
	public:
		DUIProcessCtrl();

	public:
		bool SetPos(int iValue);
		int GetPos();
		void GetRange(int& nMin,int& nMax) const;
		int GetRangeMax() const;
		int GetRangeMin() const;

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);

	public:
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);

	public:// 辅助
		CRect GetSelRect();
		int GetValue(CPoint &pt);
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTARROWS;};		

	public:  
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::SIZE_range,OnAttributeRange)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::INT_value,OnAttributeValue)
			DM_bool_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::bool_bvert,m_bVert,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::bool_bshowtip,m_bShowTip,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::SKIN_channelskin,m_pChannelSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::COLOR_clrchannel,m_crChannel,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::SKIN_selskin,m_pSelSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::COLOR_clrsel,m_crSel,DM_ECODE_NOXMLLOADREFRESH)
			DM_STRING_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::STRING_textformat,m_strTextFormat,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
		//------------------------------------------
		// 简化,在水平状态下，总是使channel等于客户区
	public:
		DMCode OnAttributeRange(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeValue(LPCWSTR lpszValue, bool bLoadXml);
	public:
		CSize                            m_szRange;              ///< x为最小值,y为最大值
		int                              m_iValue;               ///< 当前值

		int                              m_iChannelHei;			 ///< channel高度

		bool                             m_bVert;                ///< 是否水平
		bool                             m_bShowTip;             ///< 是否显示数字提示
		IDMSkinPtr                       m_pChannelSkin;
		IDMSkinPtr                       m_pSelSkin;
		DMColor							 m_crChannel;
		DMColor                          m_crSel;
		CStringW                         m_strTextFormat;        ///< 需要显示的进度文字format
	};

}//namespace DM