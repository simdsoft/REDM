#include "DmMainAfx.h"
#include "DUIListCtrlEx.h"

namespace DM
{

	DUIListCtrlEx::DUIListCtrlEx()
	{
		m_iSelItem       = -1;
		m_iHoverItem     = -1;
		m_iHeaderHei     = 20;
		m_iDefItemHei    = 20;
		m_crItemBg.SetTextInvalid();
		m_crItemSelBg.SetTextInvalid();
		m_bHotTrack      = false;
		m_pDUIXmlInfo->m_bFocusable = true;

		// listctrl
		DMADDEVENT(DMEventLCGetDispInfoArgs::EventID);
		DMADDEVENT(DMEventLCSelChangingArgs::EventID);
		DMADDEVENT(DMEventLCSelChangedArgs::EventID);
		DMADDEVENT(DMEventLCItemDeletedArgs::EventID);

	}

	DUIListCtrlEx::~DUIListCtrlEx()
	{
		DeleteAllItems(false);
	}

	int DUIListCtrlEx::InsertItem(int nIndex, DMXmlNode&XmlNode)
	{
		int iRet = -1;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}
			if (-1 == nIndex
				||nIndex>(int)GetCount())
			{
				nIndex = (int)GetCount();
			}

			LPLCITEMEX pNewItem = new LCITEMEX(XmlNode,this);
			CStringW strHei		= XmlNode.Attribute(DMAttr::DUIListCtrlExAttr::ITEM_height);
			pNewItem->nHeight   = m_iDefItemHei;
			DMAttributeDispatch::ParseInt(strHei,pNewItem->nHeight);
			CStringW strData    = XmlNode.Attribute(DMAttr::DUIListCtrlExAttr::ITEM_data);
			int iData = 0;
			DMAttributeDispatch::ParseInt(strData,iData);
			pNewItem->lParam    = (LPARAM)iData;

			ModifyPanelBgClr(pNewItem->pPanel,m_crItemBg);/// 默认背景色

			// 初始化布局
			CRect rcLayout(0,0,m_pHeaderCtrl->GetTotalWidth(),pNewItem->nHeight);
			pNewItem->pPanel->DM_FloatLayout(rcLayout);

			m_DMArray.InsertAt(nIndex,pNewItem);
			DM_AddChildPanel(pNewItem->pPanel);

			if (m_iSelItem>=nIndex) 
			{
				m_iSelItem++;
			}
			if (m_iHoverItem>=nIndex) 
			{
				m_iHoverItem++;
			}

			UpdateItemPanelId(nIndex,-1);

			// 更新滚动条
			SetLCScrollRange();
			iRet = nIndex;
		} while (false);
		return iRet;
	}

	int DUIListCtrlEx::GetTopIndex() const
	{
		LONG tmpY = m_ptCurPos.y;
		int nIndex = 0;
		int nCount = (int)m_DMArray.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			tmpY = tmpY - m_DMArray[i]->nHeight;
			if (tmpY >= 0)
			{
				nIndex++;
			}
			else
			{
				break;
			}
		}
		return nIndex;
	}

	int DUIListCtrlEx::SetTopIndex(int nIndex)
	{
		int iErr = LB_ERR ;
		do 
		{
			if (nIndex<0 || nIndex>=(int)GetCount())
			{
				break;
			}

			int nPos = 0;
			for (int i = 0; i < nIndex; i++)
			{
				nPos += m_DMArray[i]->nHeight;
			}

			OnScroll(true,SB_THUMBPOSITION, nPos);
			iErr = 0;
		} while (false);
		return iErr;
	}

	int DUIListCtrlEx::GetTotalHeight()
	{
		int nTotalHeight = 0;
		int iCount = (int)m_DMArray.GetCount();
		for (int i = 0; i<iCount; i++)
		{
			nTotalHeight += m_DMArray[i]->nHeight;
		}
		return nTotalHeight;
	}

	int DUIListCtrlEx::GetColumnCount()
	{
		return m_pHeaderCtrl->GetItemCount();
	}

	CRect DUIListCtrlEx::GetItemRect(int iItem)
	{
		CRect rcDest;
		do 
		{
			if (iItem<0
				||iItem>=(int)GetCount())
			{
				break;
			}
	
			int nPos = 0;
			for (int i = 0; i < iItem; i++)
			{
				nPos += m_DMArray[i]->nHeight;
			}
			CRect rcClient = GetListRect();
			CRect rcRet(CPoint(0,nPos),CSize(rcClient.Width(),m_DMArray[iItem]->nHeight));
			rcRet.OffsetRect(rcClient.TopLeft()-m_ptCurPos);
			rcDest = rcRet;
		} while (false);
		return rcDest;
	}

	bool DUIListCtrlEx::SetCurSel(int nIndex)
	{
		bool bRet = false;
		do 
		{
			if (nIndex>=(int)GetCount())
			{
				break;
			}

			if (nIndex<0)
			{
				nIndex = -1;
			}

			if (m_iSelItem==nIndex)
			{
				break;
			}
			DMEventLCSelChangingArgs Evt(this);
			Evt.m_nOldSel = m_iSelItem;
			Evt.m_nNewSel = nIndex;
			DV_FireEvent(Evt);

			if (Evt.m_bCancel)
			{
				break;
			}

			int nOldSel = m_iSelItem;
			m_iSelItem  = nIndex;
			if (-1 != nOldSel)
			{
				m_DMArray[nOldSel]->pPanel->ModifyState(0,DUIWNDSTATE_Check);
				ModifyPanelBgClr(m_DMArray[nOldSel]->pPanel,m_crItemBg);
				RedrawItem(nOldSel);
			}
			if (-1 != m_iSelItem)
			{
				m_DMArray[m_iSelItem]->pPanel->ModifyState(DUIWNDSTATE_Check,0);
				ModifyPanelBgClr(m_DMArray[m_iSelItem]->pPanel,m_crItemSelBg);
				RedrawItem(m_iSelItem);
			}

			DMEventLCSelChangedArgs evt2(this);
			evt2.m_nOldSel = nOldSel;
			evt2.m_nNewSel = m_iSelItem;
			DV_FireEvent(evt2);
			bRet = true;
		} while (false);
		return bRet;
	}

	int DUIListCtrlEx::GetCurSel() const
	{
		return m_iSelItem;
	}

	void DUIListCtrlEx::DeleteItem(int nIndex)
	{
		do 
		{
			if (nIndex<0 || nIndex>=(int)GetCount())
			{
				break;
			}

			if (m_pCapturePanel == m_DMArray[nIndex]->pPanel)
			{
				m_pCapturePanel.Release();
				DM_ReleaseCapture();
			}
			RemoveObj(nIndex);
			if (m_iSelItem==nIndex) 
			{
				m_iSelItem = -1;
			}
			else if (m_iSelItem>nIndex) 
			{
				m_iSelItem--;
			}

			if (m_iHoverItem==nIndex) 
			{
				m_iHoverItem = -1;
			}
			else if (m_iHoverItem>nIndex) 
			{
				m_iHoverItem--;
			}
			UpdateItemPanelId(nIndex,-1);
			SetLCScrollRange();
		} while (false);
	}

	void DUIListCtrlEx::DeleteAllItems(bool bUpdate /*= true*/)
	{
		OnReleaseCapture(m_pCapturePanel);// RemoveAll会delete所有的对象，所以m_pCapturePanel如果有值（正常为NULL）,要提前释放引用计数
		RemoveAll();
		m_iSelItem		 = -1;
		m_iHoverItem	 = -1;
		SetRangeSize(CSize(0,0));
		if (bUpdate)
		{
			DM_Invalidate();
		}
	}

	void DUIListCtrlEx::EnsureVisible(int nIndex)
	{
		do 
		{
			if (nIndex<0 || nIndex>=(int)GetCount())
			{
				break;
			}

			CRect rcClient = GetListRect();
			int nTargetY = 0;
			for (int i = 0; i < nIndex; i++)
			{//计算出目标顶部Y坐标
				nTargetY += m_DMArray[i]->nHeight;
			}
			if (nTargetY < m_ptCurPos.y || m_DMArray[nIndex]->nHeight >= rcClient.Height())
			{//目标顶部在可视范围上方，或者可视范围内只能显示目标项目，则把可视范围滚动到目标顶部
				OnScroll(true,SB_THUMBPOSITION,nTargetY);
				break;
			}
			nTargetY += m_DMArray[nIndex]->nHeight;
			if (nTargetY > m_ptCurPos.y + rcClient.Height())
			{//目标底部在可视范围下方，则往前回溯最后一个能完整显示的项目的顶坐标
				int nShowHeight = 0;
				for (int i = nIndex; i >= 0; i--)
				{
					nShowHeight += m_DMArray[i]->nHeight;
					if (nShowHeight > rcClient.Height())
					{
						break;
					}
					else
					{
						nTargetY -= m_DMArray[i]->nHeight;
					}
				}
				OnScroll(true,SB_THUMBPOSITION,nTargetY);
				break;
			}
		} while (false);
	}

	LPARAM DUIListCtrlEx::GetItemData(int nIndex)
	{
		if (nIndex<0 || nIndex >= (int)GetCount())
		{
			return 0;
		}

		return m_DMArray[nIndex]->lParam;
	}

	bool DUIListCtrlEx::SetItemData(int nIndex, LPARAM lParam)
	{
		bool bRet = false;
		do 
		{
			if (nIndex<0 || nIndex>=(int)GetCount())
			{
				break;
			}

			m_DMArray[nIndex]->lParam = lParam;
			bRet = true;
		} while (false);
		return bRet;
	}

	// 绘制
	void DUIListCtrlEx::DrawItem(IDMCanvas* pCanvas, CRect& rc, int iItem)
	{
		do 
		{
			if (!DM_IsVisible(true))
			{
				break;
			}

			if (iItem >= (int)GetCount())
			{
				break;
			}

			// 先更新所有子项的位置
			CRect rcCol(rc);
			rcCol.right = rcCol.left;// 第一项左边开始计算
			int iColCount  = GetColumnCount();
			for (int i=0; i<iColCount; i++)
			{
				DMHDITEM hdi;
				hdi.mask = DMHDI_WIDTH|DMHDI_ORDER;
				m_pHeaderCtrl->GetItem(i, &hdi);
				rcCol.left  = rcCol.right;
				rcCol.right = rcCol.left + hdi.cxy;
				CRect rcChild(rcCol);
				DUIWindow *pChild = m_DMArray[iItem]->pPanel->DM_GetWindow(GDW_FIRSTCHILD);
				int iIndex = 0;
				while (pChild)
				{
					if (iIndex == hdi.iOrder)
					{
						rcChild.OffsetRect(-rc.TopLeft());
						pChild->DM_FloatLayout(rcChild);
						break;
					}
					iIndex++;
					pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);// 只取兄弟窗口
				}
			}

			DMEventLCGetDispInfoArgs evt(this);
			evt.m_bHover = (iItem == m_iHoverItem);
			evt.m_bSel   = (iItem == m_iSelItem);
			evt.m_pItem  = m_DMArray[iItem]->pPanel;
			evt.m_iItem  = iItem;
			DV_FireEvent(evt);
			if (!m_bHotTrack)//不需要热追踪
			{
				m_DMArray[iItem]->pPanel->ModifyState(0,DUIWNDSTATE_Hover);
			}
			m_DMArray[iItem]->pPanel->DrawItem(pCanvas,rc);
		} while (false);
	}

	void DUIListCtrlEx::RedrawItem(int iItem)
	{
		do 
		{
			if (!DM_IsVisible(true))
			{
				break;
			}

			if (iItem >= (int)GetCount())
			{
				break;
			}

			CRect rcClient = GetListRect();
			CRect rcItem = GetItemRect(iItem);
			CRect rcInter;
			rcInter.IntersectRect(&rcClient,&rcItem);
			if (rcInter.IsRectEmpty())
			{
				break;
			}
			IDMCanvas* pCanvas = DM_GetCanvas(&rcInter,DMOLEDC_PAINTBKGND);
			if (NULL == pCanvas)
			{
				break;
			}
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas,DrawEnviron);
			DM_SendMessage(WM_ERASEBKGND,(WPARAM)(HDC)pCanvas);
			DrawItem(pCanvas,rcItem,iItem);
			DV_PopDrawEnviron(pCanvas,DrawEnviron);
			DM_ReleaseCanvas(pCanvas);
		} while (false);
	}

	// 
	DMCode DUIListCtrlEx::OnHeaderSizeChanging(DMEventArgs *pEvt)
	{
		SetLCScrollRange();
		CRect rcDraw;
		DUIWindow::DV_GetClientRect(&rcDraw);
		rcDraw.top += m_iHeaderHei;
		DM_InvalidateRect(rcDraw);
		return DM_ECODE_OK;
	}

	DMCode DUIListCtrlEx::OnHeaderSwap(DMEventArgs *pEvt)
	{
		DM_InvalidateRect(GetListRect());
		return DM_ECODE_OK;
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUIListCtrlEx::OnDestroy()
	{
		if (m_pHeaderCtrl)
		{
			m_pHeaderCtrl->m_EventMgr.UnSubscribeEvent(DM::DMEventHeaderItemChangingArgs::EventID, Subscriber(&DUIListCtrlEx::OnHeaderSizeChanging, this));
			m_pHeaderCtrl->m_EventMgr.UnSubscribeEvent(DM::DMEventHeaderItemSwapArgs::EventID, Subscriber(&DUIListCtrlEx::OnHeaderSwap, this));
		}
	
		DM_RemoveAllChildPanel();
		DeleteAllItems(false);
		__super::OnDestroy();
	}

	void DUIListCtrlEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		DUIWindow *pOwner = DM_GetWindow(GDW_OWNER);
		if (pOwner)
		{
			pOwner->DM_SendMessage(WM_CHAR, nChar, MAKELONG(nFlags, nRepCnt));
		}
		if (-1!=m_iSelItem)
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_CHAR,nChar, MAKELONG(nFlags, nRepCnt));
		}
	}

	void DUIListCtrlEx::OnSize(UINT nType,CSize size)
	{
		__super::OnSize(nType,size);
		UpdateHeaderCtrl();// 先更新header，再重设置panel，不然会出现panel小于header
		UpdateAllPanelLayout();
	}

	void DUIListCtrlEx::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			DUIDrawEnviron  DrawEnviron;
			DV_PushDrawEnviron(pCanvas,DrawEnviron);

			CRect rcList = GetListRect();
			pCanvas->PushClip(rcList);
			
			int nTotalHeight = 0;
			int iCount = (int)GetCount();
			for (int iItem = 0; iItem < iCount; iItem++)
			{
				if ((nTotalHeight >= m_ptCurPos.y && nTotalHeight < m_ptCurPos.y + rcList.Height())
					|| (nTotalHeight + m_DMArray[iItem]->nHeight >= m_ptCurPos.y && nTotalHeight + m_DMArray[iItem]->nHeight < m_ptCurPos.y + rcList.Height())
					|| (nTotalHeight <= m_ptCurPos.y && nTotalHeight + m_DMArray[iItem]->nHeight >= m_ptCurPos.y + rcList.Height())
					)
				{
					CRect rcItem(0,0,rcList.Width(),m_DMArray[iItem]->nHeight);
					rcItem.OffsetRect(-m_ptCurPos.x,nTotalHeight-m_ptCurPos.y);
					rcItem.OffsetRect(rcList.TopLeft());
					DrawItem(pCanvas,rcItem,iItem);
				}
				nTotalHeight += m_DMArray[iItem]->nHeight;
			}
			pCanvas->PopClip();
			DV_PopDrawEnviron(pCanvas,DrawEnviron);
		} while (false);
	}

	void DUIListCtrlEx::DM_OnSetFocus()
	{
		__super::DM_OnSetFocus();
		if (-1!=m_iSelItem) 
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_SETFOCUS,0,0);
		}
	}

	void DUIListCtrlEx::DM_OnKillFocus()
	{
		__super::DM_OnKillFocus();
		if (-1!=m_iSelItem) 
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_KILLFOCUS,0,0);
			RedrawItem(m_iSelItem);
		}
	}

	void DUIListCtrlEx::OnMouseLeave()
	{
		__super::OnMouseLeave();
		if (-1!=m_iHoverItem)
		{
			int nOldHover = m_iHoverItem;
			m_iHoverItem  = -1;
			m_DMArray[nOldHover]->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
		}
	}

	BOOL DUIListCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		return __super::OnMouseWheel(nFlags,zDelta,pt);
	}

	void DUIListCtrlEx::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (-1!=m_iSelItem)
			{
				m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_KEYDOWN, nChar, MAKELONG(nFlags, nRepCnt));
				if (m_DMArray[m_iSelItem]->pPanel->IsMsgHandled())// 如下拉框，内部处理了就不能下滚了
				{
					break;
				}
			}

			int  iNewSelItem = -1;
			DUIWindow *pOwner = DM_GetWindow(GDW_OWNER);
			if (pOwner 
				&&(VK_ESCAPE == nChar))
			{
				pOwner->DM_SendMessage(WM_KEYDOWN, nChar, MAKELONG(nFlags, nRepCnt));
				break;
			}

			if (VK_DOWN==nChar&&m_iSelItem<(int)GetCount()-1)
			{
				iNewSelItem = m_iSelItem+1;
			}
			else if (VK_UP==nChar&&m_iSelItem>0)
			{
				iNewSelItem = m_iSelItem-1;
			}
			else if (VK_RETURN==nChar&&pOwner)                     
			{
				iNewSelItem = m_iSelItem;
			}

			if (-1!=iNewSelItem)
			{
				EnsureVisible(iNewSelItem);
				SetCurSel(iNewSelItem);
			}
		} while (false);
	}


	LRESULT DUIListCtrlEx::OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		LRESULT lRet=__super::OnNcCalcSize(bCalcValidRects,lParam);
		UpdateHeaderCtrl();
		UpdateAllPanelLayout();
		return lRet;
	}

	LRESULT DUIListCtrlEx::OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{

			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			if (WM_LBUTTONUP == uMsg
				&& m_iHoverItem != m_iSelItem)
			{
				SetCurSel(m_iHoverItem);
			}
			if (m_pCapturePanel)
			{
				CRect rcItem;
				m_pCapturePanel->OnGetContainerRect(rcItem);
				pt.Offset(-rcItem.TopLeft());///< 转换成面板坐标
				lRet = m_pCapturePanel->OnFrameEvent(uMsg,wParam,MAKELPARAM(pt.x,pt.y));
				break;
			}

			if (m_pDUIXmlInfo->m_bFocusable
				&& (uMsg==WM_LBUTTONDOWN || uMsg== WM_RBUTTONDOWN || uMsg==WM_LBUTTONDBLCLK))
			{
				DV_SetFocusWnd();
			}

			int iHoverItem = HitTest(pt);
			if (iHoverItem != m_iHoverItem)
			{
				int iOldHoverItem = m_iHoverItem;
				m_iHoverItem  = iHoverItem;
				if (-1!=iOldHoverItem)
				{
					RedrawItem(iOldHoverItem);
					m_DMArray[iOldHoverItem]->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
				}

				if (-1!=m_iHoverItem)
				{
					RedrawItem(m_iHoverItem);
					m_DMArray[m_iHoverItem]->pPanel->OnFrameEvent(WM_MOUSEHOVER,wParam,MAKELPARAM(pt.x,pt.y));
				}
			}
			if (WM_LBUTTONDOWN == uMsg&& -1!=m_iSelItem
				&& m_iSelItem != m_iHoverItem )
			{///原有行失去焦点
				m_DMArray[m_iSelItem]->pPanel->m_FocusMgr.SetFocusedWnd(NULL);
			}

			if (-1!=m_iHoverItem)
			{
				m_DMArray[m_iHoverItem]->pPanel->OnFrameEvent(uMsg,wParam,MAKELPARAM(pt.x,pt.y));
			}
		} while (false);
		return lRet;
	}

	LRESULT DUIListCtrlEx::OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			if (m_pCapturePanel)
			{
				lRet = m_pCapturePanel->OnFrameEvent(uMsg,wParam,lParam);
				SetMsgHandled(m_pCapturePanel->IsMsgHandled());
			}
			else if (-1!=m_iSelItem)
			{
				lRet = m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(uMsg,wParam,lParam);
				SetMsgHandled(m_DMArray[m_iSelItem]->pPanel->IsMsgHandled());
			}
			else
			{
				SetMsgHandled(FALSE);
			}
		} while (false);
		return lRet;
	}

	//---------------------------------------------------
	// Function Des: IDMItemPanelOwner实现
	DUIWindow* DUIListCtrlEx::GetOwnerWindow()
	{
		return this;
	}

	DMCode DUIListCtrlEx::OnSetCapture(DUIItemPanel* pPanel)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pPanel)
			{
				break;
			}
			iErr = DM_SetCapture();
			m_pCapturePanel = pPanel;
		} while (false);
		return iErr;
	}

	DMCode DUIListCtrlEx::OnReleaseCapture(DUIItemPanel* pPanel)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pPanel
				||m_pCapturePanel!=pPanel)
			{
				break;
			}

			iErr = DM_ReleaseCapture();
			m_pCapturePanel.Release();
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIListCtrlEx::OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pPanel
				||-1 == pPanel->GetItemId())
			{
				break;
			}
			int iItem = pPanel->GetItemId();
			*lpRect = GetItemRect(iItem);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des: 可重载函数
	DMCode DUIListCtrlEx::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}

			if (!DMSUCCEEDED(__super::DV_CreateChildWnds(XmlNode)))
			{// 无论
				break;
			}
			DUIWindow *pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{// 查找HeaderCtrl
				if(pChild->IsClass(DUIHeaderCtrl::GetClassName()))
				{
					m_pHeaderCtrl = (DUIHeaderCtrl*)pChild;
					break;// 跳出内循环
				}
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}
			if (NULL == m_pHeaderCtrl)
			{
				DMASSERT_EXPR(0,L"listctrlex必须有headctrl");
				break;
			}

			CStringW strPos;
			strPos.Format(L"0,0,-0,%d",m_iHeaderHei);
			m_pHeaderCtrl->SetAttribute(L"pos",strPos,true);

			// 设置接收header消息
			m_pHeaderCtrl->m_EventMgr.SubscribeEvent(DM::DMEventHeaderItemChangingArgs::EventID, Subscriber(&DUIListCtrlEx::OnHeaderSizeChanging, this));
			m_pHeaderCtrl->m_EventMgr.SubscribeEvent(DM::DMEventHeaderItemSwapArgs::EventID, Subscriber(&DUIListCtrlEx::OnHeaderSwap, this));

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::DUIListCtrlExAttr::NODE_item);
			while (XmlItem.IsValid())
			{
				InsertItem(-1,XmlItem);
				XmlItem = XmlItem.NextSibling();
			}
			int nSelItem = -1;
			DMAttributeDispatch::ParseInt(XmlNode.Attribute(DMAttr::DUIListCtrlExAttr::INT_cursel),nSelItem);
			SetCurSel(nSelItem);
		} while (false);
		return iErr;
	}

	DMCode DUIListCtrlEx::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (-1==m_iHoverItem)
			{
				iErr = __super::DV_OnUpdateToolTip(pt, tipInfo);
				break;
			}
			iErr = m_DMArray[m_iHoverItem]->pPanel->DV_OnUpdateToolTip(pt, tipInfo);
		} while (false);
		return iErr;
	}

	DMCode DUIListCtrlEx::DV_OnSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pCapturePanel)
			{
				CRect rcItem;
				m_pCapturePanel->OnGetContainerRect(rcItem);
				if (0!=m_pCapturePanel->OnFrameEvent(WM_SETCURSOR, 0, MAKELPARAM(pt.x-rcItem.left,pt.y-rcItem.top))) 
				{
					iErr = DM_ECODE_OK;
					break;
				}
			}

			if (-1!=m_iHoverItem)
			{
				CRect rcItem = GetItemRect(m_iHoverItem);
				if (0!=m_DMArray[m_iHoverItem]->pPanel->OnFrameEvent(WM_SETCURSOR, 0, MAKELPARAM(pt.x-rcItem.left,pt.y-rcItem.top))) 
				{
					iErr = DM_ECODE_OK;
					break;
				}
			}
			iErr = __super::DV_OnSetCursor(pt);
		} while (false);
		return iErr;
	}

	void DUIListCtrlEx::OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew)
	{
		do 
		{
			if (-1==m_iSelItem)
			{
				break;
			}
			if (DM_IsFocusWnd())
			{
				m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_KILLFOCUS,0,0);
				m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_SETFOCUS,0,0);
			}
		} while (false);
	}

	bool DUIListCtrlEx::OnScroll(bool bVert,UINT uCode,int nPos)
	{
		 bool bRet = __super::OnScroll(bVert, uCode, nPos);
		 if (!bVert)
		 {
			 //  处理列头滚动
			 UpdateHeaderCtrl();
			 if (SB_THUMBTRACK == uCode)
			 {
				 ScrollUpdateWindow();//在header调整位置后再实时更新一次，不然由于在调整前实时更新了，header会比list滚动慢
			 }
		 }
		 return bRet;
	}

	//
	void DUIListCtrlEx::PreArrayObjRemove(const LPLCITEMEX &obj)
	{
		delete obj;
	}


	DMCode DUIListCtrlEx::OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr)
	{
		do 
		{
			if (DM_ECODE_NOXMLLOADREFRESH == iErr)
			{
				if (false == bLoadXml)
				{
					if (0 == _wcsicmp(DMAttr::DUIListCtrlExAttr::COLOR_clritembg,pszAttribute))
					{
						int nCount = (int)GetCount();
						for (int i=0; i<nCount; i++)
						{
							if (i!=m_iSelItem)
							{
								ModifyPanelBgClr(m_DMArray[i]->pPanel,m_crItemBg);
							}
						}
						DM_InvalidateRect(m_rcWindow);// 处理非客户区
					}

					if (0 == _wcsicmp(DMAttr::DUIListCtrlExAttr::COLOR_clritemselbg,pszAttribute))
					{
						if (-1!=m_iSelItem&&m_iSelItem<(int)GetCount())
						{
							ModifyPanelBgClr(m_DMArray[m_iSelItem]->pPanel,m_crItemSelBg);
						}
						DM_InvalidateRect(m_rcWindow);// 处理非客户区
					}
				}
				iErr = DM_ECODE_OK;/// 已处理
				break;
			}
			__super::OnAttributeFinished(pszAttribute,pszValue,bLoadXml,iErr);

		} while (false);
		return iErr;
	}

	// 辅助
	void DUIListCtrlEx::UpdateItemPanelId(int iFirst/*=0*/, int iLast/* = -1*/)
	{
		int iCount  = (int)GetCount();
		if (-1 == iLast)
		{
			iLast = iCount;
		}
		for (int i=iFirst;i<iCount&&i<iLast;i++)
		{
			m_DMArray[i]->pPanel->SetItemId((LPARAM)i);
		}
	}

	void DUIListCtrlEx::UpdateAllPanelLayout()
	{
		int iCount  = (int)GetCount();
		CRect rcHeader;
		m_pHeaderCtrl->DV_GetClientRect(rcHeader);
		int iWid = rcHeader.Width();
		if (iWid<m_pHeaderCtrl->GetTotalWidth())
		{
			iWid = m_pHeaderCtrl->GetTotalWidth();
		}
		for (int i=0;i<iCount;i++)
		{
			CRect rcLayout(0,0,iWid,m_DMArray[i]->nHeight);
			m_DMArray[i]->pPanel->DM_FloatLayout(rcLayout);
		}
	}

	void DUIListCtrlEx::ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr)
	{
		do 
		{
			if (NULL == pPanel)
			{
				break;
			}
			if (Clr.IsTextInvalid())
			{
				break;
			}
			CStringW strClr;
			strClr.Format(L"rgba(%02x,%02x,%02x,%02x)",Clr.r,Clr.g,Clr.b,Clr.a);
			pPanel->m_pDUIXmlInfo->m_pStyle->SetAttribute(L"clrbg",strClr,false);
		} while (false);
	}

	void DUIListCtrlEx::UpdateHeaderCtrl()
	{
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		CRect rcHeader(rcClient);
		rcHeader.bottom = rcHeader.top+m_iHeaderHei;
		rcHeader.left -= m_ptCurPos.x;
		if (m_pHeaderCtrl)
		{
			m_pHeaderCtrl->DM_FloatLayout(rcHeader);
		}
	}

	void DUIListCtrlEx::SetLCScrollRange()
	{
		CRect rcClient;
		DUIWindow::DV_GetClientRect(&rcClient);
		int iTotalHeight = GetTotalHeight();
		int iTotalWidth  = m_pHeaderCtrl->GetTotalWidth();
		CSize szView(iTotalWidth, iTotalHeight);
		SetRangeSize(szView);
	}

	CRect DUIListCtrlEx::GetListRect()
	{
		CRect rcList;
		DV_GetClientRect(&rcList);
		rcList.top += m_iHeaderHei;
		CRect rcHead;
		m_pHeaderCtrl->DV_GetClientRect(&rcHead);
		rcList.right = rcList.left + rcHead.Width();
		return rcList;
	}

	int DUIListCtrlEx::HitTest(CPoint &pt)
	{
		int iRet = -1;
		do 
		{
			CRect rcList = GetListRect();
			if (!rcList.PtInRect(pt))
			{
				break;
			}
			CPoint pt2 = pt;
			pt2.y -= rcList.top-m_ptCurPos.y;
			int nTotalHeight = 0;
			int iCount = (int)GetCount();
			for (int i = 0; i < iCount; i++)
			{
				nTotalHeight += m_DMArray[i]->nHeight;
				if (pt2.y < nTotalHeight)
				{
					iRet = i;
					pt.x -= rcList.left - m_ptCurPos.x;
					pt.y = pt2.y - nTotalHeight + m_DMArray[i]->nHeight;
					break;
				}
			}
		} while (false);
		return iRet;
	}

	DMCode DUIListCtrlEx::OnAttributeHeaderHei(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int iTemp = m_iHeaderHei;
			dm_parseint(lpszValue,m_iHeaderHei);
			if (iTemp == m_iHeaderHei)
			{
				break;
			}
			if (!bLoadXml)
			{
				if (m_pHeaderCtrl)
				{
					CStringW strPos;
					strPos.Format(L"0,0,-0,%d",m_iHeaderHei);
					m_pHeaderCtrl->SetAttribute(L"pos",strPos,true);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIListCtrlEx::OnAttributeCurSel(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!bLoadXml)
			{
				int iSelItem = -1;
				dm_parseint(lpszValue,iSelItem);
				SetCurSel(iSelItem);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

}//namespace DM