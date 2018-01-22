#include "DmMainAfx.h"
#include "DUIFlowLayout.h"

namespace DM
{
	DUIFlowLayout::DUIFlowLayout()
	{
		m_iChildPaddLen = 0;
	}
	///--------------------------------DUIVLayout----------------
	DMCode DUIVLayout::DV_UpdateChildLayout()
	{
		do 
		{
			if (!DM_IsLayoutFinished())
			{ 
				DMASSERT_EXPR(0,L"窗口未完成自身布局!");
				break;
			}

			// 1.获得子控件的布局空间
			CRect rcLayout;
			DV_GetChildMeasureLayout(rcLayout);
			if (rcLayout.IsRectEmpty())
			{
				break;
			}
			// 2.判断容器子控件是不是为空
			if (0==m_Node.m_nChildrenCount)
			{
				break;
			}

			// 3.计算容器可用的大小
			SIZE szAvailable = rcLayout.Size();

			// 4.计算最小的大小 
			int nAdjustNum	= 0;   //  没有设置高度的需自动计算高度的个数
			int nCyFixedLen = 0;   //  总估值高度
			int nEstimateNum = 0;  //  估值高度的个数,包括了nAdjustNum

			DUIWindow *pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->DM_IsVisible(false)&&(false == pChild->m_pDUIXmlInfo->m_bPlaceHolder)// 不可见也不占位
					||pChild->m_bFloatLayout)
				{
					pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}

				// 5. 获得预设的fixsize，像这种竖直布局，一般会设置固定的高度，对应xml的height值
				CSize szMeasure(POS_INIT,POS_INIT);
				pChild->DV_Measure(szMeasure);// 如设置了高度，会自动赋值
				CRect &rcPadd  = pChild->m_pDUIXmlInfo->m_rcPadd;
				if (POS_INIT == szMeasure.cy)
				{
					nAdjustNum++;// 没有设置高度的需自动计算高度的个数
					nCyFixedLen += rcPadd.top+rcPadd.bottom;
				}
				else
				{
					int nMinHeight = pChild->m_pDUIXmlInfo->m_nMinHeight;
					int nMaxHeight = pChild->m_pDUIXmlInfo->m_nMaxHeight;
				
					if (nMinHeight!=-1
						&&szMeasure.cy<nMinHeight)
					{
						szMeasure.cy = nMinHeight;
					}
					if (nMaxHeight!=-1
						&&szMeasure.cy>nMaxHeight)
					{
						szMeasure.cy = nMaxHeight;
					}

					nCyFixedLen += szMeasure.cy+rcPadd.top+rcPadd.bottom;
				}

				nEstimateNum++;
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}

			// 8.子控件之间的额外距离
			nCyFixedLen += (nEstimateNum-1)*m_iChildPaddLen;

			// 开始摆放子控件
			int ncyAdjustItemLen	= 0;
			if (nAdjustNum>0)
			{
				// 9.判断自动计算高度够不够
				ncyAdjustItemLen = (szAvailable.cy>nCyFixedLen)?((szAvailable.cy-nCyFixedLen)/nAdjustNum):0;
			}

			SIZE szRemain = szAvailable;
			int iAdjustIndex = 0;
			int nCyFixedLenRemain = nCyFixedLen;
			int iPosY = rcLayout.top;
			int iPosX = rcLayout.left;

			pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->DM_IsVisible(false)&&(false == pChild->m_pDUIXmlInfo->m_bPlaceHolder)// 不可见也不占位
					||pChild->m_bFloatLayout)
				{
					pChild=pChild->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}

				int nMinHeight = pChild->m_pDUIXmlInfo->m_nMinHeight;
				int nMaxHeight = pChild->m_pDUIXmlInfo->m_nMaxHeight;
				int nMinWidth  = pChild->m_pDUIXmlInfo->m_nMinWidth;
				int nMaxWidth  = pChild->m_pDUIXmlInfo->m_nMaxWidth;
				CRect &rcPadd = pChild->m_pDUIXmlInfo->m_rcPadd;
				szRemain.cy -= rcPadd.top;// 这里先减去top

				// 计算y
				CSize szMeasure(POS_INIT,POS_INIT);
				pChild->DV_Measure(szMeasure);
				if (POS_INIT == szMeasure.cy)// 没有设置高度的控件
				{
					iAdjustIndex++;
					szMeasure.cy = ncyAdjustItemLen;

					if (iAdjustIndex == nAdjustNum)
					{// 最后一个没有设置高度的控件，因为前面可能有最大最小限制使得cy!=ncyAdjustItemLen
					 // +rcPadd.top是因为前面减了rcPadd.top,而nCyFixedLenRemain中包含了rcPadd.top+rcPadd.bottm
					 // 这只是最后一个没设置高度的控件，不一定是最后的控件，所以要-nCyFixedLenRemain
						szMeasure.cy = szRemain.cy+rcPadd.top-nCyFixedLenRemain;
						szMeasure.cy = szMeasure.cy<=0?0:szMeasure.cy;
					}
					if (nMinHeight!=-1
						&&szMeasure.cy<nMinHeight)
					{
						szMeasure.cy = nMinHeight;
					}
					if (nMaxHeight!=-1
						&&szMeasure.cy>nMaxHeight)
					{
						szMeasure.cy = nMaxHeight;
					}
				}
				else
				{
					if (nMinHeight!=-1
						&&szMeasure.cy<nMinHeight)
					{
						szMeasure.cy = nMinHeight;
					}
					if (nMaxHeight!=-1
						&&szMeasure.cy>nMaxHeight)
					{
						szMeasure.cy = nMaxHeight;
					}
					nCyFixedLenRemain -= szMeasure.cy;
				}

				// 计算x
				szMeasure.cx = pChild->m_pDUIXmlInfo->m_iWidth;
				if (-1 == szMeasure.cx)
				{// 初始默认值-1
					szMeasure.cx = szAvailable.cx-rcPadd.left-rcPadd.right;
				}
				else if (szMeasure.cx<0)
				{
					szMeasure.cx = 0;
				}
				if (nMinWidth!=-1
					&&szMeasure.cx<nMinWidth)
				{
					szMeasure.cx = nMinWidth;
				}
				if (nMaxWidth!=-1
					&&szMeasure.cx>nMaxWidth)
				{
					szMeasure.cx = nMaxWidth;
				}

				CRect &rcWindow  = rcLayout;// 容器大小
				CRect rcArrange(iPosX+rcPadd.left,iPosY+rcPadd.top,iPosX+rcPadd.left+szMeasure.cx, iPosY+rcPadd.top+szMeasure.cy);
				pChild->DV_Arrange(rcArrange);

				pChild=pChild->DM_GetWindow(GDW_NEXTSIBLING);
				iPosY += szMeasure.cy + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.top+rcPadd.bottom;
				szRemain.cy -= szMeasure.cy + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.bottom;// 最前面减去top了
				nCyFixedLenRemain -= (NULL == pChild?0:m_iChildPaddLen) + rcPadd.top+rcPadd.bottom;
			}
		} while (false);
		return DM_ECODE_OK;
	}

	///--------------------------------DUIHLayout----------------
	DMCode DUIHLayout::DV_UpdateChildLayout()
	{
		do 
		{
			if (!DM_IsLayoutFinished())
			{ 
				DMASSERT_EXPR(0,L"窗口未完成自身布局!");
				break;
			}
			// 1.获得子控件的布局空间
			CRect rcLayout;
			DV_GetChildMeasureLayout(rcLayout);
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 3.计算容器可用的大小
			SIZE szAvailable = rcLayout.Size();

			// 4.计算最小的大小
			int nAdjustNum	= 0;   //  没有设置宽度的需自动计算宽度的个数
			int nCxFixedLen = 0;   //  总估值宽度
			int nEstimateNum = 0;  //  估值宽度的个数,包括了nAdjustNum

			DUIWindow *pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->DM_IsVisible(false)&&(false == pChild->m_pDUIXmlInfo->m_bPlaceHolder)// 不可见也不占位
					||m_bFloatLayout)
				{
					pChild=pChild->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}

				// 5. 获得预设的fixsize，像这种水平布局，一般会设置固定的宽度，对应xml的width值
				CSize szMeasure(POS_INIT,POS_INIT);
				pChild->DV_Measure(szMeasure);
				
				CRect &rcPadd  = pChild->m_pDUIXmlInfo->m_rcPadd;
				if (POS_INIT == szMeasure.cx)
				{
					nCxFixedLen += rcPadd.left+rcPadd.right;
					nAdjustNum++;// 没有设置宽度的需自动计算宽度的个数
				}
				else
				{
					int nMinWidth  = pChild->m_pDUIXmlInfo->m_nMinWidth;
					int nMaxWidth  = pChild->m_pDUIXmlInfo->m_nMaxWidth;
					if (nMinWidth!=-1
						&&szMeasure.cx<nMinWidth)
					{
						szMeasure.cx = nMinWidth;
					}
					if (nMaxWidth!=-1
						&&szMeasure.cx>nMaxWidth)
					{
						szMeasure.cx = nMaxWidth;
					}
					nCxFixedLen += szMeasure.cx+rcPadd.left+rcPadd.right;
				}
				nEstimateNum++;
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}

			// 8.子控件之间的额外距离
			nCxFixedLen += (nEstimateNum-1)*m_iChildPaddLen;

			// 开始摆放子控件
			int ncxAdjustItemLen	= 0;
			if (nAdjustNum>0)
			{
				// 9.判断自动计算宽度够不够
				ncxAdjustItemLen = (szAvailable.cx>nCxFixedLen)?((szAvailable.cx-nCxFixedLen)/nAdjustNum):0;
			}

			SIZE szRemain = szAvailable;
			int iAdjustIndex = 0;
			int nCxFixedLenRemain = nCxFixedLen;
			int iPosX = rcLayout.left;

			pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->DM_IsVisible(false)&&(false == pChild->m_pDUIXmlInfo->m_bPlaceHolder)// 不可见也不占位
					||m_bFloatLayout)
				{
					pChild=pChild->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}

				int nMinHeight = pChild->m_pDUIXmlInfo->m_nMinHeight;
				int nMaxHeight = pChild->m_pDUIXmlInfo->m_nMaxHeight;
				int nMinWidth  = pChild->m_pDUIXmlInfo->m_nMinWidth;
				int nMaxWidth  = pChild->m_pDUIXmlInfo->m_nMaxWidth;
				CRect &rcPadd = pChild->m_pDUIXmlInfo->m_rcPadd;
				szRemain.cx -= rcPadd.left;
				// 计算x
				CSize szMeasure(POS_INIT,POS_INIT);
				pChild->DV_Measure(szMeasure);
				if (POS_INIT == szMeasure.cx)// 没有设置宽度的控件
				{
					iAdjustIndex++;
					szMeasure.cx = ncxAdjustItemLen;
					if (iAdjustIndex == nAdjustNum)
					{// 最后一个没有设置宽度的控件，因为前面可能有最大最小限制使得cx!=ncxAdjustItemLen
						szMeasure.cx = szRemain.cx+rcPadd.left-nCxFixedLenRemain;
						szMeasure.cx = szMeasure.cx<=0?0:szMeasure.cx;
					}
					if (nMinWidth!=-1
						&&szMeasure.cx<nMinWidth)
					{
						szMeasure.cx = nMinWidth;
					}
					if (nMaxWidth!=-1
						&&szMeasure.cx>nMaxWidth)
					{
						szMeasure.cx = nMaxWidth;
					}
				}
				else
				{
					if (nMinWidth!=-1
						&&szMeasure.cx<nMinWidth)
					{
						szMeasure.cx = nMinWidth;
					}
					if (nMaxWidth!=-1
						&&szMeasure.cx>nMaxWidth)
					{
						szMeasure.cx = nMaxWidth;
					}
					nCxFixedLenRemain -=szMeasure.cx;
				}

				// 计算y
				szMeasure.cy = pChild->m_pDUIXmlInfo->m_iHeight;
				if (-1 == szMeasure.cy)
				{
					szMeasure.cy =szAvailable.cy-rcPadd.bottom-rcPadd.top;
				}else if (szMeasure.cy<0)
				{
					szMeasure.cy = 0;
				}
				if (nMinHeight!=-1
					&&szMeasure.cy<nMinHeight)
				{
					szMeasure.cy = nMinHeight;
				}
				if (nMaxHeight!=-1
					&&szMeasure.cy>nMaxHeight)
				{
					szMeasure.cy = nMaxHeight;
				}

				CRect &rcWindow  = m_rcWindow;// 容器大小
				CRect rcArrange(iPosX+rcPadd.left,rcWindow.top+rcPadd.top,iPosX+rcPadd.left+szMeasure.cx, rcWindow.top+rcPadd.top+szMeasure.cy);
				pChild->DV_Arrange(rcArrange);
				pChild=pChild->DM_GetWindow(GDW_NEXTSIBLING);
				iPosX += szMeasure.cx + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.left+rcPadd.right;
				szRemain.cx -= szMeasure.cx + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.right;
				nCxFixedLenRemain -= (NULL == pChild?0:m_iChildPaddLen) + rcPadd.left+rcPadd.right;
			}
		} while (false);
		return DM_ECODE_OK;
	}

	/// DUITabLayout---------------------------------------
	DMCode DUITabLayout::DV_UpdateChildLayout()
	{
		do 
		{
			if (!DM_IsLayoutFinished())
			{ 
				DMASSERT_EXPR(0,L"窗口未完成自身布局!");
				break;
			}
			// 1.获得子控件的布局空间
			CRect rcLayout;
			DV_GetChildMeasureLayout(rcLayout);
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 2.判断容器子控件是不是为空
			if (0==m_Node.m_nChildrenCount)
			{
				break;
			}

			if (0 == m_szItem.cx||0==m_szItem.cy)
			{
				break;
			}

			CPoint ptTab(rcLayout.left, rcLayout.top);
			bool bEnd = false;
			DUIWindowPtr pObj = DM_GetWindow(GDW_FIRSTCHILD);
			while (pObj)
			{
				if (!pObj->DM_IsVisible(false)&&(false == pObj->m_pDUIXmlInfo->m_bPlaceHolder)// 不可见也不占位
					||m_bFloatLayout)
				{
					pObj=pObj->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}

				// 设置
				if (rcLayout.right<ptTab.x+m_szItem.cx)
				{
					ptTab.x = rcLayout.left;
					ptTab.y += m_szItem.cy;// 换行
				}
				if (ptTab.y+m_szItem.cy>rcLayout.bottom)
				{
					bEnd = true;// 结束
				}
				CRect rcArrange;
				if (bEnd)
				{
					rcArrange.SetRectEmpty();// 其余的置空吧
				}
				else
				{
					rcArrange.left = ptTab.x;
					rcArrange.top  = ptTab.y;
					rcArrange.right = rcArrange.left+m_szItem.cx;
					rcArrange.bottom = rcArrange.top+m_szItem.cy;
					ptTab.x += m_szItem.cx + m_iChildPaddLen;
				}
				CRect &rcPadd  = pObj->m_pDUIXmlInfo->m_rcPadd;
				rcArrange.DeflateRect(rcPadd);
				pObj->DV_Arrange(rcArrange);
				pObj=pObj->DM_GetWindow(GDW_NEXTSIBLING);
			}
		} while (false);
		return DM_ECODE_OK;
	}

	/// DUIWrapLayout----------------------------------------
	DMCode DUIWrapLayout::DV_UpdateChildLayout()
	{
		do 
		{
			if (!DM_IsLayoutFinished())
			{ 
				DMASSERT_EXPR(0,L"窗口未完成自身布局!");
				break;
			}
			// 1.获得子控件的布局空间
			CRect rcLayout;
			DV_GetChildMeasureLayout(rcLayout);
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 2.判断容器子控件是不是为空
			if (0==m_Node.m_nChildrenCount)
			{
				break;
			}

			// 3.计算容器可用的大小
			SIZE szAvailable = rcLayout.Size();
			if (m_szItem.cx>0)
			{// 也可以为0，这时需要根据设定的m_nColumns来确认
				m_nColumns = rcLayout.Width()/m_szItem.cx;
			}
			if (0 == m_nColumns)
			{
				m_nColumns = 1;
			}

			int cyNeedLen = 0;
			int cxWidth = rcLayout.Width()/m_nColumns;  // Wrap项的宽度

			int cyHeight = m_szItem.cy;
			int iCount   = 0;
			CPoint ptWrap(rcLayout.left, rcLayout.top);
			int iPosX = rcLayout.left;
			CArray<DUIWindowPtr> m_ItemArray;
			DUIWindowPtr pObj = DM_GetWindow(GDW_FIRSTCHILD);
			while (pObj)
			{
				if (!pObj->DM_IsVisible(false)&&(false == pObj->m_pDUIXmlInfo->m_bPlaceHolder)// 不可见也不占位
					||m_bFloatLayout)
				{
					pObj=pObj->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}
				m_ItemArray.Add(pObj);
				pObj=pObj->DM_GetWindow(GDW_NEXTSIBLING);
			}

			int nArrayCount = (int)m_ItemArray.GetCount();
			for (int i=0; i<nArrayCount; i++)
			{
				DUIWindowPtr &pChild = m_ItemArray[i];
				CRect rcWrapItem(ptWrap.x, ptWrap.y, ptWrap.x+cxWidth, ptWrap.y);
				if (0 == (iCount%m_nColumns))// 一行的最开始,在此计算这行的最大height
				{
					int iIndex = iCount;
					for (int j=i;j<nArrayCount; j++)
					{
						DUIWindowPtr &pLineChild = m_ItemArray[j];
						CRect &rcPadd = pLineChild->m_pDUIXmlInfo->m_rcPadd;
						CSize szAvailable(rcWrapItem.right-rcWrapItem.left-rcPadd.left-rcPadd.right,POS_INIT);
						if (iIndex == iCount // 一行的最开始
							||0==(iIndex+1)%m_nColumns// 一行的最后
							)
						{
							szAvailable.cx -= m_iChildPaddLen/2;
						}
						else
						{
							szAvailable.cx -= m_iChildPaddLen;
						}

						int nMinHeight = pLineChild->m_pDUIXmlInfo->m_nMinHeight;
						int nMaxHeight = pLineChild->m_pDUIXmlInfo->m_nMaxHeight;
						int nMinWidth  = pLineChild->m_pDUIXmlInfo->m_nMinWidth;
						int nMaxWidth  = pLineChild->m_pDUIXmlInfo->m_nMaxWidth;
						if (nMinWidth!=-1&&
							szAvailable.cx<nMinWidth)
						{
							szAvailable.cx = nMinWidth;
						}
						if (nMaxWidth!=-1&&
							szAvailable.cx>nMaxWidth)
						{
							szAvailable.cx = nMaxWidth;
						}

						CSize szWrap = szAvailable;
						pLineChild->DV_Measure(szWrap);
						if (POS_INIT == szWrap.cy)
						{// 子项没有设置高度！,设置成默认字体高度，这个后续重构再改
							IDMFontPtr pDefultFont = g_pDMFontPool->GetFont(L"");
							szWrap.cy = DMABS(pDefultFont->GetLogFont()->lfHeight)+10;
						}
						if (nMinWidth!=-1&&
							szWrap.cx<nMinWidth)
						{
							szWrap.cx = nMinWidth;
						}
						if (nMaxWidth!=-1&&
							szWrap.cx>nMaxWidth)
						{
							szWrap.cx = nMaxWidth;
						}
						if (nMinHeight!=-1&&
							szWrap.cy<nMinHeight)
						{
							szWrap.cy = nMinHeight;
						}
						if (nMaxHeight!=-1&&
							szWrap.cy>nMaxHeight)
						{
							szWrap.cy = nMaxHeight;
						}
						int nHeightTemp = szWrap.cy+rcPadd.top+rcPadd.bottom;
						cyHeight = cyHeight>nHeightTemp?cyHeight:nHeightTemp; // 计算出这一列中最高的项做为一行的height
						if (0==(++iIndex%m_nColumns))
						{
							break;
						}
					}
				}
				CRect &rcPadd = pChild->m_pDUIXmlInfo->m_rcPadd;
				rcWrapItem.left += rcPadd.left + m_iChildPaddLen/2;
				rcWrapItem.right -= rcPadd.right + m_iChildPaddLen/2;
				if (0 == (iCount%m_nColumns))
				{// 一行的最开始
					rcWrapItem.left -= m_iChildPaddLen/2;
				}
				if (0 == (iCount+1)%m_nColumns)
				{// 一行的最后
					rcWrapItem.right += m_iChildPaddLen/2;
				}

				int nMinHeight = pChild->m_pDUIXmlInfo->m_nMinHeight;
				int nMaxHeight = pChild->m_pDUIXmlInfo->m_nMaxHeight;
				int nMinWidth  = pChild->m_pDUIXmlInfo->m_nMinWidth;
				int nMaxWidth  = pChild->m_pDUIXmlInfo->m_nMaxWidth;
				rcWrapItem.top = ptWrap.y + rcPadd.top;
				rcWrapItem.bottom = ptWrap.y + cyHeight;
				CSize szAvailable(rcWrapItem.right-rcWrapItem.left, rcWrapItem.bottom-rcWrapItem.top);
				CSize szWrap = szAvailable;
				pChild->DV_Measure(szWrap);
				if (-1!=nMinWidth&&
					szWrap.cx<nMinWidth)
				{
					szWrap.cx = nMinWidth;
				}
				if (-1!=nMaxWidth&&
					szWrap.cx>nMaxWidth)
				{
					szWrap.cx = nMaxWidth;
				}
				if (-1!=nMinHeight&&
					szWrap.cy<nMinHeight)
				{
					szWrap.cy = nMinHeight;
				}
				if (-1!=nMaxHeight&&
					szWrap.cy>nMaxHeight)
				{
					szWrap.cy = nMaxHeight;
				}
				CRect rcArrange;
				rcArrange.left = (rcWrapItem.left+rcWrapItem.right-szWrap.cx)/2;
				rcArrange.top  = (rcWrapItem.top+rcWrapItem.bottom-szWrap.cy)/2;
				rcArrange.right = rcArrange.left + szWrap.cx;
				rcArrange.bottom = rcArrange.top + szWrap.cy;
				pChild->DV_Arrange(rcArrange);
				if (0 == (++iCount%m_nColumns))
				{
					ptWrap.x = iPosX;
					ptWrap.y += cyHeight + m_iChildPaddLen;
					cyHeight = m_szItem.cy;
				}
				else
				{
					ptWrap.x += cxWidth;
				}
				cyNeedLen = rcWrapItem.bottom-rcWrapItem.top;
			}

		} while (false);
		return DM_ECODE_OK;
	}

	//--------------------------
	DUISplitLayout::DUISplitLayout()
	{
		m_bVert				= false;
		m_bFirstChange		= true;
		m_iFirstChildWidth  = 20;
		m_iFirstChildPercent= -1;
		m_iSliderWid		= 1;
		m_iFixWid			= -1;
		m_bDrag				= false;
		m_ClrSlider.SetTextInvalid();
	}

	DMCode DUISplitLayout::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			iErr = __super::DV_CreateChildWnds(XmlNode);
			if (2!=m_Node.m_nChildrenCount)
			{
				DMASSERT_EXPR(0,L"内置splitter仅支持两个子窗口");
				break;
			}
		} while (false);
		return iErr;
	}

	//
	void DUISplitLayout::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			__super::DM_OnPaint(pCanvas);
			if (!m_pSliderSkin.isValid()&&m_ClrSlider.IsTextInvalid())
			{
				break;
			}
			CRect rcSlider = GetSliderRect();
			if (m_pSliderSkin)
			{
				m_pSliderSkin->Draw(pCanvas,rcSlider,0);
			}
			else if (!m_ClrSlider.IsTextInvalid())
			{
				pCanvas->FillSolidRect(rcSlider,m_ClrSlider);
			}
		} while (false);
	}

	void DUISplitLayout::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		do 
		{
			__super::OnLButtonDown(nFlags,pt);
			m_bDrag = true;
			m_ptDrag = pt;
		} while (false);
	}

	void DUISplitLayout::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		do 
		{
			__super::OnLButtonUp(nFlags,pt);
			m_bDrag = false;
		} while (false);
	}

	void DUISplitLayout::OnMouseMove(UINT nFlags,CPoint pt)
	{
		do 
		{
			if (false == m_bDrag)
			{
				__super::OnMouseMove(nFlags,pt);
				break;
			}

			CPoint ptDiff = m_ptDrag;
			m_ptDrag      = pt;
			ptDiff        = m_ptDrag - ptDiff;
			int iDiff     = m_bVert?ptDiff.y:ptDiff.x;
			UpdateDrag(iDiff);	
		} while (false);
	}

	//
	DMCode DUISplitLayout::DV_UpdateChildLayout()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!DM_IsLayoutFinished())
			{ 
				DMASSERT_EXPR(0,L"窗口未完成自身布局!");
				break;
			} 
			if (m_rcWindow.IsRectEmpty())
			{
				break;
			}

			if (2!=m_Node.m_nChildrenCount)
			{
				DMASSERT_EXPR(0,L"内置splitter仅支持两个子窗口");
				break;
			} 
			if (m_bVert)// 坚直
			{
				//1.第一次初始化
				if (-1 == m_iFixWid)
				{
					CRect rcFirst = m_rcWindow;
					int iFirstChildWidth = m_iFirstChildWidth;
					if (-1  != m_iFirstChildPercent)
					{
						iFirstChildWidth = (m_rcWindow.Height()*m_iFirstChildPercent)/100;
					}
					rcFirst.bottom = rcFirst.top+iFirstChildWidth;
					if (m_rcWindow.bottom-rcFirst.bottom<m_iSliderWid)//1.1.剩余高度不足，补足
					{
						rcFirst.bottom = m_rcWindow.bottom- m_iSliderWid;
					}
					m_Node.m_pFirstChild->DV_Arrange(rcFirst);
					CRect rcSecond = m_rcWindow;
					rcSecond.top = rcFirst.bottom+m_iSliderWid;
					m_Node.m_pLastChild->DV_Arrange(rcSecond);

					UpdateFixWidth();
				} 
				else//2.已初始化
				{
					CRect rcFirst = m_rcWindow;
					if (m_iFixWid+m_iSliderWid>m_rcWindow.Height())// 子窗口高度+滑块高度大于父窗口的可用区域了
					{
						m_iFixWid = m_rcWindow.Height()-m_iSliderWid;
					}
					m_iFixWid = m_iFixWid>0?m_iFixWid:0;
					if (m_bFirstChange)
					{
						rcFirst.bottom -= m_iFixWid+m_iSliderWid;
					}
					else
					{
						rcFirst.bottom = rcFirst.top+m_iFixWid;
					}

					m_Node.m_pFirstChild->DV_Arrange(rcFirst);
					CRect rcSecond = m_rcWindow;
					rcSecond.top = rcFirst.bottom+m_iSliderWid;
					m_Node.m_pLastChild->DV_Arrange(rcSecond);
				}
			}
			else
			{
				//1.第一次初始化
				if (-1 == m_iFixWid)
				{
					CRect rcFirst = m_rcWindow;
					int iFirstChildWidth = m_iFirstChildWidth;
					if (-1  != m_iFirstChildPercent)
					{
						iFirstChildWidth = (m_rcWindow.Width()*m_iFirstChildPercent)/100;
					}
					rcFirst.right = rcFirst.left+iFirstChildWidth;
					if (m_rcWindow.right-rcFirst.right<m_iSliderWid)//1.1.剩余高度不足，补足
					{
						rcFirst.right = m_rcWindow.right- m_iSliderWid;
					}
					m_Node.m_pFirstChild->DV_Arrange(rcFirst);
					CRect rcSecond = m_rcWindow;
					rcSecond.left = rcFirst.right+m_iSliderWid;
					m_Node.m_pLastChild->DV_Arrange(rcSecond);

					UpdateFixWidth();
				}
				else//2.已初始化
				{
					CRect rcFirst = m_rcWindow;
					if (m_iFixWid+m_iSliderWid>m_rcWindow.Width())// 子窗口宽度+滑块宽度大于父窗口的可用区域了
					{
						m_iFixWid = m_rcWindow.Width()-m_iSliderWid;
					}
					m_iFixWid = m_iFixWid>0?m_iFixWid:0;
					if (m_bFirstChange)
					{
						rcFirst.right -= m_iFixWid+m_iSliderWid;
					}
					else
					{
						rcFirst.right = rcFirst.left+m_iFixWid;
					}
					m_Node.m_pFirstChild->DV_Arrange(rcFirst);
					CRect rcSecond = m_rcWindow;
					rcSecond.left = rcFirst.right+m_iSliderWid;
					m_Node.m_pLastChild->DV_Arrange(rcSecond);
				}
			}
		} while (false);
		return iErr;
	}

	DMCode DUISplitLayout::DV_OnSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			HCURSOR hCursor = NULL;
			m_pDUIXmlInfo->m_pStyle->GetCursor(hCursor);
			if (NULL == hCursor)
			{
				hCursor = ::LoadCursor(NULL,m_bVert?IDC_SIZENS:IDC_SIZEWE);
			}
			::SetCursor(hCursor);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	///
	DMCode DUISplitLayout::UpdateFixWidth()
	{
		if (m_bFirstChange)// 固定大小为第二个窗口
		{
			if (m_bVert)
			{
				m_iFixWid = m_Node.m_pLastChild->m_rcWindow.Height();
			}
			else
			{
				m_iFixWid = m_Node.m_pLastChild->m_rcWindow.Width();
			}
		}
		else
		{
			if (m_bVert)
			{
				m_iFixWid = m_Node.m_pFirstChild->m_rcWindow.Height();
			}
			else
			{
				m_iFixWid = m_Node.m_pFirstChild->m_rcWindow.Width();
			}
		}
		if (0>m_iFixWid)
		{
			DMASSERT_EXPR(0,L"固定的m_iFixWid大小竟然小于0！");
		}

		return DM_ECODE_OK;
	}

	DMCode DUISplitLayout::UpdateDrag(int iDiff)
	{
		do 
		{
			if (m_bFirstChange)
			{
				m_iFixWid -= iDiff; 
			}
			else
			{
				m_iFixWid += iDiff;
			}
			if (m_iFixWid<0)
			{
				m_iFixWid = 0;
			}
			int nlimit = m_bVert?m_rcWindow.Height()-m_iSliderWid:m_rcWindow.Width()-m_iSliderWid;
			if (m_iFixWid>nlimit)
			{
				m_iFixWid = nlimit;
			}
			DV_UpdateChildLayout();
			DM_Invalidate();
		} while (false);
		return DM_ECODE_OK;
	}

	CRect DUISplitLayout::GetSliderRect()
	{
		CRect rcSlider;
		do 
		{
			CRect rcFirstWnd = m_Node.m_pFirstChild->m_rcWindow;
			if (m_bVert)// 竖直
			{
				rcSlider = m_rcWindow;
				rcSlider.top = m_Node.m_pFirstChild->m_rcWindow.bottom;
				rcSlider.bottom = rcSlider.top+m_iSliderWid;
			}
			else// 水平
			{
				rcSlider = m_rcWindow;
				rcSlider.left = m_Node.m_pFirstChild->m_rcWindow.right;
				rcSlider.right = rcSlider.left+m_iSliderWid;
			}
		} while (false);
		return rcSlider;
	}

	DMCode DUISplitLayout::OnAttributeFirstChildWidth(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			int temp = m_iFirstChildWidth;
			dm_parseint(lpszValue,temp);
			if (temp<0)
			{
				break;
			}
			m_iFirstChildWidth = temp;
			if (!bLoadXml)// 重新初始化
			{
				m_iFixWid = -1;
				DV_UpdateChildLayout();
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}


	DMCode DUISplitLayout::OnAttributeFirstChildPercent(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CStringW strValue = lpszValue;
			if (strValue.IsEmpty())
			{
				break;
			}

			if (0 == strValue.CompareNoCase(L"-1"))
			{
				m_iFirstChildPercent = -1;
				iErr = DM_ECODE_OK;
				break;
			}

			if (strValue[0] != L'%')
			{
				break;
			}
			strValue = ++lpszValue;
			int temp = m_iFirstChildPercent;
			dm_parseint(lpszValue,temp);
			m_iFirstChildPercent = DMABS(temp);
			iErr = DM_ECODE_OK;
		} while (false);
		if (DMSUCCEEDED(iErr))
		{		
			if (!bLoadXml)// 重新初始化
			{
				m_iFixWid = -1;
				DV_UpdateChildLayout();
			}
		}
		return iErr;
	}

}//namespace DM