#include "DmMainAfx.h"
#include "DUIStylePool.h"

namespace DM
{
	DMStylePoolItem::~DMStylePoolItem()
	{
		RemoveAll();
	}

	void DMStylePoolItem::PreArrayObjRemove(const IDMStylePtr &obj)
	{
		obj->Release();
	}

	// ------------------------------DUIStylePool
	DUIStylePool::~DUIStylePool()
	{
		RemoveAll();
	}

	DMCode DUIStylePool::AddStylePoolItem(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		DMStylePoolItemPtr pItem = NULL;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}

			CStringW strName = XmlNode.Attribute(L"name");
			bool bExist = true;
			if (false == GetObjByKey(strName, pItem))
			{
				bExist  = false;
				pItem   = new DMStylePoolItem;
			}

			DMXmlNode XmlStyle = XmlNode.FirstChild();
			while (XmlStyle.IsValid())
			{
				LPCWSTR lpszClassName = XmlStyle.GetName();
				IDMStylePtr pStylePtr = NULL;
				if (DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pStylePtr,lpszClassName,DMREG_Style)))
				{
					pStylePtr->InitDMData(XmlStyle);
					pItem->AddObj(pStylePtr);
				}
				XmlStyle = XmlStyle.NextSibling();
			}

			if (false == bExist)
			{
				AddKey(strName,pItem);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUIStylePool::RemoveStylePoolItem(LPCWSTR lpszName)
	{
		CStringW szKey = lpszName;
		RemoveKey(szKey);
		return DM_ECODE_OK;
	}

	DMCode DUIStylePool::RemoveAllStylePoolItem()
	{
		RemoveAll();
		return DM_ECODE_OK;
	}

	IDMStylePtr DUIStylePool::FindStyle(LPCWSTR lpszKey,LPCWSTR lpszName,bool bLoopFind)
	{
		IDMStylePtr  pStyle = NULL;
		do 
		{
			if (NULL == lpszKey||wcslen(lpszKey)<=0)
			{
				break;
			}

			DMStylePoolItemPtr pCur = NULL;
			CStringW strName = lpszName;
			if (false == GetObjByKey(strName, pCur))
			{
				if (bLoopFind)
				{
					pStyle = FindStyleFromAll(lpszKey);
				}
				break;
			}
			else
			{
				int count = (int)pCur->m_DMArray.GetCount();
				wchar_t szId[MAX_PATH] = {0};
				for (int i=0; i<count; i++)
				{
					memset(szId,0,2*MAX_PATH);
					IDMStylePtr &pTemp = pCur->m_DMArray[i];
					pTemp->GetID(szId, MAX_PATH);
					if (0 == _wcsicmp(szId, lpszKey))
					{
						pStyle = pTemp;// 已找到
						break;
					}
				}

				if (pStyle)
				{
					break;// 已找到
				}

				if (bLoopFind)
				{
					pStyle = FindStyleFromAll(lpszKey);
				}
			}
		} while (false);
		return pStyle;
	}

	IDMStylePtr DUIStylePool::FindStyle(LPCWSTR lpszBuf,bool bLoopFind)
	{
		IDMStylePtr  pStyle = NULL;
		do 
		{
			if (NULL == lpszBuf||wcslen(lpszBuf)<=0)
			{
				break;
			}

			CStringW strValue = lpszBuf;
			CStringWList strList;
			CStringW strName;
			CStringW strKey;
			int nCount = SplitStringT(strValue,L':',strList);
			if (1==nCount)
			{
				strKey = strValue;
			}
			else if (2 == nCount)
			{
				strName = strList[0];
				strKey  = strList[1];
			}
			else
			{
				CStringW strInfo;
				strInfo.Format(L"style-%s设置错误",strValue);
				DMASSERT_EXPR(0,strInfo);
				break;
			}

			pStyle = FindStyle(strKey,strName,bLoopFind);
		} while (false);
		return pStyle;
	}

	IDMStylePtr DUIStylePool::FindStyleFromAll(LPCWSTR lpszKey)
	{
		IDMStylePtr  pStyle = NULL;
		do 
		{
			if (NULL == lpszKey||wcslen(lpszKey)<=0)
			{
				break;
			}

			POSITION pos = m_Map.GetStartPosition();
			while (pos)
			{
				DM::CMap<CStringW,DMStylePoolItemPtr>::CPair *p = m_Map.GetNext(pos);
				DMStylePoolItemPtr &pCur = p->m_value;
				int count = (int)pCur->m_DMArray.GetCount();
				wchar_t szId[MAX_PATH] = {0};
				for (int i=0; i<count; i++)
				{
					memset(szId,0,2*MAX_PATH);
					IDMStylePtr &pTemp = pCur->m_DMArray[i];
					pTemp->GetID(szId, MAX_PATH);
					if (0 == _wcsicmp(szId, lpszKey))
					{
						pStyle = pTemp;// 已找到
						break;
					}
				}
				if (pStyle)
				{
					break;// 已找到
				}
			}
		} while (false);
		return pStyle;
	}

	void DUIStylePool::PreMapKeyRemove(const DMStylePoolItemPtr &obj)
	{
		obj->Release();
	}
}//namespace DM