#include "DmMainAfx.h"
#include "DUICheckBox.h"

namespace DM
{
	DUICheckBox::DUICheckBox()
	{
		m_bRightBox   = false;
		m_bMultiLines = false;
		m_nLineInter  = 5;
		m_nTextOffset = 0;
		m_pCheckSkin  = NULL;
		m_pIconSkin   = NULL;
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUICheckBox::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			CRect rcCheck = GetCheckRect();
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

			// 1. 绘制box图标
			byte alpha = 0xff; 
			m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
			if (NULL!=m_pCheckSkin)
			{
				int iStates = 0;
				m_pCheckSkin->GetStates(iStates);
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
				m_pCheckSkin->Draw(pCanvas, rcCheck,iStates,alpha);
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

	void DUICheckBox::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		DM_SetCheck(!DM_IsChecked());
		__super::OnLButtonUp(nFlags, pt);
	}

	DMCode DUICheckBox::DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat)
	{
		do //hgy: 为什么XML中\R\N不能被直接识别，因为XML文件中的"/n",会被认为是一个字符串"///n"是两个字符'//'和'/n'，而不是转义字符"/n",
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

	//---------------------------------------------------
	// Function Des: 换肤
	DMCode DUICheckBox::DV_UpdateSkin(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (lp == DMREG_Skin)// skin换肤
			{
				DMSmartPtrT<IDMSkin> pSkin;
				DMSmartPtrT<IDMSkin> pNcSkin;
				m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
				m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pNcSkin);
				// 确认skin是否在要更新的列表中
				if (g_pDMApp->IsNeedUpdateSkin(pSkin)
					||g_pDMApp->IsNeedUpdateSkin(pNcSkin))
				{
					iErr = DM_ECODE_OK;
					break;
				}
				if (m_pIconSkin&&g_pDMApp->IsNeedUpdateSkin(m_pIconSkin))
				{
					iErr = DM_ECODE_OK;
					break;
				}
				if (m_pCheckSkin&&g_pDMApp->IsNeedUpdateSkin(m_pCheckSkin))
				{
					iErr = DM_ECODE_OK;
					break;
				}
			}

			// todo.其余换肤
		} while (false);
		return iErr;
	}

	int DUICheckBox::GetDrawState()
	{
		int iState = 0;//8分图,前4为未选中四种状态，后4为选中时四种状态
		do 
		{
			if (NULL==m_pCheckSkin)
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

	CRect DUICheckBox::GetCheckRect()
	{
		CRect rcDest;
		do 
		{
			if (NULL==m_pCheckSkin)
			{
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CSize sz;
			m_pCheckSkin->GetStateSize(sz);
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

	DMCode DUICheckBox::OnAttributeChecked(LPCWSTR lpszValue, bool bLoadXml)
	{
		bool bCheck = false;
		dm_parsebool(lpszValue,bCheck);
		//DM_SetCheck(bCheck);
		if (bCheck)
		{
			DM_ModifyState(DUIWNDSTATE_Check, 0, true);
		}
		else
		{
			DM_ModifyState(0, DUIWNDSTATE_Check, true);
		}
		return DM_ECODE_NOXMLLOADREFRESH;
	}
}//namespace DM