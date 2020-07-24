#include "DMDesignerAfx.h" 
#include "ResHelper.h"

namespace DM
{
	/// ResItem---------------------------------------
	ResItem::ResItem(LPCSTR lpszType, LPCSTR lpszName, LPCWSTR lpszPath)
	{
		memset(this,0,sizeof(ResItem));
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

	// ResFolderItem-----------------------------------
	ResFolderItem::~ResFolderItem()
	{
		LOG_INFO("[start]m_Array-count:%d\n",GetCount());
		RemoveAll();
		LOG_INFO("[end]m_Array-count:%d\n",GetCount());
	}

	void ResFolderItem::PreArrayObjRemove(const ResItemPtr &obj)
	{
		delete obj;
	}

	bool ResFolderItem::EqualArrayObj(const ResItemPtr &objsrc, const ResItemPtr &objdest)
	{
		if (0 == dm_xmlstrcmp(objsrc->m_szType,objdest->m_szType)
			&& 0 == dm_xmlstrcmp(objsrc->m_szName,objdest->m_szName))
		{
			return true;
		}

		return false;
	}

}//namespace DM