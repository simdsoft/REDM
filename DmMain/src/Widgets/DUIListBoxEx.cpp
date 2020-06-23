#include "DmMainAfx.h"
#include "DUIListBoxEx.h"

namespace DM
{
	DUIListBoxEx::DUIListBoxEx()
	{
		m_iSelItem       = -1;
		m_iHoverItem     = -1;
		m_iDefItemHei    = 20;
		for (int i=0;i<3;i++)
		{
			m_crItemBg[i].SetTextInvalid();
		}
		m_pDUIXmlInfo->m_bFocusable = true;

		// listbox
		DMADDEVENT(DMEventLBSelChangingArgs::EventID);
		DMADDEVENT(DMEventLBSelChangedArgs::EventID);
	}

	//---------------------------------------------------
	// Function Des: ����ӿ� methods
	//---------------------------------------------------
#pragma region Public methods
	int DUIListBoxEx::InsertItem(int nIndex, DMXmlNode&XmlNode, bool bUpdate/* = true*/)
	{
		int iRet = -1;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}
			if (-1 == nIndex
				||nIndex>GetCount())
			{
				nIndex = GetCount();
			}
			LPLBITEMEX pNewItem = new LBITEMEX(XmlNode,this);
			CStringW strHei		= XmlNode.Attribute(DMAttr::DUIListBoxExAttr::ITEM_height);
			pNewItem->nHeight   = m_iDefItemHei;
			DMAttributeDispatch::ParseInt(strHei,pNewItem->nHeight);
			CStringW strData    = XmlNode.Attribute(DMAttr::DUIListBoxExAttr::ITEM_data);
			int iData = 0;
			DMAttributeDispatch::ParseInt(strData,iData);
			pNewItem->lParam    = (LPARAM)iData;
			ModifyPanelBgClr(pNewItem->pPanel);/// Ĭ�ϱ���ɫ

			// ��ʼ������
			CRect rcLayout(0,0,m_rcsbClient.Width(),pNewItem->nHeight);
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
			if (bUpdate)
			{
				UpdateVisibleMap();
				UpdateScrollRange();
			}
			iRet = nIndex;
		} while (false);
		return iRet;
	}

	int DUIListBoxEx::GetTopIndex() const
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

	int DUIListBoxEx::SetTopIndex(int nIndex)
	{
		int iErr = LB_ERR ;
		do 
		{
			if (nIndex<0 || nIndex>=GetCount())
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

	int DUIListBoxEx::GetItemHeight(int nIndex) 
	{
		if (nIndex < 0 || nIndex >= GetCount())
			return LB_ERR;

		return m_DMArray[nIndex]->nHeight;
	}

	int DUIListBoxEx::GetAllItemHeight()
	{
		int nTotalHeight = 0;
		int iCount = GetCount();
		for (int i = 0; i<iCount; i++)
		{
			nTotalHeight += m_DMArray[i]->nHeight;
		}
		return nTotalHeight;
	}

	int DUIListBoxEx::SetAllItemHeight(int cyItemHeight, bool bUpdate /*= true*/)
	{
		int iCount = GetCount();
		for (int i = 0; i<iCount; i++)
		{
			SetItemHeight(i,cyItemHeight,bUpdate);
		}
		return 0;
	}

	int DUIListBoxEx::SetItemHeight(int nIndex, int cyItemHeight, bool bUpdate /*= true*/)
	{
		if (cyItemHeight < 0 || nIndex < 0 || nIndex >= GetCount())
			return LB_ERR;
		if (cyItemHeight==m_DMArray[nIndex]->nHeight)
		{
			return 0;
		}
		m_DMArray[nIndex]->nHeight = cyItemHeight;
		if (bUpdate)
		{
			UpdateScrollRange();
		}

		return 0;
	}

	bool DUIListBoxEx::SetCurSel(int nIndex)
	{
		bool bRet = false;
		do 
		{
			if (nIndex>=GetCount())
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
			if (-1 != nOldSel)
			{
				m_DMArray[nOldSel]->pPanel->ModifyState(0,DUIWNDSTATE_Check);
				RedrawItem(nOldSel);
			}
			if (-1 != m_iSelItem)
			{
				m_DMArray[m_iSelItem]->pPanel->ModifyState(DUIWNDSTATE_Check,0);
				m_DMArray[m_iSelItem]->pPanel->DM_SendMessage(WM_SETFOCUS,0,0);
				RedrawItem(m_iSelItem);
			}
			POSITION pos = m_Map.GetStartPosition();
			while(pos)
			{
				DM::CMap<int,CRect>::CPair *p = m_Map.GetNext(pos);
				if (p->m_key != m_iSelItem)
				{
					m_DMArray[p->m_key]->pPanel->OnSetFocusWnd(NULL);
				}
			}

			DMEventLBSelChangedArgs evt2(this);
			evt2.m_nOldSel = nOldSel;
			evt2.m_nNewSel = m_iSelItem;
			DV_FireEvent(evt2);
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUIListBoxEx::SetCurHover(int nIndex)
	{
		bool bRet = false;
		do 
		{
			if (nIndex>=GetCount())
			{
				break;
			}

			if (nIndex<0)
			{
				nIndex = -1;
			}

			if (m_iHoverItem==nIndex)
			{
				break;
			}

			int nOldHover = m_iHoverItem;
			m_iHoverItem  = nIndex;
			if (-1 != nOldHover)
			{
				m_DMArray[nOldHover]->pPanel->ModifyState(0,DUIWNDSTATE_Hover);
				RedrawItem(nOldHover);
			}
			if (-1 != m_iHoverItem)
			{
				m_DMArray[m_iHoverItem]->pPanel->ModifyState(DUIWNDSTATE_Hover,0);
				RedrawItem(m_iHoverItem);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	int DUIListBoxEx::GetCurSel() const
	{
		return m_iSelItem;
	}

	int DUIListBoxEx::GetCount()
	{
		return (int)DMArrayT<LPLBITEMEX>::GetCount();
	}

	void DUIListBoxEx::DeleteItem(int nIndex)
	{
		do 
		{
			if (nIndex<0 || nIndex>=GetCount())
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
			UpdateScrollRange();
			if (DMMapT<int,CRect>::IsKeyExist(nIndex))
			{// ɾ���˿���������
				UpdateVisibleMap();
			}
		} while (false);
	}

	void DUIListBoxEx::DeleteAllItems(bool bUpdate /*= true*/)
	{
		DM_RemoveAllChildPanel();
		OnReleaseCapture(m_pCapturePanel);// RemoveAll��delete���еĶ�������m_pCapturePanel�����ֵ������ΪNULL��,Ҫ�������ͷ����ü���
		DMArrayT<LPLBITEMEX>::RemoveAll();
		DMMapT<int,CRect>::RemoveAll();
		m_iSelItem		 = -1;
		m_iHoverItem	 = -1;
		if (bUpdate)
		{
			SetRangeSize(CSize(0,0));
			DM_Invalidate();
		}
	}

	void DUIListBoxEx::EnsureVisible(int nIndex)
	{
		do 
		{
			if (nIndex<0 || nIndex>=GetCount())
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

	LPARAM DUIListBoxEx::GetItemData(int nIndex)
	{
		if (nIndex<0 || nIndex >= GetCount())
		{
			return 0;
		}

		return m_DMArray[nIndex]->lParam;
	}

	bool DUIListBoxEx::SetItemData(int nIndex, LPARAM lParam)
	{
		bool bRet = false;
		do 
		{
			if (nIndex<0 || nIndex>=GetCount())
			{
				break;
			}

			m_DMArray[nIndex]->lParam = lParam;
			bRet = true;
		} while (false);
		return bRet;
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: IDMItemPanelOwner methods
	//---------------------------------------------------
#pragma region IDMItemPanelOwner
	DUIWindow* DUIListBoxEx::GetOwnerWindow()
	{
		return this;
	}

	DMCode DUIListBoxEx::OnSetCapture(DUIItemPanel* pPanel)
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

	DMCode DUIListBoxEx::OnReleaseCapture(DUIItemPanel* pPanel)
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

	DMCode DUIListBoxEx::OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pPanel)
			{
				break;
			}
			int iItem = (int)pPanel->GetItemId();
			*lpRect = GetItemRect(iItem);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: Draw methods
	//---------------------------------------------------
#pragma region Draw
	void DUIListBoxEx::DrawItem(IDMCanvas* pCanvas, CRect& rcItem, int iItem)
	{
		CRect rcClient;
		DV_GetClientRect(rcClient);
		rcItem.right = rcItem.right>rcClient.right?rcClient.right:rcItem.right;
		m_DMArray[iItem]->pPanel->DrawItem(pCanvas,rcItem);
	}

	void DUIListBoxEx::RedrawItem(int iItem)
	{
		do 
		{
			CRect rcItem;
			if (!DMMapT<int,CRect>::GetObjByKey(iItem,rcItem))
			{
				break;
			}
			DM_InvalidateRect(rcItem);
		} while (false);
	}

	DMCode DUIListBoxEx::OnScrollEvent(DMEventArgs *pEvt)
	{
		UpdateVisibleMap();
		DMEventOnScrollArgs* pEvent = (DMEventOnScrollArgs*)pEvt;
		if (SB_THUMBTRACK == pEvent->m_iSbCode)
		{
			ScrollUpdateWindow();
		}
		return DM_ECODE_FAIL;
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: DV methods
	//---------------------------------------------------
#pragma region DV
	DMCode DUIListBoxEx::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}

			m_EventMgr.SubscribeEvent(DM::DMEventOnScrollArgs::EventID, Subscriber(&DUIListBoxEx::OnScrollEvent, this));

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::DUIListBoxExAttr::NODE_item);
			while (XmlItem.IsValid())
			{
				InsertItem(-1,XmlItem);
				XmlItem = XmlItem.NextSibling();
			}
			int nSelItem = -1;
			DMAttributeDispatch::ParseInt(XmlNode.Attribute(DMAttr::DUIListBoxExAttr::INT_cursel),nSelItem);
			SetCurSel(nSelItem);
			UpdateScrollRange();
		} while (false);
		return iErr;
	}

	DMCode DUIListBoxEx::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
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

	DMCode DUIListBoxEx::DV_OnSetCursor(const CPoint &pt)
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

	CRect DUIListBoxEx::GetItemRect(int iItem)
	{
		CRect rcDest;
		do 
		{
			if (iItem<0
				||iItem>=GetCount())
			{
				break;
			}

			// �ȴӿ����б��в���
			if (DMMapT<int,CRect>::GetObjByKey(iItem,rcDest))
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

	int DUIListBoxEx::HitTest(CPoint &pt)
	{
		// �����ptΪrcClient��������ϵ������
		int iRet = -1;
		POSITION pos = m_Map.GetStartPosition();
		while(pos)
		{
			DM::CMap<int,CRect>::CPair *p = m_Map.GetNext(pos);
			if(p->m_value.top<=pt.y&&p->m_value.bottom>=pt.y)// ���������,ˮƽ����Ϊ��ֵ
			{
				pt -= p->m_value.TopLeft();
				iRet = p->m_key;
				break;
			}
		}
		return iRet;
	}

	void DUIListBoxEx::UpdateScrollRange()
	{
		int iHei = GetAllItemHeight();
		CSize szView(0,iHei);
		SetRangeSize(szView);
	}

	void DUIListBoxEx::UpdateVisibleMap()
	{
		DMMapT<int,CRect>::RemoveAll();
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		if (rcClient.IsRectEmpty())
		{
			return;
		}

		int iTotalHei = 0;
		int iCount = GetCount();
		for (int iItem = 0; iItem < iCount; iItem++)
		{
			if ((iTotalHei >= m_ptCurPos.y && iTotalHei < m_ptCurPos.y + rcClient.Height())
				|| (iTotalHei + m_DMArray[iItem]->nHeight >= m_ptCurPos.y && iTotalHei + m_DMArray[iItem]->nHeight < m_ptCurPos.y + rcClient.Height())
				||(iTotalHei<=m_ptCurPos.y && iTotalHei + m_DMArray[iItem]->nHeight >= m_ptCurPos.y + m_rcsbClient.Height())
				)
			{
				CRect rcItem(0,0,rcClient.Width(),m_DMArray[iItem]->nHeight);// �ڴ�ƽ������꣬�Դ�ƽ�����Ͻ�Ϊԭ��
				rcItem.OffsetRect(0,iTotalHei-m_ptCurPos.y);
				rcItem.OffsetRect(rcClient.TopLeft());// ת����rcList���ڵ�����ϵ����
				DMMapT<int,CRect>::AddKey(iItem,rcItem);
				CRect rcLayout(0,0,rcItem.Width(),rcItem.Height());
				m_DMArray[iItem]->pPanel->DM_FloatLayout(rcLayout);
			}
			iTotalHei += m_DMArray[iItem]->nHeight;
			if (iTotalHei>=m_ptCurPos.y+rcClient.Height())// �ܸ߶��ѳ���������
			{
				break;
			}
		}
	}

	void DUIListBoxEx::PreArrayObjRemove(const LPLBITEMEX &obj)
	{
		delete obj;
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: ���� methods
	//---------------------------------------------------
#pragma region ����
	void DUIListBoxEx::UpdateItemPanelId(int iFirst/*=0*/, int iLast /*= -1*/)
	{
		int iCount  = GetCount();
		if (-1 == iLast)
		{
			iLast = iCount;
		}
		for (int i=iFirst;i<iCount&&i<iLast;i++)
		{
			m_DMArray[i]->pPanel->SetItemId((LPARAM)i);
		}
	}

	void DUIListBoxEx::ReLayoutVisibleItems()
	{
		POSITION pos = m_Map.GetStartPosition();
		while(pos)
		{
			DM::CMap<int,CRect>::CPair *p = m_Map.GetNext(pos);
			RelayoutItem(p->m_key,p->m_value);
		}	
	}

	void DUIListBoxEx::RelayoutItem(int iItem,CRect rcItem)
	{
		do 
		{
			int iCount = GetCount();
			if (iItem < 0||iItem>iCount)
			{
				break;
			}

			//1.��������
			CRect rcLayout(0,0,rcItem.Width(),rcItem.Height());
			m_DMArray[iItem]->pPanel->DM_FloatLayout(rcLayout);
		} while (false);
	}

	void DUIListBoxEx::ModifyPanelBgClr(DUIItemPanel* pPanel)
	{
		if (pPanel)
		{
			CStringW strClr;
			if (!m_crItemBg[0].IsTextInvalid())
			{
				strClr.Format(L"pbgra(%02x,%02x,%02x,%02x)",m_crItemBg[0].b,m_crItemBg[0].g,m_crItemBg[0].r,m_crItemBg[0].a);
				pPanel->m_pDUIXmlInfo->m_pStyle->SetAttribute(L"clrbg",strClr,false);
			}
			if (!m_crItemBg[1].IsTextInvalid())
			{
				strClr.Format(L"pbgra(%02x,%02x,%02x,%02x)",m_crItemBg[1].b,m_crItemBg[1].g,m_crItemBg[1].r,m_crItemBg[1].a);
				pPanel->m_pDUIXmlInfo->m_pStyle->SetAttribute(L"clrbghover",strClr,false);
			}
			if (!m_crItemBg[2].IsTextInvalid())
			{
				strClr.Format(L"pbgra(%02x,%02x,%02x,%02x)",m_crItemBg[2].b,m_crItemBg[2].g,m_crItemBg[2].r,m_crItemBg[2].a);
				pPanel->m_pDUIXmlInfo->m_pStyle->SetAttribute(L"clrbgpush",strClr,false);
			}
		}
	}

	void DUIListBoxEx::ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr)
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
#pragma endregion


	//---------------------------------------------------
	// Function Des: DUI����Ϣ�ַ�ϵ�к���
	//---------------------------------------------------
#pragma region MsgDispatch
	void DUIListBoxEx::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			CRect rcInvalid;
			pCanvas->GetClipBox(rcInvalid);
			if (rcInvalid.IsRectEmpty())
			{
				break;
			}
			if (-1 != m_iHoverItem)
			{
				CRect rcHover;
				if (DMMapT<int,CRect>::GetObjByKey(m_iHoverItem,rcHover))
				{
					CRect rcNeed;rcNeed.UnionRect(rcHover,rcInvalid);
					if (rcNeed.EqualRect(rcHover))
					{
						DrawItem(pCanvas,rcHover,m_iHoverItem);
						break;
					}
				}
			}
			if (-1 != m_iSelItem)
			{
				CRect rcSel;
				if (DMMapT<int,CRect>::GetObjByKey(m_iSelItem,rcSel))
				{
					CRect rcNeed;rcNeed.UnionRect(rcSel,rcInvalid);
					if (rcNeed.EqualRect(rcSel))
					{
						DrawItem(pCanvas,rcSel,m_iSelItem);
						break;
					}
				}
			}
			POSITION pos = m_Map.GetStartPosition();
			while(pos)
			{
				DM::CMap<int,CRect>::CPair *p = m_Map.GetNext(pos);
				CRect rcNeed = p->m_value;
				rcNeed.IntersectRect(rcNeed,rcInvalid);
				if (!rcNeed.IsRectEmpty())
				{
					DrawItem(pCanvas,p->m_value,p->m_key);
				}
			}
		} while (false);
	}

	void DUIListBoxEx::OnSize(UINT nType, CSize size)
	{
		__super::OnSize(nType,size);
		UpdateVisibleMap();
	}

	void DUIListBoxEx::OnDestroy()
	{
		m_EventMgr.UnSubscribeEvent(DM::DMEventOnScrollArgs::EventID, Subscriber(&DUIListBoxEx::OnScrollEvent, this));
		DeleteAllItems(false);
		__super::OnDestroy();
	}

	void DUIListBoxEx::DM_OnSetFocus()
	{
		__super::DM_OnSetFocus();
		if (-1!=m_iSelItem) 
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_SETFOCUS,0,0);
		}
	}

	void DUIListBoxEx::DM_OnKillFocus()
	{
		__super::DM_OnKillFocus();
		if (-1!=m_iSelItem) 
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_KILLFOCUS,0,0);
			RedrawItem(m_iSelItem);
		}
	}

	void DUIListBoxEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	void DUIListBoxEx::OnMouseLeave()
	{
		if (DM_IsVisible(true))// ��һ���жϣ���ֹ�ǿͻ�����list����ʱ����
		{
			__super::OnMouseLeave();
			if (-1!=m_iHoverItem)
			{
				int nOldHover = m_iHoverItem;
				SetCurHover(-1);
				m_DMArray[nOldHover]->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
			}
		}
	}

	BOOL DUIListBoxEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		return __super::OnMouseWheel(nFlags,zDelta,pt);
	}

	void DUIListBoxEx::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
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

			if (VK_DOWN==nChar&&m_iSelItem<GetCount()-1)
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

	LRESULT DUIListBoxEx::OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				DM_SetCapture();
			}
			break;
		case WM_LBUTTONUP:
			{
				DM_ReleaseCapture();
				if (m_iHoverItem != m_iSelItem)
				{
					SetCurSel(m_iHoverItem);
				}
			}
			break;
		default:
			break;
		}

		do 
		{
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
				SetCurHover(iHoverItem);
				if (-1!=iOldHoverItem)
				{
					m_DMArray[iOldHoverItem]->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
				}

				if (-1!=m_iHoverItem)
				{
					m_DMArray[m_iHoverItem]->pPanel->OnFrameEvent(WM_MOUSEHOVER,wParam,MAKELPARAM(pt.x,pt.y));
				}
			}
			if (WM_LBUTTONDOWN == uMsg&& -1!=m_iSelItem
				&& m_iSelItem != m_iHoverItem )
			{///ԭ����ʧȥ����
				m_DMArray[m_iSelItem]->pPanel->m_FocusMgr.SetFocusedWnd(NULL);
			}
			if (-1 != m_iHoverItem)
			{
				m_DMArray[m_iHoverItem]->pPanel->OnFrameEvent(uMsg, wParam, MAKELPARAM(pt.x, pt.y));//panel������õ�hoverʱ������ԭ���õ�hover��panelʧȥhover
			}
			if (-1 != m_iHoverItem)
			{
				m_DMArray[m_iHoverItem]->pPanel->ModifyState(DUIWNDSTATE_Hover, 0);//listctrlex��item�»���һ��window,����listctrlexʧȥhover����window,���õ���
			}

		} while (false);
		return lRet;
	}

	LRESULT DUIListBoxEx::OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
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

	DMCode DUIListBoxEx::OnAttributeCurSel(LPCWSTR lpszValue, bool bLoadXml)
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
#pragma endregion

}//namespace DM