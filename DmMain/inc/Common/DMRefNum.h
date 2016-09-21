//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMRefNum.h 
// File Des: SMart指针的基类
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		DMRefNum:SMart指针的基类,被<see cref="DMSmartPtrT"/>使用
	/// </summary>
	/// <remarks>
	///		引用计数是为了保证在释放时对象已不再使用 
	/// <remarks>
	class DM_EXPORT DMRefNum
	{
	public:

		/// <summary>
		///     增加引用计数
		/// </summary>
		/// <returns>增加后的引用计数Num</returns>
		int AddRef(void){return ::InterlockedIncrement(&m_RefCount);}

		/// <summary>
		///     减少引用计数
		/// </summary>
		/// <remarks>
		///		为0时自动调用<see cref="OnFinalRelease"/>
		/// </remarks>
		/// <returns>减少后的引用计数Num</returns>
		int Release(void)
		{
			int r = ::InterlockedDecrement(&m_RefCount);
			if (0 == r)
			{
				OnFinalRelease();
			}

			return r;
		}

		/// <summary>
		///     获取当前的引用计数
		/// </summary>
		/// <returns>当前的引用计数Num</returns>
		int GetRefCount(void) const {return m_RefCount;}

		/// <summary>
		///     设置引用计数
		/// </summary>
		/// <remarks>
		///		用于强制调整引用计数
		/// </remarks>
		/// <param name="RefCount">引用计数Num</param>
		/// <returns>减少后的引用计数Num</returns>
		void SetRefCount(long RefCount) {m_RefCount = RefCount;}

		/// <summary>
		///     在引用计算为0时被调用
		/// </summary>
		/// <remarks>
		///		默认为删除自身，子类可重载
		/// </remarks>
		/// <returns>无</returns>
		virtual void OnFinalRelease()	
		{
			delete this;
		}

	protected:
		DMRefNum(void):m_RefCount(1){}
		virtual ~DMRefNum(void)
		{
			DMASSERT(0== m_RefCount);						// 此标志用于辅助测试我们的资源都已正常释放 
		}

	public:
		volatile long              m_RefCount;
	};
}// namespace DM 