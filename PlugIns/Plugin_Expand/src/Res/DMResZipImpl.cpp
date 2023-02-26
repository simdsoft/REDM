#include "Plugin_ExpandAfx.h"
#include "DMResZipImpl.h"
#include "DMResZipParam.h"

namespace DM
{
	DMResZipImpl::DMResZipImpl()
	{
		m_bThemeLoop = true;
		m_pCurTheme  = NULL;
		m_bOutStyle  = false;
		m_bAssert    = true;
	}

	DMResZipImpl::~DMResZipImpl()
	{
		RemoveAll();
	}

	DMCode DMResZipImpl::LoadResPack(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (0 == wp||IsBadReadPtr((void*)wp,4))
			{
				break;
			}
			DMResZipParam *pZipParam = (DMResZipParam*)wp;
			if (pZipParam->m_bPERes)// ��Ƕ��exe�е�
			{
				HINSTANCE hInst = (NULL == pZipParam->m_PEResInfo.hInst)?GetModuleHandle(NULL):pZipParam->m_PEResInfo.hInst;
				if (!m_zipFile.LoadFromPE(hInst, pZipParam->m_PEResInfo.lpszResName,pZipParam->m_PEResInfo.lpszResType))
				{
					break;
				}
			}
			else// �ⲿzip
			{
				wchar_t szPath[MAX_PATH] = {0};
				if (!GetRootFullPath((LPWSTR)pZipParam->m_lpszZipPath,szPath,MAX_PATH))
				{
					break;
				}
				if (!m_zipFile.LoadFromFile(szPath)) 
				{
					break;
				}
			}
			m_zipFile.SetPassword(pZipParam->m_lpszPsw);

			m_pLayout.Release();// ���ͷ�ԭʼ�������-----------��
			m_pCurTheme = NULL; // ��ǰʹ�õ�������ÿ�---------��
			RemoveAll();		// ���ͷ�ԭʼthemes-------------��

			// ����Layout
			m_pLayout.Attach(new DMZipItemArray);
			CStringW strLayoutPathIndex = DMRES_LAYOUT_FOLDER;strLayoutPathIndex += DMRES_DMINDEX;
			iErr = ParseIndex(strLayoutPathIndex, &m_pLayout);
			

			// ����themes
			CStringW strThemesPathIndex = DMRES_THEMES_FOLDER;strThemesPathIndex += DMRES_DMINDEX;
			iErr = ParseThemes(strThemesPathIndex);
		} while (false);
		return iErr;
	}

	DMCode DMResZipImpl::IsItemExists(LPCSTR lpszType, LPCSTR lpszName, LPCSTR lpszThemeName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszType||NULL == lpszName)
			{
				break;
			}
			DMZipItem Item(lpszType, lpszName);// ����������EqualArrayObj��ֻҪǰ����������ȣ�����Ϊ�Ѵ���
			if (0 == dm_xmlstrcmp(lpszType, RES_LAYOUT)||0 == dm_xmlstrcmp(lpszType, RES_GLOBAL))
			{
				if (NULL == m_pLayout || DM_INVALID_VALUE == m_pLayout->FindObj(&Item))
				{
					break;
				}
			}
			else
			{
				DMZipItemArrayPtr pTheme = FindResItemObj(lpszThemeName);
				if (pTheme && DM_INVALID_VALUE == pTheme->FindObj(&Item))
				{
					if (false == m_bThemeLoop)
					{
						break;
					}
					bool bFind = false;// ������themes�в���
					int nCount = (int)GetCount();
					for (int i=0;i<nCount;i++)
					{
						DMZipItemArrayPtr &pCur = GetObj(i);
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

	DMCode DMResZipImpl::GetItemSize(LPCSTR lpszType, LPCSTR lpszName, unsigned long& ulSize, LPCSTR lpszThemeName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		size_t size = 0;
		do 
		{
			LPCWSTR lpszFilePath = GetItemPath(lpszType, lpszName,lpszThemeName);
			if (NULL == lpszFilePath) 
			{
				break;
			}
			if (L'\\' == lpszFilePath[0]||L'/'==lpszFilePath[0])// ��ǰ�治�ܴ�\\����ȻZIP�޷�ʶ��
			{
				lpszFilePath = lpszFilePath+1;
			}

			ZIP_FIND_DATA zfd;
			HANDLE hf = m_zipFile.FindFirstFile(lpszFilePath,&zfd);
			if (INVALID_HANDLE_VALUE == hf) 
			{
				break;
			}
			m_zipFile.FindClose(hf);
			ulSize = zfd.nFileSizeUncompressed;
			iErr = DM_ECODE_OK;
		} while (false);
		if (!DMSUCCEEDED(iErr)&&m_bAssert)
		{
			CStringW strInfo;
			strInfo.Format(L"Res��Դ��%s:%s��ȡsizeʧ��",lpszType,lpszName);
			DMASSERT_EXPR(0,strInfo);
		}
		return iErr;
	}

	DMCode DMResZipImpl::GetItemBuf(LPCSTR lpszType, LPCSTR lpszName, DMBufT<byte>& lpBuf, PULONG lpULSize, LPCSTR lpszThemeName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			LPCWSTR lpszFilePath = GetItemPath(lpszType, lpszName,lpszThemeName);
			if (NULL == lpszFilePath) 
			{
				break;
			}
			if (L'\\' == lpszFilePath[0]||L'/'==lpszFilePath[0])// ��ǰ�治�ܴ�\\����ȻZIP�޷�ʶ��
			{
				lpszFilePath = lpszFilePath+1;
			}

			CDMZipFile zf;
			if (!m_zipFile.GetFile(lpszFilePath, zf))
			{
				break;
			}
			DWORD dataSize = zf.GetSize();
			if (lpULSize) *lpULSize = dataSize;
			lpBuf.AllocateBytes(dataSize);
			memcpy(lpBuf,zf.GetData(),zf.GetSize());
			// lpBuf.Free();
			iErr = DM_ECODE_OK;
		} while (false);
		if (!DMSUCCEEDED(iErr)&&m_bAssert)
		{
			CStringW strInfo;
			strInfo.Format(L"Res��Դ��%s:%s��ȡbufʧ��",lpszType,lpszName);
			DMASSERT_EXPR(0,strInfo);
		}
		return iErr;
	}

	DMCode DMResZipImpl::LoadTheme(WPARAM wp, LPARAM lp)
	{
		return DM_ECODE_NOTIMPL;
	}

	DMCode DMResZipImpl::SetCurTheme(LPCSTR lpszName, LPCSTR lpszOldName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			if (lpszOldName)
			{// �����ɵ�theme����,���۳ɹ���񣬶���ȡ��
				memcpy((void*)lpszOldName,m_strCurTheme,m_strCurTheme.GetLength()*2);
			}

			DMZipItemArrayPtr pCurTheme=NULL;
			CStringA strName = lpszName;
			if (strName == m_strCurTheme// Ҫ���õĺ͵�ǰ����ͬ
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
			DMZipItemArrayPtr	pOldTheme = m_pCurTheme;  
			m_pCurTheme = pCurTheme;
			m_strCurTheme = strName;
			// 1.�Ƚ�����themes֮ǰ��index�Ĳ����ȫ��ͬ����Ϊ��Ҫ���£�����Ҫ���µ�type+name
			int nCount    = (int)m_pCurTheme->GetCount();
			int nCmpCount = (int)pOldTheme->GetCount();
			if (m_bOutStyle||NULL == pOldTheme)
			{
				nCmpCount = 0;// �ⲿģʽ��Ӧ��Ҫ�������е�theme,����ֱ�ӰѱȽ�������Ϊ0
			}
			bool bFind = false;// �Ƿ����ǰthemesһ��
			CStringA strUpdateInfo;
			for (int i=0;i<nCount;i++)
			{
				DMZipItemPtr &pItem = m_pCurTheme->m_DMArray[i];
				for (int j=0;j<nCmpCount;j++)
				{
					bFind = false;// ��Ԥ��Ϊfalse
					DMZipItemPtr &pCmpItem = pOldTheme->m_DMArray[j];
					if (0 == dm_xmlstrcmp(pItem->m_szName,pCmpItem->m_szName)
						&&0 == dm_xmlstrcmp(pItem->m_szType,pCmpItem->m_szType))
					{
						if (0 == _wcsicmp(pItem->m_szPath,pCmpItem->m_szPath))
						{
							bFind = true;
						}
						break;// ������ѭ��
					}
				}
				if (false == bFind)
				{
					strUpdateInfo += pItem->m_szType;
					strUpdateInfo += ':';
					strUpdateInfo += pItem->m_szName;
					strUpdateInfo += ';';
				}
			}
			// TODO.��ʼ�㲥��Ϣ������Skin--
			g_pDMApp->UpdateSkin((WPARAM)(LPCSTR)strUpdateInfo,(LPARAM)strUpdateInfo.GetLength());
			m_bOutStyle = false;// �������ⲿģʽ��
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMResZipImpl::SendExpandInfo(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			CStringA strAllTheme;
			int nCount = (int)GetCount();
			for (int i=0; i<nCount; i++)
			{
				DMZipItemArrayPtr &pCur = GetObj(i);
				CStringA strTheme = pCur->m_strThemeName;
				strTheme += ";";
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

	// ����
	DMCode DMResZipImpl::ParseThemes(LPCWSTR lpszIndexPath)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszIndexPath)
			{
				break;
			}
			if (L'\\' == lpszIndexPath[0]||L'/'==lpszIndexPath[0])// ��ǰ�治�ܴ�\\����ȻZIP�޷�ʶ��
			{
				lpszIndexPath = lpszIndexPath+1;
			}
			CDMZipFile zf;
			if (!m_zipFile.GetFile(lpszIndexPath,zf))
			{
				break;
			}
			DMXmlDocument XmlDoc;
			if (false == XmlDoc.LoadFromBuffer(zf.GetData(),zf.GetSize()))
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
				InitDMData(XmlNode);// ʹ��xml�����滻
				DMXmlNode XmlFileNode = XmlNode.FirstChild("file");
				while (XmlFileNode.IsValid())
				{
					LPCSTR lpszName = (XmlFileNode.Attribute("name"));
					CStringW strFilePath = DMA2W(XmlFileNode.Attribute("path"));
					if (!strFilePath.IsEmpty())
					{
						DMZipItemArrayPtr pItem = new DMZipItemArray;
						if (DMSUCCEEDED(ParseIndex(strFilePath,&pItem)))
						{
							pItem->m_strThemeName = lpszName;
							if (false == AddObj(pItem))
							{
								CStringA szInfo = lpszName;szInfo += "������Ѵ���,�������ԣ�";DMASSERT_EXPR(0,ntcvt::from_chars(szInfo).c_str());
								pItem->Release();
							}
						}
						else
						{
							pItem->Release();
						}
					}
					XmlFileNode = XmlFileNode.NextSibling("file");
				}
				XmlNode = XmlNode.NextSibling();
			}

			// ��ʼ����ǰʹ�õ�theme
			if ((int)GetCount()>=1)
			{
				m_pCurTheme = FindResItemObj(m_strCurTheme);
				if (NULL == m_pCurTheme)
				{
					m_pCurTheme = GetObj(0);
					m_strCurTheme = m_pCurTheme->m_strThemeName;
				}	
			}
			DMASSERT_EXPR(NULL!=m_pCurTheme,L"m_pCurTheme��ȻΪ�գ�������");
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMResZipImpl::ParseIndex(LPCWSTR lpszIndexPath,DMZipItemArray** ppItem)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszIndexPath||NULL == ppItem)
			{
				break;
			}

			if (L'\\' == lpszIndexPath[0]||L'/'==lpszIndexPath[0])// ��ǰ�治�ܴ�\\����ȻZIP�޷�ʶ��
			{
				lpszIndexPath = lpszIndexPath+1;
			}

			CDMZipFile zf;
			if (!m_zipFile.GetFile(lpszIndexPath,zf))
			{
				break;
			}

			DMXmlDocument XmlDoc;
			if (false == XmlDoc.LoadFromBuffer(zf.GetData(),zf.GetSize()))
			{
				CStringW strInfo;strInfo.Format(L"xml LoadFromFile:%s fail",lpszIndexPath);DMASSERT_EXPR(0,strInfo);
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
				LPCSTR lpszType = (XmlNode.GetName());
				DMXmlNode XmlFileNode = XmlNode.FirstChild("file");
				while (XmlFileNode.IsValid())
				{
					LPCSTR lpszName = (XmlFileNode.Attribute("name"));
					CStringW filePath = DMA2W(XmlFileNode.Attribute("path"));
					if (!filePath.IsEmpty())
					{
#if defined(_DEBUG)
						CDMZipFile zftemp;
						if (!m_zipFile.GetFile(filePath,zftemp))
						{
							CStringW szInfo = filePath;szInfo+=L"�ļ������ڣ�";DMASSERT_EXPR(0,szInfo);
						}
						zftemp.Close();
#endif
						DMZipItem *pResItem = new DMZipItem(lpszType,lpszName, filePath);
						(*ppItem)->AddObj(pResItem);// ����DMArrayT
					}
					XmlFileNode = XmlFileNode.NextSibling("file");
				}

				XmlNode = XmlNode.NextSibling();
			}

			(*ppItem)->m_DMArray;//������DEBUG�鿴m_Array�ĵط�,watchһ�¾ͺ���
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	LPCWSTR DMResZipImpl::GetItemPath(LPCSTR lpszType,LPCSTR lpszName,LPCSTR lpszThemeName)
	{
		LPCWSTR lpszPath = NULL;
		do 
		{
			if (NULL == lpszType||NULL == lpszName)
			{
				break;
			}
			DMZipItem Item(lpszType, lpszName);// ����������EqualArrayObj��ֻҪǰ����������ȣ�����Ϊ�Ѵ���
			if (0 == dm_xmlstrcmp(lpszType, RES_LAYOUT)||0 == dm_xmlstrcmp(lpszType, RES_GLOBAL))
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
				DMZipItemArrayPtr pTheme = FindResItemObj(lpszThemeName);
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
					bool bFind = false;// ������themes�в���
					int nCount = (int)GetCount();
					for (int i=0;i<nCount;i++)
					{
						DMZipItemArrayPtr &pCur = GetObj(i);
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

	DMZipItemArrayPtr DMResZipImpl::FindResItemObj(LPCSTR lpszName)
	{
		DMZipItemArrayPtr pFindItem = NULL;
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
				DMZipItemArrayPtr &pItem = GetObj(i);
				if (pItem&&0==pItem->m_strThemeName.CompareNoCase(lpszName))
				{
					pFindItem = pItem;
					break;
				}
			}
		} while (false);
		return pFindItem;
	}

	void DMResZipImpl::PreArrayObjRemove(const DMZipItemArrayPtr &obj)
	{
		obj->Release(); // �����ü���Ϊ0�����Զ�delete this
	}

}//namespace DM