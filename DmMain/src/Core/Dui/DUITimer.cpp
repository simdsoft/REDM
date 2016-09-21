#include "DmMainAfx.h"
#include "DUITimer.h"

namespace DM
{
	DMCode DUITimer::SetTimer(DUIWND hDUIWnd,UINT_PTR uTimerID,UINT nElapse)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			KillTimer(hDUIWnd,uTimerID);
			UINT_PTR uRealTimeID = ::SetTimer(NULL,uTimerID,nElapse,TimerProc);
			if (0 == uRealTimeID) 
			{
				break;
			}
			TIMERINFO TimeInfo = {hDUIWnd,uTimerID};
			if (!AddKey(uRealTimeID,TimeInfo))
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUITimer::KillTimer(DUIWND hDUIWnd,UINT_PTR uTimerID)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			POSITION pos = m_Map.GetStartPosition();
			while (pos)
			{
				CMap<UINT_PTR,TIMERINFO>::CPair*p = m_Map.GetNext(pos);
				if (p->m_value.hDUIWnd==hDUIWnd&&p->m_value.uTimerID==uTimerID)
				{
					::KillTimer(NULL,p->m_key);
					m_Map.RemoveKey(p->m_key);
					iErr = DM_ECODE_OK;
					break;
				}
			}
		} while (false);
		return iErr;
	}

	DMCode DUITimer::KillTimer(DUIWND hDUIWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			POSITION pos = m_Map.GetStartPosition();
			while (pos)
			{
				CMap<UINT_PTR,TIMERINFO>::CPair *p = m_Map.GetNext(pos);
				if(p->m_value.hDUIWnd==hDUIWnd)
				{
					::KillTimer(NULL,p->m_key);
					m_Map.RemoveKey(p->m_key);
					iErr = DM_ECODE_OK;
				}
			}
		} while (false);
		return iErr;
	}

	void CALLBACK DUITimer::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR uRealTimeID, DWORD dwTime)
	{
		TIMERINFO TimeInfo;
		if (g_pDMAppData->m_Timer->GetObjByKey(uRealTimeID,TimeInfo))
		{
			DUIWindow *pWnd = g_pDMDWndPool->FindDUIWnd(TimeInfo.hDUIWnd);
			if (pWnd)
			{
				pWnd->DM_SendMessage(WM_DUITIMER,TimeInfo.uTimerID);
			}
		}
	}
}//namespace DM