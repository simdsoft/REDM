#include "DmMainAfx.h"
#include "DUILabel.h"

namespace DM
{
	DUILabel::DUILabel()
	{
		m_bMultiLines = false;
		m_nLineInter  = 5;
		m_pDUIXmlInfo->m_bMsgNoHandle = true;
	}

	DMCode DUILabel::DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat)
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

}//namespace DM