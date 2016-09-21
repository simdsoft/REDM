//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMLock.h 
// File Des: 自动锁
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-01-11	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		锁定模板类，使用<see cref="DMAutoLock"/>实现自动锁定、释放
	/// </summary>
	class DM_EXPORT DMLock
	{
	public:
		DMLock()
		{
			::InitializeCriticalSection(&m_sesion);
		}
		~DMLock()
		{
			::DeleteCriticalSection(&m_sesion);
		}
	public:
		/// -------------------------------------------------
		/// @brief 锁定
		/// @return 锁定
		void lock(void)
		{
			::EnterCriticalSection(&m_sesion);
		}

		/// -------------------------------------------------
		/// @brief 解锁
		/// @return 解锁
		void unlock(void)
		{
			::LeaveCriticalSection(&m_sesion);
		}

		/// -------------------------------------------------
		/// @brief 尝试锁定
		/// @return 尝试锁定
		bool tryLock()
		{
			return (TRUE==::TryEnterCriticalSection(&m_sesion));
		}

	public:
		CRITICAL_SECTION			m_sesion;
	};

	/// <summary>
	///		自动锁定模板类，用于<see cref="DMLock"/>实现自动锁定、释放
	/// </summary>
	/// <remarks>
	///		DMLock m_pLock,临时声明变量DMAutoLock autolock(m_pLock);可在作用域控制自动锁
	/// </remarks>
	class DM_EXPORT DMAutoLock
	{
	public:
		DMAutoLock(IN DMLock* pLock)
			:m_pLock(pLock)
		{
			if (m_pLock)	
			{
				m_pLock->lock();
			}
		}
		~DMAutoLock()
		{
			if (m_pLock) 
			{
				m_pLock->unlock();
			}
		}

	public:
		DMLock*						m_pLock;
	};

}//namespace DM