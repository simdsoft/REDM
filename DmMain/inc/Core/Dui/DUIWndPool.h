//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIWndPool.h 
// File Des: 给每个DUI模拟窗口一个唯一的关联标识符
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-30	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// -------------------------------------------------
	class DUIWindow;
	/// <summary>
	///		管理模拟DUI窗口句柄的池
	/// </summary>
	class DM_EXPORT DUIWndPool:public DMMapT<DUIWND,DUIWindowPtr>
	{
	public:
		DUIWndPool();
		~DUIWndPool();

	public:
		/// -------------------------------------------------
		/// @brief 加入主DUI窗口
		/// @param[in]		 hDUIWnd		DUI窗口的唯一句柄
		/// @return DMCode 
		DMCode AddMainDUIWnd(DUIWND hDUIWnd);
		DMCode RemoveMainDUIWnd(DUIWND hDUIWnd);///< 移除

		DMCode AddRealDUIWnd(DUIWND hDUIWnd);	
		DMCode HideAllRealDUIWnd(DUIWND hMainDUIWnd);
		DMCode ShowAllRealDUIWnd(DUIWND hMainDUIWnd); //lzlong add
		DMCode RemoveRealDUIWnd(DUIWND hDUIWnd);///< 移除

		/// -------------------------------------------------
		/// @brief 找到DUI窗口
		/// @param[in]		 hDUIWnd		DUI窗口的唯一句柄
		/// @return DUIWindowPtr，失败为NULL
		DUIWindowPtr FindDUIWnd(DUIWND hDUIWnd);

		/// -------------------------------------------------
		/// @brief 加入到DUIP窗口句柄中
		/// @param[in]		 pDUIWindow    要关联的DUI窗口的指针
		/// @return DUI窗口的唯一句柄
		DUIWND NewDUIWnd(DUIWindowPtr pDUIWindow);

		/// -------------------------------------------------
		/// @brief 移除DUIP窗口句柄
		/// @param[in]		 hDUIWnd		要移除的DUI窗口的句柄
		/// @return true:成功,false:失败
		bool DestoryDUIWnd(DUIWND hDUIWnd);

		/// -------------------------------------------------
		/// @brief 刷新使用skin列表的DUI窗口，
		/// @param[in]		 wp		参数
		/// @param[in]		 lp		参数
		/// @remark 用于换肤
		/// @return DMCode
		DMCode UpdateSkin(WPARAM wp, LPARAM lp);

		DMCode RedrawAll();

		DMCode UpdateSkinLoop(DUIWindowPtr pWnd);

	public:
		DMLock                       m_Lock;
		DUIWND                       m_hNextWnd;		///< 当前使用的最高DUI窗口句柄
		CArray<DUIWND>               m_MainDUIWndArray; ///< 主窗口的DUI列
		CArray<DUIWND>               m_RealDUIWndArray; ///< 真实窗口的DUI列
	};

	/// -------------------------------------------------


}//namespace DM