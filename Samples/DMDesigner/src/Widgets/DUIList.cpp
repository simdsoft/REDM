#include "DMDesignerAfx.h"
#include "DUIList.h"

namespace DM
{
	DUIList::DUIList()
	{
		m_iSelItem       = -1;
		m_iHoverItem     = -1;
		m_iDefItemHei    = 20;
		m_pExpandArrSkin = NULL;

		m_bSwapLine      = false;
		m_bDragging      = false;
		m_dwHitTest      = -1;
		m_dwDragTo       = -1;

		m_pDUIXmlInfo->m_bFocusable = true;

		// listbox
		DMADDEVENT(DMEventLBSelChangingArgs::EventID);
		DMADDEVENT(DMEventLBSelChangedArgs::EventID);
		DMADDEVENT(DMEventLBGetDispInfoArgs::EventID);
	}

	DUIList::~DUIList()
	{
		DeleteAllItems(false);
	}

	int DUIList::InsertItem(int nIndex, DMXmlNode&XmlNode)
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
				nIndex = GetCount();
			} 

			LPDSLBITEMEX pNewItem = new DSLBITEMEX(XmlNode,this);
			CStringW strHei		= XmlNode.Attribute(DMAttr::DUIListAttr::ITEM_height);
			pNewItem->nHeight   = m_iDefItemHei;
			DMAttributeDispatch::ParseInt(strHei,pNewItem->nHeight);

			CStringW strExpandHei = XmlNode.Attribute(DMAttr::DUIListAttr::ITEM_expandheight);
			pNewItem->nExpandHeight = pNewItem->nHeight;
			DMAttributeDispatch::ParseInt(strExpandHei,pNewItem->nExpandHeight);

			CStringW strbExpand = XmlNode.Attribute(DMAttr::DUIListAttr::ITEM_bexpand);
			pNewItem->bExpand = false;
			DMAttributeDispatch::ParseBool(strbExpand,pNewItem->bExpand);

			CStringW strData    = XmlNode.Attribute(DMAttr::DUIListAttr::ITEM_data);
			int iData = 0;
			DMAttributeDispatch::ParseInt(strData,iData);
			pNewItem->lParam    = (LPARAM)iData;
	
			// 初始化布局
			CRect rcLayout(0,0,m_rcsbClient.Width(),pNewItem->nHeight);
			if (pNewItem->bExpand)
			{
				rcLayout.bottom = pNewItem->nExpandHeight;
			}
			pNewItem->pPanel->DM_FloatLayout(rcLayout);

			pNewItem->XmlDoc.ResetByXmlNode(XmlNode);//复制XmlNode

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
			UpdateScrollRangeSize();
			iRet = nIndex;
		} while (false);
		return iRet;
	}

	CRect DUIList::GetItemRect(int iItem)
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
				int nHei = m_DMArray[i]->bExpand?m_DMArray[i]->nExpandHeight:m_DMArray[i]->nHeight;
				nPos += nHei;
			}
			int nHei = m_DMArray[iItem]->bExpand?m_DMArray[iItem]->nExpandHeight:m_DMArray[iItem]->nHeight;
			rcDest.SetRect(0,nPos,rcClient.Width(), nHei+nPos);
			rcDest.OffsetRect(rcClient.TopLeft()-m_ptCurPos);

		} while (false);
		return rcDest;
	}

	bool DUIList::SetCurSel(int nIndex)
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
			int nOldSel = m_iSelItem;
			m_iSelItem  = nIndex;
			NotifySelChange(nOldSel,m_iSelItem);

			bRet = true;
		} while (false);
		return bRet;
	}

	int DUIList::GetCurSel() const
	{
		return m_iSelItem;
	}

	void DUIList::DeleteItem(int nIndex)
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

	void DUIList::DeleteAllItems(bool bUpdate /*= true*/)
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

	void DUIList::EnsureVisible(int nIndex)
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
			{//计算出目标顶部Y坐标
				int nHei = m_DMArray[i]->bExpand?m_DMArray[i]->nExpandHeight:m_DMArray[i]->nHeight;
				nTargetY += nHei;
			}
			int nHei = m_DMArray[nIndex]->bExpand?m_DMArray[nIndex]->nExpandHeight:m_DMArray[nIndex]->nHeight;
			if (nTargetY < m_ptCurPos.y || nHei >= rcClient.Height())
			{//目标顶部在可视范围上方，或者可视范围内只能显示目标项目，则把可视范围滚动到目标顶部
				OnScroll(true,SB_THUMBPOSITION,nTargetY);
				break;
			}
			nTargetY += nHei;
			if (nTargetY > m_ptCurPos.y + rcClient.Height())
			{//目标底部在可视范围下方，则往前回溯最后一个能完整显示的项目的顶坐标
				int nShowHeight = 0;
				for ( int i = nIndex; i >= 0; i--)
				{
					int nHei = m_DMArray[i]->bExpand?m_DMArray[i]->nExpandHeight:m_DMArray[i]->nHeight;
					nShowHeight += nHei;
					if (nShowHeight > rcClient.Height())
					{
						break;
					}
					else
					{
						nTargetY -= nHei;
					}
				}
				OnScroll(true,SB_THUMBPOSITION,nTargetY);
				break;
			}
		} while (false);
	}

	bool DUIList::ExpandItem(int nIndex,bool bExpand)
	{
		bool bRet = false;
		do 
		{
			if (nIndex<0
				||nIndex>=(int)GetCount())
			{
				break;
			}
			if (bExpand == m_DMArray[nIndex]->bExpand)
			{
				break;
			}
			m_DMArray[nIndex]->bExpand = bExpand;
			UpdateAllPanelLayout();
			UpdateScrollRangeSize();

			bRet = true;
		} while (false);
		return bRet;
	}

	// 绘制
	void DUIList::DrawItem(IDMCanvas* pCanvas, CRect& rc, int iItem)
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

			// 
			if (m_pExpandArrSkin)
			{
				CSize sz;
				m_pExpandArrSkin->GetStateSize(sz);
				CRect rcArr(rc.left,rc.top,sz.cx+rc.left,sz.cy+rc.top);
				m_pExpandArrSkin->Draw(pCanvas,rcArr,m_DMArray[iItem]->bExpand);
			}

		} while (false);
	}

	void DUIList::RedrawItem(int iItem)
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
				int nHei = m_DMArray[i]->bExpand?m_DMArray[i]->nExpandHeight:m_DMArray[i]->nHeight;
				nTargetY += nHei;
			}
			CRect rcClient;
			DV_GetClientRect(&rcClient);
			int nHei = m_DMArray[iItem]->bExpand? m_DMArray[iItem]->nExpandHeight:m_DMArray[iItem]->nHeight;
			if ((nTargetY >= m_ptCurPos.y && nTargetY <= m_ptCurPos.y + rcClient.Height())
				||(nTargetY + nHei >= m_ptCurPos.y && nTargetY + nHei <= m_ptCurPos.y + rcClient.Height()) 
				||(nTargetY <= m_ptCurPos.y && nTargetY + nHei >= m_ptCurPos.y + rcClient.Height()))
			{//目标有部分在可视范围内
				
				CRect rcItem(0,0,rcClient.Width(),nHei);
				rcItem.OffsetRect(0,nTargetY-m_ptCurPos.y);// 促使第一个item可能只绘制一部分
				rcItem.OffsetRect(rcClient.TopLeft());
				IDMCanvas* pCanvas = DM_GetCanvas(&rcItem,DMOLEDC_PAINTBKGND);// todo.暂时不抓背景
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
	// Function Des: DUI的消息分发系列函数
	void DUIList::OnDestroy()
	{
		DeleteAllItems(false);
		__super::OnDestroy();
	}
	void DUIList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	void DUIList::OnSize(UINT nType,CSize size)
	{
		__super::OnSize(nType,size);
		UpdateAllPanelLayout();
	}

	void DUIList::DM_OnPaint(IDMCanvas* pCanvas)
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
				int nHei = m_DMArray[iItem]->bExpand?m_DMArray[iItem]->nExpandHeight:m_DMArray[iItem]->nHeight;
				if ((nTotalHeight >= m_ptCurPos.y && nTotalHeight < m_ptCurPos.y + rcClient.Height())
					|| (nTotalHeight + nHei >= m_ptCurPos.y && nTotalHeight + nHei < m_ptCurPos.y + m_rcsbClient.Height())
					|| (nTotalHeight <= m_ptCurPos.y && nTotalHeight + nHei >= m_ptCurPos.y + m_rcsbClient.Height())
					)
				{
					CRect rcItem(0,0,rcClient.Width(),nHei);
					rcItem.OffsetRect(0,nTotalHeight-m_ptCurPos.y);
					rcItem.OffsetRect(rcClient.TopLeft());
					DrawItem(pCanvas,rcItem,iItem);
				}
				nTotalHeight += nHei;
			}

			DV_PopDrawEnviron(pCanvas,DrawEnviron);
		} while (false);
	}

	void DUIList::DM_OnSetFocus()
	{
		__super::DM_OnSetFocus();
		if (-1!=m_iSelItem) 
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_SETFOCUS,0,0);
		}
	}

	void DUIList::DM_OnKillFocus()
	{
		__super::DM_OnKillFocus();
		if (-1!=m_iSelItem) 
		{
			m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_KILLFOCUS,0,0);
			RedrawItem(m_iSelItem);
		}
	}

	void DUIList::OnMouseLeave()
	{
		__super::OnMouseLeave();
		if (-1!=m_iHoverItem)
		{
			int nOldHover = m_iHoverItem;
			m_iHoverItem  = -1;
			m_DMArray[nOldHover]->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
		}
	}

	void DUIList::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		DM_SetCapture();
		SetMsgHandled(FALSE);
	}

	void DUIList::OnLButtonUp(UINT nFlags,CPoint pt)
	{ 
		DM_ReleaseCapture();
		// 这里改变expand状态
		if (false == m_bDragging && -1!=m_iHoverItem&&m_iHoverItem<(int)GetCount())
		{
			if (false == m_DMArray[m_iHoverItem]->bExpand)
			{
				m_DMArray[m_iHoverItem]->bExpand = true;
				UpdateAllPanelLayout();
				UpdateScrollRangeSize();
			} 
			else  
			{

				CPoint ptCopy = pt;
				bool bInExpand = false;
				int iHot = HitTest(ptCopy,&bInExpand);
				if (-1!=iHot&&iHot == m_iHoverItem &&false == bInExpand)// 
				{
					m_DMArray[m_iHoverItem]->bExpand = false;
					UpdateAllPanelLayout();
					UpdateScrollRangeSize();
				}
			}
		} 
		SetMsgHandled(FALSE);
	}

	BOOL DUIList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		BOOL bRet = TRUE;
		do 
		{
			if (m_iSelItem<(int)GetCount()&&m_iSelItem>=0)
			{
				CPoint pnnt = pt;
				CRect rcItem;
				m_DMArray[m_iSelItem]->pPanel->OnGetContainerRect(rcItem);
				pnnt.Offset(-rcItem.TopLeft());///< 转换成面板坐标
				bRet = m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_MOUSEWHEEL, MAKEWPARAM(nFlags,zDelta), MAKELPARAM(pnnt.x, pnnt.y));
				if (m_DMArray[m_iSelItem]->pPanel->IsMsgHandled())
				{
					break;
				}
			}
			bRet =  __super::OnMouseWheel(nFlags,zDelta,pt);
		} while (FALSE);
		return bRet;
	}

	void DUIList::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
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
				NotifySelChange(m_iSelItem,iNewSelItem);
			}
		} while (false);
	}

	LRESULT DUIList::OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
	{
		LRESULT lRet=__super::OnNcCalcSize(bCalcValidRects,lParam);
		UpdateAllPanelLayout();
		return lRet;
	}

	LRESULT DUIList::OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{

			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			// 加入拖动判断
			if (m_bSwapLine)
			{
				if (WM_LBUTTONDOWN == uMsg)
				{
					m_ptClick   = pt;
					CPoint pt2 = pt;
					m_dwHitTest = HitTest(pt2);// pt会在HitTest中自动改变
				}
				else if (WM_MOUSEMOVE == uMsg)
				{
					if (m_bDragging || wParam&MK_LBUTTON)
					{
						if (!m_bDragging)
						{
							m_bDragging = true;      /// 开始移动
							if (-1!=m_dwHitTest) 
							{ 
								m_dwDragTo    = m_dwHitTest;
								CRect rcItem  = GetItemRect(m_dwHitTest);
								CreateDragCanvas(m_dwHitTest);//  创建拖动时的画布
								CPoint pt     = m_ptClick-rcItem.TopLeft();
								DMDragWnd::DragBegin(m_pDragCanvas,pt,0,200,LWA_ALPHA|LWA_COLORKEY);
							}
						}
						if (-1!=m_dwHitTest)  
						{
							CPoint pt0 = pt; 
							DWORD dwDragTo = HitTest(pt0);

							// 限制拖动范围
							CRect rcItem  = GetItemRect(m_dwHitTest);
							CPoint pt2(pt.x,pt.y);
							pt2.x = m_ptClick.x;// 强制坚直边对齐
							if (pt2.y<m_rcWindow.top+DMDragWnd::ms_pCurDragWnd->m_ptHot.y)// 强制不能拖出listboxex的顶部
							{
								pt2.y = m_rcWindow.top+DMDragWnd::ms_pCurDragWnd->m_ptHot.y;
							}
							if (pt.y+rcItem.Height()>m_rcWindow.bottom+DMDragWnd::ms_pCurDragWnd->m_ptHot.y)// 强制不能拖出listboxex的底部
							{
								pt2.y = m_rcWindow.bottom+DMDragWnd::ms_pCurDragWnd->m_ptHot.y-rcItem.Height();
							}
							ClientToScreen(GetContainer()->OnGetHWnd(),&pt2);
							if (m_dwDragTo != dwDragTo)
							{
								m_dwDragTo = dwDragTo;
								DrawDraggingState(dwDragTo);// 自动调整item位置
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
						LPDSLBITEMEX  pItem = m_DMArray[m_dwHitTest];
						m_DMArray.RemoveAt(m_dwHitTest);
						m_DMArray.InsertAt(m_dwDragTo,pItem);
						UpdateItemPanelId();// 更新下Panel的唯一ID
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
				NotifySelChange(m_iSelItem,m_iHoverItem);
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

	LRESULT DUIList::OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
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
	DUIWindow* DUIList::GetOwnerWindow()
	{
		return this;
	}

	DMCode DUIList::OnSetCapture(DUIItemPanel* pPanel)
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

	DMCode DUIList::OnReleaseCapture(DUIItemPanel* pPanel)
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

	DMCode DUIList::OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pPanel
				||-1==pPanel->GetItemId())
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
	DMCode DUIList::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::DUIListAttr::NODE_item);
			while (XmlItem.IsValid())
			{
				InsertItem(-1,XmlItem);
				XmlItem = XmlItem.NextSibling();
			}
			int nSelItem = -1;
			DMAttributeDispatch::ParseInt(XmlNode.Attribute(DMAttr::DUIListAttr::INT_cursel),nSelItem);
			SetCurSel(nSelItem);
		} while (false);
		return iErr;
	}

	DMCode DUIList::DV_OnSetCursor(const CPoint &pt)
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

	void DUIList::OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew)
	{
		do 
		{
			if (-1==m_iSelItem)
			{
				break;
			}
			DUIWND hDUIWnd = 0;
			GetContainer()->OnGetFocusWnd(hDUIWnd);
			if (hDUIWnd == m_hDUIWnd)
			{
				m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_KILLFOCUS,0,0);
				m_DMArray[m_iSelItem]->pPanel->OnFrameEvent(WM_SETFOCUS,0,0);
			}
		} while (false);
	}

	// 
	void DUIList::PreArrayObjRemove(const LPDSLBITEMEX &obj)
	{
		delete obj;
	}

	void DUIList::UpdateItemPanelId(int iFirst/*=0*/, int iLast/* = -1*/)
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

	void DUIList::UpdateAllPanelLayout()
	{
		int iCount  = (int)GetCount();
		for (int i=0;i<iCount;i++)
		{
			int nHei = m_DMArray[i]->bExpand?m_DMArray[i]->nExpandHeight:m_DMArray[i]->nHeight;
			CRect rcLayout(0,0,m_rcsbClient.Width(),nHei);
			m_DMArray[i]->pPanel->DM_FloatLayout(rcLayout);
		}
	}

	void DUIList::NotifySelChange(int nOldSel,int nNewSel)
	{
		do 
		{
			DMEventLBSelChangingArgs Evt(this);

			Evt.m_nOldSel = nOldSel;
			Evt.m_nNewSel = nNewSel;
			DV_FireEvent(Evt);

			if (Evt.m_bCancel)
			{
				break;
			}

			m_iSelItem = nNewSel;
			// TODO.如果要设置选中项的背景色，可以打开此代码
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
			evt2.m_nNewSel = nNewSel;
			DV_FireEvent(evt2);
		} while (false);
	}

	void DUIList::ModifyPanelBgClr(DUIItemPanel* pPanel,DMColor &Clr)
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

	int DUIList::HitTest(CPoint &pt,bool* pbInExpand)
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
				int nHei = m_DMArray[i]->bExpand?m_DMArray[i]->nExpandHeight:m_DMArray[i]->nHeight;
				nTotalHeight += nHei;
				if (pt2.y < nTotalHeight)
				{
					if (pbInExpand)
					{
						if (false == m_DMArray[i]->bExpand)
						{
							*pbInExpand = false;
						}
						else
						{
							int iDelta = pt2.y+nHei-nTotalHeight;
							if (iDelta<=m_DMArray[i]->nHeight)
							{
								*pbInExpand = false;
							}
							else
							{
								*pbInExpand = true;
							}
						}
					}
					iRet = i;
					pt.x -= rcClient.left;
					pt.y = pt2.y - nTotalHeight + nHei;
					break;
				}
			}
		} while (false);
		return iRet;
	}

	void DUIList::UpdateScrollRangeSize()
	{
		CRect rcClient;
		DUIWindow::DV_GetClientRect(&rcClient);
		int nTotalHeight = 0;
		int iCount = (int)m_DMArray.GetCount();
		for (int i = 0; i<iCount; i++)
		{
			int nHei = m_DMArray[i]->bExpand?m_DMArray[i]->nExpandHeight:m_DMArray[i]->nHeight;
			nTotalHeight += nHei;
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

	void DUIList::CreateDragCanvas(UINT iItem)
	{
		do 
		{
			if (iItem>=(int)GetCount())
			{
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			int nHei = m_DMArray[iItem]->bExpand?m_DMArray[iItem]->nExpandHeight:m_DMArray[iItem]->nHeight;
			CRect rcItem(0,0,rcClient.Width(),nHei);

			// 创建拖动画布
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

	void DUIList::DrawDraggingState(DWORD dwDragTo)
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
				if( i!=iDragFrom) //0项把所有的项都加入，除了拖动的项
				{
					items.Add(i);
				}
			}
			items.InsertAt(iDragTo,iDragFrom);// 把拖动的项加入到拖到的项之后

			int nTotalHeight = 0;
			for (UINT i=0;i<items.GetCount();i++)
			{
				int nHei = m_DMArray[i]->bExpand?m_DMArray[i]->nExpandHeight:m_DMArray[i]->nHeight;
				if ((nTotalHeight >= m_ptCurPos.y && nTotalHeight < m_ptCurPos.y + rcClient.Height())
					|| nTotalHeight + nHei >= m_ptCurPos.y && nTotalHeight + nHei < m_ptCurPos.y + m_rcsbClient.Height()
					||(nTotalHeight <= m_ptCurPos.y && nTotalHeight + nHei >= m_ptCurPos.y + rcClient.Height()))
				{
					CRect rcItem(0,0,rcClient.Width(),nHei);
					rcItem.OffsetRect(0,nTotalHeight-m_ptCurPos.y);
					rcItem.OffsetRect(rcClient.TopLeft());
					DrawItem(pCanvas,rcItem,items[i]);
				}
				nTotalHeight += nHei;
			}

			DV_PopDrawEnviron(pCanvas, DrawEnviron);
			DM_ReleaseCanvas(pCanvas);
		} while (false);
	}


}//namespace DM