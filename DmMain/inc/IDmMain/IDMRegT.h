//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMRegT.h 
// File Des: 用于外部注册辅助接口，每个外部注册的类必须基于DMObj
// File Summary: 使用方式：如class DMRegister:public DMBase
// 则DMRegT(DMRegister)就具有创建DMRegister对象的能力了，可以做为注册类传入
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-29	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class IDMReg;
	typedef IDMReg* IDMRegPtr; 
	/// <summary>
	///		每个注册类通过Helper操作必须有创建自身对象(NewObj)、复制自身的能力(Clone)
	/// </summary>
	class DM_EXPORT IDMReg
	{
	public:
		virtual ~IDMReg(){}
		virtual PVOID NewObj() = 0;				 ///< 创建自身对象(NewObj)
		virtual IDMRegPtr Clone() = 0;           ///< 复制自身

		// 取得基于DMBase注册类的标识、父类的标识、注册类型，建议使用DMDECLARE_CLASS_NAME宏
		virtual LPCWSTR GetClassName() = 0;		///< 取得注册类名
		virtual LPCWSTR GetBaseClassName() = 0; ///< 取得注册类父类名
		virtual int     GetClassType() = 0;     ///< 取得注册类型
	};

	/// <summary>
	///		辅助注册类，T必须是基于DMBase的类，或者实现了(static)GetClassName与GetBaseClassName的类
	/// </summary>
	template<class T=DMBase>
	class  DMRegHelperT:public IDMReg
	{
	public:// 默认构造函数是必须的，不然无法初始化对象
		DMRegHelperT(){}
		~DMRegHelperT(){}
	public:
		virtual PVOID NewObj()
		{
			return new T;
		}

		virtual IDMRegPtr Clone()
		{
			return new DMRegHelperT<T>;
		}

		virtual LPCWSTR GetClassName()
		{
			return T::GetClassName();
		}

		virtual LPCWSTR GetBaseClassName()
		{
			return T::GetBaseClassName();
		}

		virtual int GetClassType()
		{
			return T::GetClassType();	///注意，此处使用的是静态GetClassType，另外还有个虚GetClassType
		}
	};

}