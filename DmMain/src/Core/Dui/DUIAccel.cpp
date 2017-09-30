#include "DmMainAfx.h"
#include "DUIAccel.h"

namespace DM
{
	/// DUIAccel --------------------------------------------------
	DUIAccel::DUIAccel(UINT vKey,bool bCtrl,bool bAlt,bool bShift):m_wVK(vKey),m_wModifier(0)
	{
		if (bCtrl)   m_wModifier |= HOTKEYF_CONTROL;
		if (bAlt)    m_wModifier |= HOTKEYF_ALT;
		if (bShift)  m_wModifier |= HOTKEYF_SHIFT;
	}

	DUIAccel::DUIAccel(DWORD dwAccel):m_wVK(LOWORD(dwAccel)),m_wModifier(HIWORD(dwAccel))
	{
	}

	DUIAccel::~DUIAccel(void)
	{
	}

	CStringW DUIAccel::FormatHotkey()
	{
		if (0==m_wModifier&&0==m_wVK) return L"无";
		CStringW str;
		if (m_wModifier == HOTKEYF_ALT)		str = L"Alt+";
		if (m_wModifier == HOTKEYF_CONTROL) str = L"Ctrl+";
		if (m_wModifier ==  HOTKEYF_SHIFT)	str = L"Shift+";
		if (m_wModifier == (HOTKEYF_ALT|HOTKEYF_SHIFT))		str = L"Shift+Alt+";
		if (m_wModifier == (HOTKEYF_ALT|HOTKEYF_CONTROL))	str = L"Ctrl+Alt+";
		if (m_wModifier == (HOTKEYF_CONTROL|HOTKEYF_SHIFT)) str = L"Ctrl+Shift+";
		if (m_wModifier == (HOTKEYF_ALT|HOTKEYF_CONTROL|HOTKEYF_SHIFT)) str = L"Ctrl+Shift+Alt+";
		str += GetKeyName(m_wVK);
		return str;
	}

	CStringW DUIAccel::GetKeyName(WORD vk)
	{
		CStringW str;
		switch(vk)
		{
		case VK_TAB:   str = L"TAB";	break;
		case VK_ESCAPE:str = L"ESC";	break;
		case VK_RETURN:str = L"Enter";  break;
		case VK_UP:	   str = L"Up";		break;
		case VK_DOWN:  str = L"Down";	break;
		case VK_LEFT:  str = L"Left";	break;
		case VK_RIGHT: str = L"Right";	break;
		case VK_HOME:  str = L"Home";	break;
		case VK_END:   str = L"End";	break;
		case VK_PRIOR: str = L"PageUp";	break;
		case VK_NEXT:  str = L"PageDown";break;
		case VK_INSERT:str = L"Insert"; break;
		case VK_DELETE:str = L"Delete"; break;
		default:
			if ((vk>='0' && vk<='9')||(vk>='A' && vk<='Z')) str = (wchar_t)vk;
			else if (vk>=VK_NUMPAD0 && vk<=VK_NUMPAD9)      str.Format(L"Num %d",vk-VK_NUMPAD0);
			else if (vk==VK_MULTIPLY)		str=L"Num *";
			else if (vk==VK_ADD)			str=L"Num +";            
			else if (vk==VK_DECIMAL)		str=L"Num Del";            
			else if (vk>=VK_F1&&vk<=VK_F12) str.Format(L"F%d",vk-VK_F1+1);
			else
			{
				wchar_t c = MapVirtualKeyW(vk,2);
				switch (c)
				{
				case L'-':
				case L'=':
				case L'[':
				case L']':
				case L'\\':
				case L';':
				case L'\'':
				case L',':
				case L'.':
				case L'/':
				case L'`':
					str+=c;
					break;
				}
			}
			break;
		}
		return str;
	}

	// 将字符串翻译为加速键
	DWORD DUIAccel::TranslateAccelKey(LPCWSTR lpszAccelKey)
	{
		if (NULL == lpszAccelKey||0==wcslen(lpszAccelKey))
		{
			return 0;
		}

		WORD wModifier	   = 0;
		WORD wKey		   = 0;
		CStringW strValue = lpszAccelKey;
		strValue = strValue.MakeLower();
		CStringWList strKeyList;
		SplitStringT(strValue,L'+',strKeyList);
		LPWSTR pszKey =  NULL;
		int nCount = strKeyList.GetCount();
		for (int i=0;i<nCount;i++)
		{
			strKeyList[i].Trim();
			pszKey = strKeyList[i].GetBuffer();
			if (_wcsicmp(pszKey,L"ctrl")==0)       wModifier |= HOTKEYF_CONTROL;
			else if (_wcsicmp(pszKey,L"alt")==0)   wModifier |= HOTKEYF_ALT;
			else if (_wcsicmp(pszKey,L"shift")==0) wModifier |= HOTKEYF_SHIFT;
			else
			{
				if (_wcsicmp(pszKey,L"esc")==0)			 wKey = VK_ESCAPE;
				else if (_wcsicmp(pszKey,L"enter")==0)   wKey = VK_RETURN;
				else if (_wcsicmp(pszKey,L"up")==0)		 wKey = VK_UP;
				else if (_wcsicmp(pszKey,L"down")==0)	 wKey = VK_DOWN;
				else if (_wcsicmp(pszKey,L"left")==0)	 wKey = VK_LEFT;
				else if (_wcsicmp(pszKey,L"right")==0)   wKey = VK_RIGHT;
				else if (_wcsicmp(pszKey,L"home")==0)	 wKey = VK_HOME;
				else if (_wcsicmp(pszKey,L"pageup")==0)  wKey = VK_PRIOR;
				else if (_wcsicmp(pszKey,L"pagedown")==0)wKey = VK_NEXT;
				else if (_wcsicmp(pszKey,L"insert")==0)  wKey = VK_INSERT;
				else if (_wcsicmp(pszKey,L"delete")==0)  wKey = VK_DELETE;
				else if ( wcslen(pszKey)>1)
				{
					if (pszKey[0]==L'f') wKey=VK_F1+_tstoi(pszKey+1)-1;//F1-F12
					else if (_wcsnicmp(pszKey,L"num ",4)==0)
					{//Num 0 - Num 9 || Num Del
						if (_wcsicmp(pszKey+4,L"del")==0)	wKey = VK_DECIMAL;
						else if (_wcsicmp(pszKey+4,L"*")==0)wKey = VK_MULTIPLY;
						else if (_wcsicmp(pszKey+4,L"+")==0)wKey = VK_ADD;
						else								wKey = VK_NUMPAD0+_tstoi(pszKey+4);
					}
				}
				else//if( wcslen(pszKey)==1)
				{
					if (pszKey[0]>=L'a' && pszKey[0]<=L'z') wKey = pszKey[0]-0x20;
					else if (pszKey[0]>=L'0' && pszKey[0]<=L'9')wKey = pszKey[0];
					else if (pszKey[0] == L'-')	wKey = VK_OEM_MINUS;
					else if (pszKey[0] == L'=')	wKey = VK_OEM_PLUS;
					else if (pszKey[0] == L',')	wKey = VK_OEM_COMMA;
					else if (pszKey[0] == L'.')	wKey = VK_OEM_PERIOD;
					else if (pszKey[0] == L';')	wKey = VK_OEM_1;
					else if (pszKey[0] == L'/')	wKey = VK_OEM_2;
					else if (pszKey[0] == L'`')	wKey = VK_OEM_3;
					else if (pszKey[0] == L'[')	wKey = VK_OEM_4;
					else if (pszKey[0] == L'\\')wKey = VK_OEM_5;
					else if (pszKey[0] == L']')	wKey = VK_OEM_6;
					else if (pszKey[0] == L'\'')wKey = VK_OEM_7;
				}
				//break;
			}
			strKeyList[i].ReleaseBuffer();
		}
		return MAKELONG(wKey,wModifier);
	}

}//namespace DM
