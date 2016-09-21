//----------------------------------------------------------------
// Copyright (c)     
// All rights reserved.  
//          
// File name:	DUIObjEditor.h 
// File mark:   
// File summary:对象视图编辑主窗口
// Author:		guoyouhuang
// Edition:     1.0 
// Create date: 2016-8-6
// ----------------------------------------------------------------
#pragma once
#include "DUIRoot.h" 

class ObjXml;
class DUIObjEditor : public DUIScrollBase
{
	DMDECLARE_CLASS_NAME(DUIObjEditor, L"dsobjeditor",DMREG_Window)
public:
	DUIObjEditor();

public:
	DMCode InitObjEditor();
	DMCode UnInitObjEditor();
	DMCode SetDesignMode(DesignMode ds_mode);

	DUIRoot* InitDesignChild(HDMTREEITEM hRootTree);			///< 初始化Design子窗口
	DUIRoot* GetShowDesignChild();
	DMCode ShowDesignChild(DUIRoot* pShow);
	DMCode RemoveAllDesignChild();

	// 控制滚动范围
	void UpdateScrollRangeSize();												
	void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);		
	
	DMCode DV_GetChildMeasureLayout(LPRECT lpRect);	
	DMCode OnDesignModeChanged(DMEventArgs* pEvt);
	DMCode OnCheckChanging(DMEventArgs* pEvt);
	DMCode OnCheckChanged(DMEventArgs* pEvt);

	// 控制各frame
	DMCode HoverInSelOrAddMode(DUIWindow* pDUIHover);
	DMCode SelFrameInAllMode();
	DMCode DragFrameInSelMode();

public:
	DUIRoot*											m_pShow;
	DUIStatic*											m_pHoverlFrame;					///< 在SelectMode或AddMode时框选停留DUI
	DUIStatic*                                          m_pSelFrame;                    ///< 在所有模式下框选 选中的树形控件 对应的DUI
	DUIDragFrame*                                       m_pDragFrame;                   ///< 在SelectMode下框选 选中的树形控件  对应的DUI

	//
	ObjXml*												m_pObjXml;
	DUITabCtrl*											m_pTools;
	DUICheckBox*									    m_pCheckBtn;					///< m_pChcekBtn和m_pTools互相控制，只有在AddMode下才能选中m_pChcekBtn
	DesignMode											m_DesignMod;
	CRect												m_rcMeasure;
	bool                                                m_bInit;
};