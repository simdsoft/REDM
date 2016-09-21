// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	Data.h
// File mark:   
// File summary:绑定数据定义
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once
/// <summary>
///		每个XML Doc维护一个结构体
/// </summary>
class DocData;
typedef DocData* DocDataPtr;
class DocData : public DMArrayT<DocDataPtr>///< 自身XML找到的sub列,仅用于dm xml
{
public:
	DocData();
	DocData(CStringW strXmlPath);
	~DocData();
	bool IsValid() const;

public:
	DMXmlDocument*                        m_pXmlDoc;			///< 自身的XML DOC
	CStringW                              m_strXmlPath;			///< 自身XML所在的Doc的全路径
	DMXmlNode                             m_XmlRoot;			///< 根结点
	bool                                  m_bChange;			///< 是否被改变(被改变后才能被保存)
};

/// <summary>
///		objtree每一项绑定的数据
/// </summary>
 enum {DMVISIBLE_NULL,DMVISIBLE_HIDE,DMVISIBLE_SHOW};
 class ObjTreeData
 {
 public:
	 ObjTreeData(DocDataPtr pDoc,DUIRoot* pRootWnd,DUIWindowPtr pDUI,DMXmlNode& Node,bool bPanel=false);
	 ~ObjTreeData();
	 void SetData(DocDataPtr pDoc,DUIRoot* pRootWnd,DUIWindowPtr pDUI,DMXmlNode& Node,bool bPanel=false);
	 bool IsValid() const;

 public:
	 DocDataPtr                           m_pDoc;				///< 自身XML
	 DUIRoot*							  m_pRootWnd;			///< 自身所在编辑主窗口
	 DUIWindowPtr                         m_pDUIWnd;			///< 自身所在窗口
	 DMXmlNodePtr						  m_pXmlNode;			///< 自身XML结点
	 int                                  m_iOrgVisible;        ///< 用于在eye关闭时记录原始的visible状态    
	 bool                                 m_bPanel;				///< 是否为PanelItem;
 };
 typedef ObjTreeData* ObjTreeDataPtr;