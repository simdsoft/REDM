﻿//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIStylePool.h 
// File Des:  管理所有style,和skin池一致管理
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-30	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		Style的池对象,用于支持局部Style
	/// </summary>
	class DMStylePoolItem:public DMRefNum, public DMMapT<CStringA,DMXmlNode>
	{
	public:
		DMStylePoolItem(CStringA strName,DMXmlNode &xmlNodes);

	public:
		void AddStyles(DMXmlNode &XmlStyles);
		DMXmlNode GetStyle(CStringA strId);

	public:
		DMXmlDocument               m_Doc;
		CStringA                    m_strName;
	};
	typedef DMStylePoolItem* DMStylePoolItemPtr; 

	/// <summary>
	///		管理Style的池列表
	/// </summary>
	/// <remarks>
	///		采用池列表的好处是可以移除某些局部的池,如某个Style池中所有对象只用某个窗口，当窗口销毁后，就可以考虑把这个池移除
	/// </remarks>
	class DM_EXPORT DUIStylePool:public DMMapT<CStringA, DMStylePoolItemPtr>
	{
	public:
		~DUIStylePool();
	public:
		/// -------------------------------------------------
		/// @brief  加入一个style池，如style池已存在,则加入解析的项
		/// @param[in]  XmlNode		xml节点
		/// @return  DMCode
		DMCode AddStylePoolItem(DMXmlNode &XmlNode);

		/// -------------------------------------------------
		/// @brief  移除一个style池
		/// @param[in]  lpszName	style池的名称
		/// @return  DMCode
		DMCode RemoveStylePoolItem(LPCSTR lpszName);
	
		/// -------------------------------------------------
		/// @brief  移除所有的style池
		/// @return  DMCode
		DMCode RemoveAllStylePoolItem();

		/// -------------------------------------------------
		/// @brief  查找style
		/// @param[in]  lpszKey			用于查找style的key
		/// @param[in]  lpszName		用于查找style池的name
		/// @param[in]  bLoopFind		是否遍历所有池查找
		/// @remark 默认先在lpszName的style池中找，如果找不到,而且bLoopFind为真，就全部遍历找
		/// @return style的xmlnode
		DMXmlNode FindStyle(LPCSTR lpszKey,LPCSTR lpszName,bool bLoopFind = true);

		/// -------------------------------------------------
		/// @brief   查找style
		/// @param[in]  lpszBuf			style池的名称:style的key
		/// @return style的xmlnode
		DMXmlNode FindStyle(LPCSTR lpszBuf,bool bLoopFind = true);

		/// -------------------------------------------------
		/// @brief  全部遍历找查找style
		/// @param[in]  lpszKey			用于查找style的key
		/// @return style的xmlnode
		DMXmlNode FindStyleFromAll(LPCSTR lpszKey);

	public:
		virtual void PreMapKeyRemove(const DMStylePoolItemPtr &obj);

	};


}//namespace DM