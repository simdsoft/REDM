#include "DmMainAfx.h"
#include "DMLayoutImpl.h"

namespace DM
{
	DMLayoutImpl::DMLayoutImpl()
	{
		m_pOwner		= NULL;
		m_nCount		= 0;
		m_uPositionType = 0;
		m_nSepSpace     = 2;
		m_size.SetSize(POS_INIT,POS_INIT);
	}

	DMCode DMLayoutImpl::SetOwner(LPVOID lpWnd)
	{
		m_pOwner = (DUIWindow *)lpWnd;
		return DM_ECODE_OK;
	}

	DMCode DMLayoutImpl::UpdateLayout(LPRECT lpRcContainer,OUT CRect &rcWindow)
	{
		DMCode iErr = DM_ECODE_FAIL;
		int nRet = 0; // 未初始化成功的次数
		do 
		{
			rcWindow.SetRect(POS_INIT,POS_INIT,POS_INIT,POS_INIT);// 先使原窗口坐标无效
			CRect rcContainer;
			if (!lpRcContainer)
			{
				if (!m_pOwner->DM_GetWindow(GDW_PARENT))
				{
					break;
				}
				m_pOwner->DM_GetWindow(GDW_PARENT)->DV_GetChildMeasureLayout(rcContainer);
				lpRcContainer = &rcContainer;
			}
			rcContainer = lpRcContainer;

			if (4 == m_nCount)// 指定了4个坐标
			{
				if (!Update4(lpRcContainer,rcWindow))
				{
					break;
				}
			}
			else
			{
				if (2 == m_nCount)// 指定了2个坐标
				{
					if (!Update2(lpRcContainer,rcWindow))
					{
						break;
					}
				}
				else if (0 == m_nCount && // 使用了填满布局可用空间方式
					((m_uPositionType&SizeX_FitParent)&&(m_uPositionType &SizeY_FitParent)))
				{
					if (!UpdateFull(lpRcContainer,rcWindow))
					{
						break;
					}
				}
				else// 自动排版
				{
					if (!UpdateAuto(lpRcContainer,rcWindow))
					{
						break;
					}
				}
			}
			rcWindow.NormalizeRect();
			rcWindow = rcWindow&rcContainer;// 锁定子区域不能超过容器区域
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMLayoutImpl::UpdateChildLayout()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DM::CList<DUIWindow*> lstWnd;
			DUIWindow *pChild = m_pOwner->DM_GetWindow(GDW_FIRSTCHILD);
			while (pChild)
			{
				if (!pChild->m_bFloatLayout)
				{
					pChild->m_rcWindow.SetRect(POS_INIT,POS_INIT,POS_INIT,POS_INIT);
					lstWnd.AddTail(pChild);
				}
				else
				{
					// 绝对坐标不需要重置
				}
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}
			if (ParseChildPosition(&lstWnd))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	// 辅助--------------------------
	bool DMLayoutImpl::ParseChildPosition(DM::CList<DUIWindow*> *pList)
	{
		CRect rcContainer;
		m_pOwner->DV_GetChildMeasureLayout(rcContainer);
		POSITION pos	  = pList->GetHeadPosition();
		int iCount		  = pList->GetCount();
		while (pos)
		{
			POSITION posOld	  = pos;
			DUIWindow *pChild = pList->GetNext(pos);
			//if (0 == pChild->DM_SendMessage(WM_WINDOWPOSCHANGED,0,(LPARAM)&rcContainer))
			if (0 == pChild->DM_UpdateLayout(&rcContainer))// 不发消息了，直接调用
			{
				pList->RemoveAt(posOld);
			}
		}
		if (0 == pList->GetCount())
		{
			return true;
		}

		if(iCount == pList->GetCount())
		{
			DMASSERT_EXPR(0,L"亲,布局有问题,死锁了!");
			return false;
		}
		else
		{
			return ParseChildPosition(pList);// 递归
		}
	}

	bool DMLayoutImpl::ParsePostion()
	{
		CStringW strPos = m_strPosValue;
		bool bRet = false;
		do 
		{
			CStringWList strPosList;
			SplitStringT(strPos,L',',strPosList);
			m_nCount = (int)strPosList.GetCount();
			if (2!=m_nCount&&4!=m_nCount)
			{
				break;
			}

			// 解析item-----------------------------
			POS_ITEM item[4];
			memset(item,0, sizeof(POS_ITEM)*4);
			for (int i=0;i<m_nCount;i++)
			{
				ParseItem(strPosList[i],item[i]);
			}
			if (PIT_OFFSET==item[0].pit||PIT_OFFSET==item[1].pit)
			{
				DMASSERT_EXPR(0,L"布局前两个参数不能使用@!");
				break;
			}
			m_Left = item[0];m_Top = item[1];m_Right = item[2]; m_Bottom = item[3];
			if(2 == m_nCount)
			{
				m_uPositionType = (m_uPositionType&~SizeX_Mask) | SizeX_FitContent;
				m_uPositionType = (m_uPositionType&~SizeY_Mask) | SizeY_FitContent;
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	// 仅在ParsePostion解析后，m_nCount<4时调用---------------------------- 
	bool DMLayoutImpl::ParsePostionType()
	{
		bool bRet = false;
		do 
		{
			if (4 == m_nCount)
			{
				bRet = true;
				break;
			}
			if (POS_INIT == m_size.cx||POS_INIT == m_size.cy)
			{
				break;
			}
			int nWidValue = m_size.cx;
			int nHeiValue = m_size.cy;
			// 先处理width ------------------------
			if (-1 == nWidValue && 0 == m_nCount)// 填满整个父窗口
			{
				m_pOwner->m_rcWindow.right = 0;
				m_uPositionType			   = (m_uPositionType&~SizeX_Mask) | SizeX_FitParent;
			}
			else
			{
				if (nWidValue>=0)// 指定宽度
				{
					m_pOwner->m_rcWindow.right = nWidValue;
					m_uPositionType            = (m_uPositionType&~SizeX_Mask) | SizeX_Specify;
				}
				else if(-1 == nWidValue)// 使用内容宽度
				{
					m_pOwner->m_rcWindow.right = 0;
					m_uPositionType			   = (m_uPositionType&~SizeX_Mask) | SizeX_FitContent;
				}
			}

			// 再处理Height-----------------------
			if (-1 == nHeiValue&&0 == m_nCount)
			{
				m_pOwner->m_rcWindow.bottom = 0;
				m_uPositionType             = (m_uPositionType&~SizeY_Mask) | SizeY_FitParent;
			}
			else
			{
				if (nHeiValue >= 0)
				{
					m_pOwner->m_rcWindow.bottom = nHeiValue;
					m_uPositionType             = (m_uPositionType&~SizeY_Mask) | SizeY_Specify;
				}
				else if(-1 == nHeiValue)
				{
					m_pOwner->m_rcWindow.bottom = 0;
					m_uPositionType             = (m_uPositionType&~SizeY_Mask) | SizeY_FitContent;
				}
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMLayoutImpl::ParseItem(CStringW &strPos, POS_ITEM &item)
	{
		bool bRet = false;
		do 
		{
			if (strPos.IsEmpty())
			{
				DMASSERT_EXPR(0,L"ParseItem项请不要使用空值");
				break;
			}

			LPCWSTR lpszPos = strPos;
			switch (lpszPos[0])
			{
			case POSFLAG_REFCENTER:		item.pit=PIT_CENTER,	lpszPos++;	break;		// 3.1.“|”代表参考父窗口的中心, PIT_CENTER:参考父窗口中心点,以"|"开始
			case POSFLAG_PERCENT:		item.pit=PIT_PERCENT,	lpszPos++;	break;		// 3.2. %”代表在父窗口的百分比, PIT_PERCENT:指定在父窗口坐标的中的百分比,以"%"开头
			case POSFLAG_REFPREV_NEAR:  item.pit=PIT_PREV_NEAR, lpszPos++;  break;		// 3.3.“[”相对于前一兄弟窗口。用于X时，参考前一兄弟窗口的right，用于Y时参考前一兄弟窗口的bottom,PIT_PREV_NEAR:参考前一个兄弟窗口与自己近的边
			case POSFLAG_REFNEXT_NEAR:  item.pit=PIT_NEXT_NEAR, lpszPos++;  break;		// 3.4.“]”相对于后一兄弟窗口。用于X时，参考后一兄弟的left,用于Y时参考后一兄弟的top,PIT_NEXT_NEAR:参考下一个兄弟窗口与自己近的边
			case POSFLAG_REFPREV_FAR:   item.pit=PIT_PREV_FAR,	lpszPos++;  break;		// 3.5.“{”相对于前一兄弟窗口。用于X时，参考前一兄弟窗口的left，用于Y时参考前一兄弟窗口的top,PIT_PREV_FAR:参考前一个兄弟窗口与自己远的边
			case POSFLAG_REFNEXT_FAR:   item.pit=PIT_NEXT_FAR,	lpszPos++;	break;		// 3.6.“}”相对于后一兄弟窗口。用于X时，参考后一兄弟的right,用于Y时参考后一兄弟的bottom,PIT_NEXT_FAR:参考下一个兄弟窗口与自己远的边
			case POSFLAG_DEFSIZE:		item.pit=PIT_OFFSET,	lpszPos++;	break;		// 3.7.@:指定窗口的size。只能用于x2,y2，用于x2时，指定窗口的width，用于y2时指定窗口的height,PIT_OFFSET:相对于前面x1,y1的偏移,只能在x2,y2中使用，以@开头
			default: item.pit=PIT_NORMAL;break;
			}

			item.bMinus = false;
			if (L'-' == lpszPos[0])
			{
				lpszPos++;
				if (PIT_PERCENT != item.pit)// 非百分比值，百分比值不允许使用负值
				{
					item.bMinus = true;
				}
			}
			item.nPos = (float)_wtof(lpszPos);
			if (item.nPos<0.0f && PIT_OFFSET == item.pit) 
			{
				DMASSERT_EXPR(0,L"在使用@时请不要使用负值,内部强制转成正值了");
				item.nPos = DMABS(item.nPos);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	int DMLayoutImpl::ParseItemValue(const POS_ITEM &item,int nMin, int nMax,bool bX)
	{
		int nRet	=	0;
		int nWid	=	nMax-nMin;

		switch(item.pit)
		{
		case PIT_CENTER: 
			nRet = (int)item.nPos*(item.bMinus?-1:1) + nWid/2 + nMin;
			break;

		case PIT_NORMAL: 
			if (item.bMinus)
				nRet = nMax-(int)item.nPos;
			else			
				nRet = nMin+(int)item.nPos;
			break;

		case PIT_PERCENT: 
			nRet = nMin+(int)(item.nPos*nWid/100);
			if (nRet>nMax) 
				nRet=nMax;
			break;

		case PIT_PREV_NEAR:
		case PIT_PREV_FAR:
			{
				// 获得前一个兄弟窗口，没有，则取得父窗口
				DUIWindow *pRefWnd = m_pOwner->DM_GetWindow(GDW_PREVSIBLING);
				CRect rcRef;
				if (pRefWnd)
				{
					pRefWnd->DV_GetWindowRect(&rcRef);
				}
				if (!pRefWnd) 
				{
					pRefWnd = m_pOwner->DM_GetWindow(GDW_PARENT);
					if (pRefWnd)
					{
						pRefWnd->DV_GetChildMeasureLayout(&rcRef);
						CRect rcTemp = rcRef;
						rcRef.right = rcTemp.left;// 父窗口的远近和兄弟窗口相反了
						rcRef.left = rcTemp.right;
						rcRef.top = rcTemp.bottom;
						rcRef.bottom = rcTemp.top;
					}
				}

				if (pRefWnd) //需要确定参考窗口是否完成布局
				{
					if (bX)
					{
						LONG refPos = (item.pit == PIT_PREV_NEAR)?rcRef.right:rcRef.left;
						if (refPos == POS_INIT||refPos==POS_WAIT)
							nRet = POS_WAIT;
						else
							nRet = refPos+(int)item.nPos*(item.bMinus?-1:1);
					}
					else
					{
						LONG refPos = (item.pit == PIT_PREV_NEAR)?rcRef.bottom:rcRef.top;
						if(refPos == POS_INIT || refPos==POS_WAIT)
							nRet = POS_WAIT;
						else
							nRet = refPos+(int)item.nPos*(item.bMinus?-1:1);
					}
				}
			}
			break;
		case PIT_NEXT_NEAR:
		case PIT_NEXT_FAR:
			{
				DUIWindow *pRefWnd = m_pOwner->DM_GetWindow(GDW_NEXTSIBLING);
				CRect rcRef;
				if (pRefWnd)
				{
					pRefWnd->DV_GetWindowRect(&rcRef);
				}
				if (!pRefWnd) 
				{
					pRefWnd = m_pOwner->DM_GetWindow(GDW_PARENT);
					if (pRefWnd)
					{
						pRefWnd->DV_GetChildMeasureLayout(&rcRef);
						CRect rcTemp = rcRef;
						rcRef.right = rcTemp.left;// 父窗口的远近和兄弟窗口相反了
						rcRef.left = rcTemp.right;
						rcRef.top = rcTemp.bottom;
						rcRef.bottom = rcTemp.top;
					}
				}
				if (pRefWnd)//需要确定参考窗口是否完成布局
				{
					if (bX)
					{
						LONG refPos = (item.pit == PIT_NEXT_NEAR)?rcRef.left:rcRef.right;
						if (refPos == POS_INIT || refPos==POS_WAIT)
							nRet = POS_WAIT;
						else
							nRet = refPos+(int)item.nPos*(item.bMinus?-1:1);
					}else
					{
						LONG refPos = (item.pit == PIT_NEXT_NEAR)?rcRef.top:rcRef.bottom;
						if(refPos == POS_INIT || refPos==POS_WAIT)
							nRet = POS_WAIT;
						else
							nRet = refPos+(int)item.nPos*(item.bMinus?-1:1);
					}
				}
			}
			break;
		}
		return nRet;
	}

	bool DMLayoutImpl::Update4(LPRECT lpRcContainer,OUT CRect &rcWindow)
	{
		int nRet = 0;
		if (IsUnInitPos(rcWindow.left))// left
		{
			rcWindow.left = ParseItemValue(m_Left,lpRcContainer->left,lpRcContainer->right,true);
		}
		if (IsUnInitPos(rcWindow.top))// top
		{
			rcWindow.top = ParseItemValue(m_Top,lpRcContainer->top,lpRcContainer->bottom,false);
		}
		if (IsUnInitPos(rcWindow.right))// right
		{
			if (PIT_OFFSET == m_Right.pit)
			{
				if (!IsUnInitPos(rcWindow.left))
				{
					rcWindow.right = rcWindow.left+(LONG)m_Right.nPos;// 如果PIT_OFFSET，那就是加上宽度
				}
			}
			else
			{	
				rcWindow.right = ParseItemValue(m_Right,lpRcContainer->left,lpRcContainer->right,true);
			}
		}
		if (IsUnInitPos(rcWindow.bottom))// bottom
		{
			if (PIT_OFFSET == m_Bottom.pit)
			{
				if (!IsUnInitPos(rcWindow.top))
				{
					rcWindow.bottom = rcWindow.top+(LONG)m_Bottom.nPos;// 如果PIT_OFFSET，那就是加上高度
				}
			}
			else
			{
				rcWindow.bottom = ParseItemValue(m_Bottom,lpRcContainer->top,lpRcContainer->bottom,false);
			}
		}
		if (POS_WAIT==rcWindow.left||POS_WAIT==rcWindow.top||POS_WAIT==rcWindow.right||POS_WAIT==rcWindow.bottom)
		{
			return false;
		}
		return true;
	}

	bool DMLayoutImpl::Update2(LPRECT lpRcContainer,OUT CRect &rcWindow)
	{
		bool bRet = false;
		do 
		{
			CPoint pt = rcWindow.TopLeft();
			CSize sz  = CalcSize(lpRcContainer);
			if (IsUnInitPos(pt.x)) 
			{
				pt.x = ParseItemValue(m_Left,lpRcContainer->left,lpRcContainer->right,true);
			}
			if (IsUnInitPos(pt.y))
			{
				pt.y = ParseItemValue(m_Top,lpRcContainer->top,lpRcContainer->bottom,false);
			}
			if (POS_WAIT == pt.x||POS_WAIT == pt.y)
			{
				break;
			}
			rcWindow = CRect(pt,sz);
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMLayoutImpl::UpdateFull(LPRECT lpRcContainer,OUT CRect &rcWindow)
	{
		bool bRet = false;
		do 
		{
			rcWindow = lpRcContainer;
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMLayoutImpl::UpdateAuto(LPRECT lpRcContainer,OUT CRect &rcWindow)
	{
		bool bRet = false;
		do 
		{
			CPoint pt(lpRcContainer->left,lpRcContainer->top);
			CSize sz  = CalcSize(lpRcContainer);
			DUIWindow *pSibling = m_pOwner->DM_GetWindow(GDW_PREVSIBLING);
			if (pSibling)
			{
				CRect rcSib;
				pSibling->DV_GetWindowRect(&rcSib);
				if (IsUnInitPos(rcSib.left)||IsUnInitPos(rcSib.top)||IsUnInitPos(rcSib.right)||IsUnInitPos(rcSib.bottom))
				{
					pt.x = lpRcContainer->left;
					pt.y = lpRcContainer->top;
				}
				else
				{
					pt.y = rcSib.bottom+m_nSepSpace;
				}
			}
			rcWindow = CRect(pt,sz);
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMLayoutImpl::IsUnInitPos(int iPos)
	{
		return POS_INIT == iPos || POS_WAIT == iPos;
	}

	CSize DMLayoutImpl::CalcSize(LPRECT pRcContainer)
	{
		CSize sz;
		if (m_uPositionType&SizeX_Specify)
		{
			sz.cx = m_size.cx;
		}
		else if(m_uPositionType&SizeX_FitParent)
		{
			sz.cx = pRcContainer->right-pRcContainer->left;
		}

		if (m_uPositionType&SizeY_Specify)
		{
			sz.cy = m_size.cy;
		}
		else if (m_uPositionType&SizeY_FitParent)
		{
			sz.cy = pRcContainer->bottom-pRcContainer->top;
		}

		if((m_uPositionType&SizeX_FitContent) || (m_uPositionType&SizeY_FitContent) && m_nCount!=4)
		{
			CSize szDesire;
			m_pOwner->DV_GetDesiredSize(pRcContainer,szDesire);    
			if (m_uPositionType&SizeX_FitContent)
			{
				sz.cx = szDesire.cx;
			}
			if (m_uPositionType&SizeY_FitContent)
			{
				sz.cy = szDesire.cy;
			}
		}
		return sz;
	}

	DMCode DMLayoutImpl::OnAttributePos(LPCWSTR pszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CStringW strOldPosValue = m_strPosValue;
			m_strPosValue = pszValue;
			if (!ParsePostion())
			{
				break;
			}
			if (!ParsePostionType())
			{
				break;
			}
			
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMLayoutImpl::OnAttributePosSize(LPCWSTR pszValue, bool bLoadXml)
	{
		dm_parsesize(pszValue,m_size);
	
		ParsePostion();
		ParsePostionType();

		return DM_ECODE_OK;
	}
}//namespace DM
