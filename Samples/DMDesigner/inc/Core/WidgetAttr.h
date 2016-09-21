// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	WidgetAttr.h
// File mark:   
// File summary:列出除POS外DUI窗口的所有属性
// Author:		guoyouhuang 
// Edition:     1.0
// Create date: 2016-8-15
// ----------------------------------------------------------------
#pragma once

class WidgetAttr : public AttrBase,public IAttrExpandOwner, DMArrayT<DMXmlInitAttrPtr>
{
public:
	WidgetAttr();
	~WidgetAttr();
	DMCode UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel);
	DMCode OnExpand();
	DMCode OnPropValueChanged(DMEventArgs *pEvt);
	DMCode OnPropDeling(DMEventArgs *pEvt);
	DMCode OnPropDel(DMEventArgs *pEvt);
	void OnLButtonDown(UINT nFlags, CPoint point);

	virtual DUIWindow* GetOwnerWindow(); 
	virtual DMCode OnTreeInit(AttrTree* pTree);
	virtual DMCode OnTreeSel(AttrTree* pTree);
	virtual DMCode OnTreeEnd(AttrTree* pTree);

	DMCode InitAttrArray();
	IPropPtr FindPropByName(CStringW strName);
	DMXmlInitAttrPtr FindAttrByName(CStringW strName);
	virtual void PreArrayObjRemove(const DMXmlInitAttrPtr &obj);

private:
	void ReleaseProp();
	void InitSpecialValue(CStringW& strValue, DMXmlInitAttrPtr pInitAttr);

public:
	static DMCode s_DMHandleEvent(DM::DMEventArgs *pEvt);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP()

public:
	DMSmartPtrT<AttrExpandWnd>						   m_pExpandWnd;
	DUIButton*										   m_pExpandBtn;
	DUIPropFrame*									   m_pPropFrame;
	DUIWindow*										   m_pCurDUI;
	CStringWList                                       m_strList;
	PropString*                                        m_pPropStyle;      ///< 用于对style特殊处理
	// 用于转发消息
	static WidgetAttr*                                 ms_pthis;
};