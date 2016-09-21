// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	SkinDlg.h
// File mark:   
// File summary:ÐÂ½¨»ò±à¼­Æ¤·ô
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-15
// ----------------------------------------------------------------
#pragma once 
  
class ObjXml;
class ProjTree;
class SkinDlg : public DMHDialog,public DMMapT<HDMTREEITEM,DMXmlNodePtr>
{
public: 
	SkinDlg(bool bEditMode = false);
	~SkinDlg();
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP() 

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);
	void OnDestroy();

	DMCode OnTypeCbxChange(DMEventArgs *pEvt);
	DMCode OnThemeCbxChange(DMEventArgs *pEvt);
	DMCode OnTreeChange(DMEventArgs *pEvt);
	DMCode OnEditChange(DMEventArgs *pEvt);
	DMCode OnCheckChange(DMEventArgs *pEvt);
	DMCode OnDSRectChange(DMEventArgs *pEvt);
	DMCode ReleaseSkinTree();

protected: 
	DMCode OnOK();

private:
	DMCode InitThemeCbx();
	DMCode BindSkinTreeData(DMXmlNode& DataNode,HDMTREEITEM hTreeItem);
	HDMTREEITEM InsertSkinTreeItem(DMXmlNode& TreeNode,CStringW strText,HDMTREEITEM hParent =DMTVI_ROOT);
	DMCode UpdateXml();
	virtual void PreMapKeyRemove(const DMXmlNodePtr &obj);

private:
	bool							m_bEditMode;
	DUIComboBox*                    m_pTypeCbx;
	DUIComboBox*                    m_pThemeCbx;
	DUIWindow*                      m_pImg9;
	DUIRect*					    m_pRect;
	DUImgEditor*                    m_pImgEditor;
	DUICheckBox*                    m_pVert;
	DUICheckBox*                    m_pTitle;
	DUIEdit*                        m_pId;
	DUIEdit*                        m_pState;
	ProjTree*                       m_pSkinTree;

	// ¸¨Öú
	ObjXml*							m_pObjXml;
	ProjTree*						m_pProjTree;

	DMXmlDocument                   m_SkinDoc;
	DMXmlNode                       m_SkinNode;
};