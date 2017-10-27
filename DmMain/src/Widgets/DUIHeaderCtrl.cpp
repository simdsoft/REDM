#include "DmMainAfx.h"
#include "DUIHeaderCtrl.h"

namespace DM
{
	DUIHeaderCtrl::DUIHeaderCtrl()
	{
		m_pItemSkin       = NULL;
		m_pSortSkin       = NULL;
		m_bFixWidth       = false;
		m_bSwap           = true;
		m_bSort           = true;
		m_bDragging       = false;
		m_dwHitTest       = -1;
		m_nItemDelta      = 4;
		for (int i=0; i<3; i++)
		{
			m_crItemBg[i].SetTextInvalid();
		}

		m_pDUIXmlInfo->m_bOnlyDrawClient = true;

		// header
		DMADDEVENT(DMEventHeaderClickArgs::EventID);
		DMADDEVENT(DMEventHeaderItemChangingArgs::EventID);
		DMADDEVENT(DMEventHeaderItemChangedArgs::EventID);
		DMADDEVENT(DMEventHeaderItemSwapArgs::EventID);
	}

	DUIHeaderCtrl::~DUIHeaderCtrl()
	{
		RemoveAll();
	}

	//---------------------------------------------------
	// Function Des: 接口
	int DUIHeaderCtrl::InsertItem(int nIndex, DMXmlNode&XmlNode)
	{
		int iRet = -1;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}
			int iCount = (int)GetCount();
			if (-1 == nIndex
				||nIndex>iCount)
			{
				nIndex = iCount;
			}

			LPDMHDITEM pNewItem = new DMHDITEM;
			pNewItem->mask      = 0xFFFFFFFF;

			// 解析
			CStringW strValue = XmlNode.Attribute(DMAttr::DUIHeaderCtrlAttr::ITEM_width);
			DMAttributeDispatch::ParseInt(strValue,pNewItem->cxy);
			strValue = XmlNode.Attribute(DMAttr::DUIHeaderCtrlAttr::ITEM_skin);
			pNewItem->pSkin = g_pDMApp->GetSkin(strValue);
			if (NULL == pNewItem->pSkin)
			{
				pNewItem->pSkin = m_pItemSkin;
			}
			strValue = (LPWSTR)XmlNode.Attribute(DMAttr::DUIHeaderCtrlAttr::ITEM_text);
			pNewItem->lpszText	 = _wcsdup(strValue);
			pNewItem->cchTextMax = strValue.GetLength();
			strValue = XmlNode.Attribute(DMAttr::DUIHeaderCtrlAttr::ITEM_data);
			int iData = 0;
			DMAttributeDispatch::ParseInt(strValue, iData);
			pNewItem->lParam	 = iData;
			int stFlag = DMT_NULL;
			strValue = XmlNode.Attribute(DMAttr::DUIHeaderCtrlAttr::ITEM_stflag);
			DMAttributeDispatch::ParseInt(strValue,stFlag);
			pNewItem->stFlag	 = (DMHDSORTFLAG)stFlag;
			pNewItem->iOrder	 = nIndex;
			m_DMArray.InsertAt(nIndex,pNewItem);


			// 需要更新列的序号,不一定第0排就是0，因为swap
			for (int i=0;i<iCount;i++)
			{
				if (i==nIndex) 
				{
					continue;
				}
				if (m_DMArray[i]->iOrder>=nIndex)
				{
					m_DMArray[i]->iOrder++;
				}
			}
			DM_Invalidate();
			iRet = nIndex;
		} while (false);
		return iRet;
	}

	bool DUIHeaderCtrl::GetItem(int iItem,DMHDITEM *pItem)
	{
		bool bRet = false;
		do 
		{
			if ((UINT)iItem>=GetCount())
			{
				break;
			}

			if (pItem->mask & DMHDI_TEXT)
			{
				if (pItem->cchTextMax<m_DMArray[iItem]->cchTextMax)
				{
					break;
				}
				_tcscpy_s(pItem->lpszText,_tcslen(m_DMArray[iItem]->lpszText),m_DMArray[iItem]->lpszText);
			}

			if (pItem->mask & DMHDI_WIDTH)		pItem->cxy    = m_DMArray[iItem]->cxy;
			if (pItem->mask & DMHDI_LPARAM)     pItem->lParam = m_DMArray[iItem]->lParam;
			if (pItem->mask & DMHDI_SORTFLAG)   pItem->stFlag = m_DMArray[iItem]->stFlag;
			if (pItem->mask & DMHDI_ORDER)      pItem->iOrder   = m_DMArray[iItem]->iOrder;
			bRet = true;
		} while (false);
		return bRet;
	}

	size_t DUIHeaderCtrl::GetItemCount() 
	{
		return GetCount();
	}    

	int DUIHeaderCtrl::GetTotalWidth()
	{
		int nRet = 0;
		int iCount = (int)GetCount();
		for (int i=0; i<iCount; i++)
		{
			nRet += m_DMArray[i]->cxy;
		}
		return nRet;
	}

	int DUIHeaderCtrl::GetItemWidth(int iItem)
	{
		if (iItem<0 
			||iItem>=(int)GetCount())
		{
			return -1;
		}
		return m_DMArray[iItem]->cxy;
	}

	int DUIHeaderCtrl::SetItemWidth(int iItem,int iWid,bool bFire/*=true*/)
	{
		int iRet = -1;
		do 
		{
			if (iItem<0 
				||iItem>=(int)GetCount()
				||iWid<0)
			{
				break;
			}

			if (bFire)
			{
				DMEventHeaderItemChangingArgs Evt0(this);
				Evt0.m_iItem  = iItem;
				Evt0.m_nWidth = iWid;
				DV_FireEvent(Evt0);
				if (TRUE == Evt0.m_bCancel)
				{
					break;
				}
				iWid = Evt0.m_nWidth;// 外部可改变
			}
			iRet = m_DMArray[iItem]->cxy;
			m_DMArray[iItem]->cxy = iWid;

			if (bFire)
			{
				DMEventHeaderItemChangedArgs Evt1(this);
				Evt1.m_iItem  = iItem;
				Evt1.m_nWidth = iWid;
				DV_FireEvent(Evt1);
			}
			
			DM_Invalidate();
		} while (false);
		return iRet;
	}

	bool DUIHeaderCtrl::DeleteItem(int iItem)
	{
		bool bRet = false;
		do 
		{
			if (iItem<0 
				||iItem>=(int)GetCount())
			{
				break;
			}
			int iOrder = m_DMArray[iItem]->iOrder;
			RemoveObj(iOrder);

			// 更新排序
			int iCount = (int)GetCount();
			for (int i=0;i<iCount;i++)
			{
				if (m_DMArray[i]->iOrder>iOrder)
				{
					m_DMArray[i]->iOrder--;
				}
			}

			DM_Invalidate();
			bRet = true;
		} while (false);
		return bRet;
	}

	void DUIHeaderCtrl::DeleteAllItems()
	{
		RemoveAll();
		DM_Invalidate();
	}

	void DUIHeaderCtrl::DrawItem(IDMCanvas* pCanvas, CRect& rc, const LPDMHDITEM pItem)
	{
		do 
		{
			CRect rcItem = rc;
			rcItem.DeflateRect(m_rcItemMargin);
			if (pItem->pSkin)
			{// 绘背景
				pItem->pSkin->Draw(pCanvas,rcItem,pItem->ulState);
			}
			else 
			{
				if (pItem->ulState<3&&!m_crItemBg[pItem->ulState].IsTextInvalid())
				{
					pCanvas->FillSolidRect(rcItem,m_crItemBg[pItem->ulState]);
				}
			}

			UINT ulAlign = 0;
			if (pItem->lpszText)
			{// 绘文字
				m_pDUIXmlInfo->m_pStyle->GetTextAlign(ulAlign);
				CStringW strTrans = DMTR(pItem->lpszText);
				pCanvas->DrawText(strTrans,strTrans.GetLength(),rcItem,ulAlign);
			}

			if (DMT_NULL ==pItem->stFlag
				|| NULL == m_pSortSkin)
			{
				break;
			}

			// 绘制sort图标
			CSize szSort;
			m_pSortSkin->GetStateSize(szSort);
			CPoint ptSort;
			ptSort.y = rcItem.top+(rcItem.Height()-szSort.cy)/2;


			m_pDUIXmlInfo->m_pStyle->GetTextAlign(ulAlign);
			if (ulAlign&DT_RIGHT)
			{
				ptSort.x = rcItem.left+2;
			}
			else
			{
				ptSort.x = rcItem.right-szSort.cx-2;
			}
			m_pSortSkin->Draw(pCanvas,CRect(ptSort,szSort),DMT_UP==pItem->stFlag?0:1);
		} while (false);

	}

	void DUIHeaderCtrl::RedrawItem(int iItem)
	{
		do 
		{
			if (iItem<0
				||iItem>=(int)GetCount())
			{
				break;
			}

			CRect rcItem = GetItemRect(iItem);
			DM_InvalidateRect(rcItem);
		} while (false);
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUIHeaderCtrl::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas, DrawEnviron);
			CRect rcClient;
			DV_GetClientRect(&rcClient);
			CRect rcItem(rcClient.left,rcClient.top,rcClient.left,rcClient.bottom);
			int iCount = (int)GetCount();
			for (int i=0;i<iCount;i++)
			{
				rcItem.left  = rcItem.right;
				rcItem.right = rcItem.left+m_DMArray[i]->cxy;
				DrawItem(pCanvas,rcItem,m_DMArray[i]);
				if (rcItem.right>=rcClient.right)
				{//跳出
					break;
				}
			}
			if (rcItem.right<rcClient.right)
			{
				rcItem.left  = rcItem.right;
				rcItem.right = rcClient.right;
				if (m_pItemSkin)
				{
					m_pItemSkin->Draw(pCanvas,rcItem,0);
				}
			}
			DV_PopDrawEnviron(pCanvas,DrawEnviron);
		} while (false);
	}

	void DUIHeaderCtrl::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		DM_SetCapture();
		m_ptClick   =  pt;
		m_dwHitTest = HitTest(pt);
		if (IsItemHover(m_dwHitTest))
		{
			if (m_bSort)
			{
				m_DMArray[LOWORD(m_dwHitTest)]->ulState = DUIWNDSTATE_PushDown;// pushdown
				RedrawItem(LOWORD(m_dwHitTest));
			}
		}
		else if(-1!=m_dwHitTest)
		{ 
			m_nAdjItemOldWidth = m_DMArray[LOWORD(m_dwHitTest)]->cxy;
		}
	}

	void DUIHeaderCtrl::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		if (IsItemHover(m_dwHitTest))
		{
			if (m_bDragging)
			{// 拖动表头项
				if (m_bSwap)
				{
					DMDragWnd::DragEnd();
					m_pDragCanvas.Release();
					m_DMArray[LOWORD(m_dwHitTest)]->ulState = DUIWNDSTATE_Normal;//normal

					if (m_dwDragTo!=m_dwHitTest
						&&IsItemHover(m_dwDragTo))
					{
						LPDMHDITEM pItem = m_DMArray[LOWORD(m_dwHitTest)];
						m_DMArray.RemoveAt(LOWORD(m_dwHitTest));
						int nPos = LOWORD(m_dwDragTo);
						m_DMArray.InsertAt(LOWORD(m_dwDragTo),pItem);
						// 发消息通知宿主表项位置发生变化,如ListCtrl
						DMEventHeaderItemSwapArgs Evt(this);
						Evt.m_iOldIndex = LOWORD(m_dwHitTest);
						Evt.m_iNewIndex = nPos;
						DV_FireEvent(Evt);
					}
					m_dwHitTest = HitTest(pt);
					m_dwDragTo  = -1;
					DM_Invalidate();
				}
			}
			else
			{// 点击表头项
				if(m_bSort)
				{
					m_DMArray[LOWORD(m_dwHitTest)]->ulState = DUIWNDSTATE_Hover;
					RedrawItem(LOWORD(m_dwHitTest));
					DMEventHeaderClickArgs Evt(this);
					Evt.m_iItem = LOWORD(m_dwHitTest);
					DV_FireEvent(Evt);
				}
			}
		}
		else if (-1!=m_dwHitTest)
		{// 调整表头宽度，发送一个调整完成消息
			DMEventHeaderItemChangedArgs Evt(this);
			Evt.m_iItem  = LOWORD(m_dwHitTest);
			Evt.m_nWidth = m_DMArray[Evt.m_iItem]->cxy;
			DV_FireEvent(Evt);
		}
		m_bDragging = false;
		DM_ReleaseCapture();
	}

	void DUIHeaderCtrl::OnMouseMove(UINT nFlags,CPoint pt)
	{
		if (m_bDragging || nFlags&MK_LBUTTON)
		{
			if (!m_bDragging)
			{
				m_bDragging = true;      /// 开始移动
				if (IsItemHover(m_dwHitTest) && m_bSwap)
				{
					m_dwDragTo    = m_dwHitTest;
					CRect rcItem  = GetItemRect(LOWORD(m_dwHitTest));
					DrawDraggingState(m_dwDragTo);
					CreateDragCanvas(LOWORD(m_dwHitTest));//  创建拖动时的画布
					CPoint pt     = m_ptClick-rcItem.TopLeft();
					DMDragWnd::DragBegin(m_pDragCanvas,pt,0,128,LWA_ALPHA|LWA_COLORKEY);
				}
			}
			if (IsItemHover(m_dwHitTest))
			{
				if (m_bSwap)
				{
					DWORD dwDragTo = HitTest(pt);
					//CPoint pt2(pt.x,m_ptClick.y);
					CPoint pt2(pt.x,pt.y);
					ClientToScreen(GetContainer()->OnGetHWnd(),&pt2);
					if (IsItemHover(dwDragTo) 
						&&m_dwDragTo!=dwDragTo)
					{
						m_dwDragTo = dwDragTo;
						DrawDraggingState(dwDragTo);// 自动调整item位置
					}
					DMDragWnd::DragMove(pt2);
				}
			}
			else if (m_dwHitTest!=-1)
			{// 调节宽度
				if (!m_bFixWidth)
				{
					int cxNew = m_nAdjItemOldWidth + pt.x - m_ptClick.x;
					if (cxNew<0)
					{
						cxNew = 0;
					}

					DMEventHeaderItemChangingArgs Evt0(this);
					Evt0.m_iItem  = LOWORD(m_dwHitTest);
					Evt0.m_nWidth = cxNew;
					DV_FireEvent(Evt0);
					if (false == Evt0.m_bCancel)
					{
						m_DMArray[LOWORD(m_dwHitTest)]->cxy = Evt0.m_nWidth;
						// 发出调节宽度消息
						DMEventHeaderItemChangedArgs Evt(this);
						Evt.m_nWidth = Evt0.m_nWidth;
						DV_FireEvent(Evt);
						DM_Invalidate();
						GetContainer()->OnUpdateWindow();// 立即更新窗口
					}
				}
			}
		}
		else
		{
			DWORD dwHitTest = HitTest(pt);
			if (dwHitTest!=m_dwHitTest)
			{
				if (m_bSort)
				{
					if (IsItemHover(m_dwHitTest))
					{
						WORD iHover = LOWORD(m_dwHitTest);
						m_DMArray[iHover]->ulState = DUIWNDSTATE_Normal;
						RedrawItem(iHover);
					}
					if(IsItemHover(dwHitTest))
					{
						WORD iHover=LOWORD(dwHitTest);
						m_DMArray[iHover]->ulState = DUIWNDSTATE_Hover;//hover
						RedrawItem(iHover);
					}
				}
				m_dwHitTest = dwHitTest;
			}
		}
	}

	void DUIHeaderCtrl::OnMouseLeave()
	{
		if (!m_bDragging)
		{
			if(IsItemHover(m_dwHitTest) && m_bSort)
			{
				m_DMArray[LOWORD(m_dwHitTest)]->ulState = DUIWNDSTATE_Normal;
				RedrawItem(LOWORD(m_dwHitTest));
			}
			m_dwHitTest = -1;
		}
	}

	void DUIHeaderCtrl::OnActivateApp( BOOL bActive, DWORD dwThreadID )
	{
		if (m_bDragging)
		{
			if(m_bSort
				&&m_dwHitTest!=-1)
			{
				m_DMArray[LOWORD(m_dwHitTest)]->ulState = DUIWNDSTATE_Normal;//normal
			}
			m_dwHitTest = -1;

			DMDragWnd::DragEnd();
			m_pDragCanvas.Release();
			m_bDragging = false;
			DM_ReleaseCapture();
			DM_Invalidate();
		}
	}

	void DUIHeaderCtrl::OnDestroy()
	{
		DeleteAllItems();
		__super::OnDestroy();
	}
		
	//---------------------------------------------------
	// Function Des: 可重载函数
	DMCode DUIHeaderCtrl::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}

			DMXmlNode XmlItem = XmlNode.FirstChild(DMAttr::DUIHeaderCtrlAttr::NODE_item);
			while (XmlItem.IsValid())
			{
				InsertItem(-1,XmlItem);
				XmlItem = XmlItem.NextSibling();
			}
		} while (false);
		return iErr;

	}

	DMCode DUIHeaderCtrl::DV_OnSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_bFixWidth)
			{
				break;
			}
			DWORD dwHit = HitTest(pt);
			if (HIWORD(dwHit)==LOWORD(dwHit))
			{
				break;
			}

			HCURSOR hCursor = ::LoadCursorW(NULL, IDC_SIZEWE);
			::SetCursor(hCursor);
			iErr = DM_ECODE_OK;
		} while (false);
		if (!DMSUCCEEDED(iErr))
		{
			iErr = __super::DV_OnSetCursor(pt);
		}
		return iErr;
	}

	void DUIHeaderCtrl::PreArrayObjRemove(const LPDMHDITEM &obj)
	{
		delete obj;
	}

	DWORD DUIHeaderCtrl::HitTest(CPoint pt)
	{
		DWORD dwRet = -1;
		do 
		{
			CRect rcClient;
			DV_GetClientRect(&rcClient);

			if (!rcClient.PtInRect(pt))
			{
				break;
			}	
			CRect rcItem(rcClient.left,rcClient.top,rcClient.left,rcClient.bottom);
			int nMargin = m_bSort?m_nItemDelta:0;
			int iCount  = (int)GetCount();
			for (int i=0; i<iCount; i++)
			{// 查找pt位于哪个Item中
				if (0 == m_DMArray[i]->cxy) 
				{
					continue;    // 跳过宽度为0的项
				}
				rcItem.left  = rcItem.right;
				rcItem.right = rcItem.left+m_DMArray[i]->cxy;
				if (pt.x<rcItem.left+nMargin)
				{
					int nLeft = i>0?i-1:0;
					dwRet = MAKELONG(nLeft,i);  
					break;
				}
				else if (pt.x<rcItem.right-nMargin)
				{
					dwRet = MAKELONG(i,i);
					break;
				}
				else if (pt.x<rcItem.right)
				{
					WORD nRight = (WORD)i+1;
					if (nRight>=iCount)
					{
						nRight = -1;//采用-1代表末尾
					}
					dwRet = MAKELONG(i,nRight);
					break;
				}
			}

		} while (false);
		return dwRet;
	}

	bool DUIHeaderCtrl::IsItemHover(DWORD dwHitTest)
	{
		return dwHitTest!=-1 && LOWORD(dwHitTest)==HIWORD(dwHitTest);
	}

	void DUIHeaderCtrl::CreateDragCanvas(UINT iItem)
	{
		HBITMAP hBmp = NULL;
		do 
		{
			if (iItem>=(int)GetCount())
			{
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CRect rcItem(0,0,m_DMArray[iItem]->cxy,rcClient.Height());
		
			// 创建拖动画布
			m_pDragCanvas.Release();
			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			pRender->CreateCanvas(rcItem.Width(),rcItem.Height(),&m_pDragCanvas);
			DV_SetDrawEnvironEx(m_pDragCanvas);
			DrawItem(m_pDragCanvas,rcItem,m_DMArray[iItem]);
		} while (false);
	}

	void DUIHeaderCtrl::DrawDraggingState(DWORD dwDragTo)
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
			int iDragTo   = LOWORD(dwDragTo);
			int iDragFrom = LOWORD(m_dwHitTest);

			CArray<UINT_PTR>items;
			int iCount = (int)GetCount();
			for (int i=0; i<iCount; i++)
			{
				if( i!=iDragFrom) 
				{
					items.Add(i);
				}
			}
			items.InsertAt(iDragTo,iDragFrom);
			if (m_pItemSkin)
			{
				m_pItemSkin->Draw(pCanvas,rcClient,0);
			}
		
			for (UINT i=0;i<items.GetCount();i++)
			{
				rcItem.left  = rcItem.right;
				rcItem.right = rcItem.left+m_DMArray[items[i]]->cxy;
				if (items[i]!=iDragFrom)
				{
					DrawItem(pCanvas,rcItem,m_DMArray[items[i]]);
				}
			}
			DV_PopDrawEnviron(pCanvas, DrawEnviron);
			DM_ReleaseCanvas(pCanvas);
		} while (false);
	}

	CRect DUIHeaderCtrl::GetItemRect(UINT iItem)
	{
		CRect rcClient;
		DV_GetClientRect(&rcClient);
		CRect rcItem(rcClient.left,rcClient.top,rcClient.left,rcClient.bottom);
		int iCount = (int)GetCount();
		for (int i=0;i<=(int)iItem && i<iCount;i++)
		{
			rcItem.left = rcItem.right;
			rcItem.right = rcItem.left+m_DMArray[i]->cxy;
		}
		return rcItem;
	}

	DMCode DUIHeaderCtrl::OnAttributeItemSkin(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			IDMSkinPtr pOldSkin = m_pItemSkin;
			m_pItemSkin = g_pDMApp->GetSkin(lpszValue);
			if (pOldSkin == m_pItemSkin)
			{
				iErr = DM_ECODE_OK;
				break;
			}
			if (!bLoadXml)
			{
				int iCount = (int)GetCount();
				for (int i=0; i<iCount; i++)
				{
					LPDMHDITEM& pItem = GetObj(i);
					if (pItem->pSkin == pOldSkin// 使用了m_pItemSkin
						||NULL == pItem->pSkin
						)
					{
						pItem->pSkin = m_pItemSkin;
					}
				}
				DM_Invalidate();
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

}//namespace DM