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
	///		<see cref="DM::DUILink"/>的xml属性定义
	/// </summary>
	class DUILinkAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* STRING_href;                                 ///< 设置链接，点击后触发,示例:href="www.hgy413.com"
	};
	DMAttrValueInit(DUILinkAttr,STRING_href)
}

namespace DM
{
	/// <summary>
	///		 DUILink的内置实现，属性：<see cref="DMAttr::DUILinkAttr"/>
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
		// Function Des: 重载
		//---------------------------------------------------
		DMCode DV_OnSetCursor(const CPoint &pt); // 仅在字体范围内才设置光标
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);

	protected:// 辅助
		void CalcTextRect(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_STRING_ATTRIBUTE(DMAttr::DUILinkAttr::STRING_href, m_strLinkUrl, DM_ECODE_OK)
		DM_END_ATTRIBUTES()
	public:
		CRect                            m_rcText;					///< 文本显示所在位置
		CStringW                         m_strLinkUrl;				///< 窗口URL
	};
	


}//namespace DM