#include "DMDesignerAfx.h"
#include "NameAttr.h"

BEGIN_EVENT_MAP(NameAttr)
	EVENT_NAME_HANDLER("ds_attr_name",DMEVT_RENOTIFY,OnEditChange)
	EVENT_NAME_HANDLER("ds_attr_id",DMEVT_RENOTIFY,OnEditChange)
END_EVENT_INBASE()
NameAttr*  NameAttr::ms_pthis = NULL;
NameAttr::NameAttr()
{
	m_pPanel	= g_pMainWnd->FindChildByName("ds_attr_name_panel");		DMASSERT(m_pPanel);
	m_pNameEdit = m_pPanel->FindChildByNameT<DUIEdit>("ds_attr_name");		DMASSERT(m_pNameEdit);
	m_pIdEdit	= m_pPanel->FindChildByNameT<DUIEdit>("ds_attr_id");		DMASSERT(m_pIdEdit);
	m_pClSta	= m_pPanel->FindChildByNameT<DUIStatic>("ds_attr_clsname");DMASSERT(m_pClSta);
	m_pNameEdit->SetEventMask(ENM_CHANGE|m_pNameEdit->GetEventMask());
	m_pIdEdit->SetEventMask(ENM_CHANGE|m_pIdEdit->GetEventMask());
	//
	ms_pthis   = this;
}

NameAttr::~NameAttr()
{
	ms_pthis   = NULL;
	m_pPanel->DM_SetVisible(false,true);
}

DMCode NameAttr::UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hSel);
		if (NULL == pData||NULL == pData->m_pDUIWnd)
		{
			DM_MessageBox(L"这是理论上不应该出现的弹框!");
			break;
		}
		bool bMain = false;
		if (hOldSel && 0 == m_pObjTree->GetParentItem(hOldSel))// 切换前是DMHWnd窗口
		{
			m_pIdEdit->SetReadOnly(false);
			m_pNameEdit->SetReadOnly(false);
		}
		if (hSel && 0 == m_pObjTree->GetParentItem(hSel))// 切换到最上层了，默认为DMHWnd窗口
		{
			m_pIdEdit->SetReadOnly(true);
			m_pNameEdit->SetReadOnly(true);
			bMain = true;
		}
		CStringA strClsName = pData->m_pDUIWnd->V_GetClassName();
		CStringA strName    = pData->m_pDUIWnd->GetName();
		CStringA strId; //      = (0!=pData->m_pDUIWnd->GetID())?IntToString(pData->m_pDUIWnd->GetID()):L"";
		if(pData->m_pDUIWnd->GetID() != 0)
			strId.Format("%d", pData->m_pDUIWnd->GetID());
		if (bMain)
		{
			strClsName = "DMHWnd";
			strName    = "";
			strId      = "";
		}
		else if (0 == strClsName.CompareNoCase(pData->m_pRootWnd->V_GetClassName()))// 第二层，即root层
		{
			strName    = pData->m_pXmlNode->Attribute(XML_NAME);
			strId      = pData->m_pXmlNode->Attribute(XML_ID);
		}
		if (pData->m_bPanel)
		{
			strClsName = "ItemPanel";
		}
		m_pClSta->SetTextA(strClsName);
		m_pNameEdit->SetTextA(strName);
		m_pIdEdit->SetTextA(strId);

		m_pPanel->DM_SetVisible(true,true);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode NameAttr::OnEditChange(DMEventArgs *pEvt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMEventRENotifyArgs *pEvent = (DMEventRENotifyArgs*)(pEvt);
		if (EN_CHANGE != pEvent->m_iNotify ||NULL == m_hObjSel)
		{
			break;
		}
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(m_hObjSel);
		if (NULL == pData
			||!pData->IsValid()
			||NULL == pData->m_pDUIWnd)
		{
			break;
		}

		bool bName = (pEvent->m_pSender == m_pNameEdit);
		CStringA strKey = bName?XML_NAME:XML_ID;
		CStringA strOldValue = pData->m_pXmlNode->Attribute(strKey);strOldValue.Trim();
		CStringA strValue = bName?m_pNameEdit->GetTextA():m_pIdEdit->GetTextA();strValue.Trim();
		if (strValue.CompareNoCase(strOldValue))
		{
			iErr = pData->m_pDUIWnd->SetAttribute(strKey,strValue);
			if (DMSUCCEEDED(iErr))
			{
				pData->m_pXmlNode->SetAttribute(strKey,strValue,false);
				m_pObjXml->SetDocUnSave(pData->m_pXmlNode);
				if (bName)
				{// 同步更新objtree上的显示
					CStringA strText = pData->m_pDUIWnd->V_GetClassName();
					if (pData->m_bPanel)
					{
						strText = "ItemPanel";
					}
					CStringA strName = pData->m_pDUIWnd->GetName();
					if (!strName.IsEmpty())
					{
						strText.Format("%s[%s]", (LPCSTR)strText, (LPCSTR)strName);
					}
					DM::LPTVITEMEX pTvi = m_pObjTree->GetItem(m_hObjSel);
					pTvi->pPanel->m_Node.m_pFirstChild->m_pDUIXmlInfo->m_strText = DMA2W(strText);
					m_pObjTree->UpdateItemRect(m_hObjSel);
				}
			}
			else
			{
				bName?m_pNameEdit->SetTextA(strOldValue):m_pIdEdit->SetTextA(strOldValue);
			}
		}
		else
		{
			iErr = DM_ECODE_OK;
		}

		if (!DMSUCCEEDED(iErr))
		{
			DM_MessageBox(L"名字/ID区设置失败!");
		}
	} while (false);
	return iErr;
}

DMCode NameAttr::s_DMHandleEvent(DMEventArgs *pEvt)
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


