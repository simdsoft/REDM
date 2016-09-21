#include "QQDemoAfx.h"
#include "DMScriptHelper.h"
#include <commdlg.h>
#include "QQMainWnd.h"
#include "DMScriptImpl.h"

extern CQQMainWnd* g_pMainWnd;
DMScriptHelper::DMScriptHelper()
{
	m_pMainWnd = NULL;
	m_pLogEdit = NULL;
	m_pStrEdit = NULL;
}

void DMScriptHelper::InitScriptHelper(DMHWnd* pMainWnd)
{
	m_pMainWnd = pMainWnd;
	if (m_pMainWnd)
	{
		DUIButton* pStrBtn = pMainWnd->FindChildByNameT<DUIButton>(L"lua_str_btn");
		if (pStrBtn)
		{
			pStrBtn->m_EventMgr.SubscribeEvent(DMEventCmdArgs::EventID,Subscriber(&DMScriptHelper::OnStrBtn,this));
		}

		DUIButton* pFileBtn = pMainWnd->FindChildByNameT<DUIButton>(L"lua_file_btn");
		if (pFileBtn)
		{
			pFileBtn->m_EventMgr.SubscribeEvent(DMEventCmdArgs::EventID,Subscriber(&DMScriptHelper::OnFileBtn,this));
		}

		DUIButton* pBufBtn = pMainWnd->FindChildByNameT<DUIButton>(L"lua_buf_btn");
		if (pBufBtn)
		{
			pBufBtn->m_EventMgr.SubscribeEvent(DMEventCmdArgs::EventID,Subscriber(&DMScriptHelper::OnBufBtn,this));
		}

		DUIButton* pStopBtn = pMainWnd->FindChildByNameT<DUIButton>(L"lua_stop_btn");
		if (pStopBtn)
		{
			pStopBtn->m_EventMgr.SubscribeEvent(DMEventCmdArgs::EventID,Subscriber(&DMScriptHelper::OnStopBtn,this));
		}

		m_pLogEdit = pMainWnd->FindChildByNameT<DUIRichEdit>(L"lua_log_edit");
		m_pStrEdit = pMainWnd->FindChildByNameT<DUIRichEdit>(L"lua_str_edit");
		m_pCheckBox =  pMainWnd->FindChildByNameT<DUICheckBox>(L"lua_check_box");

		pMainWnd->DMADDEVENT(DMEventScriptStartArgs::EventID);
		pMainWnd->DMADDEVENT(DMEventScriptEndArgs::EventID);
		pMainWnd->DMADDEVENT(DMEventScriptTimerArgs::EventID);
	}
}

void DMScriptHelper::RunScriptTimer()
{
	DMEventScriptTimerArgs evt(m_pMainWnd);
	m_pMainWnd->DV_FireEvent(evt);
}

DMCode DMScriptHelper::OnStrBtn(DMEventArgs* pEvent)
{
	do    
	{
		m_pScript.Release();
		g_pDMApp->CreateRegObj((void**)&m_pScript,DMScriptImpl::GetClassName(), DMREG_Script); 
		if (NULL == m_pScript || NULL == m_pStrEdit)
		{
			break;
		} 

		CStringW  wstrLua = m_pStrEdit->GetWindowText();
		if (wstrLua.IsEmpty())
		{
			break; 
		}  

		CStringA strLua = DMW2A(wstrLua,CP_ACP);

		LogStart();
		m_pScript->InitScript();
		m_pScript->RunString((LPSTR)(const char*)strLua);
		if (!m_pCheckBox||!m_pCheckBox->DM_IsChecked())
		{
			DMEventScriptStartArgs evt(m_pMainWnd);
			m_pMainWnd->DV_FireEvent(evt);
		}
		
		LogEnd();

		// 恢复Edit的焦点
		m_pStrEdit->SetSel(MAKELONG(-1,-1));
		m_pStrEdit->DM_OnSetFocus();
	} while (false);

	return DM_ECODE_OK;
}

DMCode DMScriptHelper::OnFileBtn(DMEventArgs* pEvent)
{
	do 
	{
		m_pScript.Release();
		g_pDMApp->CreateRegObj((void**)&m_pScript,DMScriptImpl::GetClassName(), DMREG_Script); 
		if (NULL == m_pScript)
		{
			break;
		} 

		char szFileTitle[_MAX_FNAME] = {0};  
		char szFileName[_MAX_PATH]   = {0};    
		OPENFILENAMEA ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = _MAX_PATH;
		ofn.lpstrDefExt = "gif";
		ofn.lpstrFileTitle = (LPSTR)szFileTitle;
		ofn.nMaxFileTitle = _MAX_FNAME;
		ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING| OFN_NOCHANGEDIR;
		ofn.lpstrFilter = "lua files(*.gif)\0*.lua\0All files (*.*)\0*.*\0\0";
		ofn.hwndOwner = m_pMainWnd->m_hWnd;
		if (false == ::GetOpenFileNameA(&ofn))
		{
			break;
		}
		LogStart();
		if (m_pStrEdit)
		{
			DWORD dwSize = GetFileSizeW(DMA2W(szFileName).GetBuffer());
			if (dwSize)
			{
				DMBufT<byte>pBuf;pBuf.Allocate(dwSize);
				DWORD dwRead = 0;
				GetFileBufW(DMA2W(szFileName).GetBuffer(),(void**)&pBuf, dwSize,dwRead);
				CStringA cstr = (char*)pBuf.get();
				CStringW str = DMA2W(cstr);
				m_pStrEdit->SetWindowText(str);
			}
		}
		m_pScript->InitScript();
		m_pScript->RunFile(szFileName);
		if (!m_pCheckBox||!m_pCheckBox->DM_IsChecked())
		{
			DMEventScriptStartArgs evt(m_pMainWnd);
			m_pMainWnd->DV_FireEvent(evt);
		}
		LogEnd();

		// 恢复Edit的焦点
		m_pStrEdit->SetSel(MAKELONG(-1,-1));
		m_pStrEdit->DM_OnSetFocus();
	} while (false);
	return DM_ECODE_OK;
}

DMCode DMScriptHelper::OnBufBtn(DMEventArgs* pEvent)
{
	do 
	{
		m_pScript.Release();
		g_pDMApp->CreateRegObj((void**)&m_pScript,DMScriptImpl::GetClassName(), DMREG_Script); 
		DMSmartPtrT<IDMRes> pRes;
		g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
		if (pRes.isNull())
		{
			break;
		}

		unsigned long ulSize = 0;
		if(!DMSUCCEEDED(pRes->GetItemSize(RES_LAYOUT,L"test_lua",ulSize)))
		{
			break;
		}

		DMBufT<byte>pBuf;pBuf.Allocate(ulSize);
		if(!DMSUCCEEDED(pRes->GetItemBuf(RES_LAYOUT,L"test_lua", pBuf, ulSize)))
		{
			break;
		}

		LogStart();
		CStringA strLua((char*)pBuf.get(),ulSize);
		if (m_pStrEdit)
		{
			m_pStrEdit->SetWindowText(DMA2W(strLua));
		}
		m_pScript->InitScript();
		m_pScript->RunBuf((LPCSTR)pBuf.get(), ulSize);
		if (!m_pCheckBox||!m_pCheckBox->DM_IsChecked())
		{
			DMEventScriptStartArgs evt(m_pMainWnd);
			m_pMainWnd->DV_FireEvent(evt);
		}
		LogEnd();

		// 恢复Edit的焦点
		m_pStrEdit->SetSel(MAKELONG(-1,-1));
		m_pStrEdit->DM_OnSetFocus();
	} while (false);

	return DM_ECODE_OK;
}

DMCode DMScriptHelper::OnStopBtn(DMEventArgs* pEvent)
{
	do 
	{
		if (NULL == m_pMainWnd)
		{
			break;
		}
		DMEventScriptEndArgs evt(m_pMainWnd);
		m_pMainWnd->DV_FireEvent(evt);
	} while (false);

	return DM_ECODE_OK;
}


// private
void DMScriptHelper::LogStart()
{
	if (m_pLogEdit)
	{
		m_pLogEdit->SetWindowText(L"");
	}
}

void DMScriptHelper::LogEnd()
{
	wchar_t szPath[MAX_PATH] = {0};
	GetRootFullPath(L".\\luaLog.dat",szPath,MAX_PATH);
	char szLuaLogPath[MAX_PATH] = {0};
	UnicodeToAscii(szPath,szLuaLogPath,MAX_PATH);
	if (m_pLogEdit)
	{
		DWORD dwSize = GetFileSizeW(szPath);
		if (dwSize)
		{
			DMBufT<byte>pBuf;pBuf.Allocate(dwSize);
			DWORD dwRead = 0;
			GetFileBufW(szPath,(void**)&pBuf, dwSize,dwRead);
			CStringA cstrLog = (char*)pBuf.get();
			CStringW strLog = DMA2W(cstrLog);
			m_pLogEdit->SetWindowText(strLog);
		}
	}
	remove(szLuaLogPath);
}
