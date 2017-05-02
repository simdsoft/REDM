#include "DmMainAfx.h"
#include "DUITabCtrl.h"

namespace DM
{
	DUITabPage::DUITabPage()
	{
		m_pDUIXmlInfo->m_bVisible = false;// 默认Page页是隐藏的
		m_TabAniType = TAB_NULL;
	}

	DMCode DUITabPage::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{
		return DM_ECODE_FAIL;
	}

	///DUITabCtrl-----------------------------------------------------
	DUITabCtrl::DUITabCtrl()
	{
		m_TextPt.SetPoint(-1,-1);           // 默认为-1，-1
		m_nHoverItem         = -1;
		m_nCurSelItem        = 0;
		m_nFirstItemOffset   = 0;
		m_nTabAlign          = AlignTop;
		m_nAnimateSteps      = 0;
		m_pTabAnimate        = NULL;
		m_pDUIXmlInfo->m_bFocusable = true;
		m_nItemSpace         = 0;

		// tabctrl
		DMADDEVENT(DMEventTabSelChangingArgs::EventID);
		DMADDEVENT(DMEventTabSelChangedArgs::EventID);
	}

	// 返回-1表示解析失败
	int DUITabCtrl::InsertItem(DMXmlNode &XmlNode, int iInsert/*=-1*/, bool bLoading/*=false*/)
	{
		int iRealInsert = iInsert;
		do 
		{
			if (!XmlNode.IsValid())
			{
				iRealInsert = -1;
				break;
			}
			
			if (m_pTabAnimate)
			{
				m_pTabAnimate->StopFrame();
			}
			if (_wcsicmp(XmlNode.GetName(), DUITabPage::GetClassName()))
			{
				iRealInsert = -1;
				break;
			}

			DUITabPage* pChild = NULL;
			g_pDMApp->CreateRegObj((void**)&pChild, XmlNode.GetName(),DMREG_Window);
			if (!pChild)
			{
				iRealInsert = -1;
				break;
			}

			if (-1 == iInsert||iInsert>GetItemCount())
			{
				iRealInsert = GetItemCount();
			}
			DM_InsertChild(pChild);
			m_PageArray.InsertAt(iRealInsert, pChild);
			pChild->InitDMData(XmlNode);
			pChild->SetAttribute(L"pos",L"0,0,-0,-0",bLoading);
			pChild->DM_SetVisible(false,true);
			if (!bLoading && m_nCurSelItem>=iRealInsert)
			{
				m_nCurSelItem++;
			}

			if (!bLoading)
			{
				DV_UpdateChildLayout();
				DM_Invalidate();
			}
		} while (false);
		return iRealInsert;
	}

	int DUITabCtrl::GetCurSel()
	{	
		return m_nCurSelItem;
	}

	/*
	1.广播tab页要改变的消息
	2.是否启用滑动效果
	3.给旧的发不显示消息，给新的发显示消息
	4.广播tab页已改变的消息
	5.开始滑动效果
	*/
	bool DUITabCtrl::SetCurSel(int nIndex)
	{
		bool bRet = false;
		do 
		{
			if (nIndex<0
				||nIndex>=GetItemCount()
				||(m_nCurSelItem == nIndex))
			{
				break;
			}

			int nOldSelItem = m_nCurSelItem;

			// 1.
			DMEventTabSelChangingArgs Evt(this);
			Evt.m_uOldSel = nOldSelItem;
			Evt.m_uNewSel = nIndex;
			DV_FireEvent(Evt);
			if (Evt.m_bCancel)
			{
				break;
			}

			// 2.
			DUITabPage *pPage = GetItem(nIndex);
			DUITabPage *pOldPage = GetItem(nOldSelItem);
			if (m_pTabAnimate)
			{
				m_pTabAnimate->StopFrame();
			}

			// 3.
			m_nCurSelItem = nIndex;
			int iType = TABANI_NULL;
			if (pPage && pOldPage)
			{
				iType = OnTabAnimateType(nOldSelItem,m_nCurSelItem,pPage->m_TabAniType);
			}
			if (TABANI_NULL!=iType
				&& m_nAnimateSteps && DM_IsVisible(true))
			{
				m_pTabAnimate = new DUITabAnimate(this,pOldPage,pPage,m_nAnimateSteps,iType);
			}
			else
			{
				if (pOldPage)
				{
					pOldPage->DM_SetVisible(false,true);
				}
				if (pPage)
				{
					pPage->DM_SetVisible(true, true);
				}
			}
			
			CRect rcTitle = GetTitleRect();
			DM_InvalidateRect(rcTitle);
	
			// 4.
			DMEventTabSelChangedArgs EndEvt(this);
			EndEvt.m_uNewSel = nIndex;
			EndEvt.m_uOldSel = nOldSelItem;
			DV_FireEvent(EndEvt);
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUITabCtrl::SetCurSel(LPCWSTR lpszTitle)
	{
		for (int i=0;i<GetItemCount();i++)
		{
			if (0 == _wcsicmp(m_PageArray[i]->m_strTitle,lpszTitle))
				return SetCurSel(i);
		}
		return false;
	}

	bool DUITabCtrl::SetItemTitle(int nIndex, LPCWSTR lpszTitle)
	{
		DUITabPage* pTab = GetItem(nIndex);
		if (pTab)
		{
			pTab->m_strTitle = lpszTitle;
			CRect rcTitle = GetTitleRect();
			DM_InvalidateRect(rcTitle);
			return true;
		}

		return false;
	}

	bool DUITabCtrl::GetItemRect(int nIndex, CRect&rcItem)
	{
		bool bRet = false;
		do 
		{
			if (nIndex<0
				||nIndex>=GetItemCount())
			{
				break;
			}

			CRect rcTitle = GetTitleRect();
			rcItem = CRect(rcTitle.TopLeft(),m_ItemSize);
			switch (m_nTabAlign)
			{
			case AlignTop:
			case AlignBottom:
				rcItem.OffsetRect(m_nFirstItemOffset+nIndex * (rcItem.Width()+ m_nItemSpace),0);
				break;
			case AlignLeft:
			case AlignRight:
				rcItem.OffsetRect(0, m_nFirstItemOffset+nIndex * (rcItem.Height()+ m_nItemSpace));
				break;
			}
			rcItem.IntersectRect(rcItem,rcTitle);
			bRet = true;
		} while (false);

		return bRet;
	}

	CRect DUITabCtrl::GetTitleRect()
	{
		CRect rcTitle;
		DV_GetClientRect(rcTitle);
		switch (m_nTabAlign)
		{
		case AlignTop:	 rcTitle.bottom = rcTitle.top+m_ItemSize.cy; break;
		case AlignBottom:rcTitle.top = rcTitle.bottom-m_ItemSize.cy; break;
		case AlignLeft:  rcTitle.right = rcTitle.left+m_ItemSize.cx; break;
		case AlignRight: rcTitle.left = rcTitle.right-m_ItemSize.cx; break;
		}
		return rcTitle;    
	}

	int DUITabCtrl::GetItemCount()
	{
		return (int)m_PageArray.GetCount();
	}

	DUITabPage* DUITabCtrl::GetItem(int nIndex)
	{
		DUITabPage* pPage = NULL;
		do 
		{
			if (nIndex<0||nIndex>=GetItemCount())
			{
				break;
			}

			pPage = m_PageArray[nIndex];
		} while (false);

		return pPage;
	}

	bool DUITabCtrl::RemoveItem(int nIndex, int nSelPage/*=0*/)
	{
		DUITabPage* pPage = GetItem(nIndex);
		DM_DestroyChildWnd(pPage);
		m_PageArray.RemoveAt(nIndex);

		if (nIndex == m_nCurSelItem)// 删除的是当前选中页
		{
			if (nSelPage<0)
			{
				nSelPage = 0;
			}

			if (nSelPage>=GetItemCount())
			{
				nSelPage = GetItemCount()-1;
			}
			m_nCurSelItem=-1;
			SetCurSel(nSelPage);
		}
		else
		{
			if (m_nCurSelItem>nIndex) 
			{
				m_nCurSelItem--;
			}
		}
		CRect rcTitle = GetTitleRect();
		DM_InvalidateRect(rcTitle);
		return true;
	}

	bool DUITabCtrl::RemoveItem(DUIWindow* pPage,int nSelPage)
	{
		bool bRet = false;
		do 
		{
			if (NULL == pPage)
			{
				break;
			}
			DM_DestroyChildWnd(pPage);
			int nCount = GetItemCount();
			bool bDel = false;
			int nIndex = 0;
			for (nIndex=0;nIndex<nCount; nIndex++)
			{
				if (m_PageArray[nIndex] == pPage)
				{
					m_PageArray.RemoveAt(nIndex);
					bDel = true;
					break;
				}
			}
			if (false == bDel)
			{
				break;
			}

			if (nIndex == m_nCurSelItem)// 删除的是当前选中页
			{
				if (nSelPage<0)
				{
					nSelPage = 0;
				}

				if (nSelPage>=(int)m_PageArray.GetCount())
				{
					nSelPage = (int)m_PageArray.GetCount()-1;
				}
				m_nCurSelItem=-1;
				SetCurSel(nSelPage);
			}
			else
			{
				if (m_nCurSelItem>nIndex) 
				{
					m_nCurSelItem--;
				}
			}
			CRect rcTitle = GetTitleRect();
			DM_InvalidateRect(rcTitle);

			bRet = true;
		} while (false);
		return bRet;
	}

	void DUITabCtrl::RemoveAllItems(void)
	{
		for (int i = GetItemCount()-1; i >= 0; i--)
		{
			DUITabPage * pTab = GetItem(i);
			DM_DestroyChildWnd(pTab);
			m_PageArray.RemoveAt(i);
		}
		DM_Invalidate();
	}

	void DUITabCtrl::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			DUIDrawEnviron painter;
			DV_PushDrawEnviron(pCanvas,painter);

			CRect rcTitle = GetTitleRect();
			pCanvas->PushClip(&rcTitle,RGN_AND);

			if (m_pMainBgSkin)
			{
				m_pMainBgSkin->Draw(pCanvas, rcTitle, DUIWNDSTATE_Normal);
			}

			DWORD dwState = DUIWNDSTATE_Normal;
			for (int i=0; i<GetItemCount(); i++)
			{
				dwState = DUIWNDSTATE_Normal;
				if (i== m_nCurSelItem)
				{
					dwState = DUIWNDSTATE_PushDown;
				}
				else if (i == m_nHoverItem)
				{
					dwState = DUIWNDSTATE_Hover;
				}
				CRect rcItem;
				GetItemRect((int)i,rcItem);
				DrawItem(pCanvas,rcItem,(int)i,dwState);
			}
			pCanvas->PopClip();
			DV_PopDrawEnviron(pCanvas,painter);
		} while (false);
	}

	void DUITabCtrl::OnDestroy()
	{
		for(int i=GetItemCount()-1; i>=0; i--)
		{
			DM_DestroyChildWnd(m_PageArray[i]);
		}
		m_PageArray.RemoveAll();
	}

	void DUITabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
	{
		DUIWindow::OnLButtonDown(nFlags,point);
		int iClickItem = HitTest(point);
		if (iClickItem != m_nCurSelItem)
		{
			SetCurSel(iClickItem);
		}
	}

	int DUITabCtrl::HitTest(CPoint pt)
	{
		int nItemCount = GetItemCount();
		for (int i=0; i<nItemCount; i++)
		{
			CRect rcItem;
			GetItemRect(i, rcItem);

			if (rcItem.PtInRect(pt))
			{
				return i;
			}
		}
		return -1;
	}

	void DUITabCtrl::OnMouseMove(UINT nFlags, CPoint point)
	{
		CRect rcItem;
		int nOldHoverItem = m_nHoverItem;
		m_nHoverItem = HitTest(point);
		if (m_nHoverItem != nOldHoverItem) // 更新状态
		{
			GetItemRect(nOldHoverItem, rcItem);
			DM_InvalidateRect(rcItem);

			if (-1 != m_nHoverItem)
			{
				GetItemRect(m_nHoverItem, rcItem);
				DM_InvalidateRect(rcItem);
			}
		}
	}

	void DUITabCtrl::OnMouseLeave()
	{
		OnMouseMove(0,CPoint(-1,-1));
	}

	void DUITabCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		switch (nChar)
		{
		case VK_LEFT:
		case VK_UP:
			{
				if (!SetCurSel(m_nCurSelItem-1))// 轮询到最右边了
				{
					SetCurSel(GetItemCount()-1);
				}
			}
			break;

		case VK_RIGHT:
		case VK_DOWN:
			{
				if (!SetCurSel(m_nCurSelItem+1))// 轮询到最左边了
				{
					SetCurSel(0);
				}
			}
			break;
		case VK_HOME:
			SetCurSel(0);
			break;
		case VK_END:
			SetCurSel(GetItemCount()-1);
		default:
			break;
		}
	}

	DMCode DUITabCtrl::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			// 加入子Page
			for (DMXmlNode XmlChildNode = XmlNode.FirstChild(); XmlChildNode.IsValid(); XmlChildNode=XmlChildNode.NextSibling())
			{
				InsertItem(XmlChildNode,-1, TRUE);
			}

			if (-1 == m_nCurSelItem
				||m_nCurSelItem>=GetItemCount())
			{
				m_nCurSelItem = 0;
			}
			if (0 == m_PageArray.GetCount())
			{
				m_nCurSelItem = -1;
			}

			if (-1 != m_nCurSelItem)
			{
				GetItem(m_nCurSelItem)->DM_SendMessage(WM_SHOWWINDOW,TRUE,NormalShow);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUITabCtrl::DV_GetChildMeasureLayout(LPRECT lpRect)
	{
		DV_GetClientRect(lpRect);
		switch (m_nTabAlign)
		{
		case AlignLeft:  lpRect->left+= m_ItemSize.cx;	break;
		case AlignRight: lpRect->right-=m_ItemSize.cx;	break;
		case AlignTop:   lpRect->top += m_ItemSize.cy;	break;
		case AlignBottom:lpRect->bottom -= m_ItemSize.cy;break;
		}

		return DM_ECODE_OK;
	}

	DMCode DUITabCtrl::DV_UpdateSkin(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			iErr = __super::DV_UpdateSkin(wp, lp);
			if (DMSUCCEEDED(iErr))
			{
				break;
			}

			if (g_pDMApp->IsNeedUpdateSkin(m_pMainBgSkin))
			{
				iErr = DM_ECODE_OK;
				break;
			}
		} while (false);
		return iErr;
	}

	DMCode DUITabCtrl::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int iHoverItem = HitTest(pt);
			if (-1==iHoverItem)
			{
				break;
			}

			DUITabPage* pPage = GetItem(iHoverItem);
			if (NULL == pPage)
			{
				break;
			}
			if (pPage->m_pDUIXmlInfo->m_strTooltipText.IsEmpty())
			{
				break;
			}
			CRect rcItem;
			GetItemRect(iHoverItem, rcItem);

			tipInfo.hDUIWnd  = m_hDUIWnd;
			tipInfo.rcTarget = rcItem;
			tipInfo.strTip    = DMTR(pPage->m_pDUIXmlInfo->m_strTooltipText);
			tipInfo.iDelayTime =  pPage->m_pDUIXmlInfo->m_iTooltipDelayTime;
			tipInfo.iSpanTime  =  pPage->m_pDUIXmlInfo->m_iTooltipSpanTime;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	int DUITabCtrl::OnTabAnimateType(int iOld, int iCur, int iType)
	{
		int iTabAniType = TABANI_NULL;
		switch (iType)
		{
		case TAB_CINFLATE:
			{
				iTabAniType = TABANI_CINFLATE;
			}	
			break;
		case TAB_CDEFLATE:
			{
				iTabAniType = TABANI_CDEFLATE;
			}	
			break;
		case TAB_SLIDER:
			{
				if (m_nTabAlign==AlignTop || m_nTabAlign == AlignBottom)
				{
					if (iOld<iCur)
					{
						iTabAniType = TABANI_SD_RIGHTLEFT;
					}
					else
					{
						iTabAniType = TABANI_SD_LEFTRIGHT;
					}
				}
				else
				{
					if (iOld<iCur)
					{
						iTabAniType = TABANI_SD_BOTTOMTOP;
					}
					else
					{
						iTabAniType = TABANI_SD_TOPBOTTOM;
					}
				}
			}
			break;
		}
		return iTabAniType;
	}

	DMCode DUITabCtrl::OnTabAnimateFinish()
	{
		DM_DestroyChildWnd(m_pTabAnimate);
		m_pTabAnimate = NULL;
		if (-1!=m_nCurSelItem&&m_PageArray[m_nCurSelItem])
		{
			m_PageArray[m_nCurSelItem]->DM_SetVisible(true, true);
		}
		return true;
	}

	DMCode DUITabCtrl::OnCurSel(LPCWSTR pszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int nCurSel = m_nCurSelItem;
			dm_parseint(pszValue,nCurSel);
			if (!bLoadXml)
			{
				if (nCurSel!=m_nCurSelItem)
				{
					SetCurSel(nCurSel);
				}
			}
			else
			{
				m_nCurSelItem = nCurSel;
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	
	void DUITabCtrl::DrawItem(IDMCanvas* pCanvas,const CRect&rcItem,int iItem,DWORD dwState)
	{
		do 
		{
			if (rcItem.IsRectEmpty())
			{
				break;
			}

			DUITabPage* pPage = GetItem(iItem);
			if (!pPage)
			{
				break;
			}

			//0.先叠一次
			if (m_pItemBgSkin)// 不同于pPage->m_pItemSkin
			{
				int iDraw = 0;
				m_pItemBgSkin->GetStates(iDraw);
				iDraw = iDraw>(int)dwState?(int)dwState:0;
				m_pItemBgSkin->Draw(pCanvas,rcItem,iDraw);
			}

			//1.绘制背景图
			if (pPage->m_pItemSkin)
			{
				int iDraw = 0;
				pPage->m_pItemSkin->GetStates(iDraw);
				iDraw = iDraw>(int)dwState?(int)dwState:0;
				CRect rcSkin = rcItem;
				CSize sz;
				pPage->m_pItemSkin->GetStateSize(sz);
				MeetRect(rcSkin,sz);
				pPage->m_pItemSkin->Draw(pCanvas,rcSkin,iDraw);
			}

			// 2.绘制文字
			DUIDrawEnviron paint;
			DWORD dwOld = pPage->DM_ModifyState(dwState,DUIWNDSTATE_FULL,false);
			pPage->DV_PushDrawEnviron(pCanvas,paint);
			if (-1!=m_TextPt.x&&-1!=m_TextPt.y)
			{
				pCanvas->TextOut(DMTR(GetItem(iItem)->m_strTitle),-1,rcItem.left+m_TextPt.x,rcItem.top+m_TextPt.y);
			}
			else// x,y至少有一个为-1
			{
				CRect rcText = rcItem;
				UINT uTextAlign = 0;
				m_pDUIXmlInfo->m_pStyle->GetTextAlign(uTextAlign);
				UINT uAlign = uTextAlign;
				if (m_TextPt.y!=-1)// 指定了Y偏移，X居中
				{
					rcText.top += m_TextPt.y;
					uAlign = uTextAlign&(DT_CENTER|DT_RIGHT|DT_SINGLELINE|DT_END_ELLIPSIS);
				}
				else if(m_TextPt.x!=-1)
				{
					rcText.left += m_TextPt.x;
					uAlign = uTextAlign&(DT_VCENTER|DT_BOTTOM|DT_SINGLELINE|DT_END_ELLIPSIS);
				}
				pCanvas->DrawText(DMTR(GetItem(iItem)->m_strTitle),-1,&rcText,uAlign);
			}

			pPage->DV_PopDrawEnviron(pCanvas,paint);
			pPage->DM_ModifyState(dwOld,0,false);

		} while (false);
	}
}//namespace DM