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
	pItem->SetAttribute(L"bassert",L"0");
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

DMCode ResMultFolder::IsItemExists(LPCWSTR lpszType, LPCWSTR lpszName,LPCWSTR lpszThemeName/*=NULL*/)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ResFolderPtr pItem = GetResFolderPtr(lpszName);
		iErr = pItem->IsItemExists(lpszType,lpszName, lpszThemeName);
	} while (false);
	return iErr;
}

DMCode ResMultFolder::GetItemSize(LPCWSTR lpszType, LPCWSTR lpszName, unsigned long& ulSize,LPCWSTR lpszThemeName/*=NULL*/)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ResFolderPtr pItem = GetResFolderPtr(lpszName);
		iErr = pItem->GetItemSize(lpszType,lpszName, ulSize,lpszThemeName);
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		CStringW strInfo;
		strInfo.Format(L"Res资源中%s:%s获取size失败",lpszType,lpszName);
		DMASSERT_EXPR(0,strInfo);
	}
	return iErr;
}

DMCode ResMultFolder::GetItemBuf(LPCWSTR lpszType, LPCWSTR lpszName, LPVOID lpBuf, unsigned long ulSize,LPCWSTR lpszThemeName/*=NULL*/)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		ResFolderPtr pItem = GetResFolderPtr(lpszName);
		iErr = pItem->GetItemBuf(lpszType,lpszName, lpBuf,ulSize,lpszThemeName);
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		CStringW strInfo;
		strInfo.Format(L"Res资源中%s:%s获取buf失败",lpszType,lpszName);
		DMASSERT_EXPR(0,strInfo);
	}
	return iErr;
}

DMCode ResMultFolder::LoadTheme(WPARAM wp, LPARAM lp)
{
	return DM_ECODE_OK;
}

DMCode ResMultFolder::SetCurTheme(LPCWSTR lpszName, LPCWSTR lpszOldName/*=NULL*/)
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

bool ResMultFolder::IsDMDesignerRes(CStringW strName)
{
	bool bRet = false;
	do 
	{
		if (strName.GetLength()<=3)
		{
			break;
		}
		if (strName.Left(3).CompareNoCase(L"ds_")
			||(g_pMainWnd && g_pMainWnd->m_pDesignerXml && 0 == g_pMainWnd->m_pDesignerXml->m_strGlobalName.CompareNoCase(L"ds_global"))//支持DesignerRes自身编辑
			)
		{
			break;
		}
		bRet = true;
	} while (false);
	return bRet;
}

ResFolderPtr ResMultFolder::GetResFolderPtr(CStringW strName)
{
	ResFolderPtr pItem = GetObj(0);
	if (!IsDMDesignerRes(strName))
	{
		if (2==(int)GetCount())
		{
			pItem = GetObj(1);
		}
	}
	return pItem;
}

