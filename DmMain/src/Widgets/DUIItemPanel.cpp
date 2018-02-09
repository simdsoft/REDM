#include "DmMainAfx.h"
#include "DUIItemPanel.h"

namespace DM
{

	DUIItemPanel::DUIItemPanel(DMXmlNode XmlNode,IDMItemPanelOwner* pItemOwner)
		:DMContainerImpl(this)// 
	{
		DMASSERT(NULL!=pItemOwner);
		m_pDUIWnd				= pItemOwner->GetOwnerWindow();
		m_pItemOwner            = pItemOwner;
		m_dwData				= 0;
		m_dwItemId				= -1;

		SetContainer(this);
		if (XmlNode.IsValid())
		{
			InitDMData(XmlNode);
			DM_SetVisible(true);// 更新父子visible关系,如DUIWNDSTATE_Novisible
		}

		// itempanel
		DMADDEVENT(DMEventOfPanelArgs::EventID);
		DMADDEVENT(DMEventOfPanelClickArgs::EventID);
		DMADDEVENT(DMEventOfPanelRClickArgs::EventID);
	}

	DUIItemPanel::~DUIItemPanel()
	{
		SetRefCount(0);
	}

	void DUIItemPanel::DrawItem(IDMCanvas* pCanvas, CRect& rc)
	{
		pCanvas->OffsetViewportOrg(rc.left,rc.top);
		DM_RedrawRegion(pCanvas,NULL);
		pCanvas->OffsetViewportOrg(-rc.left,-rc.top);
	}

	void DUIItemPanel::ModifyState(DWORD dwStateAdd, DWORD dwStateRemove)
	{
		DM_ModifyState(dwStateAdd,dwStateRemove,false);
	}

	void DUIItemPanel::SetItemId(LPARAM id)
	{
		m_dwItemId = id;
	}

	LPARAM DUIItemPanel::GetItemId()
	{
		return m_dwItemId;
	}

	void DUIItemPanel::OnFinalRelease()
	{
		AddRef();// 防止重复进入该函数
		DM_SendMessage(WM_DESTROY);
		__super::OnFinalRelease();
	}

	DMCode DUIItemPanel::OnSetCaptureWnd(DUIWND DUIWnd,DUIWNDPtr pOldDUIWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			iErr = m_pItemOwner->OnSetCapture(this);
			if (!DMSUCCEEDED(iErr))
			{
				break;
			}
			iErr = __super::OnSetCaptureWnd(DUIWnd,pOldDUIWnd);
		} while (false);
		return iErr;
	}

	DMCode DUIItemPanel::OnReleaseCaptureWnd()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			iErr = __super::OnReleaseCaptureWnd();
			if (!DMSUCCEEDED(iErr))
			{
				break;
			}
			iErr = m_pItemOwner->OnReleaseCapture(this);
		} while (false);
		return iErr;
	}

	DMCode DUIItemPanel::OnGetContainerRect(LPRECT lpRect)
	{
		return m_pItemOwner->OnGetPanelRect(this,lpRect);
	}

	// 重载处理--------------------------------------------------
	LRESULT DUIItemPanel::OnFrameEvent(UINT uMsg,WPARAM wp,LPARAM lp)
	{
		LRESULT lr = 0;
		AddRef();
		do 
		{
			bool bHandle = PreHandleFrameEvent(uMsg, wp, lp);
			SetMsgHandled(bHandle);
			lr = __super::OnFrameEvent(uMsg,wp,lp);
		} while (false);
		Release();
		return lr;
	}

	DMCode DUIItemPanel::OnFireEvent(DMEventArgs &Evt)
	{
		DMEventOfPanelArgs EvtOf(this, &Evt);
		EvtOf.m_szNameFrom = m_pDUIWnd->GetName();
		EvtOf.m_IdFrom     = m_pDUIWnd->GetID();
		return m_pDUIWnd->DV_FireEvent(EvtOf);
	}

	DMCode DUIItemPanel::OnGetDraw(IDMDraw** ppObj)
	{
		if (m_pDUIWnd && m_pDUIWnd->GetContainer())
		{
			return m_pDUIWnd->GetContainer()->OnGetDraw(ppObj);
			
		}
		return DM_ECODE_FAIL;
	}

	DMCode DUIItemPanel::OnGetCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas**ppCanvas)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcItem;
			if (!DMSUCCEEDED(OnGetContainerRect(rcItem)))
			{
				break;
			}
			CRect rcInvalid = lpRect;
			rcInvalid.OffsetRect(rcItem.TopLeft());
			IDMCanvas* pCanvas = m_pDUIWnd->DM_GetCanvas(rcInvalid, dcFlags);
			if (NULL==pCanvas)
			{
				break;
			}
			if (dcFlags&DMOLEDC_PAINTBKGND)
			{
				m_pDUIWnd->DM_SendMessage(WM_ERASEBKGND,(WPARAM)pCanvas);
			}
			pCanvas->OffsetViewportOrg(rcItem.left,rcItem.top);
			*ppCanvas = pCanvas;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIItemPanel::OnReleaseCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas*pCanvas)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcItem;
			if (!DMSUCCEEDED(OnGetContainerRect(rcItem)))
			{
				break;
			}
			pCanvas->OffsetViewportOrg(-rcItem.left,-rcItem.top);
			m_pDUIWnd->DM_ReleaseCanvas(pCanvas);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	HWND DUIItemPanel::OnGetHWnd()
	{
		return m_pDUIWnd->GetContainer()->OnGetHWnd();
	}

	DMCode DUIItemPanel::OnIsTranslucent()
	{
		return m_pDUIWnd->GetContainer()->OnIsTranslucent();
	}

	DMCode DUIItemPanel::OnUpdateWindow()
	{
		return m_pDUIWnd->GetContainer()->OnUpdateWindow();
	}

	DMCode DUIItemPanel::OnForceUpdateWindow()
	{
		return m_pDUIWnd->GetContainer()->OnForceUpdateWindow();
	}

	DMCode DUIItemPanel::OnUpdateRect(LPCRECT lpRect, DUIWND hDUIWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect
				||m_pDUIWnd->DM_IsUpdateLocked()
				||!m_pDUIWnd->DM_IsVisible(true))
			{
				break;
			}

			CRect rcItem;
			if (!DMSUCCEEDED(OnGetContainerRect(rcItem)))
			{
				break;
			}
			if (rcItem.IsRectEmpty())
			{
				break;
			}
			CRect rcInvalide = lpRect;
			rcInvalide.OffsetRect(rcItem.TopLeft());
			rcInvalide.IntersectRect(rcInvalide, rcItem);
			iErr = m_pDUIWnd->DM_InvalidateRect(rcInvalide);
		} while (false);
		return iErr;
	}

	DMCode DUIItemPanel::OnClientToScreen(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcItem;
			if (!DMSUCCEEDED(OnGetContainerRect(rcItem)))
			{
				break;
			}
			CRect rcDest = lpRect;
			m_pDUIWnd->GetContainer()->OnClientToScreen(rcDest);
			rcDest.OffsetRect(rcItem.TopLeft());
			*lpRect = rcDest;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIItemPanel::OnRegisterTimeline(IDMTimeline *pHandler)
	{
		return m_pDUIWnd->GetContainer()->OnRegisterTimeline(pHandler);
	}

	DMCode DUIItemPanel::OnUnregisterTimeline(IDMTimeline *pHandler)
	{
		return m_pDUIWnd->GetContainer()->OnUnregisterTimeline(pHandler);
	}

	DMCode DUIItemPanel::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CRect rcItem;
			if (!DMSUCCEEDED(OnGetContainerRect(rcItem)))
			{
				break;
			}
			if (m_hDUIHoverWnd==m_hDUIWnd)
			{
				iErr = DUIWindow::DV_OnUpdateToolTip(pt,tipInfo);
				break;
			}
			DUIWindow *pHover = g_pDMDWndPool->FindDUIWnd(m_hDUIHoverWnd);
			if (!pHover
				|| pHover->DM_IsDisable(true))
			{
				tipInfo.hDUIWnd = 0;
				break;
			}
			pt -= rcItem.TopLeft();
			iErr = pHover->DV_OnUpdateToolTip(pt,tipInfo);
			if (!DMSUCCEEDED(iErr))
			{
				break;
			}
			tipInfo.rcTarget.OffsetRect(rcItem.TopLeft());
		} while (false);
		return iErr;
	}

	// 辅助--------------------------------------------------------
	bool DUIItemPanel::PreHandleFrameEvent(UINT uMsg,WPARAM wp,LPARAM lp)
	{
		do 
		{
			if (WM_LBUTTONUP == uMsg)
			{
				DMEventOfPanelClickArgs Evt(this); 
				DV_FireEvent(Evt);
				break;
			}
			if (WM_RBUTTONUP == uMsg)
			{
				DMEventOfPanelRClickArgs Evt(this); 
				DV_FireEvent(Evt);
				break;
			}

			if (DM_IsDisable())
			{
				break;
			}

			bool bNeedState = false;
			DMSmartPtrT<IDMSkin>pSkin;
			m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
			if (pSkin)
			{
				int iStates = 1;
				pSkin->GetStates(iStates);
				if (iStates>1)
				{
					bNeedState = true;
				}
			}
			if (false == bNeedState)
			{
				break;
			}

			switch (uMsg)
			{
			case WM_MOUSEHOVER: 
				{
					DM_ModifyState(DUIWNDSTATE_Hover,0,true);
				}
				break;

			case WM_MOUSELEAVE: 
				{
					DM_ModifyState(0,DUIWNDSTATE_Hover,true);
				}
				break;
			}
		} while (false);
		return false;
	}

	CStringW DUIItemPanel::OnGetTransId()
	{
		return m_pDUIWnd->GetContainer()->OnGetTransId();
	}
}//namespace DM
