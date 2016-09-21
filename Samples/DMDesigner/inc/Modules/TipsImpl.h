// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	TipsImpl.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-3
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	class TipHWnd: public DMHWnd
	{  
	public:

	};

	/// <summary>
	///		DUI窗口形式显示tips
	/// </summary>
	class TipsImpl:public IDMToolTip,public DMCWnd
	{
		DMDECLARE_CLASS_NAME(TipsImpl,L"TipsImpl",DMREG_ToolTip);

	public:
		TipsImpl(void);
		virtual ~TipsImpl(void);

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
		DMSmartPtrT<DMHWnd>			 m_pWnd;

	protected:
		int							 m_iDelayTime;
		int							 m_iSpanTime;
		CStringW					 m_strXmlId;
		CStringW					 m_strTip; 
		DWORD                        m_dwReserve;
		CRect						 m_rcTarget;
		bool						 m_bShowing;                         ///< 显示定时器已启用，但窗口还未显示
		CRect                        m_rcPosFlags;
		CRect                        m_rcScreenTarget;
	};

}//namespace DM