//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUISliderCtrl.h 
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
	///		<see cref="DM::DUISliderCtrl"/>��xml���Զ���
	/// </summary>
	class DUISliderCtrlAttr:public DUIWindowAttr
	{
	public:
		static const char* STRING_tipformat;                              ///< tip��ʾ������ø�ʽ,ʾ��:tipformat="this is%d"
		static const char* SIZE_range;									   ///< ��Χ��Сֵ,���ֵ,ʾ��:range="0,100"
		static const char* INT_value;									   ///< ��Χ��ǰֵ,ʾ��:value="0"
		static const char* INT_thumbwidth;								   ///< thumb���,ʾ��:thumbwidth="10"
		static const char* INT_channelhei;								   ///< channel�߶�,ʾ��:channelhei="10"
		static const char* bool_bvert;                                    ///< �Ƿ���ˮƽ��,ʾ��:bvert="1"
		static const char* bool_bshowtip;                                 ///< �Ƿ���ʾtip,ʾ��:bshowtip="1"

		static const char* SKIN_channelskin;                              ///< channelͼ,ʾ��:channelskin="PNG:channel"
		static const char* COLOR_clrchannel;                              ///< channel��ɫ,��channelͼ��Чʱʹ��,ʾ��:clrchannel="pbgra(ff,ff,ff,ff)"
		static const char* SKIN_thumbskin;                                ///< thumbͼ,ʾ��:thumbskin="PNG:iconbtn"
		static const char* COLOR_clrthumb;                                ///< thumb��ɫ,��thumbͼ��Чʱʹ��,ʾ��:clrthumb="pbgra(ff,ff,ff,ff)"
	};
	DMAttrValueInit(DUISliderCtrlAttr,STRING_tipformat)DMAttrValueInit(DUISliderCtrlAttr,SIZE_range)DMAttrValueInit(DUISliderCtrlAttr,INT_value)
	DMAttrValueInit(DUISliderCtrlAttr,INT_thumbwidth)DMAttrValueInit(DUISliderCtrlAttr,INT_channelhei)DMAttrValueInit(DUISliderCtrlAttr,bool_bvert)
	DMAttrValueInit(DUISliderCtrlAttr,SKIN_channelskin)DMAttrValueInit(DUISliderCtrlAttr,COLOR_clrchannel)
	DMAttrValueInit(DUISliderCtrlAttr,SKIN_thumbskin)DMAttrValueInit(DUISliderCtrlAttr,COLOR_clrthumb)DMAttrValueInit(DUISliderCtrlAttr,bool_bshowtip)
}

namespace DM
{
	/// <summary>
	///		 DUISliderCtrl������ʵ�֣����ԣ�<see cref="DMAttr::DUISliderCtrlAttr"/>
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
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags, CPoint point);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnMouseLeave();
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	public:
		DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);

	public:// ����
		CRect GetChannelRect();
		CRect GetThumbRect();
		int HitTest(CPoint &pt);
		int GetValue(CPoint &pt);
		UINT DV_OnGetDlgCode(){return DMDLGC_WANTARROWS;};		

	public:  
		DM_BEGIN_ATTRIBUTES()
			DM_WSTRING_ATTRIBUTE(DMAttr::DUISliderCtrlAttr::STRING_tipformat,m_strTipFormat,DM_ECODE_OK)
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
		// ��,��ˮƽ״̬�£�����ʹthumb�ĸ߶Ⱥ�client�߶���ͬ��channel�Ŀ�Ⱥ�wid��ͬ
	public:
		DMCode OnAttributeRange(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeValue(LPCSTR lpszValue, bool bLoadXml);
	public:
		CStringW                         m_strTipFormat;
		CSize                            m_szRange;              ///< xΪ��Сֵ,yΪ���ֵ
		int                              m_iValue;               ///< ��ǰֵ

		int                              m_iThumbWid;            ///< thumb���
		int                              m_iChannelHei;			 ///< channel�߶�

		bool                             m_bVert;                ///< �Ƿ�ˮƽ
		bool                             m_bShowTip;             ///< �Ƿ���ʾ������ʾ
		IDMSkinPtr                       m_pChannelSkin;
		IDMSkinPtr                       m_pThumbSkin;
		DMColor							 m_crChannel;
		DMColor                          m_crThumb;

		// ����
		bool                             m_bDrag;                ///< �϶�״̬����갴��ʱ�ڻ����ϣ�
		CPoint						   	 m_DragPt;			     ///< �϶���λ�ã�x��ʾˮƽ��y��ʾ��ֱ)
		int                              m_iDragValue;           ///< �϶�����
		int                              m_iHitPre;              ///< �ϴ����λ��
	};

}//namespace DM