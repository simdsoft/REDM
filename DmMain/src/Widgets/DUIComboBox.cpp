#include "DmMainAfx.h"
#include "DUIComboBox.h"

namespace DM
{
	DUIComboBox::DUIComboBox()
	{
		// combobox
		DMADDEVENT(DMEventCBSelChangeArgs::EventID);
		m_strClsListBox = DUIListBox::GetClassName();
	}

	DUIComboBox::~DUIComboBox()
	{
		if (m_pListBox)
		{
			m_pListBox->DM_SendMessage(WM_DESTROY);
		}
	}

	//---------------------------------------------------
	// Function Des: ����ӿ�
	//---------------------------------------------------
	LPARAM DUIComboBox::GetItemData(UINT iItem) const
	{
		return m_pListBox->GetItemData(iItem);
	}

	int DUIComboBox::SetItemData(UINT iItem, LPARAM lParam)
	{
		return m_pListBox->SetItemData(iItem,lParam);
	}

	int DUIComboBox::InsertItem(UINT iPos,LPCTSTR pszText,int nHei,int iIcon,LPARAM lParam)
	{
		return m_pListBox->InsertString(iPos,pszText,nHei,iIcon,lParam);
	}

	bool DUIComboBox::DeleteString(UINT iItem)
	{
		return LB_ERR!=m_pListBox->DeleteString(iItem);
	}

	void DUIComboBox::ResetContent()
	{
		return m_pListBox->DeleteAll();
	}

	DUIListBox* DUIComboBox::GetListBox()
	{
		return m_pListBox;
	}

	DMCode DUIComboBox::OnDropDown(DMDropWnd *pDropDown)
	{
		DMCode iErr = __super::OnDropDown(pDropDown);
		//���Ӷ������б����Ƿ�͸�����Կ���---wzq add 20160729
		pDropDown->SetAttribute("btranslucent", m_bDropTranslucent ? "1" : "0");

		pDropDown->DM_InsertChild(m_pListBox);
		pDropDown->DV_UpdateChildLayout();

		//һ��Ҫ��ʽ���ý��㣬��������������͸��ʱ���޷���Ӧ��������Ϣ
		pDropDown->SetFocus();

		m_pListBox->DM_SetVisible(true);
		m_pListBox->DV_SetFocusWnd();
		m_pListBox->EnsureVisible(GetCurSel());
		return iErr;
	}

	DMCode DUIComboBox::OnCloseUp(DMDropWnd *pDropDown,UINT uCode)
	{
		pDropDown->DM_RemoveChildWnd(m_pListBox);
		m_pListBox->DM_SetVisible(false);
		//������ر�ʱ���˴������û�Container�����ܻ�����Container�����Ͻǻ��Ƴ�list�ķǿͻ��ˣ�
		//��listʵ���ϲ���Ҫ���û�Container,Combox��Fire ѡ����������δ˴�
		//Combox֮���Ի�ת����Ϣ������Ϊlistbox������DM_SetOwnerWnd(this),���򲻻�ת����Ϣ
		m_pListBox->SetContainer(GetContainer());
		CRect rcWnd;
		m_pListBox->DV_GetWindowRect(rcWnd);
		DM_InvalidateRect(rcWnd);
		return __super::OnCloseUp(pDropDown,uCode);
	}

	//---------------------------------------------------
	// Function Des: ����
	//---------------------------------------------------
	int DUIComboBox::GetCount()const
	{
		if (m_pListBox)
		{
			return (int)m_pListBox->GetCount();
		}
		return 0;
	}

	int DUIComboBox::GetListBoxHeight()
	{
		int nDropHeight = m_nDropHeight;
		do 
		{
			int nCount = GetCount();
			if (0 >= nCount)
			{
				nDropHeight = m_nDropEmptyHeight;
				break;
			}

			int nTotalHeight = 0;
			for (int i = 0; i < nCount; i++)
			{
				nTotalHeight += m_pListBox->GetItemHeight(i);
			}
	
			CRect rcMargin;
			m_pListBox->m_pDUIXmlInfo->m_pStyle->GetNcMargin(rcMargin);
			nDropHeight = min(nDropHeight,nTotalHeight+rcMargin.top+rcMargin.bottom);
		} while (false);
		return nDropHeight;
	}

	int DUIComboBox::GetCurSel()const
	{
		if (m_pListBox)
		{
			return m_pListBox->GetCurSel();
		}
		return -1;
	}
	CStringW DUIComboBox::GetLBText(int iItem)
	{
		CStringW strText;
		do 
		{
			if (-1 == iItem)
			{
				break;
			}

			if (m_pListBox)
			{
				m_pListBox->GetText(iItem,strText);
			}

		} while (false);
		return strText;
	}

	bool DUIComboBox::SetCurSel(int iSel)
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pListBox)
			{
				break;
			}
			if (!m_pListBox->SetCurSel(iSel))
			{
				break;
			}
			OnSelChanged();

			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUIComboBox::CreateListBox(const DMXmlNode &XmlNode)
	{
		bool bRet = false;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}
			g_pDMApp->CreateRegObj((void**)&m_pListBox, m_strClsListBox,DMREG_Window);
			if (NULL == m_pListBox)
			{
                DMFAIL_MSG("create EDIT listbox fail");
				break;
			}

			m_pListBox->SetContainer(GetContainer());
			DMCode dwCode =	m_pListBox->InitDMData(XmlNode.FirstChild(DMAttr::DUIComboBoxAttr::ITEM_sublistbox));
			m_pListBox->SetAttribute("pos","0,0,-0,-0",true);
			m_pListBox->SetAttribute("clrbg", "pbgra(ff,ff,ff,ff)", true); //Ĭ�ϰ�ɫ��ͼ
			m_pListBox->SetAttribute("hotTrack","1",true);// ������ѡ��
			m_pListBox->DM_SetOwnerWnd(this);				// chain notify message to combobox
			m_pListBox->SetID(CB_LIST_ID);
			m_pListBox->DM_SetVisible(false);				// ֻ������ʱ����ʾ����ʱҲû�г�ʼ����layout

			if (-1 != m_iInitSel)
			{
				SetCurSel(m_iInitSel);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	void DUIComboBox::OnSelChanged()
	{
		do 
		{
			int iSel = GetCurSel();
			if(!m_bHideEdit&&m_pEdit
				&&!m_pEdit->m_EventMgr.IsMuted())
			{
				CStringW strText = GetLBText(iSel);
				m_pEdit->m_EventMgr.SetMuted(true);
				m_pEdit->SetWindowText(strText);
				m_pEdit->m_EventMgr.SetMuted(false);
			}
			DM_Invalidate();
			__super::OnSelChanged();

		} while (false);
	}

	DMCode DUIComboBox::DV_FireEvent(DMEventArgs &Evt)
	{
		if (CB_LIST_ID == Evt.m_IdFrom
			&&m_pDropDownWnd)
		{
			if (Evt.GetEventID()==DMEventLBSelChangedArgs::EventID)
			{
				OnSelChanged();
				//return true;
			}
			if(Evt.GetEventID() == DMEventCmdArgs::EventID)
			{
				CloseUp();
				return true;
			}
		}
		return DUIComboBoxBase::DV_FireEvent(Evt);
	}

}//namespace DM