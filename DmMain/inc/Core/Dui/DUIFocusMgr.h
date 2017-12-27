//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIFocusMgr.h 
// File Des: 焦点处理类
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-11-10	1.0			
//-------------------------------------------------------
#pragma once
#include "DUIAccel.h"

namespace DM
{
#define  PUSH_CTRL         (GetKeyState(VK_CONTROL)&0x8000)
#define  PUSH_SHIFT        (GetKeyState(VK_SHIFT)&0x8000)
#define  PUSH_ALT          (GetKeyState(VK_MENU)&0x8000)
#define  PUSH_LBUTTONDOWN  (GetAsyncKeyState(VK_LBUTTON)&0x8000)
	class FocusSearch
	{
	public:
		FocusSearch(DUIWindow* pHWnd, bool bCycle);
		virtual ~FocusSearch();

		/// -------------------------------------------------
		/// @brief			 查找下一个有焦点的窗口，如在一组,则认为check状态的窗口为组代表
		/// @param[in]		 pStartWnd		开始查找的窗口,如为NULL,则取m_pHWnd最底层最后一个子窗口开始（bReverse=true）或从m_pHWnd的第一个子窗口开始(bReverse=false)
		/// @param[in]		 bReverse		为true表示从后向前找
		/// @remark			 在没有找到状态下，如果m_bCycle为true,则从头再找一次(循环一次，m_bCycle置为false)
		/// @return  t:NULL表示没有找到
		DUIWindow* FindNextFocusableWnd(DUIWindow* pStartWnd,bool bReverse);

	public:
		bool IsFocusable(DUIWindow* pWnd);   
		bool IsWndFocusableCandidate(DUIWindow* pWnd, DUIWindow* pGroupOwner);

		// 返回下一个有焦点的窗口,bResetStartWnd为true表示起始窗口是内部自动设定的
		DUIWindow* FindNextFocusableWndImpl(DUIWindow* pStartWnd,bool bResetStartWnd,bool bFindUp,bool bFindDown, DUIWindow* pSkipGroupOwner);
		DUIWindow* FindPreviousFocusableWndImpl(DUIWindow* pStartWnd,bool bResetStartWnd,bool bFindUp,bool bFindDown,DUIWindow* pSkipGroupOwner);

	protected:
		DUIWindow*			m_pHWnd;
		bool				m_bCycle;    ///< 是否到了最后再从头循环查找
	};

	template<>// 此代码用于DUIFocusMgr中的CMap初始化、比较
	class DM::CElementTraits<DM::DUIAccel>:public DM::CElementTraitsBase<DM::DUIAccel>
	{
	public:
		static ULONG Hash(INARGTYPE element) throw()
		{
			return MAKELONG(element.GetModifier(),element.GetKey());
		}

		static bool CompareElements(INARGTYPE element1, INARGTYPE element2)
		{
			return Hash(element1)==Hash(element2);
		}

		static int CompareElementsOrdered(INARGTYPE element1, INARGTYPE element2)
		{
			return Hash(element1)-Hash(element2);
		}
	};

	typedef IDMAccelHandler* IDMAccelHandlerPtr;
	/// <summary>
	///		管理焦点在DUI窗口中的分发，以及加速键的注册
	/// </summary>
	class DM_EXPORT DUIFocusMgr : public IDMAccelMgr
	{
	public:
		enum FocusChangeReason
		{
			FocusReasonByTab,			 // Tab or Shift+Tab                
			FocusReasonByDirect,		 // 点击或直接跳转
			FocusReasonByRestore,		 // 主窗口失去再尝试恢复焦点
		};

		DUIFocusMgr(DUIWindow *pHWnd);
		~DUIFocusMgr(void);

		/// -------------------------------------------------
		/// @brief 设置焦点
		/// @param[in]  DUIWnd		需设置焦点的DUI句柄
		/// @return 无
		void SetFocusedWnd(DUIWND DUIWnd);

		/// -------------------------------------------------
		/// @brief 获得焦点
		/// @return 当前设置焦点的DUI句柄
		DUIWND GetFocusedWnd();

		//---------------------------------------------------
		// Function Des: 注册热键
		//---------------------------------------------------
		void RegisterAccel(const DUIAccel& Accel,IDMAccelHandler* pHandler);	///< 注册加速键以及事件处理接口对象
		void UnregisterAccel(const DUIAccel& Accel,IDMAccelHandler* pHandler);	///< 反注册加速键以及事件处理接口对象
		void UnregisterAccels(IDMAccelHandler* pHandler);						///< 反注册事件处理接口对象的所有加速键
		bool ProcessAccel(const DUIAccel& Accel);								///< 循环处理注册的加速键事件

	public:// 辅助
		bool IsTabChanged(UINT vKey);  
		bool OnKeyDown(UINT vKey);        // 1. tab 2.类似RedioButton 3.加速键被处理

		//---------------------------------------------------
		// Function Des: 更新窗口状态
		//---------------------------------------------------
		void AdvanceFocus(bool bReverse);
		void SetFocuseWndWithReason(DUIWND DUIWnd, FocusChangeReason reason);

		//---------------------------------------------------
		// Function Des: 存储和恢复focuse窗口，用于窗口在active/inactive中切换
		//---------------------------------------------------
		void StoreFocuseWnd();
		void RestoreFocuseWnd();

		DUIWindow* GetNextFocusableWnd(DUIWindow* pStartWnd, bool bReverse, bool bLoop);
		void ClearFocus();
		void CheckFocusedWnd();
		
	private:
		DUIWND								m_hDUIFocusWnd;
		DUIWND								m_hDUIFocusBackupWnd;   
		FocusChangeReason					m_FocusChangeReason;

		DUIWindow*							m_pHWnd;

		typedef CList<IDMAccelHandler*> AccelHandlerList;
		typedef CMap<DUIAccel, AccelHandlerList> AccelMap;
		AccelMap						    m_AccelMap;
	};

}//namespace DM
