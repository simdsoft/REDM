#include "QQDemoAfx.h"
#include "ComboBox_Test.h"
#include "QQMainWnd.h"
extern CQQMainWnd* g_pMainWnd;

DMCode ComboBox_Test::OnSel(DMEventArgs *pEvt)
{
	do 
	{
		DUIComboBox* pComboBox = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUIComboBox>(L"combobox1");
		if (NULL == pComboBox)
		{
			break;
		}

		int nSel = pComboBox->GetCurSel();
		int nCount = pComboBox->GetCount();
		if (nSel>=nCount-1)
		{
			nSel = -1;//++nSel¾Í³É0ÁË
		}
		pComboBox->SetCurSel(++nSel);
	} while (false);
	return DM_ECODE_OK;
}
