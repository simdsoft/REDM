﻿#include "DMDesignerAfx.h"
#include "AddExpandWnd.h"
 
BEGIN_MSG_MAP(AddExpandWnd)
	MSG_WM_INITDIALOG(OnInitDialog) 
	MSG_WM_LBUTTONUP(OnLButtonUp)
	CHAIN_MSG_MAP(DMHWnd)
END_MSG_MAP()
BEGIN_EVENT_MAP(AddExpandWnd)
END_EVENT_MAP()

BOOL AddExpandWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// 默认初始态为m_pRadioBtn指向Button
	FindChildByName("ds_w_Button")->DM_SetCheck(true);
	return TRUE;
}

void AddExpandWnd::OnLButtonUp(UINT nFlags,CPoint pt)
{
	DUITabLayout* pLayout = FindChildByNameT<DUITabLayout>("ds_w_tablayout");DMASSERT(pLayout);
	DUIWindow* pChild = pLayout->DM_GetWindow(GDW_FIRSTCHILD);
	while (pChild)
	{
		if (pChild->DM_IsChecked())
		{
			DUICheckBox* pCbx = g_pMainWnd->FindChildByNameT<DUICheckBox>("ds_w_widget");
			CStringA strSkin = DMW2A(pChild->m_pDUIXmlInfo->m_strText);
			strSkin.Format("ds_w_%s",strSkin);
			pCbx->SetAttribute("checkskin",strSkin);
			pCbx->SetAttribute("text", DMW2A(pChild->m_pDUIXmlInfo->m_strText));
			pCbx->SetAttribute("tiptext", DMW2A(pChild->m_pDUIXmlInfo->m_strText));
			Hide();
			break;
		}
		pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
	}
	SetMsgHandled(FALSE);
}

DMCode AddExpandWnd::Show()
{
	ShowWindow(SW_SHOW);
	DM_Invalidate();
	return DM_ECODE_OK;
}

DMCode AddExpandWnd::Hide()
{
	DM_Invalidate();
	ShowWindow(SW_HIDE);
	return DM_ECODE_OK;
}
