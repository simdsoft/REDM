#include "TGPDemoAfx.h"
#include "TGPListBoxEx.h"

namespace DM
{
	TGPListBoxEx::TGPListBoxEx()
	{
		m_iSelItem       = -1;
		m_iHoverItem     = -1;
		m_iDefItemHei    = 20;
		m_bNoDrawBg      = false;
		
		m_bSwapLine      = true;
		m_bDragging      = false;
		m_dwHitTest      = -1;
		m_dwDragTo       = -1;
		m_crItemBg.SetTextInvalid();
		m_crItemSelBg.SetTextInvalid();

		m_pDUIXmlInfo->m_bFocusable = true;
 
		// listbox
		DMADDEVENT(DMEventLBSelChangingArgs::EventID);
		DMADDEVENT(DMEventLBSelChangedArgs::EventID);
		DMADDEVENT(DMEventLBGetDispInfoArgs::EventID);
		DMADDEVENT(DMTgpListCloseUpEventCmdArgs::EventID);

	}

	TGPListBoxEx::~TGPListBoxEx()
	{
		DeleteAllItems(false);
	}

	int TGPListBoxEx::InsertItem(int nIndex, DMXmlNode&XmlNode)
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

			LPTGPLBITEMEX pNewItem = new TGPLBITEMEX(XmlNode,this);
			LPCSTR strHei		= XmlNode.Attribute(DMAttr::TGPListBoxExAttr::ITEM_height);
			pNewItem->nHeight   = m_iDefItemHei;
			DMAttributeDispatch::ParseInt(strHei,pNewItem->nHeight);
			LPCSTR strData    = XmlNode.Attribute(DMAttr::TGPListBoxExAttr::ITEM_data);
			int iData = 0;
			DMAttributeDispatch::ParseInt(strData,iData);
			pNewItem->lParam    = (LPARAM)iData;
			ModifyPanelBgClr(pNewItem->pPanel,m_crItemBg);/// Ĭ�ϱ���ɫ

			// ��ʼ������
			CRect rcLayout(0,0,m_rcsbClient.Width(),pNewItem->nHeight);
			pNewItem->pPanel->DM_FloatLayout(rcLayout);

			pNewItem->XmlDoc.ResetByXmlNode(XmlNode);//����XmlNode
		
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

			// ���¹�����
			UpdateScrollRangeSize();
			iRet = nIndex;
		} while (false);
		return iRet;
	}

	int TGPListBoxEx::GetItemHeight(int nIndex) const
	{
		if (nIndex < 0 || nIndex >= (int)m_DMArray.GetCount())
			return LB_ERR;

		return m_DMArray[nIndex]->nHeight;
	} 

	int TGPListBoxEx::GetAllItemHeight()
	{
		int nTotalHeight = 0;
		int iCount = (int)m_DMArray.GetCount();
		for (int i = 0; i<iCount; i++)
		{
			nTotalHeight += m_DMArray[i]->nHeight;
		}
		return nTotalHeight;
	}

	int TGPListBoxEx::SetAllItemHeight(int cyItemHeight, bool bUpdate)
	{
		int iCount = (int)m_DMArray.GetCount();
		for (int i = 0; i<iCount; i++)
		{
			SetItemHeight(i,cyItemHeight,bUpdate);
		}
		return 0;
	}

	int TGPListBoxEx::SetItemHeight(int nIndex, int cyItemHeight, bool bUpdate)
	{
		if (cyItemHeight < 0 || nIndex < 0 || nIndex >= (int)GetCount())
			return LB_ERR;
		if (cyItemHeight==m_DMArray[nIndex]->nHeight)
		{
			return 0;
		}
		m_DMArray[nIndex]->nHeight = cyItemHeight;
		if (bUpdate)
		{
			UpdateScrollRangeSize();
		}

		return 0;
	}

	int TGPListBoxEx::GetTopIndex() const
	{
		LONG tmpY = m_ptCurPos.y;
		int nIndex = 0;
		for (int i = 0; i < (int)m_DMArray.GetCount(); i++)
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

	int TGPListBoxEx::SetTopIndex(int nIndex)
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

	CRect TGPListBoxEx::GetItemRect(int iItem)
	{
		CRect rcDest;
		do 
		{
			if (iItem<0
				||iItem>=(int)GetCount())
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(&rcClient);
			int nPos = 0;
		
			for (int i = 0; i < iItem; i++)
			{
				nPos += m_DMArray[i]->nHeight;
			}
			rcDest.SetRect(0,nPos,rcClient.Width(), m_DMArray[iItem]->nHeight+nPos);
			rcDest.OffsetRect(rcClient.TopLeft()-m_ptCurPos);

		} while (false);
		return rcDest;
	}

	bool TGPListBoxEx::SetCurSel(int nIndex)
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
			DMEventLBSelChangingArgs Evt(this);
			Evt.m_nOldSel = m_iSelItem;
			Evt.m_nNewSel = nIndex;
			DV_FireEvent(Evt);

			if (Evt.m_bCancel)
			{
				break;
			}
			int nOldSel = m_iSelItem;
			m_iSelItem  = nIndex;
			// TODO.���Ҫ����ѡ����ı���ɫ�����Դ򿪴˴���
			//if (-1 != nOldSel)
			//{
			//	m_DMArray[nOldSel]->pPanel->ModifyState(0,DUIWNDSTATE_Check);
			//	ModifyPanelBgClr(m_DMArray[nOldSel]->pPanel,m_crItemBg);
			//	RedrawItem(nOldSel);
			//}
			//if (-1 != m_iSelItem)
			//{
			//	m_DMArray[m_iSelItem]->pPanel->ModifyState(DUIWNDSTATE_Check,0);
			//	ModifyPanelBgClr(m_DMArray[m_iSelItem]->pPanel,m_crItemSelBg);
			//	RedrawItem(m_iSelItem);
			//}

			DMEventLBSelChangedArgs evt2(this);
			evt2.m_nOldSel = nOldSel;
			evt2.m_nNewSel = m_iSelItem;
			DV_FireEvent(evt2);

			bRet = true;
		} while (false);
		return bRet;
	}

	int TGPListBoxEx::GetCurSel() const
	{
		return m_iSelItem;
	}

	void TGPListBoxEx::DeleteItem(int nIndex)
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
			DM_RemoveChildPanel(m_DMArray[nIndex]->pPanel);
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

			UpdateScrollRangeSize();
		} while (false);
	}

	void TGPListBoxEx::DeleteAllItems(bool bUpdate /*= true*/)
	{
		DM_RemoveAllChildPanel();
		OnReleaseCapture(m_pCapturePanel);
		RemoveAll();
		m_iSelItem		 = -1;
		m_iHoverItem	 = -1;
		if (bUpdate)
		{
			SetRangeSize(CSize(0,0));
			DM_Invalidate();
		}
	}

	void TGPListBoxEx::EnsureVisible(int nIndex)
	{
		do 
		{
			if (nIndex<0 || nIndex>=(int)GetCount())
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(&rcClient);

			int nTargetY = 0;
			for (int i = 0; i < nIndex; i++)
			{//�����Ŀ�궥��Y����
				nTargetY += m_DMArray[i]->nHeight;
			}
			if (nTargetY < m_ptCurPos.y || m_DMArray[nIndex]->nHeight >= rcClient.Height())
			{//Ŀ�궥���ڿ��ӷ�Χ�Ϸ������߿��ӷ�Χ��ֻ����ʾĿ����Ŀ����ѿ��ӷ�Χ������Ŀ�궥��
				OnScroll(true,SB_THUMBPOSITION,nTargetY);
				break;
			}
			nTargetY += m_DMArray[nIndex]->nHeight;
			if (nTargetY > m_ptCurPos.y + rcClient.Height())
			{//Ŀ��ײ��ڿ��ӷ�Χ�·�������ǰ�������һ����������ʾ����Ŀ�Ķ�����
				int nShowHeight = 0;
				for ( int i = nIndex; i >= 0; i--)
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

	// ����
	void TGPListBoxEx::DrawItem(IDMCanvas* pCanvas, CRect& rc, int iItem)
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
			DMEventLBGetDispInfoArgs evt(this);
			evt.m_bHover = (iItem == m_iHoverItem);
			evt.m_bSel   = (iItem == m_iSelItem);
			evt.m_pItem  = m_DMArray[iItem]->pPanel;
			evt.m_iItem  = iItem;
			DV_FireEvent(evt);
			m_DMArray[iItem]->pPanel->DrawItem(pCanvas,rc);

		} while (false);
	}

	void TGPListBoxEx::RedrawItem(int iItem)
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

			int nTargetY = 0;
			for (int i = 0; i < iItem; i++)
			{
				nTargetY += m_DMArray[i]->nHeight;
			}
			CRect rcClient;
			DV_GetClientRect(&rcClient);
			if ((nTargetY >= m_ptCurPos.y && nTargetY <= m_ptCurPos.y + rcClient.Height())
				||(nTargetY + m_DMArray[iItem]->nHeight >= m_ptCurPos.y && nTargetY + m_DMArray[iItem]->nHeight <= m_ptCurPos.y + rcClient.Height())
				||(nTargetY <= m_ptCurPos.y && nTargetY + m_DMArray[iItem]->nHeight >= m_ptCurPos.y + rcClient.Height()))
			{//Ŀ���в����ڿ��ӷ�Χ��
				CRect rcItem(0,0,rcClient.Width(),m_DMArray[iItem]->nHeight);
				rcItem.OffsetRect(0,nTargetY-m_ptCurPos.y);// ��ʹ��һ��item����ֻ����һ����
				rcItem.OffsetRect(rcClient.TopLeft());
				IDMCanvas* pCanvas = DM_GetCanvas(&rcItem,m_bNoDrawBg?DMOLEDC_NODRAW:DMOLEDC_PAINTBKGND);// todo.��ʱ��ץ����
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
				
			}
		} while (false);
	}

	//---------------------------------------------------
	// Function Des: DUI����Ϣ�ַ�ϵ�к���
	void TGPListBoxEx::OnDestroy()
	{
		DeleteAllItems(false);
		__super::OnDestroy();
	}
	void TGPListBoxEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	void TGPListBoxEx::OnSize(UINT nType,CSize size)
	{
		__super::OnSize(nType,size);
		UpdateAllPanelLayout();
	}

	void TGPListBoxEx::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			DUIDrawEnviron  DrawEnviron;
			DV_PushDrawEnviron(pCanvas,DrawEnviron);

			CRect rcClient;
			DV_GetClientRect(&rcClient);
			int nTotalHeight = 0;
			int iCount = (int)GetCount();
			for (int iItem = 0; iItem < iCount; iItem++)
			{
				if ((nTotalHeight >= m_ptCurPos.y && nTotalHeight < m_ptCurPos.y + rcClient.Height())
					|| (nTotalHeight + m_DMArray[iItem]->nHeight >= m_ptCurPos.y && nTotalHeight + m_DMArray[iItem]->nHeight < m_ptCurPos.y + m_rcsbClient.Height())
					|| (nTotalHeight <= m_ptCurPos.y && nTotalHeight + m_DMArray[iItem]->nHeight >= m_ptCurPos.y + m_rcsbClient.Height())
					)
				{
					CRect rcItem(0,0,rcClient.Width(),m_DMArray[iItem]->nHeight);
					rcItem.OffsetRect(0,nTotalHeight-m_ptCurPos.y);
					rcItem.OffsetRect(rcClient.TopLeft());
					DrawItem(pCanvas,rcItem,iItem);
				}
				nTotalHeight += m_DMArray[iItem]->nHeight;
			}

			DV_PopDrawEnviron(pCanvas,DrawEnviron);
		} while (false);
	}

	void TGPListBoxEx::DM_OnSetFocus()
	{
		__super::DM_OnSetFocus();
		if (-1!=m_iSelItem) 
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_SETFOCUS,0,0);
		}
	}

	void TGPListBoxEx::DM_OnKillFocus()
	{
		__super::DM_OnKillFocus();
		if (-1!=m_iSelItem) 
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_KILLFOCUS,0,0);
			RedrawItem(m_iSelItem);
		}
	}

	void TGPListBoxEx::OnMouseLeave()
	{
		if (DM_IsVisible(true))// ��һ���жϣ���ֹ�ǿͻ�����list����ʱ����
		{
			__super::OnMouseLeave();
			if (-1!=m_iHoverItem)
			{
				int nOldHover = m_iHoverItem;
				m_iHoverItem  = -1;
				m_DMArray[nOldHover]->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
			}
		}
	}

	void TGPListBoxEx::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		DM_SetCapture();
		SetMsgHandled(FALSE);
	}

	void TGPListBoxEx::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		DM_ReleaseCapture();
		DMTgpListCloseUpEventCmdArgs Evt(this);
		DV_FireEvent(Evt);
		SetMsgHandled(FALSE);
	}

	BOOL TGPListBoxEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		return __super::OnMouseWheel(nFlags,zDelta,pt);
	}

	void TGPListBoxEx::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (-1!=m_iSelItem)
			{
				m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_KEYDOWN, nChar, MAKELONG(nFlags, nRepCnt));
				if (m_DMArray[m_iSelItem]->pPanel->IsMsgHandled())
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

	LRESULT TGPListBoxEx::OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		LRESULT lRet=__super::OnNcCalcSize(bCalcValidRects,lParam);
		UpdateAllPanelLayout();
		return lRet;
	}

	LRESULT TGPListBoxEx::OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			// �����϶��ж�
			if (m_bSwapLine)
			{
				if (WM_LBUTTONDOWN == uMsg)
				{
					m_ptClick   = pt;
					CPoint pt2 = pt;
					m_dwHitTest = HitTest(pt2);// pt����HitTest���Զ��ı�
				}
				else if (WM_MOUSEMOVE == uMsg)
				{
					if (m_bDragging || wParam&MK_LBUTTON)
					{
						if (!m_bDragging)
						{
							m_bDragging = true;      /// ��ʼ�ƶ�
							if (-1!=m_dwHitTest) 
							{ 
								m_dwDragTo    = m_dwHitTest;
								CRect rcItem  = GetItemRect(m_dwHitTest);
								CreateDragCanvas(m_dwHitTest);//  �����϶�ʱ�Ļ���
								CPoint pt     = m_ptClick-rcItem.TopLeft();
								DMDragWnd::DragBegin(m_pDragCanvas,pt,0,200,LWA_ALPHA|LWA_COLORKEY);
							}
						}
						if (-1!=m_dwHitTest)  
						{
							CPoint pt0 = pt; 
							DWORD dwDragTo = HitTest(pt0);
						
							// �����϶���Χ
							CRect rcItem  = GetItemRect(m_dwHitTest);
							CPoint pt2(pt.x,pt.y);
							pt2.x = m_ptClick.x;// ǿ�Ƽ�ֱ�߶���
							if (pt2.y<m_rcWindow.top+DMDragWnd::ms_pCurDragWnd->m_ptHot.y)// ǿ�Ʋ����ϳ�listboxex�Ķ���
							{
								pt2.y = m_rcWindow.top+DMDragWnd::ms_pCurDragWnd->m_ptHot.y;
							}
							if (pt.y+rcItem.Height()>m_rcWindow.bottom+DMDragWnd::ms_pCurDragWnd->m_ptHot.y)// ǿ�Ʋ����ϳ�listboxex�ĵײ�
							{
								pt2.y = m_rcWindow.bottom+DMDragWnd::ms_pCurDragWnd->m_ptHot.y-rcItem.Height();
							}
							ClientToScreen(GetContainer()->OnGetHWnd(),&pt2);
							if (m_dwDragTo != dwDragTo)
							{
								m_dwDragTo = dwDragTo;
								DrawDraggingState(dwDragTo);// �Զ�����itemλ��
							}
							DMDragWnd::DragMove(pt2);
						}

					}
				}
				else if (WM_LBUTTONUP == uMsg)
				{
					DMDragWnd::DragEnd();
					m_pDragCanvas.Release();

					if (m_dwDragTo!=m_dwHitTest
						&&-1!=m_dwDragTo)
					{
						LPTGPLBITEMEX  pItem = m_DMArray[m_dwHitTest];
						m_DMArray.RemoveAt(m_dwHitTest);
						m_DMArray.InsertAt(m_dwDragTo,pItem);
						UpdateItemPanelId();// ������Panel��ΨһID
					}

					CPoint pt2 = pt;
					m_dwHitTest = HitTest(pt2);
					m_dwDragTo  = -1;
					m_bDragging = false;
					DM_Invalidate();
				}
			}
			if (m_bDragging)
			{
				m_iHoverItem = m_dwHitTest;
				break;
			}

			if (WM_LBUTTONUP == uMsg
				&& m_iHoverItem != m_iSelItem)
			{
				SetCurSel(m_iHoverItem);
			}
			if (m_pCapturePanel)
			{
				CRect rcItem;
				m_pCapturePanel->OnGetContainerRect(rcItem);
				pt.Offset(-rcItem.TopLeft());///< ת�����������
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
			{///ԭ����ʧȥ����
				m_DMArray[m_iSelItem]->pPanel->m_FocusMgr.SetFocusedWnd(NULL);
			}

			if (-1!=m_iHoverItem)
			{
				m_DMArray[m_iHoverItem]->pPanel->OnFrameEvent(uMsg,wParam,MAKELPARAM(pt.x,pt.y));
			}
		} while (false);
		return lRet;
	}

	LRESULT TGPListBoxEx::OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
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
	// Function Des: IDMItemPanelOwnerʵ��
	DUIWindow* TGPListBoxEx::GetOwnerWindow()
	{
		return this;
	}

	DMCode TGPListBoxEx::OnSetCapture(DUIItemPanel* pPanel)
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

	DMCode TGPListBoxEx::OnReleaseCapture(DUIItemPanel* pPanel)
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

	DMCode TGPListBoxEx::OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pPanel
				||-1==pPanel->GetItemId())
			{
				break;
			}
			int iItem = (int)pPanel->GetItemId();
			*lpRect = GetItemRect(iItem);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	//---------------------------------------------------
	// Function Des: �����غ���
	DMCode TGPListBoxEx::DV_CreateChildWnds(const DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::TGPListBoxExAttr::NODE_item);
			while (XmlItem.IsValid())
			{
				InsertItem(-1,XmlItem);
				XmlItem = XmlItem.NextSibling();
			}
			int nSelItem = -1;
			DMAttributeDispatch::ParseInt(XmlNode.Attribute(DMAttr::TGPListBoxExAttr::INT_cursel),nSelItem);
			SetCurSel(nSelItem);
		} while (false);
		return iErr;
	}

	DMCode TGPListBoxEx::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
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

	DMCode TGPListBoxEx::DV_OnSetCursor(const CPoint &pt)
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

	void TGPListBoxEx::OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew)
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

	// 
	void TGPListBoxEx::PreArrayObjRemove(const LPTGPLBITEMEX &obj)
	{
		delete obj;
	}

	void TGPListBoxEx::UpdateItemPanelId(int iFirst/*=0*/, int iLast/* = -1*/)
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

	void TGPListBoxEx::UpdateAllPanelLayout()
	{
		int iCount  = (int)GetCount();
		for (int i=0;i<iCount;i++)
		{
			CRect rcLayout(0,0,m_rcsbClient.Width(),m_DMArray[i]->nHeight);
			m_DMArray[i]->pPanel->DM_FloatLayout(rcLayout);
		}
	}

	void TGPListBoxEx::ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr)
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
			CStringA strClr;
			strClr.Format("rgba(%02x,%02x,%02x,%02x)",Clr.r,Clr.g,Clr.b,Clr.a);
			pPanel->m_pDUIXmlInfo->m_pStyle->SetAttribute("clrbg",strClr,false);
		} while (false);
	}

	int TGPListBoxEx::HitTest(CPoint &pt)
	{
		int iRet = -1;
		do 
		{
			CRect rcClient;
			DV_GetClientRect(&rcClient);
			if (NULL == m_pCapturePanel&&!rcClient.PtInRect(pt))
			{
				break;
			}

			CPoint pt2 = pt;
			pt2.y -= rcClient.top-m_ptCurPos.y;
			int nTotalHeight = 0;
			int iCount = (int)GetCount();
			for (int i = 0; i < iCount; i++)
			{
				nTotalHeight += m_DMArray[i]->nHeight;
				if (pt2.y < nTotalHeight)
				{
					iRet = i;
					pt.x -= rcClient.left;
					pt.y = pt2.y - nTotalHeight + m_DMArray[i]->nHeight;
					break;
				}
			}
		} while (false);
		return iRet;
	}

	void TGPListBoxEx::UpdateScrollRangeSize()
	{
		CRect rcClient;
		DUIWindow::DV_GetClientRect(&rcClient);
		int nTotalHeight = 0;
		int iCount = (int)m_DMArray.GetCount();
		for (int i = 0; i<iCount; i++)
		{
			nTotalHeight += m_DMArray[i]->nHeight;
		}
		CSize szView(rcClient.Width(), nTotalHeight);
		if (szView.cy>rcClient.Height())
		{
			szView.cx -= m_isbWid;
			if (szView.cx<0)
			{
				szView.cx = 0;
			}
		}
		SetRangeSize(szView);
	}

	void TGPListBoxEx::CreateDragCanvas(UINT iItem)
	{
		do 
		{
			if (iItem>=(int)GetCount())
			{
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CRect rcItem(0,0,rcClient.Width(),m_DMArray[iItem]->nHeight);

			// �����϶�����
			m_pDragCanvas.Release();
			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			pRender->CreateCanvas(rcItem.Width(),rcItem.Height(),&m_pDragCanvas);
			DV_SetDrawEnvironEx(m_pDragCanvas);
			if (m_DMArray[iItem]->pPanel)
			{
				m_DMArray[iItem]->pPanel->DrawItem(m_pDragCanvas,rcItem);
			}
		} while (false);
	}

	void TGPListBoxEx::DrawDraggingState(DWORD dwDragTo)
	{
		do 
		{
			CRect rcClient;
			DV_GetClientRect(&rcClient);
			IDMCanvas *pCanvas = DM_GetCanvas(rcClient,DMOLEDC_PAINTBKGND);
			if (NULL == pCanvas)
			{
				break;
			}
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas, DrawEnviron);
			CRect rcItem(rcClient.left,rcClient.top,rcClient.left,rcClient.bottom);
			int iDragTo   = dwDragTo;
			int iDragFrom = m_dwHitTest;

			CArray<UINT_PTR>items;
			int iCount = (int)GetCount();
			for (int i=0; i<iCount; i++)
			{
				if( i!=iDragFrom) //0������е�����룬�����϶�����
				{
					items.Add(i);
				}
			}
			items.InsertAt(iDragTo,iDragFrom);// ���϶�������뵽�ϵ�����֮��

			int nTotalHeight = 0;
			for (UINT i=0;i<items.GetCount();i++)
			{
				if ((nTotalHeight >= m_ptCurPos.y && nTotalHeight < m_ptCurPos.y + rcClient.Height())
					|| nTotalHeight + m_DMArray[items[i]]->nHeight >= m_ptCurPos.y && nTotalHeight + m_DMArray[items[i]]->nHeight < m_ptCurPos.y + m_rcsbClient.Height())
				{
					CRect rcItem(0,0,rcClient.Width(),m_DMArray[items[i]]->nHeight);
					rcItem.OffsetRect(0,nTotalHeight-m_ptCurPos.y);
					rcItem.OffsetRect(rcClient.TopLeft());
					DrawItem(pCanvas,rcItem,(int)items[i]);
				}
				nTotalHeight += m_DMArray[items[i]]->nHeight;
			}

			DV_PopDrawEnviron(pCanvas, DrawEnviron);
			DM_ReleaseCanvas(pCanvas);
		} while (false);
	}


}//namespace DM