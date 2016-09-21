//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUISetTimer.h 
// File Des: DUI定时器
// File Summary: 使用原理可参考window程序设计五定时器第三种方式
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-4	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	struct TIMERINFO
	{
		DUIWND			hDUIWnd;
		UINT_PTR		uTimerID;
	};

	/// <summary>
	///		 全局的定时器，使用callback方式实现
	/// </summary>
	class DM_EXPORT DUITimer:public DMMapT<UINT_PTR, TIMERINFO>
	{
	public:
		DMCode SetTimer(DUIWND hDUIWnd,UINT_PTR uTimerID,UINT nElapse);	///< 设置定时器

		DMCode KillTimer(DUIWND hDUIWnd,UINT_PTR uTimerID);				///< 删除定时器(hDUIWnd+uTimerID均相同)
		DMCode KillTimer(DUIWND hDUIWnd);								///< 删除定时器(hDUIWnd相同)

	public:
		static void CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR uRealTimeID,DWORD dwTime);
	};

	/// <summary>
	///		 窗口的定时器,通过外部SetTimer来启用
	/// </summary>
	class DM_EXPORT FastTimerID
	{
	public:
		FastTimerID(DUIWND hDUIWnd, char TimerId)
		{
			m_Flag		 = 1;
			m_hDUIWnd	 = hDUIWnd;
			m_dwTimerID  = TimerId;
		}

		FastTimerID(DWORD dwID)
		{
			memcpy(this,&dwID,sizeof(DWORD));
		}
		operator DWORD &() const// 用于强制转换成DWORD
		{
			return *(DWORD*)this;
		}

	public:
		DWORD    m_hDUIWnd:24;     
		DWORD    m_dwTimerID:7;     
		DWORD    m_Flag:1;          ///< 标志为1时，表示该定时器为DUI定时器，设置为最高位，0x80000000
	};


}//namespace DM