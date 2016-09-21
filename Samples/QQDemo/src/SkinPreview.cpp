#include "QQDemoAfx.h"
#include "SkinPreview.h"
SkinPreview::SkinPreview()
{
	m_bDown = false;
}
void SkinPreview::DM_OnPaint(IDMCanvas* pCanvas)
{
	int iState = 0;
	DV_GetState(iState);
	if (m_pSkin)
	{
		m_pSkin->Draw(pCanvas,m_rcWindow,0);
		if (iState>0)
		{
			CRect rcDraw = m_rcWindow;
			rcDraw.DeflateRect(0,1,0,1);
			pCanvas->FillSolidRect(rcDraw,PBGRA(0,0,0,0X80));
			DMColor clrOld = pCanvas->SetTextColor(PBGRA(0xff,0xff,0xff,0xff));
			CRect rcText = m_rcWindow;rcText.bottom = rcText.top+m_rcWindow.Height()/2;
			pCanvas->DrawText(m_strThemeName,-1,rcText,DT_VCENTER|DT_SINGLELINE|DT_CENTER,0xff);
			rcText = m_rcWindow;rcText.top = rcText.bottom-m_rcWindow.Height()/2;
			pCanvas->DrawText(L"扁扁熊提供",-1,rcText,DT_VCENTER|DT_SINGLELINE|DT_CENTER,0xff);
			pCanvas->SetTextColor(clrOld);
		}
	}
	else
	{
		DMColor ClrBg;
		m_pDUIXmlInfo->m_pStyle->GetBgColor(0,ClrBg);
		if (!ClrBg.IsTextInvalid())
		{
			pCanvas->FillSolidRect(&m_rcWindow,ClrBg);
			if (iState>0)
			{
				pCanvas->FillSolidRect(m_rcWindow,PBGRA(0,0,0,0X80));
			}
		}
	}
}

void SkinPreview::OnLButtonDown(UINT nFlags,CPoint pt)
{
	m_bDown = true;
	__super::OnLButtonDown(nFlags,pt);
}

void SkinPreview::OnLButtonUp(UINT nFlags,CPoint pt)
{
	__super::OnLButtonUp(nFlags,pt);
	if (!m_bDown)
	{
		return;
	}
	m_bDown = false;
	if (m_pSkin)
	{
		DMSmartPtrT<IDMRes> pRes;
		g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
		if (pRes)
		{
			pRes->SetCurTheme(m_strThemeName);
		}
	}
	else
	{
		DMColor ClrBg;
		m_pDUIXmlInfo->m_pStyle->GetBgColor(0,ClrBg);
		if (!ClrBg.IsTextInvalid())
		{
			DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"bg");
			if (pSkin)
			{
				DMSmartPtrT<IDMBitmap> pBitmap;
				pSkin->GetBitmap(&pBitmap);
				if (pBitmap)
				{
					DMSmartPtrT<IDMCanvas> pCanvas;
					DMSmartPtrT<IDMRender> pRender;
					g_pDMApp->GetDefRegObj((void**)&pRender, DMREG_Render);
					pRender->CreateCanvas(pBitmap->GetWidth(),pBitmap->GetHeight(),&pCanvas);
					pCanvas->SelectObject(pBitmap);
					CRect rcDraw(0,0,pBitmap->GetWidth(),pBitmap->GetHeight());
					pCanvas->FillSolidRect(rcDraw,ClrBg);
					DMSmartPtrT<IDMRes> pRes;
					g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
					pRes->SetAttribute(L"boutstyle",L"1",false);// 外部设置模式
					g_pDMApp->RedrawAll();
				}
			}
		}
	}
}

