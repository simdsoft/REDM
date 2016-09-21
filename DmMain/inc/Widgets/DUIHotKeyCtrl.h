//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name:DUIHotKeyCtrl.h 
// File Des:
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-8-3	1.0			
//--------------------------------------------------------
#pragma once
#include "DUIEditHelper.h"

namespace DMAttr
{
	class DUIHotKeyCtrlAttr:public DUIWindowAttr
	{
	public:
		// 光标
		static wchar_t* COLOR_clrcaret;                                 ///< 光标颜色,示例:clrcaret="pbgra(ff,ff,ff,ff)"
		static wchar_t* INI_caretanimatecount;                          ///< 光标渐隐渐显次数，1次为正常状态，默认为6次,示例： caretanimatecount="6"
	};
	DMAttrValueInit(DUIHotKeyCtrlAttr,COLOR_clrcaret)DMAttrValueInit(DUIHotKeyCtrlAttr,INI_caretanimatecount)
}

namespace DM
{
	/// <summary>
	///		 DUIHotKeyCtrl的内置实现，属性：<see cref="DMAttr::DUIHotKeyCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUIHotKeyCtrl:public DUIWindow,public DUIAccel
	{
		DMDECLARE_CLASS_NAME(DUIHotKeyCtrl,DUINAME_HotKey,DMREG_Window);
	public:
		DUIHotKeyCtrl();

	public:// copy from CHotKeyCtrl 
		// Sets the hot key combination for the hot key control.
		void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);

		// Retrieves the virtual key code and modifier flags of the hot key from the hot key control.
		DWORD GetHotKey() const;

		// Retrieves the virtual key code and modifier flags of the hot key from the hot key control.
		void GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const;

		// Retrieves a string representation of the hot key code and flags.
		CStringW GetHotKeyName() const;

		// Defines the invalid combinations and the default modifier combination for the hot key control.
		void SetRules(WORD wInvalidComb, WORD wModifiers);

		// Retrieves a string representation of the virtual key.
		static CStringW GetKeyName(UINT vk, BOOL fExtended);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_KEYUP(OnKeyUp)
			MSG_WM_SYSKEYDOWN(OnSysKeyDown)
			MSG_WM_SYSKEYUP(OnSysKeyUp)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		void DM_OnSetFocus();
		void DM_OnKillFocus();

	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		UINT DV_OnGetDlgCode(){return (DMDLGC_WANTALLKEYS|DMDLGC_WANTSYSKEY) & (~DMDLGC_WANTTAB);}
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);


	public:// 辅助
		void UpdateModifier();
		void UpdateCaret();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIHotKeyCtrlAttr::COLOR_clrcaret,OnAttrCuretClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIHotKeyCtrlAttr::INI_caretanimatecount,OnAttrCuretAnimateCount)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttrCuretClr(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttrCuretAnimateCount(LPCWSTR pszValue, bool bLoadXml);

	public:
		bool                      m_bSetting;						///< 正在设置中
		WORD                      m_wInvalidModifiers;				///< 无效组合键的替换方案,默认方案
		WORD					  m_wInvalidComb;					///< 无效的组合键
		DUICaret*				  m_pCaret;
		DMColor                   m_clrCaret;
		int						  m_iCaretAniCount;

	};

}//namespace DM