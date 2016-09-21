#include "QQDemoAfx.h"
#include "Gif_Test.h"
#include <commdlg.h>
#include "QQMainWnd.h"
extern CQQMainWnd* g_pMainWnd;

DMCode Gif_Test::OnOpenGif(DMEventArgs *pEvt)
{
	DUIGif *pGifWnd = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUIGif>(L"biggif");
	if (pGifWnd)
	{
		wchar_t szFileTitle[_MAX_FNAME] = {0};  
		wchar_t szFileName[_MAX_PATH]   = {0};    
		OPENFILENAMEW ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = _MAX_PATH;
		ofn.lpstrDefExt = L"gif";
		ofn.lpstrFileTitle = (LPTSTR)szFileTitle;
		ofn.nMaxFileTitle = _MAX_FNAME;
		ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING| OFN_NOCHANGEDIR;
		ofn.lpstrFilter = L"gif files(*.gif)\0*.gif\0All files (*.*)\0*.*\0\0";
		ofn.hwndOwner = g_pMainWnd->m_pWidgetWnd->m_hWnd;
		if (::GetOpenFileNameW(&ofn))
		{
			if (DMSUCCEEDED(pGifWnd->LoadFromFile(szFileName)))
			{
				pGifWnd->Start();
			}
		}
	}
	return DM_ECODE_OK;
}

DMCode Gif_Test::OnReplayGif(DMEventArgs *pEvt)
{
	DUIGif *pGifWnd = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUIGif>(L"biggif");
	if (pGifWnd)
	{
		pGifWnd->Restart();
	}
	return DM_ECODE_OK;
}