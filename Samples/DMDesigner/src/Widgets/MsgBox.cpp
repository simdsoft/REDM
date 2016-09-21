#include "DMDesignerAfx.h"
#include "MsgBox.h"

struct MsgBtn_Info 
{
	int       id;
	CStringW  strText;
};
struct MsgBox_Info
{
	int			iNum;
	MsgBtn_Info BtnInfo[3];
};
MsgBox_Info g_MsgBox_Info[] = \
{
	{1,{{IDOK,L"确定"},{0,L""},{0,L""}}},						 //MB_OK
	{2,{{IDOK,L"确定"},{IDCANCEL,L"取消"},{0,L""}}},			 //MB_OKCANCEL
	{3,{{IDABORT,L"中止"},{IDRETRY,L"重试"},{IDIGNORE,L"忽略"}}},//MB_ABORTRETRYIGNORE
	{3,{{IDYES,L"是"},{IDNO,L"否"},{IDCANCEL,L"取消"}}},		 //MB_YESNOCANCEL
	{2,{{IDYES,L"是"},{IDNO,L"否"},{0,L""}}},					 //MB_YESNO
	{2,{{IDRETRY,L"重试"},{IDCANCEL,L"取消"},{0,L""}}}			 //MB_RETRYCANCEL
};

BEGIN_MSG_MAP(MsgBox)
	MSG_WM_INITDIALOG(OnInitDialog) 
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(DMHDialog)
END_MSG_MAP()
BEGIN_EVENT_MAP(MsgBox)
	EVENT_ID_COMMAND_RANGE(IDOK,IDNO,OnBtnClick)
END_EVENT_MAP()
MsgBox::MsgBox(LPCWSTR lpszXmlId)
{
	m_strXmlId = lpszXmlId;
}  

int MsgBox::MessageBox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{  
	if (m_strXmlId.IsEmpty())
	{ 
		return ::MessageBoxW(hWnd,lpText,lpCaption,uType);
	}
	m_strCaption = lpCaption;
	m_strText = lpText;
	m_uType = uType;
	return  DoModal(m_strXmlId,hWnd,true);
} 

BOOL MsgBox::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	DUITabCtrl *pTab = FindChildByNameT<DUITabCtrl>(L"ds_msgtabctrl");
	if (pTab) 
	{
		int iNum = g_MsgBox_Info[m_uType].iNum;
		pTab->SetCurSel(iNum-1);
		DUITabPage* pCurPage = pTab->GetItem(pTab->GetCurSel());
		if (pCurPage)
		{
			DMASSERT(pCurPage->DM_GetChildCount() == iNum);///< 必须保证相等
			DUIWindow* pChild = pCurPage->DM_GetWindow(GDW_FIRSTCHILD);
			for (int i=0; i<iNum; i++)
			{
				pChild->SetID(g_MsgBox_Info[m_uType].BtnInfo[i].id);
				pChild->SetAttribute(L"text",g_MsgBox_Info[m_uType].BtnInfo[i].strText);
				pChild = pChild->DM_GetWindow(GDW_NEXTSIBLING);
			}
		}
	}

	FindChildByNameT<DUIRichEdit>(L"ds_msgtext")->SetWindowText(m_strText);
	if (m_strCaption)
	{
		FindChildByName(L"ds_msgtitle")->DV_SetWindowText(m_strCaption);
	}
 
	return TRUE;
}

void MsgBox::OnSize(UINT nType, CSize size)
{
	if (!IsIconic()) 
	{
		CRect rcWnd;
		::GetWindowRect(m_hWnd, &rcWnd);
		::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);  
		HRGN hWndRgn = ::CreateRoundRectRgn(0, 0, rcWnd.right, rcWnd.bottom,4,4);
		SetWindowRgn(hWndRgn, TRUE);
		::DeleteObject(hWndRgn); 
	}             
	SetMsgHandled(FALSE);
}

DMCode MsgBox::OnBtnClick(int uID)
{
	EndDialog(uID);
	return DM_ECODE_OK;
}
