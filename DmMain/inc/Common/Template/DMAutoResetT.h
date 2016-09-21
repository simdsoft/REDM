//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name:
// File Des: 自动恢复先前值
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-11-10	1.0		
//      guoyou		2015-01-09	1.1	           增加帮助chm注释
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		给对象临时设置新值，在作用域结束时恢复原始值
	/// </summary>
	template<class T>
	class DMAutoResetT
	{
	public:

		/// <summary>
		///     给对象临时设置新值
		/// </summary>
		/// <remarks>
		///		保存原始值在内部
		/// </remarks>
		/// <param name="pCopy">要设置的对象</param>
		/// <param name="NewValue">对象将设置的新值</param>
		/// <returns>无</returns>
		DMAutoResetT(T*  pCopy, T NewValue)
		{
			m_pCopy		= pCopy;     // 指针
			m_OrgValue  = *pCopy;	 // 保存原始值
			*m_pCopy    = NewValue;  // 赋新值
		}

		~DMAutoResetT()
		{
			*m_pCopy    = m_OrgValue; // 赋回值
		}

	public:
		T*         m_pCopy;
		T          m_OrgValue;
	};


}//namespace DM