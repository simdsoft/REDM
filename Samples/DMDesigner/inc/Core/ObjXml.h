//----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ObjXml.h
// File mark:   
// File summary:管理对象视图
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once

class ObjTreeData;
class ObjXml : public ProjXml,public DMMapT<HDMTREEITEM,ObjTreeDataPtr>
{
public:
	ObjXml();
	~ObjXml();

public:
	DMCode InitObjTree();							
	DMCode ReleaseObjTree();
	DMCode ShowPreview();
	DMCode PopObjTreeMenu(HDMTREEITEM hSel);
	DMCode HandleObjTreeMenu(int nID);

	// 其他
	DMCode OnObjTreeChanged(DMEventArgs* pEvt);
	DMCode InitObjTreeNode(DMXmlNode& TreeNode,bool IsDMXml = false);
	DMCode BindObjTreeData(DocDataPtr pDoc,DUIRoot* pEditor,DUIWindowPtr pDUI, DMXmlNode& Node, HDMTREEITEM hTreeItem,bool bPanel = false);
	HDMTREEITEM InsertObjTreeItem(DMXmlNode& TreeNode,CStringW strText,HDMTREEITEM hParent =DMTVI_ROOT);
	HDMTREEITEM FindStyle(CStringW strStyle);// 注意,这里找到的HDMTREEITEM为ProjTree中的项
protected:
	// Menu项初始化
	DMCode InitCopyObjMenu(DMXmlNode& XmlNode);
	DMCode InitCustomObjMenu(DMXmlNode& XmlNode);
	
	// Menu事件处理
	DMCode ObjMenu_OpenDir();
	DMCode ObjMenu_Paste();
	DMCode ObjMenu_Copy();
	DMCode ObjMenu_LockAllChild(bool bLock);
	DMCode ObjMenu_Del();

public:
	DMCode InitObjRootTree();
	DMCode InitObjChildTree(HDMTREEITEM hRootItem);
	DMCode EnumChildTreeItem(DUIRoot*pMainWnd, DUIWindow* pWnd, HDMTREEITEM hTreeItem,bool bPanel = false);
	DMCode EnumPanelTreeItem(DUIRoot*pMainWnd, DUIWindow* pWnd, HDMTREEITEM hTreeItem);
	DMCode RemoveObjTreeItemMap(HDMTREEITEM hTreeItem);///< 移除hTreeItem及其对应所有子节点的map

private:
	HDMTREEITEM _FindStyle(HDMTREEITEM hStylePoolParent,CStringW strName,CStringW strKey);
	virtual void PreMapKeyRemove(const ObjTreeDataPtr &obj);

public:
	DUIObjEditor*										m_pObjEditor;
	bool												m_bInitObjTree;
	DMSmartPtrT<PreHWnd>								m_pPreWnd;				///<  预览窗口

	// 树形控件
	ObjTree*										    m_pObjTree;
	HDMTREEITEM											m_hObjSel;	

	// 右侧属性
	RightXml*                                           m_pRighXml;

	// Add属性
	AddXml*                                             m_pAddXml;

	// 支持面板
	DUICheckBox*                                        m_pbPanelCheck;
};
