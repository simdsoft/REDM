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
			CStringW strValue = XmlNode.Attribute(DMAttr::DUIWindowAttr::STRING_style);
			if (!strValue.IsEmpty()&&0 != _wcsicmp(XmlNode.FirstAttribute().GetName(),DMAttr::DUIWindowAttr::STRING_style))
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

	DMCode DMDataBase::OnAttributeStart(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml)
	{
		return DM_ECODE_FAIL;
	}

	DMCode DMDataBase::SetAttribute(LPCWSTR pszAttribute, LPCWSTR pszValue, bool bLoadXml)
	{
		return DefAttributeProc(pszAttribute, pszValue, bLoadXml);
	}

	DMCode DMDataBase::OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr)
	{
		return iErr;
	}

	DMCode DMDataBase::DefAttributeProc(LPCWSTR pszAttribute, LPCWSTR pszValue, bool bLoadXml)
	{
		return DM_ECODE_FAIL;//未解析
	}

	DMCode DMDataBase::OnFinished(DMXmlNode &XmlNode)
	{
		return DM_ECODE_OK;
	}

	// DMBase------------------------------------------
	LPCWSTR DMBase::GetClassName()
	{
		return NULL;
	}

	LPCWSTR DMBase::V_GetClassName()
	{
		return NULL;
	}

	LPCWSTR DMBase::GetBaseClassName()
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

	bool DMBase::IsClass(LPCWSTR lpszName)
	{
		return false;
	}

}//namespace DM