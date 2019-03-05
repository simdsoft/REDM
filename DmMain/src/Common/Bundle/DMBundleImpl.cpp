#include "DmMainAfx.h"
#include "DMBundleImpl.h"

namespace DM
{

	DMBundleData::DMBundleData():m_dbType(DB_null)
	{
		ZeroMemory(&m_Value, sizeof(m_Value));
	}

	DMBundleData::DMBundleData(const DMBundleData &src):m_dbType(DB_null)
	{
		ZeroMemory(&m_Value, sizeof(m_Value));
		this->operator = (src);
	}

	DMBundleData& DMBundleData::operator=(const DMBundleData& src)
	{
		if (DB_null == m_dbType || m_dbType == src.m_dbType)
		{
			//enum{DB_null, DB_bool=1, DB_byte=2, DB_char=3, DB_dword=4, DB_double=5, DB_float=6, DB_int=7, DB_int64=8, DB_short=9, DB_string=10, DB_stringw=11, DB_size=12, DB_rect=13, DB_voidptr=14, DB_iunknown=15,DB_xmlnode=16};
			m_dbType = src.m_dbType;
			switch (m_dbType)
			{
			case DB_bool:     PutBool(src.m_Value.ubool);      break;
			case DB_byte:     PutByte(src.m_Value.ubyte);      break;
			case DB_char:     PutChar(src.m_Value.uchar);      break;
			case DB_dword:    PutDword(src.m_Value.udword);    break;
			case DB_double:	  PutDouble(src.m_Value.udouble);  break;
			case DB_float:	  PutFloat(src.m_Value.ufloat);	   break;
			case DB_int:	  PutInt(src.m_Value.uint);		   break;
			case DB_int64:	  PutInt64(src.m_Value.uint64);    break;
			case DB_short:	  PutShort(src.m_Value.ushort);	   break;
			case DB_string:   PutString(src.m_Value.ustring);  break;
			case DB_stringw:  PutStringW(src.m_Value.ustringw);break;
			case DB_size:	  PutSize(src.m_Value.usize);	   break;
			case DB_rect:	  PutRect(src.m_Value.urect);	   break;
			case DB_voidptr:  PutVoidPtr(src.m_Value.uvoidptr);break;
			case DB_iunknown: PutIUnknownPtr(src.m_Value.uiunknown);break;
			case DB_xmlnode:
				{
					DMXmlNode XmlNode;
					if (src.m_Value.uxml)
					{
						XmlNode = src.m_Value.uxml->Root();
					}
					PutXmlNode(XmlNode);
				}
				break;
			default:memcpy(&m_Value, &src.m_Value, sizeof(m_Value));break;
			}
		}
		return *this;
	}

	DMBundleData::~DMBundleData()
	{
		if (DB_string == m_dbType && m_Value.ustring) 
		{
			free((void*)m_Value.ustring);
		}
		if (DB_stringw == m_dbType && m_Value.ustringw) 
		{
			free((void*)m_Value.ustringw);
		}
		if (DB_iunknown == m_dbType && m_Value.uiunknown) 
		{
			m_Value.uiunknown->Release();
			m_Value.uiunknown = NULL;
		}
		if (DB_xmlnode == m_dbType && m_Value.uxml)
		{
			delete m_Value.uxml;
			m_Value.uxml = NULL;
		}
	}

	bool DMBundleData::PutBool(bool value)
	{
		if (DB_null == m_dbType || DB_bool == m_dbType)
		{
			m_Value.ubool = value;
			m_dbType = DB_bool;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutByte(byte value)
	{
		if (DB_null == m_dbType || DB_byte == m_dbType)
		{
			m_Value.ubyte = value;
			m_dbType = DB_byte;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutChar(char value)
	{
		if (DB_null == m_dbType || DB_char == m_dbType)
		{
			m_Value.uchar = value;
			m_dbType = DB_char;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutDword(DWORD value)
	{
		if (DB_null == m_dbType || DB_dword == m_dbType)
		{
			m_Value.udword = value;
			m_dbType = DB_dword;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutDouble(double value)
	{
		if (DB_null == m_dbType || DB_double == m_dbType)
		{
			m_Value.udouble = value;
			m_dbType = DB_double;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutFloat(float value)
	{
		if (DB_null == m_dbType || DB_float == m_dbType)
		{
			m_Value.ufloat = value;
			m_dbType = DB_float;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutInt(int value)
	{
		if (DB_null == m_dbType || DB_int == m_dbType)
		{
			m_Value.uint = value;
			m_dbType = DB_int;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutInt64(INT64 value)
	{
		if (DB_null == m_dbType || DB_int64 == m_dbType)
		{
			m_Value.uint64 = value;
			m_dbType = DB_int64;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutShort(short value)
	{
		if (DB_null == m_dbType || DB_short == m_dbType)
		{
			m_Value.ushort = value;
			m_dbType = DB_short;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutString(LPCSTR value)
	{
		if (DB_null == m_dbType || DB_string == m_dbType)
		{
			if (IsValidString(value)) 
			{
				if (m_Value.ustring) 
				{
					free((void*)m_Value.ustring);
					m_Value.ustring = NULL;
				}
				m_Value.ustring = _strdup(value);
			} 
			else 
			{
				m_Value.ustring = NULL;
			}
			m_dbType = DB_string;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutStringW(LPCWSTR value)
	{
		if (DB_null == m_dbType || DB_stringw == m_dbType) 
		{
			if (IsValidString(value))
			{
				if (m_Value.ustringw) 
				{
					free((void*)m_Value.ustringw);
					m_Value.ustringw = NULL;
				}
				m_Value.ustringw = _wcsdup(value);
			}
			else 
			{
				m_Value.ustringw = NULL;
			}
			m_dbType = DB_stringw;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutSize(CSize value)
	{
		if (DB_null == m_dbType || DB_size == m_dbType)
		{
			m_Value.usize = value;
			m_dbType = DB_size;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutRect(CRect value)
	{
		if (DB_null == m_dbType || DB_rect == m_dbType)
		{
			m_Value.urect = value;
			m_dbType = DB_rect;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutVoidPtr(void* value)
	{
		if (DB_null == m_dbType || DB_voidptr == m_dbType)
		{
			m_Value.uvoidptr = value;
			m_dbType = DB_voidptr;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutIUnknownPtr(IUnknown* value)
	{
		if (DB_null == m_dbType || DB_iunknown == m_dbType)
		{
			if (value)
			{
				value->AddRef();
			}
			if (m_Value.uiunknown)
			{
				m_Value.uiunknown->Release();
				m_Value.uiunknown = NULL;
			}
			m_Value.uiunknown = value;
			m_dbType = DB_iunknown;
			return true;
		}
		return false;
	}

	bool DMBundleData::PutXmlNode(DMXmlNode value)
	{
		if (DB_null == m_dbType || DB_xmlnode == m_dbType)
		{
			if (NULL == m_Value.uxml)
			{
				m_Value.uxml = new DMXmlDocument;
			}
			if (value.IsValid())
			{
				DMXmlNode XmlBase = m_Value.uxml->Base();
				XmlBase.InsertCopyChildNode(&value);
			}
			m_dbType = DB_xmlnode;
			return true;
		}
		return false;
	}

	bool DMBundleData::GetBool()
	{
		if (DB_bool == m_dbType)
		{
			return m_Value.ubool;
		}
		return false;
	}

	byte DMBundleData::GetByte()
	{
		if (DB_byte == m_dbType)
		{
			return m_Value.ubyte;
		}
		return 0;
	}

	char DMBundleData::GetChar()
	{
		if (DB_char == m_dbType)
		{
			return m_Value.uchar;
		}
		return 0;
	}

	DWORD DMBundleData::GetDword()
	{
		if (DB_dword == m_dbType)
		{
			return m_Value.udword;
		}
		return 0;
	}

	double DMBundleData::GetDouble()
	{
		if (DB_double == m_dbType)
		{
			return m_Value.udouble;
		}
		return 0.0;
	}

	float DMBundleData::GetFloat()
	{
		if (DB_float == m_dbType)
		{
			return m_Value.ufloat;
		}
		return 0.0f;
	}

	int DMBundleData::GetInt()
	{
		if (DB_int == m_dbType)
		{
			return m_Value.uint;
		}
		return 0;
	}

	INT64 DMBundleData::GetInt64()
	{
		if (DB_int64 == m_dbType)
		{
			return m_Value.uint64;
		}
		return 0;
	}

	short DMBundleData::GetShort()
	{
		if (DB_short == m_dbType)
		{
			return m_Value.ushort;
		}
		return 0;
	}

	LPCSTR DMBundleData::GetString()
	{
		if (DB_string == m_dbType) 
		{
			return NULL == m_Value.ustring ? "":m_Value.ustring;
		}
		return NULL;
	}

	LPCWSTR DMBundleData::GetStringW()
	{
		if (DB_stringw == m_dbType) 
		{
			return NULL == m_Value.ustringw ? L"":m_Value.ustringw;
		}
		return NULL;
	}

	CSize DMBundleData::GetSize()
	{
		if (DB_size == m_dbType)
		{
			return m_Value.usize;
		}
		return CSize();
	}

	CRect DMBundleData::GetRect()
	{
		if (DB_rect == m_dbType)
		{
			return m_Value.urect;
		}
		return CRect();
	}

	void* DMBundleData::GetVoidPtr()
	{
		if (DB_voidptr == m_dbType)
		{
			return m_Value.uvoidptr;
		}
		return NULL;
	}

	IUnknown* DMBundleData::GetIUnknownPtr()
	{
		if (DB_iunknown == m_dbType)
		{
			if (m_Value.uiunknown)
			{// addRef
				m_Value.uiunknown->AddRef();
			}
			return m_Value.uiunknown;
		}
		return NULL;
	}

	DMXmlNode DMBundleData::GetXmlNode()
	{
		DMXmlNode XmlNode;
		if (DB_xmlnode == m_dbType)
		{
			if (m_Value.uxml)
			{
				XmlNode = m_Value.uxml->Root();
			}
		}
		return XmlNode;
	}


	///DMBundleImpl ------------------------------------------------------------------------
	#define _DMBNUDLE_DEF_KEY_DEF			 L"DM!BD!DEF"
	DMBundleImpl& DMBundleImpl::operator=(const DMBundleImpl& src)
	{
		// ¸´ÖÆMap
		POSITION pos = src.m_Map.GetStartPosition();
		while(pos)
		{
			const DM::CMap<CStringW, DMBundleData>::CPair *p = src.m_Map.GetNext(pos);
			AddKey(p->m_key,p->m_value);
		}
		return *this;
	}

	DMBundleImpl::~DMBundleImpl()
	{
		RemoveAll();
	}

	void DMBundleImpl::Clear()
	{
		RemoveAll();
	}

	int DMBundleImpl::Size()
	{
		return (int)GetCount();
	}

	bool DMBundleImpl::Remove(LPCWSTR key)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return RemoveKey(key);
	}

	bool DMBundleImpl::HasKey(LPCWSTR key)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return IsKeyExist(key);
	}

	bool DMBundleImpl::PutBool(LPCWSTR key, bool value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutBool(value);
	}

	bool DMBundleImpl::PutByte(LPCWSTR key, byte value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutByte(value);
	}

	bool DMBundleImpl::PutChar(LPCWSTR key, char value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutChar(value);
	}

	bool DMBundleImpl::PutDword(LPCWSTR key, DWORD value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutDword(value);
	}

	bool DMBundleImpl::PutDouble(LPCWSTR key, double value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutDouble(value);
	}

	bool DMBundleImpl::PutFloat(LPCWSTR key, float value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutFloat(value);
	}

	bool DMBundleImpl::PutInt(LPCWSTR key, int value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutInt(value);
	}

	bool DMBundleImpl::PutInt64(LPCWSTR key, INT64 value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutInt64(value);
	}

	bool DMBundleImpl::PutShort(LPCWSTR key, short value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutShort(value);
	}

	bool DMBundleImpl::PutString(LPCWSTR key, LPCSTR value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutString(value);
	}

	bool DMBundleImpl::PutStringW(LPCWSTR key, LPCWSTR value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutStringW(value);
	}

	bool DMBundleImpl::PutSize(LPCWSTR key, CSize value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutSize(value);
	}

	bool DMBundleImpl::PutRect(LPCWSTR key, CRect value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutRect(value);
	}

	bool DMBundleImpl::PutVoidPtr(LPCWSTR key, void* value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutVoidPtr(value);
	}

	bool DMBundleImpl::PutIUnknownPtr(LPCWSTR key, IUnknown* value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutIUnknownPtr(value);
	}

	bool DMBundleImpl::PutXmlNode(LPCWSTR key, DMXmlNode value)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		return GetObjByKey(key).PutXmlNode(value);
	}

	bool DMBundleImpl::GetBool(LPCWSTR key, bool defaultValue/*=false*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_bool == anyValue.m_dbType ? anyValue.GetBool() : defaultValue;
		}
		return defaultValue;
	}

	byte DMBundleImpl::GetByte(LPCWSTR key, byte defaultValue/*=0*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_byte == anyValue.m_dbType ? anyValue.GetByte() : defaultValue;
		}
		return defaultValue;
	}

	char DMBundleImpl::GetChar(LPCWSTR key, char defaultValue/*=0*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_char == anyValue.m_dbType ? anyValue.GetChar() : defaultValue;
		}
		return defaultValue;
	}

	DWORD DMBundleImpl::GetDword(LPCWSTR key, DWORD defaultValue/*=0*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_dword == anyValue.m_dbType ? anyValue.GetDword() : defaultValue;
		}
		return defaultValue;
	}

	double DMBundleImpl::GetDouble(LPCWSTR key, double defaultValue/*=0*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_double == anyValue.m_dbType ? anyValue.GetDouble() : defaultValue;
		}
		return defaultValue;
	}

	float DMBundleImpl::GetFloat(LPCWSTR key, float defaultValue/*=0*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_float == anyValue.m_dbType ? anyValue.GetFloat() : defaultValue;
		}
		return defaultValue;
	}

	int DMBundleImpl::GetInt(LPCWSTR key, int defaultValue/*=0*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_int == anyValue.m_dbType ? anyValue.GetInt() : defaultValue;
		}
		return defaultValue;
	}

	INT64 DMBundleImpl::GetInt64(LPCWSTR key, INT64 defaultValue/*=0*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_int64 == anyValue.m_dbType ? anyValue.GetInt64() : defaultValue;
		}
		return defaultValue;
	}

	short DMBundleImpl::GetShort(LPCWSTR key, short defaultValue/*=0*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_short == anyValue.m_dbType ? anyValue.GetShort() : defaultValue;
		}
		return defaultValue;
	}

	LPCSTR DMBundleImpl::GetString(LPCWSTR key, LPCSTR defaultValue/*=""*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_string == anyValue.m_dbType ? anyValue.GetString() : defaultValue;
		}
		return defaultValue;
	}

	LPCWSTR DMBundleImpl::GetStringW(LPCWSTR key, LPCWSTR defaultValue/*=L""*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_stringw == anyValue.m_dbType ? anyValue.GetStringW() : defaultValue;
		}
		return defaultValue;
	}

	CSize DMBundleImpl::GetSize(LPCWSTR key, CSize defaultValue/*=CSize()*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_size == anyValue.m_dbType ? anyValue.GetSize() : defaultValue;
		}
		return defaultValue;
	}

	CRect DMBundleImpl::GetRect(LPCWSTR key, CRect defaultValue/*=CRect()*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_rect == anyValue.m_dbType ? anyValue.GetRect() : defaultValue;
		}
		return defaultValue;
	}

	void* DMBundleImpl::GetVoidPtr(LPCWSTR key, void* defaultValue/*=NULL*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_voidptr == anyValue.m_dbType ? anyValue.GetVoidPtr() : defaultValue;
		}
		return defaultValue;
	}

	IUnknown* DMBundleImpl::GetIUnknownPtr(LPCWSTR key, IUnknown* defaultValue/*=NULL*/)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			DMBundleData &anyValue = GetObjByKey(key);
			return DMBundleData::DB_iunknown == anyValue.m_dbType ? anyValue.GetIUnknownPtr() : defaultValue;
		}
		return defaultValue;
	}

	DMXmlNode DMBundleImpl::GetXmlNode(LPCWSTR key)
	{
		if (!IsValidString(key))
		{
			key = _DMBNUDLE_DEF_KEY_DEF;
		}
		if (IsKeyExist(key))
		{
			return GetObjByKey(key).GetXmlNode();
		}
		DMXmlNode  XmlNode;
		return XmlNode;
	}

}// namespace DM