// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	StyleDlg.h
// File mark:   
// File summary:三份Array实际只有一份，m_ArrayUse+m_ArrayNoUse不释放资源
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-1
// ----------------------------------------------------------------
#pragma once

class StyleDlg : public DMHDialog,public IAttrExpandOwner,DMArrayT<DMXmlInitAttrPtr>
{
public:
	StyleDlg(bool bEditMode = false);
	~StyleDlg();
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP() 

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);
	void OnDestroy();
	void OnLButtonDown(UINT nFlags, CPoint point);

	virtual DUIWindow* GetOwnerWindow(); 
	virtual DMCode OnTreeInit(AttrTree* pTree);
	virtual DMCode OnTreeSel(AttrTree* pTree);
	virtual DMCode OnTreeEnd(AttrTree* pTree);

	DMCode OnPropDeling(DMEventArgs *pEvt);
	DMCode OnPropDel(DMEventArgs *pEvt);
public:
	DMCode OnExpand();
	DMCode InitAttrArray();
	virtual void PreArrayObjRemove(const DMXmlInitAttrPtr &obj);
	DMXmlInitAttrPtr FindByName(CStringW strName);

protected: 
	DMCode OnOK();

private:
	bool								m_bEditMode;
	DMSmartPtrT<AttrExpandWnd>          m_pExpandWnd;
	DUIButton*                          m_pExpandBtn;
	DUIPropFrame*                       m_pPropFrame;

	// 辅助
	ObjXml*								m_pObjXml;
	ProjTree*							m_pProjTree;

	DMXmlDocument						m_StyleDoc;
	DMXmlNode							m_StyleNode;
};

