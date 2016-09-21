#include "DMDemoAfx.h"
#include "AttributeTestWnd.h"
#include "MainWnd.h"

BEGIN_MSG_MAP(CAttributeTestWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(DMHWnd)
END_MSG_MAP()


CAttributeTestWnd::CAttributeTestWnd(CMainWnd* pMain)
{
	m_pMainWnd			 = pMain;
	m_pNameEdit			 = NULL;
	m_pAttrNameEdit		 = NULL;
	m_pAttrValueEdit	 = NULL;
	m_pBtn				 = NULL;
	m_pInfoEdit			 = NULL;
}

CAttributeTestWnd::~CAttributeTestWnd()
{
}

BOOL CAttributeTestWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_pNameEdit = FindChildByNameT<DUIEdit>(L"edit_ctrlname");
	m_pAttrNameEdit = FindChildByNameT<DUIEdit>(L"edit_attrname");
	m_pAttrValueEdit = FindChildByNameT<DUIEdit>(L"edit_attrvalue");
	m_pBtn = FindChildByNameT<DUIButton>(L"btn_testattr");
	m_pInfoEdit = FindChildByNameT<DUIRichEdit>(L"edit_testinfo");
	DMASSERT(NULL!=m_pNameEdit&&NULL!=m_pAttrNameEdit&&NULL!=m_pAttrValueEdit&&NULL!=m_pBtn&&NULL!=m_pInfoEdit);
	return TRUE;
}

DMCode CAttributeTestWnd::OnClose()
{
	DM_AnimateWindow(200,AW_VER_NEGATIVE|AW_HIDE);
	DestroyWindow();
	g_pDMApp->RemoveSkinPoolItem(L"skinprivate");
	return DM_ECODE_OK;
}

DMCode CAttributeTestWnd::OnSetAttribute()
{
	do 
	{
		CStringW szName;
		wchar_t *pBuf = szName.GetBufferSetLength(MAX_PATH);
		m_pNameEdit->GetWindowText(pBuf,-1);
		szName.Trim();
		if (szName.IsEmpty())
		{
			m_pInfoEdit->SetWindowText(L"控件名字为空");
			break;
		}
		CStringW strMainName = m_pMainWnd->GetName();
		DUIWindow* pWnd = NULL;
		if (0==strMainName.CompareNoCase(szName))
		{
			pWnd = m_pMainWnd;
		}
		else
		{
			pWnd = m_pMainWnd->FindChildByName(szName);
		}
		if (NULL == pWnd)
		{
			m_pInfoEdit->SetWindowText(L"获取控件失败");
			break;
		}

		CStringW szAttrName;
		m_pAttrNameEdit->GetWindowText(szAttrName.GetBuffer(100),-1);
		if (szAttrName.IsEmpty())
		{
			m_pInfoEdit->SetWindowText(L"控件属性名称为空");
			break;
		}

		CStringW szAttrValue;
		m_pAttrValueEdit->GetWindowText(szAttrValue.GetBuffer(100),-1);
		if (szAttrValue.IsEmpty())
		{
			m_pInfoEdit->SetWindowText(L"控件属性值为空");
			break;
		}

		DMCode iErr = DM_ECODE_FAIL;
		if (pWnd == m_pMainWnd)
		{
			iErr = m_pMainWnd->m_pHWndXmlInfo->SetAttribute(szAttrName,szAttrValue,false);
		}
		else
		{
			iErr = pWnd->SetAttribute(szAttrName,szAttrValue,false);
		}
		if (!DMSUCCEEDED(iErr))
		{
			m_pInfoEdit->SetWindowText(L"属性值设置失败");
			break;
		}
		else
		{
			m_pInfoEdit->SetWindowText(L"属性值设置OK");
			break;
		}
	} while (false);
	return DM_ECODE_OK;
}