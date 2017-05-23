#include "DmMainAfx.h"
#include "DUIWndPool.h"

namespace DM
{
	//---------------------------------------DUIWndPool-----------------------
	DUIWndPool::DUIWndPool()
	{
		m_hNextWnd = 0;// 从0开始
	}

	DUIWndPool::~DUIWndPool()
	{
		RemoveAll();
		m_MainDUIWndArray.RemoveAll();
		m_RealDUIWndArray.RemoveAll();
	}

	DMCode DUIWndPool::AddMainDUIWnd(DUIWND hDUIWnd)
	{
		m_MainDUIWndArray.Add(hDUIWnd);
		return DM_ECODE_OK;
	}

	DMCode DUIWndPool::RemoveMainDUIWnd(DUIWND hDUIWnd)
	{
		int count = (int)m_MainDUIWndArray.GetCount();
		for (int i=0; i<count; i++)
		{
			if (hDUIWnd == m_MainDUIWndArray[i])
			{
				m_MainDUIWndArray.RemoveAt(i);
				break;
			}
		}
		return DM_ECODE_OK;
	}

	DMCode DUIWndPool::AddRealDUIWnd(DUIWND hDUIWnd)
	{
		m_RealDUIWndArray.Add(hDUIWnd);
		return DM_ECODE_OK;
	}

	DMCode DUIWndPool::HideAllRealDUIWnd(DUIWND hMainDUIWnd)
	{
		do 
		{
			DUIWindowPtr pMainWnd = FindDUIWnd(hMainDUIWnd);
			if (!pMainWnd)
			{
				break;
			}

			HWND hMainWnd = pMainWnd->GetContainer()->OnGetHWnd();
			if (!::IsWindow(hMainWnd))
			{
				break;
			}
			int count = (int)m_RealDUIWndArray.GetCount();
			for (int i=0; i<count; i++)
			{
				DUIWND hDUIWnd = m_RealDUIWndArray[i];
				IDUIRealWnd* pWnd = dynamic_cast<IDUIRealWnd*>(FindDUIWnd(hDUIWnd));
				if (pWnd)
				{
					HWND hWnd = pWnd->GetContainer()->OnGetHWnd();
					if (hWnd == hMainWnd) // 同一个DUI主窗口
					{
						if (::IsWindow(pWnd->m_hRealWnd))
						{
							::ShowWindow(pWnd->m_hRealWnd, SW_HIDE);
						}
					}
				}
			}
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUIWndPool::ShowAllRealDUIWnd(DUIWND hMainDUIWnd)
	{
		do 
		{
			DUIWindowPtr pMainWnd = FindDUIWnd(hMainDUIWnd);
			if (!pMainWnd)
			{
				break;
			}

			HWND hMainWnd = pMainWnd->GetContainer()->OnGetHWnd();
			if (!::IsWindow(hMainWnd))
			{
				break;
			}
			int count = (int)m_RealDUIWndArray.GetCount();
			for (int i=0; i<count; i++)
			{
				DUIWND hDUIWnd = m_RealDUIWndArray[i];
				IDUIRealWnd* pWnd = dynamic_cast<IDUIRealWnd*>(FindDUIWnd(hDUIWnd));
				if (pWnd)
				{
					HWND hWnd = pWnd->GetContainer()->OnGetHWnd();
					if (hWnd == hMainWnd) // 同一个DUI主窗口
					{
						if (::IsWindow(pWnd->m_hRealWnd))
						{
							::ShowWindow(pWnd->m_hRealWnd, SW_SHOW);
						}
					}
				}
			}
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUIWndPool::RemoveRealDUIWnd(DUIWND hDUIWnd)
	{
		int count = (int)m_RealDUIWndArray.GetCount();
		for (int i=0; i<count; i++)
		{
			if (hDUIWnd == m_RealDUIWndArray[i])
			{
				m_RealDUIWndArray.RemoveAt(i);
				break;
			}
		}
		return DM_ECODE_OK;
	}

	DUIWindowPtr DUIWndPool::FindDUIWnd(DUIWND hDUIWnd)
	{
		DMAutoLock autolock(&m_Lock);
		DUIWindowPtr pDUIWindow = NULL;
		do 
		{
			if (0 == hDUIWnd)// 最小为0
			{
				break;
			}
			GetObjByKey(hDUIWnd,pDUIWindow);// 安全操作MAP
		} while (false);
		return pDUIWindow;
	}

	DUIWND DUIWndPool::NewDUIWnd(DUIWindowPtr pDUIWindow)
	{
		DMAutoLock autolock(&m_Lock);
		DUIWND NewDUIWnd = ++ m_hNextWnd;
		AddKey(NewDUIWnd,pDUIWindow);
		return NewDUIWnd;
	}

	bool DUIWndPool::DestoryDUIWnd(DUIWND hDUIWnd)
	{
		DMAutoLock autolock(&m_Lock);
		return RemoveKey(hDUIWnd);;
	}

	DMCode DUIWndPool::UpdateSkin(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DUIWindowPtr pDUIWindow = NULL;
			int nCount  = m_MainDUIWndArray.GetCount();
			for (int i=0;i<nCount;i++)
			{
				pDUIWindow = NULL;
				GetObjByKey(m_MainDUIWndArray[i],pDUIWindow);// 安全操作MAP
				if (pDUIWindow&&pDUIWindow->DM_IsVisible(true))
				{
					UpdateSkinLoop(pDUIWindow);
				}
			}

			iErr = DM_ECODE_OK;
			if (DMREG_Skin == lp)
			{
				g_pDMAppData->ClearUpdateSkinArray();
			}
		} while (false);
		return iErr;
	}

	DMCode DUIWndPool::RedrawAll()
	{
		DUIWindowPtr pDUIWindow = NULL;
		int nCount  = m_MainDUIWndArray.GetCount();
		for (int i=0;i<nCount;i++)
		{
			pDUIWindow = NULL;
			GetObjByKey(m_MainDUIWndArray[i],pDUIWindow);// 安全操作MAP
			if (pDUIWindow&&pDUIWindow->DM_IsVisible(true))
			{
				pDUIWindow->DM_Invalidate();
			}
		}

		return DM_ECODE_OK;
	}

	/// 当父窗口需要换肤时，子窗口就不用再计算了
	DMCode DUIWndPool::UpdateSkinLoop(DUIWindowPtr pWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pWnd||false == pWnd->DM_IsVisible(true))
			{
				break;
			}

			iErr = pWnd->DV_UpdateSkin(0,DMREG_Skin);
			if (DMSUCCEEDED(iErr))
			{
				pWnd->DM_Invalidate();
				break;
			}
		
			DUIWindow *pChild = pWnd->DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild) 	           // 递归出口0
			{
				iErr = UpdateSkinLoop(pChild);
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}
		} while (false);
		return iErr;
	}


}//namespace DM