// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	AttrExpandWnd.h
// File mark:   
// File summary:À©Õ¹´°¿Ú
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-3
// ----------------------------------------------------------------
#pragma once

class AttrTree;
class IAttrExpandOwner
{
public:
	virtual DUIWindow* GetOwnerWindow() = 0;    
	virtual DMCode OnTreeInit(AttrTree* pTree) = 0;
	virtual DMCode OnTreeSel(AttrTree* pTree) = 0;
	virtual DMCode OnTreeEnd(AttrTree* pTree) = 0;
};
 
class AttrExpandWnd : public DMHWnd
{  
public:
	AttrExpandWnd(IAttrExpandOwner* pOwner);
	~AttrExpandWnd();
	DECLARE_MESSAGE_MAP()						
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);
	void OnDestroy();

	static 	DMCode InitTreeNode(DMXmlNode& TreeNode,bool IsData);

public:
	DMCode Show();
	DMCode Hide();

	DMCode OnTreeDbChick(DMEventArgs *pEvt);

public:
	IAttrExpandOwner*               m_pOwner;
	AttrTree*                       m_pAttrTree;
};