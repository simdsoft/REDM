#include "DmMainAfx.h"
#include "DUIAnimate.h"

namespace DM
{
	DUITabAnimate::DUITabAnimate(IDMTabAnimateOwner *pOwner, DUIWindowPtr pFrom, DUIWindowPtr pTo,int iSteps,int iType)
	{
		//1. 创建二倍页宽的画布，分别记录旧状态和新状态
		DMASSERT(NULL!=pFrom&&NULL!=pTo&&NULL!=pOwner);
		DUIWindowPtr pWnd = pOwner->GetOwnerWindow();
		pWnd->DM_InsertChild(this);// 插入做为子窗口
		CRect rcPage = pFrom->m_rcWindow;
		DM_FloatLayout(rcPage);
		DMSmartPtrT<IDMRender> pDefRender;
		g_pDMApp->GetDefRegObj((void**)&pDefRender,DMREG_Render);
		pDefRender->CreateCanvas(rcPage.Width()*2,rcPage.Height(),&m_pMemCanvas);

		//2. 复制oldItem到画布前半部分
		m_pMemCanvas->SetViewportOrgEx(-rcPage.left, -rcPage.top);
		pFrom->DM_SetVisible(true);
		DM_DrawBackground(m_pMemCanvas,rcPage);
		pFrom->DM_SetVisible(false);

		//3. 复制NewItem到画布后半部分
		m_pMemCanvas->OffsetViewportOrg(rcPage.Width(),0);
		pTo->DM_SetVisible(true);
		DM_DrawBackground(m_pMemCanvas,rcPage);
		pTo->DM_SetVisible(false);

		m_pMemCanvas->SetViewportOrgEx(0,0);

		//4. 记录状态
		m_iCurSteps  = 0;
		m_nStepCount = iSteps;
		m_TabAniType = iType;
		m_pOwner     = pOwner;

		// 5.开始时间帧
		GetContainer()->OnRegisterTimeline(this);
	}

	BOOL DUITabAnimate::DM_OnEraseBkgnd(IDMCanvas* pCanvas)
	{
		return TRUE;
	}

	void DUITabAnimate::DM_OnPaint(IDMCanvas *pCanvas)
	{
		if (NULL == m_pMemCanvas)
		{
			return;
		}
		switch (m_TabAniType)
		{
		case TABANI_CINFLATE:
			{
				MyCenterInflateDraw(pCanvas);
				m_iCurSteps++;
			}
			break;
		case TABANI_CDEFLATE:
			{
				MyCenterDeflateDraw(pCanvas);
				m_iCurSteps++;
			}
			break;

		case TABANI_SD_LEFTRIGHT:
		case TABANI_SD_RIGHTLEFT:
		case TABANI_SD_TOPBOTTOM:
		case TABANI_SD_BOTTOMTOP:
			{
				MySlideDraw(m_TabAniType,pCanvas);
				m_iCurSteps++;
			}		
			break;
		default:
			break;
		}
	}

	void DUITabAnimate::OnDestroy()
	{
		GetContainer()->OnUnregisterTimeline(this);
		__super::OnDestroy();
	}

	void DUITabAnimate::OnSize(UINT nType, CSize size)
	{
		__super::OnSize(nType,size);
		if (m_pMemCanvas)
		{
			StopFrame();
		}
	}

	DMCode DUITabAnimate::OnTimeline()
	{
		do 
		{
			if (m_iCurSteps >= m_nStepCount -1)
			{
				StopFrame();
				break;
			}
			DM_Invalidate();
		} while (false);
		return DM_ECODE_OK;
	}

	void DUITabAnimate::StopFrame()
	{
		if (m_pMemCanvas)
		{
			m_pMemCanvas.Release();
			m_iCurSteps = 0;// 不要在OnTabAnimateFinish之后处理，因为OnTabAnimateFinish一般会delete 这个窗口对象,再处理就野指针了
			m_pOwner->OnTabAnimateFinish();
		}
	}

	void DUITabAnimate::BitBlt(IDMCanvas* pCanvasDest,int xDest,int yDest,int nWid,int nHei,IDMCanvas *pCanvasSrc,int xSrc,int ySrc,DWORD rop)
	{
		CRect rcDest(xDest,yDest,xDest+nWid,yDest+nHei);
		pCanvasDest->BitBlt(pCanvasSrc,xSrc,ySrc,&rcDest,rop);
	}

	void DUITabAnimate::MyCenterDeflateDraw(IDMCanvas *pCanvas)
	{
		CRect rcPage = m_rcWindow;
		int i        = m_iCurSteps;
		int nSteps   = m_nStepCount;
		int WidAvg   = rcPage.Width()/(2*nSteps);
		int HeiAvg   = rcPage.Height()/(2*nSteps);
		CPoint  ptMid((rcPage.left+rcPage.Width()/2),rcPage.top+rcPage.Height()/2);

		pCanvas->FillSolidRect(rcPage,RGBA(255,255,255,255));
		CRect rcItem(rcPage.left+i*WidAvg,rcPage.top+i*HeiAvg,rcPage.right-i*WidAvg,rcPage.bottom-i*HeiAvg);
		BitBlt(pCanvas,rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),m_pMemCanvas,i*WidAvg,i*HeiAvg,SRCCOPY);
	}

	void DUITabAnimate::MyCenterInflateDraw(IDMCanvas *pCanvas)
	{
		CRect rcPage = m_rcWindow;
		int i        = m_iCurSteps;
		int nSteps   = m_nStepCount;
		int WidAvg   = rcPage.Width()/(2*nSteps);
		int HeiAvg   = rcPage.Height()/(2*nSteps);
		CPoint  ptMid((rcPage.left+rcPage.Width()/2),rcPage.top+rcPage.Height()/2);

		pCanvas->FillSolidRect(rcPage,RGBA(255,255,255,255));
		CRect rcItem(ptMid.x-i*WidAvg,ptMid.y-i*HeiAvg,ptMid.x+i*WidAvg,ptMid.y+i*HeiAvg);
		BitBlt(pCanvas,rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),m_pMemCanvas, rcPage.Width()+(rcItem.left-rcPage.left),rcItem.top-rcPage.top,SRCCOPY);
	}

	void DUITabAnimate::MySlideDraw(int iType,IDMCanvas *pCanvas)
	{
		CRect rcPage = m_rcWindow;
		int i        = m_iCurSteps;
		int nSteps   = m_nStepCount;
		switch (iType)
		{
		case TABANI_SD_LEFTRIGHT:
			BitBlt(pCanvas,rcPage.left,rcPage.top,rcPage.Width()*(i+1)/nSteps,rcPage.Height(),m_pMemCanvas,rcPage.Width()+rcPage.Width()*(nSteps-i-1)/nSteps,0,SRCCOPY);
			BitBlt(pCanvas,rcPage.left+rcPage.Width()*(i+1)/nSteps,rcPage.top,rcPage.Width()*(nSteps-i-1)/nSteps,rcPage.Height(),m_pMemCanvas,0,0,SRCCOPY);
			break;
		case TABANI_SD_RIGHTLEFT:
			BitBlt(pCanvas,rcPage.left,rcPage.top,rcPage.Width(),rcPage.Height(),m_pMemCanvas,rcPage.Width()*(i+1)/nSteps,0,SRCCOPY);
			break;
		case TABANI_SD_TOPBOTTOM:
			BitBlt(pCanvas,rcPage.left,rcPage.top+rcPage.Height()*(nSteps-i-1)/nSteps,rcPage.Width(),rcPage.Height()*(i+1)/nSteps,m_pMemCanvas,rcPage.Width(),0,SRCCOPY);//new
			BitBlt(pCanvas,rcPage.left,rcPage.top,rcPage.Width(),rcPage.Height()*(nSteps-i-1)/nSteps,m_pMemCanvas,0,rcPage.Height()*(i+1)/nSteps,SRCCOPY);//old
			break;
		case TABANI_SD_BOTTOMTOP:
			BitBlt(pCanvas,rcPage.left,rcPage.top,rcPage.Width(),rcPage.Height()*(i+1)/nSteps,m_pMemCanvas,rcPage.Width(),rcPage.Height()*(nSteps-i-1)/nSteps,SRCCOPY);//new
			BitBlt(pCanvas,rcPage.left,rcPage.top+rcPage.Height()*(i+1)/nSteps,rcPage.Width(),rcPage.Height()*(nSteps-i-1)/nSteps,m_pMemCanvas,0,0,SRCCOPY);//old
			break;
		}
	}

}//namespace DM