#include "DmMainAfx.h"
#include "DUITreeCtrlEx.h"

namespace DM
{
	DUITreeCtrlEx::DUITreeCtrlEx()
	{
		m_pToggleSkin = m_pItemBgSkin = m_pCheckSkin = NULL;
		m_hSelItem = m_hHoverItem = m_hCaptureItem = NULL;
		m_iDefItemHei     = 20;
		m_iDefItemWid     = 50;
		m_iDefChildOffset = 16;
		m_pDUIXmlInfo->m_bFocusable = true;

		m_bRightClickSel  = m_bCheckBox = false;
		for (int i=0;i<3;i++)
		{
			m_crItemBg[i].SetTextInvalid();
		}

		SetRangeSize(CSize(0,0));
		SetRangeCurPos(CPoint(0,0));
		// treectrl
		DMADDEVENT(DMEventTCSelChangingArgs::EventID);
		DMADDEVENT(DMEventTCSelChangedArgs::EventID);
		DMADDEVENT(DMEventTCCheckStateArgs::EventID);
		DMADDEVENT(DMEventTCExpandArgs::EventID);
		DMADDEVENT(DMEventTCDbClickArgs::EventID);
	}

	DUITreeCtrlEx::~DUITreeCtrlEx()
	{

	}

	//---------------------------------------------------
	// 对外接口
	//---------------------------------------------------
	bool DUITreeCtrlEx::RemoveAllItems()
	{
		DM_RemoveAllChildPanel();
		DeleteAllItems();
		m_hSelItem		= NULL;
		m_hHoverItem	= NULL;
		m_hCaptureItem	= NULL;
		SetRangeSize(CSize(0,0));
		SetRangeCurPos(CPoint(0,0));
		return true;
	}

	bool DUITreeCtrlEx::RemoveItem(HDMTREEITEM hItem)
	{
		bool bRet = false;
		do 
		{
			if (!ItemIsValid(hItem))
			{
				break;
			}

			HDMTREEITEM hParent = GetParentItem(hItem);
			LPTVITEMEX pData = GetItem(hItem);
			if (pData->bVisible)
			{
				if (GetChildItem(hItem) && pData->bCollapsed==false)
				{// 有子项，展开状态
					SetChildrenVisible(hItem,false);
				}
			}

			if (IsAncestor(hItem,m_hHoverItem)) 
			{
				m_hHoverItem = NULL;
			}
			if (IsAncestor(hItem,m_hSelItem))
			{
				m_hSelItem = NULL;
			}
			if (IsAncestor(hItem,m_hCaptureItem)) 
			{
				m_hCaptureItem = NULL;
			}
			bool bVisible = pData->bVisible;
			int iCheckValue = pData->iCheckValue;
			DeleteItem(hItem);

			// 去掉父节点的展开标志
			if (hParent&&!GetChildItem(hParent))
			{
				LPTVITEMEX pParentData = GetItem(hParent);
				pParentData->bHasChildren = false;
				pParentData->bCollapsed = false;   
			}

			if (m_bCheckBox 
				&&hParent
				&&GetChildItem(hParent))
			{
				// 原结点复选框选中，检查父结点是否由半选变不选    
				if (iCheckValue == DMTVEXCheckBox_Checked
					||iCheckValue == DMTVEXCheckBox_PartChecked)
				{
					CheckState(hParent, false);   
				}
				//原结点复选框未选中，检查父结点是否由半选变全选    
				else
				{
					LPTVITEMEX pParentData = GetItem(hParent);
					if (pParentData->iCheckValue == DMTVEXCheckBox_PartChecked)
					{
						CheckState(hParent, true); 
					}
				}
			}

			if (bVisible)
			{
				UpdateScrollRangeSize();
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUITreeCtrlEx::SetItemRect(HDMTREEITEM hItem, CRect rcItem)
	{
		bool bRet = false;
		do 
		{		
			if (!ItemIsValid(hItem))
			{
				break;
			}

			LPTVITEMEX pData = GetItem(hItem);
			pData->pPanel->DM_FloatLayout(rcItem);
			pData->iWidth = rcItem.Width();
			pData->iHeight = rcItem.Height();
			if (pData->bVisible)
			{
				UpdateScrollRangeSize();
				DM_Invalidate();
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	HDMTREEITEM DUITreeCtrlEx::InsertItem(LPTVITEMEX pData,HDMTREEITEM hParent,HDMTREEITEM hInsertAfter,BOOL bEnsureVisible)
	{
		HDMTREEITEM hRet = NULL;
		do 
		{
			if (NULL == pData)
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);
			pData->nLevel = GetItemLevel(hParent)+1;
			if (hParent!=DMTVI_ROOT)
			{        
				LPTVITEMEX pParentItem = GetItem(hParent);
				if (pParentItem->bCollapsed || !pParentItem->bVisible) 
				{
					pData->bVisible = false;
				}

				if (!GetChildItem(hParent) && !pParentItem->bHasChildren)
				{
					pParentItem->bHasChildren = true;  // 插入一个子项了，所以为true
				}
			}   
 
			hRet = DMTreeT<LPTVITEMEX>::InsertItem(pData,hParent,hInsertAfter);
			pData->hItem = hRet;

			// 关联panel,设置绝对布局
			pData->pPanel->SetItemId((LPARAM)(pData->hItem));
			CRect rcLayout(0,0,pData->iWidth,pData->iHeight);
			pData->pPanel->DM_FloatLayout(rcLayout);
			DM_AddChildPanel(pData->pPanel);

			// 强制加上偏移
			int iOffset = 0;
			if (m_pCheckSkin&&m_bCheckBox)
			{
				CSize sz;
				m_pCheckSkin->GetStateSize(sz);
				iOffset = sz.cx;
			}

			if (m_pToggleSkin)
			{
				CSize sz;
				m_pToggleSkin->GetStateSize(sz);
				iOffset += sz.cx;
			}
			pData->iChildOffset += iOffset;

			// 更新滚动条
			if (bEnsureVisible)
			{
				EnsureVisible(hRet);
			}
		} while (false);
		return hRet;
	}

	HDMTREEITEM DUITreeCtrlEx::InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent/*=DMTVI_ROOT*/, HDMTREEITEM hInsertAfter/*=DMTVI_LAST*/,BOOL bEnsureVisible/*=FALSE*/)
	{
		LPTVITEMEX pData = new TVITEMEX(XmlItem,this);
		LoadItemAttribute(XmlItem, pData);
		return InsertItem(pData, hParent, hInsertAfter, bEnsureVisible);
	}

	///---------------------------------
	/// 获取系列函数
	//---------------------------------------------------
	HDMTREEITEM DUITreeCtrlEx::GetRootItem()
	{
		return GetChildItem(DMTVI_ROOT);
	}

	HDMTREEITEM DUITreeCtrlEx::GetRootItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEMEX>::GetRootItem(hItem);
	}

	HDMTREEITEM DUITreeCtrlEx::GetNextSiblingItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEMEX>::GetNextSiblingItem(hItem);
	}

	HDMTREEITEM DUITreeCtrlEx::GetPrevSiblingItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEMEX>::GetPrevSiblingItem(hItem);
	}

	HDMTREEITEM DUITreeCtrlEx::GetChildItem(HDMTREEITEM hItem,bool bFirst/* =TRUE*/)
	{
		return DMTreeT<LPTVITEMEX>::GetChildItem(hItem,bFirst);
	}

	HDMTREEITEM DUITreeCtrlEx::GetParentItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEMEX>::GetParentItem(hItem);
	}

	HDMTREEITEM DUITreeCtrlEx::GetSelectedItem()
	{
		return m_hSelItem;
	}

	HDMTREEITEM DUITreeCtrlEx::GetNextItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEMEX>::GetNextItem(hItem);
	}

	bool DUITreeCtrlEx::SelectItem(HDMTREEITEM hItem,bool bEnsureVisible/*=true*/)
	{
		bool bRet = false;
		do 
		{
			if (m_hSelItem==hItem)
			{
				bRet = true;
				break;
			}

			if (!ItemIsValid(hItem))
			{
				break;
			}

			DMEventTCSelChangingArgs EvtSelChanging(this);
			EvtSelChanging.m_hOldSel = m_hSelItem;
			EvtSelChanging.m_hNewSel = hItem;

			DV_FireEvent(EvtSelChanging);
			if (EvtSelChanging.m_bCancel)
			{
				break;
			}

			if (hItem&&bEnsureVisible) 
			{
				EnsureVisible(hItem);
			}

			DMEventTCSelChangedArgs EvtSelChanged(this);
			EvtSelChanged.m_hOldSel = m_hSelItem;
			EvtSelChanged.m_hNewSel = hItem;

			m_hSelItem    = hItem;
			DV_FireEvent(EvtSelChanged);

			if (EvtSelChanged.m_hOldSel)
			{
				RedrawItem(EvtSelChanged.m_hOldSel);
			}

			if (m_hSelItem)
			{
				RedrawItem(m_hSelItem);
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUITreeCtrlEx::HoverItem(HDMTREEITEM hItem,bool bEnsureVisible)
	{
		bool bRet = false;
		do 
		{
			if (m_hHoverItem==hItem)
			{
				bRet = true;
				break;
			}

			if (!ItemIsValid(hItem))
			{
				break;
			}

			if (hItem&&bEnsureVisible) 
			{
				EnsureVisible(hItem);
			}
			HDMTREEITEM hOldItem = m_hHoverItem;
			m_hHoverItem		 = hItem;
			if (hOldItem)
			{
				RedrawItem(hOldItem);
			}
			if (m_hHoverItem)
			{
				RedrawItem(m_hHoverItem);
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUITreeCtrlEx::ItemHasChildren(HDMTREEITEM hItem)
	{
		if (!hItem)
		{
			return false;
		}

		return NULL != GetChildItem(hItem);
	}

	bool DUITreeCtrlEx::SetItemData(HDMTREEITEM hItem, LPARAM lParam)
	{
		bool bRet = false;
		do 
		{
			if (hItem)
			{
				LPTVITEMEX pData = GetItem(hItem);
				if (pData)
				{
					pData->lParam = lParam;
					bRet = true;
					break;
				}
			}
		} while (false);
		return bRet;
	}

	LPARAM DUITreeCtrlEx::GetItemData(HDMTREEITEM hItem) const
	{
		if (hItem)
		{
			LPTVITEMEX pData = GetItem(hItem);
			if (pData)
			{
				return pData->lParam;
			}
		}
		return 0;
	}

	bool DUITreeCtrlEx::GetCheckState(HDMTREEITEM hItem) const
	{
		bool bRet = false;
		do 
		{
			if (!m_bCheckBox)
			{
				break;
			}

			LPTVITEMEX pData = GetItem(hItem);
			if (DMTVEXCheckBox_Checked == pData->iCheckValue)
			{
				bRet = true;
				break;
			}

		} while (false);
		return bRet;
	}

	bool DUITreeCtrlEx::SetCheckState(HDMTREEITEM hItem,bool bCheck)
	{
		bool bRet = false;
		do 
		{
			if (!m_bCheckBox)
			{
				break;
			}
			int iCheck = bCheck?DMTVEXCheckBox_Checked:DMTVEXCheckBox_UnChecked;   

			LPTVITEMEX pData = GetItem(hItem);
			pData->iCheckValue = iCheck;

			// 置子孙结点
			if (GetChildItem(hItem))
			{
				SetChildrenState(hItem, iCheck);
			}

			// 检查父结点状态
			CheckState(GetParentItem(hItem), bCheck);
			DM_Invalidate();
			bRet = true;
		} while (false);
		return bRet;	
	}

	bool DUITreeCtrlEx::Expand(HDMTREEITEM hItem,UINT nCode /*= DMTVEX_EXPAND*/)
	{
		bool bRet = false;
		do 
		{
			if (NULL == GetChildItem(hItem))
			{
				break;
			}

			LPTVITEMEX pData = GetItem(hItem);
			if (nCode==DMTVEX_COLLAPSE 
				&&!pData->bCollapsed)
			{
				pData->bCollapsed = true;
				SetChildrenVisible(hItem,false);
				bRet = true;
			}
			if (nCode==DMTVEX_EXPAND && pData->bCollapsed)
			{
				pData->bCollapsed = false;
				SetChildrenVisible(hItem,true);
				bRet = true;
			}
			if (nCode==DMTVEX_TOGGLE)
			{
				pData->bCollapsed = !pData->bCollapsed;
				SetChildrenVisible(hItem,!pData->bCollapsed);
				bRet = true;
			}
			if (bRet)
			{
				DMEventTCExpandArgs Evt(this);
				Evt.m_hItem      = hItem;
				Evt.m_bCollapsed = pData->bCollapsed;
				DV_FireEvent(Evt);
				UpdateScrollRangeSize();

				DM_Invalidate();
			}
		} while (false);
		return bRet;
	}

	bool DUITreeCtrlEx::EnsureVisible(HDMTREEITEM hItem,bool bFirstVisible/* = false*/)
	{
		bool bRet = false;
		do 
		{
			if (!ItemIsValid(hItem))
			{
				break;
			}

			LPTVITEMEX pData = GetItem(hItem);
			if (!pData->bVisible)// 全部展开
			{
				HDMTREEITEM hParent = GetParentItem(hItem);
				while (hParent)
				{
					LPTVITEMEX pParentData = GetItem(hParent);
					if (pParentData->bCollapsed)
					{
						Expand(hParent,DMTVEX_EXPAND);
					}
					hParent = GetParentItem(hParent);
				}
			}

			CRect rcItem;
			if (!GetItemRect(hItem,rcItem))
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);
			if (bFirstVisible)
			{
				int iOffset = 0;
				if (m_pCheckSkin&&m_bCheckBox)
				{
					CSize sz;
					m_pCheckSkin->GetStateSize(sz);
					iOffset = sz.cx;
				}

				LPTVITEMEX pData = GetItem(hItem);
				if (pData->bHasChildren && m_pToggleSkin)
				{
					CSize sz;
					m_pToggleSkin->GetStateSize(sz);
					iOffset += sz.cx;
				}
				SetScrollPos(true, m_ptCurPos.y-(rcClient.top-rcItem.top),true);
				SetScrollPos(false,m_ptCurPos.x-(rcClient.left-rcItem.left+iOffset),true);
			}
			else
			{
				if (rcItem.top<rcClient.top// 项在窗口上面
					||rcItem.top>rcClient.bottom)// 项在窗口下面
				{
					SetScrollPos(true, m_ptCurPos.y-(rcClient.top-rcItem.top),true);
				}
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	// 绘制
	void DUITreeCtrlEx::DrawItem(IDMCanvas* pCanvas,CRect& rc,HDMTREEITEM hItem)
	{
		do 
		{
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break; 
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);

			//1.绘制背景
			CRect rcItemBg(rcClient.left,rc.top,rcClient.right,rc.bottom);
			DUIWND_STATE iState = DUIWNDSTATE_Normal;
			if (hItem == m_hSelItem)
			{
				iState = DUIWNDSTATE_PushDown;
			}
			else if (hItem == m_hHoverItem)
			{
				iState = DUIWNDSTATE_Hover;
			}
			if (m_pItemBgSkin)
			{
				m_pItemBgSkin->Draw(pCanvas,rcItemBg,iState);
			}
			else if(!m_crItemBg[iState].IsTextInvalid())
			{
				pCanvas->FillSolidRect(rcItemBg,m_crItemBg[iState]);
			} 

			// 2.绘制小三角和checkbox
			LPTVITEMEX pData = GetItem(hItem);
			int iOffset = 0;
			if (m_pCheckSkin&&m_bCheckBox)//先绘checkbox
			{
				CSize sz;
				m_pCheckSkin->GetStateSize(sz);
				iOffset = sz.cx;
				CRect rcCheck(rc.left-sz.cx,rc.top,rc.left,rc.bottom);
				MeetRect(rcCheck,sz);
				int iImgState = IIF_STATE3(pData->dwCheckBoxState,0,1,2);
				if (pData->iCheckValue == DMTVEXCheckBox_Checked)
				{
					iImgState += 3;
				}
				else if (pData->iCheckValue == DMTVEXCheckBox_PartChecked)
				{
					iImgState += 6;
				}
				m_pCheckSkin->Draw(pCanvas,rcCheck,iImgState);
			}

			if (pData->bHasChildren && m_pToggleSkin)
			{
				int iImgState = IIF_STATE3(pData->dwToggleState,0,1,2);
				if (!pData->bCollapsed)
				{
					iImgState += 3;
				}

				CSize sz;
				m_pToggleSkin->GetStateSize(sz);
				CRect rcToggle(rc.left-iOffset-sz.cx,rc.top,rc.left-iOffset,rc.bottom);
				MeetRect(rcToggle,sz);
				m_pToggleSkin->Draw(pCanvas,rcToggle,iImgState);
			}

			// 绘制面板
			pData->pPanel->DrawItem(pCanvas,rc);
		} while (false);
	}

	void DUITreeCtrlEx::RedrawItem(HDMTREEITEM hItem)
	{
		do 
		{
			if (!DM_IsVisible(true))
			{
				break;		
			}
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break; 
			}

			LPTVITEMEX pData = GetItem(hItem);
			if (!pData->bVisible)
			{
				break;
			}

			CRect rcItem;
			if (!GetItemRect(hItem,rcItem))
			{
				break;
			}

			// 进入绘制
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CRect rcNeed(rcClient.left,rcItem.top,rcClient.right,rcItem.bottom);// 画布为整一行
			DMSmartPtrT<IDMCanvas> pCanvas = DM_GetCanvas(&rcNeed,DMOLEDC_PAINTBKGND);
			if (pCanvas)
			{
				DUIDrawEnviron DrawEnviron;
				DV_PushDrawEnviron(pCanvas,DrawEnviron);
				DrawItem(pCanvas,rcItem,hItem);
				DV_PopDrawEnviron(pCanvas,DrawEnviron);
			}
			DM_ReleaseCanvas(pCanvas);
		} while (false);
	}

	// 消息
	void DUITreeCtrlEx::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			if (DM_IsUpdateLocked())
			{
				break;
			}

			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas,DrawEnviron);
			
			CRect rcClient;
			DV_GetClientRect(rcClient);

			HDMTREEITEM hItem = GetNextItem(DMTVI_ROOT);// 从根结点开始向下查找
			int iTotalHei = 0;int iXOffset = 0;
			while (hItem) 
			{
				LPTVITEMEX pData = GetItem(hItem);
				if (pData->bVisible)
				{
					iXOffset = GetItemXOffset(hItem); 
				
					if ((iTotalHei >= m_ptCurPos.y && iTotalHei < m_ptCurPos.y + rcClient.Height())
					|| (iTotalHei + pData->iHeight >= m_ptCurPos.y && iTotalHei + pData->iHeight < m_ptCurPos.y + rcClient.Height())
					|| (iTotalHei <= m_ptCurPos.y && iTotalHei + pData->iHeight>=m_ptCurPos.y + rcClient.Height())
					)
					{
						CRect rcItem(iXOffset,iTotalHei,iXOffset+pData->iWidth,iTotalHei+pData->iHeight);// 在大平面的坐标，以大平面左上角为原点
						rcItem.OffsetRect(rcClient.TopLeft()-m_ptCurPos);// 转换成rcClient所在的坐标系坐标
						DrawItem(pCanvas,rcItem,hItem);
					}
					iTotalHei = iTotalHei+pData->iHeight;
					if (iTotalHei>=m_ptCurPos.y+rcClient.Height())// 总高度已超过可视区
					{
						break;
					}
				}

				if (pData->bCollapsed)
				{// 跳过被折叠的项
					HDMTREEITEM hChild = GetChildItem(hItem,FALSE);
					while (hChild)
					{
						hItem = hChild;
						hChild = GetChildItem(hItem,FALSE);
					}
				}
				hItem = GetNextItem(hItem);
			}
			DV_PopDrawEnviron(pCanvas,DrawEnviron);
		} while (false);
	}

	void DUITreeCtrlEx::OnDestroy()
	{
		DM_RemoveAllChildPanel();
		DeleteAllItems();
	}

	void DUITreeCtrlEx::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		m_hHoverItem = HitTest(pt);
		if (m_hHoverItem!=m_hSelItem && m_hHoverItem)
		{// 
			SelectItem(m_hHoverItem,false);
		}

		if (m_hHoverItem)
		{    
			m_hCaptureItem = m_hHoverItem;
			ItemLButtonDown(m_hHoverItem,nFlags,pt);
		}
		SetMsgHandled(FALSE);
	}

	void DUITreeCtrlEx::OnLButtonDbClick(UINT nFlags,CPoint pt)
	{
		m_hHoverItem = HitTest(pt);
		if (m_hHoverItem)
		{
			Expand(m_hHoverItem,DMTVEX_TOGGLE);
			ItemLButtonDbClick(m_hHoverItem, nFlags, pt);
		}
	}

	void DUITreeCtrlEx::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		m_hHoverItem = HitTest(pt);

		if (m_hCaptureItem)
		{
			ItemLButtonUp(m_hCaptureItem, nFlags, pt);
			m_hCaptureItem = NULL;
		}

		if (m_hHoverItem)
		{
			ItemLButtonUp(m_hHoverItem, nFlags, pt);
		}
		SetMsgHandled(FALSE);
	}

	void DUITreeCtrlEx::OnRButtonDown(UINT nFlags, CPoint pt)
	{
		do 
		{
			if (!m_bRightClickSel)
			{
				break;
			}

			m_hHoverItem = HitTest(pt);

			if (m_hHoverItem!=m_hSelItem 
				&& m_hHoverItem)
			{
				SelectItem(m_hHoverItem,false);
			}
		} while (false);	
	}

	void DUITreeCtrlEx::OnMouseMove(UINT nFlags,CPoint pt)
	{
		HDMTREEITEM hOldHoverItem = m_hHoverItem;
		do 
		{
			HDMTREEITEM hHitTest = HitTest(pt);//这里已转换pt的坐标为相对hHitTest的坐标
			// item先处理
			if (hHitTest!=m_hHoverItem)
			{
				if (m_hHoverItem)
				{
					ItemMouseLeave(m_hHoverItem);
				}
				m_hHoverItem = hHitTest;
				RedrawItem(hOldHoverItem);
			}
			if (m_hHoverItem)
			{
				ItemMouseMove(m_hHoverItem, nFlags, pt);
			}

			// panel后处理
			LPTVITEMEX pData = NULL;
			CRect rcItem;
			if (GetItemRect(hHitTest,rcItem)
				&&pt.x>=0)// 大于0表示在项内
			{
				if (m_pCapturePanel)
				{
					m_pCapturePanel->OnFrameEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(pt.x,pt.y));
					break;
				}

				if (m_hHoverItem)
				{
					pData = GetItem(m_hHoverItem);
					pData->pPanel->OnFrameEvent(WM_MOUSEHOVER,(WPARAM)nFlags,MAKELPARAM(pt.x,pt.y));
				}
			}
			CRect rcOldItem;
			if (hOldHoverItem!=m_hHoverItem)
			{
				if (NULL!= hOldHoverItem)// 给旧panel发离开消息
				{
					pData = GetItem(hOldHoverItem);
					pData->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
				}
			
				GetItemRect(hOldHoverItem,rcOldItem);
			}

			if (m_hHoverItem)
			{
				pData = GetItem(m_hHoverItem);
				pData->pPanel->OnFrameEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(pt.x,pt.y));
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);
			CRect rcNeed(rcClient.left,rcItem.top,rcClient.right,rcItem.bottom);// 画布为整一行
			if (!rcNeed.IsRectEmpty())
			{
				DM_InvalidateRect(rcNeed);
			}
			if (!rcOldItem.IsRectEmpty())
			{
				rcNeed.SetRect(rcClient.left,rcOldItem.top,rcClient.right,rcOldItem.bottom);
				DM_InvalidateRect(rcNeed);
			}
		} while (false);
		if (hOldHoverItem != m_hHoverItem)
		{
			RedrawItem(m_hHoverItem);
		}
	}

	void DUITreeCtrlEx::OnMouseLeave()
	{
		if (m_hHoverItem)
		{
			HDMTREEITEM hOldHoverItem = m_hHoverItem;
			m_hHoverItem = NULL;
			RedrawItem(hOldHoverItem);
			ItemMouseLeave(hOldHoverItem);
			if (NULL!= hOldHoverItem)// 给旧panel发离开消息
			{
				LPTVITEMEX pData = GetItem(hOldHoverItem);
				pData->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
			}
		}
	}

	void DUITreeCtrlEx::DM_OnSetFocus()
	{
		__super::DM_OnSetFocus();
		if (m_hSelItem) 
		{	
			LPTVITEMEX pData = GetItem(m_hSelItem);
			pData->pPanel->OnFrameEvent(WM_SETFOCUS,0,0);
		}
	}

	void DUITreeCtrlEx::DM_OnKillFocus()
	{
		__super::DM_OnKillFocus();
		if (m_hSelItem) 
		{	
			LPTVITEMEX pData = GetItem(m_hSelItem);
			pData->pPanel->OnFrameEvent(WM_KILLFOCUS,0,0);
		}
	}

	void DUITreeCtrlEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		DUIWindow *pOwner = DM_GetWindow(GDW_OWNER);
		if (pOwner)
		{
			pOwner->DM_SendMessage(WM_CHAR, nChar, MAKELONG(nFlags, nRepCnt));
		}
		if (m_hSelItem)
		{
			LPTVITEMEX pData = GetItem(m_hSelItem);
			pData->pPanel->OnFrameEvent(WM_CHAR,nChar, MAKELONG(nFlags, nRepCnt));
		}
	}

	void DUITreeCtrlEx::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (m_hSelItem)
			{
				LPTVITEMEX pData = GetItem(m_hSelItem);
				pData->pPanel->OnFrameEvent(WM_KEYDOWN, nChar, MAKELONG(nFlags, nRepCnt));
				if (pData->pPanel->IsMsgHandled())
				{
					break;
				}
			}

			DUIWindow *pOwner = DM_GetWindow(GDW_OWNER);
			if (pOwner 
				&&(VK_ESCAPE == nChar))
			{
				pOwner->DM_SendMessage(WM_KEYDOWN, nChar, MAKELONG(nFlags, nRepCnt));
				break;
			}

		} while (false);
	}

	BOOL DUITreeCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		return __super::OnMouseWheel(nFlags,zDelta,pt);
	}

	LRESULT DUITreeCtrlEx::OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			HDMTREEITEM hHitTest = HitTest(pt);//这里已转换pt的坐标
			CRect rcItem;
			LPTVITEMEX pData = NULL;
			if (GetItemRect(hHitTest,rcItem)
				&&pt.x>=0)
			{
				if (m_pCapturePanel)
				{
					m_pCapturePanel->OnFrameEvent(uMsg,wParam,MAKELPARAM(pt.x,pt.y));
					break;
				}

				if (uMsg == WM_LBUTTONDOWN)
				{
					int  h = 0;
				}

				if (m_hHoverItem)
				{
					pData = GetItem(m_hHoverItem);
					pData->pPanel->OnFrameEvent(uMsg,wParam,MAKELPARAM(pt.x,pt.y));
				}
			}

			if (m_pDUIXmlInfo->m_bFocusable
				&& (uMsg==WM_LBUTTONDOWN || uMsg== WM_RBUTTONDOWN || uMsg==WM_LBUTTONDBLCLK))
			{
				DV_SetFocusWnd();
			}
		} while (false);
		return lRet;
	}

	LRESULT DUITreeCtrlEx::OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRet = 0;
		do 
		{
			if (m_pCapturePanel)
			{
				lRet = m_pCapturePanel->OnFrameEvent(uMsg,wParam,lParam);
				SetMsgHandled(m_pCapturePanel->IsMsgHandled());
			}
			else if (m_hSelItem)
			{
				LPTVITEMEX pData = GetItem(m_hSelItem);
				lRet = pData->pPanel->OnFrameEvent(uMsg,wParam,lParam);
				SetMsgHandled(pData->pPanel->IsMsgHandled());
			}
			else
			{
				SetMsgHandled(FALSE);
			}
		} while (false);
		return lRet;
	}

	// 重载
	DMCode DUITreeCtrlEx::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}
			RemoveAllItems();

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::DUITreeCtrlExAttr::NODE_treeitem);
			if (XmlItem.IsValid())
			{
				LoadBranch(DMTVI_ROOT,XmlItem);
			}
		} while (false);

		UpdateScrollRangeSize();
		return DM_ECODE_OK;
	}

	void DUITreeCtrlEx::LoadBranch(HDMTREEITEM hParent,DMXmlNode &XmlItem)
	{
		while (XmlItem.IsValid())
		{
			HDMTREEITEM hItem = InsertItem(XmlItem,hParent);// 返回当前插入结点
			DMXmlNode XmlChild = XmlItem.FirstChild(DMAttr::DUITreeCtrlExAttr::NODE_treeitem);
			if (XmlChild.IsValid()) 
			{// 子分支递归
				LoadBranch(hItem,XmlChild);
			}
			XmlItem = XmlItem.NextSibling(DMAttr::DUITreeCtrlExAttr::NODE_treeitem);
		}
	}

	void DUITreeCtrlEx::LoadItemAttribute(DMXmlNode &XmlItem, LPTVITEMEX pData)
	{
		CStringW strHei	= XmlItem.Attribute(DMAttr::DUITreeCtrlExAttr::ITEM_height);
		pData->iHeight = m_iDefItemHei;
		DMAttributeDispatch::ParseInt(strHei,pData->iHeight);

		CStringW strWid	= XmlItem.Attribute(DMAttr::DUITreeCtrlExAttr::ITEM_width);
		pData->iWidth = m_iDefItemWid;
		DMAttributeDispatch::ParseInt(strWid,pData->iWidth);

		CStringW strbcollapsed = XmlItem.Attribute(DMAttr::DUITreeCtrlExAttr::ITEM_bcollapsed);
		DMAttributeDispatch::ParseBool(strbcollapsed,pData->bCollapsed);

		CStringW strChildOffset = XmlItem.Attribute(DMAttr::DUITreeCtrlExAttr::ITEM_childoffset);
		pData->iChildOffset = m_iDefChildOffset;
		DMAttributeDispatch::ParseInt(strChildOffset,pData->iChildOffset);

		CStringW strData = XmlItem.Attribute(DMAttr::DUITreeCtrlExAttr::ITEM_data);
		int iData = 0;
		DMAttributeDispatch::ParseInt(strData,iData);

		pData->lParam    = (LPARAM)iData;
	}

	DMCode DUITreeCtrlEx::DV_OnSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CPoint pnnt = pt;
			HDMTREEITEM hHitTest = HitTest(pnnt);
			if (NULL==hHitTest)
			{
				break;
			}
			int nHitTestBtn = ItemHitTest(hHitTest, pnnt);
			if (DMTVEXBtn_None!=nHitTestBtn)
			{
				break;
			}

			CRect rcItem;
			if (GetItemRect(hHitTest,rcItem)
				&&pnnt.x>=0)
			{
				if (m_pCapturePanel)
				{
					if (0!=m_pCapturePanel->OnFrameEvent(WM_SETCURSOR, 0, MAKELPARAM(pnnt.x,pnnt.y))) ///仅cursor会影响OnFrameEvent返回值
					{
						iErr = DM_ECODE_OK;
						break;
					}
				}
				if (NULL != m_hHoverItem)
				{
					LPTVITEMEX pData = GetItem(m_hHoverItem);
					if (0!=pData->pPanel->OnFrameEvent(WM_SETCURSOR, 0, MAKELPARAM(pnnt.x,pnnt.y))) 
					{
						iErr = DM_ECODE_OK;
						break;
					}
				}
			}
			iErr = __super::DV_OnSetCursor(pt);
		} while (false);
		return iErr;
	}

	DMCode DUITreeCtrlEx::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL==m_hHoverItem)
			{
				iErr = __super::DV_OnUpdateToolTip(pt, tipInfo);
				break;
			}
			LPTVITEMEX pData = GetItem(m_hHoverItem);
			iErr = pData->pPanel->DV_OnUpdateToolTip(pt, tipInfo);
		} while (false);
		return iErr;
	}

	void DUITreeCtrlEx::OnNodeFree(LPTVITEMEX &pItemData)
	{
		pItemData->pPanel->SetItemId(-1);
		DM_DELETE(pItemData);
	}

	//
	DUIWindow* DUITreeCtrlEx::GetOwnerWindow()
	{
		return this;
	}

	DMCode DUITreeCtrlEx::OnSetCapture(DUIItemPanel* pPanel)
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

	DMCode DUITreeCtrlEx::OnReleaseCapture(DUIItemPanel* pPanel)
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

	DMCode DUITreeCtrlEx::OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL; 
		do 
		{ 
			if (NULL == pPanel
				||-1 == pPanel->GetItemId())
			{
				break; 
			}
			HDMTREEITEM hItem = (HDMTREEITEM)pPanel->GetItemId();
			CRect rcItem;
			if (!GetItemRect(hItem,rcItem))
			{
				break;
			}

			*lpRect = rcItem;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	// 辅助
	void DUITreeCtrlEx::SetChildrenVisible(HDMTREEITEM hItem,bool bVisible)
	{
		HDMTREEITEM hChild = GetChildItem(hItem);
		while (hChild)
		{
			LPTVITEMEX pData = GetItem(hChild);
			pData->bVisible = bVisible;
			if (!pData->bCollapsed) 
			{
				SetChildrenVisible(hChild,bVisible);
			}
			hChild = GetNextSiblingItem(hChild);
		}
	}

	void DUITreeCtrlEx::SetChildrenState(HDMTREEITEM hItem, int iCheckValue)
	{
		HDMTREEITEM hChildItem = GetChildItem(hItem);
		while (hChildItem)
		{
			LPTVITEMEX pData = GetItem(hChildItem);
			pData->iCheckValue = iCheckValue;    
			SetChildrenState(hChildItem, iCheckValue);
			hChildItem = GetNextSiblingItem(hChildItem);
		}        
	}

	void DUITreeCtrlEx::CheckState(HDMTREEITEM hItem, bool bCheck, bool bCheckChild /*= true*/)
	{
		if (hItem)
		{
			LPTVITEMEX pData = GetItem(hItem);
			if (bCheckChild && CheckChildrenState(hItem, bCheck))
			{
				int iCheckValue = bCheck?DMTVEXCheckBox_Checked:DMTVEXCheckBox_UnChecked;
				pData->iCheckValue = iCheckValue;
				CheckState(GetParentItem(hItem), bCheck, true);
			}
			else
			{
				pData->iCheckValue = DMTVEXCheckBox_PartChecked;
				CheckState(GetParentItem(hItem),bCheck,  false);
			}
		}        
	}

	bool DUITreeCtrlEx::CheckChildrenState(HDMTREEITEM hItem,bool bCheck)
	{
		HDMTREEITEM hChildItem = GetChildItem(hItem);
		while (hChildItem)
		{
			LPTVITEMEX pData = GetItem(hChildItem);
			int iCheckValue = bCheck?DMTVEXCheckBox_Checked : DMTVEXCheckBox_UnChecked;
			if (pData->iCheckValue != iCheckValue) 
			{// 当前结点不一致立即返回
				return false;
			}
			else if (false == CheckChildrenState(hChildItem, bCheck))
			{// 检查子结点不一致立即返回
				return false;
			}

			// 检查子结点兄弟结点
			hChildItem = GetNextSiblingItem(hChildItem);
		}
		return true;
	}

	bool DUITreeCtrlEx::IsAncestor(HDMTREEITEM hItem1,HDMTREEITEM hItem2)
	{
		while (hItem2)
		{
			if (hItem2==hItem1) 
			{
				return true;
			}
			hItem2 = GetParentItem(hItem2);
		}
		return false;
	}

	bool DUITreeCtrlEx::ItemIsValid(HDMTREEITEM hItem)
	{
		bool bRet = false;
		do 
		{
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break; 
			}

			if (hItem)
			{// 判断根结点是否相同
				HDMTREEITEM hRoot = DMTreeT<LPTVITEMEX>::GetRootItem(hItem);// 得到它的首结点
				while (GetPrevSiblingItem(hRoot))
				{
					hRoot = GetPrevSiblingItem(hRoot);
				}

				if (hRoot != GetRootItem())
				{
					break;
				}
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	void DUITreeCtrlEx::ModifyToggleState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove)
	{
		LPTVITEMEX pData = GetItem(hItem);

		pData->dwToggleState |= dwStateAdd;
		pData->dwToggleState &= ~dwStateRemove;

		CRect rcToggle;
		if (GetToggleRect(hItem, rcToggle))
		{        
			//DM_InvalidateRect(rcToggle);
			DM_InvalidateRect(m_rcWindow);
		}
	}

	void DUITreeCtrlEx::ModifyChekcBoxState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove)
	{
		LPTVITEMEX pData = GetItem(hItem);

		pData->dwCheckBoxState |= dwStateAdd;
		pData->dwCheckBoxState &= ~dwStateRemove;

		CRect rcCheck;
		if (GetCheckBoxRect(hItem, rcCheck))
		{
			//DM_InvalidateRect(rcCheck);
			DM_InvalidateRect(m_rcWindow);
		}
	}

	void DUITreeCtrlEx::ItemLButtonDown(HDMTREEITEM hItem, UINT nFlags,CPoint pt)
	{
		int nHitTestBtn = ItemHitTest(hItem, pt); 
		LPTVITEMEX pData = GetItem(hItem);
		
		//清除原有pushdown按钮
		if (m_nItemPushDownBtn != nHitTestBtn)
		{
			if (m_nItemPushDownBtn == DMTVEXBtn_Toggle && 
				DUIWNDSTATE_PushDown == (pData->dwToggleState & DUIWNDSTATE_PushDown))
			{
				ModifyToggleState(hItem, 0, DUIWNDSTATE_PushDown);
			}

			if (m_nItemPushDownBtn == DMTVEXBtn_CheckBox && 
				DUIWNDSTATE_PushDown == (pData->dwCheckBoxState & DUIWNDSTATE_PushDown))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_PushDown);
			}

			m_nItemPushDownBtn = nHitTestBtn;
		}

		//置新pushdown按钮
		if (m_nItemPushDownBtn != DMTVEXBtn_None)
		{
			if (m_nItemPushDownBtn == DMTVEXBtn_Toggle && 
				DUIWNDSTATE_PushDown != (pData->dwToggleState & DUIWNDSTATE_PushDown))
			{
				ModifyToggleState(hItem, DUIWNDSTATE_PushDown, 0);
				Expand(pData->hItem, DMTVEX_TOGGLE);            
			}

			if (m_nItemPushDownBtn == DMTVEXBtn_CheckBox && 
				DUIWNDSTATE_PushDown != (pData->dwCheckBoxState & DUIWNDSTATE_PushDown))
			{
				bool bCheck = 
					pData->iCheckValue == DMTVEXCheckBox_Checked ? false:true;
				ModifyChekcBoxState(hItem, DUIWNDSTATE_PushDown, 0);
				SetCheckState(pData->hItem, bCheck);
			}
		}
	}

	void DUITreeCtrlEx::ItemLButtonUp(HDMTREEITEM hItem, UINT nFlags,CPoint pt)
	{
		LPTVITEMEX pData = GetItem(hItem);
		if (m_nItemPushDownBtn != DMTVEXBtn_None)
		{
			if (m_nItemPushDownBtn == DMTVEXBtn_Toggle && 
				DUIWNDSTATE_PushDown == (pData->dwToggleState & DUIWNDSTATE_PushDown))
			{
				ModifyToggleState(hItem, 0, DUIWNDSTATE_PushDown);
			}

			if (m_nItemPushDownBtn == DMTVEXBtn_CheckBox && 
				DUIWNDSTATE_PushDown == (pData->dwCheckBoxState & DUIWNDSTATE_PushDown))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_PushDown);
				DMEventTCCheckStateArgs evt(this);
				evt.m_hItem=hItem;
				evt.m_uCheckState = pData->dwCheckBoxState;
				DV_FireEvent(evt);
			}

			m_nItemPushDownBtn = DMTVEXBtn_None;
		}
	}

	void DUITreeCtrlEx::ItemLButtonDbClick(HDMTREEITEM hItem, UINT nFlags,CPoint pt)
	{
		int iHitTestBtn = ItemHitTest(hItem, pt);
		if (iHitTestBtn == DMTVEXBtn_CheckBox)
		{
			ItemLButtonDown(hItem, nFlags, pt);
		}
		if (hItem)
		{
			DMEventTCDbClickArgs dbClick(this);
			dbClick.m_hItem = hItem;
			DV_FireEvent(dbClick);
		}
	}

	void DUITreeCtrlEx::ItemMouseMove(HDMTREEITEM hItem, UINT nFlags,CPoint pt)
	{
		LPTVITEMEX pData = GetItem(hItem);
		int nHitTestBtn = ItemHitTest(hItem, pt);
		if (nHitTestBtn != m_nItemHoverBtn)
		{
			if (m_nItemHoverBtn == DMTVEXBtn_Toggle 
				&&DUIWNDSTATE_Hover == (pData->dwToggleState & DUIWNDSTATE_Hover))
			{
				ModifyToggleState(hItem, 0, DUIWNDSTATE_Hover);
			}

			if (m_nItemHoverBtn == DMTVEXBtn_CheckBox 
				&&DUIWNDSTATE_Hover == (pData->dwCheckBoxState & DUIWNDSTATE_Hover))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_Hover);
			}

			m_nItemHoverBtn = nHitTestBtn;
		}

		if (m_nItemHoverBtn != DMTVEXBtn_None)
		{
			if (m_nItemHoverBtn == DMTVEXBtn_Toggle 
				&&DUIWNDSTATE_Hover != (pData->dwToggleState & DUIWNDSTATE_Hover))
			{
				ModifyToggleState(hItem, DUIWNDSTATE_Hover, 0);
			}

			if (m_nItemHoverBtn == DMTVEXBtn_CheckBox 
				&&DUIWNDSTATE_Hover != (pData->dwCheckBoxState & DUIWNDSTATE_Hover))
			{
				ModifyChekcBoxState(hItem, DUIWNDSTATE_Hover, 0);
			}
		}

		/// hover绘制
		if (m_nItemHoverBtn == DMTVEXBtn_None)
		{
			RedrawItem(hItem);
		}
	}

	void DUITreeCtrlEx::ItemMouseLeave(HDMTREEITEM hItem)
	{
		LPTVITEMEX pData = GetItem(hItem);

		if (m_nItemHoverBtn != DMTVEXBtn_None)
		{
			if (m_nItemHoverBtn == DMTVEXBtn_Toggle 
				&&DUIWNDSTATE_Hover == (pData->dwToggleState & DUIWNDSTATE_Hover))
			{
				ModifyToggleState(hItem, 0, DUIWNDSTATE_Hover);
			}

			if (m_nItemHoverBtn == DMTVEXBtn_CheckBox 
				&&DUIWNDSTATE_Hover == (pData->dwCheckBoxState & DUIWNDSTATE_Hover))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_Hover);
			}

			m_nItemHoverBtn = DMTVEXBtn_None;
		}
	}

	HDMTREEITEM DUITreeCtrlEx::HitTest(CPoint &pt)
	{// 传入的pt为rcClient所在坐标系的坐标
		CRect rcClient;
		DV_GetClientRect(&rcClient);

		// ptCopy转换成滚动条大面板所在坐标系坐标
		CPoint ptCopy = pt;
		ptCopy.y -= rcClient.top - m_ptCurPos.y;
		ptCopy.x -= rcClient.left - m_ptCurPos.x;

		HDMTREEITEM hItem = GetNextItem(DMTVI_ROOT);// 从根结点开始向下查找
		int iTotalHei = 0;
		HDMTREEITEM hRet = NULL;

		int iVisible = 0;
		while (hItem) 
		{
			LPTVITEMEX pData = GetItem(hItem);
			if (pData->bVisible) 
			{
				iVisible++;
				if (iTotalHei<=ptCopy.y&&iTotalHei+pData->iHeight>=ptCopy.y)// 进入可视区
				{
					int iXOffset = GetItemXOffset(hItem); 
					CRect rcItem(iXOffset,iTotalHei,iXOffset+pData->iWidth,iTotalHei+pData->iHeight);// 在大平面的坐标，以大平面左上角为原点
					rcItem.OffsetRect(rcClient.TopLeft()-m_ptCurPos);// 转换成rcClient所在的坐标系坐标
					pt -= rcItem.TopLeft();
					hRet = hItem;
					break;
				}
				iTotalHei = iTotalHei+pData->iHeight;
			}

			if (pData->bCollapsed)
			{// 跳过被折叠的项
				HDMTREEITEM hChild = GetChildItem(hItem,FALSE);
				while (hChild)
				{
					hItem = hChild;
					hChild = GetChildItem(hItem,FALSE);
				}
			}
			hItem = GetNextItem(hItem);
		}
		//LOG_USER("iVisible:%d\n",iVisible);
		return hRet;
	}

	int DUITreeCtrlEx::ItemHitTest(HDMTREEITEM hItem,CPoint &pt) const
	{
		int iHitTestBtn = DMTVEXBtn_None;
		do 
		{
			if (pt.x>0// iChildOffset已计算了check和toggle的大小
				||NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break;
			}
			LPTVITEMEX pData = GetItem(hItem);
			if (!pData->bVisible)
			{
				break;
			}
			int iOffset = 0;
			if (m_pCheckSkin&&m_bCheckBox)
			{
				CSize sz;
				m_pCheckSkin->GetStateSize(sz);
				iOffset = sz.cx;
				if (DMABS(pt.x)<iOffset)
				{
					iHitTestBtn = DMTVEXBtn_CheckBox;
					break;
				}
			}

			if (pData->bHasChildren && m_pToggleSkin)
			{
				CSize sz;
				m_pToggleSkin->GetStateSize(sz);
				iOffset += sz.cx;
				if (DMABS(pt.x)<iOffset)
				{
					iHitTestBtn = DMTVEXBtn_Toggle;
					break;
				}
			}
		} while (false);
		//LOG_USER("iHitTestBtn:%d\n",iHitTestBtn);
		return iHitTestBtn;
	}

	int DUITreeCtrlEx::GetTotalHeight()
	{
		int iHei = 0;
		int iItemHei = 0;
		HDMTREEITEM hItem = GetNextItem(DMTVI_ROOT);
		while (hItem)
		{
			iItemHei = GetItemHeightWithAllChild(hItem);
			iHei += iItemHei;
			hItem = GetNextSiblingItem(hItem);
		}
		return iHei;
	}

	int DUITreeCtrlEx::GetTotalWidth()
	{
		int iWid = 0;
		int iItemWidth = 0;
		HDMTREEITEM hItem = GetNextItem(DMTVI_ROOT);
		while (hItem)
		{
			iItemWidth = GetItemWidthWithAllChild(hItem);
			if (iItemWidth>iWid)
			{
				iWid = iItemWidth;
			}
			hItem = GetNextSiblingItem(hItem);
		}
		return iWid;
	}

	int DUITreeCtrlEx::GetItemHeightWithAllChild(HDMTREEITEM hItem)
	{
		int iItemHei = 0;
		int iChildHei = 0;
		do 
		{
			if (NULL == hItem
			    ||DMTVI_ROOT == hItem)
			{
			 break;
			}
			LPTVITEMEX pData = GetItem(hItem);
			if (!pData->bVisible)
			{
				break;
			}
			iItemHei += pData->iHeight;
			HDMTREEITEM hChild = GetChildItem(hItem);
			while (hChild)
			{
				iChildHei = GetItemHeightWithAllChild(hChild);
				iItemHei += iChildHei;
				hChild = GetNextSiblingItem(hChild);
			 }
		} while (false);
		return iItemHei;
	}

	int DUITreeCtrlEx::GetItemWidthWithAllChild(HDMTREEITEM hItem)
	{
		int iItemWidth = 0;
		int iChildWidth = 0;
		do 
		{
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break;
			}
			LPTVITEMEX pData = GetItem(hItem);
			if (!pData->bVisible)
			{
				break;
			}

			iItemWidth = pData->iWidth+GetItemXOffset(hItem);
			HDMTREEITEM hChild = GetChildItem(hItem);
			while (hChild)
			{
				iChildWidth = GetItemWidthWithAllChild(hChild);
				if (iChildWidth > iItemWidth)
				{
					iItemWidth = iChildWidth;
				}
				hChild = GetNextSiblingItem(hChild);
			}
		} while (false);
		return iItemWidth;
	}

	int DUITreeCtrlEx::GetItemXOffset(HDMTREEITEM hItem)
	{
		int iXOffset = 0;
		do 
		{
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break;
			}
			LPTVITEMEX pData = GetItem(hItem);
			if (!pData->bVisible)
			{
				break;
			}

			iXOffset = pData->iChildOffset;
			HDMTREEITEM hParent = GetParentItem(hItem);
			while (hParent)
			{
				LPTVITEMEX pParentData = GetItem(hParent);
				iXOffset += pParentData->iChildOffset;
				hParent = GetParentItem(hParent);
			}	
		} while (false);
		return iXOffset;
	}

	int DUITreeCtrlEx::GetItemYOffset(HDMTREEITEM hItem)
	{ 
		int iYOffset = 0;
		HDMTREEITEM hCurItem = GetNextItem(DMTVI_ROOT);// 从根结点开始向下查找
		int iTotalHei = 0;
		while (hCurItem) 
		{
			LPTVITEMEX pData = GetItem(hCurItem);
			if (hItem == hCurItem)// 找到了当前节点
			{
				break;
			}
			if (pData->bVisible) 
			{
				iYOffset = iYOffset+pData->iHeight;
			}
			hCurItem = GetNextItem(hCurItem);
		}
		return iYOffset;
	}


	bool DUITreeCtrlEx::GetItemRect(HDMTREEITEM hItem,CRect &rcItem)
	{
		bool bRet = false;
		do 
		{
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break;
			}
			LPTVITEMEX pData = GetItem(hItem);
			if (!pData->bVisible)
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);

			HDMTREEITEM hNext = GetNextItem(DMTVI_ROOT);// 从根结点开始向下查找
			int iTotalHei = 0;
			while (hNext) 
			{
				pData = GetItem(hNext);
				if (pData->bVisible) 
				{
					if (hItem == hNext)
					{
						int iXOffset = GetItemXOffset(hItem); 
						rcItem.SetRect(iXOffset,iTotalHei,iXOffset+pData->iWidth,iTotalHei+pData->iHeight);
						rcItem.OffsetRect(rcClient.TopLeft()-m_ptCurPos);// 转换成rcClient所在的坐标系坐标
						bRet = true;
						break;
					}
					iTotalHei = iTotalHei+pData->iHeight;
				}

				if (pData->bCollapsed)
				{// 跳过被折叠的项
					HDMTREEITEM hChild = GetChildItem(hNext,FALSE);
					while (hChild)
					{
						hNext = hChild;
						hChild = GetChildItem(hNext,FALSE);
					}
				}
				hNext = GetNextItem(hNext);
			}
		} while (false);
		return bRet;
	}

	bool DUITreeCtrlEx::GetToggleRect(HDMTREEITEM hItem,CRect &rcToggle)
	{
		bool bRet = false;
		do 
		{
			CRect rcItem;
			if (!GetItemRect(hItem,rcItem))
			{
				break;
			}
			int iOffset = 0;
			if (m_pCheckSkin&&m_bCheckBox)
			{
				CSize sz;
				m_pCheckSkin->GetStateSize(sz);
				iOffset = sz.cx;
			}

			LPTVITEMEX pData = GetItem(hItem);
			if (pData->bHasChildren && m_pToggleSkin)
			{
				CSize sz;
				m_pToggleSkin->GetStateSize(sz);
				iOffset += sz.cx;
				rcToggle.SetRect(rcItem.left-iOffset,rcItem.top,rcItem.left-iOffset+sz.cx,rcItem.bottom);
				bRet = true;
			}
		} while (false);
		return bRet;
	}

	bool DUITreeCtrlEx::GetCheckBoxRect(HDMTREEITEM hItem,CRect &rcCheckBox)
	{
		bool bRet = false;
		do 
		{
			CRect rcItem;
			if (!GetItemRect(hItem,rcItem))
			{
				break;
			}

			if (m_pCheckSkin&&m_bCheckBox)
			{
				CSize sz;
				m_pCheckSkin->GetStateSize(sz);
				rcCheckBox.SetRect(rcItem.left-sz.cx,rcItem.top,rcItem.left,rcItem.bottom);
				bRet = true;
			}
		} while (false);
		return bRet;
	}

	void DUITreeCtrlEx::UpdateScrollRangeSize()
	{
		int iHei = GetTotalHeight();
		int iWid = GetTotalWidth();
		CSize szView(iWid,iHei);
		SetRangeSize(szView);
	}


























}//namespace DM