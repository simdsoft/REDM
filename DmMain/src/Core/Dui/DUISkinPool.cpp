#include "DmMainAfx.h"
#include "DUISkinPool.h"

namespace DM
{
	DMSkinPoolItem::~DMSkinPoolItem()
	{
		RemoveAll();
	}

	void DMSkinPoolItem::PreArrayObjRemove(const IDMSkinPtr &obj)
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

			CStringW strName = XmlNode.Attribute(L"name");
			bool bExist = true;
			if (false == GetObjByKey(strName, pItem))
			{
				bExist  = false;
				pItem   = new DMSkinPoolItem;
			}

			DMXmlNode XmlSkin = XmlNode.FirstChild();
			while (XmlSkin.IsValid())
			{
				LPCWSTR lpszClassName = XmlSkin.GetName();
				IDMSkinPtr pSkinPtr = NULL;
				if (DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pSkinPtr,lpszClassName,DMREG_Skin)))
				{
					pSkinPtr->InitDMData(XmlSkin);
					if (!DMSUCCEEDED(pSkinPtr->IsValid()))
					{
						CStringW szInfo = lpszClassName;
						szInfo += L"(skin)无效！,将自动移除";
						DMASSERT_EXPR(0, szInfo);
						pSkinPtr->Release();
					}
					else
					{
						pItem->AddObj(pSkinPtr);
					}
				}
				else
				{
					CStringW szInfo = lpszClassName;
					szInfo += L"(skinpool)类型无法解析！,请查看是否写错";
					DMASSERT_EXPR(0, szInfo);
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

	DMCode DUISkinPool::RemoveSkinPoolItem(LPCWSTR lpszName)
	{
		CStringW szKey = lpszName;
		RemoveKey(szKey);
		return DM_ECODE_OK;
	}

	DMCode DUISkinPool::RemoveAllSkinPoolItemExcept(LPCWSTR lpszName)
	{
		DMSkinPoolItemPtr pItem = NULL;
		CStringW strKey = lpszName;
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
			LPCWSTR lpSrc = (LPCWSTR)wp;
			int nLen = (int)lp;
			if (NULL == lpSrc||0>=nLen)
			{
				break;
			}

			wchar_t *pBuf = new wchar_t[nLen];
			memcpy(pBuf,lpSrc,nLen*2);
			CStringW strValue = pBuf;
			CStringWList strUpdateList;
			int nCount = SplitStringT(strValue,L';',strUpdateList);
			// 1. 更新所有需更新的skin
			m_UpdateSkinArray.RemoveAll();
			for (int i=0;i<nCount;i++)
			{
				LPCWSTR lpszUpdate = strUpdateList[i];
				POSITION pos = m_Map.GetStartPosition();
				while (pos)
				{
					DM::CMap<CStringW,DMSkinPoolItemPtr>::CPair *p = m_Map.GetNext(pos);
					DMSkinPoolItemPtr &pCur = p->m_value;
					int count = (int)pCur->m_DMArray.GetCount();
					for (int i=0; i<count; i++)
					{
						IDMSkinPtr &pTemp = pCur->m_DMArray[i];
						if (DMSUCCEEDED(pTemp->UpdateSkin((WPARAM)lpszUpdate,0)))
						{
							m_UpdateSkinArray.Add(pTemp);
						}
					}
				}
			}
			// 2.通知所有使用这些skin的DUI窗口刷新自己区域
			iErr = g_pDMDWndPool->UpdateSkin(0,DMREG_Skin);
			DM_DELETE_ARRAY(pBuf);
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

	DMCode DUISkinPool::AddSkin(void *pBuf,size_t bufLen,LPCWSTR pszType,
		LPCWSTR lpszXml,LPCWSTR lpszPoolName/*=NULL*/)
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
			int ulSize = 260;
			byte pXmlBuf[260] = {0};
			UnicodeToUtf8((PWCHAR)lpszXml,(PCHAR)pXmlBuf,ulSize);
			DMXmlDocument doc;
			if (false == doc.LoadFromBuffer(pXmlBuf, ulSize))
			{
				break;
			}
			DMXmlNode XmlNode = doc.Root();
			if (!XmlNode.IsValid())
			{
				break;
			}

			//2. 创建skin对象
			LPCWSTR lpszClassName = XmlNode.GetName();
			IDMSkinPtr pSkinPtr = NULL;
			if (!DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pSkinPtr,lpszClassName,DMREG_Skin)))
			{
				CStringW szInfo = lpszClassName;
				szInfo += L"(skinpool)类型无法解析！,请查看是否写错";
				DMASSERT_EXPR(0, szInfo);
				break;
			}

			// 3.判断skin的id是否存在
			pSkinPtr->InitDMData(XmlNode);
			wchar_t szId[MAX_PATH] = {0};
			pSkinPtr->GetID(szId, MAX_PATH);		// 取得对象唯一ID名
			if (FindSkinFromAll(szId))				// 默认从所有skin池中查找
			{
				CStringW szInfo = szId;
				szInfo += L"(skinpool)此skin名已存在";
				DMASSERT_EXPR(0, szInfo);
				pSkinPtr->Release();
				break;// 名字已存在
			}

			// 4.初始化skin的图元
			CStringW strType = (NULL==pszType)?L"png":pszType;
			if (!DMSUCCEEDED(pSkinPtr->SetBitmap((LPBYTE)pBuf,bufLen,strType)))
			{
				pSkinPtr->Release();
				break;
			}

			// 5.判断skinpool是否存在，不存在则创建加入
			bool bExist = true;
			CStringW strName = lpszPoolName;
			DMSkinPoolItemPtr pItem = NULL;
			if (false == GetObjByKey(strName, pItem))
			{
				bExist  = false;
				pItem   = new DMSkinPoolItem;
			}
			
			pItem->AddObj(pSkinPtr);
			if (false == bExist)
			{
				AddKey(strName,pItem);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUISkinPool::RemoveSkin(LPCWSTR lpszKey,LPCWSTR lpszName,bool bLoopFind /*= true*/)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszKey||wcslen(lpszKey)<=0)
			{
				break;
			}

			DMSkinPoolItemPtr pCur = NULL;
			CStringW strName = lpszName;
			if (false == GetObjByKey(strName, pCur))
			{
				if (bLoopFind)
				{
					iErr = RemoveSkinFromAll(lpszKey);
				}
				break;
			}
			else
			{
				bool bFind = false;
				int count = (int)pCur->m_DMArray.GetCount();
				wchar_t szId[MAX_PATH] = {0};
				for (int i=0; i<count; i++)
				{
					memset(szId,0,2*MAX_PATH);
					IDMSkinPtr &pTemp = pCur->m_DMArray[i];
					pTemp->GetID(szId, MAX_PATH);
					if (0 == _wcsicmp(szId, lpszKey))
					{
						pCur->RemoveObj(i);
						bFind = true;
						break;
					}
				}

				if (!bFind)
				{
					break;// 已找到
				}

				if (bLoopFind)
				{
					iErr = RemoveSkinFromAll(lpszKey);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DUISkinPool::RemoveSkinFromAll(LPCWSTR lpszKey)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszKey||wcslen(lpszKey)<=0)
			{
				break;
			}

			POSITION pos = m_Map.GetStartPosition();
			while (pos)
			{
				DM::CMap<CStringW,DMSkinPoolItemPtr>::CPair *p = m_Map.GetNext(pos);
				DMSkinPoolItemPtr &pCur = p->m_value;
				int count = (int)pCur->m_DMArray.GetCount();
				wchar_t szId[MAX_PATH] = {0};
				for (int i=0; i<count; i++)
				{
					memset(szId,0,2*MAX_PATH);
					IDMSkinPtr &pTemp = pCur->m_DMArray[i];
					pTemp->GetID(szId, MAX_PATH);
					if (0 == _wcsicmp(szId, lpszKey))
					{
						iErr = DM_ECODE_OK;
						pCur->RemoveObj(i);
						break;
					}
				}
			}
		} while (false);
		return iErr;
	}

	IDMSkinPtr DUISkinPool::FindSkin(LPCWSTR lpszKey,LPCWSTR lpszName,bool bLoopFind)
	{
		IDMSkinPtr  pSkin = NULL;
		do 
		{
			if (NULL == lpszKey||wcslen(lpszKey)<=0)
			{
				break;
			}

			DMSkinPoolItemPtr pCur = NULL;
			CStringW strName = lpszName;
			if (false == GetObjByKey(strName, pCur))
			{
				if (bLoopFind)
				{
					pSkin = FindSkinFromAll(lpszKey);
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
					IDMSkinPtr &pTemp = pCur->m_DMArray[i];
					pTemp->GetID(szId, MAX_PATH);
					if (0 == _wcsicmp(szId, lpszKey))
					{
						pSkin = pTemp;// 已找到
						break;
					}
				}

				if (pSkin)
				{
					break;// 已找到
				}

				if (bLoopFind)
				{
					pSkin = FindSkinFromAll(lpszKey);
				}
			}
		} while (false);
		return pSkin;
	}

	IDMSkinPtr DUISkinPool::FindSkin(LPCWSTR lpszBuf,bool bLoopFind/* = true*/)
	{
		IDMSkinPtr  pSkin = NULL;
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
				strInfo.Format(L"skin-%s设置错误",strValue);
				DMASSERT_EXPR(0,strInfo);
				break;
			}

			pSkin = FindSkin(strKey,strName,bLoopFind);
		} while (false);
		return pSkin;
	}

	IDMSkinPtr DUISkinPool::FindSkinFromAll(LPCWSTR lpszKey)
	{
		IDMSkinPtr  pSkin = NULL;
		do 
		{
			if (NULL == lpszKey||wcslen(lpszKey)<=0)
			{
				break;
			}

			POSITION pos = m_Map.GetStartPosition();
			while (pos)
			{
				DM::CMap<CStringW,DMSkinPoolItemPtr>::CPair *p = m_Map.GetNext(pos);
				DMSkinPoolItemPtr &pCur = p->m_value;
				int count = (int)pCur->m_DMArray.GetCount();
				wchar_t szId[MAX_PATH] = {0};
				for (int i=0; i<count; i++)
				{
					memset(szId,0,2*MAX_PATH);
					IDMSkinPtr &pTemp = pCur->m_DMArray[i];
					pTemp->GetID(szId, MAX_PATH);
					if (0 == _wcsicmp(szId, lpszKey))
					{
						pSkin = pTemp;// 已找到
						break;
					}
				}
				if (pSkin)
				{
					break;// 已找到
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