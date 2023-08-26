﻿#include "DMDesignerAfx.h"
#include "ObjTree.h"


HDMTREEITEM ObjTree::InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent/*=DMTVI_ROOT*/, HDMTREEITEM hInsertAfter/*=DMTVI_LAST*/,BOOL bEnsureVisible/*=FALSE*/)
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

DMCode ObjTree::UpdateItemRect(HDMTREEITEM hRet)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (!hRet) 
		{
			break;
		}
		DM::LPTVITEMEX pData = GetItem(hRet);
		CStringW strText = pData->pPanel->m_Node.m_pFirstChild->m_pDUIXmlInfo->m_strText;
		if (strText.IsEmpty())
		{
			break; 
		}  
		CRect rcTest (0,0,0x7FFF,0x7FFF);
		DMSmartPtrT<IDMCanvas> pCanvas;
		DMSmartPtrT<IDMRender> pRender;
		g_pDMApp->CreateRegObj((void**)&pRender,NULL, DMREG_Render);
		pRender->CreateCanvas(0, 0, &pCanvas);
		pData->pPanel->m_Node.m_pFirstChild->DV_SetDrawEnvironEx(pCanvas);
		if (!DMSUCCEEDED(pCanvas->DrawText(strText, strText.GetLength(), rcTest, DT_LEFT|DT_CALCRECT)))
		{ 
			break; 
		}
		rcTest.bottom = pData->iHeight;
		rcTest.right += 10;
		rcTest.right += 36+6;
		SetItemRect(hRet,rcTest);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

void ObjTree::OnNodeFree(DM::LPTVITEMEX &pItemData)
{
	m_DeletedItemsDataArr.Add(pItemData);
	m_DeletedItemsLparamArr.Add(pItemData ? pItemData->lParam : NULL);
	__super::OnNodeFree(pItemData);
}

bool ObjTree::IsItemStillExist(const DM::LPTVITEMEX &pItemData, LPARAM lp)
{
	if (!pItemData)
		return false;
	int nCount = (int)m_DeletedItemsDataArr.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (pItemData == m_DeletedItemsDataArr[i] && m_DeletedItemsLparamArr[i] == lp)
			return false;
	}
	return true;
}

void ObjTree::OnRButtonDown(UINT nFlags, CPoint pt)
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

		ObjTree* pObjTree = g_pMainWnd->FindChildByNameT<ObjTree>("ds_objtree");DMASSERT(pObjTree);
		if (this == pObjTree)
		{
			g_pMainWnd->m_pDesignerXml->PopObjTreeMenu(m_hHoverItem);
		}
	} while (false);	
}

