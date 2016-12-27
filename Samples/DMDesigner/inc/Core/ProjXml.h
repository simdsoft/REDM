// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ProjXml.h
// File mark:   
// File summary:管理工程视图
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once

class ProjXml:public DMArrayT<DocDataPtr>,public DMMapT<HDMTREEITEM,DMXmlNodePtr>
{
public:
	ProjXml();
	virtual~ProjXml();

public:
	DMCode ParseRes(CStringW strResDir);
	bool   IsNeedSave();
	DMCode SaveRes(bool bCheck = true);
	DMCode InitProjTree();							
	DMCode ReleaseProjTree();
	DMCode PopProjTreeMenu(HDMTREEITEM hSel);
	DMCode HandleProjTreeMenu(int nID);

	// 其他
	DMCode InitSubXmlList();
	DMCode SetDocUnSave(CStringW strPath);
	DMCode SetDocUnSave(DMXmlNodePtr pNode);
	DMCode InitProjTreeNode(DMXmlNode& TreeNode,bool IsData = false);
	DMCode BindProjTreeData(DMXmlNode& DataNode,HDMTREEITEM hTreeItem);
	HDMTREEITEM InsertProjTreeItem(DMXmlNode& TreeNode,CStringW strText,HDMTREEITEM hParent =DMTVI_ROOT);
	DocDataPtr FindDocData(CStringW strPath);
	DocDataPtr FindDocData(DMXmlNodePtr pNode);
	CStringW GetProjTreeItemText(HDMTREEITEM hSel);
	

protected:
	// Menu项初始化
	DMCode InitCustomProjMenu(DMXmlNode& XmlNode);
	DMCode InitSkinPoolsProjMenu(DMXmlNode& XmlNode);
	DMCode InitStylePoolsProjMenu(DMXmlNode& XmlNode);
	DMCode InitSkinProjMenu(DMXmlNode& XmlNode);
	DMCode InitStyleProjMenu(DMXmlNode& XmlNode);
	DMCode InitImgTypeMenu(DMXmlNode& XmlNode);
	DMCode InitThemeMenu(DMXmlNode& XmlNode);
	DMCode InitImgMenu(DMXmlNode& XmlNode);
	DMCode InitLayoutMenu(DMXmlNode& XmlNode);

	// Menu事件处理
	DMCode ProjMenu_OpenDir();
	DMCode ProjMenu_Scan();
	DMCode ProjMenu_AddSkinPool();
	DMCode ProjMenu_DelSkinPool();
	DMCode ProjMenu_EditSkinPool();
	DMCode ProjMenu_AddStylePool();
	DMCode ProjMenu_DelStylePool();
	DMCode ProjMenu_EditStylePool();
	DMCode ProjMenu_AddSkin();
	DMCode ProjMenu_DelSkin();
	DMCode ProjMenu_EditSkin();
	DMCode ProjMenu_AddStyle();
	DMCode ProjMenu_DelStyle();
	DMCode ProjMenu_EditStyle();
	DMCode ProjMenu_AddImgType();
	DMCode ProjMenu_DelImgType();
	 DMCode ProjMenu_EditImgType();
	DMCode ProjMenu_AddTheme();
	DMCode ProjMenu_DelTheme();
	DMCode ProjMenu_EditTheme();
	DMCode ProjMenu_SetDefTheme();
	DMCode ProjMenu_AddImg();
	DMCode ProjMenu_DelImg();
	DMCode ProjMenu_EditImg();
	DMCode ProjMenu_AddLayout();
	DMCode ProjMenu_DelLayout();
	DMCode ProjMenu_EditLayout();

private:
	virtual void PreArrayObjRemove(const DocDataPtr &obj);
	virtual void PreMapKeyRemove(const DMXmlNodePtr &obj);
	virtual bool EqualArrayObj(const DocDataPtr &objsrc, const DocDataPtr &objdest);

public:
	DMSmartPtrT<ResFolder>								m_pRes;				///< 资源包,预览窗口由此资源包提供资源
	CStringW											m_strResDir;		///< 资源包路径,最后带"\"
	CStringW											m_strGlobalName;	///< 全局资源包名字

	// 树形控件
	ProjTree*											m_pProjTree;	
	HDMTREEITEM                                         m_hProjLayouts;		///< 样式-皮肤-布局文件根结点
	HDMTREEITEM                                         m_hProjThemes;		///< 主题包列表根结点
	HDMTREEITEM                                         m_hProjLayoutFiles; ///< 布局文件列表根结点
	HDMTREEITEM                                         m_hProjGlobal;      ///< Global所在的结点
	HDMTREEITEM                                         m_hProjGlobalStyleSkins;
	HDMTREEITEM                                         m_hProjPrivStyleSkins;
	HDMTREEITEM                                         m_hProjGlobalStyles;///< 全局样式列表
	HDMTREEITEM                                         m_hProjGlobalSkins; ///< 全局皮肤列表

	HDMTREEITEM											m_hProjSel;			
	HDMTREEITEM                                         m_hProjDefTheme;

	bool                                                m_bSubInit;			///< sub列是否已初始化完成
	
};