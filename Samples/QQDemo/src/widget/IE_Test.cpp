#include "QQDemoAfx.h"
#include "IE_Test.h"
#include "QQMainWnd.h"
extern CQQMainWnd* g_pMainWnd;



DMCode IE_Test::OnBtnIeBack()
{
	do 
	{
		IDUIWeb* pIE = GetIeObj();
		if (!pIE)
		{
			break;
		}
		pIE->GoBack();
	} while (false);
	return DM_ECODE_OK;
}

DMCode IE_Test::OnBtnIeRefresh()
{
	do 
	{
		IDUIWeb* pIE = GetIeObj();
		if (!pIE)
		{
			break;
		}
		pIE->Refresh();
	} while (false);
	return DM_ECODE_OK;
}

DMCode IE_Test::OnBtnIefront()
{
	do 
	{
		IDUIWeb* pIE = GetIeObj();
		if (!pIE)
		{
			break;
		}
		pIE->GoForward();
	} while (false);
	return DM_ECODE_OK;
}

DMCode IE_Test::IERefreshWeb(DMEventArgs* pEvent)
{
	do 
	{
		IDUIWeb* pIE = GetIeObj();
		if (!pIE)
		{
			break;
		}
		DUIRichEdit *pRichEdit = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUIRichEdit>(L"ieurl");
		if (!pRichEdit)
		{
			break;
		}
		CStringW szUrl;
		wchar_t *pBuf = szUrl.GetBufferSetLength(1024);
		pRichEdit->GetWindowText(pBuf,-1);
		if (!szUrl.IsEmpty())
		{
			pIE->OpenUrl(szUrl);
		}
		szUrl.ReleaseBuffer();

	} while (false);
	return DM_ECODE_OK;
}

DMCode IE_Test::OnBtnIeJsTest()
{ 
	do  
	{
		IDUIWeb* pIE = GetIeObj();
		if (!pIE) 
		{
			break; 
		}

		wchar_t szUrl[MAX_PATH] = {0};
		HRESULT hr = pIE->GetUrl(szUrl,MAX_PATH);
		if (!SUCCEEDED(hr))
		{
			break;
		}
		CStringW strUrl = szUrl;
		if (strUrl.GetLength()<= 13
			||strUrl.Right(13)!=L"DMJsTest.html")
		{
			wchar_t szPath[MAX_PATH] = {0};
			GetRootFullPath(L".\\QQRes\\layout\\js\\DMJsTest.html",szPath,MAX_PATH);
			if (!PathFileExists(szPath))
			{
				DMASSERT_EXPR(false,L"DMJsTest.html文件不存在");
				break;
			} 

			pIE->OpenUrl(szPath);// 先打开
			CStringW str = L"alert(\"欢迎进入DM的js测试页面\")";
			pIE->ExecuteScript(str);
			break;
		}

		CStringW str = L"DM通过ExecuteScriptFuntion\r\n调用网页中StrFromDM函数,并传入字符串";
		CArray<LPCWSTR>vecParams;
		vecParams.Add(str);
		pIE->ExecuteScriptFuntion(L"StrFromDM",vecParams);

	} while (false);
	return DM_ECODE_OK;
}


IDUIWeb* IE_Test::GetIeObj()
{
	IDUIWeb* pIE = NULL;
	do 
	{
		IERealWnd * pIEWnd = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<IERealWnd>(L"ieobj");
		if (!pIEWnd)
		{
			break;
		}
		pIE = pIEWnd->m_pIEWnd->FindChildByNameT<IDUIWeb>(L"ie");
		if (!pIE)
		{
			break;
		}
	} while (false);
	return pIE;
}