#include "DmMainAfx.h"
#include "DMTransHelper.h"

namespace DM
{
//DMTransNode -----------------------------------------------------------------
	DMTransNode::DMTransNode(CStringW strName)
	{
		m_strNodeName = strName;
		if (m_strNodeName.IsEmpty())
		{
			m_strNodeName = L"custom";
		}
	}

//DMLanguageItem --------------------------------------------------------------
	DMLanguageItem::DMLanguageItem(CStringW strName)
	{
		DMASSERT_EXPR(!strName.IsEmpty(),L"语言包没有指定ID");
		m_strLanguageName = strName;
	}

	DMLanguageItem::~DMLanguageItem()
	{
		RemoveAll();
	}

	DMTransNodePtr DMLanguageItem::FindTransNodePtr(CStringW strNodeName)
	{
		DMTransNodePtr pFindItem = NULL;
		do 
		{
			DMTransNode LgNode(strNodeName);
			int iElement = FindObj(&LgNode);
			if (DM_INVALID_VALUE == iElement)
			{
				break;
			}
			pFindItem = GetObj(iElement);
		} while (false);
		return pFindItem;
	}

	void DMLanguageItem::PreArrayObjRemove(const DMTransNodePtr &obj)
	{
		delete obj;
	}

	bool DMLanguageItem::EqualArrayObj(const DMTransNodePtr &objsrc, const DMTransNodePtr &objdest)
	{
		if (0 == objsrc->m_strNodeName.CompareNoCase(objdest->m_strNodeName))
		{
			return true;
		}

		return false;
	}
}//namespace DM