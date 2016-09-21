#include "DmMainAfx.h"
#include "DUILink.h"

namespace DM
{
	DMCode DUILink::DV_OnSetCursor(const CPoint &pt)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (!m_rcText.IsRectEmpty()// 字体为空时，就直接设置吧，只有图标了
				&&!m_rcText.PtInRect(pt))
			{
				break;
			}

			iErr = __super::DV_OnSetCursor(pt);
		} while (false);
		return iErr;
	}

	DMCode DUILink::DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat)
	{
		CalcTextRect(pCanvas,pszBuf, cchText, lpRect, uFormat);
		return __super::DV_DrawText(pCanvas,pszBuf, cchText,lpRect,uFormat);
	}

	void DUILink::OnLButtonDown(UINT nFlags,CPoint pt)
	{
		if (!m_rcText.IsRectEmpty()// 字体为空时，就直接设置吧，只有图标了
			&&!m_rcText.PtInRect(pt)) 
		{
			return;
		}
		__super::OnLButtonDown(nFlags,pt);
	}

	void DUILink::OnLButtonUp(UINT nFlags,CPoint pt)
	{
		if(!m_rcText.IsRectEmpty()// 字体为空时，就直接设置吧，只有图标了
			&&!m_rcText.PtInRect(pt))
		{
			DM_ReleaseCapture();
			return;
		}
		__super::OnLButtonUp(nFlags,pt);
		if (!m_strLinkUrl.IsEmpty())
		{
			//先使用默认浏览器打开，若打开失败（不存在默认浏览器），则强制使用IE打开
			HINSTANCE hIns = ::ShellExecute(NULL, _T("open"), m_strLinkUrl, NULL, NULL, SW_SHOWNORMAL);
			if ( int(hIns) <= 32 )	//If the function succeeds, it returns a value greater than 32.
			{
				ShellExecute(NULL, _T("open"), _T("iexplore.exe"), m_strLinkUrl, NULL, SW_SHOW);
			}

		}
	}

	void DUILink::OnMouseMove(UINT nFlags,CPoint pt)
	{
		if (!m_rcText.IsRectEmpty()// 字体为空时，就直接设置吧，只有图标了
			&&!m_rcText.PtInRect(pt))
		{
			if (m_dwDUIState&DUIWNDSTATE_Hover)
			{
				OnMouseLeave();
			}

		}
		else
		{
			if (!(m_dwDUIState&DUIWNDSTATE_Hover))
			{
				OnMouseHover(nFlags, pt);
			}
		}
	}

	void DUILink::OnMouseHover(WPARAM wParam, CPoint pt)
	{
		if (!m_rcText.IsRectEmpty()// 字体为空时，就不管了吧，只有图标了
			&&!m_rcText.PtInRect(pt))
		{
			return;
		}
		__super::OnMouseHover(wParam, pt);
	}


	// 辅助
	void DUILink::CalcTextRect(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT pRect,UINT uFormat)
	{
		if(!(uFormat&DT_CALCRECT))
		{
			CRect rcText; //1. 计算绘制文字所占大小
			pCanvas->DrawText(pszBuf,cchText,&rcText,DT_LEFT|DT_CALCRECT);

			UINT uAlign = 0;
			m_pDUIXmlInfo->m_pStyle->GetTextAlign(uAlign);
			if (uAlign&DT_CENTER)
			{// 2.1如果水平居中，则调整文字rc的左右
				m_rcText.left  = pRect->left + (pRect->right-pRect->left - rcText.Width())/2;
				m_rcText.right = m_rcText.left + rcText.Width();
			}
			else if (uAlign&DT_RIGHT)
			{// 2.2 如果居右，则调整为右对齐
				m_rcText.left  = pRect->right - rcText.Width();
				m_rcText.right = pRect->right;
			}
			else
			{// 2.3调整为左对齐
				m_rcText.left  = pRect->left;
				m_rcText.right = pRect->left + rcText.Width();
			}

			if(uAlign&DT_VCENTER)
			{// 3.1调整上下
				m_rcText.top    = pRect->top+ (pRect->bottom-pRect->top-rcText.Height())/2;
				m_rcText.bottom = m_rcText.top+rcText.Height();
			}
			else if(uAlign&DT_BOTTOM)
			{// 3.2调整为下对齐
				m_rcText.bottom = m_rcText.bottom;
				m_rcText.top    = m_rcText.bottom-rcText.Height();
			}
			else
			{// 3.3调整为上对齐
				m_rcText.top    = m_rcText.top;
				m_rcText.bottom = m_rcText.top+rcText.Height();
			}
		}
	}

}//namespace DM