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

	CStringA DUIAccel::FormatHotkey()
	{
		if (0==m_wModifier&&0==m_wVK) return "nil";
		CStringA str;
		if (m_wModifier == HOTKEYF_ALT)		str ="Alt+";
		if (m_wModifier == HOTKEYF_CONTROL) str ="Ctrl+";
		if (m_wModifier ==  HOTKEYF_SHIFT)	str ="Shift+";
		if (m_wModifier == (HOTKEYF_ALT|HOTKEYF_SHIFT))		str ="Shift+Alt+";
		if (m_wModifier == (HOTKEYF_ALT|HOTKEYF_CONTROL))	str ="Ctrl+Alt+";
		if (m_wModifier == (HOTKEYF_CONTROL|HOTKEYF_SHIFT)) str ="Ctrl+Shift+";
		if (m_wModifier == (HOTKEYF_ALT|HOTKEYF_CONTROL|HOTKEYF_SHIFT)) str ="Ctrl+Shift+Alt+";
		str += GetKeyName(m_wVK);
		return str;
	}

	CStringA DUIAccel::GetKeyName(WORD vk)
	{
		CStringA str;
		switch(vk)
		{
		case VK_TAB:   str = "TAB";	break;
		case VK_ESCAPE:str = "ESC";	break;
		case VK_RETURN:str = "Enter";  break;
		case VK_UP:	   str = "Up";		break;
		case VK_DOWN:  str = "Down";	break;
		case VK_LEFT:  str = "Left";	break;
		case VK_RIGHT: str = "Right";	break;
		case VK_HOME:  str = "Home";	break;
		case VK_END:   str = "End";	break;
		case VK_PRIOR: str = "PageUp";	break;
		case VK_NEXT:  str = "PageDown";break;
		case VK_INSERT:str = "Insert"; break;
		case VK_DELETE:str = "Delete"; break;
		default:
			if ((vk>='0' && vk<='9')||(vk>='A' && vk<='Z')) str = (wchar_t)vk;
			else if (vk>=VK_NUMPAD0 && vk<=VK_NUMPAD9)      str.Format("Num %d",vk-VK_NUMPAD0);
			else if (vk==VK_MULTIPLY)		str="Num *";
			else if (vk==VK_ADD)			str="Num +";            
			else if (vk==VK_DECIMAL)		str="Num Del";            
			else if (vk>=VK_F1&&vk<=VK_F12) str.Format("F%d",vk-VK_F1+1);
			else
			{
				wchar_t c = MapVirtualKeyW(vk,2);
				switch (c)
				{
				case '-':
				case '=':
				case '[':
				case ']':
				case '\\':
				case ';':
				case '\'':
				case ',':
				case '.':
				case '/':
				case '`':
					str+=c;
					break;
				}
			}
			break;
		}
		return str;
	}

	// 将字符串翻译为加速键
	DWORD DUIAccel::TranslateAccelKey(LPCSTR lpszAccelKey)
	{
		if (NULL == lpszAccelKey||0==strlen(lpszAccelKey))
		{
			return 0;
		}

		WORD wModifier	   = 0;
		WORD wKey		   = 0;
		CStringA strValue = lpszAccelKey;
		strValue = strValue.MakeLower();
		CStringAList strKeyList;
		SplitStringT(strValue,'+',strKeyList);
		LPSTR pszKey =  NULL;
		int nCount = (int)strKeyList.GetCount();
		for (int i=0;i<nCount;i++)
		{
			strKeyList[i].Trim();
			pszKey = strKeyList[i].GetBuffer();
			if (_stricmp(pszKey,"ctrl")==0)       wModifier |= HOTKEYF_CONTROL;
			else if (_stricmp(pszKey,"alt")==0)   wModifier |= HOTKEYF_ALT;
			else if (_stricmp(pszKey,"shift")==0) wModifier |= HOTKEYF_SHIFT;
			else
			{
				if (_stricmp(pszKey,"esc")==0)			 wKey = VK_ESCAPE;
				else if (_stricmp(pszKey,"enter")==0)   wKey = VK_RETURN;
				else if (_stricmp(pszKey,"up")==0)		 wKey = VK_UP;
				else if (_stricmp(pszKey,"down")==0)	 wKey = VK_DOWN;
				else if (_stricmp(pszKey,"left")==0)	 wKey = VK_LEFT;
				else if (_stricmp(pszKey,"right")==0)   wKey = VK_RIGHT;
				else if (_stricmp(pszKey,"home")==0)	 wKey = VK_HOME;
				else if (_stricmp(pszKey,"pageup")==0)  wKey = VK_PRIOR;
				else if (_stricmp(pszKey,"pagedown")==0)wKey = VK_NEXT;
				else if (_stricmp(pszKey,"insert")==0)  wKey = VK_INSERT;
				else if (_stricmp(pszKey,"delete")==0)  wKey = VK_DELETE;
				else if ( strlen(pszKey)>1)
				{
					if (pszKey[0]=='f') wKey=VK_F1+atoi(pszKey+1)-1;//F1-F12
					else if (_strnicmp(pszKey,"num ",4)==0)
					{//Num 0 - Num 9 || Num Del
						if (_stricmp(pszKey+4,"del")==0)	wKey = VK_DECIMAL;
						else if (_stricmp(pszKey+4,"*")==0)wKey = VK_MULTIPLY;
						else if (_stricmp(pszKey+4,"+")==0)wKey = VK_ADD;
						else								wKey = VK_NUMPAD0+atoi(pszKey+4);
					}
				}
				else//if( wcslen(pszKey)==1)
				{
					if (pszKey[0]>='a' && pszKey[0]<='z') wKey = pszKey[0]-0x20;
					else if (pszKey[0]>=L'0' && pszKey[0]<='9')wKey = pszKey[0];
					else if (pszKey[0] == '-')	wKey = VK_OEM_MINUS;
					else if (pszKey[0] == '=')	wKey = VK_OEM_PLUS;
					else if (pszKey[0] == ',')	wKey = VK_OEM_COMMA;
					else if (pszKey[0] == '.')	wKey = VK_OEM_PERIOD;
					else if (pszKey[0] == ';')	wKey = VK_OEM_1;
					else if (pszKey[0] == '/')	wKey = VK_OEM_2;
					else if (pszKey[0] == '`')	wKey = VK_OEM_3;
					else if (pszKey[0] == '[')	wKey = VK_OEM_4;
					else if (pszKey[0] == '\\')wKey = VK_OEM_5;
					else if (pszKey[0] == ']')	wKey = VK_OEM_6;
					else if (pszKey[0] == '\'')wKey = VK_OEM_7;
				}
				//break;
			}
			strKeyList[i].ReleaseBuffer();
		}
		return MAKELONG(wKey,wModifier);
	}

}//namespace DM
