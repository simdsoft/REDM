#include "QQDemoAfx.h"
#include "ListBox_Test.h"
#include "QQMainWnd.h"
extern CQQMainWnd* g_pMainWnd;

int ListBox_Test::m_nInsertCount = 0;
DMCode ListBox_Test::OnInsert(DMEventArgs *pEvt)
{
	do 
	{
		if (m_nInsertCount>=5)
		{
			break;
		}

		DUIListBox* pListBox = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUIListBox>(L"listbox1");
		if (!pListBox)
		{
			break;
		}

		CStringW strBuf;
		strBuf.Format(L"insert%d",m_nInsertCount);
		int nHei = (m_nInsertCount+1)*20;
		pListBox->InsertString(1,strBuf,nHei);
		m_nInsertCount++;
	} while (false);
	return DM_ECODE_OK;
}

DMCode ListBox_Test::OnDelete(DMEventArgs *pEvt)
{
	do 
	{
		if (0 == m_nInsertCount)
		{
			break;
		}
		DUIListBox* pListBox = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUIListBox>(L"listbox1");
		if (!pListBox)
		{
			break;
		}

		pListBox->DeleteString(1);
		m_nInsertCount--;
	} while (false);
	return DM_ECODE_OK;
}