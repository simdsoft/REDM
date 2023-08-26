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
		for (int i=0;i<3;i++)
		{
			m_crItemBg[i].SetTextInvalid();
		}
		m_bMultiSel		 = false;
		m_bMouseDown     = false;
		m_bStartSel      = false;
		m_bSelFrameUp    = false;
		m_pWndRectangle	 = NULL;
		m_pDUIXmlInfo->m_bFocusable = true;

		// listctrl
		DMADDEVENT(DMEventLCSelChangingArgs::EventID);
		DMADDEVENT(DMEventLCSelChangedArgs::EventID);
		DMADDEVENT(DMEventLCItemDeletedArgs::EventID);
	}

	//---------------------------------------------------
	// Function Des: 对外接口 methods
	//---------------------------------------------------
#pragma region Public methods
	int DUIListCtrlEx::InsertItem(int nIndex, DMXmlNode&XmlNode, bool bUpdate/* = true*/)
	{
		int iRet = -1;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}
			if (-1 == nIndex
				||nIndex>GetItemCount())
			{
				nIndex = GetItemCount();
			}

			LPLCITEMEX pNewItem = new LCITEMEX(XmlNode,this);
			LPCSTR strHei		= XmlNode.Attribute(DMAttr::DUIListCtrlExAttr::ITEM_height);
			pNewItem->nHeight   = m_iDefItemHei;
			DMAttributeDispatch::ParseInt(strHei,pNewItem->nHeight);
			LPCSTR strData    = XmlNode.Attribute(DMAttr::DUIListCtrlExAttr::ITEM_data);
			int iData = 0;
			DMAttributeDispatch::ParseInt(strData,iData);
			pNewItem->lParam    = (LPARAM)iData;
			ModifyPanelBgClr(pNewItem->pPanel);

			// 初始化布局
			CRect rcLayout(0,0,GetTotalWidth(),pNewItem->nHeight);
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
			if (nIndex<0 || nIndex>=GetItemCount())
			{
				break;
			}

			int nPos = 0;
			for (int i=0; i<nIndex; i++)
			{
				nPos += m_DMArray[i]->nHeight;
			}

			OnScroll(true,SB_THUMBPOSITION, nPos);
			iErr = 0;
		} while (false);
		return iErr;
	}

	bool DUIListCtrlEx::SetCurSel(int nIndex)
	{
		bool bRet = false;
		do 
		{
			if (nIndex>=GetItemCount())
			{
				break;
			}
			if (nIndex<0)
			{
				nIndex = -1;
			}

			if (m_iSelItem == nIndex)
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
				RedrawItem(nOldSel);
			}

			if (-1 != m_iSelItem)
			{
				m_DMArray[m_iSelItem]->pPanel->ModifyState(DUIWNDSTATE_Check,0);
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

	bool DUIListCtrlEx::SetCurHover(int nIndex)
	{
		bool bRet = false;
		do 
		{
			if (nIndex>=GetItemCount())
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

	int DUIListCtrlEx::GetCurSel() const
	{
		return m_iSelItem;
	}

	int DUIListCtrlEx::GetColumnCount()
	{
		return (int)m_pHeaderCtrl->GetItemCount();
	}

	int DUIListCtrlEx::GetItemCount()
	{
		return (int)DMArrayT<LPLCITEMEX>::GetCount();
	}

	void DUIListCtrlEx::DeleteItem(int nIndex)
	{
		do 
		{
			if (nIndex<0 || nIndex>=GetItemCount())
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
			{// 删除了可视区的项
				UpdateVisibleMap();
			}
		} while (false);
	}

	void DUIListCtrlEx::DeleteAllItems(bool bUpdate /*= true*/)
	{
		DM_RemoveAllChildPanel();
		OnReleaseCapture(m_pCapturePanel);// RemoveAll会delete所有的对象，所以m_pCapturePanel如果有值（正常为NULL）,要提前释放引用计数
		DMArrayT<LPLCITEMEX>::RemoveAll();
		DMMapT<int,CRect>::RemoveAll();
		m_iSelItem		 = -1;
		m_iHoverItem	 = -1;
		if (bUpdate)
		{
			SetRangeSize(CSize(0,0));
			DM_Invalidate();
		}
	}

	void DUIListCtrlEx::EnsureVisible(int nIndex)
	{
		do 
		{
			if (nIndex<0 || nIndex>=(int)GetItemCount())
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
		if (nIndex<0 || nIndex >= GetItemCount())
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
			if (nIndex<0 || nIndex>=GetItemCount())
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
	void DUIListCtrlEx::DrawItem(IDMCanvas* pCanvas, CRect& rcItem, int iItem)
	{
		RelayoutItem(iItem,rcItem);
		CRect rcClient;
		DV_GetClientRect(rcClient);
		rcItem.right = rcItem.right>rcClient.right?rcClient.right:rcItem.right;
		m_DMArray[iItem]->pPanel->DrawItem(pCanvas,rcItem);
	}

	void DUIListCtrlEx::RedrawItem(int iItem)
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

	DMCode DUIListCtrlEx::OnHeaderSizeChanged(DMEventArgs *pEvt)
	{
		RelayVisibleItemsWidth(GetTotalWidth());
		UpdateScrollRange();
		return DM_ECODE_FAIL;
	}

	DMCode DUIListCtrlEx::OnHeaderSwap(DMEventArgs *pEvt)
	{
		DM_InvalidateRect(GetListRect());
		return DM_ECODE_FAIL;
	}

	DMCode DUIListCtrlEx::OnScrollEvent(DMEventArgs *pEvt)
	{
		UpdateVisibleMap();
		DMEventOnScrollArgs* pEvent = (DMEventOnScrollArgs*)pEvt;
		if (!pEvent->m_bVert)
		{
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CRect rcHeader(rcClient);
			rcHeader.bottom = rcHeader.top + m_iHeaderHei;
			rcHeader.left  -= m_ptCurPos.x;
			m_pHeaderCtrl->DM_FloatLayout(rcHeader);
		}
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
	DMCode DUIListCtrlEx::DV_CreateChildWnds(const DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!DMSUCCEEDED(__super::DV_CreateChildWnds(XmlNode)))
			{
				break;
			}

			//1.Header
			m_pHeaderCtrl = dynamic_cast<DUIHeaderCtrl*>(DM_GetWindow(GDW_FIRSTCHILD));
			if (NULL == m_pHeaderCtrl)
			{
				DMFAIL_MSG("listctrlex miss headctrl");
				break;
			}
			CStringA strPos;
			strPos.Format("0,0,-0,%d",m_iHeaderHei);
			m_pHeaderCtrl->SetAttribute("pos",strPos,true);

			//2.dragframe
			m_pWndRectangle = m_pHeaderCtrl->DM_GetWindow(GDW_NEXTSIBLING);
			if (!m_pWndRectangle)
			{
				m_pWndRectangle = new DUIWindow();
				m_pWndRectangle->SetAttribute("clrbg","pbgra(ff,ff,ff,80)",true);
				DM_InsertChild(m_pWndRectangle,DUIWND_FIRST);
			}
			m_pWndRectangle->SetAttribute("bmsgnohandle","1",true);
			m_pWndRectangle->SetAttribute("bvisible","0",true);

			// 设置接收header消息
			m_pHeaderCtrl->m_EventMgr.SubscribeEvent(DM::DMEventHeaderItemChangedArgs::EventID, Subscriber(&DUIListCtrlEx::OnHeaderSizeChanged, this));
			m_pHeaderCtrl->m_EventMgr.SubscribeEvent(DM::DMEventHeaderItemSwapArgs::EventID, Subscriber(&DUIListCtrlEx::OnHeaderSwap, this));
			m_EventMgr.SubscribeEvent(DM::DMEventOnScrollArgs::EventID, Subscriber(&DUIListCtrlEx::OnScrollEvent, this));

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::DUIListCtrlExAttr::NODE_item);
			while (XmlItem.IsValid())
			{
				InsertItem(-1,XmlItem);
				XmlItem = XmlItem.NextSibling();
			}

			int nSelItem = -1;
			DMAttributeDispatch::ParseInt(XmlNode.Attribute(DMAttr::DUIListCtrlExAttr::INT_cursel),nSelItem);
			SetCurSel(nSelItem);
			UpdateScrollRange();
			iErr = DM_ECODE_OK;
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

	void DUIListCtrlEx::UpdateScrollBar()
	{
		CRect rcClient;
		DUIWindow::DV_GetClientRect(&rcClient);
		rcClient.top += m_iHeaderHei;				 /// 不计算header,和父类不同处
		if (rcClient.IsRectEmpty())
		{
			m_isbVisible    = DMSB_NULL;		     /// 关闭滚动条
			return;
		}

		CSize size		= rcClient.Size();
		m_isbVisible    = DMSB_NULL;		     /// 关闭滚动条

		if (size.cy<m_szRange.cy				/// 需要竖直滚动条
			|| (size.cx<m_szRange.cx&&size.cy<m_szRange.cy+m_isbWid))
		{
			m_isbVisible  |= DMSB_VERT;
			m_siVer.nMin   = 0;
			m_siVer.nMax   = m_szRange.cy-1;
			m_siVer.nPage  = size.cy;

			if (size.cx<m_szRange.cx+m_isbWid)/// 需要水平滚动条
			{
				m_isbVisible	|= DMSB_HORZ;
				m_siVer.nPage    = size.cy-m_isbWid > 0 ? size.cy-m_isbWid : 0;

				m_siHoz.nMin     = 0;
				m_siHoz.nMax     = m_szRange.cx-1;
				m_siHoz.nPage    = size.cx-m_isbWid > 0 ? size.cx-m_isbWid : 0;
			}
			else
			{
				m_siHoz.nPage = size.cx;
				m_siHoz.nMin  = 0;
				m_siHoz.nMax  = m_siHoz.nPage-1;
				m_siHoz.nPos  = 0;
				m_ptCurPos.x     = m_siHoz.nPos;/// 记录水平的nPos
			}
		}
		else
		{
			/// 不需要水平滚动条
			m_siVer.nPage = size.cy;
			m_siVer.nMin  = 0;
			m_siVer.nMax  = size.cy-1;
			m_siVer.nPos  = 0;
			m_ptCurPos.y     = m_siVer.nPos;

			if (size.cx<m_szRange.cx)
			{
				/// 需要水平滚动条
				m_isbVisible  |= DMSB_HORZ;
				m_siHoz.nMin   = 0;
				m_siHoz.nMax   = m_szRange.cx-1;
				m_siHoz.nPage  = size.cx;
			}
			else/// 不需要水平滚动条
			{
				m_siHoz.nPage  = size.cx;
				m_siHoz.nMin   = 0;
				m_siHoz.nMax   = m_siHoz.nPage-1;
				m_siHoz.nPos   = 0;
				m_ptCurPos.x   = m_siHoz.nPos;
			}
		}

		SetScrollPos(true,m_siVer.nPos,true);
		SetScrollPos(false,m_siHoz.nPos,false);

		DM_SendMessage(WM_NCCALCSIZE);   ///< 计算非客户区大小

		//  根据需要调整原点位置，和父类不同处
		if (HasScrollBar(false) && (m_ptCurPos.x+(LONG)m_siHoz.nPage)>m_szRange.cx)
		{
			m_ptCurPos.x = m_szRange.cx-m_siHoz.nPage;
		}

		if (HasScrollBar(true) && (m_ptCurPos.y+(LONG)m_siVer.nPage)>m_szRange.cy)
		{
			m_ptCurPos.y = m_szRange.cy-m_siVer.nPage;
		}

		DM_InvalidateRect(m_rcWindow);
	}

	int DUIListCtrlEx::GetTotalHeight()
	{
		int nTotalHeight = 0;
		int iCount = GetItemCount();
		for (int i = 0; i<iCount; i++)
		{
			nTotalHeight += m_DMArray[i]->nHeight;
		}
		return nTotalHeight;
	}

	int DUIListCtrlEx::GetTotalWidth()
	{
		return m_pHeaderCtrl->GetTotalWidth();
	}

	CRect DUIListCtrlEx::GetListRect()
	{
		CRect rcClient;
		DV_GetClientRect(rcClient);

		CRect rcList = rcClient;
		rcList.top += m_iHeaderHei;
		rcList.right = rcList.left + GetTotalWidth();
		return rcList;
	}

	CRect DUIListCtrlEx::GetItemRect(int iItem)
	{
		CRect rcDest;
		do 
		{
			if (iItem<0
				||iItem>=GetItemCount())
			{
				break;
			}

			// 先从可视列表中查找
			if (DMMapT<int,CRect>::GetObjByKey(iItem,rcDest))
			{
				break;
			}

			int nPos = 0;
			for (int i = 0; i<iItem; i++)
			{
				nPos += m_DMArray[i]->nHeight;
			}
			CRect rcClient = GetListRect();
			CRect rcRet(CPoint(0,nPos),CSize(GetTotalWidth(),m_DMArray[iItem]->nHeight));
			rcRet.OffsetRect(rcClient.TopLeft()-m_ptCurPos);
			rcDest = rcRet;
		} while (false);
		return rcDest;
	}

	int DUIListCtrlEx::HitTest(CPoint &pt)
	{// 传入的pt为rcClient所在坐标系的坐标
		int iRet = -1;
		POSITION pos = m_Map.GetStartPosition();
		while(pos)
		{
			DM::CMap<int,CRect>::CPair *p = m_Map.GetNext(pos);
			if(p->m_value.top<=pt.y&&p->m_value.bottom>=pt.y)// 进入可视区,水平可能为负值
			{
				pt -= p->m_value.TopLeft();
				iRet = p->m_key;
				break;
			}
		}
		return iRet;
	}

	int DUIListCtrlEx::HitTotalY(int iTotalY)
	{
		int iRet = -1;
		do 
		{
			if (iTotalY<0)
			{
				break;
			}
			int nTotalHeight = m_iHeaderHei;
			int iCount = GetItemCount();
			for (int i = 0; i < iCount; i++)
			{
				nTotalHeight += m_DMArray[i]->nHeight;
				if (iTotalY < nTotalHeight)
				{
					iRet = i;
					break;
				}
			}
		} while (false);
		return iRet;
	}

	void DUIListCtrlEx::UpdateScrollRange()
	{
		int iHei = GetTotalHeight();
		int iWid = GetTotalWidth();
		CSize szView(iWid,iHei);
		SetRangeSize(szView);
	}

	void DUIListCtrlEx::UpdateVisibleMap()
	{
		DMMapT<int,CRect>::RemoveAll();
		CRect rcList = GetListRect();
		if (rcList.IsRectEmpty())
		{
			return;
		}

		int iTotalHei = 0;
		int iCount = GetItemCount();
		for (int iItem = 0; iItem < iCount; iItem++)
		{
			if ((iTotalHei >= m_ptCurPos.y && iTotalHei < m_ptCurPos.y + rcList.Height())
				|| (iTotalHei + m_DMArray[iItem]->nHeight >= m_ptCurPos.y && iTotalHei + m_DMArray[iItem]->nHeight < m_ptCurPos.y + rcList.Height())
				|| (iTotalHei <= m_ptCurPos.y && iTotalHei + m_DMArray[iItem]->nHeight >= m_ptCurPos.y + rcList.Height())
				)
			{
				CRect rcItem(0,iTotalHei,rcList.Width(),iTotalHei+m_DMArray[iItem]->nHeight);// 在大平面的坐标，以大平面左上角为原点
				rcItem.OffsetRect(rcList.TopLeft()-m_ptCurPos);// 转换成rcList所在的坐标系坐标
				DMMapT<int,CRect>::AddKey(iItem,rcItem);
			}
			iTotalHei += m_DMArray[iItem]->nHeight;
			if (iTotalHei>=m_ptCurPos.y+rcList.Height())// 总高度已超过可视区
			{
				break;
			}
		}
	}

	void DUIListCtrlEx::PreArrayObjRemove(const LPLCITEMEX &obj)
	{
		delete obj;
	}
#pragma endregion

	//---------------------------------------------------
	// Function Des: 辅助 methods
	//---------------------------------------------------
#pragma region Helpers
	void DUIListCtrlEx::UpdateItemPanelId(int iFirst/*=0*/, int iLast /*= -1*/)
	{
		int iCount  = GetItemCount();
		if (-1 == iLast)
		{
			iLast = iCount;
		}
		for (int i=iFirst;i<iCount&&i<iLast;i++)
		{
			m_DMArray[i]->pPanel->SetItemId((LPARAM)i);
		}
	}

	void DUIListCtrlEx::RelayVisibleItemsWidth(int iNewWid)
	{
		POSITION pos = m_Map.GetStartPosition();
		while(pos)
		{
			DM::CMap<int,CRect>::CPair *p = m_Map.GetNext(pos);
			p->m_value.right = p->m_value.left + iNewWid;
		}	
	}

	void DUIListCtrlEx::ReLayoutVisibleItems()
	{
		POSITION pos = m_Map.GetStartPosition();
		while(pos)
		{
			DM::CMap<int,CRect>::CPair *p = m_Map.GetNext(pos);
			RelayoutItem(p->m_key,p->m_value);
		}	
	}

	void DUIListCtrlEx::RelayoutItem(int iItem,CRect rcItem)
	{
		do 
		{
			int iCount = GetItemCount();
			if (iItem < 0||iItem>iCount)
			{
				break;
			}

			//1.布局自身
			CRect rcLayout(0,0,rcItem.Width(),rcItem.Height());
			m_DMArray[iItem]->pPanel->DM_FloatLayout(rcLayout);

			//2.布局子控件
			CRect rcCol(rcLayout);
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
						pChild->DM_FloatLayout(rcChild);
						break;
					}
					iIndex++;
					pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);// 只取兄弟窗口
				}
			}
		} while (false);
	}

	void DUIListCtrlEx::ModifyPanelBgClr(DUIItemPanel* pPanel)
	{
		if (pPanel)
		{
			CStringA strClr;
			if (!m_crItemBg[0].IsTextInvalid())
			{
				strClr.Format("pbgra(%02x,%02x,%02x,%02x)",m_crItemBg[0].b,m_crItemBg[0].g,m_crItemBg[0].r,m_crItemBg[0].a);
				pPanel->m_pDUIXmlInfo->m_pStyle->SetAttribute("clrbg",strClr,false);
			}
			if (!m_crItemBg[1].IsTextInvalid())
			{
				strClr.Format("pbgra(%02x,%02x,%02x,%02x)",m_crItemBg[1].b,m_crItemBg[1].g,m_crItemBg[1].r,m_crItemBg[1].a);
				pPanel->m_pDUIXmlInfo->m_pStyle->SetAttribute("clrbghover",strClr,false);
			}
			if (!m_crItemBg[2].IsTextInvalid())
			{
				strClr.Format("pbgra(%02x,%02x,%02x,%02x)",m_crItemBg[2].b,m_crItemBg[2].g,m_crItemBg[2].r,m_crItemBg[2].a);
				pPanel->m_pDUIXmlInfo->m_pStyle->SetAttribute("clrbgpush",strClr,false);
			}
		}
	}

	void DUIListCtrlEx::ModifySelItems(int nOldSel,int nNewSel)
	{
		bool bRet = false;
		do 
		{
			DMEventLCSelChangingArgs Evt(this);
			Evt.m_nOldSel = nOldSel;
			Evt.m_nNewSel = nNewSel;
			DV_FireEvent(Evt);
			if (Evt.m_bCancel)
			{
				break;
			}
			if (m_bStartSel && m_bMultiSel)
			{// 框选
				if (-1 != nNewSel)
				{
					if (-1 == nOldSel)
					{
						nOldSel = 0;
					}
					int iMax =  DMMAX(nOldSel,nNewSel);
					int iMin = (iMax == nOldSel) ? nNewSel : nOldSel;
					if (!PUSH_CTRL)
					{// 没有按住CTRL,那就把原有的选中全清掉,否则不清空
						for (int i=0; i<iMin; i++)
						{
							if (DMSUCCEEDED(m_DMArray[i]->pPanel->DM_SetCheck(false)))
							{
								RedrawItem(i);
							}
						}
						for (int i=iMax+1; i<GetItemCount(); i++)
						{
							if (DMSUCCEEDED(m_DMArray[i]->pPanel->DM_SetCheck(false)))
							{
								RedrawItem(i);
							}
						}
					}
					for (int i=iMin; i<=iMax; i++)
					{
						if (DMSUCCEEDED(m_DMArray[i]->pPanel->DM_SetCheck(true)))
						{
							RedrawItem(i);
						}
					}
					SetCurHover(-1);
					EnsureVisible(m_bSelFrameUp?iMin:iMax);
				}
			}
			else if(PUSH_CTRL && m_bMultiSel)
			{
				if (-1 != nNewSel)
				{// CTRL改变原来的状态
					m_DMArray[nNewSel]->pPanel->DM_SetCheck(!m_DMArray[nNewSel]->pPanel->DM_IsChecked());
					m_iSelItem = m_DMArray[nNewSel]->pPanel->DM_IsChecked()?nNewSel:-1;
					RedrawItem(nNewSel);
				}
			}
			else if (PUSH_SHIFT && m_bMultiSel)
			{// SHIFT选中所有
				if (-1 != nNewSel)
				{
					if (-1 == nOldSel)
					{
						nOldSel = 0;
					}
					int iMax =  DMMAX(nOldSel,nNewSel);
					int iMin = (iMax == nOldSel) ? nNewSel : nOldSel;
					for (int i=0; i<GetItemCount(); i++)
					{
						bool bCheck = (i>=iMin && i<=iMax);
						if (DMSUCCEEDED(m_DMArray[i]->pPanel->DM_SetCheck(bCheck)))
						{
							RedrawItem(i);
						}
					}
				}
			}
			else
			{//一般是鼠标点击，单选
				m_iSelItem = -1;
				for (int i=0; i<GetItemCount(); i++)
				{ 
					if (DMSUCCEEDED(m_DMArray[i]->pPanel->DM_SetCheck(false)))
					{
						RedrawItem(i);
					}
				}
				if (nNewSel != -1) 
				{
					m_DMArray[nNewSel]->pPanel->DM_SetCheck(true);
					m_iSelItem = nNewSel;
					RedrawItem(nNewSel);
				}
			}
			bRet = true;
		} while (false);
		if (bRet)
		{
			DMEventLCSelChangedArgs evt2(this);
			evt2.m_nOldSel = nOldSel;
			evt2.m_nNewSel = nNewSel;
			DV_FireEvent(evt2);

			// 恢复光标
			if (-1 != m_iSelItem)
			{
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
		}
	}
#pragma endregion

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
#pragma region MsgDispatch
	void DUIListCtrlEx::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			CRect rcInvalid;
			pCanvas->GetClipBox(rcInvalid);
			if (rcInvalid.IsRectEmpty())
			{
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			pCanvas->PushClip(rcClient);
			if (-1 != m_iHoverItem)
			{
				CRect rcHover;
				if (DMMapT<int,CRect>::GetObjByKey(m_iHoverItem,rcHover))
				{
					CRect rcNeed;rcNeed.UnionRect(rcHover,rcInvalid);
					if (rcNeed.EqualRect(rcHover))
					{
						DrawItem(pCanvas,rcHover,m_iHoverItem);
						pCanvas->PopClip();
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
						pCanvas->PopClip();
						break;
					}
				}
			}
			POSITION pos = m_Map.GetStartPosition();
			while(pos)
			{
				DM::CMap<int,CRect>::CPair *p = m_Map.GetNext(pos);
				CRect rcNeed = p->m_value;
				rcNeed.left = rcClient.left;
				rcNeed.IntersectRect(rcNeed,rcInvalid);
				if (!rcNeed.IsRectEmpty())
				{
					DrawItem(pCanvas,p->m_value,p->m_key);
				}
			}
			pCanvas->PopClip();
		} while (false);
	}

	void DUIListCtrlEx::OnSize(UINT nType, CSize size)
	{
		__super::OnSize(nType,size);
		UpdateVisibleMap();
	}

	void DUIListCtrlEx::OnDestroy()
	{
		if (m_pHeaderCtrl)
		{
			m_pHeaderCtrl->m_EventMgr.UnSubscribeEvent(DM::DMEventHeaderItemChangedArgs::EventID, Subscriber(&DUIListCtrlEx::OnHeaderSizeChanged, this));
			m_pHeaderCtrl->m_EventMgr.UnSubscribeEvent(DM::DMEventHeaderItemSwapArgs::EventID, Subscriber(&DUIListCtrlEx::OnHeaderSwap, this));
		}
		m_EventMgr.UnSubscribeEvent(DM::DMEventOnScrollArgs::EventID, Subscriber(&DUIListCtrlEx::OnScrollEvent, this));

		DeleteAllItems(false);
		__super::OnDestroy();
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

	void DUIListCtrlEx::OnMouseLeave()
	{
		__super::OnMouseLeave();
		if (-1!=m_iHoverItem)
		{
			int nOldHover = m_iHoverItem;
			SetCurHover(-1);
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

			if (m_bStartSel && m_bMultiSel)
			{// 框选
				break;
			}

			int  iNewSelItem = -1;
			DUIWindow *pOwner = DM_GetWindow(GDW_OWNER);
			if (pOwner 
				&&(VK_ESCAPE == nChar))
			{
				pOwner->DM_SendMessage(WM_KEYDOWN, nChar, MAKELONG(nFlags, nRepCnt));
				break;
			}

			if (VK_DOWN==nChar&&m_iSelItem<GetItemCount()-1)
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
				ModifySelItems(m_iSelItem,iNewSelItem);
			}
		} while (false);
	}

	LRESULT DUIListCtrlEx::OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				if (m_bMultiSel)
				{
					m_bMouseDown = true;
					m_bStartSel = false;
					m_ptStart = pt + m_ptCurPos; // 如果不记录m_ptCurPos,当滚轮滚动时,m_ptStart就无效了
				}
				else
				{
					m_bMouseDown = false;
					m_bStartSel = false;
				}
				DM_SetCapture();
			}
			break;
		case WM_LBUTTONUP:
			{
				DM_ReleaseCapture();
				m_bMouseDown = false;
				if (false == m_bStartSel)
				{
					ModifySelItems(m_iSelItem,m_iHoverItem);
				}
				else
				{
					m_pWndRectangle->DM_SetVisible(false,true);
					DM_Invalidate();
				}
				m_bStartSel = false;
			}
			break;
		case WM_MOUSEMOVE:
			{
				if (m_bMouseDown)
				{
					CRect rcList = GetListRect();
					CRect rcClient;
					DV_GetClientRect(rcClient);
					rcList.right = DMMAX(rcList.right,rcClient.right);
					CPoint ptLeftTop = m_ptStart - m_ptCurPos;
					CPoint ptRightBtm = pt;
					CRect rcDragFrame(ptLeftTop,ptRightBtm);
					rcDragFrame.NormalizeRect();
					m_bSelFrameUp = ptLeftTop.y > ptRightBtm.y;
					if (rcDragFrame.Width()>10 || rcDragFrame.Height()>10)
					{
						m_bStartSel = true; 
						m_iSelItem = -1;//放弃单独那条
						rcDragFrame.IntersectRect(rcClient, rcDragFrame);
						m_pWndRectangle->DM_FloatLayout(rcDragFrame);
						m_rcDragFrame = rcDragFrame;
						m_pWndRectangle->DM_SetVisible(true,true);
						CRect rcInvalid(rcList.left,m_rcDragFrame.top,rcList.right,m_rcDragFrame.bottom);
						DM_Invalidate();

						int iFirst = HitTotalY(m_rcDragFrame.top+m_ptCurPos.y-rcClient.top);
						int iLast = HitTotalY(m_rcDragFrame.bottom+m_ptCurPos.y-rcClient.top);
						ModifySelItems(iFirst,iLast); 	
					}
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

	DMCode DUIListCtrlEx::OnAttributeHeaderHei(LPCSTR lpszValue, bool bLoadXml)
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
					CStringA strPos;
					strPos.Format("0,0,-0,%d",m_iHeaderHei);
					m_pHeaderCtrl->SetAttribute("pos",strPos,true);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIListCtrlEx::OnAttributeCurSel(LPCSTR lpszValue, bool bLoadXml)
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