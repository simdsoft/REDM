//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUILink.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-2	1.0			
//-------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUILink"/>��xml���Զ���
	/// </summary>
	class DUILinkAttr:public DUIWindowAttr
	{
	public:
		static const char* STRING_href;                                 ///< �������ӣ�����󴥷�,ʾ��:href="www.hgy413.com"
	};
	DMAttrValueInit(DUILinkAttr,STRING_href)
}

namespace DM
{
	/// <summary>
	///		 DUILink������ʵ�֣����ԣ�<see cref="DMAttr::DUILinkAttr"/>
	/// </summary>
	class DM_EXPORT DUILink:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIWindow,DUINAME_Link,DMREG_Window);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSEHOVER(OnMouseHover)
		DM_END_MSG_MAP()
	public:
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseHover(WPARAM wParam, CPoint pt);

	public:
		//---------------------------------------------------
		// Function Des: ����
		//---------------------------------------------------
		DMCode DV_OnSetCursor(const CPoint &pt); // �������巶Χ�ڲ����ù��
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);

	protected:// ����
		void CalcTextRect(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_WSTRING_ATTRIBUTE(DMAttr::DUILinkAttr::STRING_href, m_strLinkUrl, DM_ECODE_OK)
		DM_END_ATTRIBUTES()
	public:
		CRect                            m_rcText;					///< �ı���ʾ����λ��
		CStringW                         m_strLinkUrl;				///< ����URL
	};
	


}//namespace DM