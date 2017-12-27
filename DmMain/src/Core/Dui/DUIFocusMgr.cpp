#include "DmMainAfx.h"
#include "DUIFocusMgr.h"

namespace DM
{
	//----------------------------------------------------FocusSearch-------------------
	FocusSearch::FocusSearch(DUIWindow* pHWnd, bool bCycle)
		:m_pHWnd(pHWnd),m_bCycle(bCycle)
	{
	}

	FocusSearch::~FocusSearch()
	{
	}

	DUIWindow* FocusSearch::FindNextFocusableWnd(DUIWindow* pStartWnd, bool bReverse)
	{
		DUIWindow* pWnd = NULL;
		bool bResetStartWnd = false;
		do 
		{
			if (0 == m_pHWnd->DM_GetChildCount())
			{
				break;
			}

			if (NULL == pStartWnd)
			{
				if (bReverse)// m_pHWnd最底层最后一个子窗口从后向前遍历
				{
					pStartWnd = m_pHWnd->DM_GetWindow(GDW_LASTCHILD);
					while (pStartWnd->DM_GetChildCount())
					{
						pStartWnd = pStartWnd->DM_GetWindow(GDW_LASTCHILD);
					}
				}
				else// m_pHWnd的第一个子窗口开始
				{
					pStartWnd = m_pHWnd->DM_GetWindow(GDW_FIRSTCHILD);
				}

				bResetStartWnd = true;
			}

			DUIWindow *pStartGroupOwner = NULL;
			if (pStartWnd&&pStartWnd->DV_IsSiblingsAutoGroup()) // 一般这是在RadioButton中
			{
				pStartGroupOwner = pStartWnd->DM_GetWindow(GDW_PARENT);
			}

			if (bReverse)
			{
				bool bFindDown = !IsFocusable(pStartWnd);
				pWnd = FindPreviousFocusableWndImpl(pStartWnd,bResetStartWnd,true,bFindDown,pStartGroupOwner);
			}
			else
			{
				pWnd = FindNextFocusableWndImpl(pStartWnd,bResetStartWnd,true,true,pStartGroupOwner);
			}

			if (!pWnd&&m_bCycle&&pStartWnd)
			{
				m_bCycle = false;
				pWnd = FindNextFocusableWnd(NULL,bReverse);
			}

		} while (false);
		return pWnd;
	}

	// 辅助
	bool FocusSearch::IsFocusable(DUIWindow* pWnd)
	{//有焦点的条件: 1.有焦点 2.可见 3.不是Disable状态
		bool bRet = false;
		do 
		{
			if (NULL == pWnd                            // 窗口为NULL
				||!pWnd->DV_IsFocusable()				// 没有焦点
				||false == pWnd->DM_IsVisible(TRUE)    // 不可见
				||true == pWnd->DM_IsDisable(TRUE)		// Disable状态
				)
			{
				break;
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	// 窗口是有焦点的，且不属于指定的group,如rediobutton
	bool FocusSearch::IsWndFocusableCandidate(DUIWindow* pWnd, DUIWindow* pGroupOwner)
	{
		bool bRet = false;
		do 
		{	
			if (false == IsFocusable(pWnd))
			{
				break;
			}

			if (pGroupOwner
				&&pWnd->DV_IsSiblingsAutoGroup()
				&&pWnd->DM_GetWindow(GDW_PARENT)==pGroupOwner)
			{
				break;
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	DUIWindow* FocusSearch::FindNextFocusableWndImpl(DUIWindow* pStartWnd, bool bResetStartWnd, bool bFindUp, bool bFindDown, DUIWindow* pSkipGroupOwner)
	{
		if (bResetStartWnd)
		{
			if (IsWndFocusableCandidate(pStartWnd, pSkipGroupOwner))
			{
				DUIWindow* pWnd = NULL;
				if (pStartWnd->DV_IsSiblingsAutoGroup())
				{// 这里继续判断,radio支持不分组,tab切换焦点
					pWnd = pStartWnd->DV_GetSelSiblingInGroup();
				}
				if (!pWnd)
				{
					pWnd = pStartWnd;
				}

				if (IsFocusable(pWnd))
				{
					return pWnd;
				}
			}
		}

		if (bFindDown)
		{
			DUIWindow *pChild = pStartWnd->DM_GetWindow(GDW_FIRSTCHILD);
			if (pChild)
			{
				DUIWindow* pWnd = FindNextFocusableWndImpl(pChild, true, false, true, pSkipGroupOwner);
				if (pWnd)
				{
					return pWnd;
				}
			}
		}

		DUIWindow* pSiblingWnd = pStartWnd->DM_GetWindow(GDW_NEXTSIBLING);
		if (pSiblingWnd)
		{
			DUIWindow* pWnd = FindNextFocusableWndImpl(pSiblingWnd,true, false, true, pSkipGroupOwner);
			if(pWnd )
			{
				return pWnd;
			}
		}

		if (bFindUp)
		{
			DUIWindow* parent = pStartWnd->DM_GetWindow(GDW_PARENT);
			while (parent)
			{
				pSiblingWnd = parent->DM_GetWindow(GDW_NEXTSIBLING);
				if (pSiblingWnd)
				{
					return FindNextFocusableWndImpl(pSiblingWnd,true, true, true, pSkipGroupOwner);
				}
				parent = parent->DM_GetWindow(GDW_PARENT);
			}
		}

		// We found nothing.
		return NULL;
	}

	DUIWindow* FocusSearch::FindPreviousFocusableWndImpl(DUIWindow* pStartWnd, bool bResetStartWnd, bool bFindUp, bool bFindDown, DUIWindow * pSkipGroupOwner)
	{
		if (bFindDown)
		{
			DUIWindow *pChild = pStartWnd->DM_GetWindow(GDW_LASTCHILD);
			if (pChild)
			{
				DUIWindow *pRet = FindPreviousFocusableWndImpl(pChild,true,false,true,pSkipGroupOwner);
				if(pRet) return pRet;
			}
		}

		if (bResetStartWnd&&IsWndFocusableCandidate(pStartWnd,pSkipGroupOwner))
		{
			DUIWindow* pWnd = NULL;
			if (pStartWnd->DV_IsSiblingsAutoGroup())
			{// 这里继续判断,radio支持不分组,tab切换焦点
				pWnd = pStartWnd->DV_GetSelSiblingInGroup();
			}
			if (!pWnd)
			{
				pWnd = pStartWnd;
			}

			if (IsFocusable(pWnd))
			{
				return pWnd;
			}
		}

		DUIWindow *pPrevSibling = pStartWnd->DM_GetWindow(GDW_PREVSIBLING);
		if (pPrevSibling)
		{
			return FindPreviousFocusableWndImpl(pPrevSibling,true,true,true,pSkipGroupOwner);
		}
		if (bFindUp)
		{
			DUIWindow *pParent = pStartWnd->DM_GetWindow(GDW_PARENT);
			if (pParent) 
			{
				return FindPreviousFocusableWndImpl(pParent,true,true,false,pSkipGroupOwner);
			}
		}

		return NULL;
	}


	// --------------------------------------------------DUIFocusMgr 
	DUIFocusMgr::DUIFocusMgr(DUIWindow *pHWnd):m_pHWnd(pHWnd)
	{
		m_hDUIFocusBackupWnd = 0;
		m_hDUIFocusWnd		 = 0;
	}

	DUIFocusMgr::~DUIFocusMgr(void)
	{
	}

	void DUIFocusMgr::SetFocusedWnd(DUIWND DUIWnd)
	{
		SetFocuseWndWithReason(DUIWnd, FocusReasonByDirect);
	}

	DUIWND DUIFocusMgr::GetFocusedWnd()
	{
		return m_hDUIFocusWnd;
	}

	void DUIFocusMgr::RegisterAccel(const DUIAccel& Accel, IDMAccelHandler* pHandler)
	{
		// 注意，如果Accel原来不存在于m_AccelMap中，那么它会自动创建一个新的对象
		AccelHandlerList& handlerList = m_AccelMap[Accel];
		handlerList.AddHead(pHandler);
	}

	void DUIFocusMgr::UnregisterAccel(const DUIAccel& Accel, IDMAccelHandler* pHandler)
	{
		do 
		{
			if (!m_AccelMap.Lookup(Accel))
			{
				break;
			}
			AccelHandlerList* pHandlerList = &m_AccelMap[Accel];
			POSITION pos = pHandlerList->Find(pHandler);
			if (pos)
			{
				pHandlerList->RemoveAt(pos);
			}
		} while (false);
	}

	void DUIFocusMgr::UnregisterAccels(IDMAccelHandler* pHandler)
	{
		POSITION Pos = m_AccelMap.GetStartPosition();
		while (Pos)
		{
			AccelHandlerList& HandlerList = m_AccelMap.GetValueAt(Pos);
			POSITION PosFind = HandlerList.Find(pHandler);
			if (PosFind)
			{
				HandlerList.RemoveAt(PosFind);
			}
			m_AccelMap.GetNext(Pos);
		}
	}

	bool DUIFocusMgr::ProcessAccel(const DUIAccel& Accel)
	{
		bool bRet = false;
		do 
		{
			if (!m_AccelMap.Lookup(Accel)
				||m_AccelMap[Accel].IsEmpty())
			{
				break;
			}

			AccelHandlerList  HandlerList; 
			POSITION Pos = m_AccelMap[Accel].GetHeadPosition();
			while (Pos)// 复制一份，防止原数据被污染
			{
				IDMAccelHandlerPtr &t = m_AccelMap[Accel].GetNext(Pos);
				HandlerList.AddTail(t);
			}

			Pos = HandlerList.GetHeadPosition();
			while (Pos)// 循环处理
			{
				IDMAccelHandler*pHandler = HandlerList.GetNext(Pos);
				if (pHandler->OnAccelPressed(Accel))
				{
					bRet = true;
					break;
				}
			}
		} while (false);
		return bRet;
	}


	// 是否为Tab切换
	bool DUIFocusMgr::IsTabChanged(UINT vKey)
	{
		bool bRet = false;
		do 
		{
			if (VK_TAB != vKey)
			{
				break;
			}

			DUIWindow *pFocus = g_pDMDWndPool->FindDUIWnd(m_hDUIFocusWnd);
			if (pFocus&&pFocus->DV_OnGetDlgCode()&DMDLGC_WANTTAB)
			{
				break;
			}
			if (!!PUSH_CTRL)// 同时按下了Ctrl
			{// If the high-order bit is 1, the key is down; otherwise, it is up. 
				break;
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUIFocusMgr::OnKeyDown(UINT vKey)
	{
		bool bRet =  false;
		do 
		{
			if (IsTabChanged(vKey)) 
			{
				AdvanceFocus(!!PUSH_SHIFT);// 同时按下了Shift
				bRet = true;// 1. tab
				break;
			}

			DUIWindow *pFocusWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIFocusWnd);
			if (pFocusWnd									    // RedioButton，并且按下的键是上下左右箭头
				&&pFocusWnd->DV_IsSiblingsAutoGroup()
				&&(VK_LEFT==vKey||VK_RIGHT==vKey||VK_UP==vKey||VK_DOWN==vKey)
				)
			{
				UINT uCode = GDW_PREVSIBLING;
				if (VK_RIGHT==vKey
					||VK_DOWN==vKey)
				{
					uCode = GDW_NEXTSIBLING;
				}
				DUIWindow* pNextWnd = pFocusWnd->DM_GetWindow(uCode);
				while (pNextWnd)
				{
					if (pNextWnd->DV_IsSiblingsAutoGroup())
					{
						SetFocuseWndWithReason(pNextWnd->GetDUIWnd(),FocusReasonByTab); 
						break;
					}
					pNextWnd = pNextWnd->DM_GetWindow(uCode);
				}
				if (!pNextWnd)
				{
					pNextWnd = pFocusWnd->DM_GetWindow(GDW_PARENT)->DM_GetWindow(uCode==GDW_NEXTSIBLING?GDW_FIRSTCHILD:GDW_LASTCHILD);
					while (pNextWnd)
					{
						if (pNextWnd->DV_IsSiblingsAutoGroup())
						{
							SetFocuseWndWithReason(pNextWnd->GetDUIWnd(),FocusReasonByTab);
							break;
						}
						pNextWnd = pNextWnd->DM_GetWindow(uCode);
					}
				}
				bRet = true;// 2.类似RedioButton
				break;
			}

			DUIAccel Accel(vKey, !!PUSH_CTRL,!!PUSH_ALT,!!PUSH_SHIFT);
			if (ProcessAccel(Accel))
			{
				bRet = true;// 3.加速键被处理
				break;
			}
		} while (false);
		return bRet;
	}

	void DUIFocusMgr::AdvanceFocus(bool bReverse)
	{
		CheckFocusedWnd();
		DUIWindow *pFocus = g_pDMDWndPool->FindDUIWnd(m_hDUIFocusWnd);
		DUIWindow *pWnd   = GetNextFocusableWnd(pFocus,bReverse,true);
		if (pWnd)
		{
			SetFocuseWndWithReason(pWnd->GetDUIWnd(),FocusReasonByTab);
		}
	}

	void DUIFocusMgr::SetFocuseWndWithReason(DUIWND DUIWnd, FocusChangeReason reason)
	{
		do 
		{
			if (DUIWnd == m_hDUIFocusWnd)
			{
				break;
			}
			m_FocusChangeReason = reason;
			DUIWindow *pOldFocus=g_pDMDWndPool->FindDUIWnd(m_hDUIFocusWnd);
			DUIWindow *pNewFocus=g_pDMDWndPool->FindDUIWnd(DUIWnd);
			if (pOldFocus)  
			{
				pOldFocus->DM_SendMessage(WM_KILLFOCUS,(WPARAM)DUIWnd);
			}

			if (pNewFocus&&!pNewFocus->DM_IsDisable(TRUE))
			{
				pNewFocus->DM_SendMessage(WM_SETFOCUS,(WPARAM)m_hDUIFocusWnd,(LPARAM)reason);
				m_hDUIFocusWnd = DUIWnd;
			}
			else
			{
				m_hDUIFocusWnd=0;
			}
		} while (false);	
	}

	void DUIFocusMgr::StoreFocuseWnd()
	{
		CheckFocusedWnd();
		m_hDUIFocusBackupWnd = m_hDUIFocusWnd;
		m_hDUIFocusWnd = 0;
		DUIWindow *pWnd = g_pDMDWndPool->FindDUIWnd(m_hDUIFocusBackupWnd);
		if (pWnd)
		{
			pWnd->DM_SendMessage(WM_KILLFOCUS);
		}
	}

	void DUIFocusMgr::RestoreFocuseWnd()
	{
		DUIWindow *pWnd=g_pDMDWndPool->FindDUIWnd(m_hDUIFocusBackupWnd);
		if (pWnd
			&&!pWnd->DM_IsDisable(TRUE))
		{
			m_hDUIFocusWnd = m_hDUIFocusBackupWnd;
			pWnd->DM_SendMessage(WM_SETFOCUS,0,(LPARAM)FocusReasonByRestore);
		}
		m_hDUIFocusBackupWnd = 0;
	}

	// 辅助
	DUIWindow* DUIFocusMgr::GetNextFocusableWnd(DUIWindow* pStartWnd, bool bReverse, bool bLoop)
	{
		FocusSearch fs(m_pHWnd,bLoop);
		return fs.FindNextFocusableWnd(pStartWnd,bReverse);
	}

	void DUIFocusMgr::ClearFocus()
	{
		SetFocusedWnd(NULL);
	}

	// 校验Focuse窗口是否合法
	void DUIFocusMgr::CheckFocusedWnd()
	{
		if (m_hDUIFocusWnd)
		{
			DUIWindow *pFocus = g_pDMDWndPool->FindDUIWnd(m_hDUIFocusWnd);
			if (pFocus)
			{
				pFocus = pFocus->DM_GetTopParentWnd();
				if (pFocus!=m_pHWnd)
				{
					m_hDUIFocusWnd = 0;
				}
			}
			else
			{
				m_hDUIFocusWnd = 0;
			}
		}
	}
}//namespace DM
