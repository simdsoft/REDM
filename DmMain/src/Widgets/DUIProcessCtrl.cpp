#include "DmMainAfx.h"
#include "DUIProcessCtrl.h"

namespace DM
{
	DUIProcessCtrl::DUIProcessCtrl()
	{
		m_szRange.cx	 = 0;
		m_szRange.cy	 = 100;
		m_iValue		 = 0;
		m_pSelSkin  	 = NULL;
		m_pChannelSkin   = NULL;
		m_crChannel.SetTextInvalid();
		m_crSel.SetTextInvalid();
		m_bVert          = false;	
		m_bShowTip       = false;
	}

	bool DUIProcessCtrl::SetPos(int iValue)
	{
		bool bRet = false;
		do 
		{
			if (iValue>m_szRange.cy)
			{
				iValue = m_szRange.cy;
			}

			if (iValue<m_szRange.cx)
			{
				iValue = m_szRange.cx;;
			}
			m_iValue = iValue;
			DM_Invalidate();
			bRet = true;
		} while (false);
		return bRet;
	}

	int DUIProcessCtrl::GetPos()
	{
		return m_iValue;
	}

	void DUIProcessCtrl::GetRange(int& nMin,int& nMax) const
	{
		nMin = m_szRange.cx;
		nMax = m_szRange.cy;
	}

	int DUIProcessCtrl::GetRangeMax() const
	{
		return m_szRange.cy;
	}

	int DUIProcessCtrl::GetRangeMin() const
	{
		return m_szRange.cx;
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUIProcessCtrl::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			byte alpha = 0xff;
			m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
			///1绘制channel
			if (m_pChannelSkin)
			{
				CRect rcChannel; 
				DV_GetClientRect(rcChannel);
				m_pChannelSkin->Draw(pCanvas, rcChannel, 0,alpha);
			}
			else if (!m_crChannel.IsTextInvalid())
			{
				CRect rcChannel; 
				DV_GetClientRect(rcChannel);
				pCanvas->FillSolidRect(rcChannel,m_crChannel);
			}

			//2绘制Sel
			if (m_pSelSkin)
			{
				CRect rcSel = GetSelRect();
				m_pSelSkin->Draw(pCanvas, rcSel, 0,alpha);
			}
			else if (!m_crSel.IsTextInvalid())
			{
				CRect rcSel = GetSelRect();
				pCanvas->FillSolidRect(rcSel,m_crSel);
			}

			//3绘制进度文字format
			__super::DM_OnPaint(pCanvas);
			if (!m_strTextFormat.IsEmpty())
			{
				DUIDrawEnviron DrawEnviron;
				DV_PushDrawEnviron(pCanvas, DrawEnviron);

				CStringW strValue;
				strValue.Format(DMTR(m_strTextFormat),m_iValue);
				UINT uAlign = 0;
				m_pDUIXmlInfo->m_pStyle->GetTextAlign(uAlign);
				CRect rcText;
				DV_GetClientRect(rcText);// 以客户区为空间
				DV_DrawText(pCanvas, strValue, strValue.GetLength(),rcText, uAlign);
				DV_PopDrawEnviron(pCanvas, DrawEnviron);
			}

		} while (false);
	}

	DMCode DUIProcessCtrl::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!m_bShowTip)
			{
				break;
			}

			tipInfo.hDUIWnd  = m_hDUIWnd;
			tipInfo.rcTarget = m_rcWindow;
			CStringW strTrans = DMTR(m_pDUIXmlInfo->m_strTooltipText);
			strTrans.Format(L"%d",m_iValue);
			tipInfo.strTip    = strTrans;
			tipInfo.iDelayTime =  50;
			tipInfo.iSpanTime  =  5000;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	// 辅助
	CRect DUIProcessCtrl::GetSelRect()
	{
		CRect rcSel;
		do 
		{
			if (m_szRange.cx>=m_szRange.cy)
			{
				DMASSERT_EXPR(0,L"slidectrl max<=min!");
				break;
			}

			if (m_iValue<m_szRange.cx||m_iValue>m_szRange.cy)
			{
				DMASSERT_EXPR(0,L"slidectrl value越界了");
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			int iRailLen = 0;
			if (m_bVert)
			{
				iRailLen = rcClient.Height();
			}
			else
			{
				iRailLen = rcClient.Width();
			}
			int iSel = iRailLen*(m_iValue-m_szRange.cx)/(m_szRange.cy-m_szRange.cx);
			rcSel  = rcClient;
			if (m_bVert)
			{
				rcSel.bottom = rcClient.top+iSel;
			}
			else
			{
				rcSel.right = rcClient.left+iSel;
			}
		} while (false);
		return rcSel;
	}

	int DUIProcessCtrl::GetValue(CPoint &pt)
	{
		CRect rcChannel;
		DV_GetClientRect(rcChannel);
		int iValue = 0;
		if (m_bVert)
		{
			iValue = (pt.y-rcChannel.top)*(m_szRange.cy-m_szRange.cx)/(rcChannel.Height());
		}
		else
		{	
			iValue = (pt.x-rcChannel.left)*(m_szRange.cy-m_szRange.cx)/(rcChannel.Width());
		}
		return iValue+m_szRange.cx;
	}

	///
	DMCode DUIProcessCtrl::OnAttributeRange(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CSize szRange;
			dm_parsesize(lpszValue,szRange);
			if (szRange.cx>=szRange.cy)
			{
				DMASSERT_EXPR(0,L"slider范围设置错误");
				break;
			}
			m_szRange = szRange;
			if (!bLoadXml)
			{
				if (m_iValue<m_szRange.cx)
				{
					m_iValue = m_szRange.cx;
				}
				if (m_iValue>m_szRange.cy)
				{
					m_iValue = m_szRange.cy;
				}
				DM_Invalidate();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIProcessCtrl::OnAttributeValue(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			dm_parseint(lpszValue,m_iValue);
			if (!bLoadXml)
			{
				if (m_iValue<m_szRange.cx)
				{
					m_iValue = m_szRange.cx;
				}
				if (m_iValue>m_szRange.cy)
				{
					m_iValue = m_szRange.cy;
				}
				DM_Invalidate();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}
}// namespace DM