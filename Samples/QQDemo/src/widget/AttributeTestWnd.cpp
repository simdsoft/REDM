#include "QQDemoAfx.h"
#include "AttributeTestWnd.h"
#include "WidgetWnd.h"

BEGIN_MSG_MAP(CAttributeTestWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(DMHWnd)
END_MSG_MAP()


CAttributeTestWnd::CAttributeTestWnd(CWidgetWnd* pWidgetWnd)
{
	m_pWidgetWnd		 = pWidgetWnd;
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
	m_pNameEdit = FindChildByNameT<DUIEdit>("edit_ctrlname");
	m_pAttrNameEdit = FindChildByNameT<DUIEdit>("edit_attrname");
	m_pAttrValueEdit = FindChildByNameT<DUIEdit>("edit_attrvalue");
	m_pBtn = FindChildByNameT<DUIButton>("btn_testattr");
	m_pInfoEdit = FindChildByNameT<DUIRichEdit>("edit_testinfo");
	DMASSERT(NULL!=m_pNameEdit&&NULL!=m_pAttrNameEdit&&NULL!=m_pAttrValueEdit&&NULL!=m_pBtn&&NULL!=m_pInfoEdit);
	return TRUE;
}

DMCode CAttributeTestWnd::OnClose()
{
	DM_AnimateWindow(200,AW_VER_NEGATIVE|AW_HIDE);
	DestroyWindow();
	return DM_ECODE_OK;
}

DMCode CAttributeTestWnd::OnSetAttribute()
{
	do 
	{
		CStringA szName = m_pNameEdit->GetTextA();
		szName.Trim();
		if (szName.IsEmpty())
		{
			m_pInfoEdit->SetWindowText(L"控件名字为空");
			break;
		}
		CStringA strMainName = m_pWidgetWnd->GetName();
		DUIWindow* pWnd = NULL;
		if (0==strMainName.CompareNoCase(szName))
		{
			pWnd = m_pWidgetWnd;
		}
		else
		{
			pWnd = m_pWidgetWnd->FindChildByName(szName);
		}
		if (NULL == pWnd)
		{
			m_pInfoEdit->SetWindowText(L"获取控件失败");
			break;
		}

		CStringA szAttrName = m_pAttrNameEdit->GetTextA();
		if (szAttrName.IsEmpty())
		{
			m_pInfoEdit->SetWindowText(L"控件属性名称为空");
			break;
		}

		CStringA szAttrValue = m_pAttrValueEdit->GetTextA();
		if (szAttrValue.IsEmpty())
		{
			m_pInfoEdit->SetWindowText(L"控件属性值为空");
			break;
		}

		DMCode iErr = DM_ECODE_FAIL;
		if (pWnd == m_pWidgetWnd)
		{
			iErr = m_pWidgetWnd->m_pHWndXmlInfo->SetAttribute(szAttrName,szAttrValue,false);
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