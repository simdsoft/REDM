//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name:
// File Des: XML解析宏
// File Summary: 宏定义函数可使用MacroTool换成多行源码，方便调试
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
// XML属性声明宏--------------------------------------------------------------------------------------------
#define dm_parseint          DMAttributeDispatch::ParseInt  
#define dm_parsebool         DMAttributeDispatch::ParseBool		
#define dm_parsecolor        DMAttributeDispatch::ParseColor	
#define dm_parsesize         DMAttributeDispatch::ParseSize
#define dm_parsepoint        DMAttributeDispatch::ParsePoint
#define dm_parserect         DMAttributeDispatch::ParseRect


#define dm_wcsicmp			_wcsicmp
#define dm_wcsnicmp			_wcsnicmp

// IN_AttributeFun:  输入函数,先跳转到函数处理DMCode IN_AttributeFun(pszAttribute,pszValue,bLoadXml)
// IN_AttributeName：输入Attribute Name值，用于比较
// fun2_Value_Load: DMCode fun2(pszValue,bLoadXml)
// fun3_Value_Load: DMCode fun3(pszAttribute,pszValue,bLoadXml)// 用于多个fun2相似，统一处理
// 为防止转换工具把注释清除，使用/**/，且放在定义实体中间,同时注释中不会出现宏定义同样的名字
// OUT_Variable为要赋值的变量，IN_IsBreak是否中断解析
// In_EnumType为枚举类型，In_EnumValue为枚举值，In_EnumString标识In_EnumValue


//-------------------------------------------------------
// 调试时可使用MacroTool转换成换行的源代码调试
#define DM_BEGIN_ATTRIBUTES()\
	public:	/* _BEGIN_ATTRIBUTES()宏*/\
	virtual DMCode SetAttribute(LPCWSTR pszAttribute, LPCWSTR pszValue, bool bLoadXml = false)\
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
// 结束
#define DM_END_ATTRIBUTES()\
			iErr = __super::SetAttribute(pszAttribute, pszValue, bLoadXml);\
		} while (false);/*_END_ATTRIBUTES宏*/\
		return OnAttributeFinished(pszAttribute, pszValue, bLoadXml,iErr);\
	}
				
#define DM_END_BREAK()\
		} while (false);/*_END_BREAK宏*/\
		return OnAttributeFinished(pszAttribute, pszValue, bLoadXml,iErr);\
	}

//-------------------------------------------------------
// Custom部分
#define DM_CUSTOM_ATTRIBUTE(IN_AttributeName,fun2_Value_Load)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))	/* _CUSTOM_ATTRIBUTE()宏*/\
		{\
			iErr = fun2_Value_Load(pszValue,bLoadXml);\
			break;\
		}
	

#define DM_CUSTOMEX_ATTRIBUTE(IN_AttributeName,fun3_Value_Load)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))	/* _CUSTOM_ATTRIBUTE()宏*/\
		{\
			iErr = fun3_Value_Load(pszAttribute,pszValue,bLoadXml);\
			break;\
		}	

#define DM_CUSTOM_LIKE_ATTRIBUTE(IN_AttributeName, fun2_Value_Load)\
		if (0 == dm_wcsnicmp(IN_AttributeName, pszAttribute,countof(IN_AttributeName)-1))\
		{\
			iErr = fun2_Value_Load(pszValue, bLoadXml);\
			break;\
		}																				   

//-------------------------------------------------------
// Int = %d StringW  或 Int = 0x%Hex StringW,支持10进制或16进制表示
#define DM_INT_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_INT_ATTRIBUTE宏*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}																					

// DWORD = %u StringW 或 DWORD = 0x%Hex StringW,支持10进制或16进制表示
#define DM_DWORD_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_DWORD_ATTRIBUTE宏*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}																					

// UINT = %u StringW 或 UINT = 0x%Hex StringW,支持10进制或16进制表示
#define DM_UINT_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_UINT_ATTRIBUTE宏*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}						

// LONG = %u StringW 或 LONG = 0x%Hex StringW,支持10进制或16进制表示
#define DM_LONG_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
	if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_UINT_ATTRIBUTE宏*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}			

// WORD = %u StringW 或 WORD = 0x%Hex StringW,支持10进制或16进制表示
#define DM_WORD_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_WORD_ATTRIBUTE宏*/\
		{\
			int temp = OUT_Variable;\
			dm_parseint(pszValue,temp);\
			OUT_Variable = temp; \
			iErr = IN_IsBreak;\
			break;\
		}																								

// 字符"yes"、"true"表示真，其余先转换成数字，如>0则为真，否则均为假
#define DM_bool_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_BOOL_ATTRIBUTE宏*/\
		{\
			dm_parsebool(pszValue,OUT_Variable);\
			iErr = IN_IsBreak;\
			break;\
		}																								

//-------------------------------------------------------
// COLORREF_RGBA = #FFFFFFF 或 COLORREF_RGBA = rgb(ff,00,ff) 或 COLORREF_RGBA = rgba(ff,00,ff,ff),另支持一种特殊表示COLORREF_RGBA = "white"
#define DM_COLOR_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_COLOR_ATTRIBUTE宏*/\
		{\
			dm_parsecolor(pszValue, OUT_Variable);/*如失败,OUT_Variable原来的值不会被改变*/\
			iErr = IN_IsBreak;\
			break;\
		}																								

// Size = %d,%d
#define DM_SIZE_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_SIZE_ATTRIBUTE宏*/\
		{\
			dm_parsesize(pszValue, OUT_Variable);/*如失败,OUT_Variable原来的值不会被改变*/\
			iErr = IN_IsBreak;\
			break;\
		}				

// Point = %d,%d
#define DM_POINT_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_POINT_ATTRIBUTE宏*/\
		{\
			dm_parsepoint(pszValue, OUT_Variable);/*如失败,OUT_Variable原来的值不会被改变*/\
			iErr = IN_IsBreak;\
			break;\
		}				

// Rect = %d,%d,%d,%d
#define DM_RECT_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_RECT_ATTRIBUTE宏*/\
		{\
			dm_parserect(pszValue, OUT_Variable);/*如失败,OUT_Variable原来的值不会被改变*/\
			iErr = IN_IsBreak;\
			break;\
		}																								

// CStringW = CStringW
#define  DM_STRING_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_STRING_ATTRIBUTE宏*/\
		{\
			OUT_Variable = pszValue;\
			iErr = IN_IsBreak;\
			break;\
		}			
		

#define  DM_FONTPTR_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_FONTPTR_ATTRIBUTE宏*/\
		{\
			OUT_Variable = g_pDMApp->GetFont(pszValue);\
			iErr = IN_IsBreak;\
			break;\
		}			

#define  DM_SKINPTR_ATTRIBUTE(IN_AttributeName, OUT_Variable, IN_IsBreak)\
	if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))/*_FONTPTR_ATTRIBUTE宏*/\
		{\
			OUT_Variable = g_pDMApp->GetSkin(pszValue);\
			iErr = IN_IsBreak;\
			break;\
		}			

//-------------------------------------------------------
// ENUM部分
#define DM_ENUM_BEGIN(IN_AttributeName, In_EnumType, IN_IsBreak)\
		if (0 == dm_wcsicmp(IN_AttributeName, pszAttribute))\
		{\
			In_EnumType VarEnumType;\
			iErr = IN_IsBreak;																	

#define DM_ENUM_VALUE(In_EnumString,In_EnumValue)\
			if (0 == dm_wcsicmp(pszValue,In_EnumString))\
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
#define  DMAttrValueInit(cls,x)						__declspec(selectany)   wchar_t*  cls::x = DMInitAttrDispatch::GetAttrValue(L#cls,L#x);
}