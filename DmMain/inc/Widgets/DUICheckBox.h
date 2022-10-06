//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUICheckBox.h 
// File Des: ����CheckBoxʵ��
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-16	1.0			
//--------------------------------------------------------
#pragma once


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUICheckBox"/>��xml���Զ���
	/// </summary>
	class DUICheckBoxAttr :public DUIWindowAttr
	{
	public:
		static const char* bool_brightbox;                                 ///< boxͼ���Ƿ����ұ�,ʾ��:brightbox="1"
		static const char* bool_bmultilines;								///< �Ƿ�֧�ֶ���,ʾ��:bmultilines="1"
		static const char* INT_lineinter;                                  ///< �ڶ�����,�����м��,ʾ��:lineinter="1"
		static const char* SKIN_checkskin;                                 ///< checkͼ,8��ͼ��2��ͼ��1��ͼ,8��ͼ,ǰ4Ϊδѡ������״̬����4Ϊѡ��ʱ����״̬,ʾ��:checkskin="PNG:checkbox"
		static const char* SKIN_iconskin;                                  ///< iconͼ,��ʹ����iconͼ,��text���ٻ���,ʾ��:iconskin="PNG:iconbtn"
		static const char* INT_textoffset;                                 ///< �ı�����icon��ƫ�ƣ�Ĭ��Ϊ0,ʾ��:textoffset=10
		static const char* bool_bchecked;									///< �Ƿ�Ĭ�ϱ�ѡ��,ʾ��:bchecked="1"
	};
	DMAttrValueInit(DUICheckBoxAttr, bool_brightbox)
	DMAttrValueInit(DUICheckBoxAttr, bool_bmultilines)
	DMAttrValueInit(DUICheckBoxAttr, INT_lineinter)
	DMAttrValueInit(DUICheckBoxAttr, SKIN_checkskin)
	DMAttrValueInit(DUICheckBoxAttr, SKIN_iconskin)
	DMAttrValueInit(DUICheckBoxAttr, INT_textoffset)
	DMAttrValueInit(DUICheckBoxAttr, bool_bchecked)
}

namespace DM
{
	/// <summary>
	///		DUICheckBox������ʵ�֣����ԣ�<see cref="DMAttr::DUICheckBoxAttr"/>
	/// </summary>
	class DM_EXPORT DUICheckBox :public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUICheckBox, DUINAME_CheckBox, DMREG_Window)
	public:
		DUICheckBox();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonUp(UINT nFlags, CPoint pt);

	public:
		//---------------------------------------------------
		// Function Des: ����DUIWindow
		//---------------------------------------------------
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf, int cchText, LPRECT lpRect, UINT uFormat);
		bool DV_IsStateChangeRedraw() { return true; }

		//---------------------------------------------------
		// Function Des: ����
		//---------------------------------------------------
		DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);

	public:// ����
		int GetDrawState();
		CRect GetCheckRect();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUICheckBoxAttr::bool_brightbox, m_bRightBox, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUICheckBoxAttr::bool_bmultilines, m_bMultiLines, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUICheckBoxAttr::INT_lineinter, m_nLineInter, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUICheckBoxAttr::INT_textoffset, m_nTextOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUICheckBoxAttr::SKIN_checkskin, m_pCheckSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUICheckBoxAttr::SKIN_iconskin, m_pIconSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUICheckBoxAttr::bool_bchecked, OnAttributeChecked)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeChecked(LPCSTR lpszValue, bool bLoadXml);

	public:
		bool                                m_bRightBox;
		bool                                m_bMultiLines;
		int                                 m_nLineInter;
		int                                 m_nTextOffset;
		IDMSkinPtr                          m_pCheckSkin;
		IDMSkinPtr                          m_pIconSkin;
	};

}//namespace DM