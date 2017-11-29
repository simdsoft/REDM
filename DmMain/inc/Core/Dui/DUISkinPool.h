//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUISkinPool.h 
// File Des:  管理所有skin
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
	///		Skin的池对象,用于支持局部skin
	/// </summary>
	class DMSkinPoolItem:public DMRefNum,public DMMapT<CStringW,IDMSkinPtr>
	{
	public:
		virtual ~DMSkinPoolItem();

	public:// 辅助重载函数
	   void PreMapKeyRemove(const IDMSkinPtr &obj);
	};
	typedef DMSkinPoolItem* DMSkinPoolItemPtr; 

	/// <summary>
	///		管理Skin的池列表
	/// </summary>
	/// <remarks>
	///		采用池列表的好处是可以移除某些局部的池,如某个skin池中所有对象只用某个窗口，当窗口销毁后，就可以考虑把这个池移除
	/// </remarks>
	class DM_EXPORT DUISkinPool:public DMMapT<CStringW, DMSkinPoolItemPtr>
	{
	public:
		~DUISkinPool();
	public:
		/// -------------------------------------------------
		/// @brief  加入一个skin池，如skin池已存在,则加入解析的项
		/// @param[in]  XmlNode		xml节点
		/// @return  DMCode
		DMCode AddSkinPoolItem(DMXmlNode &XmlNode);
		
		/// -------------------------------------------------
		/// @brief  移除一个skin池，如skin池已存在,则加入解析的项
		/// @param[in]  lpszName	skin池的名称
		/// @return  DMCode
		DMCode RemoveSkinPoolItem(LPCWSTR lpszName);

		/// -------------------------------------------------
		/// @brief  删除所有的skin池，除了指定的key以外
		/// @param[in]  lpszName			被排除的skin池的名称
		/// @return DMCode
		DMCode RemoveAllSkinPoolItemExcept(LPCWSTR lpszName);
		
		/// -------------------------------------------------
		/// @brief  用于换肤
		/// @param[in]  wp		需要更新的res字符串，以;分开
		/// @param[in]  lp		需要更新的res字符串长度
		/// @return  DMCode
		DMCode UpdateSkin(WPARAM wp, LPARAM lp);
		bool   IsNeedUpdateSkin(IDMSkinPtr pSkin);  ///< 当前skin指针是否需要更新
		DMCode ClearUpdateSkinArray();				///< 清空需更新的skin列表 

		/// -------------------------------------------------
		/// @brief			把指定buf设置到skin池中
		/// @param[in]		pBuf				图片内存
		/// @param[in]		bufLen				图片内存大小
		/// @param[in]		pszType				图片类型，一般默认为png
		/// @param[in]		lpszXml				描述skin的xml
		/// @param[in]		lpszPoolName		skin池名称
		DMCode AddSkin(void *pBuf,size_t bufLen,LPCWSTR pszType,
			LPCWSTR lpszXml,LPCWSTR lpszPoolName=NULL);

		/// -------------------------------------------------
		/// @brief  移除skin
		/// @param[in]  lpszKey			用于查找skin的key
		/// @param[in]  lpszName		用于查找skin池的name
		/// @remark 默认先在lpszName的skin池中移除，如果找不到,而且bLoopFind为真，就全部遍历找到并移除
		/// @return 从skin池中移除skin,并将引用计数减1
		DMCode RemoveSkin(LPCWSTR lpszKey,LPCWSTR lpszName,bool bLoopFind = true);

		/// -------------------------------------------------
		/// @brief  全部遍历删除skin
		/// @param[in]  lpszKey			用于查找skin的key
		/// @return IDMSkinPtr，为NULL表示失败
		DMCode RemoveSkinFromAll(LPCWSTR lpszKey);

		/// -------------------------------------------------
		/// @brief  查找skin
		/// @param[in]  lpszKey			用于查找skin的key
		/// @param[in]  lpszName		用于查找skin池的name
		/// @remark 默认先在lpszName的skin池中找，如果找不到,而且bLoopFind为真，就全部遍历找
		/// @return IDMSkinPtr，为NULL表示失败
		IDMSkinPtr FindSkin(LPCWSTR lpszKey,LPCWSTR lpszName,bool bLoopFind = true);

		/// -------------------------------------------------
		/// @brief  找查找skin
		/// @param[in]  lpszBuf			skin池的名称:skin的key
		/// @return IDMSkinPtr，为NULL表示失败
		IDMSkinPtr FindSkin(LPCWSTR lpszBuf,bool bLoopFind = true);

		/// -------------------------------------------------
		/// @brief  全部遍历找查找skin
		/// @param[in]  lpszKey			用于查找skin的key
		/// @return IDMSkinPtr，为NULL表示失败
		IDMSkinPtr FindSkinFromAll(LPCWSTR lpszKey);

	public:
		virtual void PreMapKeyRemove(const DMSkinPoolItemPtr &obj);

	public:// 更新皮肤列表
		CArray<IDMSkinPtr>			m_UpdateSkinArray;							///< 目前仅做skin更换使用，后续扩展成一个类
	};


}//namespace DM