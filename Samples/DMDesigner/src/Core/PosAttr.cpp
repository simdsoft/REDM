#include "DMDesignerAfx.h"
#include "PosAttr.h"
BEGIN_MSG_MAP(PosAttr)
END_MSG_MAP()
BEGIN_EVENT_MAP(PosAttr)
END_EVENT_INBASE()
PosAttr*  PosAttr::ms_pthis = NULL;
PosAttr::PosAttr()
{
	m_pPanel = m_pObjXml->m_pRighXml->m_pList->FindChildByName(L"ds_attr_pos_panel",true);DMASSERT(m_pPanel);
	m_pPos   = m_pPanel->FindChildByNameT<DUIPos>(L"ds_pos",true);DMASSERT(m_pPos);
	ms_pthis = this; 
	m_pCurDUI  = NULL;
}

DMCode PosAttr::UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel)
{ 
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hSel);
		if (NULL == pData||NULL == pData->m_pDUIWnd)
		{
			break;
		}
		Layout* pLayout = dynamic_cast<Layout*>(pData->m_pDUIWnd->m_pLayout.get());
		if (hSel && 0 == m_pObjTree->GetParentItem(hSel)// 切换到最上层了，默认为DMHWnd窗口
			||false == DUIDragFrame::IsSupportPos(pData)
			)
		{
			m_pObjXml->m_pRighXml->m_pList->ExpandItem(POS_ATTR,false);
			m_pCurDUI = NULL;
			m_pPos->UnInitLayout();
			m_pPanel->DM_SetVisible(false,true);
			break;
		}

		if (pData->m_pDUIWnd == pData->m_pRootWnd||m_pCurDUI != pData->m_pDUIWnd)// 不相等就切换
		{
			m_pCurDUI = pData->m_pDUIWnd;
			InitPos();
		}
		m_pObjXml->m_pRighXml->m_pList->ExpandItem(POS_ATTR,true);
		m_pPanel->DM_SetVisible(true,true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode PosAttr::InitPos()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (NULL == m_pCurDUI||NULL == m_pPos)
		{
			break;
		}
		m_pPos->InitLayout(m_pCurDUI->m_pLayout);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode PosAttr::s_DMHandleEvent(DM::DMEventArgs *pEvt)
{
	if (ms_pthis&&!ms_pthis->m_bMuted)
	{
		if (DMEventOfPanelArgs::EventID == pEvt->GetEventID())
		{
			DMEventOfPanelArgs* pEvent = (DMEventOfPanelArgs*)pEvt;
			return ms_pthis->DMHandleEvent(pEvent->m_pOrgEvt);
		}
		else  
		{
			return ms_pthis->DMHandleEvent(pEvt);// 转发
		}
	}

	return DM_ECODE_FAIL;
}


