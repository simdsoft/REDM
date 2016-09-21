// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ObjTree.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-8
// ----------------------------------------------------------------
#pragma once

class ObjTree : public DUITreeEx
{
	DMDECLARE_CLASS_NAME(ObjTree, L"dsobjtree",DMREG_Window)
public:
	HDMTREEITEM InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,BOOL bEnsureVisible=FALSE);
	DMCode UpdateItemRect(HDMTREEITEM hRet);

	DM_BEGIN_MSG_MAP()
		MSG_WM_RBUTTONDOWN(OnRButtonDown);
	DM_END_MSG_MAP()
	void OnRButtonDown(UINT nFlags, CPoint pt);
};