
//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSingletonT.h 
// File Des: 单例模式
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-9-12	1.0			
//      guoyou		2015-01-09	1.1	           增加帮助chm注释
//-------------------------------------------------------
#pragma once
#include <assert.h>

namespace DM
{
	/// <summary>
	///		单例模式模板类,特别注意:跨模块传递时,T类需要重新实现static T& getSingleton()及static T* getSingletonPtr()
	/// </summary>
	template <typename T> 
	class DMSingletonT
	{
	public:
		static T* ms_Singleton;

	public:
		DMSingletonT(void)
		{
			DMASSERT(!ms_Singleton);
			ms_Singleton = static_cast<T*>(this);
		}
		virtual ~DMSingletonT(void)
		{
			DMASSERT( ms_Singleton );
			ms_Singleton = 0;
		}

		/// <summary>
		///		获取单例模式的对象
		/// </summary>
		/// <returns>单例模式的对象</returns>
		static T& getSingleton()
		{
			DMASSERT(ms_Singleton);
			return (*ms_Singleton);
		}

		/// <summary>
		///		获取单例模式的对象指针
		/// </summary>
		/// <returns>单例模式的对象指针</returns>
		static T* getSingletonPtr()
		{
			return (ms_Singleton);
		}

	private:
		DMSingletonT& operator=(const DMSingletonT&)
		{
			return *this;
		}

		DMSingletonT(const DMSingletonT&) {}
	};

	template<class T>
	typename T* DMSingletonT<T>::ms_Singleton = 0;

}//namespace DM

