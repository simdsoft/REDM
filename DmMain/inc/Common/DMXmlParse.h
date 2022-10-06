//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMXmlParse.h 
// File Des:  ��װxml������ʵ���࣬�����ɿ��ǳ�ɽӿ�
// File Summary: ��װ�ĺô��ǽ�������ʹ��tinyxml/rapidxml�ȷ����滻,ѧϰʹ�÷�ʽ���Բο�gtest-DMXmlParse_Test.cpp
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-10-22	1.0			
//-------------------------------------------------------
#pragma once
#include "pugixml/pugixml.hpp"

namespace DM
{
	class DMXmlNode;
	void DM_EXPORT Init_Debug_XmlBuf(DMXmlNode &XmlNode);
	/// <summary>
	///		��װ��xml attribute
	/// </summary>
	class DM_EXPORT DMXmlAttribute
	{
	public:
		DMXmlAttribute();

		/// -------------------------------------------------
		/// @brief		�Ƿ���Ч����
		/// @return		true:��Ч,false:��Ч
		bool IsValid() const;

		/// -------------------------------------------------
		/// @brief		�Ƿ���ͬ
		/// @param[in]	other    ���ڱȽϵ�<see cref="DMXmlAttribute"/>����
		/// @return		true:��ͬ,false:��ͬ
		bool IsEqual(const DMXmlAttribute &other) const;

		/// -------------------------------------------------
		/// @brief		ȡ��attribute����
		/// @return		����attribute����
		LPCSTR GetName() const;

		/// -------------------------------------------------
		/// @brief		ȡ��attribute����
		/// @return		����attribute����
		LPCSTR GetValue() const;

		/// -------------------------------------------------
		/// @brief		ȡ����һ��attribute
		/// @return		������һ��attribute
		DMXmlAttribute NextAttribute() const;

		/// -------------------------------------------------
		/// @brief		ȡ����һ��attribute
		/// @return		������һ��attribute
		DMXmlAttribute PreviousAttribute() const;

	public:
		pugi::xml_attribute				m_XmlAttrbute;
	};
	typedef DMXmlAttribute* DMXmlAttributePtr;

	/// <summary>
	///		��װ��xml node
	/// </summary>
	class DM_EXPORT DMXmlNode
	{
	public:

	public:
		DMXmlNode();

		/// -------------------------------------------------
		/// @brief		�Ƿ���Ч����
		/// @return		true:��Ч,false:��Ч
		bool IsValid() const;

		/// -------------------------------------------------
		/// @brief		�Ƿ���ͬ
		/// @param[in]	other Ҫ�Ƚϵ�Node����
		/// @return		true:��ͬ,false:��ͬ
		bool IsEqual(const DMXmlNode &other) const;

		/// -------------------------------------------------
		/// @brief		�Ƿ���ͬ�ĸ������Ƿ�Ϊͬһ��xmldoc
		/// @param[in]	other Ҫ�Ƚϵ�Node����
		/// @return		true:��ͬ,false:��ͬ
		bool IsEqualRoot(const DMXmlNode &other) const;

		/// -------------------------------------------------
		/// @brief		��һ����Node
		/// @param[in]	XmlTag Node������
		/// @return		��һ����Node
		DMXmlNode FirstChild(LPCSTR XmlTag = NULL) const;

		/// -------------------------------------------------
		/// @brief		��һ���ֵ�Node
		/// @param[in]	XmlTag Node������
		/// @return		��һ���ֵ�Node
		DMXmlNode PreviousSibling(LPCSTR XmlTag = NULL) const;

		/// -------------------------------------------------
		/// @brief		��һ���ֵ�Node
		/// @param[in]	XmlTag Node������
		/// @return		��һ���ֵ�Node
		DMXmlNode NextSibling(LPCSTR XmlTag = NULL) const;

		/// -------------------------------------------------
		/// @brief		ȡ��node����
		/// @return		����node����
		LPCSTR GetName() const;

		/// -------------------------------------------------
		/// @brief		����node����
		/// @return		����true.false
		bool SetName(LPCSTR lpszName);

		/// -------------------------------------------------
		/// @brief		ȡ��node����
		/// @return		����node����
		LPCSTR GetValue() const;

		/// -------------------------------------------------
		/// @brief		��һ��attribute
		/// @remark		���ڱ�������attribute
		/// @return		��һ��attribute
		DMXmlAttribute FirstAttribute() const;

		/// -------------------------------------------------
		/// @brief		ָ�����ֵ�attribute������
		/// @param[in]	name attribute������
		/// @return		ָ�����ֵ�attribute�����ݣ�LPCWSTR��
		LPCSTR Attribute(LPCSTR name) const;

		/// -------------------------------------------------
		/// @brief		ָ�����ֵ�attribute������
		/// @param[in]	name attribute������
		/// @return		ָ�����ֵ�attribute�����ݣ�int��
		int AttributeInt(LPCSTR name) const;

		/// -------------------------------------------------
		/// @brief		ָ�����ֵ�attribute������
		/// @param[in]	name attribute������
		/// @return		ָ�����ֵ�attribute�����ݣ�int64��
		INT64 AttributeInt64(LPCSTR name) const;

		/// -------------------------------------------------
		/// @brief  ����һ���µĽڵ�
		/// @param[in]	name �½ڵ������
		/// @param[in]	pXmlNode ������ľɽ�㣬ΪNULL��ʾֱ�Ӳ���
		/// @param[in]	bAfter ����ľɽ��ǰ��
		/// @return	�½ڵ�
		DMXmlNode InsertChildNode(LPCSTR name,DMXmlNode* pXmlNode = NULL,bool bAfter = true);

		/// -------------------------------------------------
		/// @brief  �������ط�����һ���½ڵ�
		/// @param[in]	pCopyXmlNode �½ڵ�
		/// @param[in]	pXmlNode ������ľɽ�㣬ΪNULL��ʾֱ�Ӳ���
		/// @param[in]	bAfter ����ľɽ��ǰ��
		/// @return	�½ڵ�
		DMXmlNode InsertCopyChildNode(DMXmlNode* pCopyXmlNode,DMXmlNode* pXmlNode = NULL,bool bAfter = true);

		/// -------------------------------------------------
		/// @brief		����node
		/// @param[in]	XmlAppend �����Ƶ�node
		/// @return		��
		void Append_Copy(DMXmlNode &XmlAppend);

		/// -------------------------------------------------
		/// @brief  �Ƴ�һ���ӽڵ�
		/// @ @return	
		bool RemoveChildNode(DMXmlNode* pXmlNode);

		/// -------------------------------------------------
		/// @brief  ������޸�һ�����ԣ����Դ������޸�,���Բ��������������룩
		/// @param[in]	name  ���Ե�����
		/// @param[in]	value ���Ե�ֵ
		/// @return	�½ڵ�
		bool SetAttribute(LPCSTR name, LPCSTR value,bool bAppend = true);
		bool SetAttribute(LPCSTR name, const CStringW& value, bool bAppend = true) { return SetAttribute(name, DMW2A(value), bAppend); }
		bool SetAttributeInt(LPCSTR name, int value,bool bAppend = true);
		bool SetAttributeInt64(LPCSTR name, INT64 value,bool bAppend = true);

		/// -------------------------------------------------
		/// @brief  �Ƴ�name���ֵ�����
		/// @param[in]	name  ���Ե�����
		bool RemoveAttribute(LPCSTR name);

	public:/// ����,����Debug��ӡXML����
		bool GetXmlContent(CStringA &strDebug);

	public:
		pugi::xml_node				m_XmlNode;						///< Ĭ�Ϲ���m_XmlNode._rootΪ0
#ifdef _DEBUG
		CStringA                    m_strDebugBuf;					///< Debug�¸�������,��������������Եȸı�xml�ĺ���,��������¸�node��m_strDebugBuf,���Լ��ֶ�����һ��GetXmlContentȥ�鿴
#endif 
	};
	typedef DMXmlNode* DMXmlNodePtr;

	/// <summary>
	///		��װ��xml Doc ����
	/// </summary>
	class DM_EXPORT DMXmlDocHandle
	{
	public:
		DMXmlDocHandle(pugi::xml_document*pXmlDoc);
	public:
		/// -------------------------------------------------
		/// @brief		��utf8�ļ���ʽ����
		/// @param[in]	pszFilePath		�ļ�·�����ڲ���ǿ��ת���ɾ���·��
		/// @remark     ������utf8��ʽ����Ȼ�����޷�ʶ��
		/// @return		true:�ɹ�,false:ʧ��
		bool LoadFromFile(LPCWSTR pszFilePath);

		/// -------------------------------------------------
		/// @brief		��utf8�ڴ��ʽ����
		/// @param[in]	pContent		�ڴ���ʼ
		/// @param[in]	size			�ڴ��С
		/// @remark     ������utf8��ʽ����Ȼ�����޷�ʶ��
		/// @return		true:�ɹ�,false:ʧ��
		bool LoadFromBuffer(const PVOID pContent, size_t size);

		bool IsValid() const;							 ///< �Ƿ����
		bool IsError() const;							 ///< �Ƿ����
	
		bool IsEqual(const DMXmlDocHandle &other) const; ///< �Ƿ���ͬ

		/// -------------------------------------------------
		/// @brief  ��ø�Root��Ҳ���Ǳ��������
		/// @return	���ظ�node
		DMXmlNode Root(LPCSTR XmlTag = NULL) const;

		/// -------------------------------------------------
		/// @brief  ��ø�Base����ʼֵ
		/// @return	����base
		DMXmlNode Base() const;

		/// -------------------------------------------------
		/// @brief  �����ļ�
		/// @return	�ɹ�,ʧ��
		bool SaveXml(LPCWSTR lpszPath);
	
		/// -------------------------------------------------
		/// @brief		ͨ��XmlNode����,����֧��XMLģ��
		/// @param[in]	XmlAppend �����Ƶ�node
		/// @return		true:�ɹ�,false:ʧ��
		bool ResetByXmlNode(DMXmlNode &XmlAppend);

	protected:
		pugi::xml_document*                 m_pXmlDoc;
		bool                                m_bXmlErr;

	};

	/// <summary>
	///		��װ��xml Doc
	/// </summary>
	class DM_EXPORT DMXmlDocument
		:public DMXmlDocHandle
	{
	public:
		DMXmlDocument();
		~DMXmlDocument();
	};
}//namespace DM