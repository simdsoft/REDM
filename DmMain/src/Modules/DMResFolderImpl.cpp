#include "DmMainAfx.h"
#include "DMResFolderImpl.h"

namespace DM
{
	///
	DMResFolderImpl::DMResFolderImpl()
	{
		m_bThemeLoop = true;
		m_pCurTheme  = NULL;
		m_bOutStyle  = false;
	}

	DMResFolderImpl::~DMResFolderImpl()
	{
		RemoveAll();
	}

	DMCode DMResFolderImpl::LoadResPack(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (0 == wp||IsBadReadPtr((void*)wp,4))
			{
				break;
			}

			LPWSTR lpszPath = (LPWSTR)wp;
			wchar_t szPath[MAX_PATH] = {0};
			if (!GetRootFullPath(lpszPath,szPath,MAX_PATH))
			{
				break;
			}
			m_strDir = szPath;// 初始化资源包位置
		
			DM::CStringW strLayoutPathIndex = szPath;DM::CStringW strThemesPathIndex = szPath;
			strLayoutPathIndex += DMRES_LAYOUT_FOLDER;strLayoutPathIndex += DMRES_DMINDEX;
			strThemesPathIndex += DMRES_THEMES_FOLDER;strThemesPathIndex += DMRES_DMINDEX;
			LPCWSTR lpszLayoutPathIndex = strLayoutPathIndex;
			if (!CheckFileExistW((wchar_t*)lpszLayoutPathIndex))
			{
				CStringW szErrInfo = strLayoutPathIndex;szErrInfo+=L"文件不存在";
				DMASSERT_EXPR(0, szErrInfo);
				iErr = DM_ECODE_FAIL;
				break;
			}

			m_pLayout.Release();// 先释放原始布局相关-----------！
			m_pCurTheme = NULL; // 当前使用的主题包置空---------！
			RemoveAll();		// 先释放原始themes-------------！
			
			// 解析Layout
			m_pLayout.Attach(new DMResFolderItem);
			iErr = ParseIndex(lpszLayoutPathIndex, &m_pLayout);

			// 解析themes
			LPCWSTR lpszThemesPathIndex = strThemesPathIndex;
			iErr = ParseThemes(lpszThemesPathIndex);
		} while (false);
		return iErr;
	}

	DMCode DMResFolderImpl::IsItemExists(LPCWSTR lpszType, LPCWSTR lpszName,LPCWSTR lpszThemeName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszType||NULL == lpszName)
			{
				break;
			}
			DMResItem Item(lpszType, lpszName);// 这里重载了EqualArrayObj，只要前两个变量相等，就认为已存在
			if (0 == _wcsicmp(lpszType, RES_LAYOUT)||0 == _wcsicmp(lpszType, RES_GLOBAL))
			{
				if (NULL == m_pLayout || DM_INVALID_VALUE == m_pLayout->FindObj(&Item))
				{
					break;
				}
			}
			else
			{
				DMResFolderItemPtr pTheme = FindResItemObj(lpszThemeName);
				if (pTheme && DM_INVALID_VALUE == pTheme->FindObj(&Item))
				{
					if (false == m_bThemeLoop)
					{
						break;
					}
					bool bFind = false;// 从整个themes中查找
					int nCount = (int)GetCount();
					for (int i=0;i<nCount;i++)
					{
						DMResFolderItemPtr &pCur = GetObj(i);
						if (pCur && DM_INVALID_VALUE !=pCur->FindObj(&Item))
						{
							bFind = true;
							break;
						}
					}
					if (false == bFind)
					{
						break;
					}
				}
				else if(NULL == pTheme)
				{
					break;
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMResFolderImpl::GetItemSize(LPCWSTR lpszType, LPCWSTR lpszName, unsigned long& ulSize,LPCWSTR lpszThemeName)
	{
		DMCode iErr = DM_ECODE_OK;
		size_t size = 0;
		do 
		{
			LPCWSTR lpszFilePath = GetItemPath(lpszType, lpszName,lpszThemeName);
			if (NULL == lpszFilePath) 
			{
				iErr = DM_ECODE_FAIL;
				break;
			}

			size = DM::GetFileSizeW((wchar_t*)lpszFilePath);
			if (0 == size)
			{
				iErr = DM_ECODE_FAIL;
				break;
			}
			ulSize = size;
		} while (false);
		if (!DMSUCCEEDED(iErr)&&m_bAssert)
		{
			CStringW strInfo;
			strInfo.Format(L"Res资源中%s:%s获取size失败",lpszType,lpszName);
			DMASSERT_EXPR(0,strInfo);
		}
		return iErr;
	}

	DMCode DMResFolderImpl::GetItemBuf(LPCWSTR lpszType, LPCWSTR lpszName, LPVOID lpBuf, unsigned long ulSize,LPCWSTR lpszThemeName)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			LPCWSTR lpszFilePath = GetItemPath(lpszType, lpszName,lpszThemeName);
			if (NULL == lpszFilePath) 
			{
				iErr = DM_ECODE_FAIL;
				break;
			}

			DWORD dwReadSize = 0;
			if (false == DM::GetFileBufW((wchar_t*)lpszFilePath, &lpBuf, ulSize, dwReadSize))
			{
				iErr = DM_ECODE_FAIL;
				break;
			}
		} while (false);
		if (!DMSUCCEEDED(iErr)&&m_bAssert)
		{
			CStringW strInfo;
			strInfo.Format(L"Res资源中%s:%s获取buf失败",lpszType,lpszName);
			DMASSERT_EXPR(0,strInfo);
		}
		return iErr;
	}

	DMCode DMResFolderImpl::LoadTheme(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (0 == wp||IsBadReadPtr((void*)wp,4)||0 == lp||IsBadReadPtr((void*)lp,4))
			{
				break;
			}
			LPCWSTR lpszName = (LPCWSTR)wp;
			LPWSTR lpszFilePath = (LPWSTR)lp;
			wchar_t szPath[MAX_PATH] = {0};
			if (!GetRootFullPath(lpszFilePath,szPath,MAX_PATH))
			{
				break;
			}
		
			DMResFolderItemPtr pItem = new DMResFolderItem;
			if (!DMSUCCEEDED(ParseIndex(szPath,&pItem)))
			{
				pItem->Release();
				break;
			}
			pItem->m_strThemeName = lpszName;
			if (AddObj(pItem))
			{
				iErr = DM_ECODE_OK;
			}
			else
			{
				CStringW szInfo = lpszName;
				szInfo += L"主题包已存在,将被忽略！";
				DMASSERT_EXPR(0,szInfo);
				pItem->Release();
			}
		} while (false);
		return iErr;
	}

	DMCode DMResFolderImpl::SetCurTheme(LPCWSTR lpszName, LPCWSTR lpszOldName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			if (lpszOldName)
			{// 传出旧的theme名称,无论成功与否，都先取得
				memcpy((void*)lpszOldName,m_strCurTheme,m_strCurTheme.GetLength()*2);
			}

			DMResFolderItemPtr pCurTheme=NULL;
			CStringW strName = lpszName;
			if (strName == m_strCurTheme// 要设置的和当前的相同
				&&NULL != m_pCurTheme&&!m_bOutStyle)
			{
				iErr = DM_ECODE_OK;
				break;
			}

			pCurTheme = FindResItemObj(lpszName);
			if (NULL == pCurTheme)
			{
				break;
			}
			DMResFolderItemPtr	pOldTheme = m_pCurTheme;  
			m_pCurTheme = pCurTheme;
			m_strCurTheme = strName;
			// 1.比较两个themes之前的index的差别，完全不同的认为需要更新，保留要更新的type+name
			int nCount    = (int)m_pCurTheme->GetCount();
			int nCmpCount = (int)pOldTheme->GetCount();
			if (m_bOutStyle||NULL == pOldTheme)
			{
				nCmpCount = 0;// 外部模式，应该要更新所有的theme,所以直接把比较数设置为0
			}
			bool bFind = false;// 是否和先前themes一样
			CStringW strUpdateInfo;
			for (int i=0;i<nCount;i++)
			{
				DMResItemPtr &pItem = m_pCurTheme->m_DMArray[i];
				for (int j=0;j<nCmpCount;j++)
				{
					bFind = false;// 先预置为false
					DMResItemPtr &pCmpItem = pOldTheme->m_DMArray[j];
					if (0 == _wcsicmp(pItem->m_szName,pCmpItem->m_szName)
						&&0 == _wcsicmp(pItem->m_szType,pCmpItem->m_szType))
					{
						if (0 == _wcsicmp(pItem->m_szPath,pCmpItem->m_szPath))
						{
							bFind = true;
						}
						break;// 跳出内循环
					}
				}
				if (false == bFind)
				{
					strUpdateInfo += pItem->m_szType;
					strUpdateInfo += L':';
					strUpdateInfo += pItem->m_szName;
					strUpdateInfo += L';';
				}
			}
			// TODO.开始广播消息，更换Skin--
			int nLen = strUpdateInfo.GetLength()+1;
			DMBufT<wchar_t>pBuf;pBuf.Allocate(nLen);
			memcpy(pBuf,strUpdateInfo.GetBuffer(),(nLen-1)*2);
			strUpdateInfo.ReleaseBuffer();
			g_pDMApp->UpdateSkin((WPARAM)pBuf.get(),(LPARAM)nLen);
			m_bOutStyle = false;// 不再是外部模式了
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMResFolderImpl::SendExpandInfo(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CStringW strAllTheme;
			int nCount = (int)GetCount();
			for (int i=0; i<nCount; i++)
			{
				DMResFolderItemPtr &pCur = GetObj(i);
				CStringW strTheme = pCur->m_strThemeName;
				strTheme += L";";
				strAllTheme += strTheme;
			}
			int nSize = 2*(strAllTheme.GetLength()+1);
			if (wp&&NULL==lp)
			{
				if (!IsBadReadPtr((void*)wp,4))
				{
					*(DWORD*)wp  = nSize;
				}
			}
			else if(wp&&lp>=nSize)
			{
				if (!IsBadReadPtr((void*)wp,4))
				{
					memcpy((void*)wp,strAllTheme.GetBuffer(),nSize);
				}
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	// 辅助
	DMCode DMResFolderImpl::ParseThemes(LPCWSTR lpszIndexPath)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMXmlDocument XmlDoc;
			if (false == XmlDoc.LoadFromFile(lpszIndexPath))
			{
				break;
			}

			DMXmlNode XmlNode = XmlDoc.Root();
			if (false == XmlNode.IsValid())
			{
				break;
			}

			XmlNode = XmlNode.FirstChild();
			while (XmlNode.IsValid())
			{
				InitDMData(XmlNode);// 使用xml解析替换
				DMXmlNode XmlFileNode = XmlNode.FirstChild(L"file");
				while (XmlFileNode.IsValid())
				{
					LPCWSTR lpszName = XmlFileNode.Attribute(L"name");
					LPCWSTR lpszFilePath = XmlFileNode.Attribute(L"path");
					wchar_t szPath[MAX_PATH] = {0};
					if (0 != PathCombineW(szPath, m_strDir, lpszFilePath))
					{
						DMResFolderItemPtr pItem = new DMResFolderItem;
						if (DMSUCCEEDED(ParseIndex(szPath,&pItem)))
						{
							pItem->m_strThemeName = lpszName;
							if (false == AddObj(pItem))
							{
								CStringW szInfo = lpszName;
								szInfo += L"主题包已存在,将被忽略！";
								DMASSERT_EXPR(0,szInfo);
								pItem->Release();
							}
						}
						else
						{
							pItem->Release();
						}
					}
					XmlFileNode = XmlFileNode.NextSibling(L"file");
				}
				XmlNode = XmlNode.NextSibling();
			}

			// 初始化当前使用的theme
			if ((int)GetCount()>=1)
			{
				m_pCurTheme = FindResItemObj(m_strCurTheme);
				if (NULL == m_pCurTheme)
				{
					m_pCurTheme = GetObj(0);
					m_strCurTheme = m_pCurTheme->m_strThemeName;
				}	
			}
			DMASSERT_EXPR(NULL!=m_pCurTheme,L"m_pCurTheme竟然为空！！！！");
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMResFolderImpl::ParseIndex(LPCWSTR lpszIndexPath,DMResFolderItem** ppItem)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszIndexPath||NULL == ppItem)
			{
				break;
			}
	
			DMXmlDocument XmlDoc;
			if (false == XmlDoc.LoadFromFile(lpszIndexPath))
			{
				CStringW strInfo;
				strInfo.Format(L"xml LoadFromFile:%s fail",lpszIndexPath);
				DMASSERT_EXPR(0,strInfo);
				LOG_ERR("[mid]-xml LoadFromFile:%s fail\n",lpszIndexPath);
				break;
			}

			DMXmlNode XmlNode = XmlDoc.Root();
			if (false == XmlNode.IsValid())
			{
				LOG_ERR("[mid]-xmlnode inValid\n");
				break;
			}

			XmlNode = XmlNode.FirstChild();
			while (XmlNode.IsValid())
			{
				LPCWSTR lpszType = XmlNode.GetName();
				DMXmlNode XmlFileNode = XmlNode.FirstChild(L"file");
				while (XmlFileNode.IsValid())
				{
					LPCWSTR lpszName = XmlFileNode.Attribute(L"name");
					LPCWSTR lpszFilePath = XmlFileNode.Attribute(L"path");
					if (NULL!=lpszFilePath&&0!=wcslen(lpszFilePath))
					{
						wchar_t szPath[MAX_PATH] = {0};
						if (0 != PathCombineW(szPath, m_strDir, lpszFilePath))
						{
#if defined(_DEBUG)
							if (!CheckFileExistW(szPath))
							{
								CStringW szInfo = szPath;szInfo+=L"文件不存在！";
								DMASSERT_EXPR(0,szInfo);
							}
#endif 
							DMResItem *pResItem = new DMResItem(lpszType,lpszName,szPath);
							(*ppItem)->AddObj(pResItem);// 来自DMArrayT
						}
					}
					XmlFileNode = XmlFileNode.NextSibling(L"file");
				}

				XmlNode = XmlNode.NextSibling();
			}

			(*ppItem)->m_DMArray;//给个可DEBUG查看m_Array的地方,watch一下就好了
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	LPCWSTR DMResFolderImpl::GetItemPath(LPCWSTR lpszType,LPCWSTR lpszName,LPCWSTR lpszThemeName)
	{
		LPCWSTR lpszPath = NULL;
		do 
		{
			if (NULL == lpszType||NULL == lpszName)
			{
				break;
			}
			DMResItem Item(lpszType, lpszName);// 这里重载了EqualArrayObj，只要前两个变量相等，就认为已存在
			if (0 == _wcsicmp(lpszType, RES_LAYOUT)||0 == _wcsicmp(lpszType, RES_GLOBAL))
			{
				if (NULL == m_pLayout)
				{
					break;
				}
				int iElement = m_pLayout->FindObj(&Item);
				if (DM_INVALID_VALUE == iElement)
				{
					break;
				}
				lpszPath = m_pLayout->GetObj(iElement)->m_szPath;
			}
			else
			{
				DMResFolderItemPtr pTheme = FindResItemObj(lpszThemeName);
				if (NULL==pTheme)
				{
					break;
				}
				int iElement = pTheme->FindObj(&Item);
				if (DM_INVALID_VALUE == iElement)
				{
					if (false == m_bThemeLoop)
					{
						break;
					}
					bool bFind = false;// 从整个themes中查找
					int nCount = (int)GetCount();
					for (int i=0;i<nCount;i++)
					{
						DMResFolderItemPtr &pCur = GetObj(i);
						if (pCur)
						{
							iElement = pCur->FindObj(&Item);
							if (DM_INVALID_VALUE != iElement)
							{
								bFind = true;
								lpszPath = pCur->GetObj(iElement)->m_szPath;
								break;
							}
						}
					}
					if (false == bFind)
					{
						break;
					}
				}
				else
				{
					lpszPath = pTheme->GetObj(iElement)->m_szPath;
				}
			}

		} while (false);
		return lpszPath;
	}

	DMResFolderItemPtr DMResFolderImpl::FindResItemObj(LPCWSTR lpszName)
	{
		DMResFolderItemPtr pFindItem = NULL;
		do 
		{
			if (NULL == lpszName)
			{
				pFindItem = m_pCurTheme;
				break;
			}

			int nCount = (int)GetCount();
			for (int i=0; i<nCount; i++)
			{
				DMResFolderItemPtr &pItem = GetObj(i);
				if (pItem&&0==pItem->m_strThemeName.CompareNoCase(lpszName))
				{
					pFindItem = pItem;
					break;
				}
			}
		} while (false);
		return pFindItem;
	}

	void DMResFolderImpl::PreArrayObjRemove(const DMResFolderItemPtr &obj)
	{
		obj->Release(); // 如引用计数为0，则自动delete this
	}

}//namespace DM