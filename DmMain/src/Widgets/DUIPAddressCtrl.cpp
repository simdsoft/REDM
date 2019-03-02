#include "DmMainAfx.h"
#include "DUIPAddressCtrl.h"

namespace DM
{
	DUIPEdit::DUIPEdit()
	{
		m_byMin = 0;
		m_byMax = 255;
	}

	BYTE DUIPEdit::GetField()
	{
		if (0 == GetWindowTextLength())
		{
			return 0;
		}
		CStringW str = GetWindowText();
		return (BYTE)_wtoi(str);
	}

	void DUIPEdit::SetField(BYTE byField)
	{
		do 
		{
			if (byField<m_byMin||byField>m_byMax)
			{
				break;
			}
			CStringW str;
			str.Format(L"%u",byField);
			SetWindowText(str);
		} while (false);
	}

	void DUIPEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (VK_BACK==nChar)
			{
				if (0==GetWindowTextLength()&&!PUSH_ALT&&!PUSH_SHIFT&&!PUSH_CTRL)
				{
					DUIWindow* pPre = DM_GetWindow(GDW_PREVSIBLING);
					if (NULL!=pPre&&pPre->IsClass(DUIPEdit::GetClassName()))
					{
						pPre->DV_SetFocusWnd();
					}
				}
				break;
			}
			if (0 == isdigit(nChar))
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
				if (NULL!=pNext&&pNext->IsClass(DUIPEdit::GetClassName()))
				{
					pNext->DV_SetFocusWnd();
				}
			}

		} while (false);
	}

	bool DUIPEdit::AdjustNum()
	{
		bool bRet = true;
		do 
		{
			CStringW str = GetWindowText();
			int iValue = _wtoi(str);
			if (iValue > (int)m_byMax)
			{
				SetField(m_byMax);
				break;
			}
			if (iValue < (int)m_byMin)
			{
				SetField(m_byMin);
				break;
			}
			bRet = false;
		} while (false);
		return bRet;
	}

	//DUIPAddressCtrl-------------------------------------------------------------
#define  EDIT_DELTA           2
#define	 DMMAKEIPADDRESS(b1,b2,b3,b4)  ((LPARAM)(((DWORD)(b1)<<24)+((DWORD)(b2)<<16)+((DWORD)(b3)<<8)+((DWORD)(b4))))
	// Get individual number copy from CommCtrl.h
#define DMFIRST_IPADDRESS(x)  (((x) >> 24) & 0xff)
#define DMSECOND_IPADDRESS(x) (((x) >> 16) & 0xff)
#define DMTHIRD_IPADDRESS(x)  (((x) >> 8) & 0xff)
#define DMFOURTH_IPADDRESS(x) ((x) & 0xff)

	DUIPAddressCtrl::DUIPAddressCtrl()
	{
		m_clrDot = PBGRA(255,0,0,255);
	}

	BOOL DUIPAddressCtrl::IsBlank() const
	{
		BOOL bRet = FALSE;
		do 
		{
			if (0!=m_pEdit[0]->GetWindowTextLength()
				||0!=m_pEdit[1]->GetWindowTextLength()
				||0!=m_pEdit[2]->GetWindowTextLength()
				||0!=m_pEdit[3]->GetWindowTextLength()
				)
			{
				break;
			}
			bRet = TRUE;
		} while (FALSE);
		return bRet;
	}

	void DUIPAddressCtrl::ClearAddress()
	{
		for (int i=0;i<4;i++)
		{
			m_pEdit[i]->SetWindowText(L"");
		}
	}

	int DUIPAddressCtrl::GetAddress(BYTE& nField0, BYTE& nField1, BYTE& nField2, BYTE& nField3) const
	{
		nField0 = m_pEdit[0]->GetField();
		nField1 = m_pEdit[1]->GetField();
		nField2 = m_pEdit[2]->GetField();
		nField3 = m_pEdit[3]->GetField();
		int iNoBlank = 0;
		for (int i=0;i<4;i++)
		{
			if (0!=m_pEdit[i]->GetWindowTextLength())
			{
				iNoBlank++;
			}
		}
		return iNoBlank;
	}

	int DUIPAddressCtrl::GetAddress(DWORD& dwAddress) const
	{
		BYTE nField[4] = {0};
		int iNoBlank = GetAddress(nField[0],nField[1],nField[2],nField[3]);
		dwAddress = DMMAKEIPADDRESS(nField[0],nField[1],nField[2],nField[3]);
		return iNoBlank;
 	}

	void DUIPAddressCtrl::SetAddress(DWORD dwAddress)
	{
		BYTE nField0 = (BYTE) DMFIRST_IPADDRESS(dwAddress);
		BYTE nField1 = (BYTE) DMSECOND_IPADDRESS(dwAddress);
		BYTE nField2 = (BYTE) DMTHIRD_IPADDRESS(dwAddress);
		BYTE nField3 = (BYTE) DMFOURTH_IPADDRESS(dwAddress);
		SetAddress(nField0,nField1,nField2,nField3);
	}

	void DUIPAddressCtrl::SetAddress(BYTE nField0, BYTE nField1, BYTE nField2, BYTE nField3)
	{
		m_pEdit[0]->SetField(nField0);
		m_pEdit[1]->SetField(nField1);
		m_pEdit[2]->SetField(nField2);
		m_pEdit[3]->SetField(nField3);
	}

	void DUIPAddressCtrl::SetFieldFocus(WORD nField)
	{
		WORD Index = nField;
		if (Index<0||Index>=4)
		{
			Index = 0;
		}
		m_pEdit[Index]->DV_SetFocusWnd();
	}

	void DUIPAddressCtrl::SetFieldRange(int nField, BYTE nLower, BYTE nUpper)
	{
		do 
		{
			WORD Index = nField;
			if (Index<0||Index>=4)
			{
				break;
			}
			m_pEdit[Index]->m_byMin = nLower;
			m_pEdit[Index]->m_byMax = nUpper;
		} while (false);
		
	}

	void DUIPAddressCtrl::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			if (m_pEdit[0].isNull())
			{
				break;
			}

			CRect rcClient;
			DV_GetClientRect(rcClient);
			int iEditWid = (rcClient.Width()-EDIT_DELTA*3)/4;
			if (iEditWid<=0)
			{
				break;
			}
			CRect rcStep = rcClient;
			rcStep.left += iEditWid;
			rcStep.right = rcStep.left + EDIT_DELTA;
			for (int i=0;i<3;i++)
			{
				CRect rcDot = rcStep;
				rcDot.top += (rcStep.Height()-EDIT_DELTA)/2;
				rcDot.bottom = rcDot.top+EDIT_DELTA;
				pCanvas->FillSolidRect(rcDot,m_clrDot);
				rcStep.OffsetRect(iEditWid+EDIT_DELTA,0);
			}
		} while (false);
	}

	void DUIPAddressCtrl::OnSize(UINT nType, CSize size)
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
			int iEditWid = (rcClient.Width()-EDIT_DELTA*3)/4;
			if (iEditWid<=0)
			{
				break;
			}
			CStringW strAttr;
			int iLeft = 0;
			for (int i=0;i<4;i++)
			{
				strAttr.Format(L"%d,0,@%d,-0",iLeft,iEditWid);
				m_pEdit[i]->SetAttribute(L"pos",strAttr,false);
				iLeft += iEditWid;
				iLeft += EDIT_DELTA;
			}
		} while (false);
	}

	//---------------------------------------------------
	// Function Des: жиди
	DMCode DUIPAddressCtrl::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = __super::DV_CreateChildWnds(XmlNode);
		CStringW strWXml = L"<edit textalign=\"center\" clrtext=\"pbgra(00,00,00,ff)\" clrcaret=\"pbgra(00,00,00,ff)\" caretanimatecount=\"6\" align=\"left\" bautosel=\"1\"/>";
		CStringA strXml = DMW2A(strWXml,CP_UTF8);
		DMXmlDocument doc;
		doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
		DMXmlNode EditNode = doc.Root();
		CStringW strTextFont = XmlNode.Attribute(DMAttr::DUIPAddressCtrlAttr::FONT_font);
		CStringW strTextClr =  XmlNode.Attribute(DMAttr::DUIPAddressCtrlAttr::COLOR_clrtext);
		CStringW strCaretClr = XmlNode.Attribute(DMAttr::DUIPAddressCtrlAttr::COLOR_clrcaret);
		CStringW strCaretAniCount = XmlNode.Attribute(DMAttr::DUIPAddressCtrlAttr::INI_caretanimatecount);
		CStringW strAlign = XmlNode.Attribute(DMAttr::DUIPAddressCtrlAttr::OPTION_align);
		for (int i=0;i<4;i++)
		{
			g_pDMApp->CreateRegObj((void**)&m_pEdit[i], DUINAME_IPEdit,DMREG_Window);
			if (m_pEdit[i])
			{
				DM_InsertChild(m_pEdit[i]);
				m_pEdit[i]->AddRef();
				m_pEdit[i]->InitDMData(EditNode);
				m_pEdit[i]->SetLimitText(3);
				if (!strTextFont.IsEmpty())
				{
					m_pEdit[i]->SetAttribute(DMAttr::DUIRichEditAttr::FONT_font,strTextFont,false);
				}
				if (!strTextClr.IsEmpty())
				{
					m_pEdit[i]->SetAttribute(DMAttr::DUIRichEditAttr::COLOR_clrtext,strTextClr,false);
				}
				if (!strCaretClr.IsEmpty())
				{
					m_pEdit[i]->SetAttribute(DMAttr::DUIRichEditAttr::COLOR_clrcaret,strCaretClr,false);
				}
				if (!strCaretAniCount.IsEmpty())
				{
					m_pEdit[i]->SetAttribute(DMAttr::DUIRichEditAttr::INI_caretanimatecount,strCaretAniCount,false);
				}
				if (!strAlign.IsEmpty())
				{
					m_pEdit[i]->SetAttribute(DMAttr::DUIRichEditAttr::OPTION_align,strAlign,false);
				}
			}
		}
		return iErr;
	}

	//
	DMCode DUIPAddressCtrl::OnAttrTextColor(LPCWSTR pszValue, bool bLoadXml)
	{
		for (int i=0;i<4;i++)
		{
			if (m_pEdit[i])
			{
				m_pEdit[i]->SetAttribute(DMAttr::DUIRichEditAttr::COLOR_clrtext,pszValue,bLoadXml);
			}
		}
		return DM_ECODE_OK;
	}

	DMCode DUIPAddressCtrl::OnAttrCuretClr(LPCWSTR pszValue, bool bLoadXml)
	{
		for (int i=0;i<4;i++)
		{
			if (m_pEdit[i])
			{
				m_pEdit[i]->OnAttrCuretClr(pszValue,bLoadXml);
			}
		}
		return DM_ECODE_OK;
	}

	DMCode DUIPAddressCtrl::OnAttrTextFont(LPCWSTR pszValue, bool bLoadXml)
	{
		for (int i=0;i<4;i++)
		{
			if (m_pEdit[i])
			{
				m_pEdit[i]->OnAttrTextFont(pszValue,bLoadXml);
			}
		}
		return DM_ECODE_OK;
	}

	DMCode DUIPAddressCtrl::OnAttrCuretAnimateCount(LPCWSTR pszValue, bool bLoadXml)
	{
		for (int i=0;i<4;i++)
		{
			if (m_pEdit[i])
			{
				m_pEdit[i]->OnAttrCuretAnimateCount(pszValue,bLoadXml);
			}
		}
		return DM_ECODE_OK;
	}


	DMCode DUIPAddressCtrl::OnAttrAlign(LPCWSTR pszValue, bool bLoadXml)
	{
		for (int i=0;i<4;i++)
		{
			if (m_pEdit[i])
			{
				m_pEdit[i]->OnAttrAlign(pszValue,bLoadXml);
			}
		}
		return DM_ECODE_OK;
	}

}//namespace DM