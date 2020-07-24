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
	DMDECLARE_CLASS_NAME(ObjTree, "dsobjtree",DMREG_Window)
public:
	HDMTREEITEM InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,BOOL bEnsureVisible=FALSE);
	DMCode UpdateItemRect(HDMTREEITEM hRet);
	virtual void OnNodeFree(DM::LPTVITEMEX &pItemData);
	bool IsItemStillExist(const DM::LPTVITEMEX &pItemData, LPARAM lp);	

	DM_BEGIN_MSG_MAP()
		MSG_WM_RBUTTONDOWN(OnRButtonDown);
	DM_END_MSG_MAP()
	void OnRButtonDown(UINT nFlags, CPoint pt);

private:
	CArray<DM::LPTVITEMEX>			m_DeletedItemsDataArr;//被删除的项需要记起来  防止删除之后  再select
	CArray<LPARAM>					m_DeletedItemsLparamArr;
};