// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIDragFrame.h
// File mark:   
// File summary:选中时拖动的四边框
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-10
// ----------------------------------------------------------------
#pragma once
#include "DMDelegate.h"// 写了这么久，总得用一次吧，

/// <summary>
///		辅助点坐标
/// </summary>
enum DSDOT
{
	DSDOT_MOVE,
	DSDOT_LEFT,
	DSDOT_LEFTOP,
	DSDOT_TOP,
	DSDOT_RIGHTOP,
	DSDOT_RIGHT,
	DSDOT_RIGHTBTM,
	DSDOT_BTM,
	DSDOT_LEFTBTM,
	DSDOT_BOX,
};

class DragMeta;
typedef DMDelegate<void(DragMeta&, int)> DragMetaEventHandler;
class DragMeta
{
public:
	DragMeta(int idx=-1,HCURSOR hCursor = ::LoadCursor(NULL,IDC_ARROW));
	DMCode OnRender(IDMCanvas* pCanvas);
	DMCode SetCurSor();

public:
	int					 m_Index;			///< 0为不含所有dot的矩形区（即控件区）,1为左中dot，顺时针8个点，9为左上角的mov小图标,对应DSDOT
	CRect				 m_Rect;			///< 此点的矩形区，默认1-9点都在控件区外围，不占用控件区
	HCURSOR				 m_hCursor;			///< 停留在此矩形区时显示的坐标
	DMColor				 m_Clr;				///< 矩形区正常时的填充色
	DMColor              m_GrayClr;			///< 矩形区不可时的填充色
	bool				 m_bDragDot;		///< 除了0外，其他都是拖动点(true)
	bool				 m_bEnable;			///< 此点是否可用
	DragMetaEventHandler m_DragMetaAction;

	DMSmartPtrT<IDMSkin>	m_pSkin;		///< 绘制mov小图标
};

class ObjTreeData;
class RightXml;
class DUIDragFrame : public DUIWindow
{
	DMDECLARE_CLASS_NAME(DUIDragFrame,L"duidragframe",DMREG_Window);
public:
	DUIDragFrame();

	DMCode InitDragFrame(ObjTreeData* pData,CRect& rcLayout);
	static bool IsSupportPos(ObjTreeData* pData);

private:
	void LinkDragMetaEvent();
	void InitDragMeta(CRect Rect,bool bMain,bool bAllGray=false);
	DragMeta* HittestDragMeta(CPoint pt,bool bMain);

	void OnDragLeft(DragMeta& meta, int iAction);		 // 拖动左边
	void OnDragLeftTop(DragMeta& meta, int iAction);	 // 拖动左上角
	void OnDragTop(DragMeta& meta, int iAction);		 // 拖动上边
	void OnDragRightTop(DragMeta& meta, int iAction);	 // 拖动右上角
	void OnDragRight(DragMeta& meta, int iAction);		 // 拖动右边
	void OnDragRightBottom(DragMeta& meta, int iAction); // 拖动右下角
	void OnDragBottom(DragMeta& meta, int iAction);		 // 拖动下边
	void OnDragLeftBottom(DragMeta& meta, int iAction);  // 拖动左下角
	void OnDragBox(DragMeta& meta, int iAction);
	void OnDragMove(DragMeta& meta, int iAction);		 // 移动

	CRect CalcDragFrameRect(CRect rcDrag);
	void InternalHandleDrag(CRect& rect, int* pHori, int* pVert);
	void SetElementWidth(int wid);
	void SetElementHeight(int hei);
	bool SetElementRect(CRect rect);

public:
	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
	DM_END_MSG_MAP()
	void DM_OnPaint(IDMCanvas* pCanvas);
	void OnLButtonDown(UINT nFlags,CPoint pt);
	void OnLButtonUp(UINT nFlags,CPoint pt);
	void OnRButtonDown(UINT nFlags, CPoint pt);
	void OnMouseMove(UINT nFlags,CPoint pt);
	DUIWND HitTestPoint(CPoint pt,bool bFindNoMsg);///< 过滤消息分发

public:
	RightXml*                                m_pRightXml;		///< 右边属性,所有的坐标都通过右边属性来控制
	DMSmartPtrT<Layout>						 m_pLayout;			///< 指向要编辑的layout			
	DragMeta*								 m_pCurdragMeta;
	DragMeta								 m_dragMetas[10];
	int										 m_dragMetaCount;
	bool								     m_bMain;			///< 将Root层也视为最上层
	bool                                     m_bDown;			///< 只有在按下时停留在dot上或box上才设置m_bDown
	
	CPoint									 m_StartDragPt;
	CPoint									 m_TrackDragPt;
	CRect									 m_StartDragRc;		///< 开始拖动时元素区域大小

	// 辅助
	ObjTreeData*							 m_pData;
};