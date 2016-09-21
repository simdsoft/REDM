#include "DMDesignerAfx.h"
#include "ProjTree.h" 

ProjTree::ProjTree()
{
	
}

void ProjTree::OnRButtonDown(UINT nFlags, CPoint pt)
{
	do 
	{
		if (!m_bRightClickSel)
		{
			break;
		}
		m_hHoverItem = HitTest(pt);
		if (m_hHoverItem!=m_hSelItem 
			&& m_hHoverItem)
		{
			SelectItem(m_hHoverItem,false);
		}

		// 是否弹出菜单
		if (NULL == m_hHoverItem||NULL == g_pMainWnd||NULL == g_pMainWnd->m_pDesignerXml)
		{
			break;
		}

		ProjTree* pProjTree = g_pMainWnd->FindChildByNameT<ProjTree>(L"ds_projtree");DMASSERT(pProjTree);
		if (this == pProjTree)// 此tree还可用于skin的选择
		{
			g_pMainWnd->m_pDesignerXml->PopProjTreeMenu(m_hHoverItem);
		}
	} while (false);	
}

HDMTREEITEM ProjTree::InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent/*=DMTVI_ROOT*/, HDMTREEITEM hInsertAfter/*=DMTVI_LAST*/,BOOL bEnsureVisible/*=FALSE*/)
{
	HDMTREEITEM hRet = NULL;
	do 
	{
		hRet =  __super::InsertItem(XmlItem,hParent,hInsertAfter,bEnsureVisible);
		if (!hRet)
		{
			break;
		}
		UpdateItemRect(hRet);
	} while (false);
	return hRet;
}

DMCode ProjTree::UpdateItemRect(HDMTREEITEM hRet)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (!hRet) 
		{
			break;
		}
		DM::LPTVITEMEX pData = GetItem(hRet);
		CStringW strText = pData->pPanel->m_pDUIXmlInfo->m_strText;
		if (strText.IsEmpty())
		{
			break; 
		}  
		CRect rcTest (0,0,0x7FFF,0x7FFF);
		DMSmartPtrT<IDMCanvas> pCanvas;
		DMSmartPtrT<IDMRender> pRender;
		g_pDMApp->CreateRegObj((void**)&pRender,NULL, DMREG_Render);
		pRender->CreateCanvas(0, 0, &pCanvas);
		pData->pPanel->DV_SetDrawEnvironEx(pCanvas);
		if (!DMSUCCEEDED(pCanvas->DrawText(strText, strText.GetLength(), rcTest, DT_LEFT|DT_CALCRECT)))
		{ 
			break; 
		}
		rcTest.bottom = pData->iHeight;
		rcTest.right += 10;
		SetItemRect(hRet,rcTest);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;

}

// AttrTree---------------------------------------------
DMCode AttrTree::DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL==m_hHoverItem)
		{
			break;
		}
		if (!DMSUCCEEDED(DUIWindow::DV_OnUpdateToolTip(pt,tipInfo)))
		{
			break;
		}

		GetItemRect(m_hHoverItem,tipInfo.rcScreenTarget);
		GetContainer()->OnClientToScreen(tipInfo.rcScreenTarget);
		tipInfo.iDelayTime = 100;
		tipInfo.iSpanTime  = 7000;
		tipInfo.rcPosFlags = CRect(0,tipInfo.rcScreenTarget.Width(),-tipInfo.rcScreenTarget.Height(),1);
		DMXmlInitAttrPtr pData = (DMXmlInitAttrPtr)GetItemData(m_hHoverItem);
		tipInfo.dwReserve = (DWORD)pData;
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}
