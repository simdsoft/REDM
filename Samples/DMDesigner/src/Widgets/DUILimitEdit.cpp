#include "DMDesignerAfx.h"
#include "DUILimitEdit.h"

DUILimitEdit::DUILimitEdit()
{
	m_bNoCase = true;
}

void DUILimitEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	do  
	{
		if (VK_BACK==nChar)
		{
			break;
		}

		CStringW strChar = nChar;
		if (m_bNoCase)
		{
			strChar = strChar.MakeLower();
			m_strFirstIncludeChars = m_strFirstIncludeChars.MakeLower();
			m_strFirstExcludeChars = m_strFirstExcludeChars.MakeLower();
			m_strIncludeChars = m_strIncludeChars.MakeLower();
			m_strExcludeChars = m_strExcludeChars.MakeLower();
		}

		int nLen = GetWindowText().GetLength();
		if (0 == nLen)
		{
			if (!m_strFirstIncludeChars.IsEmpty())
			{
				if (strChar != strChar.SpanIncluding(m_strFirstIncludeChars))
				{
					break;
				}
			}

			if (!m_strFirstExcludeChars.IsEmpty())
			{
				if (strChar == strChar.SpanExcluding(m_strFirstExcludeChars))
				{
					break;
				}
			}
		}

		if (!m_strIncludeChars.IsEmpty())
		{
			if (strChar != strChar.SpanIncluding(m_strIncludeChars))
			{
				break;
			}
		}

		if (!m_strExcludeChars.IsEmpty())
		{
			if (strChar == strChar.SpanExcluding(m_strExcludeChars))
			{
				break;
			}
		}

		__super::OnChar(nChar,nRepCnt,nFlags);
	} while (false);
}

LRESULT DUILimitEdit::OnImeChar(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// For a Unicode window, this message is the same as WM_CHAR.
	OnChar((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16));
	return 0;
}

BOOL DUILimitEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (DV_IsFocusable()&&m_dwEditStyle&ES_NUMBER) // Êý×Ö
	{
		CStringW strValue = GetWindowText();
		int iValue = 0;
		dm_parseint(strValue,iValue);
		iValue += (zDelta>0?1:-1);
		strValue.Format(L"%d",iValue);
		SetWindowText(strValue);
		SetSel(MAKELONG(-1,-1));
	}
	else
	{
		SetMsgHandled(FALSE);
	}
	return TRUE;
}