// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	HostAttr.h 
// File mark:    
// File summary:Host属性区
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-10
// ----------------------------------------------------------------
#pragma once
#include "DUIPropCtrlHelper.h"
class HostAttr : public AttrBase, public IAttrExpandOwner, DMArrayT<DMXmlInitAttrPtr>
{
public:
	HostAttr();
	~HostAttr();
	DMCode UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel);
	DMCode OnExpand();
	DMCode OnPropValueChanged(DMEventArgs *pEvt);
	DMCode OnPropDeling(DMEventArgs *pEvt);
	DMCode OnPropDel(DMEventArgs *pEvt);
	DMCode OnSetWidth(int wid);
	DMCode OnSetHei(int hei);
	void OnLButtonDown(UINT nFlags, CPoint point);

	virtual DUIWindow* GetOwnerWindow(); 
	virtual DMCode OnTreeInit(AttrTree* pTree);
	virtual DMCode OnTreeSel(AttrTree* pTree);
	virtual DMCode OnTreeEnd(AttrTree* pTree);

	DMCode InitAttrArray(ObjTreeDataPtr pData);
	DMXmlInitAttrPtr FindAttrByName(CStringW strName);
	virtual void PreArrayObjRemove(const DMXmlInitAttrPtr &obj);

private:
	void ReleaseProp();

public:
	static DMCode s_DMHandleEvent(DM::DMEventArgs *pEvt);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP()

public:   
	DMSmartPtrT<AttrExpandWnd>						   m_pExpandWnd;
	DUIButton*										   m_pExpandBtn;
	DUIPropFrame*									   m_pPropFrame;
	DUIRoot*										   m_pHost;
	PropSize*                                          m_pInitSize;///< 用于配合dragframe设置宽高
	// 用于转发消息
	static HostAttr*                                   ms_pthis;
};