#include "DmMainAfx.h"
#include "DUIButton.h"

namespace DM
{
	DUIButton::DUIButton()
	{
		m_bAnimate       = false;
		m_iAnimateSteps  = 20;
		m_iAnimateDelta  = 3;
		m_byCurAlpha     = 0xff;
		m_dwPreFrameTime = 0;
		m_bMultiLines    = false;
		m_nLineInter     = 5;
		m_pIconSkin      = NULL;
		m_accel          = 0;
	}

	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	BOOL DUIButton::DM_OnEraseBkgnd(IDMCanvas* pCanvas)
	{
		BOOL bRet = 0;
		do 
		{
			if (!m_bAnimate
				||0xff == m_byCurAlpha///< 0xff时表示没有启用动画或动画结束了
				)
			{
				bRet = __super::DM_OnEraseBkgnd(pCanvas);
				break;
			}

			// 设置新的alpha
			byte byAlphaBk = 0xff;
			m_pDUIXmlInfo->m_pStyle->GetAlpha(byAlphaBk);
			CStringW strValue;
			byte byCurAlpha = m_byCurAlpha*byAlphaBk/255;
			strValue.Format(L"%d",byCurAlpha);
			m_pDUIXmlInfo->m_pStyle->SetAttribute(L"alpha",strValue,true);

			bRet = __super::DM_OnEraseBkgnd(pCanvas);

			// 恢复原有alpha
			strValue.Format(L"%d",byAlphaBk);
			m_pDUIXmlInfo->m_pStyle->SetAttribute(L"alpha",strValue,true);
		} while (false);
		return bRet;
	}

	void DUIButton::DM_OnPaint(IDMCanvas* pCanvas)
	{
		byte alpha = 0xff;
		do 
		{
			m_pDUIXmlInfo->m_pStyle->GetAlpha(alpha);
			if (!m_bAnimate
				||0xff == m_byCurAlpha///< 0xff时表示没有启用动画或动画结束了
				)
			{
				__super::DM_OnPaint(pCanvas);
				break;
			}
			// 设置新的alpha
			byte byAlphaBk = alpha;
			CStringW strValue;
			alpha = m_byCurAlpha*byAlphaBk/255;
			strValue.Format(L"%d",alpha);
			m_pDUIXmlInfo->m_pStyle->SetAttribute(L"alpha",strValue,true);

			__super::DM_OnPaint(pCanvas);

			// 恢复原有alpha
			strValue.Format(L"%d",byAlphaBk);
			m_pDUIXmlInfo->m_pStyle->SetAttribute(L"alpha",strValue,true);

			if (GetTickCount()-m_dwPreFrameTime>10)
			{
				m_dwPreFrameTime = GetTickCount();// 调整时间
				if (255-m_byCurAlpha<m_iAnimateDelta)
				{
					m_byCurAlpha = 0xff;
				}
				else
				{
					m_byCurAlpha += m_iAnimateDelta;
				}
			}

		} while (false);

		if (m_pIconSkin)
		{
			CRect rcClient;
			DV_GetClientRect(rcClient);
			CRect rcIcon = rcClient;
			CSize sz;
			m_pIconSkin->GetStateSize(sz);
			MeetRect(rcIcon, sz);
			m_pIconSkin->Draw(pCanvas, rcIcon,0,alpha);
		}
	}

	void DUIButton::DM_OnNcPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			if (!m_bAnimate
				||0xff == m_byCurAlpha///< 0xff时表示没有启用动画或动画结束了
				)
			{
				__super::DM_OnNcPaint(pCanvas);
				break;
			}

			// 设置新的alpha
			byte byAlphaBk = 0xff;
			m_pDUIXmlInfo->m_pStyle->GetAlpha(byAlphaBk);
			CStringW strValue;
			byte byCurAlpha = m_byCurAlpha*byAlphaBk/255;
			strValue.Format(L"%d",byCurAlpha);
			m_pDUIXmlInfo->m_pStyle->SetAttribute(L"alpha",strValue,true);

			__super::DM_OnNcPaint(pCanvas);

			// 恢复原有alpha
			strValue.Format(L"%d",byAlphaBk);
			m_pDUIXmlInfo->m_pStyle->SetAttribute(L"alpha",strValue,true);
		} while (false);	
	}

	void DUIButton::OnDestroy()
	{
		StopCurAnimate();
		if (m_accel)
		{
			DUIAccel acc(m_accel);
			GetContainer()->GetAccelMgr()->UnregisterAccel(acc,this);
		}
		__super::OnDestroy();
	}

	// 重载
	DMCode DUIButton::DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat)
	{
		do //hgy: 为什么XML中\R\N不能被直接识别，因为XML文件中的"/n",会被认为是一个字符串"///n"是两个字符'//'和'/n'，而不是转义字符"/n",
		{
			if (NULL != m_pIconSkin)// icon下不绘制文字
			{
				break;
			}

			if (!m_bMultiLines)
			{
				__super::DV_DrawText(pCanvas, pszBuf, cchText, pRect, uFormat);
				break;
			}
			DV_DrawMultText(pCanvas,pszBuf,cchText,pRect,uFormat,m_nLineInter);
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUIButton::DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState)
	{
		StopCurAnimate();
		if (DM_GetCapture()==m_hDUIWnd)
		{
			return DM_ECODE_OK;
		}
		if (m_bAnimate && dwOldState==DUIWNDSTATE_Normal && dwNewState==DUIWNDSTATE_Hover)
		{// 启动动画
			m_byCurAlpha = 255/55;// 从1/5开始
			if (m_iAnimateSteps)
			{
				m_iAnimateDelta = (255-m_byCurAlpha)/m_iAnimateSteps;
			}
			m_dwPreFrameTime = 0;
			GetContainer()->OnRegisterTimeline(this);
		}
		return DM_ECODE_OK;
	}

	DMCode DUIButton::OnTimeline()
	{
		do 
		{
			if (0xff == m_byCurAlpha) 
			{
				StopCurAnimate();
				DM_InvalidateRect(m_rcWindow);
			}

			if (GetTickCount()-m_dwPreFrameTime<10)
			{
				break;
			}
			if (255-m_byCurAlpha<m_iAnimateDelta)
			{
				m_byCurAlpha = 0xff;
			}
			else
			{
				m_byCurAlpha += m_iAnimateDelta;
			}
			DM_Invalidate();
		} while (false);
		return DM_ECODE_OK;
	}

	bool DUIButton::DV_IsStateChangeRedraw()
	{
		bool bRet = false;
		do 
		{
			DMSmartPtrT<IDMSkin> pSkin;
			DMSmartPtrT<IDMSkin> pNcSkin;
			m_pDUIXmlInfo->m_pStyle->GetBgSkin(&pSkin);
			m_pDUIXmlInfo->m_pStyle->GetNcSkin(&pNcSkin);
			int iStates  = 0;
			int iNcSates = 0;
			if (pSkin)
			{
				pSkin->GetStates(iStates);
			}
			if (pNcSkin)
			{
				pNcSkin->GetStates(iNcSates);
			}
			if (iStates>1||iNcSates>1)
			{
				bRet = true;
			}
		} while (false);

		if (bRet)
		{
			return true;
		}
		return __super::DV_IsStateChangeRedraw();
	}

	bool DUIButton::OnAccelPressed(const DUIAccel& Accel)
	{
		if (DM_IsDisable(true)||!DM_IsVisible(true))
		{
			return false;
		}
		DMEventCmdArgs Evt(this);
		DV_FireEvent(Evt);
		return true;
	}


	//---------------------------------------------------
	// Function Des: 换肤
	DMCode DUIButton::DV_UpdateSkin(WPARAM wp, LPARAM lp)
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
			}

			// todo.其余换肤
		} while (false);
		return iErr;
	}

	void DUIButton::StopCurAnimate()
	{
		GetContainer()->OnUnregisterTimeline(this);
		m_byCurAlpha = 0xff;
	}

	DMCode DUIButton::OnAttributeAccel(LPCWSTR pszValue, bool bLoadXml)
	{
		 CStringW strValue = pszValue;
		 m_accel = DUIAccel::TranslateAccelKey(strValue);
		 if (m_accel)
		 {
			 DUIAccel acc(m_accel);
			 GetContainer()->GetAccelMgr()->RegisterAccel(acc,this);
		 }
		 return DM_ECODE_OK;
	}

}// namespace DM