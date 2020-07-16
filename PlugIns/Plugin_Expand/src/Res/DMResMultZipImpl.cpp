#include "Plugin_ExpandAfx.h"
#include "DMResMultZipImpl.h"

namespace DM
{
	DMResMultZipImpl::DMResMultZipImpl()
	{

	}

	DMResMultZipImpl::~DMResMultZipImpl()
	{
		RemoveAll();
	}

	DMCode DMResMultZipImpl::LoadResPack(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		DMResZipImplPtr pItem = new DMResZipImpl;
		pItem->SetAttribute("bassert","0");
		do 
		{
			if (!DMSUCCEEDED(pItem->LoadResPack(wp,lp)))
			{
			
				break;
			}

			pItem->m_strZipKey = (wchar_t*)(lp);
			if (pItem->m_strZipKey.IsEmpty())
			{
				DMASSERT_EXPR(FALSE,L"zip包的key不能为空");
				break;
			}

			if (DM_INVALID_VALUE != FindObj(pItem))
			{
				DMASSERT_EXPR(FALSE,L"zip包的key已存在");
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

	DMCode DMResMultZipImpl::IsItemExists(LPCSTR lpszType, LPCSTR lpszName, LPCSTR lpszThemeName/*=NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		int iCount = (int)GetCount();
		for (int i=0; i<iCount; i++)
		{
			DMResZipImplPtr pItem = NULL;
			if (GetObj(i, pItem))
			{
				iErr = pItem->IsItemExists(lpszType,lpszName, lpszThemeName);
				if (DMSUCCEEDED(iErr))
				{
					break;// 查找到一个就退出
				}
			}
		}
		return iErr;
	}

	DMCode DMResMultZipImpl::GetItemSize(LPCSTR lpszType, LPCSTR lpszName, unsigned long& ulSize, LPCSTR lpszThemeName/*=NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		int iCount = (int)GetCount();
		for (int i=0; i<iCount; i++)
		{
			DMResZipImplPtr pItem = NULL;
			if (GetObj(i, pItem))
			{
				iErr = pItem->GetItemSize(lpszType,lpszName, ulSize,lpszThemeName);
				if (DMSUCCEEDED(iErr))
				{
					break;// 查找到一个就退出
				}
			}
		}
		return iErr;
	}

	DMCode DMResMultZipImpl::GetItemBuf(LPCSTR lpszType, LPCSTR lpszName, DMBufT<byte>& lpBuf, PULONG lpULSize, LPCSTR lpszThemeName/*=NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		int iCount = (int)GetCount();
		for (int i=0; i<iCount; i++)
		{
			DMResZipImplPtr pItem = NULL;
			if (GetObj(i, pItem))
			{
				iErr = pItem->GetItemBuf(lpszType,lpszName, lpBuf, lpULSize,lpszThemeName);
				if (DMSUCCEEDED(iErr))
				{
					break;// 查找到一个就退出
				}
			}
		}
		return iErr;
	}

	DMCode DMResMultZipImpl::LoadTheme(WPARAM wp, LPARAM lp)
	{
		return DM_ECODE_OK;
	}

	DMCode DMResMultZipImpl::SetCurTheme(LPCSTR lpszName, LPCSTR lpszOldName/*=NULL*/)
	{
		return DM_ECODE_OK;
	}

	DMCode DMResMultZipImpl::SendExpandInfo(WPARAM wp, LPARAM lp)
	{
		if (NULL != lp)
		{
			wchar_t* szzipKey = (wchar_t*)lp;
			int iCount = (int)GetCount();
			for (int i=0; i<iCount; i++)
			{
				DMResZipImplPtr pItem = NULL;
				if (GetObj(i, pItem)&&0==_wcsicmp(pItem->m_strZipKey,szzipKey))
				{
					RemoveObj(i);
					break;
				}
			}
		}
	
		return DM_ECODE_OK;
	}

	void DMResMultZipImpl::PreArrayObjRemove( const DMResZipImplPtr &obj )
	{
		obj->Release();
	}

	bool DMResMultZipImpl::EqualArrayObj(const DMResZipImplPtr &objsrc, const DMResZipImplPtr &objdest)
	{
		if (0 == _wcsicmp(objsrc->m_strZipKey,objdest->m_strZipKey))
		{
			return true;
		}
		return false;
	}

}//namespace DM