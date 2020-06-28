#include "DMDesignerAfx.h"
#include "DUIPropCtrlHelper.h"
#include "DUIPropCtrl.h"

namespace DM
{ 
#define PROP_SE_FRAMECLR        PBGRA(22,22,22,0xff)// 绘制选中边框
	PropGrid::PropGrid()
	{
		m_pOwner     = NULL;
		m_pParent    = NULL;
		m_pValueEdit = NULL;
		m_bExpanded  = true;
		m_bVisible   = true;
		m_bInPlaceEdit= false;
		m_bGroup     = false;
		m_lpData     = 0;
	}

	PropGrid::~PropGrid()
	{
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pProp = m_lstSubItems.GetNext(pos);
			pProp->Release();
		}
		m_lstSubItems.RemoveAll();
	}

	DMCode PropGrid::SetOwner(DUIPropList* pOwner)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL!=m_pOwner)
			{
				break;
			}

			m_pOwner = pOwner;
			for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
			{
				IPropPtr pProp = m_lstSubItems.GetNext(pos);
				pProp->SetOwner(pOwner);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::SetParent(IProp* pParent)
	{
		m_pParent = pParent;
		return DM_ECODE_OK;
	}

	DMCode PropGrid::Expand(bool bExpand)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_bExpanded == bExpand||m_lstSubItems.IsEmpty())
			{
				break;
			}
			m_bExpanded = bExpand;
			if (m_pOwner)
			{
				m_pOwner->AdjustLayout();
				m_pOwner->DM_Invalidate();
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::Reposition(int& y)
	{
		bool bShowProperty = m_bVisible && IsParentExpanded();
		if (bShowProperty)
		{
			int dx = GetHierarchyLevel() * m_pOwner->m_iRowHei;
			CRect rcList;
			m_pOwner->DV_GetClientRect(rcList);
			m_Rect = CRect(rcList.left + dx, y, rcList.right, y + m_pOwner->m_iRowHei);
			m_rcExpandBox.SetRectEmpty();
			if (!m_lstSubItems.IsEmpty())// 非空就有expandbox
			{
				m_rcExpandBox = CRect(m_Rect.left,m_Rect.top,m_Rect.left+m_pOwner->m_iRowHei,m_Rect.bottom);
			}
			if (!IsGroup())
			{
				m_rcName = CRect(m_Rect.left+m_rcExpandBox.Width(),m_Rect.top,rcList.left+m_pOwner->m_iLeftColWid,m_Rect.bottom);
				m_rcValue = CRect(m_rcName.right,m_Rect.top,m_Rect.right,m_Rect.bottom);
				m_rcDrag  = CRect(m_rcName.right-4,m_rcName.top,m_rcName.right+4,m_rcName.bottom);// 处于name和vale之间
			}
			else// group只有name没有value
			{
				m_rcName = m_Rect;
				m_rcName.left = m_Rect.left+m_rcExpandBox.Width();
				m_rcValue.SetRectEmpty();
				m_rcDrag.SetRectEmpty();
			}

			OnInPlaceCtrlShow(false);
	
			y += m_pOwner->m_iRowHei;
		}
		else
		{
			m_Rect.SetRectEmpty();
			m_rcName.SetRectEmpty();
			m_rcValue.SetRectEmpty();
			m_rcDrag.SetRectEmpty();
		}

		// 遍历子项
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pProp = m_lstSubItems.GetNext(pos);
			pProp->Reposition(y);
		}
		return DM_ECODE_OK;
	}

	DMCode PropGrid::Invalidate()
	{
		if (m_pOwner)
		{
			m_pOwner->DM_InvalidateRect(m_Rect);
		}
		return DM_ECODE_OK;
	}

	DMCode PropGrid::AddSubItem(IPropPtr pProp)
	{
		if (pProp->GetOwner())
		{
			for (POSITION pos = pProp->GetOwner()->m_lstProps.GetHeadPosition(); pos != NULL;)
			{
				IPropPtr pListProp = pProp->GetOwner()->m_lstProps.GetNext(pos);
				if (pListProp == pProp || pListProp->IsSubItem(pProp))
				{
					// Can't ad the same property twice
					return DM_ECODE_FAIL;
				}
			}
		}

		pProp->SetParent(this);
		m_lstSubItems.AddTail(pProp);
		pProp->SetOwner(m_pOwner);
		return DM_ECODE_OK;
	}

	DMCode PropGrid::RemoveSubItem(IProp* &pProp,bool bDelete)
	{
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			POSITION posSaved = pos;
			IProp* pListProp = m_lstSubItems.GetNext(pos);

			if (pListProp == pProp)
			{
				m_lstSubItems.RemoveAt(posSaved);

				if (m_pOwner != NULL && m_pOwner->m_pSel == pProp)
				{
					m_pOwner->m_pSel = NULL;
				}

				if (bDelete)
				{
					pProp->PreDelete();
					pProp->Release();
				}

				return DM_ECODE_OK;
			}

			if (DMSUCCEEDED(pListProp->RemoveSubItem(pProp, bDelete)))
			{
				return DM_ECODE_OK;
			}
		}
		return DM_ECODE_FAIL;
	}

	DMCode PropGrid::RemoveAllSubItem(bool bDelete)
	{
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pProp = m_lstSubItems.GetNext(pos);
			if (bDelete)
			{
				pProp->PreDelete();
				pProp->Release();
			}
		}
		m_lstSubItems.RemoveAll();
		return DM_ECODE_OK;
	}

	DMCode PropGrid::PreDelete()
	{
		OnInPlaceCtrlShow(false);
		if (m_pOwner&&m_pOwner->GetCurSel() == this)
		{
			m_pOwner->SetCurSel(NULL);
		}
		if (m_pValueEdit)
		{
			if (m_pOwner)
			{
				m_pOwner->DM_DestroyChildWnd(m_pValueEdit);
			}
		}
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pProp = m_lstSubItems.GetNext(pos);
			pProp->PreDelete();
		}
		return DM_ECODE_OK;
	}

	bool PropGrid::IsSubItem(IPropPtr pSubProp)
	{
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pProp = m_lstSubItems.GetNext(pos);
			if (pSubProp == pProp || pProp->IsSubItem(pSubProp))
			{
				return true;
			}
		}
		return false;
	}

	bool PropGrid::IsParentExpanded()
	{
		for (IPropPtr pProp = m_pParent; pProp != NULL;)
		{
			if (!pProp->IsExpanded())
			{
				return false;
			}

			pProp = pProp->GetParent();
		}
		return true;
	}

	bool PropGrid::IsExpanded()
	{
		return m_bExpanded;
	}

	bool PropGrid::IsInPlaceEdit()
	{
		return m_bInPlaceEdit;
	}

	bool PropGrid::IsGroup()
	{
		return m_bGroup;
	}

	IPropPtr PropGrid::HitTest(CPoint pt,IProp::ClickArea* pnArea)
	{
		IPropPtr pHit = NULL;
		do 
		{
			if (m_Rect.PtInRect(pt))
			{
				if (pnArea != NULL)
				{
					if (m_rcExpandBox.PtInRect(pt))
					{
						*pnArea = ClickExpandBox;
					}
					else if (m_rcDrag.PtInRect(pt))
					{
						*pnArea = ClickDrag;
					}
					else if (m_rcValue.PtInRect(pt))
					{
						*pnArea = ClickValue;
					}
					else
					{
						*pnArea = ClickName;
					}
				}
				pHit = this;
				break;
			}

			for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
			{
				IPropPtr pProp = m_lstSubItems.GetNext(pos);
				pHit = pProp->HitTest(pt, pnArea);
				if (pHit != NULL)
				{
					break;
				}
			}
		} while (false);
		return pHit;
	}

	int PropGrid::GetHierarchyLevel()
	{
		int nLevel = 0;
		for (IPropPtr pParent = m_pParent; pParent != NULL; pParent = pParent->GetParent())
		{
			nLevel++;
		}

		return nLevel;
	}

	int PropGrid::GetHeiWithSub()
	{
		int iHei = 0;
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			IPropPtr pProp = m_lstSubItems.GetNext(pos);
			int iSubHei = pProp->GetHeiWithSub();
			iHei += iSubHei;
		}
		iHei += m_Rect.Height();
		return iHei;
	}

	DUIPropList* PropGrid::GetOwner()
	{
		return m_pOwner;
	}

	IProp* PropGrid::GetParent()
	{
		return m_pParent;
	}

	LPARAM PropGrid::GetData()
	{
		return m_lpData;
	}

	void PropGrid::SetData(LPARAM lpData)
	{
		m_lpData = lpData;
	}

	IProp* PropGrid::FindSubItemByData(LPARAM lpData)
	{
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			IProp* pProp = m_lstSubItems.GetNext(pos);

			if (pProp->GetData() == lpData)
			{
				return pProp;
			}

			pProp = pProp->FindSubItemByData(lpData);
			if (pProp != NULL)
			{
				return pProp;
			}
		}
		return NULL;
	}

	void PropGrid::UpdateSubXml(DMXmlNode &XmlNode)
	{
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			IProp* pProp = m_lstSubItems.GetNext(pos);
			XmlNode.SetAttribute(pProp->GetName(),pProp->GetValue());
			pProp->UpdateSubXml(XmlNode);
		}
	}

	CStringW PropGrid::GetName()
	{
		return m_strName;
	}

	CStringW PropGrid::GetValue()
	{
		return m_strValue;
	}

	DMCode PropGrid::InitDMData(DMXmlNode &XmlNode)
	{
		DMBase::InitDMData(XmlNode);

		DMXmlNode XmlItem = XmlNode.FirstChild();
		while (XmlItem.IsValid())
		{
			IPropPtr pNew = NULL;
			m_pOwner->CreateRegObj((void**)&pNew,XmlItem.GetName());
			if (pNew)
			{
				pNew->SetOwner(m_pOwner);
				pNew->InitDMData(XmlItem);
				pNew->OnCreateInPlaceCtrl();
				AddSubItem(pNew);
			}
			
			XmlItem = XmlItem.NextSibling();
		}
		return DM_ECODE_OK;
	}

	DMCode PropGrid::OnRender(IDMCanvas* pCanvas)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_Rect.IsRectEmpty())
			{
				break;
			}
		
			//1.绘制expandbox
			OnDrawExpandBox(pCanvas,m_rcExpandBox);

			//2.绘制name
			OnDrawName(pCanvas,m_rcName);

			//3.绘制value
			OnDrawValue(pCanvas, m_rcValue);

			// 递归绘制子项
			for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
			{
				IPropPtr pProp = m_lstSubItems.GetNext(pos);
				pProp->OnRender(pCanvas);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnDrawExpandBox(IDMCanvas* pCanvas,CRect rcExpand)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcExpand.IsRectEmpty())
			{
				break;
			}
			if (m_pOwner->m_pExpandSkin)
			{
				m_pOwner->m_pExpandSkin->Draw(pCanvas,rcExpand,m_bExpanded);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnDrawName(IDMCanvas* pCanvas, CRect rcName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcName.IsRectEmpty())
			{
				break;
			}
			if (m_pOwner->m_pItemBgSkin)
			{
				bool bSel = this == m_pOwner->m_pSel;
				m_pOwner->m_pItemBgSkin->Draw(pCanvas,rcName,bSel);
			}

			rcName.left += 10;
			pCanvas->DrawText(m_strName,-1,rcName,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnDrawValue(IDMCanvas* pCanvas, CRect rcValue)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcValue.IsRectEmpty())
			{
				break;
			}

			if (m_pOwner->m_pItemBgSkin)
			{
				m_pOwner->m_pItemBgSkin->Draw(pCanvas,rcValue,0);
			}

			rcValue.left += 10;
			pCanvas->DrawText(m_strValue,-1,rcValue,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnDrawDesc(IDMCanvas* pCanvas, CRect rcDesc)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			//1.绘边框
			AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcDesc,CPoint(4,4));

			//2.计算字符高度
			CSize szChar;
			pCanvas->MeasureText(L"A",1,&szChar);
			rcDesc.DeflateRect(4,4);
			
			//3.绘name字符
			CRect rcName = rcDesc;
			rcName.bottom = rcDesc.top + szChar.cy + 15;
			AutoDrawText(pCanvas,L"face:新宋体,size:20,weight:700",PBGRA(0x0,215,255,0xff),m_strName, -1, rcName, DT_WORDBREAK|DT_LEFT|DT_TOP);

			//4.绘desc字符
			rcDesc.top = rcName.bottom;
			AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:400",PBGRA(0x0,215,255,0xff),m_strDescr, -1, rcDesc, DT_WORDBREAK|DT_LEFT|DT_TOP);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnClickValue(UINT uiMsg, CPoint point)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (WM_LBUTTONDOWN == uiMsg)
			{
				OnInPlaceCtrlShow(true);
			}	

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnClickName(UINT uiMsg, CPoint point)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (WM_LBUTTONDOWN == uiMsg)
			{
				OnInPlaceCtrlShow(false);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnClickDrag( UINT uiMsg, CPoint point )
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (WM_LBUTTONDOWN == uiMsg)
			{
				OnInPlaceCtrlShow(false);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			if (NULL != m_pValueEdit)
			{
				break;
			}

			// 增加一个显示值的edit
			CStringW strWXml = L"<edit textalign=\"center\" clrbg=\"pbgra(ff,ff,ff,ff)\" clrcaret=\"pbgra(ff,ff,ff,ff)\" bautosel=\"1\"/>";
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pValueEdit, L"edit",DMREG_Window);
			m_pOwner->DM_InsertChild(m_pValueEdit);

			m_pValueEdit->InitDMData(EditNode);
			m_pValueEdit->OnAttrCustomEx(L"bmultiLines", L"1", false);
			m_pValueEdit->SetWindowText(m_strValue);
			m_pValueEdit->DM_SetVisible(false);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropGrid::OnInPlaceCtrlShow(bool bShow /*= true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_rcValue.IsRectEmpty()
				||NULL == m_pValueEdit)
			{
				break;
			}

			m_pValueEdit->DM_SetVisible(bShow, true);
			m_bInPlaceEdit = bShow;
			m_pValueEdit->DM_FloatLayout(m_rcValue);
			if (bShow)
			{
				m_pValueEdit->DV_SetFocusWnd();
			}
			else
			{
				CStringW strOldValue = m_strValue;
				m_strValue = m_pValueEdit->GetWindowText();
				SendValueChangedEvt(strOldValue);
				m_pValueEdit->DV_KillFocusWnd();
			}
			m_pOwner->DM_InvalidateRect(m_rcValue);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	void PropGrid::SendValueChangedEvt(CStringW strOldValue)
	{
		if (m_strValue.CompareNoCase(strOldValue))
		{
			PropValueChangedArgs Evt(m_pOwner);
			Evt.m_pSel = this;
			Evt.m_strOldValue = strOldValue;
			if (m_pOwner)
			{
				m_pOwner->DV_FireEvent(Evt);
			}
		}
	}

	//PropString---------------------------------------------------------------------------
	DMCode PropString::OnDrawName(IDMCanvas* pCanvas, CRect rcName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcName.IsRectEmpty())
			{
				break;
			} 
			if (m_pOwner->m_pSel == this)
			{
				CRect rcFrame = m_Rect;
				rcFrame.left += m_rcExpandBox.Width();
				AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcFrame,CPoint(4,4));
			}

			rcName.right-=5;
			pCanvas->DrawText(m_strName,-1,rcName,DT_RIGHT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
			//AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:100",PBGRA(240,240,240,255),m_strName,-1,rcName,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropString::OnDrawValue(IDMCanvas* pCanvas, CRect rcValue)
	{ 
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcValue.IsRectEmpty())
			{
				break;
			}
			if (m_pOwner->m_pSel == this)
			{
				CRect rcFrame = m_Rect;
				rcFrame.left += m_rcExpandBox.Width();
				AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcFrame,CPoint(4,4));
			}

			if (false == m_bInPlaceEdit&&m_pValueEdit)
			{
				DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"ds_attreditframe");
				CRect rcEdit = m_pValueEdit->m_rcWindow;
				pSkin->Draw(pCanvas,rcEdit,0);
				rcEdit.left+=10;
				AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:100",PBGRA(240,240,240,255),m_strValue,-1,rcEdit,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropString::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			if (NULL != m_pValueEdit)
			{
				break;
			}

			// 增加一个显示值的edit
			CStringW strWXml = L"<edit textalign=\"left\" rcinsertmargin=\"10,0,0,0\" clrcaret=\"pbgra(ff,ff,ff,ff)\" skin=\"ds_attreditframe\" font=\"face:新宋体,size:14,weight:100\" clrtext=\"pbgra(F0,F0,F0,FF)\" bautosel=\"1\"/>";
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pValueEdit, L"edit",DMREG_Window);
			m_pOwner->DM_InsertChild(m_pValueEdit);

			m_pValueEdit->InitDMData(EditNode);
			m_pValueEdit->OnAttrCustomEx(L"bmultilines", L"1", false);
			m_pValueEdit->SetWindowText(m_strValue);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropString::OnInPlaceCtrlShow(bool bShow /*= true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_rcValue.IsRectEmpty()
				||NULL == m_pValueEdit)
			{
				break;
			}
			CRect rcValue = m_rcValue;
			rcValue.DeflateRect(4,1,4,1);
			m_pValueEdit->DM_SetVisible(bShow,true);
			m_pValueEdit->DM_FloatLayout(rcValue);
			m_bInPlaceEdit = bShow;
			if (true == bShow)
			{
				m_pValueEdit->DV_SetFocusWnd();
			}
			else
			{
				CStringW strOldValue = m_strValue;
				m_strValue = m_pValueEdit->GetWindowText();
				SendValueChangedEvt(strOldValue);
				m_pValueEdit->DV_KillFocusWnd();
			}
		
			m_pOwner->DM_InvalidateRect(m_rcValue);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}


	// PropInt---------------------------------------------------
	DMCode PropInt::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			// 防止oxff这种出现
			int iValue = 0;
			dm_parseint(m_strValue,iValue);
			m_strValue.Format(L"%d",iValue);
			__super::OnCreateInPlaceCtrl();
			if (m_pValueEdit)
			{
				m_pValueEdit->SetAttribute(L"bnumber",L"1");
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	// PropByte---------------------------------------------------
	DMCode PropByte::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do  
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			if (NULL != m_pValueEdit)
			{
				break;
			}

			// 增加一个显示值的edit
			CStringW strWXml = L"<limitedit textalign=\"left\" includechars=\"0123456789\" rcinsertmargin=\"10,0,0,0\" clrcaret=\"pbgra(ff,ff,ff,ff)\" skin=\"ds_attreditframe\" font=\"face:新宋体,size:14,weight:100\" clrtext=\"pbgra(F0,F0,F0,FF)\" bautosel=\"1\"/>";
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pValueEdit, L"limitedit",DMREG_Window);
			m_pOwner->DM_InsertChild(m_pValueEdit);

			m_pValueEdit->InitDMData(EditNode);
			// 防止oxff这种出现
			int iValue = 0;
			dm_parseint(m_strValue,iValue);
			m_strValue.Format(L"%d",iValue);
			m_pValueEdit->SetWindowText(m_strValue);
			m_pValueEdit->SetAttribute(L"bnumber",L"1");
			m_pValueEdit->SetEventMask(ENM_CHANGE|m_pValueEdit->GetEventMask());
			m_pValueEdit->m_EventMgr.SubscribeEvent(DM::DMEventRENotifyArgs::EventID, Subscriber(&PropByte::OnEditChange, this));

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropByte::OnEditChange(DMEventArgs* pEvt)
	{
		DMEventRENotifyArgs *pEvent = (DMEventRENotifyArgs*)(pEvt);
		if (EN_CHANGE == pEvent->m_iNotify)
		{
			CStringW str = m_pValueEdit->GetWindowText();
			int iValue = _wtoi(str);
			if (iValue > (int)255)
			{
				m_pValueEdit->SetWindowText(L"255");
			}
			if (iValue < (int)0)
			{
				m_pValueEdit->SetWindowText(L"0");
			}
		}
		return DM_ECODE_OK;
	}

	//PropOption---------------------------------------------
	PropOption::PropOption()
	{
		m_pValueCbx = NULL;
	}

	DMCode PropOption::OnDrawValue(IDMCanvas* pCanvas, CRect rcValue)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pOwner->m_pSel == this)
			{
				CRect rcFrame = m_Rect;
				rcFrame.left += m_rcExpandBox.Width();
				AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcFrame,CPoint(4,4));
			}

			if (false == m_bInPlaceEdit)
			{
				DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"ds_attreditframe");
				CRect rcEdit = m_pValueCbx->m_rcWindow;
				pSkin->Draw(pCanvas,rcEdit,0);
				rcEdit.left += 10;
				AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:100",PBGRA(240,240,240,255),m_strValue,-1,rcEdit,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropOption::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			if (NULL != m_pValueCbx)
			{
				break;
			} 

			CStringW strWXml = L"<combobox skin=\"ds_attreditframe\"  textoffset=\"10\" clrtext=\"pbgra(F0,F0,F0,FF)\" btnskin=\"ds_propcomboxbtn\"  bhideedit=\"1\" dropheight=\"100\">"\
				L"<subedit  clrtext=\"pbgra(F0,F0,F0,FF)\" font=\"face:新宋体,size:14,weight:100\" />"\
				L"<sublistbox textpoint=\"10,-1\" skin=\"ds_menubg\" clritemtext=\"pbgra(F0,F0,F0,FF)\" clrbg=\"pbgra(33,33,33,ff)\" bhottrack=\"1\" clritemselbg=\"pbgra(bd,bd,bd,ff)\" clritemseltext=\"pbgra(33,33,33,ff)\"/>"\
				L"</combobox>";

			// 增加一个显示值的combox
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pValueCbx, DUINAME_ComboxBox,DMREG_Window);
			m_pOwner->DM_InsertChild(m_pValueCbx);

			m_pValueCbx->InitDMData(EditNode);
			CStringWList strList;
			int nCount = (int)SplitStringT(m_strOptions,L'|',strList);
			int iSel = 0;
			if (nCount>0)
			{
				for (int i=0; i<nCount; i++)
				{
					if (0 == strList[i].CompareNoCase(m_strValue))
					{
						iSel = i;
					}
					m_pValueCbx->InsertItem(-1,strList[i]);
				}
			}
			m_pValueCbx->SetCurSel(iSel);
			m_pValueCbx->DM_SetVisible(false);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropOption::OnInPlaceCtrlShow(bool bShow /*= true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_rcValue.IsRectEmpty()
				||NULL == m_pValueCbx)
			{
				break;
			}
			CRect rcValue = m_rcValue;
			rcValue.DeflateRect(4,2,4,2);
			m_pValueCbx->DM_SetVisible(bShow, true);
			m_pValueCbx->DM_FloatLayout(rcValue);
			m_bInPlaceEdit = bShow;
			CStringW strOldValue = m_strValue;
			m_strValue = m_pValueCbx->GetLBText(m_pValueCbx->GetCurSel());
			SendValueChangedEvt(strOldValue);

			m_pOwner->DM_InvalidateRect(m_rcValue);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropOption::PreDelete()
	{
		__super::PreDelete();
		if (m_pValueCbx)
		{
			if (m_pOwner)
			{
				m_pOwner->DM_DestroyChildWnd(m_pValueCbx);
			}
		}
		return DM_ECODE_OK;
	}
	//PropClr---------------------------------------------
	PropClr::PropClr()
	{
		m_pcrRect = NULL;
		m_crValue.SetTextInvalid();
	}

	DMCode PropClr::Reposition(int& y)
	{
		do 
		{
			__super::Reposition(y);
			if (m_rcValue.IsRectEmpty())
			{
				break;
			}
			CRect rcBg = m_rcValue;
			rcBg.DeflateRect(4,1,4,1);
			m_rcClr = CRect(rcBg.left,rcBg.top,rcBg.left+30,rcBg.bottom);

			m_rcText = m_rcValue;
			m_rcText.left = m_rcClr.right;
			if (m_rcText.Width()>128)
			{
				m_rcText.right = m_rcText.left + 128;
			}
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode PropClr::OnDrawValue(IDMCanvas* pCanvas, CRect rcValue)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcValue.IsRectEmpty())
			{
				break;
			}

			if (m_pOwner->m_pSel == this)
			{
				CRect rcFrame = m_Rect;
				rcFrame.left += m_rcExpandBox.Width();
				AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcFrame,CPoint(4,4));
			}

			DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"ds_attreditframe");
			CRect rcEdit = rcValue;
			rcEdit.DeflateRect(4,1,4,1);
			pSkin->Draw(pCanvas,rcEdit,0);
			CRect rcClr = m_rcClr;
			rcClr.DeflateRect(4,4,4,4);
			AutoFillRoundRect(pCanvas,m_crValue,m_rcClr,CPoint(4,4)); 

			if (false == m_bInPlaceEdit)
			{
				CRect rcText = m_rcText;
				rcText.DeflateRect(4,3,4,3);
				rcText.right+=10;
				AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:100",PBGRA(240,240,240,255),m_strValue,-1,rcText,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropClr::OnClickValue(UINT uiMsg, CPoint point)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (WM_LBUTTONDOWN == uiMsg)
			{
				OnInPlaceCtrlShow(true);
				if (m_rcClr.PtInRect(point))
				{
					DMHook hook;
					CHOOSECOLOR csc;                 // common dialog box structure 
					static COLORREF arrayClr[16];	 // array of custom colors 

					// Initialize CHOOSECOLOR 
					ZeroMemory(&csc, sizeof(csc));
					csc.lStructSize = sizeof(csc);
					csc.hwndOwner = m_pOwner->GetContainer()->OnGetHWnd();
					csc.lpCustColors = (LPDWORD) arrayClr;
					csc.rgbResult = m_crValue.ToCOLORREF();
					csc.Flags = CC_SOLIDCOLOR;

					if (ChooseColor(&csc))
					{
						m_crValue.r = GetRValue(csc.rgbResult);
						m_crValue.g = GetGValue(csc.rgbResult);
						m_crValue.b = GetBValue(csc.rgbResult);
						m_pcrRect->SetAddress((int)m_crValue.b,(int)m_crValue.g,(int)m_crValue.r,(int)m_crValue.a);
						CStringW strOldValue = m_strValue;
						m_strValue.Format(L"pbgra(%02x,%02x,%02x,%02x)",(int)m_crValue.b,(int)m_crValue.g,(int)m_crValue.r,(int)m_crValue.a);
						SendValueChangedEvt(strOldValue);
						m_pOwner->DM_InvalidateRect(m_rcClr);
					}
				}
			}	

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropClr::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			if (NULL != m_pcrRect)
			{
				break;
			} 

			CStringW strWXml = L"<duirect bvisible=\"0\" ncmargin=\"1,1,1,1\" clrbg=\"pbgra(cc,cc,cc,ff)\" clrnc=\"pbgra(cc,cc,cc,ff)\" clrdot=\"pbgra(ff,ff,ff,ff)\" clrtext=\"pbgra(ff,ff,ff,ff)\" clrcaret=\"pbgra(ff,ff,ff,ff)\"/>";
			// 增加一个显示BGRA值的RECT
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pcrRect, L"duirect",DMREG_Window);
			m_pOwner->DM_InsertChild(m_pcrRect);
			m_pcrRect->InitDMData(EditNode);
			for (int i=0; i<4; i++)
			{
				m_pcrRect->SetFieldRange(i,0,255);
			}
			m_pcrRect->SetID(-1);
			m_pcrRect->DM_SendMessage(EM_SETEVENTMASK,0 ,ENM_CHANGE);
			m_pcrRect->SetAddress((int)m_crValue.b,(int)m_crValue.g,(int)m_crValue.r,(int)m_crValue.a);
			m_strValue.Format(L"pbgra(%02x,%02x,%02x,%02x)",(int)m_crValue.b,(int)m_crValue.g,(int)m_crValue.r,(int)m_crValue.a);
			m_pcrRect->m_EventMgr.SubscribeEvent(DUIRectChangedArgs::EventID, Subscriber(&PropClr::OnEditChange, this));

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropClr::OnInPlaceCtrlShow(bool bShow /*= true*/)
	{ 
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_rcValue.IsRectEmpty()
				||NULL == m_pcrRect)
			{
				break;
			}
			m_pcrRect->DM_SetVisible(bShow,true);
			m_bInPlaceEdit = bShow;

			CRect rcText = m_rcText;
			rcText.DeflateRect(4,3,4,3);
			m_pcrRect->DM_FloatLayout(rcText);
			m_pOwner->DM_InvalidateRect(m_rcValue);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropClr::PreDelete()
	{
		__super::PreDelete();
		if (m_pcrRect)
		{
			m_pcrRect->m_EventMgr.UnSubscribeEvent(DUIRectChangedArgs::EventID, Subscriber(&PropClr::OnEditChange, this));
			if (m_pOwner)
			{
				m_pOwner->DM_DestroyChildWnd(m_pcrRect);
			}
		}
		return DM_ECODE_OK;
	}

	DMCode PropClr::OnEditChange(DMEventArgs *pEvt)
	{
		CRect rcAddr;
		m_pcrRect->GetAddress(rcAddr);
		m_crValue.SetRGBA((BYTE)rcAddr.right,(BYTE)rcAddr.top,(BYTE)rcAddr.left,(BYTE)rcAddr.bottom);
		CStringW strOldValue = m_strValue;
		m_strValue.Format(L"pbgra(%02x,%02x,%02x,%02x)",(int)m_crValue.b,(int)m_crValue.g,(int)m_crValue.r,(int)m_crValue.a);
		SendValueChangedEvt(strOldValue);
		m_pOwner->DM_InvalidateRect(m_rcClr);

		return DM_ECODE_OK;
	} 

	//PropRect------------------------------------------------------------
	PropRect::PropRect()
	{
		m_pRect = NULL;
	}


	DMCode PropRect::Reposition(int& y)
	{
		do 
		{
			__super::Reposition(y);
			if (m_rcValue.IsRectEmpty())
			{
				break;
			}
			m_rcRect = m_rcValue;
			if (m_rcRect.Width()>128)
			{
				m_rcRect.right = m_rcRect.left + 128;
			}
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode PropRect::OnDrawValue(IDMCanvas* pCanvas, CRect rcValue)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcValue.IsRectEmpty())
			{
				break;
			}

			if (m_pOwner->m_pSel == this)
			{
				CRect rcFrame = m_Rect;
				rcFrame.left += m_rcExpandBox.Width();
				AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcFrame,CPoint(4,4));
			}

			if (false == m_bInPlaceEdit)
			{
				DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"ds_attreditframe");
				CRect rcEdit = rcValue;
				rcEdit.DeflateRect(4,1,4,1);
				pSkin->Draw(pCanvas,rcEdit,0);
				rcEdit.left+=10;
				AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:100",PBGRA(240,240,240,255),m_strValue,-1,rcEdit,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropRect::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			if (NULL != m_pRect)
			{
				break;
			} 

			CStringW strWXml = L"<duirect bvisible=\"0\" ncmargin=\"1,1,1,1\" clrbg=\"pbgra(cc,cc,cc,ff)\" clrnc=\"pbgra(cc,cc,cc,ff)\" clrdot=\"pbgra(ff,ff,ff,ff)\" clrtext=\"pbgra(ff,ff,ff,ff)\" clrcaret=\"pbgra(ff,ff,ff,ff)\"/>";
			// 增加一个显示BGRA值的RECT
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pRect, L"duirect",DMREG_Window);
			m_pOwner->DM_InsertChild(m_pRect);
			m_pRect->InitDMData(EditNode);
			m_pRect->SetID(-1);
			m_pRect->DM_SendMessage(EM_SETEVENTMASK,0 ,ENM_CHANGE);
			m_pRect->SetAddress((int)m_valueRect.left,(int)m_valueRect.top,(int)m_valueRect.right,(int)m_valueRect.bottom);
			m_strValue.Format(L"%02d,%02d,%02d,%02d",(int)m_valueRect.left,(int)m_valueRect.top,(int)m_valueRect.right,(int)m_valueRect.bottom);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropRect::OnInPlaceCtrlShow(bool bShow /*= true*/)
	{ 
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_rcValue.IsRectEmpty()
				||NULL == m_pRect)
			{
				break;
			}
			m_pRect->DM_SetVisible(bShow,true);
			m_bInPlaceEdit = bShow;

			m_pRect->GetAddress(m_valueRect);
			CStringW strOldValue = m_strValue;
			m_strValue.Format(L"%02d,%02d,%02d,%02d",(int)m_valueRect.left,(int)m_valueRect.top,(int)m_valueRect.right,(int)m_valueRect.bottom);
			SendValueChangedEvt(strOldValue);

			CRect rcRect = m_rcRect;
			rcRect.DeflateRect(4,3,4,3);
			m_pRect->DM_FloatLayout(rcRect);
			m_pOwner->DM_InvalidateRect(m_rcValue);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropRect::PreDelete()
	{
		__super::PreDelete();
		if (m_pRect)
		{
			if (m_pOwner)
			{
				m_pOwner->DM_DestroyChildWnd(m_pRect);
			}
		}
		return DM_ECODE_OK;
	}

	//PropSize------------------------------------------------------------
	PropSize::PropSize()
	{
		m_pSize = NULL;
	}

	DMCode PropSize::Reposition(int& y)
	{
		do 
		{
			__super::Reposition(y);
			if (m_rcValue.IsRectEmpty())
			{
				break;
			}
			m_rcSize = m_rcValue;
			if (m_rcSize.Width()>84)
			{
				m_rcSize.right = m_rcSize.left + 84;
			}
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode PropSize::OnDrawValue(IDMCanvas* pCanvas, CRect rcValue)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcValue.IsRectEmpty())
			{
				break;
			}

			if (m_pOwner->m_pSel == this)
			{
				CRect rcFrame = m_Rect;
				rcFrame.left += m_rcExpandBox.Width();
				AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcFrame,CPoint(4,4));
			}

			if (false == m_bInPlaceEdit)
			{

				DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"ds_attreditframe");
				CRect rcEdit = rcValue;
				rcEdit.DeflateRect(4,1,4,1);
				pSkin->Draw(pCanvas,rcEdit,0);
				rcEdit.left+=10;
				AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:100",PBGRA(240,240,240,255),m_strValue,-1,rcEdit,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropSize::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			if (NULL != m_pSize)
			{
				break;
			} 

			CStringW strWXml = L"<duisize bvisible=\"0\" ncmargin=\"1,1,1,1\" clrbg=\"pbgra(cc,cc,cc,ff)\" clrnc=\"pbgra(cc,cc,cc,ff)\" clrdot=\"pbgra(ff,ff,ff,ff)\" clrtext=\"pbgra(ff,ff,ff,ff)\" clrcaret=\"pbgra(ff,ff,ff,ff)\"/>";
			// 增加一个显示BGRA值的RECT
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pSize, L"duisize",DMREG_Window);
			m_pOwner->DM_InsertChild(m_pSize);
			m_pSize->InitDMData(EditNode);
			m_pSize->SetID(-1);
			m_pSize->DM_SendMessage(EM_SETEVENTMASK,0 ,ENM_CHANGE);
			m_pSize->SetSize((int)m_valueSize.cx,(int)m_valueSize.cy);
			m_strValue.Format(L"%02d,%02d",(int)m_valueSize.cx,(int)m_valueSize.cy);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropSize::OnInPlaceCtrlShow(bool bShow /*= true*/)
	{ 
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_rcValue.IsRectEmpty()
				||NULL == m_pSize)
			{
				break;
			}
			m_pSize->DM_SetVisible(bShow,true);
			m_bInPlaceEdit = bShow;

			m_pSize->GetSize(m_valueSize);
			CStringW strOldValue = m_strValue;
			m_strValue.Format(L"%02d,%02d",(int)m_valueSize.cx,(int)m_valueSize.cy);
			SendValueChangedEvt(strOldValue);

			CRect rcRect = m_rcSize;
			rcRect.DeflateRect(4,3,4,3);
			m_pSize->DM_FloatLayout(rcRect);
			m_pOwner->DM_InvalidateRect(m_rcValue);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropSize::PreDelete()
	{
		__super::PreDelete();
		if (m_pSize)
		{
			if (m_pOwner)
			{
				m_pOwner->DM_DestroyChildWnd(m_pSize);
			}
		}
		return DM_ECODE_OK;
	}


	//PropFont------------------------------------------------------------
	PropFont::PropFont()
	{
		m_pBtn = NULL;
	}

	DMCode PropFont::Reposition(int& y)
	{
		do 
		{
			__super::Reposition(y);
			if (m_rcValue.IsRectEmpty())
			{
				break;
			}

			m_rcEdit = m_rcValue;
			m_rcEdit.right -= 40;

			m_rcBtn = m_rcValue;
			m_rcBtn.left = m_rcEdit.right;
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode PropFont::OnDrawValue(IDMCanvas* pCanvas, CRect rcValue)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pOwner->m_pSel == this)
			{
				CRect rcFrame = m_Rect;
				rcFrame.left += m_rcExpandBox.Width();
				AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcFrame,CPoint(4,4));
			}

			if (false == m_bInPlaceEdit)
			{
				DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"ds_attreditframe");
				CRect rcEdit = rcValue;
				rcEdit.DeflateRect(4,1,4,1);
				pSkin->Draw(pCanvas,rcEdit,0);
				rcEdit.left+=10;
				AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:100",PBGRA(240,240,240,255),m_strValue,-1,rcEdit,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropFont::OnClickValue(UINT uiMsg, CPoint point)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (WM_LBUTTONDOWN == uiMsg)
			{
				OnInPlaceCtrlShow(true);
			}	
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropFont::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			__super::OnCreateInPlaceCtrl();
			if (NULL!=m_pBtn)
			{
				break;
			}

			// 创建小按钮
			CStringW strWXml = L"<button skin=\"ds_custombutton\" text=\"...\"/>";
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pBtn, L"button",DMREG_Window);
			m_pOwner->DM_InsertChild(m_pBtn);
			m_pBtn->InitDMData(EditNode);
			m_pBtn->SetID(-2);
			m_pBtn->m_EventMgr.SubscribeEvent(DMEventCmdArgs::EventID, Subscriber(&PropFont::OnBtnClick, this));

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;

	}

	DMCode PropFont::OnInPlaceCtrlShow(bool bShow /*= true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_rcValue.IsRectEmpty()
				||NULL == m_pValueEdit
				||NULL == m_pBtn)
			{
				break;
			}
		
			CRect rcEdit = m_rcEdit;
			rcEdit.DeflateRect(4,1,4,1);
			m_pValueEdit->DM_FloatLayout(rcEdit);
			CRect rcBtn = m_rcBtn;
			rcBtn.DeflateRect(1,3,1,3);
			m_pBtn->DM_FloatLayout(rcBtn);
			m_pOwner->DM_InvalidateRect(m_rcValue);

			m_pValueEdit->DM_SetVisible(bShow,true);
			m_pBtn->DM_SetVisible(bShow,true);
			CStringW strOldValue = m_strValue;
			m_strValue = m_pValueEdit->GetWindowText();
			m_pValueEdit->SetEventMask(ENM_CHANGE|m_pValueEdit->GetEventMask());
			m_pValueEdit->m_EventMgr.SubscribeEvent(DMEventRENotifyArgs::EventID, Subscriber(&PropFont::OnEditChange, this));
			SendValueChangedEvt(strOldValue);
			m_bInPlaceEdit = bShow;

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropFont::OnBtnClick(DMEventArgs* pEvt)
	{
		DMHook hook;
		IDMFont* pFont = g_pDMApp->GetFont(m_pValueEdit->GetWindowText());

		CHOOSEFONTW cf;
		LOGFONTW lfw = {0};
		memcpy(&lfw,pFont->GetLogFont(),sizeof(LOGFONTW));

		// Initialize CHOOSEFONT 
		wchar_t szStyleName[64]; // contains style name after return
		ZeroMemory(&cf, sizeof(cf));
		cf.lStructSize = sizeof(cf);
		cf.lpszStyle = (LPWSTR)&szStyleName;
		cf.lpLogFont = &lfw;
		cf.Flags = CF_EFFECTS | CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT;
		cf.hwndOwner = m_pOwner->GetContainer()->OnGetHWnd();

		if (ChooseFont(&cf))
		{
			CStringW strOldValue = m_strValue;
			CStringW strNewValue = GetFontKey(&lfw);
			if (0 != strOldValue.CompareNoCase(strNewValue))
			{
				m_pValueEdit->SetWindowText(strNewValue);
				SendValueChangedEvt(strOldValue);
			}
		}
	
		return DM_ECODE_OK;
	}

	DMCode PropFont::PreDelete()
	{
		if (m_pValueEdit)
		{
			m_pValueEdit->m_EventMgr.UnSubscribeEvent(DMEventRENotifyArgs::EventID, Subscriber(&PropFont::OnEditChange, this));
		}
		__super::PreDelete();
		if (m_pBtn)
		{
			if (m_pOwner)
			{
				m_pOwner->DM_DestroyChildWnd(m_pBtn);
			}
		}
		return DM_ECODE_OK;
	}

	CStringW PropFont::GetFontKey(const LPLOGFONTW lpLogFont)
	{
		CStringW szKey;
		CStringW strFaceName = lpLogFont->lfFaceName;
		szKey.Format(L"weight:%d,charset:%d,underline:%d,italic:%d,strike:%d,size:%d,face:%s",lpLogFont->lfWeight,lpLogFont->lfCharSet,lpLogFont->lfUnderline,lpLogFont->lfItalic,lpLogFont->lfStrikeOut,lpLogFont->lfHeight,strFaceName);
		return szKey;
	}

	DMCode PropFont::OnEditChange(DMEventArgs *pEvt)
	{
		CStringW strOldValue = m_strValue;
		m_strValue = m_pValueEdit->GetWindowText();
		SendValueChangedEvt(strOldValue);
		return DM_ECODE_OK;
	}


	//PropAccel------------------------------------------------------------
	DMCode PropAccel::Reposition(int& y)
	{
		do 
		{
			__super::Reposition(y);
			if (m_rcValue.IsRectEmpty())
			{
				break;
			}
			m_rcHotkey = m_rcValue;
			if (m_rcHotkey.Width()>64)
			{
				m_rcHotkey.right = m_rcHotkey.left + 64;
			}
		} while (false);
		return DM_ECODE_OK;
	}

	PropAccel::PropAccel()
	{
		m_pHotkey = NULL;
	}

	DMCode PropAccel::OnDrawValue(IDMCanvas* pCanvas, CRect rcValue)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (rcValue.IsRectEmpty())
			{
				break;
			}

			if (m_pOwner->m_pSel == this)
			{
				CRect rcFrame = m_Rect;
				rcFrame.left += m_rcExpandBox.Width();
				AutoDrawRoundRect(pCanvas,PROP_SE_FRAMECLR,PS_SOLID,1,rcFrame,CPoint(4,4));
			}

			if (false == m_bInPlaceEdit)
			{
				DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(L"ds_attreditframe");
				CRect rcEdit = rcValue;
				rcEdit.DeflateRect(4,1,4,1);
				pSkin->Draw(pCanvas,rcEdit,0);
				rcEdit.left+=10;
				AutoDrawText(pCanvas,L"face:新宋体,size:14,weight:100",PBGRA(240,240,240,255),m_strValue,-1,rcEdit,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropAccel::OnCreateInPlaceCtrl()
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}

			if (NULL != m_pHotkey)
			{
				break;
			} 

			CStringW strWXml = L"<hotkey bvisible=\"0\" ncmargin=\"1,1,1,1\" clrbg=\"pbgra(cc,cc,cc,ff)\" clrnc=\"pbgra(cc,cc,cc,ff)\" clrtext=\"pbgra(ff,ff,ff,ff)\" clrcaret=\"pbgra(ff,ff,ff,ff)\"/>";
			// 增加一个显示BGRA值的RECT
			CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
			DMXmlNode EditNode = doc.Root();

			g_pDMApp->CreateRegObj((void**)&m_pHotkey, L"hotkey",DMREG_Window);
			m_pOwner->DM_InsertChild(m_pHotkey);
			m_pHotkey->InitDMData(EditNode);
			m_pHotkey->SetID(-1);
			DWORD dwAccel = DUIAccel::TranslateAccelKey(m_strValue);
			m_pHotkey->SetHotKey(LOWORD(dwAccel),HIWORD(dwAccel));
			m_strValue = m_pHotkey->GetHotKeyName();

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropAccel::OnInPlaceCtrlShow(bool bShow /*= true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_rcValue.IsRectEmpty()
				||NULL == m_pHotkey)
			{
				break;
			}
			m_pHotkey->DM_SetVisible(bShow,true);
			m_bInPlaceEdit = bShow;

			CStringW strOldValue = m_strValue;
			m_strValue = m_pHotkey->GetHotKeyName();
			SendValueChangedEvt(strOldValue);

			CRect rcRect = m_rcHotkey;
			rcRect.DeflateRect(4,3,4,3);
			m_pHotkey->DM_FloatLayout(rcRect);
			m_pOwner->DM_InvalidateRect(m_rcValue);

			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode PropAccel::PreDelete()
	{
		__super::PreDelete();
		if (m_pHotkey)
		{
			if (m_pOwner)
			{
				m_pOwner->DM_DestroyChildWnd(m_pHotkey);
			}
		}
		return DM_ECODE_OK;
	}

}//namespace DM


