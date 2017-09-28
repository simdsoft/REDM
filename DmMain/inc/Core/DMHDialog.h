//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMHDialog.h 
// File Des: 主窗口类,用于CDialog相关DoModal
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-3	1.0			
//-------------------------------------------------------
#pragma once
#include "DMHWnd.h"

namespace DM
{
	/// <summary>
	///		 主窗口类,用于CDialog相关DoModal
	/// </summary>
	class DM_EXPORT DMHDialog:public DMHWnd
	{
	public:
		DMHDialog();
		virtual~DMHDialog();

	public:
		virtual INT_PTR DoModal(LPCWSTR lpszXmlId, HWND hParent=NULL, bool bShadow=false,DM::CRect rect=DM::CRect(0,0,0,0));
		void EndDialog(INT_PTR nResult);

	protected:
		virtual DMCode OnOK();
		virtual DMCode OnCancel();
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	protected:
		DECLARE_MSG_MAP()// 仿MFC消息映射宏
		DECLARE_EVENT_MAP()

	protected:
		 INT_PTR		m_nRetCode;
	};

}//namespace DM