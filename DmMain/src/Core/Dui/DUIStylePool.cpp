#include "DmMainAfx.h"
#include "DUIStylePool.h"

namespace DM
{

	DMStylePoolItem::DMStylePoolItem(CStringW strName,DMXmlNode &xmlNodes)
	{
		m_strName = strName;
		m_Doc.Base().InsertChildNode(L"root");
		AddStyles(xmlNodes);
	}

	void DMStylePoolItem::AddStyles(DMXmlNode &XmlStyles)
	{
		do 
		{
			if (!XmlStyles.IsValid())
			{
				break;
			}
			 
			DMXmlNode XmlNode = m_Doc.Root().InsertCopyChildNode(&XmlStyles);
			DMXmlNode XmlStyle = XmlNode.FirstChild();
			while (XmlStyle.IsValid())
			{
				CStringW strId = XmlStyle.Attribute(L"id");strId.MakeLower();
				if (strId.IsEmpty())
				{
					CStringW szInfo; 
					XmlStyle.GetXmlContent(szInfo);
					szInfo += L"(style)未设置id,将自动忽视";
					DMASSERT_EXPR(0, szInfo);
				}
				else
				{
					if (!IsKeyExist(strId))
					{// key不存在时才加入
						AddKey(strId, XmlStyle);
					}
				}
				XmlStyle = XmlStyle.NextSibling();
			}
		} while (false);
	}

	DMXmlNode DMStylePoolItem::GetStyle(CStringW strId)
	{
		DMXmlNode XmlStyle;
		GetObjByKey(strId, XmlStyle);
		return XmlStyle;
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
			if (false == GetObjByKey(strName, pItem))
			{
				pItem = new DMStylePoolItem(strName,XmlNode);
				AddKey(strName,pItem);
			}
			else
			{
				if (pItem)
				{
					pItem->AddStyles(XmlNode);
				}
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

	DMXmlNode DUIStylePool::FindStyle(LPCWSTR lpszKey,LPCWSTR lpszName,bool bLoopFind)
	{
		DMXmlNode XmlStyle;
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
					XmlStyle = FindStyleFromAll(strKey);
				}
				break;
			}
			else
			{
				if (pCur->GetObjByKey(strKey,XmlStyle))
				{
					break;// 已找到
				}

				if (bLoopFind)
				{
					XmlStyle = FindStyleFromAll(strKey);
				}
			}
		} while (false);
		return XmlStyle;
	}

	DMXmlNode DUIStylePool::FindStyle(LPCWSTR lpszBuf,bool bLoopFind)
	{
		DMXmlNode XmlStyle;
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
			int nCount = (int)SplitStringT(strValue,L':',strList);
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

			XmlStyle = FindStyle(strKey,strName,bLoopFind);
		} while (false);
		return XmlStyle;
	}

	DMXmlNode DUIStylePool::FindStyleFromAll(LPCWSTR lpszKey)
	{
		DMXmlNode XmlStyle;
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
				if (pCur->GetObjByKey(strKey,XmlStyle))
				{
					break;
				}
			}
		} while (false);
		return XmlStyle;
	}

	void DUIStylePool::PreMapKeyRemove(const DMStylePoolItemPtr &obj)
	{
		obj->Release();
	}
}//namespace DM