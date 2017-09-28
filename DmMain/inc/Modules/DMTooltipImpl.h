//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMTooltipImpl.h 
// File Des: Tooltip的逻辑为:
//              1.DMHWnd中鼠标处理函数增加ToolTip消息过滤,并获取当前DUI窗口要显示的tip信息更新到DMTooltipImpl
//              2.PreTranslateMessage消息过滤中转发消息到RelayEvent判断显示状态
//                2.1.如在目标区域,并且未开显示定时器,则开显示定时器,延迟显示
//                2.2.如不在目标区域内,则直接kill显示定时器,消失
//              3.显示定时器触发后,立即开启消失定时器.
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-29	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class DM_EXPORT DMTooltipImpl:public IDMToolTip,public DMCWnd
	{
		DMDECLARE_CLASS_NAME(DMTooltipImpl,L"DMTooltipImpl",DMREG_ToolTip);
	public:
		DMTooltipImpl(void);
		virtual ~DMTooltipImpl(void);

		bool Create();
	public:
		//---------------------------------------------------
		// Function Des: 消息分发系列函数
		//---------------------------------------------------
		void OnPaint(HDC hdc);
		void OnTimer(UINT_PTR idEvent);

	public:
		DMCode Update(PDMToolTipInfo pInfo);
		DMCode Hide();
		DMCode Clear();
		DMCode RelayEvent(const MSG *pMsg);
		BOOL PreTranslateMessage(MSG* pMsg);

	public:// 辅助
		void ShowTooltip(bool bShow);
	
	public:
		DECLARE_MSG_MAP()										// 仿MFC消息映射宏

	protected:
		int						  m_iDelayTime;
		int						  m_iSpanTime;
		CStringW				  m_strTip;
		CRect					  m_rcTarget;
		HFONT					  m_font;
		bool                      m_bShowing;                       // 显示定时器已启用，但窗口还未显示
	};
}//namespace DM