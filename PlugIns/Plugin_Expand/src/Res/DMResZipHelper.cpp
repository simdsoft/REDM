#include "Plugin_ExpandAfx.h"
#include "DMResZipHelper.h"

namespace DM
{
	/// DMZipItem---------------------------------------
	DMZipItem::DMZipItem(LPCSTR lpszType, LPCSTR lpszName, LPCWSTR lpszPath)
	{
		memset(this,0,sizeof(DMZipItem));
		if (lpszType)
		{
			strcpy_s(m_szType,MAX_RES_TYPE,lpszType);
		}

		if (lpszName) 
		{
			strcpy_s(m_szName,MAX_RES_NAME,lpszName);
		}

		if (lpszPath)
		{
			wcscpy_s(m_szPath,MAX_PATH,lpszPath);
		}

		_strlwr_s(m_szType);
		_strlwr_s(m_szName);
		_wcslwr_s(m_szPath);
	}

	// DMZipItemArray-----------------------------------
	DMZipItemArray::~DMZipItemArray()
	{
		RemoveAll();
	}

	void DMZipItemArray::PreArrayObjRemove(const DMZipItemPtr &obj)
	{
		delete obj;
	}

	bool DMZipItemArray::EqualArrayObj(const DMZipItemPtr &objsrc, const DMZipItemPtr &objdest)
	{
		if (0 == dm_xmlstrcmp(objsrc->m_szType,objdest->m_szType)
			&& 0 == dm_xmlstrcmp(objsrc->m_szName,objdest->m_szName))
		{
			return true;
		}

		return false;
	}

}//namespace DM