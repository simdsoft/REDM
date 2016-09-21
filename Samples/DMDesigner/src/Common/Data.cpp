#include "DMDesignerAfx.h"
#include "Data.h"

DocData::DocData()
{
	m_pXmlDoc = NULL;
	m_bChange = false;
}

DocData::DocData(CStringW strXmlPath)
{
	m_pXmlDoc = new DMXmlDocument;
	if (m_pXmlDoc->LoadFromFile(strXmlPath))
	{
		m_XmlRoot = m_pXmlDoc->Root();
		m_strXmlPath = strXmlPath;
	}
	else
	{
		DM_DELETE(m_pXmlDoc);
		CStringW strInfo;strInfo+=strXmlPath;strInfo+=L"½âÎöÊ§°Ü";
		DM_MessageBox(strInfo);
	}
	m_bChange = false;
}

DocData::~DocData()
{
	DM_DELETE(m_pXmlDoc);
	m_bChange = false;
	RemoveAll();
}

bool DocData::IsValid() const
{
	return NULL != m_pXmlDoc;
}

// ObjTreeData
ObjTreeData::ObjTreeData(DocDataPtr pDoc,DUIRoot* pRootWnd,DUIWindowPtr pDUI,DMXmlNode& Node,bool bPanel)
{
	m_pXmlNode = new DMXmlNode;
	SetData(pDoc,pRootWnd,pDUI,Node,bPanel);
	m_iOrgVisible = DMVISIBLE_NULL;
}

ObjTreeData::~ObjTreeData()
{
	DM_DELETE(m_pXmlNode);
	m_pDoc	   = NULL;
	m_pRootWnd = NULL;
	m_pDUIWnd  = NULL;
	m_pXmlNode = NULL;
	m_iOrgVisible = DMVISIBLE_NULL;
}

void ObjTreeData::SetData(DocDataPtr pDoc,DUIRoot* pRootWnd,DUIWindowPtr pDUI,DMXmlNode& Node,bool bPanel)
{
	m_pDoc	   = pDoc;
	m_pRootWnd = pRootWnd;
	m_pDUIWnd  = pDUI;
	*m_pXmlNode= Node;
	m_bPanel   = bPanel;
}

bool ObjTreeData::IsValid() const
{
	return m_pDoc&&m_pDoc->IsValid()&&m_pXmlNode&&m_pXmlNode->IsValid();
}
