//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name:	DMThreadMgr.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-2	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		map<线程,激活窗口>管理工具
	/// </summary>
	class DM_EXPORT DMThreadActiveWndTool:public DMMapT<DWORD, HWND>
	{
	public:
		DMThreadActiveWndTool();
		~DMThreadActiveWndTool();

		/// -------------------------------------------------
		/// @brief  设置当前线程激活窗口，并返回先前保存的激活窗口
		/// @param[in]		 hWnd    激活窗口句柄
		/// @return 先前线程激活窗口
		HWND SetActiveWnd(HWND hWnd);

		/// -------------------------------------------------
		/// @brief  获取当前线程激活窗口
		/// @return 当前线程激活窗口
		HWND GetActiveWnd();

	protected:
		DMLock              *m_pLock;
	};
}//namespace DM