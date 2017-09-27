#include "DmMainAfx.h"
#include "DUITreeCtrlEx.h"

namespace DM
{
	DUITreeCtrlEx::DUITreeCtrlEx()
	{
		m_pToggleSkin = m_pItemBgSkin = m_pCheckSkin = NULL;
		m_hSelItem = m_hHoverItem = m_hCaptureItem = NULL;
		m_pCapturePanel = NULL;
		m_iDefItemHei     = 20;
		m_iDefItemWid     = 50;
		m_iDefChildOffset = 16;
		m_pDUIXmlInfo->m_bFocusable = true;
		m_dwUpdateInterval = 40;

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

	//---------------------------------------------------
	// Function Des: 对外接口 methods
	//---------------------------------------------------
#pragma region 对外接口
	bool DUITreeCtrlEx::RemoveAllItems()
	{
		DMMapT<HDMTREEITEM,CRect>::RemoveAll();
		DM_RemoveAllChildPanel();
		DeleteAllItems();
		m_hSelItem		= NULL;
		m_hHoverItem	= NULL;
		m_hCaptureItem	= NULL;
		SetRangeSize(CSize(0,0));
		SetRangeCurPos(CPoint(0,0));
		m_pCapturePanel.Release();
		return true;
	}

	bool DUITreeCtrlEx::RemoveItem(HDMTREEITEM hItem)
	{
		bool bRet = false;
		do 
		{
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
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
			DM_RemoveChildPanel(pData->pPanel);
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
				UpdateScrollRange();
				UpdateVisibleMap();
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	HDMTREEITEM DUITreeCtrlEx::InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent/*=DMTVI_ROOT*/, HDMTREEITEM hInsertAfter/*=DMTVI_LAST*/,bool bEnsureVisible/*=false*/)
	{
		LPTVITEMEX pData = new TVITEMEX(XmlItem,this);
		LoadItemAttribute(XmlItem, pData);
		return InsertItem(pData, hParent, hInsertAfter, bEnsureVisible);
	}

	bool DUITreeCtrlEx::EnsureVisible(HDMTREEITEM hItem,bool bFirstVisible /*= false*/)
	{
		bool bRet = false;
		do 
		{
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break; 
			}

			bool bExistVisibleMap = DMMapT<HDMTREEITEM,CRect>::IsKeyExist(hItem);
			if (!bExistVisibleMap)
			{
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
				int iOffset = m_szCheck.cx + m_szToggle.cx;
				SetScrollPos(true, m_ptCurPos.y-(rcClient.top-rcItem.top),true);
				SetScrollPos(false,m_ptCurPos.x-(rcClient.left-rcItem.left+iOffset),true);
				UpdateVisibleMap();
			}
			else
			{
				if (rcItem.top<rcClient.top// 项在窗口上面
					||rcItem.top>rcClient.bottom)// 项在窗口下面
				{
					SetScrollPos(true, m_ptCurPos.y-(rcClient.top-rcItem.top),true);
					UpdateVisibleMap();
				}
			}
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
				UpdateScrollRange();
				UpdateVisibleMap();
			}
		} while (false);
		return bRet;
	}

	bool DUITreeCtrlEx::SetCheckState(HDMTREEITEM hItem,bool bCheck)
	{
		bool bRet = false;
		do 
		{
			if (!m_bCheckBox||!m_pCheckSkin)
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
				LPTVITEMEX pData = GetItem(m_hSelItem);
				pData->pPanel->ModifyState(0,DUIWNDSTATE_Check);
				RedrawItem(EvtSelChanged.m_hOldSel);
			}

			if (m_hSelItem)
			{
				LPTVITEMEX pData = GetItem(m_hSelItem);
				pData->pPanel->ModifyState(DUIWNDSTATE_Check,0);
				pData->pPanel->DM_SendMessage(WM_SETFOCUS,0,0);
				RedrawItem(m_hSelItem);
			}

			POSITION pos = m_Map.GetStartPosition();
			while(pos)
			{
				DM::CMap<HDMTREEITEM,CRect>::CPair *p = m_Map.GetNext(pos);
				if (p->m_key != m_hSelItem)
				{
					LPTVITEMEX pData = GetItem(p->m_key);
					pData->pPanel->OnSetFocusWnd(NULL);
				}
			}

			bRet = true;
		} while (false);
		return bRet;
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: IDMItemPanelOwner methods
	//---------------------------------------------------
#pragma region IDMItemPanelOwner
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
#pragma endregion

	//---------------------------------------------------
	// Function Des: Draw methods
	//---------------------------------------------------
#pragma region Draw
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
				m_pItemBgSkin->Draw(pCanvas,rc,iState);
			}
			else if(!m_crItemBg[iState].IsTextInvalid())
			{
				pCanvas->FillSolidRect(rc,m_crItemBg[iState]);
			} 

			// 2.绘制小三角和checkbox
			LPTVITEMEX pData = GetItem(hItem);
			int iOffset = 0;
			if (m_pCheckSkin&&m_bCheckBox)//先绘checkbox
			{
				iOffset = m_szCheck.cx;
				CRect rcCheck(rc.left-m_szCheck.cx,rc.top,rc.left,rc.bottom);
				MeetRect(rcCheck,m_szCheck);
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
				m_pToggleSkin->GetStateSize(m_szToggle);
				CRect rcToggle(rc.left-iOffset-m_szToggle.cx,rc.top,rc.left-iOffset,rc.bottom);
				MeetRect(rcToggle,m_szToggle);
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
			CRect rcItem;
			if (!DMMapT<HDMTREEITEM,CRect>::GetObjByKey(hItem,rcItem))
			{
				break;
			}
			DM_InvalidateRect(rcItem);
		} while (false);
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: DV methods
	//---------------------------------------------------
#pragma region DV
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
		UpdateScrollRange();
		return DM_ECODE_OK;
	}

	DMCode DUITreeCtrlEx::DV_OnSetCursor(const CPoint &pt)
	{
		return DM_ECODE_FAIL;
#if 1
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
#endif
	}

	DMCode DUITreeCtrlEx::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{
		return DM_ECODE_FAIL;
#if 1
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
#endif
	}

	bool DUITreeCtrlEx::DV_IsSupportFastRefresh()
	{
		return true;
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
		pData->lParam = (LPARAM)iData;
	}

	bool DUITreeCtrlEx::OnScroll(bool bVert,UINT uCode,int nPos)
	{
		bool bRet = __super::OnScroll(bVert, uCode, nPos);
		UpdateVisibleMap();
		if (SB_THUMBTRACK == uCode)
		{
			ScrollUpdateWindow();
		}
		return bRet;
	}

	int DUITreeCtrlEx::GetItemWidth(HDMTREEITEM hItem)
	{
		int iWidth = 0;
		do 
		{
			if (NULL == hItem
				||DMTVI_ROOT == hItem)
			{
				break;
			}
			LPTVITEMEX pData = GetItem(hItem);
			iWidth = pData->iWidth;
		} while (false);
		return iWidth;
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

			iItemWidth = GetItemWidth(hItem)+GetItemXOffset(hItem);
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
	{// hItem必须是可见的
		int iYOffset = 0;

		CRect rcItem;
		if (DMMapT<HDMTREEITEM,CRect>::GetObjByKey(hItem,rcItem))
		{// 优先从可视列表中查找
			CRect rcClient;
			DV_GetClientRect(rcClient);
			rcItem.OffsetRect(m_ptCurPos-rcClient.TopLeft());
			iYOffset = rcItem.top;
			return iYOffset;
		}

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
			if (pData->bCollapsed)
			{// 跳过被折叠的项
				HDMTREEITEM hChild = GetChildItem(hCurItem,false);
				while (hChild)
				{
					hCurItem = hChild;
					hChild = GetChildItem(hCurItem,false);
				}
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

			// 先从可视列表中查找
			if (DMMapT<HDMTREEITEM,CRect>::GetObjByKey(hItem,rcItem))
			{
				bRet = true;
				break;
			}

			// 遍历查找
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
						rcItem.SetRect(iXOffset,iTotalHei,iXOffset+ GetItemWidth(hItem),iTotalHei+pData->iHeight);
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
			if (!m_pToggleSkin)
			{
				break;
			}

			CRect rcItem;
			if (!GetItemRect(hItem,rcItem))
			{
				break;
			}
			int iOffset = 0;
			if (m_pCheckSkin&&m_bCheckBox)
			{
				iOffset = m_szCheck.cx;
			}

			LPTVITEMEX pData = GetItem(hItem);
			if (pData->bHasChildren && m_pToggleSkin)
			{
				iOffset += m_szToggle.cx;
				rcToggle.SetRect(rcItem.left-iOffset,rcItem.top,rcItem.left-iOffset+m_szToggle.cx,rcItem.bottom);
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
			if (!m_bCheckBox || !m_pCheckSkin)
			{
				break;
			}

			CRect rcItem;
			if (!GetItemRect(hItem,rcItem))
			{
				break;
			}

			if (m_pCheckSkin&&m_bCheckBox)
			{
				rcCheckBox.SetRect(rcItem.left-m_szCheck.cx,rcItem.top,rcItem.left,rcItem.bottom);
				bRet = true;
			}
		} while (false);
		return bRet;
	}


	HDMTREEITEM DUITreeCtrlEx::HitTest(CPoint &pt)
	{// 传入的pt为rcClient所在坐标系的坐标
		HDMTREEITEM hRet = NULL;
		POSITION pos = m_Map.GetStartPosition();
		while(pos)
		{
			DM::CMap<HDMTREEITEM,CRect>::CPair *p = m_Map.GetNext(pos);
			if(p->m_value.top<=pt.y&&p->m_value.bottom>=pt.y)// 进入可视区,水平可能为负值
			{
				pt -= p->m_value.TopLeft();
				hRet = p->m_key;
				break;
			}
		}
		return hRet;
	}

	int DUITreeCtrlEx::ItemHitTest(HDMTREEITEM hItem,CPoint &pt)
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
				iOffset = m_szCheck.cx;
				if (DMABS(pt.x)<iOffset)
				{
					iHitTestBtn = DMTVEXBtn_CheckBox;
					break;
				}
			}

			if (pData->bHasChildren && m_pToggleSkin)
			{
				iOffset += m_szToggle.cx;
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

	void DUITreeCtrlEx::UpdateScrollRange()
	{
		int iHei = GetTotalHeight();
		int iWid = GetTotalWidth();
		CSize szView(iWid,iHei);
		SetRangeSize(szView);
	}

	void DUITreeCtrlEx::UpdateVisibleMap()
	{
		DMMapT<HDMTREEITEM,CRect>::RemoveAll();
		CRect rcClient;
		DV_GetClientRect(rcClient);
		if (rcClient.IsRectEmpty())
		{
			return;
		}

		HDMTREEITEM hItem = GetNextItem(DMTVI_ROOT);// 从根结点开始向下查找
		int iTotalHei = 0;int iXOffset = 0;
		while (hItem) 
		{
			LPTVITEMEX pData = GetItem(hItem);
			if (pData->bVisible)
			{
				if ((iTotalHei >= m_ptCurPos.y && iTotalHei < m_ptCurPos.y + rcClient.Height())
					|| (iTotalHei + pData->iHeight >= m_ptCurPos.y && iTotalHei + pData->iHeight < m_ptCurPos.y + rcClient.Height())
					|| (iTotalHei <= m_ptCurPos.y && iTotalHei + pData->iHeight>=m_ptCurPos.y + rcClient.Height())
					)
				{
					iXOffset = GetItemXOffset(hItem); 
					CRect rcItem(iXOffset,iTotalHei,iXOffset+ GetItemWidth(hItem),iTotalHei+pData->iHeight);// 在大平面的坐标，以大平面左上角为原点
					rcItem.OffsetRect(rcClient.TopLeft()-m_ptCurPos);// 转换成rcClient所在的坐标系坐标
					if (m_hSelItem != hItem)
					{
						pData->pPanel->OnSetFocusWnd(NULL);
					}
					DMMapT<HDMTREEITEM,CRect>::AddKey(hItem,rcItem);
				}
				iTotalHei = iTotalHei+pData->iHeight;
				if (iTotalHei>=m_ptCurPos.y+rcClient.Height())// 总高度已超过可视区
				{
					break;
				}
			}

			if (pData->bCollapsed)
			{// 跳过被折叠的项
				HDMTREEITEM hChild = GetChildItem(hItem,false);
				while (hChild)
				{
					hItem = hChild;
					hChild = GetChildItem(hItem,false);
				}
			}
			hItem = GetNextItem(hItem);
		}
	}

	void DUITreeCtrlEx::OnNodeFree(LPTVITEMEX &pItemData)
	{
		pItemData->pPanel->SetItemId(-1);
		DM_DELETE(pItemData);
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: 辅助 methods
	//---------------------------------------------------
#pragma region 辅助
	HDMTREEITEM DUITreeCtrlEx::InsertItem(LPTVITEMEX pData,HDMTREEITEM hParent,HDMTREEITEM hInsertAfter,bool bEnsureVisible)
	{
		HDMTREEITEM hRet = NULL;
		do 
		{
			DMASSERT(pData);
			if (DMTVI_ROOT != hParent)
			{
				LPTVITEMEX pParentData = GetItem(hParent);
				pData->nLevel = pParentData->nLevel + 1;
				if (pParentData->bCollapsed || !pParentData->bVisible) 
				{
					pData->bVisible = false;
				}
				if (!pParentData->bHasChildren)
				{
					pParentData->bHasChildren = true;  // 插入一个子项了，所以为true
				}
			}
			hRet = pData->hItem = DMTreeT<LPTVITEMEX>::InsertItem(pData,hParent,hInsertAfter);
			pData->pPanel->SetItemId((LPARAM)(pData->hItem));
			CRect rcLayout(0,0, GetItemWidth(hRet),pData->iHeight);
			pData->pPanel->DM_FloatLayout(rcLayout);
			DM_AddChildPanel(pData->pPanel);	// 关联panel
			if (DMTVI_ROOT == hParent)			// 根结点的iChildOffset为它的实际偏移+Check+Toggle，其他结点为相对于父节点的偏移
			{
				pData->iChildOffset += m_szCheck.cx + m_szToggle.cx;
			}

			if (bEnsureVisible)
			{
				EnsureVisible(hRet);
			}
		} while (false);
		return hRet;
	}

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

	void DUITreeCtrlEx::SetChildrenState(HDMTREEITEM hItem,int iCheckValue)
	{
		if (!m_bCheckBox || !m_pCheckSkin)
		{
			return;
		}
		HDMTREEITEM hChildItem = GetChildItem(hItem);
		while (hChildItem)
		{
			LPTVITEMEX pData = GetItem(hChildItem);
			pData->iCheckValue = iCheckValue;    
			SetChildrenState(hChildItem, iCheckValue);
			hChildItem = GetNextSiblingItem(hChildItem);
		}        
	}

	void DUITreeCtrlEx::CheckState(HDMTREEITEM hItem, bool bCheck,bool bCheckChild /*= true*/)
	{
		if (!m_bCheckBox || !m_pCheckSkin)
		{
			return;
		}
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
		if (!m_bCheckBox || !m_pCheckSkin)
		{
			return false;
		}
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

	void DUITreeCtrlEx::ModifyToggleState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove)
	{
		LPTVITEMEX pData = GetItem(hItem);

		pData->dwToggleState |= dwStateAdd;
		pData->dwToggleState &= ~dwStateRemove;

		CRect rcToggle;
		if (GetToggleRect(hItem, rcToggle))
		{        
			DM_InvalidateRect(rcToggle);
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
			DM_InvalidateRect(rcCheck);
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

			if (m_bCheckBox && m_pCheckSkin && m_nItemHoverBtn == DMTVEXBtn_CheckBox 
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

			if (m_bCheckBox && m_pCheckSkin && m_nItemHoverBtn == DMTVEXBtn_CheckBox 
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

			if (m_bCheckBox && m_pCheckSkin && m_nItemHoverBtn == DMTVEXBtn_CheckBox 
				&&DUIWNDSTATE_Hover == (pData->dwCheckBoxState & DUIWNDSTATE_Hover))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_Hover);
			}

			m_nItemHoverBtn = DMTVEXBtn_None;
		}
	}
#pragma endregion


	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
#pragma region MsgDispatch
	void DUITreeCtrlEx::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			CRect rcInvalid;
			pCanvas->GetClipBox(rcInvalid);
			if (rcInvalid.IsRectEmpty())
			{
				break;
			}
			pCanvas->PushClip(m_rcWindow);
			if (m_hHoverItem)
			{
				CRect rcHover;
				if (DMMapT<HDMTREEITEM,CRect>::GetObjByKey(m_hHoverItem,rcHover))
				{
					CRect rcNeed;rcNeed.UnionRect(rcHover,rcInvalid);
					if (rcNeed.EqualRect(rcHover))
					{
						DrawItem(pCanvas,rcHover,m_hHoverItem);
						pCanvas->PopClip();
						break;
					}
				}
			}
			if (m_hSelItem)
			{
				CRect rcSel;
				if (DMMapT<HDMTREEITEM,CRect>::GetObjByKey(m_hSelItem,rcSel))
				{
					CRect rcNeed;rcNeed.UnionRect(rcSel,rcInvalid);
					if (rcNeed.EqualRect(rcSel))
					{
						DrawItem(pCanvas,rcSel,m_hSelItem);
						pCanvas->PopClip();
						break;
					}
				}
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			POSITION pos = m_Map.GetStartPosition();
			while(pos)
			{
				DM::CMap<HDMTREEITEM,CRect>::CPair *p = m_Map.GetNext(pos);
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

	void DUITreeCtrlEx::OnSize(UINT nType, CSize size)
	{
		CRect rcOld = m_rcWindow;
		__super::OnSize(nType,size);
		UpdateVisibleMap();
	}

	void DUITreeCtrlEx::OnDestroy()
	{
		DMMapT<HDMTREEITEM,CRect>::RemoveAll();
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

	void DUITreeCtrlEx::DM_OnSetFocus()
	{
		__super::DM_OnSetFocus();
		if (m_hSelItem) 
		{	
			LPTVITEMEX pData = GetItem(m_hSelItem);
			pData->pPanel->m_FocusMgr.RestoreFocuseWnd();
		}
	}

	void DUITreeCtrlEx::DM_OnKillFocus()
	{
		__super::DM_OnKillFocus();
		if (m_hSelItem) 
		{	
			LPTVITEMEX pData = GetItem(m_hSelItem);
			pData->pPanel->m_FocusMgr.StoreFocuseWnd();
		}
	}

	void DUITreeCtrlEx::OnMouseLeave()
	{
		__super::OnMouseLeave();
		if(m_hHoverItem)
		{
			LPTVITEMEX pData = GetItem(m_hHoverItem);
			pData->pPanel->OnFrameEvent(WM_MOUSELEAVE,0,0);
			pData->pPanel->DM_Invalidate();
			m_hHoverItem = NULL;
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
			if (WM_MOUSEMOVE == uMsg)
			{
				m_dwStartTime = ::GetTickCount();
			}
			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			HDMTREEITEM hHitTest = HitTest(pt);//这里已转换pt的坐标
			if (m_pCapturePanel)
			{
				m_pCapturePanel->OnFrameEvent(uMsg,wParam,MAKELPARAM(pt.x,pt.y));
				break;// 跳出
			}

			if (m_pDUIXmlInfo->m_bFocusable
				&& (uMsg==WM_LBUTTONDOWN || uMsg== WM_RBUTTONDOWN || uMsg==WM_LBUTTONDBLCLK))
			{
				DV_SetFocusWnd();
			}

			if (hHitTest != m_hHoverItem)
			{
				HDMTREEITEM hOldItem = m_hHoverItem;
				m_hHoverItem		 = hHitTest;
				if (hOldItem)
				{
					ItemMouseLeave(hOldItem);
					LPTVITEMEX pData = GetItem(hOldItem);
					pData->pPanel->OnFrameEvent(WM_MOUSELEAVE, 0, 0);
					pData->pPanel->DM_Invalidate();
				}
				if (m_hHoverItem)
				{
					ItemMouseMove(m_hHoverItem,0,pt);
					LPTVITEMEX pData = GetItem(m_hHoverItem);
					pData->pPanel->OnFrameEvent(WM_MOUSEHOVER, 0, 0);
					pData->pPanel->DM_Invalidate();
				}
			}
			if (m_hHoverItem)
			{
				LPTVITEMEX pData = GetItem(m_hHoverItem);
				pData->pPanel->OnFrameEvent(uMsg,wParam,MAKELPARAM(pt.x,pt.y));
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
#pragma endregion


	DMCode DUITreeCtrlEx::OnAttributeToggleSkin(LPCWSTR pszValue, bool bLoadXml)
	{
		m_pToggleSkin = g_pDMApp->GetSkin(pszValue);
		if (m_pToggleSkin)
		{
			m_pToggleSkin->GetStateSize(m_szToggle);
		}
		return DM_ECODE_OK;
	}

	DMCode DUITreeCtrlEx::OnAttributeCheckSkin(LPCWSTR pszValue, bool bLoadXml)
	{
		m_pCheckSkin = g_pDMApp->GetSkin(pszValue);
		if (m_pCheckSkin)
		{
			m_pCheckSkin->GetStateSize(m_szCheck);
		}
		return DM_ECODE_OK;
	}


















}