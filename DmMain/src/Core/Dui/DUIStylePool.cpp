#include "DmMainAfx.h"
#include "DUIStylePool.h"

namespace DM
{

	DMStylePoolItem::DMStylePoolItem(CStringA strName,DMXmlNode &xmlNodes)
	{
		m_strName = strName;
		m_Doc.Base().InsertChildNode("root");
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
				CStringA strId = XmlStyle.Attribute("id");strId.MakeLower();
				if (strId.IsEmpty())
				{
					CStringA szInfo; 
					XmlStyle.GetXmlContent(szInfo);
					szInfo += "(style)未设置id,将自动忽视";
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

	DMXmlNode DMStylePoolItem::GetStyle(CStringA strId)
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

			CStringA strName = XmlNode.Attribute("name");strName.MakeLower();
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

	DMCode DUIStylePool::RemoveStylePoolItem(LPCSTR lpszName)
	{
		CStringA szKey = lpszName;szKey.MakeLower();
		RemoveKey(szKey);
		return DM_ECODE_OK;
	}

	DMCode DUIStylePool::RemoveAllStylePoolItem()
	{
		RemoveAll();
		return DM_ECODE_OK;
	}

	DMXmlNode DUIStylePool::FindStyle(LPCSTR lpszKey,LPCSTR lpszName,bool bLoopFind)
	{
		DMXmlNode XmlStyle;
		do 
		{
			if (NULL == lpszKey||strlen(lpszKey)<=0)
			{
				break;
			}

			DMStylePoolItemPtr pCur = NULL;
			CStringA strName = lpszName;strName.MakeLower();
			CStringA strKey = lpszKey;  strKey.MakeLower();
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

	DMXmlNode DUIStylePool::FindStyle(LPCSTR lpszBuf,bool bLoopFind)
	{
		DMXmlNode XmlStyle;
		do 
		{
			if (NULL == lpszBuf||strlen(lpszBuf)<=0)
			{
				break;
			}

			CStringA strValue = lpszBuf;
			CStringAList strList;
			CStringA strName;
			CStringA strKey;
			int nCount = (int)SplitStringT(strValue,':',strList);
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
				CStringA strInfo;
				strInfo.Format("style-%s设置错误",strValue);
				DMASSERT_EXPR(0,strInfo);
				break;
			}

			XmlStyle = FindStyle(strKey,strName,bLoopFind);
		} while (false);
		return XmlStyle;
	}

	DMXmlNode DUIStylePool::FindStyleFromAll(LPCSTR lpszKey)
	{
		DMXmlNode XmlStyle;
		do 
		{
			if (NULL == lpszKey||strlen(lpszKey)<=0)
			{
				break;
			}

			CStringA strKey = lpszKey;strKey.MakeLower();
			POSITION pos = m_Map.GetStartPosition();
			while (pos)
			{
				DM::CMap<CStringA,DMStylePoolItemPtr>::CPair *p = m_Map.GetNext(pos);
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