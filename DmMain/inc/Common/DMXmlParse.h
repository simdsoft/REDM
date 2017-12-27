//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMXmlParse.h 
// File Des:  封装xml解析的实现类，后续可考虑抽成接口
// File Summary: 封装的好处是解析可以使用tinyxml/rapidxml等方便替换,学习使用方式可以参考gtest-DMXmlParse_Test.cpp
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-10-22	1.0			
//-------------------------------------------------------
#pragma once
#include "pugixml.hpp"

namespace DM
{
	class DMXmlNode;
	void DM_EXPORT Init_Debug_XmlBuf(DMXmlNode &XmlNode);
	/// <summary>
	///		封装的xml attribute
	/// </summary>
	class DM_EXPORT DMXmlAttribute
	{
	public:
		DMXmlAttribute();

		/// -------------------------------------------------
		/// @brief		是否有效数据
		/// @return		true:有效,false:无效
		bool IsValid() const;

		/// -------------------------------------------------
		/// @brief		是否相同
		/// @param[in]	other    用于比较的<see cref="DMXmlAttribute"/>对象
		/// @return		true:相同,false:不同
		bool IsEqual(const DMXmlAttribute &other) const;

		/// -------------------------------------------------
		/// @brief		取得attribute名字
		/// @return		返回attribute名字
		LPCWSTR GetName() const;

		/// -------------------------------------------------
		/// @brief		取得attribute内容
		/// @return		返回attribute内容
		LPCWSTR GetValue() const;

		/// -------------------------------------------------
		/// @brief		取得下一个attribute
		/// @return		返回下一个attribute
		DMXmlAttribute NextAttribute() const;

		/// -------------------------------------------------
		/// @brief		取得上一个attribute
		/// @return		返回上一个attribute
		DMXmlAttribute PreviousAttribute() const;

	public:
		pugi::xml_attribute				m_XmlAttrbute;
	};
	typedef DMXmlAttribute* DMXmlAttributePtr;

	/// <summary>
	///		封装的xml node
	/// </summary>
	class DM_EXPORT DMXmlNode
	{
	public:

	public:
		DMXmlNode();

		/// -------------------------------------------------
		/// @brief		是否有效数据
		/// @return		true:有效,false:无效
		bool IsValid() const;

		/// -------------------------------------------------
		/// @brief		是否相同
		/// @param[in]	other 要比较的Node对象
		/// @return		true:相同,false:不同
		bool IsEqual(const DMXmlNode &other) const;

		/// -------------------------------------------------
		/// @brief		是否相同的根，即是否为同一个xmldoc
		/// @param[in]	other 要比较的Node对象
		/// @return		true:相同,false:不同
		bool IsEqualRoot(const DMXmlNode &other) const;

		/// -------------------------------------------------
		/// @brief		第一个子Node
		/// @param[in]	XmlTag Node的名字
		/// @return		第一个子Node
		DMXmlNode FirstChild(LPCWSTR XmlTag = NULL) const;

		/// -------------------------------------------------
		/// @brief		上一个兄弟Node
		/// @param[in]	XmlTag Node的名字
		/// @return		上一个兄弟Node
		DMXmlNode PreviousSibling(LPCWSTR XmlTag = NULL) const;

		/// -------------------------------------------------
		/// @brief		下一个兄弟Node
		/// @param[in]	XmlTag Node的名字
		/// @return		下一个兄弟Node
		DMXmlNode NextSibling(LPCWSTR XmlTag = NULL) const;

		/// -------------------------------------------------
		/// @brief		取得node名字
		/// @return		返回node名字
		LPCWSTR GetName() const;

		/// -------------------------------------------------
		/// @brief		设置node名字
		/// @return		返回true.false
		bool SetName(LPCWSTR lpszName);

		/// -------------------------------------------------
		/// @brief		取得node内容
		/// @return		返回node内容
		LPCWSTR GetValue() const;

		/// -------------------------------------------------
		/// @brief		第一个attribute
		/// @remark		用于遍历所有attribute
		/// @return		第一个attribute
		DMXmlAttribute FirstAttribute();

		/// -------------------------------------------------
		/// @brief		指定名字的attribute的内容
		/// @param[in]	name attribute的名字
		/// @return		指定名字的attribute的内容（LPCWSTR）
		LPCWSTR Attribute(LPCWSTR name);

		/// -------------------------------------------------
		/// @brief		指定名字的attribute的内容
		/// @param[in]	name attribute的名字
		/// @return		指定名字的attribute的内容（int）
		int AttributeInt(LPCWSTR name);

		/// -------------------------------------------------
		/// @brief		指定名字的attribute的内容
		/// @param[in]	name attribute的名字
		/// @return		指定名字的attribute的内容（int64）
		INT64 AttributeInt64(LPCWSTR name);

		/// -------------------------------------------------
		/// @brief  插入一个新的节点
		/// @param[in]	name 新节点的名字
		/// @param[in]	pXmlNode 被插入的旧结点，为NULL表示直接插入
		/// @param[in]	bAfter 插入的旧结点前后
		/// @return	新节点
		DMXmlNode InsertChildNode(LPCWSTR name,DMXmlNode* pXmlNode = NULL,bool bAfter = true);

		/// -------------------------------------------------
		/// @brief  从其他地方插入一个新节点
		/// @param[in]	pCopyXmlNode 新节点
		/// @param[in]	pXmlNode 被插入的旧结点，为NULL表示直接插入
		/// @param[in]	bAfter 插入的旧结点前后
		/// @return	新节点
		DMXmlNode InsertCopyChildNode(DMXmlNode* pCopyXmlNode,DMXmlNode* pXmlNode = NULL,bool bAfter = true);

		/// -------------------------------------------------
		/// @brief		复制node
		/// @param[in]	XmlAppend 被复制的node
		/// @return		无
		void Append_Copy(DMXmlNode &XmlAppend);

		/// -------------------------------------------------
		/// @brief  移除一个子节点
		/// @ @return	
		bool RemoveChildNode(DMXmlNode* pXmlNode);

		/// -------------------------------------------------
		/// @brief  插入或修改一个属性（属性存在则修改,属性不存在则在最后插入）
		/// @param[in]	name  属性的名字
		/// @param[in]	value 属性的值
		/// @return	新节点
		bool SetAttribute(LPCWSTR name, LPCWSTR value,bool bAppend = true);
		bool SetAttributeInt(LPCWSTR name, int value,bool bAppend = true);
		bool SetAttributeInt64(LPCWSTR name, INT64 value,bool bAppend = true);

		/// -------------------------------------------------
		/// @brief  移除name名字的属性
		/// @param[in]	name  属性的名字
		bool RemoveAttribute(LPCWSTR name);

	public:/// 辅助,用于Debug打印XML内容
		bool GetXmlContent(CStringW &strDebug);

	public:
		pugi::xml_node				m_XmlNode;						///< 默认构造m_XmlNode._root为0
#ifdef _DEBUG
		CStringW                    m_strDebugBuf;					///< Debug下辅助调试,如调用了设置属性等改变xml的函数,并不会更新父node的m_strDebugBuf,需自己手动调用一次GetXmlContent去查看
#endif 
	};
	typedef DMXmlNode* DMXmlNodePtr;

	/// <summary>
	///		封装的xml Doc 基类
	/// </summary>
	class DM_EXPORT DMXmlDocHandle
	{
	public:
		DMXmlDocHandle(pugi::xml_document*pXmlDoc);
	public:
		/// -------------------------------------------------
		/// @brief		以utf8文件格式加载
		/// @param[in]	pszFilePath		文件路径，内部会强制转换成绝对路径
		/// @remark     必须是utf8格式，不然中文无法识别
		/// @return		true:成功,false:失败
		bool LoadFromFile(LPCWSTR pszFilePath);

		/// -------------------------------------------------
		/// @brief		以utf8内存格式加载
		/// @param[in]	pContent		内存起始
		/// @param[in]	size			内存大小
		/// @remark     必须是utf8格式，不然中文无法识别
		/// @return		true:成功,false:失败
		bool LoadFromBuffer(const PVOID pContent, size_t size);

		bool IsValid() const;							 ///< 是否可用
		bool IsError() const;							 ///< 是否出错
	
		bool IsEqual(const DMXmlDocHandle &other) const; ///< 是否相同

		/// -------------------------------------------------
		/// @brief  获得根Root，也就是遍历的起点
		/// @return	返回根node
		DMXmlNode Root(LPCWSTR XmlTag = NULL) const;

		/// -------------------------------------------------
		/// @brief  获得根Base，初始值
		/// @return	返回base
		DMXmlNode Base() const;

		/// -------------------------------------------------
		/// @brief  保存文件
		/// @return	成功,失败
		bool SaveXml(LPCWSTR lpszPath);
	
		/// -------------------------------------------------
		/// @brief		通过XmlNode加载,用于支持XML模板
		/// @param[in]	XmlAppend 被复制的node
		/// @return		true:成功,false:失败
		bool ResetByXmlNode(DMXmlNode &XmlAppend);

	protected:
		pugi::xml_document*                 m_pXmlDoc;
		bool                                m_bXmlErr;

	};

	/// <summary>
	///		封装的xml Doc
	/// </summary>
	class DM_EXPORT DMXmlDocument
		:public DMXmlDocHandle
	{
	public:
		DMXmlDocument();
		~DMXmlDocument();
	};
}//namespace DM