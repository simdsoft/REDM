#include "DmMainAfx.h"

#if !defined(DM_EXCLUDE_MUI)
#include "DMTransImpl.h"

namespace DM
{
	DMTransImpl::DMTransImpl()
	{
		m_pCurLanguage = NULL;
	}

	DMTransImpl::~DMTransImpl()
	{
		RemoveAll();
	}

	DMCode DMTransImpl::LoadTrans(WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (0 == wp||IsBadReadPtr((void*)wp,4))
			{
				break;
			}

			//1. 加载XML
			DMXmlDocument XmlDoc;
			if (0 == lp)
			{
				LPCSTR lpszXmlId = (LPCSTR)wp;
				if (!DMSUCCEEDED(g_pDMApp->InitDMXmlDocument(XmlDoc,RES_LAYOUT,lpszXmlId)))
				{
					break;
				}	
			}
			else if (1 == lp)
			{
				LPWSTR lpszPath = (LPWSTR)wp;
				wchar_t szPath[MAX_PATH] = {0};
				if (!GetRootFullPath(lpszPath,szPath,MAX_PATH))
				{
					break;
				}
				if (!XmlDoc.LoadFromFile(szPath))
				{
					break;
				}
			}

			//2.解析XML
			DMXmlNode XmlLanguage = XmlDoc.Root(DMLAG_ROOT);
			if (!XmlLanguage.IsValid())
			{
				break;
			}

			//3.获得语言包对象
			DMLanguageItemPtr pLanguageItem = FindLanguageItemPtr(DMCA2W(XmlLanguage.Attribute(DMLAG_NAME)));
			if (NULL == pLanguageItem)// 如果原来不存在这个语言包对象，就新建一个，并加入
			{
				pLanguageItem = new DMLanguageItem(DMCA2W(XmlLanguage.Attribute(DMLAG_NAME)));
				AddObj(pLanguageItem);
			}

			//4.添加语言包的node
			DMXmlNode XmlNode = XmlLanguage.FirstChild(DMLAG_NODE);// loop 1
			while (XmlNode.IsValid())
			{
				DMTransNodePtr pNode = pLanguageItem->FindTransNodePtr(DMCA2W(XmlNode.Attribute(DMLAG_NAME)));
				if (NULL == pNode)// 如果此Node不存在，就创建并加入
				{
					pNode = new DMTransNode(DMCA2W(XmlNode.Attribute(DMLAG_NAME)));
					pLanguageItem->AddObj(pNode);
				}

				//5. 添加node的item
				DMXmlNode XmlItem = XmlNode.FirstChild(DMLAG_ITEM);// loop 2
				while (XmlItem.IsValid())
				{ // TODO: mui/i18n translate
					CStringW strSrc = DMCA2W(XmlItem.Attribute(DMLAG_SRC));
					CStringW strTrans = DMCA2W(XmlItem.Attribute(DMLAG_TRANS));
					if (pNode->IsKeyExist(strSrc))
					{// 以最后一个为准
						pNode->RemoveKey(strSrc);
					}
					pNode->AddKey(strSrc,strTrans);
					XmlItem = XmlItem.NextSibling(DMLAG_ITEM);// loop 2
				}

				XmlNode = XmlNode.NextSibling(DMLAG_NODE);// loop 1
			}
		} while (false);
		return iErr;
	}

	DMCode DMTransImpl::SetTrans(CStringW strLanguageName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			//1. 为空表示使用内部原始的,所以清空字符串
			if (strLanguageName.IsEmpty())
			{
				m_pCurLanguage = NULL;
				m_strCurLanguageName = L"";
				iErr = DM_ECODE_OK;
				break;
			}

			//2. 设置相同,不处理
			if (0 == strLanguageName.CompareNoCase(m_strCurLanguageName))
			{
				break;
			}

			//3. 获得语言包,如果获取成功,则更新m_strCurLanguageName,如果获取失败,则设置m_strCurLanguageName为空
			m_pCurLanguage = FindLanguageItemPtr(strLanguageName);
			if (m_pCurLanguage)
			{
				m_strCurLanguageName = strLanguageName;
			}
			else
			{
				m_strCurLanguageName = L"";
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMTransImpl::AddTransItem(CStringW strSrc, CStringW strTrans,CStringW strNodeName,CStringW strLanguageName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			//1.查找语言包对象,如果不存在,则创建它
			DMLanguageItemPtr pLanguageItem = NULL;
			if (strLanguageName.IsEmpty())
			{
				pLanguageItem = m_pCurLanguage;
			}
			else
			{
				pLanguageItem = FindLanguageItemPtr(strLanguageName);
			}
			if (NULL == pLanguageItem&&!strLanguageName.IsEmpty())// 创建新的语言包
			{
				pLanguageItem = new DMLanguageItem(strLanguageName);
				AddObj(pLanguageItem);
			}
			if (NULL == pLanguageItem)
			{
				break;
			}

			//2.查找语言包对象中Node子对象,如果不存在,则创建
			DMTransNodePtr pNode = pLanguageItem->FindTransNodePtr(strNodeName);
			if (NULL == pNode)
			{
				pNode = new DMTransNode(strNodeName);
				pLanguageItem->AddObj(pNode);
			}

			//3.如果翻译已存在,则移除原翻译,加入新翻译
			if (pNode->IsKeyExist(strSrc))
			{
				pNode->RemoveKey(strSrc);
			}
			pNode->AddKey(strSrc,strTrans);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	CStringW DMTransImpl::GetTrans(CStringW strSrc,CStringW strNodeName,CStringW strLanguageName)
	{
		CStringW strTrans = strSrc;// 默认返回原始的
		do 
		{
			if (strSrc.IsEmpty()||0==(int)GetCount())
			{
				break;
			}
			//1.查找语言包对象
			DMLanguageItemPtr pLanguageItem = NULL;
			if (strLanguageName.IsEmpty())// 从当前设置的语言包中查找
			{
				pLanguageItem = m_pCurLanguage;
			}
			else
			{
				pLanguageItem = FindLanguageItemPtr(strLanguageName);
			}
			if (NULL == pLanguageItem)
			{
				break;
			}


			if (strNodeName.IsEmpty())
			{
				strNodeName = L"custom";
			}
			//2.优先从特定strNodeName配置查找,如果查找不到,则从custom中查找
			if (0!=strNodeName.CompareNoCase(L"custom"))// 如果不是custom
			{
				DMTransNodePtr pNode = pLanguageItem->FindTransNodePtr(strNodeName);
				if (NULL == pNode || !pNode->IsKeyExist(strSrc))// 节点不存在,或节点存在但key不存在,则试着从custom中找
				{
					pNode = pLanguageItem->FindTransNodePtr(L"custom");
					if (NULL != pNode)
					{
						pNode->GetObjByKey(strSrc,strTrans);
					}
				}
				else
				{
					pNode->GetObjByKey(strSrc,strTrans);
				}
			}
			else//3.直接从custom中查找
			{
				DMTransNodePtr pNode = pLanguageItem->FindTransNodePtr(strNodeName);
				if (NULL != pNode)
				{
					pNode->GetObjByKey(strSrc,strTrans);
				}
			}

			//4.如果转换为空,则返回原始
			if (strTrans.IsEmpty())
			{
				strTrans = strSrc;
			}
		} while (false);
		return strTrans;
	}

	DMLanguageItemPtr DMTransImpl::FindLanguageItemPtr(CStringW strLanguageName)
	{
		DMLanguageItemPtr pFindItem = NULL;
		do 
		{
			if (strLanguageName.IsEmpty())
			{
				break;
			}

			DMLanguageItem Item(strLanguageName);
			int iElement = FindObj(&Item);
			if (DM_INVALID_VALUE == iElement)
			{
				break;
			}
			pFindItem = GetObj(iElement);
		} while (false);
		return pFindItem;
	}

	void DMTransImpl::PreArrayObjRemove(const DMLanguageItemPtr &obj)
	{
		delete obj;
	}

	bool DMTransImpl::EqualArrayObj(const DMLanguageItemPtr &objsrc, const DMLanguageItemPtr &objdest)
	{
		if (0 == objsrc->m_strLanguageName.CompareNoCase(objdest->m_strLanguageName))
		{
			return true;
		}
		return false;
	}

}//namespace DM

#endif

