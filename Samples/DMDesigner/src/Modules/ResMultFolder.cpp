#include "DMDesignerAfx.h"
#include "ResMultFolder.h"

ResMultFolder::ResMultFolder()
{
}

ResMultFolder::~ResMultFolder()
{
	RemoveAll();
}

DMCode ResMultFolder::LoadResPack(WPARAM wp, LPARAM lp)
{
	DMCode iErr = DM_ECODE_FAIL;
	ResFolderPtr pItem = new ResFolder;
	pItem->SetAttribute("bassert","0");
	do 
	{
		if (!DMSUCCEEDED(pItem->LoadResPack(wp,lp)))
		{
			break;
		}

		AddObj(pItem);
		iErr = DM_ECODE_OK;
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		DM_DELETE(pItem);
	}
	return iErr;
}

DMCode ResMultFolder::IsItemExists(LPCSTR lpszType, LPCSTR lpszName, LPCSTR lpszThemeName/*=NULL*/)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ResFolderPtr pItem = GetResFolderPtr(lpszName);
		iErr = pItem->IsItemExists(lpszType,lpszName, lpszThemeName);
	} while (false);
	return iErr;
}

DMCode ResMultFolder::GetItemSize(LPCSTR lpszType, LPCSTR lpszName, unsigned long& ulSize, LPCSTR lpszThemeName/*=NULL*/)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ResFolderPtr pItem = GetResFolderPtr(lpszName);
		iErr = pItem->GetItemSize(lpszType,lpszName, ulSize,lpszThemeName);
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		CStringA strInfo;
		strInfo.Format("Res资源中%s:%s获取size失败",lpszType,lpszName);
		DMASSERT_EXPR(0,strInfo);
	}
	return iErr;
}

LPCWSTR ResMultFolder::GetItemPath(LPCSTR lpszType, LPCSTR lpszName, LPCSTR lpszThemeName)
{
	ResFolderPtr pItem = GetResFolderPtr(lpszName);
	return pItem->GetItemPath(lpszType, lpszName, lpszThemeName);
}

DMCode ResMultFolder::GetItemBuf(LPCSTR lpszType, LPCSTR lpszName, DMBufT<byte>& pBuf, PULONG lpULSize, LPCSTR lpszThemeName)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ResFolderPtr pItem = GetResFolderPtr(lpszName);
		iErr = pItem->GetItemBuf(lpszType,lpszName, pBuf, lpULSize,lpszThemeName);
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		CStringA strInfo;
		strInfo.Format("Res资源中%s:%s获取buf失败",lpszType,lpszName);
		DMASSERT_EXPR(0,strInfo);
	}
	return iErr;
}

DMCode ResMultFolder::LoadTheme(WPARAM wp, LPARAM lp)
{
	return DM_ECODE_OK;
}

DMCode ResMultFolder::SetCurTheme(LPCSTR lpszName, LPCSTR lpszOldName/*=NULL*/)
{
	return DM_ECODE_OK;
}

DMCode ResMultFolder::SendExpandInfo(WPARAM wp, LPARAM lp)
{
	return DM_ECODE_OK;
}

void ResMultFolder::PreArrayObjRemove(const ResFolderPtr &obj)
{
	obj->Release();
}

bool ResMultFolder::IsDMDesignerRes(const char* pszName)
{
	return (strlen(pszName) > 3 && memcmp(pszName, "ds_", 3) == 0) &&
		// 支持编辑自身
		(!g_pMainWnd || !g_pMainWnd->m_pDesignerXml || g_pMainWnd->m_pDesignerXml->m_strGlobalName.CompareNoCase("ds_global") != 0);
}

ResFolderPtr ResMultFolder::GetResFolderPtr(const char* pszName)
{
	ResFolderPtr pItem = GetObj(0);
	if (!IsDMDesignerRes(pszName))
	{
		if (2==(int)GetCount())
		{
			pItem = GetObj(1);
		}
	}
	return pItem;
}

