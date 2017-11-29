#include "DmMainAfx.h"
#include "DUIStylePool.h"

namespace DM
{
	DMStylePoolItem::~DMStylePoolItem()
	{
		RemoveAll();
	}

	void DMStylePoolItem::PreMapKeyRemove(const IDMStylePtr &obj)
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

			CStringW strName = XmlNode.Attribute(L"name");strName.MakeLower();
			bool bExist = true;
			if (false == GetObjByKey(strName, pItem))
			{
				bExist  = false;
				pItem   = new DMStylePoolItem;
			}

			DMXmlNode XmlStyle = XmlNode.FirstChild();
			while (XmlStyle.IsValid())
			{
				CStringW strId =XmlStyle.Attribute(L"id");strId.MakeLower();
				if (strId.IsEmpty())
				{
					CStringW szInfo; 
					XmlStyle.GetXmlContent(szInfo);
					szInfo += L"(style)未设置id,将自动忽视";
					DMASSERT_EXPR(0, szInfo);
				}
				else
				{
					if (!pItem->IsKeyExist(strId))
					{// key不存在时才加入
						LPCWSTR lpszClassName = XmlStyle.GetName();
						IDMStylePtr pStylePtr = NULL;
						if (DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pStylePtr,lpszClassName,DMREG_Style)))
						{
							pStylePtr->InitDMData(XmlStyle);
							pItem->AddKey(strId,pStylePtr);
						}
					}
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
		CStringW szKey = lpszName;szKey.MakeLower();
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
			CStringW strName = lpszName;strName.MakeLower();
			CStringW strKey = lpszKey;  strKey.MakeLower();
			if (false == GetObjByKey(strName, pCur))
			{
				if (bLoopFind)
				{
					pStyle = FindStyleFromAll(strKey);
				}
				break;
			}
			else
			{
				if (pCur->GetObjByKey(strKey,pStyle))
				{
					break;// 已找到
				}

				if (bLoopFind)
				{
					pStyle = FindStyleFromAll(strKey);
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

			CStringW strKey = lpszKey;strKey.MakeLower();
			POSITION pos = m_Map.GetStartPosition();
			while (pos)
			{
				DM::CMap<CStringW,DMStylePoolItemPtr>::CPair *p = m_Map.GetNext(pos);
				DMStylePoolItemPtr &pCur = p->m_value;
				if (pCur->GetObjByKey(strKey,pStyle))
				{
					break;
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