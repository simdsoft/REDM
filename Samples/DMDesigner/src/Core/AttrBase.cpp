#include "DMDesignerAfx.h"
#include "AttrBase.h"

AttrBase::AttrBase()
{
	m_pObjTree = g_pMainWnd->FindChildByNameT<ObjTree>(L"ds_objtree");DMASSERT(m_pObjTree); 
	m_pObjXml  = g_pMainWnd->m_pDesignerXml;
	m_hObjSel  = NULL;
	m_bMuted   = false;
	m_pObjTree->m_EventMgr.SubscribeEvent(DMEventTCSelChangedArgs::EventID, Subscriber(&AttrBase::OnObjTreeChanged, this));
	m_pObjTree->m_EventMgr.SubscribeEvent(DUITreeExLockChangedArgs::EventID, Subscriber(&AttrBase::OnLockChanged, this));

	m_pPanel  = NULL;
	m_bMsgHandled = FALSE;
}

AttrBase::~AttrBase()
{
	m_pObjTree->m_EventMgr.UnSubscribeEvent(DMEventTCSelChangedArgs::EventID, Subscriber(&AttrBase::OnObjTreeChanged, this));
	m_pObjTree->m_EventMgr.UnSubscribeEvent(DUITreeExLockChangedArgs::EventID, Subscriber(&AttrBase::OnLockChanged, this));
	m_pObjTree = NULL;
	m_hObjSel  = NULL;
	m_bMuted   = false;
	m_pPanel   = NULL;
	m_bMsgHandled = FALSE;
}

DMCode AttrBase::OnObjTreeChanged(DMEventArgs* pEvt)
{
	DMAutoResetT<bool> AutoMuted(&m_bMuted,true);// 窗口切换时，不影响消息
	DMEventTCSelChangedArgs* pEvent = (DMEventTCSelChangedArgs*)pEvt;
	m_hObjSel = pEvent->m_hNewSel;
	UpdateLock(m_hObjSel);// 更新lock
	UpdataInfo(pEvent->m_hNewSel,pEvent->m_hOldSel);
	return DM_ECODE_OK;
}

DMCode AttrBase::OnLockChanged(DMEventArgs* pEvt)
{
	DMAutoResetT<bool> AutoMuted(&m_bMuted,true);// 窗口切换时，不影响消息
	DUITreeExLockChangedArgs* pEvent =  (DUITreeExLockChangedArgs*)pEvt;
	UpdateLock(pEvent->m_hItem);
	return DM_ECODE_OK;
}

DMCode AttrBase::UpdateLock(HDMTREEITEM hItem)
{
	do 
	{ 
		if (NULL == hItem
			||hItem != m_hObjSel
			||NULL == m_pPanel)
		{
			break;
		}
		DM::LPTVITEMEX pData = m_pObjTree->GetItem(hItem);
		if (DMTVEXLock_UnLocked == pData->iLockValue)
		{
			m_pPanel->DM_EnableWindow(TRUE,true);
		}
		else
		{
			m_pPanel->DM_EnableWindow(FALSE,true);
		}		
	} while (false);
	return DM_ECODE_OK;
}
