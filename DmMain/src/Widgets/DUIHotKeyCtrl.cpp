#include "DmMainAfx.h"
#include "DUIHotKeyCtrl.h"
#include <CommCtrl.h>

namespace DM
{
#define CARET_OFFSET               2
	DUIHotKeyCtrl::DUIHotKeyCtrl()
	{
		m_bSetting = false;
		m_wInvalidModifiers = 0;
		m_wInvalidComb = HKCOMB_NONE;
		m_pDUIXmlInfo->m_bFocusable = true;
		m_clrCaret           = PBGRA(0,0,0,0xff);
		m_iCaretAniCount     = 6;
		m_pCaret             = NULL;
		SetAttribute(L"align",L"left",false);
	}

	void DUIHotKeyCtrl::SetHotKey(WORD wVirtualKeyCode, WORD wModifiers)
	{
		m_wVK = wVirtualKeyCode;
		m_wModifier = wModifiers;
		UpdateCaret();
		DM_Invalidate();
	}

	DWORD DUIHotKeyCtrl::GetHotKey() const
	{
		return MAKELONG(m_wVK, m_wModifier);
	}

	void DUIHotKeyCtrl::GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const
	{
		wVirtualKeyCode = m_wVK;
		wModifiers = m_wModifier;
	}

	static const wchar_t szPlus[] = L" + ";
	CStringW DUIHotKeyCtrl::GetHotKeyName() const
	{
		CStringW strKeyName;
		WORD wCode;
		WORD wModifiers;

		GetHotKey(wCode, wModifiers);
		if (wCode != 0 || wModifiers != 0)
		{
			if (wModifiers & HOTKEYF_CONTROL)
			{
				strKeyName += GetKeyName(VK_CONTROL, FALSE);
				strKeyName += szPlus;
			}

			if (wModifiers & HOTKEYF_SHIFT)
			{
				strKeyName += GetKeyName(VK_SHIFT, FALSE);
				strKeyName += szPlus;
			}

			if (wModifiers & HOTKEYF_ALT)
			{
				strKeyName += GetKeyName(VK_MENU, FALSE);
				strKeyName += szPlus;
			}

			strKeyName += GetKeyName(wCode, wModifiers & HOTKEYF_EXT);
		}

		return strKeyName;
	}

	void DUIHotKeyCtrl::SetRules(WORD wInvalidComb, WORD wModifiers)
	{
		m_wInvalidComb = wInvalidComb;
		m_wInvalidModifiers = wModifiers;
	}

	CStringW DUIHotKeyCtrl::GetKeyName(UINT vk, BOOL fExtended)
	{
		LONG lScan = MapVirtualKey(vk, 0) << 16;

		// if it's an extended key, add the extended flag
		if (fExtended)
			lScan |= 0x01000000L;

		CStringW str;
		int nBufferLen = 64;
		int nLen;
		do
		{
			nBufferLen *= 2;
			LPTSTR psz = str.GetBufferSetLength(nBufferLen);
			nLen = ::GetKeyNameText(lScan, psz, nBufferLen + 1);
			str.ReleaseBuffer(nLen);
		}
		while (nLen == nBufferLen);
		return str;
	}

	void DUIHotKeyCtrl::DM_OnPaint(IDMCanvas* pCanvas)
	{
		DUIDrawEnviron DrawEnviron;
		DV_PushDrawEnviron(pCanvas, DrawEnviron);

		CRect rcText;
		DV_GetClientRect(&rcText);
		
		CStringW strText = FormatHotkey();
		UINT ulAlgin = 0;
		m_pDUIXmlInfo->m_pStyle->GetTextAlign(ulAlgin);
		if (ulAlgin&DT_CENTER)
		{
			ulAlgin = DT_CENTER;
		}
		else if (ulAlgin&DT_RIGHT)
		{
			ulAlgin = DT_RIGHT;
			rcText.OffsetRect(-CARET_OFFSET*2,0);
		}
		else
		{
			rcText.OffsetRect(CARET_OFFSET,0);
		}
		DV_DrawText(pCanvas, strText, strText.GetLength(),rcText, ulAlgin|DT_VCENTER|DT_SINGLELINE);
		DV_PopDrawEnviron(pCanvas, DrawEnviron);
	}

	void DUIHotKeyCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (false == m_bSetting)
			{
				m_bSetting = true;
				m_wVK = 0;
				m_wModifier = m_wInvalidModifiers;
			}
			CStringW strKey = DUIAccel::GetKeyName(nChar);
			if (!strKey.IsEmpty())///< 有效键值
			{
				m_wVK = nChar;
			}
			UpdateModifier();
			UpdateCaret();
			DM_Invalidate();
		} while (false);
	}

	void DUIHotKeyCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (false == m_bSetting)
			{
				break;
			}
			if (nChar == m_wVK)
			{
				m_bSetting = false;
				break;
			}
			if (0 == m_wVK && !PUSH_SHIFT && !PUSH_ALT && !PUSH_CTRL)
			{
				m_bSetting = false;
				UpdateModifier();
				UpdateCaret();
				DM_Invalidate();
			}
			else if (VK_SHIFT == nChar||VK_MENU == nChar||VK_CONTROL == nChar)
			{
				UpdateModifier();
				UpdateCaret();
				DM_Invalidate();
			}
		} while (false);
	}

	void DUIHotKeyCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (PUSH_ALT)
			{
				OnKeyDown(nChar,nRepCnt,nFlags);
				break;
			}
			SetMsgHandled(FALSE);
		} while (false);
	}

	void DUIHotKeyCtrl::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		do 
		{
			if (VK_MENU==nChar||PUSH_ALT)
			{
				OnKeyUp(nChar,nRepCnt,nFlags);
				break;
			}
			SetMsgHandled(FALSE);
		} while (false);
	}

	void DUIHotKeyCtrl::DM_OnSetFocus()
	{
		UpdateCaret();
		m_pCaret->Show();
	}

	void DUIHotKeyCtrl::DM_OnKillFocus()
	{
		m_pCaret->Hide();
	}

	// 重载-----------------------------------------------
	DMCode DUIHotKeyCtrl::DV_CreateChildWnds(DMXmlNode &XmlNode)
	{
		DMCode iErr = __super::DV_CreateChildWnds(XmlNode);
		if (NULL == m_pCaret)
		{
			m_pCaret = new DUICaret();
			m_pCaret->m_clr = m_clrCaret;
			m_pCaret->SetAniCount(m_iCaretAniCount);
		}
		DM_InsertChild(m_pCaret);
		CStringW strValue = L"0,0,@0,@0";
		m_pCaret->SetAttribute(L"pos",strValue,true);
		m_pCaret->DM_SetVisible(false,false);
		return iErr;
	}

	// 辅助-----------------------------------------------
	void DUIHotKeyCtrl::UpdateModifier()
	{
		WORD wCombKey = 0;
		bool bAlt	  = PUSH_ALT>0;
		bool bCtrl    = PUSH_CTRL>0;
		bool bShift   = PUSH_SHIFT>0;

		if (!bAlt && !bCtrl && !bShift)
		{
			 wCombKey = HKCOMB_NONE,m_wModifier = 0;
		}
		else if (bAlt && !bCtrl && !bShift)
		{
			wCombKey = HKCOMB_A,m_wModifier = HOTKEYF_ALT;
		}
		else if (!bAlt && bCtrl && !bShift)
		{
			wCombKey = HKCOMB_C,m_wModifier = HOTKEYF_CONTROL;
		}
		else if (!bAlt && !bCtrl && bShift)
		{
			 wCombKey = HKCOMB_S,m_wModifier = HOTKEYF_SHIFT;
		}
		else if (bAlt && bCtrl && !bShift)
		{
			wCombKey = HKCOMB_CA,m_wModifier = HOTKEYF_ALT|HOTKEYF_CONTROL;
		}
		else if (bAlt && !bCtrl && bShift)
		{
			 wCombKey = HKCOMB_SA,m_wModifier = HOTKEYF_SHIFT|HOTKEYF_ALT;
		}
		else if (!bAlt && bCtrl && bShift)
		{
			wCombKey = HKCOMB_SC,m_wModifier = HOTKEYF_SHIFT|HOTKEYF_CONTROL;
		}
		else 
		{
			wCombKey = HKCOMB_SCA,m_wModifier = HOTKEYF_ALT|HOTKEYF_SHIFT|HOTKEYF_CONTROL;
		}
		if (wCombKey&m_wInvalidComb)
		{
			m_wModifier = m_wInvalidModifiers;
		}
	}

	void DUIHotKeyCtrl::UpdateCaret()
	{
		// 计算文本大小
		CStringW strText = FormatHotkey();
		CSize szFont;
		IDMCanvas *pCanvas = DM_GetCanvas(m_rcWindow,DMOLEDC_NODRAW);
		DUIDrawEnviron DrawEnviron;
		DV_PushDrawEnviron(pCanvas, DrawEnviron);
		pCanvas->MeasureText(DMTR(strText),-1,&szFont);

		m_pCaret->SetSize(CSize(1,szFont.cy));

		CPoint point;
		UINT ulAlgin = 0;
		m_pDUIXmlInfo->m_pStyle->GetTextAlign(ulAlgin);
		CRect rcClient;
		DV_GetClientRect(rcClient);
		if (ulAlgin&DT_CENTER)
		{
			point.SetPoint(szFont.cx+(rcClient.Width()-szFont.cx)/2+CARET_OFFSET,(rcClient.Height()-szFont.cy)/2);
		}
		else if (ulAlgin&DT_RIGHT)
		{
			point.SetPoint(rcClient.Width()-CARET_OFFSET,(rcClient.Height()-szFont.cy)/2);
		}
		else// left
		{
			point.SetPoint(szFont.cx+CARET_OFFSET,(rcClient.Height()-szFont.cy)/2);
		}
		if (point.y<0)
		{
			point.y = 0;
		}
		m_pCaret->SetPos(point);

		DV_PopDrawEnviron(pCanvas, DrawEnviron);
		DM_ReleaseCanvas(pCanvas);
	}

	DMCode DUIHotKeyCtrl::OnAttrCuretClr(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			DMAttributeDispatch::ParseColor(pszValue,m_clrCaret);
			if (NULL == m_pCaret)// 初化时光标是在create后
			{
				break;	
			}
			m_pCaret->m_clr = m_clrCaret;
		} while (false);
		return DM_ECODE_OK;
	}

	DMCode DUIHotKeyCtrl::OnAttrCuretAnimateCount(LPCWSTR pszValue, bool bLoadXml)
	{
		do 
		{
			DMAttributeDispatch::ParseInt(pszValue,m_iCaretAniCount);
			if (NULL == m_pCaret)// 初化时光标是在create后
			{
				break;	
			}
			m_pCaret->SetAniCount(m_iCaretAniCount);
		} while (false);
		return DM_ECODE_OK;
	}


}//namespace DM