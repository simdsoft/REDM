// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIRoot.h 
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-8
// ----------------------------------------------------------------
#pragma once

class DUIObjEditor;
class ObjXml;
class ObjTree;
class DUIRoot:public DUIWindow
{
	DMDECLARE_CLASS_NAME(DUIRoot, L"root",DMREG_Window)
public:
	DUIRoot();

	DMCode InitDesignEditor(HDMTREEITEM hRootTree);
	DMCode SetDesignMode(DesignMode ds_mode);

public:
	DM_BEGIN_MSG_MAP()
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
	DM_END_MSG_MAP()
	void OnLButtonDown(UINT nFlags,CPoint pt);
	void OnLButtonUp(UINT nFlags,CPoint pt);
	void OnRButtonDown(UINT nFlags, CPoint pt);
	void OnMouseMove(UINT nFlags,CPoint pt);

	DUIWND HitTestPoint(CPoint pt,bool bFindNoMsg);///< 过滤消息分发

	// 模式处理
	HDMTREEITEM SelOrHoverTreeItemByDUIWnd(HDMTREEITEM hRoot,DUIWindow* pDUIWnd,bool bSel = false);
	bool MLDownInSelMode(CPoint pt,DUIWindow* pCurSelWnd);
	bool MLDownInAddMode(CPoint pt);

public:
	// AddMode下的操作
	static bool IsSupportAddChild(DUIWindow* pParentWnd,CStringW strReg);								///< 是否支持Add 子控件
	static DUIWindow* CreateAddChild(DUIWindow* pParentWnd,CStringW strReg);							///< 创建子控件
	static bool InitAddChild(ObjTreeData* pParentData,DUIWindow* pWnd,CRect rcDrag,bool bRight=false);	///< 初始化子控件
	DUIWindow* GetAddChild();
	

public:
	DM_BEGIN_ATTRIBUTES()
		DM_CHAIN_ATTRIBUTTE(m_pDUIXmlInfo)
	DM_END_ATTRIBUTES()		
	DMCode OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr);

	//
	DUIObjEditor*										m_pParent;
	ObjTree*											m_pObjTree;
	ObjXml*												m_pObjXml;
	DesignMode											m_DesignMod;
	HDMTREEITEM											m_hRoot;

	// 拖动操作
	bool												m_bDown;							///<鼠标按下
	CPoint												m_StartDragPt;
	CPoint												m_TrackDragPt;
	CRect												m_StartDragRc;

	// Add操作
	DUIWindow*                                          m_pAddWnd;
	static CPoint                                       m_pAddParentPt;
};