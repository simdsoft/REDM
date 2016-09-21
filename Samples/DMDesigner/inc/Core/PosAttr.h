// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	PosAttr.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-31
// ----------------------------------------------------------------
#pragma once

class PosAttr : public AttrBase
{
public:
	PosAttr();
	DMCode UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel);
	DMCode InitPos();

public:
	static DMCode s_DMHandleEvent(DM::DMEventArgs *pEvt);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP()

public:
	DUIPos*                                            m_pPos;
	DUIWindow*										   m_pCurDUI;
	//
	static PosAttr*									   ms_pthis;
};