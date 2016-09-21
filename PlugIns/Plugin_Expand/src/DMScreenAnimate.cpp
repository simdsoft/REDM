#include "Plugin_ExpandAfx.h"
#include "DMScreenAnimate.h"

namespace DM
{
	BEGIN_MSG_MAP(DMScreenAnimate)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_PAINT(OnPaint)
		CHAIN_MSG_MAP(DMAnimateWndBase)
	END_MSG_MAP()
	DMScreenAnimate::DMScreenAnimate()
	{
		m_pOwner  = NULL;
		m_byAlpha = 0xff;
		m_pTempWnd = NULL;
	}

	DMCode DMScreenAnimate::AnimateInit(IDMAnimateOwner*pOwner,IDMCanvas* pStartCanvas,IDMCanvas* pEndCanvas,WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pOwner||NULL == pStartCanvas)
			{
				break;
			}
			m_pOwner	   = pOwner;
			m_pStartCanvas = pStartCanvas;

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMScreenAnimate::AnimateBegin(WPARAM wp, LPARAM lp)
	{	
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner||NULL==m_pStartCanvas)
			{
				break;
			}	

			m_pOwner->AnimateBegin_Callback(this,0,0);
			if (DMSCREEN_4 == wp)
			{
				Scatter4(LOWORD(lp),HIWORD(lp));
			}
			if (DMSCREEN_MELT == wp)
			{
				MeltLeft(LOWORD(lp),HIWORD(lp));
				MeltTop(LOWORD(lp),HIWORD(lp));
			}
			m_pOwner->AnimateEnd_Callback(this,0,0);
		
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMScreenAnimate::AnitmateGet(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMScreenAnimate::AnimateEnd(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}
			if (IsWindow())
			{
				DestroyWindow();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	void DMScreenAnimate::Scatter4(int count, int nSleeptime)
	{
		if (0==count)
		{
			count = 10;
		}
		if (0 == nSleeptime)
		{
			nSleeptime = 1;
		}
		CRect rcInit = m_pOwner->GetAnimateOwnerWnd()->m_rcWindow;
		HWND hWnd = m_pOwner->GetAnimateOwnerWnd()->GetContainer()->OnGetHWnd();
		DMANI_ClientToScreen(hWnd, rcInit);
		CRect rcLeftTop = rcInit;
		int nWid = rcInit.Width()/2;int nHei = rcInit.Height()/2;
		rcLeftTop.right = rcLeftTop.left+nWid;rcLeftTop.bottom = rcLeftTop.top+nHei;
		CRect rcRightTop = rcLeftTop;CRect rcLeftBtm = rcLeftTop;CRect rcRightBtm = rcLeftTop;
		rcRightTop.OffsetRect(nWid,0);rcLeftBtm.OffsetRect(0,nHei);rcRightBtm.OffsetRect(nWid,nHei);

		DMAnimateWndBase lefttopWnd,righttopWnd,leftbtmWnd,rightbtmWnd;
		lefttopWnd.Create(rcLeftTop);
		lefttopWnd.UpdateLayerWnd(m_pStartCanvas,m_byAlpha,CPoint(0,0));

		righttopWnd.Create(rcRightTop);
		righttopWnd.UpdateLayerWnd(m_pStartCanvas,m_byAlpha,CPoint(nWid,0));

		leftbtmWnd.Create(rcLeftBtm);
		leftbtmWnd.UpdateLayerWnd(m_pStartCanvas,m_byAlpha,CPoint(0,nHei));

		rightbtmWnd.Create(rcRightBtm);
		rightbtmWnd.UpdateLayerWnd(m_pStartCanvas,m_byAlpha,CPoint(nWid,nHei));
		lefttopWnd.ShowWindow(SW_SHOWNOACTIVATE);
		righttopWnd.ShowWindow(SW_SHOWNOACTIVATE);
		leftbtmWnd.ShowWindow(SW_SHOWNOACTIVATE);
		rightbtmWnd.ShowWindow(SW_SHOWNOACTIVATE);
		Sleep(1);
		

		// 简洁点，直接sleep动画，要改的自己改吧，仅做演示
		CPoint ptCenter = rcInit.CenterPoint();
		MONITORINFO mi = {sizeof(MONITORINFO)};
		HMONITOR hMonitor = ::MonitorFromWindow(m_pOwner->GetAnimateOwnerWnd()->GetContainer()->OnGetHWnd(), MONITOR_DEFAULTTONULL);
		::GetMonitorInfo(hMonitor, &mi);
		CRect rcWork = mi.rcWork;
		int difflx = (ptCenter.x-rcWork.left)/count;
		int diffrx = (rcWork.right-ptCenter.x)/count;
		int diffty = (ptCenter.y-rcWork.top)/count;
		int diffby = (rcWork.bottom-ptCenter.y)/count;

		for (int i=0;i<count;i++)
		{
			rcLeftTop.OffsetRect(-difflx,-diffty);
			lefttopWnd.MoveWindow(rcLeftTop);
			rcRightTop.OffsetRect(diffrx,-diffty);
			righttopWnd.MoveWindow(rcRightTop);
			rcLeftBtm.OffsetRect(-difflx,diffby);
			leftbtmWnd.MoveWindow(rcLeftBtm);
			rcRightBtm.OffsetRect(diffrx,diffby);
			rightbtmWnd.MoveWindow(rcRightBtm);
			Sleep(nSleeptime);
		}

		// 恢复
		for (int i=0;i<count;i++)
		{
			rcLeftTop.OffsetRect(difflx,diffty);
			lefttopWnd.MoveWindow(rcLeftTop);
			rcRightTop.OffsetRect(-diffrx,diffty);
			righttopWnd.MoveWindow(rcRightTop);
			rcLeftBtm.OffsetRect(difflx,-diffby);
			leftbtmWnd.MoveWindow(rcLeftBtm);
			rcRightBtm.OffsetRect(-diffrx,-diffby);
			rightbtmWnd.MoveWindow(rcRightBtm);
			Sleep(nSleeptime);
		}

		lefttopWnd.DestroyWindow();
		righttopWnd.DestroyWindow();
		leftbtmWnd.DestroyWindow();
		rightbtmWnd.DestroyWindow();
	}

	void DMScreenAnimate::MeltLeft(int count,int nSleeptime)
	{
		if (0==count)
		{
			count = 10;
		}
		if (0 == nSleeptime)
		{
			nSleeptime = 1;
		}
		CRect rcInit = m_pOwner->GetAnimateOwnerWnd()->m_rcWindow;
		HWND hWnd = m_pOwner->GetAnimateOwnerWnd()->GetContainer()->OnGetHWnd();
		DMANI_ClientToScreen(hWnd, rcInit);
		CRect rcMeltLeft[10];
		rcMeltLeft[0] = rcInit;
		int iHei = rcInit.Height()/10;
		rcMeltLeft[0].bottom = rcMeltLeft[0].top+iHei;
		for (int i=1;i<10;i++)
		{
			rcMeltLeft[i] = rcMeltLeft[0];
			rcMeltLeft[i].OffsetRect(0,i*iHei);
		}
		
		DMAnimateWndBase MeltLeftWnd[10];
		for (int i=0;i<10;i++)
		{
			MeltLeftWnd[i].Create(rcMeltLeft[i]);
			MeltLeftWnd[i].UpdateLayerWnd(m_pStartCanvas,m_byAlpha,CPoint(0,i*iHei));
		}
		MONITORINFO mi = {sizeof(MONITORINFO)};
		HMONITOR hMonitor = ::MonitorFromWindow(m_pOwner->GetAnimateOwnerWnd()->GetContainer()->OnGetHWnd(), MONITOR_DEFAULTTONULL);
		::GetMonitorInfo(hMonitor, &mi);
		CRect rcWork = mi.rcWork;
		int diffx = (rcInit.right-rcWork.left)/count;

		// 先错开
		int iDelta = rcInit.Width()/10;
		for (int i=0;i<10;i++)
		{
			rcMeltLeft[i].OffsetRect(-iDelta*(5-i),0);
			MeltLeftWnd[i].MoveWindow(rcMeltLeft[i]);
			MeltLeftWnd[i].ShowWindow(SW_SHOWNOACTIVATE);
		}

		for (int j=0;j<count;j++)
		{
			for (int i=0;i<10;i++)
			{
				rcMeltLeft[i].OffsetRect(-diffx,0);
				MeltLeftWnd[i].MoveWindow(rcMeltLeft[i]);
			}
			Sleep(nSleeptime);
		}
		Sleep(nSleeptime);

		for (int i=0;i<10;i++)
		{
			MeltLeftWnd[i].DestroyWindow();
		}
	}

	void DMScreenAnimate::MeltTop(int count,int nSleeptime)
	{
		if (0==count)
		{
			count = 10;
		}
		if (0 == nSleeptime)
		{
			nSleeptime = 1;
		}
		CRect rcInit = m_pOwner->GetAnimateOwnerWnd()->m_rcWindow;
		HWND hWnd = m_pOwner->GetAnimateOwnerWnd()->GetContainer()->OnGetHWnd();
		DMANI_ClientToScreen(hWnd, rcInit);
		MONITORINFO mi = {sizeof(MONITORINFO)};
		HMONITOR hMonitor = ::MonitorFromWindow(m_pOwner->GetAnimateOwnerWnd()->GetContainer()->OnGetHWnd(), MONITOR_DEFAULTTONULL);
		::GetMonitorInfo(hMonitor, &mi);
		CRect rcWork = mi.rcWork;
		CRect rcMeltTop[10];
		rcMeltTop[0] = rcInit;
		int iWid = rcInit.Width()/10;
		rcMeltTop[0].bottom = rcWork.top;
		rcMeltTop[0].top = rcMeltTop[0].bottom-rcInit.Height();
		rcMeltTop[0].right = rcMeltTop[0].left+iWid;
		for (int i=1;i<10;i++)
		{
			rcMeltTop[i] = rcMeltTop[0];
			rcMeltTop[i].OffsetRect(i*iWid,0);
		}
		DMAnimateWndBase MeltTopWnd[10];
		for (int i=0;i<10;i++)
		{
			MeltTopWnd[i].Create(rcMeltTop[i]);
			MeltTopWnd[i].UpdateLayerWnd(m_pStartCanvas,m_byAlpha,CPoint(i*iWid,0));
			MeltTopWnd[i].ShowWindow(SW_SHOWNOACTIVATE);
		}
		int diffy = (rcInit.bottom-rcWork.top)/count;
		for (int i=0;i<10;i++)
		{
			for (int j=0;j<count;j++)
			{
				rcMeltTop[i].OffsetRect(0,diffy);
				MeltTopWnd[i].MoveWindow(rcMeltTop[i]);
				Sleep(nSleeptime);
			}
		}
		for (int i=0;i<10;i++)
		{
			MeltTopWnd[i].DestroyWindow();
		}
	}


	void DMScreenAnimate::OnPaint(HDC hdc)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		ClientToScreen(rcClient);
		DMAutoDC hdcDst;
		do 
		{
			BLENDFUNCTION bf = {AC_SRC_OVER,0,0xff,AC_SRC_ALPHA};
			if (m_bFadeing)// 渐显渐隐中
			{
				bf.SourceConstantAlpha = (byte)m_byFadeAlpha;
				HDC dcMem = m_pStartCanvas->GetDC();
				::UpdateLayeredWindow(m_hWnd,hdcDst,&rcClient.TopLeft(),&rcClient.Size(),dcMem,&CPoint(0,0),0,&bf,ULW_ALPHA);
				m_pStartCanvas->ReleaseDC(dcMem);
				break;
			}
		} while (false);
	}

	void DMScreenAnimate::OnTimer(UINT_PTR idEvent)
	{
		do 
		{
			__super::OnTimer(idEvent);
			if (m_bFadeing)
			{
				break;
			}

		} while (false);
	}

}