#include "DMDesignerAfx.h"
#include "ActionSlot.h"

ActionSlot::ActionSlot()
{
}

ActionSlot::~ActionSlot()
{
}

NullActionSlot::NullActionSlot()
{
}

NullActionSlot::~NullActionSlot()
{
}

DMCode NullActionSlot::PerformUndoActionSlot()
{
	return DM_ECODE_FAIL;
}

DMCode NullActionSlot::PerformRedoActionSlot()
{
	return DM_ECODE_FAIL;
}

EditorElemPosChgActionSlot::EditorElemPosChgActionSlot(HDMTREEITEM hItem, CRect OldRect, CRect NewRect, bool bMain, ObjTree* pTreeCtrl, DUIDragFrame* pDragFrame)
{
	m_hItem			= hItem;
	m_pObjTree		= pTreeCtrl;
	m_pDragFrame	= pDragFrame;
	m_pTreeParam	= NULL;
	m_bMain			= bMain;
	m_OldRect		= OldRect;
	m_NewRect		= NewRect;
	m_pTreeDataLaw	= NULL;
	if (m_hItem)
	{
		m_pTreeDataLaw = m_pObjTree->GetItem(m_hItem);
		if (m_pTreeDataLaw) m_pTreeParam = m_pTreeDataLaw->lParam;
	}
}

EditorElemPosChgActionSlot::~EditorElemPosChgActionSlot()
{
}

DMCode EditorElemPosChgActionSlot::PerformUndoActionSlot()
{
	DMCode iRet = DM_ECODE_FAIL;
	do
	{
		if (!m_hItem || !m_pObjTree->IsItemStillExist(m_pTreeDataLaw, m_pTreeParam)) //是否已经删除
			break;
		if (!m_pObjTree->SelectItem(m_hItem))
			break;

		DMAutoResetT<bool> bAutoMute(&g_pMainWnd->m_ActionSlotMgr.GetMemberbMuteAddSlot(), true);
		m_pDragFrame->m_StartDragRc = m_NewRect;
		m_pDragFrame->m_dragMetas[0].m_Rect = m_OldRect;
		if (m_bMain)
		{
			m_pDragFrame->SetElementWidth(m_OldRect.Width());
			m_pDragFrame->SetElementHeight(m_OldRect.Height());
		}
		else
			m_pDragFrame->SetElementRect(m_OldRect);
		m_pDragFrame->OnLButtonUp(0, 0);
		iRet = DM_ECODE_OK;
	} while (false);
	return iRet;
}

DMCode EditorElemPosChgActionSlot::PerformRedoActionSlot()
{
	DMCode iRet = DM_ECODE_FAIL;
	do
	{
		if (!m_hItem || !m_pObjTree->IsItemStillExist(m_pTreeDataLaw, m_pTreeParam))//是否已经删除
			break;
		if (!m_pObjTree->SelectItem(m_hItem))
			break;

		DMAutoResetT<bool> bAutoMute(&g_pMainWnd->m_ActionSlotMgr.GetMemberbMuteAddSlot(), true);
		m_pDragFrame->m_StartDragRc = m_OldRect;
		m_pDragFrame->m_dragMetas[0].m_Rect = m_NewRect;
		if (m_bMain)
		{
			m_pDragFrame->SetElementWidth(m_NewRect.Width());
			m_pDragFrame->SetElementHeight(m_NewRect.Height());
		}
		else
			m_pDragFrame->SetElementRect(m_NewRect);
		m_pDragFrame->OnLButtonUp(0, 0);		
		iRet = DM_ECODE_OK;
	} while (false);
	return iRet;
}