#include "TGPDemoAfx.h"
#include "TGPComboboxEx.h"


int TGPComboboxEx::InsertItem(int nIndex, DMXmlNode&XmlNode)
{
	if (m_pListBox)
	{
		return m_pListBox->InsertItem(nIndex,XmlNode);
	}
	return -1;
}

TGPListBoxEx* TGPComboboxEx::GetListBox() 
{
	return m_pListBox;
}

DMCode TGPComboboxEx::OnDropDown(DMDropWnd *pDropDown)
{
	DM_ModifyState(DUIWNDSTATE_PushDown,DUIWNDSTATE_Hover,true);// TODO.COMBOX背景图为小三角，切换其状态
	DMCode iErr = __super::OnDropDown(pDropDown);
	pDropDown->DM_InsertChild(m_pListBox);
	pDropDown->DV_UpdateChildLayout();

	m_pListBox->DM_SetVisible(true);
	m_pListBox->DV_SetFocusWnd();
	m_pListBox->EnsureVisible(GetCurSel());
	return iErr;
}

DMCode TGPComboboxEx::OnCloseUp(DMDropWnd *pDropDown,UINT uCode)
{
	DM_ModifyState(DUIWNDSTATE_Hover,DUIWNDSTATE_PushDown,true);
	pDropDown->DM_RemoveChildWnd(m_pListBox);
	m_pListBox->DM_SetVisible(false);
	//弹出框关闭时，此处如设置回Container，可能会引起Container框左上角绘制出list的非客户端，
	//而list实际上不需要设置回Container,Combox会Fire 选中项，所以屏蔽此处
	m_pListBox->SetContainer(GetContainer());

	return __super::OnCloseUp(pDropDown,uCode);
}

DMCode TGPComboboxEx::DV_CreateChildWnds(DMXmlNode &XmlNode)
{
	// todo.初始显示信息,这里按TGP显示大区和延迟两个子控件处理
	do 
	{    
		DMXmlNode XmlShowStyle = XmlNode.FirstChild(DMAttr::DUIComboBoxExAttr::ITEM_subshow);
		DUIWindow::DV_CreateChildWnds(XmlShowStyle);

		CreateListBox(XmlNode);// 创建List
	} while (false);

	return DM_ECODE_OK;
}

DMCode TGPComboboxEx::DV_GetTextRect( LPRECT lpRect )
{
	return DV_GetClientRect(lpRect);
}

bool TGPComboboxEx::SetCurSel(int iSel)
{
	bool bRet = false;
	if (m_pListBox)
	{
		bRet = m_pListBox->SetCurSel(iSel);
	}
	return bRet;
}

int TGPComboboxEx::GetCurSel() const
{
	if (m_pListBox)
	{
		return m_pListBox->GetCurSel();
	}
	return -1;
}

int TGPComboboxEx::GetCount() const
{
	if (m_pListBox)
	{
		return m_pListBox->GetCount();
	}
	return  -1;
}

CStringW TGPComboboxEx::GetLBText(int iItem) 
{
	if (m_pListBox)
	{
		// todo. 
	}
	CStringW strText;
	return strText;
}  

int TGPComboboxEx::GetListBoxHeight()
{
	if (m_pListBox) 
	{
		return m_pListBox->GetAllItemHeight();
	} 

	return -1;
}

bool TGPComboboxEx::CreateListBox(DMXmlNode &XmlNode)
{
	bool bRet = false;
	do 
	{
		if (!XmlNode.IsValid())
		{
			break;
		}
		g_pDMApp->CreateRegObj((void**)&m_pListBox, TGPListBoxEx::GetClassName(),DMREG_Window);
		if (NULL == m_pListBox)
		{
			DMASSERT_EXPR(0,L"EDIT的listbox创建失败");
			break;
		}

		m_pListBox->SetContainer(GetContainer());
		DMCode dwCode =	m_pListBox->InitDMData(XmlNode.FirstChild(DMAttr::DUIComboBoxExAttr::ITEM_sublistboxex));
		m_pListBox->SetAttribute(L"pos",L"0,0,-0,-0",true);
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

void TGPComboboxEx::OnSelChanged()
{  
	do 
	{
		if (NULL == m_pListBox)
		{
			break;
		}
		int iSel = m_pListBox->GetCurSel();
		if (-1 == iSel||iSel>=m_pListBox->GetCount())
		{
			break;
		}
		LPTGPLBITEMEX pSelItem = NULL;
		m_pListBox->GetObj(iSel,pSelItem);
		if (NULL == pSelItem
			||false == pSelItem->XmlDoc.IsValid())
		{
			break;
		}
		DMXmlNode XmlNode = pSelItem->XmlDoc.Root();
		// todo.XMLNODE都给了！自己设计combox要怎么显示
		// 这里仅做个示例:
		if (m_Node.m_pFirstChild) 
		{
			DMXmlNode StaticXmlNode =  XmlNode.FirstChild();
			if (StaticXmlNode.IsValid())
			{
				CStringW strText = StaticXmlNode.Attribute(L"text");
				m_Node.m_pFirstChild->SetAttribute(L"text",strText);
			}
		}

		if (m_Node.m_pFirstChild->m_Node.m_pNextSibling)
		{
			DMXmlNode MsXmlNode =  XmlNode.FirstChild().NextSibling();
			if (MsXmlNode.IsValid())
			{
				CStringW strText = MsXmlNode.Attribute(L"text");
				m_Node.m_pLastChild->SetAttribute(L"text",strText);

				//todo.设置tips文件路径
				wchar_t szTipsPath[MAX_PATH] = {0};
				GetRootFullPath(L"TGPRes\\layout\\xml\\tips\\dui_cftips1.xml",szTipsPath,MAX_PATH);
				m_Node.m_pLastChild->SetAttribute(L"tipxmlid",szTipsPath);
				m_Node.m_pLastChild->SetAttribute(L"tipflags",L"0,0,0,1");// 参数属性参看TGPTipsImpl::ShowTips
			}
		}

	} while (false);
} 

DMCode TGPComboboxEx::DV_FireEvent(DMEventArgs &Evt)
{
	DMCode iErr = DM_ECODE_OK;

	do 
	{
		if (CB_LIST_ID != Evt.m_IdFrom
			||NULL == m_pDropDownWnd)
		{
			iErr = DUIComboBoxBase::DV_FireEvent(Evt);
			break;
		}

		if (Evt.GetEventID()==DMEventLBSelChangedArgs::EventID)
		{
			OnSelChanged();
			break;
		}

		if (Evt.GetEventID() == DMTgpListCloseUpEventCmdArgs::EventID)
		{
			CloseUp();
			break;
		}

	} while (false);

	return iErr;
}

bool TGPComboboxEx::CalcPopupRect( int nHeight,CRect& rcPopup )
{
	bool bRet = DUIComboBoxBase::CalcPopupRect(nHeight,rcPopup);
	rcPopup.right+=91;
	return bRet;
}





