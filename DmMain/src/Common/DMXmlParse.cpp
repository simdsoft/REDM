#include "DmMainAfx.h"
#include "DMXmlParse.h"

namespace DM
{

	void Init_Debug_XmlBuf(DMXmlNode &XmlNode)
	{
#if defined(_DEBUG)
		DUIWindow *pMain = g_pDMApp->FindDUIWnd(1);
		if (NULL == pMain																													 // ���ز��ʱ��,������δ��ʼ��
			||(0 != pMain->GetData("1C3A5807-CEE1-438C-BC46-624F74BDC8D1").CompareNoCase("440A2781-8BC2-4AC4-8225-9AC451FE42B4")))// �����ڳ�ʼ�˺�,�������ģʽ
		{// �����ģʽ�£��������ڷ�������ʱ���Ŵ˸�������Ȼ������Ӱ�����Ч��
			XmlNode.GetXmlContent(XmlNode.m_strDebugBuf);
		}
#endif	
	}
#define INIT_DEBUG_XMLBUF(XmlNode) Init_Debug_XmlBuf(XmlNode);

	// DMXmlAttribute---------------------------------------
	DMXmlAttribute::DMXmlAttribute()
	{
	}

	bool DMXmlAttribute::IsValid() const
	{
		return (!!m_XmlAttrbute);//˫!!
	}

	bool DMXmlAttribute::IsEqual(const DMXmlAttribute &other) const
	{
		return (m_XmlAttrbute == other.m_XmlAttrbute);
	}

	LPCSTR DMXmlAttribute::GetName() const
	{
		if (NULL == m_XmlAttrbute)
		{
			return NULL;
		}

		return m_XmlAttrbute.name().data();
	}

	LPCSTR DMXmlAttribute::GetValue() const
	{
		if (NULL == m_XmlAttrbute)
		{
			return NULL;
		}

		return m_XmlAttrbute.value().data();
	}

	DMXmlAttribute DMXmlAttribute::NextAttribute() const
	{
		DMXmlAttribute XmlAttribute;// Ĭ��һ����ʼֵ��XmlAttribute.m_XmlAttrbute = NULL
		if (m_XmlAttrbute)
		{
			XmlAttribute.m_XmlAttrbute = m_XmlAttrbute.next_attribute();
		}

		return XmlAttribute;
	}

	DMXmlAttribute DMXmlAttribute::PreviousAttribute() const
	{
		DMXmlAttribute XmlAttribute;// Ĭ��һ����ʼֵ��XmlAttribute.m_XmlAttrbute = NULL
		if (m_XmlAttrbute)
		{
			XmlAttribute.m_XmlAttrbute = m_XmlAttrbute.previous_attribute();
		}

		return XmlAttribute;
	}

	// DMXmlNode-------------------------------------------
	DMXmlNode::DMXmlNode()
	{
	}

	bool DMXmlNode::IsValid() const
	{
		return (!!m_XmlNode);//˫!!
	}

	bool DMXmlNode::IsEqual(const DMXmlNode &other) const
	{
		return (m_XmlNode == other.m_XmlNode);
	}

	bool DMXmlNode::IsEqualRoot(const DMXmlNode &other) const
	{
		bool bRet = false;
		do 
		{
			if (!IsValid()
				||!other.IsValid())
			{
				break;
			}
			__try
			{
				if (m_XmlNode.root() != other.m_XmlNode.root())
				{
					break;
				}
			}
			__except(EXCEPTION_EXECUTE_HANDLER)// ���other��XMLDOC����ǰdestory���ˣ���ʱ��������ֵ�ģ��������ڴ汻�ͷ��ˣ�������Ҫtry catch��ֹ�쳣
			{
				break;
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	DMXmlNode DMXmlNode::FirstChild(LPCSTR XmlTag/* = NULL*/) const
	{
		DMXmlNode XmlNode;
		if (m_XmlNode)
		{
			if (NULL == XmlTag)
			{
				XmlNode.m_XmlNode = m_XmlNode.first_child();
			}
			else
			{
				XmlNode.m_XmlNode = m_XmlNode.child(XmlTag);
			}
		}

		INIT_DEBUG_XMLBUF(XmlNode)
		return XmlNode;
	}

	DMXmlNode DMXmlNode::PreviousSibling(LPCSTR XmlTag/* = NULL*/) const
	{
		DMXmlNode XmlNode;
		if (m_XmlNode)
		{
			if (NULL == XmlTag)
			{
				XmlNode.m_XmlNode = m_XmlNode.previous_sibling();
			}
			else
			{
				XmlNode.m_XmlNode = m_XmlNode.previous_sibling(XmlTag);
			}
		}

		INIT_DEBUG_XMLBUF(XmlNode)
			return XmlNode;
	}

	DMXmlNode DMXmlNode::NextSibling(LPCSTR XmlTag/* = NULL*/) const
	{
		DMXmlNode XmlNode;
		if (m_XmlNode)
		{
			if (NULL == XmlTag)
			{
				XmlNode.m_XmlNode = m_XmlNode.next_sibling();
			}
			else
			{
				XmlNode.m_XmlNode = m_XmlNode.next_sibling(XmlTag);
			}
		}

		INIT_DEBUG_XMLBUF(XmlNode)
			return XmlNode;
	}

	LPCSTR DMXmlNode::GetName() const
	{
		if (!m_XmlNode)
		{
			return NULL;
		}

		auto name = m_XmlNode.name();
		if (name == "tabctrl")
			return name.data();
		return name.data();
	}


	bool DMXmlNode::SetName(LPCSTR lpszName)
	{
		bool bRet = false;
		do 
		{
			if (!m_XmlNode)
			{
				break;
			}
			bRet = m_XmlNode.set_name(lpszName);
		} while (false);
		return bRet;
	}

	LPCSTR DMXmlNode::GetValue() const
	{
		if (!m_XmlNode)
		{
			return NULL;
		}
		// ע����text,���ڵõ�<node>this is node<node/>�е�this is node
		return m_XmlNode.text().as_string().data();
	}

	DMXmlAttribute DMXmlNode::FirstAttribute() const
	{
		DMXmlAttribute Attribute;
		if (m_XmlNode)
		{
			Attribute.m_XmlAttrbute = m_XmlNode.first_attribute();
		}
		return Attribute;
	}

	LPCSTR DMXmlNode::Attribute(LPCSTR name) const
	{
		if (!m_XmlNode)
		{
			return NULL;
		}

		return m_XmlNode.attribute(name).as_string().data();
	}

	int DMXmlNode::AttributeInt(LPCSTR name) const
	{
		if (!m_XmlNode)
		{
			return NULL;
		}

		return m_XmlNode.attribute(name).as_int();
	}

	INT64 DMXmlNode::AttributeInt64(LPCSTR name) const
	{
		if (!m_XmlNode)
		{
			return NULL;
		}

		return m_XmlNode.attribute(name).as_llong();
	}

	DMXmlNode DMXmlNode::InsertChildNode(LPCSTR name,DMXmlNode* pXmlNode,bool bAfter /*= true*/)
	{
		DMXmlNode XmlElement;

		if (NULL == pXmlNode
			||!pXmlNode->IsValid())
		{
			XmlElement.m_XmlNode = m_XmlNode.append_child(name);

		}
		else
		{
			if (bAfter)
			{
				XmlElement.m_XmlNode = m_XmlNode.insert_child_after(name,pXmlNode->m_XmlNode);
			}
			else
			{
				XmlElement.m_XmlNode = m_XmlNode.insert_child_before(name,pXmlNode->m_XmlNode);
			}
		}
		INIT_DEBUG_XMLBUF(XmlElement)
		INIT_DEBUG_XMLBUF(*this)
			return XmlElement;
	}

	DMXmlNode DMXmlNode::InsertCopyChildNode(DMXmlNode* pCopyXmlNode,DMXmlNode* pXmlNode /*= NULL*/,bool bAfter /*= true*/)
	{
		DMXmlNode XmlElement;
		do 
		{
			if (NULL == pCopyXmlNode)
			{
				break;
			}
			if (NULL == pXmlNode
				||!pXmlNode->IsValid())
			{
				XmlElement.m_XmlNode = m_XmlNode.append_copy(pCopyXmlNode->m_XmlNode);
			}
			else
			{
				if (bAfter)
				{
					XmlElement.m_XmlNode = m_XmlNode.insert_copy_after(pCopyXmlNode->m_XmlNode,pXmlNode->m_XmlNode);
				}
				else
				{
					XmlElement.m_XmlNode = m_XmlNode.insert_copy_before(pCopyXmlNode->m_XmlNode,pXmlNode->m_XmlNode);
				}
			}
		} while (false);
		
		INIT_DEBUG_XMLBUF(XmlElement)
			INIT_DEBUG_XMLBUF(*this)
			return XmlElement;
	}

	void DMXmlNode::Append_Copy(DMXmlNode &XmlAppend)
	{
		if (XmlAppend.IsValid())
		{
			m_XmlNode.append_copy(XmlAppend.m_XmlNode);
			INIT_DEBUG_XMLBUF(*this);
		}
	}

	bool DMXmlNode::RemoveChildNode(DMXmlNode* pXmlNode)
	{
		bool bRet = false;
		do 
		{
			if (NULL == pXmlNode
				||!pXmlNode->IsValid())
			{
				break;
			}

			bRet = m_XmlNode.remove_child(pXmlNode->m_XmlNode);
		} while (false);
		INIT_DEBUG_XMLBUF(*this)
		return bRet;
	}

	bool DMXmlNode::SetAttribute(LPCSTR name, LPCSTR value,bool bAppend)
	{
		bool bRet = false;
		do 
		{
			if (!IsValid()
				|| NULL == name
				|| 0 == strlen(name))
			{
				break;
			}

			pugi::xml_attribute attr = m_XmlNode.attribute(name);
			if (attr.empty())//�ڵ㲻����
			{
				if (bAppend)
				{
					bRet = m_XmlNode.append_attribute(name).set_value(value);
				}
				else
				{
					bRet = m_XmlNode.prepend_attribute(name).set_value(value);
				}	
			}
			else
			{
				bRet = attr.set_value(value);
			}
		} while (false);
		INIT_DEBUG_XMLBUF(*this)
		return bRet;
	}

	bool DMXmlNode::SetAttributeInt(LPCSTR name, int value,bool bAppend /*= true*/)
	{
		DMBufT<char> pBuf;pBuf.Allocate(20);
		sprintf_s(pBuf.get(),20,"%d",value);
		return SetAttribute(name,pBuf.get(),bAppend);
	}

	bool DMXmlNode::SetAttributeInt64(LPCSTR name, INT64 value,bool bAppend /*= true*/)
	{
		DMBufT<char> pBuf;pBuf.Allocate(40);
		sprintf_s(pBuf.get(),40,"%I64d",value);
		return SetAttribute(name,pBuf.get(),bAppend);
	}

	bool DMXmlNode::RemoveAttribute(LPCSTR name)
	{
		bool bRet = false;
		do 
		{
			if (!IsValid()
				|| NULL == name
				|| 0 == strlen(name))
			{
				break;
			}

			pugi::xml_attribute attr = m_XmlNode.attribute(name);
			if (attr.empty())//�ڵ㲻����
			{
				break;
			}
			else
			{
				bRet = m_XmlNode.remove_attribute(name);
			}
		} while (false);
		INIT_DEBUG_XMLBUF(*this)
		return bRet;
	}

	bool DMXmlNode::GetXmlContent(CStringA &strDebug)
	{
		class xml_string_writer : public pugi::xml_writer
		{
		public:
			xml_string_writer(CStringA& rbuf) : buffer(rbuf), pugi::xml_writer()
			{
			}
		
			// Write memory chunk into stream/file/whatever
			virtual void write(const void* data, size_t size) override
			{
				buffer.Append((const char*)data, size);
			}
			CStringA& buffer;
		};
		xml_string_writer writer(strDebug);
		m_XmlNode.print(writer, "\t", pugi::format_default, pugi::encoding_utf8);
		
		return true;
	}

	// DMXmlDocHandle--------------------------------------------
	DMXmlDocHandle::DMXmlDocHandle(pugi::xml_document*pXmlDoc)
		:m_pXmlDoc(pXmlDoc)
		,m_bXmlErr(false)
	{

	}

	bool DMXmlDocHandle::LoadFromFile(LPCWSTR pszFilePath)
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pXmlDoc|| NULL == pszFilePath)
			{
				break;
			}

			wchar_t szExeDir[MAX_PATH] = {0};
			if (!GetRootDirW(szExeDir, MAX_PATH))
			{
				break;
			}

			wchar_t szPath[MAX_PATH] = {0};
			if (0 == PathCombineW(szPath, szExeDir, pszFilePath))
			{
				break;
			}

			pugi::xml_parse_result result = m_pXmlDoc->load_file(szPath, pugi::parse_default,pugi::encoding_utf8);
			bRet = (0 == result.status);
		} while (false);
		m_bXmlErr = !bRet;
		return bRet;
	}

	bool DMXmlDocHandle::LoadFromBuffer(const PVOID pContent, size_t size)
	{
		bool bRet = false;
		do 
		{
			if (NULL == m_pXmlDoc|| NULL == pContent||size<=0)
			{
				break;
			}

			pugi::xml_parse_result result = m_pXmlDoc->load_buffer(pContent, size, pugi::parse_default,pugi::encoding_utf8);
			bRet = (0 == result.status);
		} while (false);
		m_bXmlErr = !bRet;
		return bRet;
	}

	bool DMXmlDocHandle::IsValid() const
	{
		return (NULL != m_pXmlDoc);
	}

	bool DMXmlDocHandle::IsError() const
	{
		return m_bXmlErr;
	}

	bool DMXmlDocHandle::IsEqual(const DMXmlDocHandle &other) const
	{
		return (m_pXmlDoc == other.m_pXmlDoc);
	}

	DMXmlNode DMXmlDocHandle::Root(LPCSTR XmlTag/* = NULL*/) const
	{
		DMXmlNode XmlElement;
		if (NULL == XmlTag)
		{
			XmlElement.m_XmlNode = m_pXmlDoc->first_child();
		}
		else
		{
			XmlElement.m_XmlNode = m_pXmlDoc->child(XmlTag);
		}

		INIT_DEBUG_XMLBUF(XmlElement)
			return XmlElement;
	}

	DMXmlNode DMXmlDocHandle::Base() const
	{
		DMXmlNode XmlElement;
		XmlElement.m_XmlNode = m_pXmlDoc->root();
		INIT_DEBUG_XMLBUF(XmlElement)
			return XmlElement;
	}

	bool DMXmlDocHandle::SaveXml(LPCWSTR lpszPath)
	{
		return m_pXmlDoc->save_file(lpszPath);
	}

	bool DMXmlDocHandle::ResetByXmlNode(DMXmlNode &XmlAppend)
	{
		bool bRet = false;
		do 
		{
			if (!XmlAppend.IsValid())
			{
				break;
			}
			m_pXmlDoc->reset();
			m_pXmlDoc->append_copy(XmlAppend.m_XmlNode);
			bRet = true;
		} while (false);
		return bRet;
	}

	

	// DMXmlDocument-----------------------------------------------
	DMXmlDocument::DMXmlDocument()
		:DMXmlDocHandle(new pugi::xml_document)
	{

	}

	DMXmlDocument::~DMXmlDocument()
	{
		DM_DELETE(m_pXmlDoc);
	}

}//namespace DM