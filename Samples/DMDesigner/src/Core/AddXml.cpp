#include "DMDesignerAfx.h"
#include "AddXml.h"
#include "AddExpandWnd.h"

BEGIN_MSG_MAP(AddXml)
	MSG_WM_LBUTTONDOWN(OnLButtonDown)
END_MSG_MAP()
BEGIN_EVENT_MAP(AddXml)
	EVENT_NAME_COMMAND(L"ds_tool",OnExpand)
END_EVENT_INBASE()// 基类

AddXml*  AddXml::ms_pthis = NULL;
AddXml::AddXml()
{
	ms_pthis = this;
}

AddXml::~AddXml()
{
	ms_pthis   = NULL;
	if (m_pExpandWnd.isValid()&&m_pExpandWnd->IsWindow())
	{
		m_pExpandWnd->DestroyWindow();
	}
	m_pExpandWnd.Release();
}

DMCode AddXml::UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel)
{
	return DM_ECODE_OK;
}

DMCode AddXml::InitAddXml()
{
	m_pTools = g_pMainWnd->FindChildByNameT<DUITabCtrl>(L"ds_tool");DMASSERT(m_pTools);
	return DM_ECODE_OK;
}

DMCode AddXml::UnInitAddXml()
{
	m_pTools = NULL;
	if (m_pExpandWnd.isValid()&&m_pExpandWnd->IsWindow())
	{
		m_pExpandWnd->DestroyWindow();
	}
	m_pExpandWnd.Release();
	return DM_ECODE_OK;
}

DMCode AddXml::OnExpand()
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (2 != m_pTools->GetCurSel())
		{
			break;
		}

		if (m_pExpandWnd.isNull())
		{
			m_pExpandWnd.Attach(new AddExpandWnd());
			m_pExpandWnd->DM_CreateWindowEx(L"ds_add_dlg",DM_DEF_WINDOW_NAME,WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,0,0,0,0,g_pMainWnd->m_hWnd,NULL,false);
			m_pExpandWnd->SendMessage(WM_INITDIALOG);
		} 

		if (m_pExpandWnd->IsWindowVisible())
		{
			m_pExpandWnd->Hide();
		}
		else
		{
			CRect rcExpand = GetExpandRect();
			m_pExpandWnd->SetWindowPos(g_pMainWnd->m_hWnd, rcExpand.right,rcExpand.bottom-34,0, 0, SWP_NOSIZE);
			m_pExpandWnd->Show();
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

void AddXml::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptScreen = point;
	::ClientToScreen(g_pMainWnd->m_hWnd,&ptScreen);
	CRect rcExpand = GetExpandRect();
	if (!rcExpand.PtInRect(ptScreen))
	{
		if (m_pExpandWnd.isValid()&&m_pExpandWnd->IsWindowVisible())
		{
			m_pExpandWnd->Hide();
		}
	}

	SetMsgHandled(FALSE);
}

CRect AddXml::GetExpandRect()
{
	CRect rect;
	if (m_pTools)
	{
		m_pTools->GetItemRect(2,rect);
		g_pMainWnd->ClientToScreen(rect);
	}
	return rect;
}

DMCode AddXml::s_DMHandleEvent(DM::DMEventArgs *pEvt)
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

BOOL AddXml::s_ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID /*= 0*/)
{
	if (ms_pthis)
	{
		ms_pthis->ProcessWindowMessage(hWnd,uMsg,wParam,lParam,lResult,dwMsgMapID);
	}
	return FALSE;
}


