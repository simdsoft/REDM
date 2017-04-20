
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
#include "DMMapT.h"
__pragma(warning(disable: 4661))
namespace DM
{
	/// <summary>
	///		多线程单例模式模板类
	/// </summary>
	template <typename T> 
	class DMSingletonT
	{
	public:
		static T* ms_Singleton;

	public:
		DMSingletonT(void)
		{
			if (m_Map.IsKeyExist(::GetCurrentThreadId()))
			{
				DMASSERT_EXPR(false,L"不要初始化多次DMApp对象");
			}
			T* pObj = static_cast<T*>(this);
			m_Map.AddKey(::GetCurrentThreadId(),pObj);
		}
		virtual ~DMSingletonT(void)
		{
			m_Map.RemoveKey(::GetCurrentThreadId());
		}

		/// <summary>
		///		获取单例模式的对象
		/// </summary>
		/// <returns>单例模式的对象</returns>
		static T& getSingleton()
		{
			T* pObj = NULL;
			if (!m_Map.GetObjByKey(::GetCurrentThreadId(),pObj))
			{
				DMASSERT_EXPR(false,L"当前线程未创建DMApp对象");
			}
			return *pObj;
		}

		/// <summary>
		///		获取单例模式的对象指针
		/// </summary>
		/// <returns>单例模式的对象指针</returns>
		static T* getSingletonPtr()
		{
			T* pObj = NULL;
			if (!m_Map.GetObjByKey(::GetCurrentThreadId(),pObj))
			{
				DMASSERT_EXPR(false,L"当前线程未创建DMApp对象");
			}
			return pObj;
		}

	private:
		DMSingletonT& operator=(const DMSingletonT&)
		{
			return *this;
		}

		DMSingletonT(const DMSingletonT&) {}
	public:
		static DMMapT<DWORD, T*>   m_Map;
	};

	template<class T> typename DMMapT<DWORD, T*> DMSingletonT<T>::m_Map;

}//namespace DM

