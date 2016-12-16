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
	// Function Des: 对外接口
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
		//增加对下拉列表窗口是否透明属性控制---wzq add 20160729
		pDropDown->SetAttribute(L"btranslucent", m_bDropTranslucent ? L"1" : L"0");

		pDropDown->DM_InsertChild(m_pListBox);
		pDropDown->DV_UpdateChildLayout();

		//一定要显式设置焦点，否则在下拉窗口透明时，无法响应鼠标滚轮消息
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
		//弹出框关闭时，此处如设置回Container，可能会引起Container框左上角绘制出list的非客户端，
		//而list实际上不需要设置回Container,Combox会Fire 选中项，所以屏蔽此处
		//Combox之所以会转发消息，是因为listbox调用了DM_SetOwnerWnd(this),否则不会转发消息
		m_pListBox->SetContainer(GetContainer());
		CRect rcWnd;
		m_pListBox->DV_GetWindowRect(rcWnd);
		DM_InvalidateRect(rcWnd);
		return __super::OnCloseUp(pDropDown,uCode);
	}

	//---------------------------------------------------
	// Function Des: 重载
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
				nDropHeight = 0;
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

	bool DUIComboBox::CreateListBox(DMXmlNode &XmlNode)
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
				DMASSERT_EXPR(0,L"EDIT的listbox创建失败");
				break;
			}

			m_pListBox->SetContainer(GetContainer());
			DMCode dwCode =	m_pListBox->InitDMData(XmlNode.FirstChild(DMAttr::DUIComboBoxAttr::ITEM_sublistbox));
			m_pListBox->SetAttribute(L"pos",L"0,0,-0,-0",true);
			m_pListBox->SetAttribute(L"hotTrack",L"1",true);// 不让它选中
			m_pListBox->DM_SetOwnerWnd(this);				// chain notify message to combobox
			m_pListBox->SetID(CB_LIST_ID);
			m_pListBox->DM_SetVisible(false);				// 只在下拉时才显示，此时也没有初始化完layout

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