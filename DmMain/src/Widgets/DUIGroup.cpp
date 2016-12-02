#include "DmMainAfx.h"
#include "DUIGroup.h"

namespace DM
{
	DUIGroup::DUIGroup()
	{
		m_rcInset.SetRect(10,20,10,10);
		m_iRound       = 4;
		m_iLineWid     = 1;
		m_crLine       = PBGRA(0xe2,0xb2,0x3e,0xff);
	}

	void DUIGroup::DM_OnPaint(IDMCanvas* pCanvas)
	{
		DUIDrawEnviron DrawEnviron;
		DV_PushDrawEnviron(pCanvas, DrawEnviron);

		do 
		{
			CRect rcGroup = m_rcWindow;
			CSize szFont;
			CRect rcText  = m_rcWindow;
			rcText.left  += 20; /// ×óÓÒÁôµã¿Õ
			rcText.right -= 20;
			if (!m_pDUIXmlInfo->m_strText.IsEmpty())
			{
				pCanvas->MeasureText(DMTR(m_pDUIXmlInfo->m_strText),-1,&szFont);
				rcText.bottom = rcText.top+szFont.cy+2;
				UINT ulAlgin = 0;
				m_pDUIXmlInfo->m_pStyle->GetTextAlign(ulAlgin);
				if (ulAlgin&DT_CENTER)
				{
					rcText.left  += (rcText.Width()-szFont.cx)/2;
					rcText.right = rcText.left + szFont.cx;
				}
				else if (ulAlgin&DT_RIGHT)
				{
					rcText.left = rcText.right-szFont.cx;
				}
				else
				{
					rcText.right = rcText.left+szFont.cx;
				}
				CRect rcClip = rcText;
				rcClip.InflateRect(5,5,5,5);
				pCanvas->PushClip(rcClip,RGN_DIFF);// ÅÅ³ýtextÇø
			}

			if (!m_pDUIXmlInfo->m_strText.IsEmpty())
			{
				rcGroup.top += szFont.cy/2;
			}
			rcGroup.DeflateRect(1,1,1,0);

			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			DMSmartPtrT<IDMPen> pPen;
			DMSmartPtrT<IDMPen> pOldPen;
			pRender->CreatePen(m_crLine,PS_SOLID,m_iLineWid,&pPen);
			if (NULL == pPen)
			{
				break; 
			}
			pCanvas->SelectObject(pPen,(IDMMetaFile**)&pOldPen);
			pCanvas->DrawRoundRect(rcGroup,CPoint(m_iRound,m_iRound));
			pCanvas->SelectObject(pOldPen);
			
			if (!m_pDUIXmlInfo->m_strText.IsEmpty())
			{
				pCanvas->PopClip();
				pCanvas->DrawText(DMTR(m_pDUIXmlInfo->m_strText),-1,rcText, DT_SINGLELINE|DT_VCENTER);
			}

		} while (false);
		DV_PopDrawEnviron(pCanvas, DrawEnviron);
	}

	DMCode DUIGroup::DV_GetChildMeasureLayout(LPRECT lpRect)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}
			CRect rcLayout;
			__super::DV_GetChildMeasureLayout(rcLayout);
			rcLayout.DeflateRect(m_rcInset);
			*lpRect = rcLayout;
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}
}//namespace DM