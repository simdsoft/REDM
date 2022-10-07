#include "DmMainAfx.h"
#include "DUITreeCtrl.h"

namespace DM
{
	DUITreeCtrl::DUITreeCtrl()
	{
		Init();
	}

	DUITreeCtrl::~DUITreeCtrl()
	{
		UnInit();
	}

	void DUITreeCtrl::Init()
	{
		m_iItemHei        = 20;
		m_iChildOffset    = 16;
		m_iItemMargin     = 4;
		m_crItemBg        = PBGRA(255,255,255,255);
		m_crItemHoverBg   =  PBGRA(255,255,255,255);
		m_crItemSelBg     = PBGRA(136,0,0,255);
		m_crItemText      = PBGRA(0,0,0,255);
		m_crItemHoverText = PBGRA(0,0,0,255);
		m_crItemSelText   = PBGRA(0xcc,0xcc,0xcc,255);
		m_nVisibleItems   = 0;
		m_nMaxItemWidth   = 0;
		m_bCheckBox       = false;
		m_bRightClickSel  = false;
		m_bHover          = true;
		m_uItemMask       = 0;
		m_iTextOffset     = 0;
		m_nItemHoverBtn   = DMTVIBtn_None;
		m_nItemPushDownBtn= DMTVIBtn_None;

		m_hSelItem = m_hHoverItem = m_hCaptureItem = NULL;
		m_pItemBgSkin = m_pItemHoverSkin = m_pItemSelSkin = m_pIconSkin = m_pToggleSkin = m_pCheckSkin = NULL;

		//
		m_pDUIXmlInfo->m_bOnlyDrawClient = true;
		m_pDUIXmlInfo->m_bFocusable		= true;

		// treectrl
		DMADDEVENT(DMEventTCSelChangingArgs::EventID);
		DMADDEVENT(DMEventTCSelChangedArgs::EventID);
		DMADDEVENT(DMEventTCCheckStateArgs::EventID);
		DMADDEVENT(DMEventTCExpandArgs::EventID);
		DMADDEVENT(DMEventTCDbClickArgs::EventID);
	}

	void DUITreeCtrl::UnInit()
	{

	}

	//---------------------------------------------------
	// Function Des: ����ӿ�
	//---------------------------------------------------
	bool DUITreeCtrl::RemoveAllItems()
	{
		DeleteAllItems();
		m_nVisibleItems = 0;
		m_hSelItem		= NULL;
		m_hHoverItem	= NULL;
		m_hCaptureItem	= NULL;
		m_nMaxItemWidth	= 0;    
		SetRangeSize(CSize(0,0));
		SetRangeCurPos(CPoint(0,0));
		return true;
	}

	bool DUITreeCtrl::RemoveItem(HDMTREEITEM hItem)
	{   
		bool bRet = false;
		do 
		{
			if (NULL == hItem)
			{
				break;
			}
			HDMTREEITEM hParent = GetParentItem(hItem);
			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
			bool bVisible = pItem->bVisible;
			int nItemWidth = GetMaxItemWidth(hItem);
			int nCheckBoxValue = pItem->nCheckBoxValue;

			if (bVisible)
			{
				if (GetChildItem(hItem) && pItem->bCollapsed==false)
				{// �����չ��״̬
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
			DeleteItem(hItem);

			// ȥ�����ڵ��չ����־
			if (hParent&&!GetChildItem(hParent))
			{
				LPTVITEM pParent = GetItem(hParent);
				pParent->bHasChildren = false;
				pParent->bCollapsed = false;
				CalaItemContentWidth(pParent);            
			}

			if (m_bCheckBox 
				&&hParent
				&&GetChildItem(hParent))
			{
				//ԭ��㸴ѡ��ѡ�У���鸸����Ƿ��ɰ�ѡ�䲻ѡ    
				if (nCheckBoxValue == DMTVICheckBox_Checked
					||nCheckBoxValue == DMTVICheckBox_PartChecked)
				{
					CheckState(hParent, false);   
				}
				//ԭ��㸴ѡ��δѡ�У���鸸����Ƿ��ɰ�ѡ��ȫѡ    
				else
				{
					LPTVITEM pParentData = GetItem(hParent);
					if (pParentData->nCheckBoxValue == DMTVICheckBox_PartChecked)
					{
						CheckState(hParent, true); 
					}
				}
			}

			if (bVisible)
			{
				m_nVisibleItems--;

				//���¼���x���ߴ�
				if (nItemWidth == m_nMaxItemWidth)
				{
					GetMaxItemWidth();  
				}

				CSize szView(m_nMaxItemWidth,m_nVisibleItems*m_iItemHei);
				SetRangeSize(szView);
				DM_Invalidate();
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	HDMTREEITEM DUITreeCtrl::InsertItem(LPCWSTR lpszItem, HDMTREEITEM hParent, HDMTREEITEM hInsertAfter,bool bEnsureVisible)
	{
		return InsertItem(lpszItem, -1, -1, NULL,  hParent, hInsertAfter,bEnsureVisible);
	}

	HDMTREEITEM DUITreeCtrl::InsertItem(LPCWSTR lpszItem, int nImage,
		int nSelectedImage, HDMTREEITEM hParent, HDMTREEITEM hInsertAfter,bool bEnsureVisible)
	{
		return InsertItem(lpszItem, nImage, nSelectedImage, NULL,  hParent, hInsertAfter,bEnsureVisible);
	}

	HDMTREEITEM DUITreeCtrl::InsertItem(LPCWSTR lpszItem, int nImage,
		int nSelectedImage, LPARAM lParam,
		HDMTREEITEM hParent, HDMTREEITEM hInsertAfter,bool bEnsureVisible)
	{
		LPTVITEM pItemObj = new TVITEM();

		pItemObj->strText = lpszItem;
		pItemObj->nImage  = nImage;
		pItemObj->nSelectedImage  = nSelectedImage;
		pItemObj->lParam = lParam;

		return InsertItem(pItemObj, hParent, hInsertAfter, bEnsureVisible);
	}

	HDMTREEITEM DUITreeCtrl::InsertItem(LPTVITEM pItemObj,HDMTREEITEM hParent,HDMTREEITEM hInsertAfter,bool bEnsureVisible)
	{
		HDMTREEITEM hRet = NULL;
		do 
		{
			if (NULL == pItemObj)
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);
			pItemObj->nLevel = GetItemLevel(hParent)+1;

			if (hParent!=DMTVI_ROOT)
			{        
				LPTVITEM pParentItem = GetItem(hParent);
				if (pParentItem->bCollapsed || !pParentItem->bVisible) 
				{
					pItemObj->bVisible = false;
				}

				if (!GetChildItem(hParent) && !pParentItem->bHasChildren)
				{
					pParentItem->bHasChildren = true;  // ����һ�������ˣ�����Ϊtrue
					CalaItemContentWidth(pParentItem);
				}
			}    

			CalaItemContentWidth(pItemObj);

			hRet = DMTreeT<LPTVITEM>::InsertItem(pItemObj,hParent,hInsertAfter);
			pItemObj->hItem = hRet;

			int nViewWidth  = 0;
			if (pItemObj->bVisible)
			{
				m_nVisibleItems++;

				nViewWidth = max(rcClient.Width(), CalcItemWidth(pItemObj));
				if (nViewWidth>m_nMaxItemWidth) 
				{
					m_nMaxItemWidth = nViewWidth;
				}

				CSize szView(m_nMaxItemWidth, m_nVisibleItems*m_iItemHei);
				SetRangeSize(szView);
				DM_Invalidate();
			}

			if (bEnsureVisible)
			{
				EnsureVisible(hRet);
			}
		} while (false);
		return hRet;
	}

	HDMTREEITEM DUITreeCtrl::InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent/*=DMTVI_ROOT*/, HDMTREEITEM hInsertAfter/*=DMTVI_LAST*/,bool bEnsureVisible/*=false*/)
	{
		LPTVITEM pItemObj = new TVITEM();
		LoadItemAttribute(XmlItem, pItemObj);
		return InsertItem(pItemObj, hParent, hInsertAfter, bEnsureVisible);
	}

	HDMTREEITEM DUITreeCtrl::GetRootItem()
	{
		return GetChildItem(DMTVI_ROOT);
	}

	HDMTREEITEM DUITreeCtrl::GetNextSiblingItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEM>::GetNextSiblingItem(hItem);
	}

	HDMTREEITEM DUITreeCtrl::GetPrevSiblingItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEM>::GetPrevSiblingItem(hItem);
	}

	HDMTREEITEM DUITreeCtrl::GetChildItem(HDMTREEITEM hItem,bool bFirst/* =true*/)
	{
		return DMTreeT<LPTVITEM>::GetChildItem(hItem,bFirst);
	}

	HDMTREEITEM DUITreeCtrl::GetParentItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEM>::GetParentItem(hItem);
	}

	HDMTREEITEM DUITreeCtrl::GetSelectedItem()
	{
		return m_hSelItem;
	}

	HDMTREEITEM DUITreeCtrl::GetNextItem(HDMTREEITEM hItem)
	{
		return DMTreeT<LPTVITEM>::GetNextItem(hItem);
	}

	bool DUITreeCtrl::SelectItem(HDMTREEITEM hItem,bool bEnsureVisible/*=true*/)
	{
		bool bRet = false;
		do 
		{
			if (m_hSelItem==hItem)
			{
				bRet = true;
				break;
			}
			if (hItem)
			{
				HDMTREEITEM hRoot = DMTreeT<LPTVITEM>::GetRootItem(hItem);
				while (DMTreeT<LPTVITEM>::GetPrevSiblingItem(hRoot))
				{
					hRoot = DMTreeT<LPTVITEM>::GetPrevSiblingItem(hRoot);
				}

				if (hRoot != GetRootItem())
				{
					break;
				}
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

	CStringW DUITreeCtrl::GetItemText(HDMTREEITEM hItem) const
	{
		CStringW strRet;
		do 
		{
			if (hItem)
			{
				LPTVITEM pItem=DMTreeT<LPTVITEM>::GetItem(hItem);
				if (pItem)
				{
					strRet = pItem->strText;
					break;
				}
			}

		} while (false);

		return strRet;
	}

	bool DUITreeCtrl::SetItemText(HDMTREEITEM hItem, LPCWSTR lpszItem)
	{
		bool bRet = false;
		do 
		{
			if (hItem)
			{
				LPTVITEM pItem=DMTreeT<LPTVITEM>::GetItem(hItem);
				if (pItem)
				{
					pItem->strText = lpszItem;
					bRet = true;
					break;
				}
			}
		} while (false);
		return bRet;
	}

	bool DUITreeCtrl::GetItemImage(HDMTREEITEM hItem, int& nImage, int& nSelectedImage) const
	{
		bool bRet = false;
		do 
		{
			if (hItem)
			{
				LPTVITEM pItem=DMTreeT<LPTVITEM>::GetItem(hItem);
				if (pItem)
				{
					nImage = pItem->nImage;
					nSelectedImage = pItem->nSelectedImage;
					bRet = true;
					break;
				}
			}
		} while (false);
		return bRet;
	}

	LPARAM DUITreeCtrl::GetItemData(HDMTREEITEM hItem) const
	{
		if (hItem)
		{
			LPTVITEM pItem=DMTreeT<LPTVITEM>::GetItem(hItem);
			if (pItem)
			{
				return pItem->lParam;
			}
		}
		return 0;
	}

	bool DUITreeCtrl::SetItemData(HDMTREEITEM hItem, LPARAM lParam)
	{
		bool bRet = false;
		do 
		{
			if (hItem)
			{
				LPTVITEM pItem=DMTreeT<LPTVITEM>::GetItem(hItem);
				if (pItem)
				{
					pItem->lParam = lParam;
					bRet = true;
					break;
				}
			}
		} while (false);
		return bRet;
	}

	bool DUITreeCtrl::ItemHasChildren(HDMTREEITEM hItem) 
	{
		if (!hItem)
		{
			return false;
		}

		return GetChildItem(hItem)!=NULL;
	}

	bool DUITreeCtrl::GetCheckState(HDMTREEITEM hItem) const
	{
		bool bRet = false;
		do 
		{
			if (!m_bCheckBox)
			{
				break;
			}

			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
			if (pItem->nCheckBoxValue == DMTVICheckBox_Checked)
			{
				bRet = true;
				break;
			}

		} while (false);
		return bRet;
	}

	bool DUITreeCtrl::SetCheckState(HDMTREEITEM hItem, bool bCheck)
	{
		bool bRet = false;
		do 
		{
			if (!m_bCheckBox)
			{
				break;
			}
			int nCheck = bCheck?DMTVICheckBox_Checked:DMTVICheckBox_UnChecked;   

			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
			pItem->nCheckBoxValue = nCheck;

			// ��������
			if (DMTreeT<LPTVITEM>::GetChildItem(hItem))
			{
				SetChildrenState(hItem, nCheck);
			}

			// ��鸸���״̬
			CheckState(GetParentItem(hItem), bCheck);
			DM_Invalidate();
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUITreeCtrl::Expand(HDMTREEITEM hItem, UINT nCode)
	{
		bool bRet = false;
		do 
		{
			if (NULL == DMTreeT<LPTVITEM>::GetChildItem(hItem))
			{
				break;
			}

			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
			if (nCode==TVE_COLLAPSE 
				&&!pItem->bCollapsed)
			{
				pItem->bCollapsed = true;
				SetChildrenVisible(hItem,false);
				bRet = true;
			}
			if (nCode==TVE_EXPAND && pItem->bCollapsed)
			{
				pItem->bCollapsed = false;
				SetChildrenVisible(hItem,true);
				bRet = true;
			}
			if (nCode==TVE_TOGGLE)
			{
				pItem->bCollapsed = !pItem->bCollapsed;
				SetChildrenVisible(hItem,!pItem->bCollapsed);
				bRet = true;
			}
			if (bRet)
			{
				DMEventTCExpandArgs Evt(this);
				Evt.m_hItem      = hItem;
				Evt.m_bCollapsed = pItem->bCollapsed;
				DV_FireEvent(Evt);

				GetMaxItemWidth();
				CSize szView(m_nMaxItemWidth,m_nVisibleItems*m_iItemHei);
				SetRangeSize(szView);
				DM_Invalidate();
			}
		} while (false);
		return bRet;
	}

	bool DUITreeCtrl::EnsureVisible(HDMTREEITEM hItem)
	{
		bool bRet = false;
		do 
		{
			if (DMTreeT<LPTVITEM>::GetRootItem(hItem) != GetRootItem())
			{
				break;
			}

			LPTVITEM pItem = GetItem(hItem);
			if (!pItem->bVisible)
			{
				HDMTREEITEM hParent = GetParentItem(hItem);
				while (hParent)
				{
					LPTVITEM pParent = GetItem(hParent);
					if (pParent->bCollapsed)
					{
						Expand(hParent,TVE_EXPAND);
					}
					hParent=GetParentItem(hParent);
				}
			}
			int iVisible = GetItemShowIndex(hItem);
			int yOffset  = iVisible*m_iItemHei;
			if (yOffset+m_iItemHei>m_ptCurPos.y+m_rcsbClient.Height())
			{
				SetScrollPos(true,yOffset+m_iItemHei-m_rcsbClient.Height(),true);
			}
			else if (yOffset<m_ptCurPos.y)
			{
				SetScrollPos(true,yOffset,true);    
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	//---------------------------------------------------
	// ����
	void DUITreeCtrl::DrawItem(IDMCanvas* pCanvas, CRect& rc, HDMTREEITEM hItem)
	{    
		do 
		{
			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
			pCanvas->OffsetViewportOrg(rc.left+m_iChildOffset*pItem->nLevel,rc.top);

			CRect rcItemBg;
			rcItemBg.SetRect(m_iTextOffset+m_iItemMargin, 0, CalcItemWidth(pItem), m_iItemHei);
			if (rcItemBg.right>rc.Width() - m_iChildOffset*pItem->nLevel)
			{
				rcItemBg.right = rc.Width() - m_iChildOffset*pItem->nLevel;
			}

			DMColor crOldText = PBGRA(0XFF,0XFF,0XFF,0XFF);
			bool bTextColorChanged = false;
			///1. ���Ʊ���
			if (hItem == m_hSelItem)
			{
				if (m_pItemSelSkin != NULL)
				{
					m_pItemSelSkin->Draw(pCanvas, rcItemBg, 0);
				}
				else 
				{
					if (!m_crItemSelBg.IsTextInvalid())
					{
						pCanvas->FillSolidRect(rcItemBg, m_crItemSelBg);
					}
				}
				if (!m_crItemSelText.IsTextInvalid())
				{
					bTextColorChanged = true;
					crOldText = pCanvas->SetTextColor(m_crItemSelText);
				}
			}
			else if (hItem == m_hHoverItem)
			{
				if (m_pItemHoverSkin != NULL)
				{
					m_pItemHoverSkin->Draw(pCanvas, rcItemBg, 0);
				}
				else 
				{
					if (!m_crItemHoverBg.IsTextInvalid())
					{
						pCanvas->FillSolidRect(rcItemBg, m_crItemHoverBg);
					}
				}
				if (!m_crItemHoverText.IsTextInvalid())
				{
					bTextColorChanged = true;
					crOldText = pCanvas->SetTextColor(m_crItemHoverText);
				}
			}
			else
			{
				if (m_pItemBgSkin != NULL)
				{
					m_pItemBgSkin->Draw(pCanvas, rcItemBg, 0);
				}
				else 
				{
					if (!m_crItemBg.IsTextInvalid())
					{
						pCanvas->FillSolidRect(rcItemBg, m_crItemBg);
					}
				}
				if (!m_crItemText.IsTextInvalid())
				{
					bTextColorChanged = true;
					crOldText = pCanvas->SetTextColor(m_crItemText);
				}
			}

			if (pItem->bHasChildren &&
				DMTVIMask_Toggle == (m_uItemMask & DMTVIMask_Toggle))
			{
				int nImage = IIF_STATE3(pItem->dwToggleState, 0, 1, 2);
				if (!pItem->bCollapsed) 
				{
					nImage += 3;
				}
				m_pToggleSkin->Draw(pCanvas, m_rcToggle, nImage);
			}

			if (DMTVIMask_CheckBox == (m_uItemMask & DMTVIMask_CheckBox))
			{
				int nImage = IIF_STATE3(pItem->dwCheckBoxState, 0, 1, 2);
				if (pItem->nCheckBoxValue == DMTVICheckBox_Checked)
				{
					nImage += 3;
				}
				else if (pItem->nCheckBoxValue == DMTVICheckBox_PartChecked)
				{
					nImage += 6;
				}
				m_pCheckSkin->Draw(pCanvas, m_rcCheckBox, nImage);
			}

			if (DMTVIMask_Icon == (m_uItemMask & DMTVIMask_Icon) &&
				(pItem->nSelectedImage != -1 || pItem->nImage != -1))
			{
				if (pItem->nSelectedImage != -1 && hItem == m_hSelItem)
				{
					m_pIconSkin->Draw(pCanvas, m_rcIcon, pItem->nSelectedImage);
				}
				else 
				{
					m_pIconSkin->Draw(pCanvas, m_rcIcon, pItem->nImage);  
				}
			}

			UINT ulAlgin = 0;
			m_pDUIXmlInfo->m_pStyle->GetTextAlign(ulAlgin);
			CRect rcContent = rcItemBg;
			rcContent.right = CalaItemContentWidth(pItem);
			if (rcContent.right>rc.Width()-m_iChildOffset*pItem->nLevel)
			{
				rcContent.right = rc.Width()-m_iChildOffset*pItem->nLevel;
			}
			pCanvas->DrawText(DMTR(pItem->strText),-1,rcContent,ulAlgin);    
			if (bTextColorChanged)
			{
				pCanvas->SetTextColor(crOldText);
			}

			pCanvas->OffsetViewportOrg(-rc.left-m_iChildOffset*pItem->nLevel,-rc.top);
		} while (false);
	}

	void DUITreeCtrl::RedrawItem(HDMTREEITEM hItem)
	{
		do 
		{
			if (!DM_IsVisible(true))
			{
				break;		
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);

			int iFirstVisible = m_ptCurPos.y/m_iItemHei;
			int nPageItems = (rcClient.Height()+m_iItemHei-1)/m_iItemHei+1;
			int iItem = GetItemShowIndex(hItem);

			if (iItem!=-1 && iItem>=iFirstVisible && iItem<iFirstVisible+nPageItems)
			{
				LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);

				CRect rcItem(0,0,CalcItemWidth(pItem),m_iItemHei);
				rcItem.OffsetRect(rcClient.left-m_ptCurPos.x,rcClient.top+m_iItemHei*iItem-m_ptCurPos.y);
				DMSmartPtrT<IDMCanvas> pCanvas = DM_GetCanvas(&rcItem,m_bHover?DMOLEDC_PAINTBKGND:DMOLEDC_NODRAW);
				DM_SendMessage(WM_ERASEBKGND,(WPARAM)(void*)pCanvas);
				DrawItem(pCanvas,rcItem,hItem);
				DM_ReleaseCanvas(pCanvas);
			}

		} while (false);
	} 

	//---------------------------------------------------
	// Function Des: DUI����Ϣ�ַ�ϵ�к���
	BOOL DUITreeCtrl::DM_OnEraseBkgnd(IDMCanvas* pCanvas)
	{
		__super::DM_OnEraseBkgnd(pCanvas);
		return TRUE;
	}

	void DUITreeCtrl::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas,DrawEnviron);

			CRect rcClient;
			DV_GetClientRect(rcClient);

			int iFirstVisible = m_ptCurPos.y/m_iItemHei;
			int nPageItems = (m_rcsbClient.Height()+m_iItemHei-1)/m_iItemHei+1;

			int iVisible = -1;
			HDMTREEITEM hItem = DMTreeT<LPTVITEM>::GetNextItem(DMTVI_ROOT);// �Ӹ���㿪ʼ���²���
			while (hItem)
			{
				LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
				if (pItem->bVisible)
				{
					iVisible++;
				}
				if (iVisible>iFirstVisible+nPageItems) 
				{// �����������������
					break;
				}
				if (iVisible>=iFirstVisible)
				{// �ӿ�������һ�ʼ����
					CRect rcItem(0,0,CalcItemWidth(pItem),m_iItemHei);
					rcItem.OffsetRect(rcClient.left-m_ptCurPos.x,rcClient.top-m_ptCurPos.y+iVisible*m_iItemHei);
					DrawItem(pCanvas,rcItem,hItem);
				}
				if (pItem->bCollapsed)
				{// �������۵�����
					HDMTREEITEM hChild = GetChildItem(hItem,false);
					while (hChild)
					{
						hItem = hChild;
						hChild = GetChildItem(hItem,false);
					}
				}
				hItem = DMTreeT<LPTVITEM>::GetNextItem(hItem);
			}
			DV_PopDrawEnviron(pCanvas,DrawEnviron);
		} while (false);
	}

	void DUITreeCtrl::OnDestroy()
	{
		DeleteAllItems();
	}

	void DUITreeCtrl::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		m_hHoverItem = HitTest(pt);

		if (m_hHoverItem!=m_hSelItem && m_hHoverItem)
		{// 
			SelectItem(m_hHoverItem,false);
		}

		if(m_hHoverItem)
		{    
			m_hCaptureItem = m_hHoverItem;
			ItemLButtonDown(m_hHoverItem, nFlags, pt);
		}
	}

	void DUITreeCtrl::OnLButtonDbClick(UINT nFlags,CPoint pt)
	{     
		m_hHoverItem = HitTest(pt);
		if (m_hHoverItem)
		{
			Expand(m_hHoverItem,TVE_TOGGLE);
			ItemLButtonDbClick(m_hHoverItem, nFlags, pt);
		}
	}

	void DUITreeCtrl::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		m_hHoverItem = HitTest(pt);

		if (m_hCaptureItem)
		{
			ItemLButtonUp(m_hCaptureItem, nFlags, pt);
			m_hCaptureItem = NULL;
			return;
		}

		if (m_hHoverItem)
		{
			ItemLButtonUp(m_hHoverItem, nFlags, pt);
		}
	}

	void DUITreeCtrl::OnRButtonDown(UINT nFlags, CPoint pt)
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

	void DUITreeCtrl::OnMouseMove(UINT nFlags,CPoint pt)
	{
		HDMTREEITEM hHitTest = HitTest(pt);

		if (hHitTest!=m_hHoverItem)
		{
			if (m_hHoverItem)
			{
				ItemMouseLeave(m_hHoverItem);
			}
			HDMTREEITEM hOldHoverItem = m_hHoverItem;
			m_hHoverItem = hHitTest;
			RedrawItem(hOldHoverItem);
		}
		if (m_hHoverItem)
		{
			ItemMouseMove(m_hHoverItem, nFlags, pt);
		}
	}

	void DUITreeCtrl::OnMouseLeave()
	{
		if (m_hHoverItem)
		{
			ItemMouseLeave(m_hHoverItem);
			HDMTREEITEM hOldHoverItem = m_hHoverItem;
			m_hHoverItem = NULL;
			RedrawItem(hOldHoverItem);
		}
	}

	//---------------------------------------------------
	// Function Des: �����غ���
	DMCode DUITreeCtrl::DV_CreateChildWnds(const DMXmlNode &XmlNode)
	{
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}
			RemoveAllItems();
			ItemLayout();

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::DUITreeCtrlAttr::NODE_item);
			if (XmlItem.IsValid())
			{
				LoadBranch(DMTVI_ROOT,XmlItem);
			}
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUITreeCtrl::DV_OnSetCursor(const CPoint &pt)
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
			if (DMTVIBtn_None!=nHitTestBtn)
			{
				break;
			}
			iErr = __super::DV_OnSetCursor(pt);

		} while (false);
		return iErr;
	}

	void DUITreeCtrl::LoadBranch(HDMTREEITEM hParent,DMXmlNode &XmlItem)
	{
		while (XmlItem.IsValid())
		{
			HDMTREEITEM hItem = InsertItem(XmlItem,hParent);// ���ص�ǰ������
			DMXmlNode XmlChild = XmlItem.FirstChild("item");
			if (XmlChild.IsValid()) 
			{// �ӷ�֧�ݹ�
				LoadBranch(hItem,XmlChild);
			}

			XmlItem = XmlItem.NextSibling("item");
		}
	}

	void DUITreeCtrl::LoadItemAttribute(DMXmlNode &XmlItem, LPTVITEM pItem)
	{
		DMXmlAttribute Attr = XmlItem.FirstAttribute();
		while (Attr.IsValid())
		{
			LPCSTR lpszName = Attr.GetName();
			if (lpszName&& strlen(lpszName))
			{
				if (0 == dm_xmlstrcmp(lpszName, DMAttr::DUITreeCtrlAttr::ITEM_text))
				{
					pItem->strText = DMCA2W(Attr.GetValue());
				}
				else if (0 == dm_xmlstrcmp(lpszName, DMAttr::DUITreeCtrlAttr::ITEM_bcollapsed))
				{
					bool bCollapsed=false; 
					DMAttributeDispatch::ParseBool(Attr.GetValue(),bCollapsed);
					pItem->bCollapsed = bCollapsed;
				}
				else if (0 == dm_xmlstrcmp(lpszName ,DMAttr::DUITreeCtrlAttr::ITEM_icon))
				{
					int nImage = 0;
					DMAttributeDispatch::ParseInt(Attr.GetValue(),nImage);
					pItem->nImage = nImage;
				}
				else if (0 == dm_xmlstrcmp(lpszName, DMAttr::DUITreeCtrlAttr::ITEM_selicon))
				{
					int nSelectedImage = 0;
					DMAttributeDispatch::ParseInt(Attr.GetValue(),nSelectedImage);
					pItem->nSelectedImage = nSelectedImage;
				}
				else if (0 == dm_xmlstrcmp(lpszName, DMAttr::DUITreeCtrlAttr::ITEM_data))
				{
					int lParam = 0;
					DMAttributeDispatch::ParseInt(Attr.GetValue(),lParam);
					pItem->lParam = (LPARAM)lParam;
				}
			}
			Attr=Attr.NextAttribute();
		}
	}

	int DUITreeCtrl::GetScrollLineSize(bool bVert)
	{
		return m_iItemHei;
	}

	void DUITreeCtrl::ItemLayout()
	{
		int iOffset	= 0;
		CSize	sizeSkin;

		m_uItemMask = 0;
		m_rcToggle.SetRect(0, 0, 0, 0);
		m_rcCheckBox.SetRect(0, 0, 0, 0);
		m_rcIcon.SetRect(0, 0, 0, 0);

		// ����λ��    
		if (m_pToggleSkin)
		{
			m_uItemMask |= DMTVIMask_Toggle;
			m_pToggleSkin->GetStateSize(sizeSkin);        
			m_rcToggle.SetRect(iOffset, (m_iItemHei-sizeSkin.cy)/2,iOffset+sizeSkin.cx,m_iItemHei-(m_iItemHei-sizeSkin.cy)/2);
			iOffset += sizeSkin.cx;
		}

		if (m_bCheckBox && m_pCheckSkin) 
		{
			m_uItemMask |= DMTVIMask_CheckBox;
			m_pCheckSkin->GetStateSize(sizeSkin);      
			m_rcCheckBox.SetRect(iOffset,(m_iItemHei-sizeSkin.cy)/2,iOffset+sizeSkin.cx,m_iItemHei-(m_iItemHei-sizeSkin.cy)/2);
			iOffset += sizeSkin.cx;
		}

		if (m_pIconSkin)
		{
			m_uItemMask |= DMTVIMask_Icon;
			m_pIconSkin->GetStateSize(sizeSkin);
			m_rcIcon.SetRect(iOffset, (m_iItemHei-sizeSkin.cy)/2,iOffset+sizeSkin.cx,m_iItemHei-(m_iItemHei-sizeSkin.cy)/2);
			iOffset += sizeSkin.cx;
		}

		m_iTextOffset = iOffset;
	}

	int DUITreeCtrl::CalaItemContentWidth(LPTVITEM pItem)
	{
		if (0 == pItem->nContentWidth && !pItem->strText.IsEmpty())// δ��ʼ����
		{
			DMSmartPtrT<IDMCanvas> pCanvas;
			g_pDMRender->CreateCanvas(0,0,&pCanvas);
			DV_SetDrawEnvironEx(pCanvas);
			UINT ulAlgin = 0;
			m_pDUIXmlInfo->m_pStyle->GetTextAlign(ulAlgin);
			int nTestDrawMode = ulAlgin & ~(DT_CENTER | DT_RIGHT | DT_VCENTER | DT_BOTTOM | DT_END_ELLIPSIS);

			CRect rcTest;
			DV_DrawText(pCanvas,DMTR(pItem->strText), -1, rcTest, nTestDrawMode | DT_CALCRECT);
			pItem->nContentWidth = rcTest.Width() + m_iTextOffset + 2*m_iItemMargin;
		}
		return pItem->nContentWidth;
	}

	int DUITreeCtrl::CalcItemWidth(const LPTVITEM pItem)
	{	
		return CalaItemContentWidth(pItem) + pItem->nLevel*m_iChildOffset;
	}

	int DUITreeCtrl::GetMaxItemWidth(HDMTREEITEM hItem)
	{
		int nItemWidth = 0;
		int nChildrenWidth = 0;

		LPTVITEM pItem = GetItem(hItem);
		if (pItem->bVisible)
		{
			nItemWidth = CalcItemWidth(pItem);
		}
		else
		{
			return 0;
		}

		HDMTREEITEM hChild = GetChildItem(hItem);
		while (hChild)
		{
			nChildrenWidth = GetMaxItemWidth(hChild);
			if (nChildrenWidth > nItemWidth)
			{
				nItemWidth = nChildrenWidth;
			}

			hChild = GetNextSiblingItem(hChild);
		}

		return nItemWidth;
	}

	void DUITreeCtrl::OnNodeFree(LPTVITEM & pItemData)
	{// DMTreeTԤ����
		DM_DELETE(pItemData);
	}

	int  DUITreeCtrl::GetMaxItemWidth()
	{
		int nItemWidth = 0;
		m_nMaxItemWidth = 0;
		HDMTREEITEM hItem = DMTreeT<LPTVITEM>::GetNextItem(DMTVI_ROOT);

		while (hItem)
		{
			nItemWidth = GetMaxItemWidth(hItem);
			if (nItemWidth>m_nMaxItemWidth)
			{
				m_nMaxItemWidth = nItemWidth;
			}
			hItem=GetNextSiblingItem(hItem);
		}

		return m_nMaxItemWidth;
	}

	// ����-----------------------------
	// �Զ��޸�pt��λ��Ϊ��Ե�ǰ���ƫ����
	HDMTREEITEM DUITreeCtrl::HitTest(CPoint &pt)
	{
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		CPoint ptCopy = pt;
		ptCopy.y -= rcClient.top - m_ptCurPos.y;
		int iItem = ptCopy.y/m_iItemHei;
		if (iItem >= m_nVisibleItems)
		{
			return NULL;
		}

		HDMTREEITEM hRet = NULL;
		int iVisible = -1;
		HDMTREEITEM hItem = DMTreeT<LPTVITEM>::GetNextItem(DMTVI_ROOT);
		while (hItem)
		{
			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
			if (pItem->bVisible) 
			{
				iVisible++;
			}
			if (iVisible == iItem)
			{
				CRect rcItem(m_iChildOffset*pItem->nLevel,0,rcClient.Width(),m_iItemHei);
				rcItem.OffsetRect(rcClient.left-m_ptCurPos.x,rcClient.top-m_ptCurPos.y+iVisible*m_iItemHei);
				pt   -= rcItem.TopLeft();
				hRet = hItem;
				break;
			}
			if (pItem->bCollapsed)
			{// �������۵�����
				HDMTREEITEM hChild = GetChildItem(hItem,false);
				while (hChild)
				{
					hItem  = hChild;
					hChild = GetChildItem(hItem,false);
				}
			}
			hItem = DMTreeT<LPTVITEM>::GetNextItem(hItem);
		}        
		return hRet;
	}

	void DUITreeCtrl::PageUp()
	{
		OnScroll(true,SB_PAGEUP,0);
	}

	void DUITreeCtrl::PageDown()
	{
		OnScroll(true,SB_PAGEDOWN,0);
	}

	void DUITreeCtrl::ItemLButtonDown(HDMTREEITEM hItem, UINT nFlags,CPoint pt)
	{
		int nHitTestBtn = ItemHitTest(hItem, pt);    
		LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);    

		//���ԭ��pushdown��ť
		if (m_nItemPushDownBtn != nHitTestBtn)
		{
			if (m_nItemPushDownBtn == DMTVIBtn_Toggle && 
				DUIWNDSTATE_PushDown == (pItem->dwToggleState & DUIWNDSTATE_PushDown))
			{
				ModifyToggleState(hItem, 0, DUIWNDSTATE_PushDown);
			}

			if (m_nItemPushDownBtn == DMTVIBtn_CheckBox && 
				DUIWNDSTATE_PushDown == (pItem->dwCheckBoxState & DUIWNDSTATE_PushDown))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_PushDown);
			}

			m_nItemPushDownBtn = nHitTestBtn;
		}

		//����pushdown��ť
		if (m_nItemPushDownBtn != DMTVIBtn_None)
		{
			if (m_nItemPushDownBtn == DMTVIBtn_Toggle && 
				DUIWNDSTATE_PushDown != (pItem->dwToggleState & DUIWNDSTATE_PushDown))
			{
				ModifyToggleState(hItem, DUIWNDSTATE_PushDown, 0);
				Expand(pItem->hItem, TVE_TOGGLE);            
			}

			if (m_nItemPushDownBtn == DMTVIBtn_CheckBox && 
				DUIWNDSTATE_PushDown != (pItem->dwCheckBoxState & DUIWNDSTATE_PushDown))
			{
				bool bCheck = 
					pItem->nCheckBoxValue == DMTVICheckBox_Checked ? false : true;
				ModifyChekcBoxState(hItem, DUIWNDSTATE_PushDown, 0);
				SetCheckState(pItem->hItem, bCheck);
			}
		}
	}

	void DUITreeCtrl::ItemLButtonUp(HDMTREEITEM hItem, UINT nFlags,CPoint pt)
	{
		LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);

		if (m_nItemPushDownBtn != DMTVIBtn_None)
		{
			if (m_nItemPushDownBtn == DMTVIBtn_Toggle && 
				DUIWNDSTATE_PushDown == (pItem->dwToggleState & DUIWNDSTATE_PushDown))
			{
				ModifyToggleState(hItem, 0, DUIWNDSTATE_PushDown);
			}

			if (m_nItemPushDownBtn == DMTVIBtn_CheckBox && 
				DUIWNDSTATE_PushDown == (pItem->dwCheckBoxState & DUIWNDSTATE_PushDown))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_PushDown);
				DMEventTCCheckStateArgs evt(this);
				evt.m_hItem=hItem;
				evt.m_uCheckState = pItem->dwCheckBoxState;
				DV_FireEvent(evt);
			}

			m_nItemPushDownBtn = DMTVIBtn_None;
		}
	}

	void DUITreeCtrl::ItemLButtonDbClick(HDMTREEITEM hItem, UINT nFlags,CPoint pt)
	{
		int nHitTestBtn = ItemHitTest(hItem, pt);
		if (nHitTestBtn == DMTVIBtn_CheckBox)
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

	void DUITreeCtrl::ItemMouseMove(HDMTREEITEM hItem, UINT nFlags,CPoint pt)
	{
		LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);

		int nHitTestBtn = ItemHitTest(hItem, pt);
		if (nHitTestBtn != m_nItemHoverBtn)
		{
			if (m_nItemHoverBtn == DMTVIBtn_Toggle 
				&&DUIWNDSTATE_Hover == (pItem->dwToggleState & DUIWNDSTATE_Hover))
			{
				ModifyToggleState(hItem, 0, DUIWNDSTATE_Hover);
			}

			if (m_nItemHoverBtn == DMTVIBtn_CheckBox 
				&&DUIWNDSTATE_Hover == (pItem->dwCheckBoxState & DUIWNDSTATE_Hover))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_Hover);
			}

			m_nItemHoverBtn = nHitTestBtn;
		}

		if (m_nItemHoverBtn != DMTVIBtn_None)
		{
			if (m_nItemHoverBtn == DMTVIBtn_Toggle 
				&&DUIWNDSTATE_Hover != (pItem->dwToggleState & DUIWNDSTATE_Hover))
			{
				ModifyToggleState(hItem, DUIWNDSTATE_Hover, 0);
			}

			if (m_nItemHoverBtn == DMTVIBtn_CheckBox 
				&&DUIWNDSTATE_Hover != (pItem->dwCheckBoxState & DUIWNDSTATE_Hover))
			{
				ModifyChekcBoxState(hItem, DUIWNDSTATE_Hover, 0);
			}
		}

		/// hover����
		if (m_nItemHoverBtn == DMTVIBtn_None&&m_bHover)
		{
			RedrawItem(hItem);
		}
	}

	void DUITreeCtrl::ItemMouseLeave(HDMTREEITEM hItem)
	{
		LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);

		if (m_nItemHoverBtn != DMTVIBtn_None)
		{
			if (m_nItemHoverBtn == DMTVIBtn_Toggle 
				&&DUIWNDSTATE_Hover == (pItem->dwToggleState & DUIWNDSTATE_Hover))
			{
				ModifyToggleState(hItem, 0, DUIWNDSTATE_Hover);
			}

			if (m_nItemHoverBtn == DMTVIBtn_CheckBox 
				&&DUIWNDSTATE_Hover == (pItem->dwCheckBoxState & DUIWNDSTATE_Hover))
			{
				ModifyChekcBoxState(hItem, 0, DUIWNDSTATE_Hover);
			}

			m_nItemHoverBtn = DMTVIBtn_None;
		}
	}

	void DUITreeCtrl::ModifyToggleState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove)
	{
		LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);

		pItem->dwToggleState |= dwStateAdd;
		pItem->dwToggleState &= ~dwStateRemove;

		CRect rcItem, rcUpdate = m_rcToggle;
		if (GetItemRect(pItem, rcItem))
		{        
			rcUpdate.OffsetRect(rcItem.left, rcItem.top);
			DM_InvalidateRect(rcUpdate);
		}
	}

	void DUITreeCtrl::ModifyChekcBoxState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove)
	{
		LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);

		pItem->dwCheckBoxState |= dwStateAdd;
		pItem->dwCheckBoxState &= ~dwStateRemove;

		CRect rcItem, rcUpdate = m_rcCheckBox;
		if (GetItemRect(pItem, rcItem))
		{
			rcUpdate.OffsetRect(rcItem.left, rcItem.top);
			DM_InvalidateRect(rcUpdate);
		}
	}

	bool DUITreeCtrl::IsAncestor(HDMTREEITEM hItem1,HDMTREEITEM hItem2)
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

	void DUITreeCtrl::SetChildrenVisible(HDMTREEITEM hItem,bool bVert)
	{
		HDMTREEITEM hChild = GetChildItem(hItem);
		while (hChild)
		{
			LPTVITEM pItem = GetItem(hChild);
			pItem->bVisible = bVert;
			m_nVisibleItems += bVert?1:-1;
			if (!pItem->bCollapsed) 
			{
				SetChildrenVisible(hChild,bVert);
			}
			hChild = GetNextSiblingItem(hChild);
		}
	}

	void DUITreeCtrl::SetChildrenState(HDMTREEITEM hItem, int nCheckValue)
	{
		HDMTREEITEM hChildItem = DMTreeT<LPTVITEM>::GetChildItem(hItem);
		while (hChildItem)
		{
			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hChildItem);
			pItem->nCheckBoxValue = nCheckValue;    
			SetChildrenState(hChildItem, nCheckValue);
			hChildItem = DMTreeT<LPTVITEM>::GetNextSiblingItem(hChildItem);
		}        
	}

	bool DUITreeCtrl::CheckChildrenState(HDMTREEITEM hItem, bool bCheck)
	{
		HDMTREEITEM hChildItem = DMTreeT<LPTVITEM>::GetChildItem(hItem);
		while (hChildItem)
		{
			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hChildItem);

			int nCheckValue = bCheck?DMTVICheckBox_Checked : DMTVICheckBox_UnChecked;
			// ��ǰ��㲻һ����������
			if (pItem->nCheckBoxValue != nCheckValue) 
			{
				return false;
			}
			// ����ӽ�㲻һ����������
			else if (false == CheckChildrenState(hChildItem, bCheck))
			{
				return false;
			}

			// ����ӽ���ֵܽ��
			hChildItem = DMTreeT<LPTVITEM>::GetNextSiblingItem(hChildItem);
		}
		return true;
	}

	void DUITreeCtrl::CheckState(HDMTREEITEM hItem, bool bCheck, bool bCheckChild)
	{        
		if (hItem)
		{
			LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);

			if (bCheckChild && CheckChildrenState(hItem, bCheck))
			{
				int nCheckValue = bCheck?DMTVICheckBox_Checked:DMTVICheckBox_UnChecked;
				pItem->nCheckBoxValue = nCheckValue;
				CheckState(GetParentItem(hItem), bCheck, true);
			}
			else
			{
				pItem->nCheckBoxValue = DMTVICheckBox_PartChecked;
				CheckState(GetParentItem(hItem),bCheck,  false);
			}
		}        
	}

	int DUITreeCtrl::GetItemShowIndex(HDMTREEITEM hItemObj)
	{
		int iVisible = -1;
		HDMTREEITEM hItem = GetNextItem(DMTVI_ROOT);
		while (hItem)
		{
			LPTVITEM pItem = GetItem(hItem);
			if (pItem->bVisible) 
			{
				iVisible++;
			}
			if (hItem==hItemObj)
			{
				return iVisible;
			}
			if (pItem->bCollapsed)
			{// �������۵�����
				HDMTREEITEM hChild = GetChildItem(hItem,false);
				while(hChild)
				{
					hItem = hChild;
					hChild = GetChildItem(hItem,false);
				}
			}
			hItem = GetNextItem(hItem);
		}
		return -1;
	}

	bool DUITreeCtrl::GetItemRect(LPTVITEM pItemObj,CRect &rcItem)
	{
		bool bRet = false;
		do 
		{
			if (false == pItemObj->bVisible)
			{
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			int iFirstVisible =  m_ptCurPos.y/m_iItemHei;
			int nPageItems    = (rcClient.Height()+m_iItemHei-1)/m_iItemHei+1;
			int iVisible = -1;
			HDMTREEITEM hItem = DMTreeT<LPTVITEM>::GetNextItem(DMTVI_ROOT);
			while (hItem)
			{
				LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
				if (pItem->bVisible) 
				{
					iVisible++;
				}
				if (iVisible>iFirstVisible+nPageItems)
				{
					break;
				}
				if (iVisible>=iFirstVisible && pItem==pItemObj)
				{
					CRect rcRet(m_iChildOffset*pItemObj->nLevel,0,rcClient.Width(),m_iItemHei);
					rcRet.OffsetRect(rcClient.left-m_ptCurPos.x,rcClient.top-m_ptCurPos.y+iVisible*m_iItemHei);
					rcItem = rcRet;
					bRet   = true;
					break;
				}
				if (pItem->bCollapsed)
				{// �������۵�����
					HDMTREEITEM hChild = GetChildItem(hItem,false);
					while (hChild)
					{
						hItem  = hChild;
						hChild = GetChildItem(hItem,false);
					}
				}
				hItem = DMTreeT<LPTVITEM>::GetNextItem(hItem);
			}
		} while (false);
		return bRet;
	}

	int DUITreeCtrl::ItemHitTest(HDMTREEITEM hItem,CPoint &pt) const
	{
		LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
		int nHitTestBtn = DMTVIBtn_None;

		if (DMTVIMask_Toggle == (m_uItemMask&DMTVIMask_Toggle)
			&&pItem->bHasChildren
			&&m_rcToggle.PtInRect(pt))
		{
			nHitTestBtn = DMTVIBtn_Toggle;
		}
		else if (DMTVIMask_CheckBox == (m_uItemMask&DMTVIMask_CheckBox)
			&&m_rcCheckBox.PtInRect(pt))
		{
			nHitTestBtn = DMTVIBtn_CheckBox;
		}

		return nHitTestBtn;
	}

	DMCode DUITreeCtrl::OnAttrCustomEx(LPCSTR lpszAttribute, LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_OK;
		bool bCalcWid = false;
		do 
		{
			if (NULL == lpszAttribute||NULL == lpszValue)
			{
				break;
			}
			if (0 == dm_xmlstrcmp(lpszAttribute, DMAttr::DUITreeCtrlAttr::bool_bcheckbox))
			{
				dm_parsebool(lpszValue,m_bCheckBox);
				break;
			}

			if (0 == dm_xmlstrcmp(lpszAttribute, DMAttr::DUITreeCtrlAttr::SKIN_toggleskin))
			{
				m_pToggleSkin = g_pDMApp->GetSkin(lpszValue);
				break;
			}
			if (0 == dm_xmlstrcmp(lpszAttribute, DMAttr::DUITreeCtrlAttr::SKIN_checkskin))
			{
				m_pCheckSkin = g_pDMApp->GetSkin(lpszValue);
				break;
			}
			if (0 == dm_xmlstrcmp(lpszAttribute, DMAttr::DUITreeCtrlAttr::SKIN_iconskin))
			{
				m_pIconSkin = g_pDMApp->GetSkin(lpszValue);
				bCalcWid = true;
				break;
			}

			iErr = DM_ECODE_FAIL;// ʲô��û������
		} while (false);
		if (!bLoadXml&&DMSUCCEEDED(iErr))
		{
			ItemLayout();
			if (bCalcWid)// icon�ı������������ܿ��
			{
				HDMTREEITEM hItem = DMTreeT<LPTVITEM>::GetNextItem(DMTVI_ROOT);// �Ӹ���㿪ʼ���²���
				while (hItem)
				{
					LPTVITEM pItem = DMTreeT<LPTVITEM>::GetItem(hItem);
					CalaItemContentWidth(pItem);
					hItem = DMTreeT<LPTVITEM>::GetNextItem(hItem);
				}
			}

			DM_Invalidate();
		}
		return iErr;
	}
}//namespace DM