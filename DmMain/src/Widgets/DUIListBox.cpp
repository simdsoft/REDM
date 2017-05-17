#include "DmMainAfx.h"
#include "DUIListBox.h"

namespace DM
{
	DUIListBox::DUIListBox()
	{
		Init();
	}		

	DUIListBox::~DUIListBox()
	{
		UnInit();
	}

	void DUIListBox::Init()
	{
		m_pDUIXmlInfo->m_bFocusable = true;
		m_iSelItem       = -1;
		m_iHoverItem     = -1;
		m_nDefItemHei	 = 20;

		m_IconPt.SetPoint(-1,-1);
		m_TextPt.SetPoint(-1,-1);

		m_pIconSkin      = NULL;
		m_pItemBgSkin    = NULL;

		m_crItemBg.SetTextInvalid();
		m_crItemBg2.SetTextInvalid();
		m_crItemHoverBg.SetTextInvalid();
		m_crItemHoverText.SetTextInvalid();
		m_crItemSelBg.SetTextInvalid();
		m_crItemText.SetTextInvalid();
		m_crItemSelText.SetTextInvalid();

		m_bHotTrack      = false;


		// listbox
		DMADDEVENT(DMEventLBSelChangingArgs::EventID);
		DMADDEVENT(DMEventLBSelChangedArgs::EventID);
		DMADDEVENT(DMEventLBGetDispInfoArgs::EventID);

	}

	void DUIListBox::UnInit()
	{
	}

	//---------------------------------------------------
	// Function Des: 接口
	int DUIListBox::InsertItem(int nIndex, LPLBITEM pItem)
	{
		do 
		{
			if (NULL == pItem)
			{
				break;
			}
			if (-1 == nIndex
				||nIndex>(int)GetCount())
			{
				nIndex = (int)GetCount();
			}

			LPLBITEM pNewItem = new LBITEM;
			pNewItem->assign(pItem);

			m_DMArray.InsertAt(nIndex, pNewItem);

			if (m_iSelItem>=nIndex) 
			{
				m_iSelItem++;
			}
			if (m_iHoverItem>=nIndex) 
			{
				m_iHoverItem++;
			}

			CRect rcClient;
			DUIWindow::DV_GetClientRect(&rcClient);
			int nTotalHeight = 0;
			for (int i = 0; i < (int)m_DMArray.GetCount(); i++)
			{
				nTotalHeight += m_DMArray[i]->nHeight;
			}
			CSize szView(rcClient.Width(), nTotalHeight);
			if (szView.cy>rcClient.Height())
			{
				szView.cx -= m_isbWid;
			}
			SetRangeSize(szView);
		} while (false);
		return nIndex;
	}

	int DUIListBox::GetTopIndex() const
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

	int DUIListBox::SetTopIndex(int nIndex)
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

	int DUIListBox::GetItemHeight() const
	{
		return m_nDefItemHei;
	}

	int DUIListBox::GetItemHeight(int nIndex) const
	{
		if (nIndex < 0 || nIndex >= (int)m_DMArray.GetCount())
			return LB_ERR;

		return m_DMArray[nIndex]->nHeight;
	}

	int DUIListBox::SetItemHeight(int nIndex, int cyItemHeight)
	{
		if (cyItemHeight < 0 || nIndex < 0 || nIndex >= (int)GetCount())
			return LB_ERR;

		m_DMArray[nIndex]->nHeight = cyItemHeight;
		return 0;
	}

	int DUIListBox::GetText(int nIndex, CStringW& strText)
	{
		int nRet = GetTextLen(nIndex);

		if (nRet != LB_ERR)
		{
			strText = m_DMArray[nIndex]->strText;
		}

		return nRet;
	}

	int DUIListBox::GetTextLen(int nIndex) 
	{
		if (nIndex < 0 || nIndex >= (int)GetCount())
			return LB_ERR;

		return m_DMArray[nIndex]->strText.GetLength();
	}

	int DUIListBox::DeleteString(int nIndex)
	{
		int iErr = LB_ERR;
		do 
		{
			if (nIndex<0 || nIndex>=(int)GetCount())
			{
				break;
			}

			if (m_DMArray[nIndex])
			{
				delete m_DMArray[nIndex];
			}
			m_DMArray.RemoveAt(nIndex);

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
				m_iHoverItem=-1;
			}
			else if (m_iHoverItem>nIndex)
			{
				m_iHoverItem--;
			}


			CRect rcClient;
			DUIWindow::DV_GetClientRect(&rcClient);
			int nTotalHeight = 0;
			for (int i = 0; i < (int)m_DMArray.GetCount(); i++)
			{
				nTotalHeight += m_DMArray[i]->nHeight;
			}
			CSize szView(rcClient.Width(),nTotalHeight);
			if (szView.cy>rcClient.Height()) 
			{
				szView.cx-=m_isbWid;
			}
			SetRangeSize(szView);
			iErr = 0;
		} while (false);
		return iErr;
	}

	int DUIListBox::AddString(LPCWSTR lpszItem, int nHei, int nImage, LPARAM lParam)
	{
		return InsertString(-1, lpszItem, nHei, nImage, lParam);
	}

	int DUIListBox::InsertString(int nIndex, LPCWSTR lpszItem, int nHei, int nImage,  LPARAM lParam)
	{
		//DMASSERT(lpszItem);

		LBITEM pItem;
		pItem.strText = lpszItem;
		pItem.nImage  = nImage;
		pItem.lParam  = lParam;
		if (-1==nHei)
		{
			pItem.nHeight = m_nDefItemHei;
		}
		else
		{
			pItem.nHeight = nHei;
		}
		return InsertItem(nIndex, &pItem);
	}

	LPARAM DUIListBox::GetItemData(int nIndex) 
	{
		if (nIndex<0 || nIndex >= (int)GetCount())
		{
			return 0;
		}

		return m_DMArray[nIndex]->lParam;
	}

	bool DUIListBox::SetItemData(int nIndex, LPARAM lParam)
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

	bool DUIListBox::SetCurSel(int nIndex)
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
			if (m_iSelItem == nIndex)
			{
				break;
			}
		
			DMEventLBSelChangingArgs EvtSelChanging(this);
			EvtSelChanging.m_nOldSel = m_iSelItem;
			EvtSelChanging.m_nNewSel = nIndex;

			DV_FireEvent(EvtSelChanging);
			if(EvtSelChanging.m_bCancel)
			{
				break;
			}
			int nOldSelItem = m_iSelItem;
			m_iSelItem = nIndex;

			if (DM_IsVisible(true))
			{
				if (nOldSelItem != -1)
				{
					RedrawItem(nOldSelItem);
				}
				if (m_iSelItem != -1)
				{
					RedrawItem(m_iSelItem);
				}
			}

			DMEventLBSelChangedArgs EvtSelChanged(this);
			EvtSelChanged.m_nOldSel = nOldSelItem;
			EvtSelChanged.m_nNewSel = m_iSelItem;
			DV_FireEvent(EvtSelChanged);
			bRet = true;
		} while (false);
		return bRet;
	}

	int DUIListBox::GetCurSel() const
	{
		return m_iSelItem;
	}

	void DUIListBox::DeleteAll()
	{
		for(int i=0; i<(int)GetCount(); i++)
		{
			if (m_DMArray[i])
			{
				delete m_DMArray[i];
			}
		}
		m_DMArray.RemoveAll();

		m_iSelItem  = -1;
		m_iHoverItem = -1;

		SetRangeSize(CSize(0,0));
		DM_Invalidate();
	}

	void DUIListBox::EnsureVisible(int nIndex)
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

	void DUIListBox::DrawItem(IDMCanvas* pCanvas, CRect& rc, int iItem)
	{
		do 
		{
			if (iItem<0 || iItem>=(int)GetCount())
			{
				break;
			}
			bool bTextColorChanged = false;
			int nBgImg = 0;
			DMColor crOldText;
			DMColor crItemBg	  = m_crItemBg;
			DMColor crItemText	  = m_crItemText;
			LPLBITEM pItem        = m_DMArray[iItem];
			CRect rcIcon, rcText;

			if (iItem%2)
			{
				if (NULL!=m_pItemBgSkin)
				{
					nBgImg = 1;
				}
				else if (!m_crItemBg2.IsTranslucent())
				{
					crItemBg = m_crItemBg2;
				}
			}

			if (!m_bHotTrack)//非热键追踪状态
			{
				if (iItem == m_iSelItem)// 选中项
				{
					if (NULL!=m_pItemBgSkin)// 优先选中背景图
					{
						nBgImg = 2;
					}
					else if (!m_crItemSelBg.IsTranslucent())// 选中背景色
					{
						crItemBg = m_crItemSelBg;
					}
					if (!m_crItemSelText.IsTranslucent())
					{
						crItemText = m_crItemSelText;
					}
				}
				else if (iItem == m_iHoverItem)// hover
				{
					if (NULL!=m_pItemBgSkin)// 优先hover背景图
					{
						nBgImg = 1;
					}
					else if (!m_crItemHoverBg.IsTranslucent())// hover背景色
					{
						crItemBg = m_crItemHoverBg;
					}
					if (!m_crItemHoverText.IsTranslucent())
					{
						crItemText = m_crItemHoverText;
					}
				}
			}
			else 
			{
				if (iItem == m_iHoverItem||(m_iHoverItem==-1 && iItem== m_iSelItem))// 选中项
				{
					if (NULL!=m_pItemBgSkin)
					{
						nBgImg = 2;
					}
					else if (!m_crItemSelBg.IsTranslucent())
					{
						crItemBg = m_crItemSelBg;
					}

					if (!m_crItemSelText.IsTranslucent())
					{
						crItemText = m_crItemSelText;
					}
				}
			}

			// 绘制背景
			if (NULL != m_pItemBgSkin)
			{
				m_pItemBgSkin->Draw(pCanvas,rc,nBgImg);
			}
			else if (!crItemBg.IsTranslucent())
			{
				pCanvas->FillSolidRect(rc,crItemBg);
			}

			if (!crItemText.IsTranslucent())
			{
				bTextColorChanged = true;
				crOldText = pCanvas->SetTextColor(crItemText);
			}

			if (pItem->nImage != -1&&m_pIconSkin)
			{
				int nOffsetX = m_IconPt.x, nOffsetY = m_IconPt.y;
				CSize sizeSkin;
				m_pIconSkin->GetStateSize(sizeSkin);
				rcIcon.SetRect(0, 0, sizeSkin.cx, sizeSkin.cy);

				if (-1 == m_IconPt.x)
				{
					nOffsetX = m_nDefItemHei / 6;	//以默认高度的6分1 为图标默认左边距
				}

				if (-1 == m_IconPt.y)
				{
					nOffsetY = (m_DMArray[iItem]->nHeight-sizeSkin.cy)/2;    //y 默认居中
				}

				rcIcon.OffsetRect(rc.left + nOffsetX, rc.top + nOffsetY);// 此处rcIcon和m_rcsbClient的交集区域才会被显示，
				m_pIconSkin->Draw(pCanvas, rcIcon, pItem->nImage);
			}


			UINT align = DT_SINGLELINE;
			rcText = rc;

			if (m_TextPt.x == -1)
			{
				rcText.left = rcIcon.Width() > 0 ? rcIcon.right + m_nDefItemHei / 6 : rc.left;
			}
			else
			{
				rcText.left = rc.left + m_TextPt.x;
			}

			if (m_TextPt.y == -1)
			{
				align |= DT_VCENTER;
			}
			else
			{
				rcText.top = rc.top + m_TextPt.y;
			}
			pCanvas->DrawText(DMTR(pItem->strText),-1,rcText,align);

			if (bTextColorChanged)
			{
				pCanvas->SetTextColor(crOldText);
			}

		} while (false);
	}


	void DUIListBox::RedrawItem(int iItem)
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

			CRect rcClient;
			DV_GetClientRect(&rcClient);
			int nTargetY = 0;
			for (int i = 0; i < iItem; i++)
			{
				nTargetY += m_DMArray[i]->nHeight;
			}
			if ((nTargetY >= m_ptCurPos.y && nTargetY <= m_ptCurPos.y + rcClient.Height())
				|| (nTargetY + m_DMArray[iItem]->nHeight >= m_ptCurPos.y && nTargetY + m_DMArray[iItem]->nHeight <= m_ptCurPos.y + rcClient.Height()) )
			{//目标有部分在可视范围内
				CRect rcItem(0,0,rcClient.Width(),m_DMArray[iItem]->nHeight);
				rcItem.OffsetRect(0,nTargetY-m_ptCurPos.y);// 促使第一个item可能只绘制一部分
				rcItem.OffsetRect(rcClient.TopLeft());
				IDMCanvas* pCanvas = DM_GetCanvas(&rcItem,DMOLEDC_PAINTBKGND);
				if (pCanvas)
				{
					DUIDrawEnviron DrawEnviron;
					DV_PushDrawEnviron(pCanvas,DrawEnviron);

					DM_SendMessage(WM_ERASEBKGND,(WPARAM)(HDC)pCanvas);
					DrawItem(pCanvas,rcItem,iItem);

					DV_PopDrawEnviron(pCanvas,DrawEnviron);
					DM_ReleaseCanvas(pCanvas);
				}
			}
		} while (false);
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUIListBox::OnDestroy()
	{
		DeleteAll();
		__super::OnDestroy();
	}

	void DUIListBox::OnSize(UINT nType,CSize size)
	{
		do 
		{
			__super::OnSize(nType,size);

			CRect rcClient;
			DUIWindow::DV_GetClientRect(&rcClient);
			int nTotalHeight = 0;
			for (int i = 0; i < (int)GetCount(); i++)
			{
				nTotalHeight += m_DMArray[i]->nHeight;
			}
			CSize szView(rcClient.Width(),nTotalHeight);
			if (szView.cy>rcClient.Height()) 
			{
				szView.cx-=m_isbWid;
			}
			SetRangeSize(szView);
		} while (false);
	}

	void DUIListBox::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas,DrawEnviron);

			int nTotalHeight = 0;
			for (int iItem = 0; iItem < (int)GetCount(); iItem++)
			{
				if ((nTotalHeight >= m_ptCurPos.y && nTotalHeight < m_ptCurPos.y + m_rcsbClient.Height())
					||(nTotalHeight + m_DMArray[iItem]->nHeight >= m_ptCurPos.y && nTotalHeight + m_DMArray[iItem]->nHeight < m_ptCurPos.y + m_rcsbClient.Height())
					||(nTotalHeight <= m_ptCurPos.y && nTotalHeight + m_DMArray[iItem]->nHeight>=m_ptCurPos.y + m_rcsbClient.Height())
					)
				{
					CRect rcItem(0,0,m_rcsbClient.Width(),m_DMArray[iItem]->nHeight);
					rcItem.OffsetRect(0,nTotalHeight-m_ptCurPos.y);
					rcItem.OffsetRect(m_rcsbClient.TopLeft());
					DrawItem(pCanvas,rcItem,iItem);
				}
				nTotalHeight += m_DMArray[iItem]->nHeight;
			}

			DV_PopDrawEnviron(pCanvas,DrawEnviron);
		} while (false);
	}

	void DUIListBox::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		DUIWindow::OnLButtonDown(nFlags,pt);
		if (!m_bHotTrack)
		{
			m_iHoverItem = HitTest(pt);
			if (m_iHoverItem!=m_iSelItem)
			{
				SetCurSel(m_iHoverItem);
			}
		}
	}

	void DUIListBox::OnLButtonDbClick(UINT nFlags,CPoint pt)
	{
	}

	void DUIListBox::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		if (m_bHotTrack)
		{
			m_iHoverItem = HitTest(pt);
			if (m_iHoverItem!=m_iSelItem)
			{
				SetCurSel(m_iHoverItem);
			}
		}
		DUIWindow::OnLButtonUp(nFlags,pt);
	}

	void DUIListBox::OnMouseMove(UINT nFlags,CPoint pt)
	{
		int nOldHover = m_iHoverItem;
		m_iHoverItem  = HitTest(pt);

		if (nOldHover!=m_iHoverItem)
		{
			if (nOldHover!=-1)
			{
				RedrawItem(nOldHover);
			}

			if (m_iHoverItem!=-1)
			{
				RedrawItem(m_iHoverItem);
			}
			if (m_iSelItem!=-1) 
			{
				RedrawItem(m_iSelItem);
			}
		}
	}

	void DUIListBox::OnMouseLeave()
	{
		if (DM_IsVisible(true))// 加一个判断，防止非客户区在list隐藏时绘制
		{
			SetMsgHandled(FALSE);
		}
	}

	void DUIListBox::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			int nNewSelItem = -1;
			int iCurSel = m_iSelItem;

			if (m_bHotTrack && -1!=m_iHoverItem)
			{
				iCurSel = m_iHoverItem;
			}
			if (nChar == VK_DOWN && m_iSelItem < (int)GetCount()-1)
			{
				nNewSelItem = iCurSel+1;
			}
			else if (nChar == VK_UP && m_iSelItem > 0)
			{
				nNewSelItem = iCurSel-1;
			}

			if (-1!=nNewSelItem)
			{
				int iHover = m_iHoverItem;
				if (m_bHotTrack)
				{
					m_iHoverItem = -1;
				}
				EnsureVisible(nNewSelItem);
				SetCurSel(nNewSelItem);
				if (iHover!=-1 && iHover != m_iSelItem && iHover != nNewSelItem) 
				{
					RedrawItem(iHover);
				}
			}

		} while (false);
	}

	void DUIListBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		DUIWindow *pOwner = DM_GetWindow(GDW_OWNER);
		if (pOwner)
		{
			pOwner->DM_SendMessage(WM_CHAR, nChar, MAKELONG(nFlags, nRepCnt));
		}
	}

	void DUIListBox::OnShowWindow( BOOL bShow, UINT nStatus )
	{
		if (!bShow)
		{
			m_iHoverItem = -1;
		}
		__super::OnShowWindow(bShow,nStatus);
	}

	//---------------------------------------------------
	// Function Des: 可重载函数
	DMCode DUIListBox::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::DUIListBoxAttr::NODE_item);
			while (XmlItem.IsValid())
			{
				LBITEM pItem;
				int nHeight = 0;
				DMAttributeDispatch::ParseInt(XmlItem.Attribute(DMAttr::DUIListBoxAttr::ITEM_height),nHeight);
				pItem.nHeight = (nHeight==0) ? m_nDefItemHei : nHeight; 
				pItem.strText = XmlItem.Attribute(DMAttr::DUIListBoxAttr::ITEM_text);
				DMAttributeDispatch::ParseInt(XmlItem.Attribute(DMAttr::DUIListBoxAttr::ITEM_icon),pItem.nImage);
				
				int lParam = (int)pItem.lParam;
				DMAttributeDispatch::ParseInt(XmlItem.Attribute(DMAttr::DUIListBoxAttr::ITEM_data),lParam);
				pItem.lParam  = lParam;

				InsertItem(-1, &pItem);
				XmlItem = XmlItem.NextSibling();
			}
			int nSelItem = -1;
			DMAttributeDispatch::ParseInt(XmlNode.Attribute(DMAttr::DUIListBoxAttr::INT_cursel),nSelItem);
			SetCurSel(nSelItem);
		} while (false);
		return DM_ECODE_OK;
	}

	int DUIListBox::HitTest(CPoint &pt)
	{
		int iRet = -1;
		do 
		{
			CRect rcClient;
			DV_GetClientRect(&rcClient);
			if (!rcClient.PtInRect(pt))
			{
				break;
			}

			CPoint pt2 = pt;
			pt2.y -= rcClient.top-m_ptCurPos.y;
			int nTotalHeight = 0;
			for (int i = 0; i < (int)GetCount(); i++)
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

	DMCode DUIListBox::OnAttributeCurSel(LPCWSTR lpszValue, bool bLoadXml)
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