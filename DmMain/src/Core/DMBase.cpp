#include "DmMainAfx.h"
#include "DMBase.h"

namespace DM
{
	// DMDataBase------------------------------------------
	DMCode DMDataBase::InitDMData(DMXmlNode &XmlNode)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (false == XmlNode.IsValid())
			{
				iErr = DMDATABASE_LOADDMDATA_PARAM_INVALID;
				break;
			}
			LPCSTR strValue = XmlNode.Attribute(DMAttr::DUIWindowAttr::STRING_style);
			if (*strValue  && 0 != dm_xmlstrcmp(XmlNode.FirstAttribute().GetName(),DMAttr::DUIWindowAttr::STRING_style))
			{
				XmlNode.RemoveAttribute(DMAttr::DUIWindowAttr::STRING_style);
				XmlNode.SetAttribute(DMAttr::DUIWindowAttr::STRING_style,strValue,false);// 把style插到最前面处理
			}

			DMXmlAttribute XmlAttribute = XmlNode.FirstAttribute();
			while (XmlAttribute.IsValid())
			{
				SetAttribute(XmlAttribute.GetName(), XmlAttribute.GetValue(), true);
				XmlAttribute = XmlAttribute.NextAttribute();
			}
			OnFinished(XmlNode);	
		} while (false);
		return iErr;
	}

	DMCode DMDataBase::OnAttributeStart(LPCSTR pszAttribute, LPCSTR pszValue,bool bLoadXml)
	{
		return DM_ECODE_FAIL;
	}

	DMCode DMDataBase::SetAttribute(LPCSTR pszAttribute, LPCSTR pszValue, bool bLoadXml)
	{
		return DefAttributeProc(pszAttribute, pszValue, bLoadXml);
	}

	DMCode DMDataBase::OnAttributeFinished(LPCSTR pszAttribute, LPCSTR pszValue,bool bLoadXml,DMCode iErr)
	{
		return iErr;
	}

	DMCode DMDataBase::DefAttributeProc(LPCSTR pszAttribute, LPCSTR pszValue, bool bLoadXml)
	{
		return DM_ECODE_FAIL;//未解析
	}

	DMCode DMDataBase::OnFinished(DMXmlNode &XmlNode)
	{
		return DM_ECODE_OK;
	}

	// DMBase------------------------------------------
	LPCSTR DMBase::GetClassName()
	{
		return NULL;
	}

	LPCSTR DMBase::V_GetClassName()
	{
		return NULL;
	}

	LPCSTR DMBase::GetBaseClassName()
	{
		return NULL;
	}

	int DMBase::GetClassType()
	{
		return DMREG_Unknown;
	}

	int DMBase::V_GetClassType()
	{
		return DMREG_Unknown;
	}

	bool DMBase::IsClass(LPCSTR lpszName)
	{
		return false;
	}

}//namespace DM