//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMBase.h 
// File Des: 所有注册类的基类
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-14	1.0		
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		xml解析类的基类，用于所有数据类使用XML解析宏
	/// </summary>
	class DM_EXPORT DMDataBase:public DMRefNum
	{
	public:
		virtual DMCode InitDMData(DMXmlNode &XmlNode);															///<加载、解析XML数据
		virtual DMCode OnAttributeStart(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml);					///<解析XML的属性数据前触发
		virtual DMCode SetAttribute(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml);						///<解析XML的属性数据
		virtual DMCode OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr);    ///<解析完一个XML属性后触发
		virtual DMCode DefAttributeProc(LPCWSTR pszAttribute, LPCWSTR pszValue, bool bLoadXml);					///<默认解析XML的属性数据
		virtual DMCode OnFinished(DMXmlNode &XmlNode);															///<数据全部解析完后处理
		virtual DMCode SendExpandInfo(WPARAM wp, LPARAM lp){return DM_ECODE_NOTIMPL;}							///<扩展接口
	};

	/// <summary>
	///		所有注册类、DUI类的基类
	/// </summary>
	class DM_EXPORT DMBase:public DMDataBase
	{
	public:
		static LPCWSTR GetClassName();			///<取得注册类名
		virtual LPCWSTR V_GetClassName();       ///<取得注册类名, 这是虚函数
		static LPCWSTR GetBaseClassName();		///<取得注册类父类名
		virtual bool IsClass(LPCWSTR lpszName);	///<是否属于此窗口类向上链
		static int GetClassType();              ///<取得窗口类型
		virtual int V_GetClassType();			///<取得窗口类型，和GetClassType一样，但这是个虚函数，用于基指针指向子类时动态调用
	};

	/// <summary>
	///		可注册类的classtype
	/// </summary>
	/// <remarks>
	///		所有注册类均在Modeules中有内部实现,以做参考
	/// </remarks>
	enum DMREGTYPE
	{
		DMREG_Unknown                           = 0,	///<此类型仅供内部使用
		DMREG_Skin								= 1,    ///<皮肤类,指单张图片的绘制方式,如平铺、拉伸、9宫格
		DMREG_Style                             = 2,	///<样式类
		DMREG_Attribute                         = 3,	///<XML解析类
		DMREG_Window                            = 4,    ///<DUI窗口类
		DMREG_Layout                            = 5,    ///<锚点布局类
		DMREG_ImgDecoder                        = 6,    ///<解码类
		DMREG_Res		                        = 7,    ///<资源打包类
		DMREG_Render                            = 8,    ///<渲染类
		DMREG_Draw                              = 9,    ///<绘制类
		DMREG_Log	                            = 10,    ///<LOG类
		DMREG_FlowLayout                        = 11,   ///<流式布局类,基于DUIWindow实现
		DMREG_ToolTip                           = 12,   ///<tooltip类
		DMREG_Animate                           = 13,   ///<动画注册类
		DMREG_Script                            = 14,   ///<脚本类
		DMREG_Trans                             = 15,   ///<翻译类
		
		DMREG_MAX, 
	};

/// 所有基于DMBase的子类必须定义此宏,前两个static,后两个由子类重载了classtype只允许使用DMREGTYPE中的类型
#define DMDECLARE_CLASS_NAME(theclass, classname, classtype) \
public: \
	static LPCWSTR GetClassName() \
	{ \
		return classname; \
	} \
	virtual LPCWSTR V_GetClassName() \
	{ \
		return classname; \
	} \
	static LPCWSTR GetBaseClassName() \
	{ \
		return __super::GetClassName(); \
	} \
	virtual bool IsClass(LPCWSTR lpszName) \
	{ \
		if (0 == _wcsicmp(GetClassName(), lpszName)) \
			return true; \
		return __super::IsClass(lpszName);\
	} \
	static int GetClassType() \
	{ \
		return classtype; \
	}\
	virtual int V_GetClassType()\
	{\
		return classtype;\
	}
	




}//namespace DM