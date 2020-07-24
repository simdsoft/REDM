#include "DmMainAfx.h"
#include "DMResHelper.h"

namespace DM
{
	/// DMResItem---------------------------------------
	DMResItem::DMResItem(LPCSTR lpszType, LPCSTR lpszName, LPCWSTR lpszPath)
	{
		memset(this,0,sizeof(DMResItem));
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

	// DMResFolderItem-----------------------------------
	DMResFolderItem::~DMResFolderItem()
	{
		RemoveAll();
	}

	void DMResFolderItem::PreArrayObjRemove(const DMResItemPtr &obj)
	{
		delete obj;
	}

	bool DMResFolderItem::EqualArrayObj(const DMResItemPtr &objsrc, const DMResItemPtr &objdest)
	{
		if (0 == dm_xmlstrcmp(objsrc->m_szType,objdest->m_szType)
			&& 0 == dm_xmlstrcmp(objsrc->m_szName,objdest->m_szName))
		{
			return true;
		}

		return false;
	}

}//namespace DM