#include "DMDesignerAfx.h"
#include "DUISize.h"


namespace DM
{
	DSizeEdit::DSizeEdit()
	{
		m_iMin = -99999;
		m_iMax = 99999;
	}

	int DSizeEdit::GetField()
	{
		if (0 == GetWindowTextLength())
		{
			return 0;
		}
		CStringW str = GetWindowText();
		return _wtoi(str);
	}

	void DSizeEdit::SetField(int iField)
	{
		do 
		{
			if (iField<m_iMin||iField>m_iMax)
			{
				break;
			}
			CStringW str;
			str.Format(L"%d",iField);
			SetWindowText(str);
		} while (false);
	}

	void DSizeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (VK_BACK==nChar)
			{
				if (0==GetWindowTextLength()&&!PUSH_ALT&&!PUSH_SHIFT&&!PUSH_CTRL)
				{
					DUIWindow* pPre = DM_GetWindow(GDW_PREVSIBLING);
					if (NULL!=pPre&&pPre->IsClass(DSizeEdit::GetClassName()))
					{
						pPre->DV_SetFocusWnd();
					}
				}
				break;
			}
			if (0 == isdigit(nChar)&&nChar!='-')
			{
				break;
			}
			__super::OnChar(nChar,nRepCnt,nFlags);
			if (AdjustNum())
			{
				break;
			}
			if (GetWindowTextLength()>2)
			{

				DUIWindow* pNext = DM_GetWindow(GDW_NEXTSIBLING);
				if (NULL!=pNext&&pNext->IsClass(DSizeEdit::GetClassName()))
				{
					pNext->DV_SetFocusWnd();
				}
			}
		} while (false);
		DUIListChangedArgs Evt(this->DM_GetWindow(GDW_PARENT));
		this->DM_GetWindow(GDW_PARENT)->DV_FireEvent(Evt);
	}

	LRESULT DSizeEdit::OnImeChar(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}

	BOOL DSizeEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		if (DV_IsFocusable())
		{
			SetField(GetField()+1*(zDelta>0?1:-1));
			SetSel(MAKELONG(-1,-1));
		}
		
		return TRUE;
	}

	bool DSizeEdit::AdjustNum()
	{
		bool bRet = true;
		do 
		{
			CStringW str = GetWindowText();
			int iValue = _wtoi(str);
			if (iValue > (int)m_iMax)
			{
				SetField(m_iMax);
				break;
			}
			if (iValue < (int)m_iMin)
			{
				SetField(m_iMin);
				break;
			}
			bRet = false;
		} while (false);
		return bRet;
	}

	//DUISize-------------------------------------------------------------
#define  EDIT_DELTA           2
	DUISize::DUISize()
	{
		m_clrDot			 = PBGRA(255,0,0,255);
		m_clrText			 = PBGRA(0,0,0,255);
		m_clrCaret           = PBGRA(0,0,0,0xff);
		m_iCaretAniCount     = 6;
		DMADDEVENT(DUIListChangedArgs::EventID);//
	}

	BOOL DUISize::IsBlank() const
	{
		BOOL bRet = FALSE;
		do 
		{
			if (0!=m_pEdit[0]->GetWindowTextLength()
				||0!=m_pEdit[1]->GetWindowTextLength()
				)
			{
				break;
			}
			bRet = TRUE;
		} while (FALSE);
		return bRet;
	}

	void DUISize::ClearSize()
	{
		for (int i=0;i<2;i++)
		{
			m_pEdit[i]->SetWindowText(L'');
		}
	}

	int DUISize::GetSize(int& nField0, int& nField1) const
	{
		nField0 = m_pEdit[0]->GetField();
		nField1 = m_pEdit[1]->GetField();
		int iNoBlank = 0;
		for (int i=0;i<2;i++)
		{
			if (0!=m_pEdit[i]->GetWindowTextLength())
			{
				iNoBlank++;
			}
		}
		return iNoBlank;
	}

	int DUISize::GetSize(CSize & rcSize) const
	{
		int nField[2] = {0};
		int iNoBlank = GetSize(nField[0],nField[1]);
		rcSize.SetSize(nField[0],nField[1]);
		return iNoBlank;
	}

	void DUISize::SetSize(CSize rcSize)
	{
		SetSize(rcSize.cx,rcSize.cy);
	}

	void DUISize::SetSize(int nField0, int nField1)
	{
		m_pEdit[0]->SetField(nField0);
		m_pEdit[1]->SetField(nField1);
	}

	void DUISize::SetFieldFocus(WORD nField)
	{
		WORD Index = nField;
		if (Index<0||Index>=2)
		{
			Index = 0;
		}
		m_pEdit[Index]->DV_SetFocusWnd();
	}

	void DUISize::SetFieldRange(int nField, int nLower, int nUpper)
	{
		do 
		{
			WORD Index = nField;
			if (Index<0||Index>=2)
			{
				break;
			}
			m_pEdit[Index]->m_iMin = nLower;
			m_pEdit[Index]->m_iMax = nUpper;
		} while (false);

	}

	void DUISize::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			if (m_pEdit[0].isNull())
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);
			int iEditWid = (rcClient.Width()-EDIT_DELTA)/2;
			if (iEditWid<=0)
			{
				break;
			}
			CRect rcStep = rcClient;
			rcStep.left += iEditWid;
			rcStep.right = rcStep.left + EDIT_DELTA;
			for (int i=0;i<2;i++)
			{
				CRect rcDot = rcStep;
				rcDot.top += (rcStep.Height()-EDIT_DELTA)/2;
				rcDot.bottom = rcDot.top+EDIT_DELTA;
				pCanvas->FillSolidRect(rcDot,m_clrDot);
				rcStep.OffsetRect(iEditWid+EDIT_DELTA,0);
			}
		} while (false);
	}

	void DUISize::OnSize(UINT nType, CSize size)
	{
		do 
		{
			__super::OnSize(nType,size);
			if (m_pEdit[0].isNull())
			{
				break;
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			int iEditWid = (rcClient.Width()-EDIT_DELTA)/2;
			if (iEditWid<=0)
			{
				break;
			}
			CStringW strAttr;
			int iLeft = 0;
			for (int i=0;i<2;i++)
			{
				strAttr.Format(L"%d,0,@%d,-0",iLeft,iEditWid);
				m_pEdit[i]->SetAttribute(XML_POS,strAttr,false);

				strAttr.Format(L"pbgra(%02x,%02x,%02x,%02x)",m_clrText.b,m_clrText.g,m_clrText.r,m_clrText.a);
				m_pEdit[i]->SetAttribute(DMAttr::DUIRichEditAttr::COLOR_clrtext,strAttr,false);

				strAttr.Format(L"pbgra(%02x,%02x,%02x,%02x)",m_clrCaret.b,m_clrCaret.g,m_clrCaret.r,m_clrCaret.a);
				m_pEdit[i]->OnAttrCuretClr(strAttr,false);

				strAttr.Format(L"%d",m_iCaretAniCount);
				m_pEdit[i]->OnAttrCuretAnimateCount(strAttr,false);

				iLeft += iEditWid;
				iLeft += EDIT_DELTA;
			}
		} while (false);
	}

	//---------------------------------------------------
	// Function Des: жиди
	DMCode DUISize::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = __super::DV_CreateChildWnds(XmlNode);
		wchar_t pWBuf[] = L"<edit textalign=\"center\" bautosel=\"1\"/>";
		int ulSize = 100;
		byte pBuf[100] = {0};
		UnicodeToUtf8(pWBuf,(PCHAR)pBuf,ulSize);
		DMXmlDocument doc;
		doc.LoadFromBuffer(pBuf, ulSize);
		DMXmlNode EditNode = doc.Root();
		for (int i=0;i<2;i++)
		{
			g_pDMApp->CreateRegObj((void**)&m_pEdit[i], L"dsizeedit",DMREG_Window);
			if (m_pEdit[i])
			{
				DM_InsertChild(m_pEdit[i]);
				m_pEdit[i]->AddRef();
				m_pEdit[i]->InitDMData(EditNode);
				m_pEdit[i]->SetLimitText(3);
			}
		}
		return iErr;
	}

	//
	DMCode DUISize::OnAttrTextColor(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			DMAttributeDispatch::ParseColor(pszValue,m_clrText);
			for (int i=0;i<2;i++)
			{
				if (m_pEdit[i])
				{
					m_pEdit[i]->SetAttribute(DMAttr::DUIRichEditAttr::COLOR_clrtext,pszValue,bLoadXml);
				}
			}
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUISize::OnAttrCuretClr(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			DMAttributeDispatch::ParseColor(pszValue,m_clrCaret);
			for (int i=0;i<2;i++)
			{
				if (m_pEdit[i])
				{
					m_pEdit[i]->OnAttrCuretClr(pszValue,bLoadXml);
				}
			}
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUISize::OnAttrCuretAnimateCount(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			DMAttributeDispatch::ParseInt(pszValue,m_iCaretAniCount);
			for (int i=0;i<2;i++)
			{
				if (m_pEdit[i])
				{
					m_pEdit[i]->OnAttrCuretAnimateCount(pszValue,bLoadXml);
				}
			}
		} while (false);
		return DM_ECODE_OK;
	}

}//namespace DM