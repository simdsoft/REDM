//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name:
// File Des: XML������
// File Summary: �궨�庯����ʹ��MacroTool���ɶ���Դ�룬�������
// Cur Version: 1.1
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-15	1.0		
//-------------------------------------------------------
#pragma once

namespace DM
{
// XML����������--------------------------------------------------------------------------------------------
#define dm_parseint          DMAttributeDispatch::ParseInt  
#define dm_parsebool         DMAttributeDispatch::ParseBool		
#define dm_parsecolor        DMAttributeDispatch::ParseColor	
#define dm_parsesize         DMAttributeDispatch::ParseSize
#define dm_parsepoint        DMAttributeDispatch::ParsePoint
#define dm_parserect         DMAttributeDispatch::ParseRect


#define dm_wcsicmp			_wcsicmp
#define dm_wcsnicmp			_wcsnicmp

// IN_AttributeFun:  ���뺯��,����ת����������DMCode IN_AttributeFun(pszAttribute,pszValue,bLoadXml)
// IN_AttributeName������Attribute Nameֵ�����ڱȽ�
// fun2_Value_Load: DMCode fun2(pszValue,bLoadXml)
// fun3_Value_Load: DMCode fun3(pszAttribute,pszValue,bLoadXml)// ���ڶ��fun2���ƣ�ͳһ����
// Ϊ��ֹת�����߰�ע�������ʹ��/**/���ҷ��ڶ���ʵ���м�,ͬʱע���в�����ֺ궨��ͬ��������
// OUT_VariableΪҪ��ֵ�ı�����IN_IsBreak�Ƿ��жϽ���
// In_EnumTypeΪö�����ͣ�In_EnumValueΪö��ֵ��In_EnumString��ʶIn_EnumValue


//-------------------------------------------------------
// ����ʱ��ʹ��MacroToolת���ɻ��е�Դ�������
#define DM_BEGIN_ATTRIBUTES()\
	public:	/* _BEGIN_ATTRIBUTES()��*/\
	virtual DMCode SetAttribute(LPCSTR pszAttribute, LPCSTR pszValue, bool bLoadXml = false)\
	{\
		DMCode iErr = DM_ECODE_FAIL;\
		do \
		{\
			OnAttributeStart(pszAttribute,pszValue,bLoadXml);

#define DM_CHAIN_ATTRIBUTTE(IN_AttributeName)\
		if (IN_AttributeName && DM_ECODE_FAIL != (iErr=IN_AttributeName->SetAttribute(pszAttribute,pszValue,bLoadXml)))\
		{\
			break;\
		}

#define DM_CHAINFUN_ATTRIBUTTE(IN_AttributeFun)\
		if (DM_ECODE_FAIL != (iErr=IN_AttributeFun(pszAttribute,pszValue,bLoadXml)))\
		{\
			break;\
		}
//-------------------------------------------------------
// ����
#define DM_END_ATTRIBUTES()\
			iErr = __super::SetAttribute(pszAttribute, pszValue, bLoadXml);\
		} while (false);/*_END_ATTRIBUTES��*/\
		return OnAttributeFinished(pszAttribute, pszValue, bLoadXml,iErr);\
	}
				
#define DM_END_BREAK()\
		} while (false);/*_END_BREAK��*/\
		return OnAttributeFinished(pszAttribute, pszValue, bLoadXml,iErr);\
	}

//-------------------------------------------------------
// Custom����
#define DM_CUSTOM_ATTRIBUTE(IN_AttributeName,fun2_Value_Load)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))	/* _CUSTOM_ATTRIBUTE()��*/\
		{\
			iErr = fun2_Value_Load(pszValue,bLoadXml);\
			break;\
		}
	

#define DM_CUSTOMEX_ATTRIBUTE(IN_AttributeName,fun3_Value_Load)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))	/* _CUSTOM_ATTRIBUTE()��*/\
		{\
			iErr = fun3_Value_Load(pszAttribute,pszValue,bLoadXml);\
			break;\
		}	

#define DM_CUSTOM_LIKE_ATTRIBUTE(IN_AttributeName, fun2_Value_Load)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute,countof(IN_AttributeName)-1))\
		{\
			iErr = fun2_Value_Load(pszValue, bLoadXml);\
			break;\
		}																				   

//-------------------------------------------------------
// Int = %d StringW  �� Int = 0x%Hex StringW,֧��10���ƻ�16���Ʊ�ʾ
#define DM_INT_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_INT_ATTRIBUTE��*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}																					

// DWORD = %u StringW �� DWORD = 0x%Hex StringW,֧��10���ƻ�16���Ʊ�ʾ
#define DM_DWORD_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_DWORD_ATTRIBUTE��*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}																					

// UINT = %u StringW �� UINT = 0x%Hex StringW,֧��10���ƻ�16���Ʊ�ʾ
#define DM_UINT_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_UINT_ATTRIBUTE��*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}						

// LONG = %u StringW �� LONG = 0x%Hex StringW,֧��10���ƻ�16���Ʊ�ʾ
#define DM_LONG_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
	if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_UINT_ATTRIBUTE��*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}			

// WORD = %u StringW �� WORD = 0x%Hex StringW,֧��10���ƻ�16���Ʊ�ʾ
#define DM_WORD_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_WORD_ATTRIBUTE��*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}																								

// �ַ�"yes"��"true"��ʾ�棬������ת�������֣���>0��Ϊ�棬�����Ϊ��
#define DM_bool_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_BOOL_ATTRIBUTE��*/\
		{\
			dm_parsebool(pszValue,OUT_Variable);\
			iErr = IN_IsBreak;\
			break;\
		}																								

//-------------------------------------------------------
// COLORREF_RGBA = #FFFFFFF �� COLORREF_RGBA = rgb(ff,00,ff) �� COLORREF_RGBA = rgba(ff,00,ff,ff),��֧��һ�������ʾCOLORREF_RGBA = "white"
#define DM_COLOR_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_COLOR_ATTRIBUTE��*/\
		{\
			dm_parsecolor(pszValue, OUT_Variable);/*��ʧ��,OUT_Variableԭ����ֵ���ᱻ�ı�*/\
			iErr = IN_IsBreak;\
			break;\
		}																								

// Size = %d,%d
#define DM_SIZE_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_SIZE_ATTRIBUTE��*/\
		{\
			dm_parsesize(pszValue, OUT_Variable);/*��ʧ��,OUT_Variableԭ����ֵ���ᱻ�ı�*/\
			iErr = IN_IsBreak;\
			break;\
		}				

// Point = %d,%d
#define DM_POINT_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_POINT_ATTRIBUTE��*/\
		{\
			dm_parsepoint(pszValue, OUT_Variable);/*��ʧ��,OUT_Variableԭ����ֵ���ᱻ�ı�*/\
			iErr = IN_IsBreak;\
			break;\
		}				

// Rect = %d,%d,%d,%d
#define DM_RECT_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_RECT_ATTRIBUTE��*/\
		{\
			dm_parserect(pszValue, OUT_Variable);/*��ʧ��,OUT_Variableԭ����ֵ���ᱻ�ı�*/\
			iErr = IN_IsBreak;\
			break;\
		}																								

// CStringW = CStringW
#define  DM_STRING_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_STRING_ATTRIBUTE��*/\
		{\
			OUT_Variable = pszValue;\
			iErr = IN_IsBreak;\
			break;\
		}			

#define  DM_WSTRING_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_STRING_ATTRIBUTE��*/\
		{\
			OUT_Variable = DMCA2W(pszValue, -1, CP_UTF8);\
			iErr = IN_IsBreak;\
			break;\
		}	
		
#define  DM_FONTPTR_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_FONTPTR_ATTRIBUTE��*/\
		{\
			OUT_Variable = g_pDMApp->GetFont(pszValue);\
			iErr = IN_IsBreak;\
			break;\
		}			

#define  DM_SKINPTR_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
	if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))/*_FONTPTR_ATTRIBUTE��*/\
		{\
			OUT_Variable = g_pDMApp->GetSkin(pszValue);\
			iErr = IN_IsBreak;\
			break;\
		}			

//-------------------------------------------------------
// ENUM����
#define DM_ENUM_BEGIN(IN_AttributeName, In_EnumType, IN_IsBreak)\
		if (0 == dm_xmlstrcmp(IN_AttributeName, pszAttribute))\
		{\
			In_EnumType VarEnumType;\
			iErr = IN_IsBreak;																	

#define DM_ENUM_VALUE(In_EnumString,In_EnumValue)\
			if (0 == dm_xmlstrcmp(pszValue,In_EnumString))\
			{\
				VarEnumType = In_EnumValue;\
			}\
			else																				

#define DM_ENUM_END(OUT_Variable)\
			{\
				break;\
			}\
			OUT_Variable = VarEnumType;\
			break;\
		}																																										
}// namespace DM

namespace DMAttr
{
#define  DMAttrValueInit(cls,x)						__declspec(selectany)   const char*  cls::x = DMInitAttrDispatch::GetAttrValue(#cls,#x);
}