//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMTooltipImpl2.h 
// File Des: tips的逻辑为:
//              1.DMHWnd中鼠标处理函数增加Tips消息过滤,并获取当前DUI窗口要显示的tip信息更新到DMtipImpl
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
//      guoyou		2015-7-13	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class TGPTipHWnd: public DMHWnd
	{  
	public:

	};

	/// <summary>
	///		DUI窗口形式显示tips
	/// </summary>
	class TGPTipsImpl:public IDMToolTip,public DMCWnd
	{
		DMDECLARE_CLASS_NAME(TGPTipsImpl,L"TGPTipsImpl",DMREG_ToolTip);

	public:
		TGPTipsImpl(void);
		virtual ~TGPTipsImpl(void);

		bool Create();													///< 创建定时器的隐藏窗口
		bool CreateTipsWnd();											///< 创建真实的显示窗口

	public:
		//---------------------------------------------------
		// Function Des: 消息分发系列函数
		//---------------------------------------------------
		void OnTimer(UINT_PTR idEvent);	

	public:
		DMCode Update(PDMToolTipInfo pInfo);
		DMCode Hide();
		DMCode Clear();
		DMCode RelayEvent(const MSG *pMsg);
		BOOL PreTranslateMessage(MSG* pMsg);


	public:// 辅助
		void ShowTips(bool bShow);
		bool IsTipWindows(){return IsWindow()&&m_pWnd&&m_pWnd->IsWindow();}///< 隐藏窗口和显示窗口都OK
		bool IsCursorPtInRect();
		void DestroyTipsWnd();

	public:
		DECLARE_MESSAGE_MAP()										// 仿MFC消息映射宏

	public:
		DMSmartPtrT<TGPTipHWnd>		  m_pWnd;

	protected:
		int							 m_iDelayTime;
		int							 m_iSpanTime;
		CStringW					 m_strXmlId;
		CStringW					 m_strTip;        
		CRect						 m_rcTarget;
		bool						 m_bShowing;                         ///< 显示定时器已启用，但窗口还未显示
		CRect                        m_rcPosFlags;
		CRect                        m_rcScreenTarget;
	};

}//namespace DM