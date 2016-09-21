// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ProjTree.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once

class ProjTree : public DUITreeEx
{
	DMDECLARE_CLASS_NAME(ProjTree, L"dsprojtree",DMREG_Window)
public:
	ProjTree();
	HDMTREEITEM InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,BOOL bEnsureVisible=FALSE);
	DMCode UpdateItemRect(HDMTREEITEM hRet);

	DM_BEGIN_MSG_MAP()
		MSG_WM_RBUTTONDOWN(OnRButtonDown);
	DM_END_MSG_MAP()
	void OnRButtonDown(UINT nFlags, CPoint pt);
};

class AttrTree : public ProjTree
{
	DMDECLARE_CLASS_NAME(AttrTree, L"dsattrtree",DMREG_Window)
public:
	DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
};

