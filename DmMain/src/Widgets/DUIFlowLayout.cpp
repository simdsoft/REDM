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
				DMFAIL_MSG("window layout not ready");
				break;
			}

			// 1.����ӿؼ��Ĳ��ֿռ�
			CRect rcLayout;
			DV_GetChildMeasureLayout(rcLayout);
			if (rcLayout.IsRectEmpty())
			{
				break;
			}
			// 2.�ж������ӿؼ��ǲ���Ϊ��
			if (0==m_Node.m_nChildrenCount)
			{
				break;
			}

			// 3.�����������õĴ�С
			SIZE szAvailable = rcLayout.Size();

			// 4.������С�Ĵ�С 
			int nAdjustNum	= 0;   //  û�����ø߶ȵ����Զ�����߶ȵĸ���
			int nCyFixedLen = 0;   //  �ܹ�ֵ�߶�
			int nEstimateNum = 0;  //  ��ֵ�߶ȵĸ���,������nAdjustNum

			DUIWindow *pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->DM_IsVisible(false)&&(false == pChild->m_pDUIXmlInfo->m_bPlaceHolder)// ���ɼ�Ҳ��ռλ
					||pChild->m_bFloatLayout)
				{
					pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}

				// 5. ���Ԥ���fixsize����������ֱ���֣�һ������ù̶��ĸ߶ȣ���Ӧxml��heightֵ
				CSize szMeasure(POS_INIT,POS_INIT);
				pChild->DV_Measure(szMeasure);// �������˸߶ȣ����Զ���ֵ
				CRect &rcPadd  = pChild->m_pDUIXmlInfo->m_rcPadd;
				if (POS_INIT == szMeasure.cy)
				{
					nAdjustNum++;// û�����ø߶ȵ����Զ�����߶ȵĸ���
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

			// 8.�ӿؼ�֮��Ķ������
			nCyFixedLen += (nEstimateNum-1)*m_iChildPaddLen;

			// ��ʼ�ڷ��ӿؼ�
			int ncyAdjustItemLen	= 0;
			if (nAdjustNum>0)
			{
				// 9.�ж��Զ�����߶ȹ�����
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
				if (!pChild->DM_IsVisible(false)&&(false == pChild->m_pDUIXmlInfo->m_bPlaceHolder)// ���ɼ�Ҳ��ռλ
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
				szRemain.cy -= rcPadd.top;// �����ȼ�ȥtop

				// ����y
				CSize szMeasure(POS_INIT,POS_INIT);
				pChild->DV_Measure(szMeasure);
				if (POS_INIT == szMeasure.cy)// û�����ø߶ȵĿؼ�
				{
					iAdjustIndex++;
					szMeasure.cy = ncyAdjustItemLen;

					if (iAdjustIndex == nAdjustNum)
					{// ���һ��û�����ø߶ȵĿؼ�����Ϊǰ������������С����ʹ��cy!=ncyAdjustItemLen
					 // +rcPadd.top����Ϊǰ�����rcPadd.top,��nCyFixedLenRemain�а�����rcPadd.top+rcPadd.bottm
					 // ��ֻ�����һ��û���ø߶ȵĿؼ�����һ�������Ŀؼ�������Ҫ-nCyFixedLenRemain
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

				// ����x
				szMeasure.cx = pChild->m_pDUIXmlInfo->m_iWidth;
				if (-1 == szMeasure.cx)
				{// ��ʼĬ��ֵ-1
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

				CRect &rcWindow  = rcLayout;// ������С
				CRect rcArrange(iPosX+rcPadd.left,iPosY+rcPadd.top,iPosX+rcPadd.left+szMeasure.cx, iPosY+rcPadd.top+szMeasure.cy);
				pChild->DV_Arrange(rcArrange);

				pChild=pChild->DM_GetWindow(GDW_NEXTSIBLING);
				iPosY += szMeasure.cy + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.top+rcPadd.bottom;
				szRemain.cy -= szMeasure.cy + (NULL == pChild?0:m_iChildPaddLen) + rcPadd.bottom;// ��ǰ���ȥtop��
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
				DMFAIL_MSG("window layout not ready");
				break;
			}
			// 1.����ӿؼ��Ĳ��ֿռ�
			CRect rcLayout;
			DV_GetChildMeasureLayout(rcLayout);
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 3.�����������õĴ�С
			SIZE szAvailable = rcLayout.Size();

			// 4.������С�Ĵ�С
			int nAdjustNum	= 0;   //  û�����ÿ�ȵ����Զ������ȵĸ���
			int nCxFixedLen = 0;   //  �ܹ�ֵ���
			int nEstimateNum = 0;  //  ��ֵ��ȵĸ���,������nAdjustNum

			DUIWindow *pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->DM_IsVisible(false)&&(false == pChild->m_pDUIXmlInfo->m_bPlaceHolder)// ���ɼ�Ҳ��ռλ
					||m_bFloatLayout)
				{
					pChild=pChild->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}

				// 5. ���Ԥ���fixsize��������ˮƽ���֣�һ������ù̶��Ŀ�ȣ���Ӧxml��widthֵ
				CSize szMeasure(POS_INIT,POS_INIT);
				pChild->DV_Measure(szMeasure);
				
				CRect &rcPadd  = pChild->m_pDUIXmlInfo->m_rcPadd;
				if (POS_INIT == szMeasure.cx)
				{
					nCxFixedLen += rcPadd.left+rcPadd.right;
					nAdjustNum++;// û�����ÿ�ȵ����Զ������ȵĸ���
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

			// 8.�ӿؼ�֮��Ķ������
			nCxFixedLen += (nEstimateNum-1)*m_iChildPaddLen;

			// ��ʼ�ڷ��ӿؼ�
			int ncxAdjustItemLen	= 0;
			if (nAdjustNum>0)
			{
				// 9.�ж��Զ������ȹ�����
				ncxAdjustItemLen = (szAvailable.cx>nCxFixedLen)?((szAvailable.cx-nCxFixedLen)/nAdjustNum):0;
			}

			SIZE szRemain = szAvailable;
			int iAdjustIndex = 0;
			int nCxFixedLenRemain = nCxFixedLen;
			int iPosX = rcLayout.left;

			pChild = DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->DM_IsVisible(false)&&(false == pChild->m_pDUIXmlInfo->m_bPlaceHolder)// ���ɼ�Ҳ��ռλ
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
				// ����x
				CSize szMeasure(POS_INIT,POS_INIT);
				pChild->DV_Measure(szMeasure);
				if (POS_INIT == szMeasure.cx)// û�����ÿ�ȵĿؼ�
				{
					iAdjustIndex++;
					szMeasure.cx = ncxAdjustItemLen;
					if (iAdjustIndex == nAdjustNum)
					{// ���һ��û�����ÿ�ȵĿؼ�����Ϊǰ������������С����ʹ��cx!=ncxAdjustItemLen
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

				// ����y
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

				CRect &rcWindow  = m_rcWindow;// ������С
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
				DMFAIL_MSG("window layout not ready");
				break;
			}
			// 1.����ӿؼ��Ĳ��ֿռ�
			CRect rcLayout;
			DV_GetChildMeasureLayout(rcLayout);
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 2.�ж������ӿؼ��ǲ���Ϊ��
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
				if (!pObj->DM_IsVisible(false)&&(false == pObj->m_pDUIXmlInfo->m_bPlaceHolder)// ���ɼ�Ҳ��ռλ
					||m_bFloatLayout)
				{
					pObj=pObj->DM_GetWindow(GDW_NEXTSIBLING);
					continue;
				}

				// ����
				if (rcLayout.right<ptTab.x+m_szItem.cx)
				{
					ptTab.x = rcLayout.left;
					ptTab.y += m_szItem.cy;// ����
				}
				if (ptTab.y+m_szItem.cy>rcLayout.bottom)
				{
					bEnd = true;// ����
				}
				CRect rcArrange;
				if (bEnd)
				{
					rcArrange.SetRectEmpty();// ������ÿհ�
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
				DMFAIL_MSG("window layout not ready");
				break;
			}
			// 1.����ӿؼ��Ĳ��ֿռ�
			CRect rcLayout;
			DV_GetChildMeasureLayout(rcLayout);
			if (rcLayout.IsRectEmpty())
			{
				break;
			}

			// 2.�ж������ӿؼ��ǲ���Ϊ��
			if (0==m_Node.m_nChildrenCount)
			{
				break;
			}

			// 3.�����������õĴ�С
			SIZE szAvailable = rcLayout.Size();
			if (m_szItem.cx>0)
			{// Ҳ����Ϊ0����ʱ��Ҫ�����趨��m_nColumns��ȷ��
				m_nColumns = rcLayout.Width()/m_szItem.cx;
			}
			if (0 == m_nColumns)
			{
				m_nColumns = 1;
			}

			int cyNeedLen = 0;
			int cxWidth = rcLayout.Width()/m_nColumns;  // Wrap��Ŀ��

			int cyHeight = m_szItem.cy;
			int iCount   = 0;
			CPoint ptWrap(rcLayout.left, rcLayout.top);
			int iPosX = rcLayout.left;
			CArray<DUIWindowPtr> m_ItemArray;
			DUIWindowPtr pObj = DM_GetWindow(GDW_FIRSTCHILD);
			while (pObj)
			{
				if (!pObj->DM_IsVisible(false)&&(false == pObj->m_pDUIXmlInfo->m_bPlaceHolder)// ���ɼ�Ҳ��ռλ
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
				if (0 == (iCount%m_nColumns))// һ�е��ʼ,�ڴ˼������е����height
				{
					int iIndex = iCount;
					for (int j=i;j<nArrayCount; j++)
					{
						DUIWindowPtr &pLineChild = m_ItemArray[j];
						CRect &rcPadd = pLineChild->m_pDUIXmlInfo->m_rcPadd;
						CSize szAvailable(rcWrapItem.right-rcWrapItem.left-rcPadd.left-rcPadd.right,POS_INIT);
						if (iIndex == iCount // һ�е��ʼ
							||0==(iIndex+1)%m_nColumns// һ�е����
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
						{// ����û�����ø߶ȣ�,���ó�Ĭ������߶ȣ���������ع��ٸ�
							IDMFontPtr pDefultFont = g_pDMFontPool->GetFont("");
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
						cyHeight = cyHeight>nHeightTemp?cyHeight:nHeightTemp; // �������һ������ߵ�����Ϊһ�е�height
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
				{// һ�е��ʼ
					rcWrapItem.left -= m_iChildPaddLen/2;
				}
				if (0 == (iCount+1)%m_nColumns)
				{// һ�е����
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
		m_iMinimumWid		= -1;
		m_iMaximumWid		= -1;
		m_bDrag				= false;
		m_ClrSlider.SetTextInvalid();
	}

	DMCode DUISplitLayout::DV_CreateChildWnds(const DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			iErr = __super::DV_CreateChildWnds(XmlNode);
			if (2!=m_Node.m_nChildrenCount)
			{
				DMFAIL_MSG("buitlin splitter only support 2 child window");
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
			int iOldFixWid = m_iFixWid;
			UpdateDrag(iDiff);
			int iRealDiff = m_bFirstChange ? iOldFixWid - m_iFixWid : m_iFixWid - iOldFixWid;
			if (iRealDiff != iDiff)
			{
				if (m_bVert)
					m_ptDrag.y += iRealDiff - iDiff;
				else
					m_ptDrag.x += iRealDiff - iDiff;
			}
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
				DMFAIL_MSG("window layout not ready");
				break;
			} 
			if (m_rcWindow.IsRectEmpty())
			{
				break;
			}

			if (2!=m_Node.m_nChildrenCount)
			{
				DMFAIL_MSG("buitlin splitter only support 2 child window");
				break;
			} 
			if (m_bVert)// ��ֱ
			{
				//1.��һ�γ�ʼ��
				if (-1 == m_iFixWid)
				{
					CRect rcFirst = m_rcWindow;
					int iFirstChildWidth = m_iFirstChildWidth;
					if (-1  != m_iFirstChildPercent)
					{
						iFirstChildWidth = (m_rcWindow.Height()*m_iFirstChildPercent)/100;
					}
					rcFirst.bottom = rcFirst.top+iFirstChildWidth;
					if (m_rcWindow.bottom-rcFirst.bottom<m_iSliderWid)//1.1.ʣ��߶Ȳ��㣬����
					{
						rcFirst.bottom = m_rcWindow.bottom- m_iSliderWid;
					}
					m_Node.m_pFirstChild->DV_Arrange(rcFirst);
					CRect rcSecond = m_rcWindow;
					rcSecond.top = rcFirst.bottom+m_iSliderWid;
					m_Node.m_pLastChild->DV_Arrange(rcSecond);

					UpdateFixWidth();
				} 
				else//2.�ѳ�ʼ��
				{
					CRect rcFirst = m_rcWindow;
					if (m_iFixWid+m_iSliderWid>m_rcWindow.Height())// �Ӵ��ڸ߶�+����߶ȴ��ڸ����ڵĿ���������
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
				//1.��һ�γ�ʼ��
				if (-1 == m_iFixWid)
				{
					CRect rcFirst = m_rcWindow;
					int iFirstChildWidth = m_iFirstChildWidth;
					if (-1  != m_iFirstChildPercent)
					{
						iFirstChildWidth = (m_rcWindow.Width()*m_iFirstChildPercent)/100;
					}
					rcFirst.right = rcFirst.left+iFirstChildWidth;
					if (m_rcWindow.right-rcFirst.right<m_iSliderWid)//1.1.ʣ��߶Ȳ��㣬����
					{
						rcFirst.right = m_rcWindow.right- m_iSliderWid;
					}
					m_Node.m_pFirstChild->DV_Arrange(rcFirst);
					CRect rcSecond = m_rcWindow;
					rcSecond.left = rcFirst.right+m_iSliderWid;
					m_Node.m_pLastChild->DV_Arrange(rcSecond);

					UpdateFixWidth();
				}
				else//2.�ѳ�ʼ��
				{
					CRect rcFirst = m_rcWindow;
					if (m_iFixWid+m_iSliderWid>m_rcWindow.Width())// �Ӵ��ڿ��+�����ȴ��ڸ����ڵĿ���������
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
		if (m_bFirstChange)// �̶���СΪ�ڶ�������
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
        
        DMASSERT(m_iFixWid >= 0);

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
			if (-1 != m_iMaximumWid && m_iFixWid > m_iMaximumWid)
			{
				m_iFixWid = m_iMaximumWid;
			}
			else if (-1 != m_iMinimumWid && m_iFixWid < m_iMinimumWid)
			{
				m_iFixWid = m_iMinimumWid;
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
			if (m_bVert)// ��ֱ
			{
				rcSlider = m_rcWindow;
				rcSlider.top = m_Node.m_pFirstChild->m_rcWindow.bottom;
				rcSlider.bottom = rcSlider.top+m_iSliderWid;
			}
			else// ˮƽ
			{
				rcSlider = m_rcWindow;
				rcSlider.left = m_Node.m_pFirstChild->m_rcWindow.right;
				rcSlider.right = rcSlider.left+m_iSliderWid;
			}
		} while (false);
		return rcSlider;
	}

	DMCode DUISplitLayout::OnAttributeFirstChildWidth(LPCSTR lpszValue, bool bLoadXml)
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
			if (!bLoadXml)// ���³�ʼ��
			{
				m_iFixWid = -1;
				DV_UpdateChildLayout();
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}


	DMCode DUISplitLayout::OnAttributeFirstChildPercent(LPCSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CStringA strValue = lpszValue;
			if (strValue.IsEmpty())
			{
				break;
			}

			if (0 == strValue.CompareNoCase("-1"))
			{
				m_iFirstChildPercent = -1;
				iErr = DM_ECODE_OK;
				break;
			}

			if (strValue[0] != '%')
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
			if (!bLoadXml)// ���³�ʼ��
			{
				m_iFixWid = -1;
				DV_UpdateChildLayout();
			}
		}
		return iErr;
	}

}//namespace DM