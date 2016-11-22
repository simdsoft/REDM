#include "DmMainAfx.h"
#include "DUISliderCtrl.h"

namespace DM
{
	DUISliderCtrl::DUISliderCtrl()
	{
		m_szRange.cx	 = 0;
		m_szRange.cy	 = 100;
		m_iValue		 = 0;
		m_pThumbSkin	 = NULL;
		m_pChannelSkin   = NULL;
		m_iThumbWid      = 10;
		m_iChannelHei    = 4;
		m_bVert          = false;	
		m_bShowTip       = false;

		m_bDrag          = false;
		m_iDragValue     = 0;
		m_iHitPre        = -1;

		m_crThumb.SetTextInvalid();
		m_crChannel.SetTextInvalid();

		m_pDUIXmlInfo->m_bFocusable = true;

		DMADDEVENT(DMEventSDChangingArgs::EventID);
		DMADDEVENT(DMEventSDChangedArgs::EventID);
	}

	bool DUISliderCtrl::SetPos(int iValue)
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

			if (iValue == m_iValue)
			{
				break;
			}
			
			DMEventSDChangingArgs EvtChanging(this);
			EvtChanging.m_nOldPos = m_iValue;
			EvtChanging.m_nNewPos = iValue;
			DV_FireEvent(EvtChanging);
			if (EvtChanging.m_bCacel)
			{
				break;
			}

			int iValueOld = m_iValue;
			m_iValue = iValue;
			DMEventSDChangedArgs Evt(this);
			Evt.m_nOldPos = iValueOld;
			Evt.m_nNewPos = m_iValue;
			DV_FireEvent(Evt);
			DM_Invalidate();
			bRet = true;
		} while (false);
		return bRet;
	}

	int DUISliderCtrl::GetPos()
	{
		return m_iValue;
	}

	void DUISliderCtrl::GetRange(int& nMin,int& nMax) const
	{
		nMin = m_szRange.cx;
		nMax = m_szRange.cy;
	}

	int DUISliderCtrl::GetRangeMax() const
	{
		return m_szRange.cy;
	}

	int DUISliderCtrl::GetRangeMin() const
	{
		return m_szRange.cx;
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	void DUISliderCtrl::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			__super::DM_OnPaint(pCanvas);

			byte alpha = 0xff;
			m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
			///1绘制channel
			if (m_pChannelSkin)
			{
				CRect rcChannel = GetChannelRect();
				m_pChannelSkin->Draw(pCanvas, rcChannel, 0,alpha);
			}
			else if (!m_crChannel.IsTextInvalid())
			{
				CRect rcChannel = GetChannelRect();
				pCanvas->FillSolidRect(rcChannel,m_crChannel);
			}

			//2绘制thumb
			if (m_pThumbSkin)
			{
				CRect rcThumb = GetThumbRect();
				int iState = 0;
				if (m_bDrag)
				{
					iState = 2;
				}
				else if (DMSLIDER_THUMB==m_iHitPre)
				{
					iState = 1;
				}
				m_pThumbSkin->Draw(pCanvas, rcThumb, iState,alpha);
			}
			else if (!m_crThumb.IsTextInvalid())
			{
				CRect rcThumb = GetThumbRect();
				pCanvas->FillSolidRect(rcThumb,m_crThumb);
			}

		} while (false);
	}

	void DUISliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
	{
		do 
		{
			DM_SetCapture();
			int iHit = HitTest(point);
			if (DMSLIDER_THUMB == iHit)
			{
				m_bDrag      = true;
				m_DragPt     = point;
				m_iDragValue = m_iValue;
				DM_Invalidate();
			}
			else if(DMSLIDER_CHANNEL == iHit)// 设置
			{
				CRect rcChannel = GetChannelRect();
				int iValue = GetValue(point);
				SetPos(iValue);
				DM_Invalidate();

				m_bDrag      = true;
				m_DragPt     = point;
				m_iDragValue = m_iValue;
			}
			if (m_pDUIXmlInfo->m_bFocusable)
			{
				DV_SetFocusWnd();
			}
		} while (false);
	}

	void DUISliderCtrl::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		DM_ReleaseCapture();
		if (m_bDrag)
		{
			m_bDrag = false;
			CRect rcThumb = GetThumbRect();
			DM_InvalidateRect(rcThumb);
		}
		OnMouseMove(nFlags,pt);
	}

	void DUISliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
	{
		do 
		{
			if (m_bDrag)
			{
				CRect rcChannel = GetChannelRect();
				int iDragLen = m_bVert?point.y-m_DragPt.y:point.x-m_DragPt.x;
				int iLen =m_bVert?rcChannel.Height():rcChannel.Width();
				int iSlide = iDragLen*(m_szRange.cy-m_szRange.cx+1)/iLen;
				int iNewTrackPos = m_iDragValue + iSlide;
				if (iNewTrackPos<m_szRange.cx)
				{
					iNewTrackPos = m_szRange.cx;
				}
				else if (iNewTrackPos>m_szRange.cy)
				{
					iNewTrackPos = m_szRange.cy;
				}
				if (iNewTrackPos!= m_iValue)
				{
					SetPos(iNewTrackPos);
				}
			}
			else
			{
				int iHit = HitTest(point);
				if (iHit!=m_iHitPre &&(DMSLIDER_THUMB==m_iHitPre||DMSLIDER_THUMB==iHit))
				{
					m_iHitPre = iHit;
					CRect rcThumb = GetThumbRect();
					DM_InvalidateRect(rcThumb);
				}
			}

		} while (false);
	}

	void DUISliderCtrl::OnMouseLeave()
	{
		if (!m_bDrag && DMSLIDER_THUMB == m_iHitPre)
		{
			CRect rcThumb = GetThumbRect();
			DM_InvalidateRect(rcThumb);
			m_iHitPre = -1;
		}
	}

	void DUISliderCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		switch (nChar)
		{
		case VK_LEFT:
		case VK_UP:
			{
				SetPos(m_iValue-1);
			}
			break;

		case VK_RIGHT:
		case VK_DOWN:
			{
				SetPos(m_iValue+1);
			}
			break;
		case VK_HOME:
			{
				SetPos(m_szRange.cx);
			}
			break;
		case VK_END:
			{
				SetPos(m_szRange.cy);
			}
			break;
		default:
			break;
		}
	}

	DMCode DUISliderCtrl::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!m_bShowTip
				||m_bDrag)
			{
				break;
			}
			if (DMSLIDER_THUMB == HitTest(pt))
			{
				CRect rcThumb = GetThumbRect();
				tipInfo.hDUIWnd  = m_hDUIWnd;
				tipInfo.rcTarget = rcThumb;
				
				if (m_strTipFormat!=L"")
				{
					CStringW strTrans = DMTR(m_strTipFormat);
					m_pDUIXmlInfo->m_strTooltipText.Format(strTrans,m_iValue);
				}
				else
				{
					m_pDUIXmlInfo->m_strTooltipText.Format(L"%d",m_iValue);
				}
				tipInfo.strTip    = m_pDUIXmlInfo->m_strTooltipText;
				tipInfo.iDelayTime =  50;
				tipInfo.iSpanTime  =  5000;
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	// 辅助
	CRect DUISliderCtrl::GetChannelRect()
	{
		CRect rcClient;
		DV_GetClientRect(rcClient);
		CRect rcChannel = rcClient;
		int iHeiLen = m_bVert?rcClient.Width():rcClient.Height();
		if (m_iChannelHei>iHeiLen)
		{
			m_iChannelHei = iHeiLen;
		}
		if (m_bVert)
		{
			rcChannel.left +=(iHeiLen-m_iChannelHei)/2;
			rcChannel.right = rcChannel.left+m_iChannelHei;
		}
		else
		{
			rcChannel.top +=(rcClient.Height()-m_iChannelHei)/2;
			rcChannel.bottom = rcChannel.top+m_iChannelHei;
		}
	
		return rcChannel;
	}

	CRect DUISliderCtrl::GetThumbRect()
	{
		CRect rcThumb;
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
			if (m_iThumbWid<=0)
			{
				break;
			}
			
			CRect rcClient;
			DV_GetClientRect(rcClient);
			int iRailLen = 0;
			if (m_bVert)
			{
				iRailLen = rcClient.Height()-m_iThumbWid;
			}
			else
			{
				iRailLen = rcClient.Width()-m_iThumbWid;
			}
			int iSel = iRailLen*(m_iValue-m_szRange.cx)/(m_szRange.cy-m_szRange.cx);
			rcThumb  = rcClient;
			if (m_bVert)
			{
				rcThumb.top = rcClient.top+iSel;
				rcThumb.bottom = rcThumb.top+m_iThumbWid;
			}
			else
			{
				rcThumb.left = rcClient.left+iSel;
				rcThumb.right = rcThumb.left+m_iThumbWid;
			}
		} while (false);
		return rcThumb;
	}

	int DUISliderCtrl::HitTest(CPoint &pt)
	{
		int iRet = DMSLIDER_OTHER;
		do 
		{
			CRect rcThumb = GetThumbRect();
			if (rcThumb.PtInRect(pt))
			{
				iRet = DMSLIDER_THUMB;
				break;
			}

			CRect rcChannel = GetChannelRect();
			if (rcChannel.PtInRect(pt))
			{
				iRet = DMSLIDER_CHANNEL;
				break;
			}

		} while (false);
		return iRet;
	}

	int DUISliderCtrl::GetValue(CPoint &pt)
	{
		CRect rcChannel = GetChannelRect();
		int iValue = 0;
		if (m_bVert)
		{
			iValue = (pt.y-rcChannel.top)*(m_szRange.cy-m_szRange.cx)/(rcChannel.Height()-m_iThumbWid);
		}
		else
		{	
			iValue = (pt.x-rcChannel.left)*(m_szRange.cy-m_szRange.cx)/(rcChannel.Width()-m_iThumbWid);
		}
		return iValue+m_szRange.cx;
	}

	///
	DMCode DUISliderCtrl::OnAttributeRange(LPCWSTR lpszValue, bool bLoadXml)
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

	DMCode DUISliderCtrl::OnAttributeValue(LPCWSTR lpszValue, bool bLoadXml)
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