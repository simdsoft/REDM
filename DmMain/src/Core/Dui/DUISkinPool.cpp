#include "DmMainAfx.h"
#include "DUISkinPool.h"

namespace DM
{
	DMSkinPoolItem::~DMSkinPoolItem()
	{
		RemoveAll();
	}

	void DMSkinPoolItem::PreMapKeyRemove(const IDMSkinPtr &obj)
	{
		obj->Release();
	}

	// ------------------------------DUISkinPool
	DUISkinPool::~DUISkinPool()
	{
		RemoveAll();
	}

	DMCode DUISkinPool::AddSkinPoolItem(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_FAIL;
		DMSkinPoolItemPtr pItem = NULL;
		do 
		{
			if (!XmlNode.IsValid())
			{
				break;
			}

			CStringA strName = XmlNode.Attribute("name");strName.MakeLower();
			bool bExist = true;
			if (false == GetObjByKey(strName, pItem))
			{
				bExist  = false;
				pItem   = new DMSkinPoolItem;
			}

			DMXmlNode XmlSkin = XmlNode.FirstChild();
			while (XmlSkin.IsValid())
			{
				CStringA strId = XmlSkin.Attribute("id");strId.MakeLower();
				if (strId.IsEmpty())
				{
					CStringA szInfo; 
					XmlSkin.GetXmlContent(szInfo);
					szInfo += "(skin)未设置id,将自动忽视";
					DMASSERT_EXPR(0, szInfo);
				}
				else
				{
					if (!pItem->IsKeyExist(strId))
					{// key不存在时才加入
						LPCSTR lpszClassName = XmlSkin.GetName();  
						IDMSkinPtr pSkinPtr = NULL;
						if (DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pSkinPtr,lpszClassName,DMREG_Skin)))
						{
							pSkinPtr->InitDMData(XmlSkin);
							if (!DMSUCCEEDED(pSkinPtr->IsValid()))
							{
								CStringA szInfo = lpszClassName;
								szInfo += "(skin)无效！,将自动移除";
								DMASSERT_EXPR(0, szInfo);
								pSkinPtr->Release();
							}
							else
							{
								pItem->AddKey(strId,pSkinPtr);
							}
						}
						else
						{
							CStringA szInfo = lpszClassName;
							szInfo += "(skinpool)类型无法解析！,请查看是否写错";
							DMASSERT_EXPR(0, szInfo);
						}
					}
				}
				XmlSkin = XmlSkin.NextSibling();
			}

			if (false == bExist)
			{
				AddKey(strName,pItem);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUISkinPool::RemoveSkinPoolItem(LPCSTR lpszName)
	{
		CStringA szKey = lpszName;szKey.MakeLower();
		RemoveKey(szKey);
		return DM_ECODE_OK;
	}

	DMCode DUISkinPool::RemoveAllSkinPoolItemExcept(LPCSTR lpszName)
	{
		DMSkinPoolItemPtr pItem = NULL;
		CStringA strKey = lpszName;strKey.MakeLower();
		if (GetObjByKey(strKey, pItem))
		{
			pItem->AddRef();// 防止被删除
		}
		RemoveAll();// 此处pItem减1,但不会释放 
		if (pItem)
		{
			AddKey(strKey,pItem);
		}
		return DM_ECODE_OK;
	}

	DMCode DUISkinPool::UpdateSkin(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			LPCSTR lpSrc = (LPCSTR)wp;
			int nLen = (int)lp;
			if (NULL == lpSrc||0>=nLen)
			{
				break;
			}

			// halx99: optimized, no need copy buffer
			CStringA strValue = lpSrc;
			CStringAList strUpdateList;
			int nCount = (int)SplitStringT(strValue,';',strUpdateList);
			// 1. 更新所有需更新的skin
			m_UpdateSkinArray.RemoveAll();
			for (int i=0;i<nCount;i++)
			{
				LPCSTR lpszUpdate = strUpdateList[i];
				POSITION pos = m_Map.GetStartPosition();
				while (pos)
				{
					DM::CMap<CStringA,DMSkinPoolItemPtr>::CPair *p = m_Map.GetNext(pos);
					DMSkinPoolItemPtr &pCur = p->m_value;
					POSITION poscur = pCur->m_Map.GetStartPosition();
					while(poscur)
					{
						DM::CMap<CStringA,IDMSkinPtr>::CPair *pcur = pCur->m_Map.GetNext(poscur);
						IDMSkinPtr &pTemp = pcur->m_value;
						if (DMSUCCEEDED(pTemp->UpdateSkin((WPARAM)lpszUpdate,0)))
						{
							m_UpdateSkinArray.Add(pTemp);
						}
					}
				}
			}
			// 2.通知所有使用这些skin的DUI窗口刷新自己区域
			iErr = g_pDMDWndPool->UpdateSkin(0,DMREG_Skin);

		} while (false);
		return iErr;
	}

	bool DUISkinPool::IsNeedUpdateSkin(IDMSkinPtr pSkin)
	{
		bool bRet = false;
		do 
		{
			int nCount = (int)m_UpdateSkinArray.GetCount();
			for (int i=0;i<nCount;i++)
			{
				if (pSkin == m_UpdateSkinArray[i])
				{
					bRet = true;
					break;// 跳出
				}
			}
		} while (false);
		return bRet;
	}

	DMCode DUISkinPool::ClearUpdateSkinArray()
	{
		m_UpdateSkinArray.RemoveAll();
		return DM_ECODE_OK;
	}

	DMCode DUISkinPool::AddSkin(void *pBuf,size_t bufLen,LPCSTR pszType,
		LPCSTR lpszXml,LPCSTR lpszPoolName/*=NULL*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pBuf
				||0>=bufLen
				||NULL == lpszXml)
			{
				break;
			}

			//1. 加载xml，如L"<imglist id=\"1\" states=\"1\" />";
			//CStringW strWXml(lpszXml);
			//CStringA strXml = DMW2A(strWXml,CP_UTF8);
			DMXmlDocument doc;
			if (false == doc.LoadFromBuffer((const PVOID)(LPCSTR)lpszXml, strlen(lpszXml)))
			{
				break;
			}
			DMXmlNode XmlNode = doc.Root();
			if (!XmlNode.IsValid())
			{
				break;
			}

			//2.判断skin的id是否为空
			LPCSTR strId = XmlNode.Attribute("id");
			if (!*strId)
			{
				CStringA szInfo; 
				XmlNode.GetXmlContent(szInfo);
				szInfo += "(skin)未设置id,将自动忽视";
				DMASSERT_EXPR(0, szInfo);
				break;
			}

			//3.判断skin的id是否存在
			if (FindSkinFromAll(strId))				// 默认从所有skin池中查找
			{
				CStringA szInfo = strId;
				szInfo += "(skinpool)此skin名已存在";
				DMASSERT_EXPR(0, szInfo);
				break;// 名字已存在
			}

			//4.创建skin对象
			LPCSTR lpszClassName = XmlNode.GetName();
			IDMSkinPtr pSkinPtr = NULL;
			if (!DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pSkinPtr,lpszClassName,DMREG_Skin)))
			{
				CStringA szInfo = lpszClassName;
				szInfo += "(skinpool)类型无法解析！,请查看是否写错";
				DMASSERT_EXPR(0, szInfo);
				break;
			}
			pSkinPtr->InitDMData(XmlNode);

			//5.初始化skin的图元
			CStringA strType = (NULL==pszType)?"png":pszType;
			if (!DMSUCCEEDED(pSkinPtr->SetBitmap((LPBYTE)pBuf,bufLen,strType)))
			{
				pSkinPtr->Release();
				break;
			}

			//6.判断skinpool是否存在，不存在则创建加入
			bool bExist = true;
			CStringA strName = lpszPoolName;strName.MakeLower();
			DMSkinPoolItemPtr pItem = NULL;
			if (false == GetObjByKey(strName, pItem))
			{
				bExist  = false;
				pItem   = new DMSkinPoolItem;
			}
			
			pItem->AddKey(strId,pSkinPtr);
			if (false == bExist)
			{
				AddKey(strName,pItem);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUISkinPool::RemoveSkin(LPCSTR lpszKey,LPCSTR lpszName,bool bLoopFind /*= true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszKey||strlen(lpszKey)<=0)
			{
				break;
			}

			DMSkinPoolItemPtr pCur = NULL;
			CStringA strName = lpszName;strName.MakeLower();
			CStringA strKey = lpszKey;  strKey.MakeLower();
			if (false == GetObjByKey(strName, pCur))
			{
				if (bLoopFind)
				{
					iErr = RemoveSkinFromAll(strKey);
				}
				break;
			}
			else
			{
				if (!pCur->RemoveKey(strKey))
				{
					if (bLoopFind)
					{
						iErr = RemoveSkinFromAll(strKey);
					}
					break;
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUISkinPool::RemoveSkinFromAll(LPCSTR lpszKey)
	{
		DMCode iErr = DM_ECODE_FAIL;
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
				DM::CMap<CStringA,DMSkinPoolItemPtr>::CPair *p = m_Map.GetNext(pos);
				DMSkinPoolItemPtr &pCur = p->m_value;
				if (pCur->RemoveKey(strKey))
				{
					iErr = DM_ECODE_OK;
				}
			}
		} while (false);
		return iErr;
	}

	IDMSkinPtr DUISkinPool::FindSkin(LPCSTR lpszKey,LPCSTR lpszName,bool bLoopFind)
	{
		IDMSkinPtr  pSkin = NULL;
		do 
		{
			if (NULL == lpszKey||strlen(lpszKey)<=0)
			{
				break;
			}

			DMSkinPoolItemPtr pCur = NULL;
			CStringA strName = lpszName;strName.MakeLower();
			CStringA strKey = lpszKey;  strKey.MakeLower();
			if (false == GetObjByKey(strName, pCur))
			{
				if (bLoopFind)
				{
					pSkin = FindSkinFromAll(strKey);
				}
				break;
			}
			else
			{
				if (pCur->GetObjByKey(strKey,pSkin))
				{
					break;// 已找到
				}

				if (bLoopFind)
				{
					pSkin = FindSkinFromAll(strKey);
				}
			}
		} while (false);
		return pSkin;
	}

	IDMSkinPtr DUISkinPool::FindSkin(LPCSTR lpszBuf,bool bLoopFind/* = true*/)
	{
		IDMSkinPtr  pSkin = NULL;
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
				CStringA strInfo;
				strInfo.Format("skin-%s设置错误", (LPCSTR)strValue);
				DMASSERT_EXPR(0,strInfo);
				break;
			}

			pSkin = FindSkin(strKey,strName,bLoopFind);
		} while (false);
		return pSkin;
	}

	IDMSkinPtr DUISkinPool::FindSkinFromAll(LPCSTR lpszKey)
	{
		IDMSkinPtr  pSkin = NULL;
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
				DM::CMap<CStringA,DMSkinPoolItemPtr>::CPair *p = m_Map.GetNext(pos);
				DMSkinPoolItemPtr &pCur = p->m_value;
				if (pCur->GetObjByKey(strKey,pSkin))
				{
					break;
				}
			}
		} while (false);
		return pSkin;
	}

	void DUISkinPool::PreMapKeyRemove(const DMSkinPoolItemPtr &obj)
	{
		obj->Release();
	}

}//namespace DM