#include "DmMainAfx.h"
#include "DUIRadioButton.h"

namespace DM
{
	DUIRadioButton::DUIRadioButton()
	{
		m_bAutoGroup  = true;
		m_bRightBox   = false;
		m_bMultiLines = false;
		m_nLineInter  = 5;
		m_nTextOffset = 0;
		m_pRadioSkin  = NULL;
		m_pIconSkin   = NULL;
		m_pDUIXmlInfo->m_bFocusable = true;

	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUIRadioButton::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			CRect rcCheck = GetRadioRect();
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CRect rcText = rcClient;
			if (m_bRightBox&&!rcCheck.IsRectEmpty())
			{
				rcText.right = rcCheck.left-m_nTextOffset;
			}
			else if(!rcCheck.IsRectEmpty())
			{
				rcText.left = rcCheck.right+m_nTextOffset;
			}

			// 1. 绘制radio图标
			byte alpha = 0xff;
			m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
			if (NULL!=m_pRadioSkin)
			{
				int iStates = 0;
				m_pRadioSkin->GetStates(iStates);
				if (1 == iStates)// 1分图
				{
				}
				else if (2 == iStates)// 2分图
				{
					iStates = GetDrawState()/4;//[0-3][4-7]
				}
				else 
				{
					iStates = GetDrawState();
				}
				m_pRadioSkin->Draw(pCanvas, rcCheck,iStates,alpha);
			}

			// 2.绘制icon
			if (m_pIconSkin)
			{
				CRect rcIcon = rcText;
				CSize sz;
				m_pIconSkin->GetStateSize(sz);
				MeetRect(rcIcon, sz);
				m_pIconSkin->Draw(pCanvas, rcIcon,0,alpha);
				break;// 有ICON就不绘text了 
			}

			// 2. 绘制文字
			DUIDrawEnviron DrawEnviron;
			DV_PushDrawEnviron(pCanvas, DrawEnviron);

			UINT uAlign = 0;
			m_pDUIXmlInfo->m_pStyle->GetTextAlign(uAlign);
			CStringW strTrans = DMTR(m_pDUIXmlInfo->m_strText);
			DV_DrawText(pCanvas, strTrans, strTrans.GetLength(),rcText, uAlign);

			DV_PopDrawEnviron(pCanvas, DrawEnviron);
		} while (false);

		if (m_pDUIXmlInfo->m_bShowDrawFocus)
		{
			if (DM_IsFocusWnd())
			{
				// 绘制焦点框
				DV_DrawDefFocus(pCanvas);
			}
		}
	}

	LRESULT DUIRadioButton::DM_OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (DUIFocusMgr::FocusReasonByRestore != lParam)
		{
			DM_SetCheck(true);
		}
		return true;
	}

	void DUIRadioButton::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		__super::OnLButtonUp(nFlags,pt);
		if (!DM_IsChecked())
		{
			DM_SetCheck(true);
		}
	}

	//---------------------------------------------------
	// Function Des: 重载DUIWindow
	DMCode DUIRadioButton::DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat)
	{
		do 
		{
			if (!m_bMultiLines)
			{
				__super::DV_DrawText(pCanvas, pszBuf, cchText, pRect, uFormat);
				break;
			}

			DV_DrawMultText(pCanvas,pszBuf,cchText,pRect,uFormat,m_nLineInter);
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUIRadioButton::DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState)
	{
		do 
		{
			if (dwNewState & DUIWNDSTATE_Check)
			{
				if (!(dwOldState & DUIWNDSTATE_Check))
				{
					DUIWindow* pWnd = DV_GetSelSiblingInGroup();
					if (NULL == pWnd||0 != _wcsicmp(pWnd->V_GetClassName(),DUIRadioButton::GetClassName()))
					{
						break;
					}
					pWnd->DM_SetCheck(false);
					if (0!=m_pDUIXmlInfo->m_iID
						||!m_pDUIXmlInfo->m_strName.IsEmpty())
					{
						DMEventCmdArgs Evt(this);
						DV_FireEvent(Evt);
					}
				}
			}

		} while (false);
		return DM_ECODE_OK;
	}

	DUIWindow* DUIRadioButton::DV_GetSelSiblingInGroup()
	{
		DUIWindow* pWnd = NULL;
		do 
		{
			DUIWindow *pParent = DM_GetWindow(GDW_PARENT);
			if (!pParent)
			{
				break;
			}
			DUIWindow *pSibling = pParent->DM_GetWindow(GDW_FIRSTCHILD);
			while (pSibling)
			{
				if (0 == _wcsicmp(pSibling->V_GetClassName(),DUIRadioButton::GetClassName()))
				{
					if (pSibling->DM_IsChecked())
					{
						pWnd = pSibling;
						break;
					}
				}
				pSibling = pSibling->DM_GetWindow(GDW_NEXTSIBLING);
			}
		} while (false);
		return pWnd;
	}

	// 辅助
	int DUIRadioButton::GetDrawState()
	{
		int iState = 0;//8分图,前4为未选中四种状态，后4为选中时四种状态
		do 
		{
			if (NULL==m_pRadioSkin)
			{
				break;
			}
			if (m_dwDUIState&DUIWNDSTATE_Disable)
			{
				iState = 3;
			}
			else if (m_dwDUIState&DUIWNDSTATE_PushDown)
			{
				iState = 2;
			}
			else if (m_dwDUIState&DUIWNDSTATE_Hover)
			{
				iState = 1;
			}
			else
			{
				iState = 0;
			}
			if (m_dwDUIState&DUIWNDSTATE_Check)
			{
				iState += 4;
			}
		} while (false);
		return iState;
	}

	CRect DUIRadioButton::GetRadioRect()
	{
		CRect rcDest;
		do 
		{
			if (NULL==m_pRadioSkin)
			{
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CSize sz;
			m_pRadioSkin->GetStateSize(sz);
			rcDest.SetRect(rcClient.TopLeft(),CPoint(sz.cx+rcClient.left,sz.cy+rcClient.top));
			rcDest.OffsetRect(0,(rcClient.Height()-sz.cy)/2);
			if (m_bRightBox)
			{
				rcDest.OffsetRect(rcClient.Width()-sz.cx,0);
			}
			// 调整size
			sz.cx = sz.cx<rcClient.Width()?sz.cx:rcClient.Width();
			sz.cy = sz.cy<rcClient.Height()?sz.cy:rcClient.Height();
			MeetRect(rcDest,sz);
		} while (false);
		return rcDest;
	}


}//namespace DM
