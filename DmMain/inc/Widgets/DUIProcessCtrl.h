//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIProcessCtrl.h 
// File Des: ���õ�ѡ��ťʵ��
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-18	1.0			
//--------------------------------------------------------
#pragma once
#include "Widgets/DMWidgetDef.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIProcessCtrl"/>��xml���Զ���
	/// </summary>
	class DUIProcessCtrlAttr:public DUIWindowAttr
	{
	public:
		static const char* SIZE_range;									   ///< ��Χ��Сֵ,���ֵ,ʾ��:range="0,100"
		static const char* INT_value;									   ///< ��Χ��ǰֵ,ʾ��:value="0"
		static const char* bool_bvert;                                    ///< �Ƿ���ˮƽ��,ʾ��:bvert="1"
		static const char* bool_bshowtip;                                 ///< �Ƿ���ʾtip

		static const char* SKIN_channelskin;                              ///< channelͼ,ʾ��:channelskin="PNG:channel"
		static const char* COLOR_clrchannel;							   ///< channel��ɫ,��channelͼ��Чʱʹ��,ʾ��:clrchannel="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_selskin;                                  ///< ѡ�в���ͼ,ʾ��:selskin="PNG:iconbtn"
		static const char* COLOR_clrsel;                                  ///< ѡ����ɫ,��ѡ��ͼ��Чʱʹ��,ʾ��:clrsel="pbgra(ff,ff,ff,ff)"
		static const char* STRING_textformat;                             ///< ��Ҫ��ʾ�Ľ�������format,ʾ��:textformat="����:%d"
	};
	DMAttrValueInit(DUIProcessCtrlAttr,SIZE_range)DMAttrValueInit(DUIProcessCtrlAttr,INT_value)
	DMAttrValueInit(DUIProcessCtrlAttr,bool_bvert)DMAttrValueInit(DUIProcessCtrlAttr,SKIN_channelskin)DMAttrValueInit(DUIProcessCtrlAttr,COLOR_clrchannel)
	DMAttrValueInit(DUIProcessCtrlAttr,SKIN_selskin)DMAttrValueInit(DUIProcessCtrlAttr,COLOR_clrsel)DMAttrValueInit(DUIProcessCtrlAttr,bool_bshowtip)DMAttrValueInit(DUIProcessCtrlAttr,STRING_textformat)
}

namespace DM
{
	/// <summary>
	///		 DUIProcessCtrl������ʵ�֣����ԣ�<see cref="DMAttr::DUIProcessCtrlAttr"/>
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
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);

	public:
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);

	public:// ����
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
			DM_WSTRING_ATTRIBUTE(DMAttr::DUIProcessCtrlAttr::STRING_textformat,m_strTextFormat,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
		//------------------------------------------
		// ��,��ˮƽ״̬�£�����ʹchannel���ڿͻ���
	public:
		DMCode OnAttributeRange(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeValue(LPCSTR lpszValue, bool bLoadXml);
	public:
		CSize                            m_szRange;              ///< xΪ��Сֵ,yΪ���ֵ
		int                              m_iValue;               ///< ��ǰֵ

		int                              m_iChannelHei;			 ///< channel�߶�

		bool                             m_bVert;                ///< �Ƿ�ˮƽ
		bool                             m_bShowTip;             ///< �Ƿ���ʾ������ʾ
		IDMSkinPtr                       m_pChannelSkin;
		IDMSkinPtr                       m_pSelSkin;
		DMColor							 m_crChannel;
		DMColor                          m_crSel;
		CStringW                         m_strTextFormat;        ///< ��Ҫ��ʾ�Ľ�������format
	};

}//namespace DM