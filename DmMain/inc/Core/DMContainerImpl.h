//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIContainer.h 
// File Des: 此类用于辅助分发各类消息到DUI窗口.设置DUI窗口状态
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-11-6	1.0			
//-------------------------------------------------------
#pragma once
#include "DUIFocusMgr.h"
#include "DUIDropTargetDispatch.h"

namespace DM
{
	class DM_EXPORT DMContainerImpl:public IDMContainer
	{
	public:
		DMContainerImpl(DUIWindow *pHWnd);

	public:
		LRESULT OnFrameEvent(UINT uMsg,WPARAM wp,LPARAM lp);

		DMCode OnSetFocusWnd(DUIWND DUIWnd);
		DMCode OnGetFocusWnd(DUIWND &DUIWnd);

		DMCode OnSetCaptureWnd(DUIWND DUIWnd,DUIWNDPtr pOldDUIWnd=NULL);
		DMCode OnGetCaptureWnd(DUIWND &DUIWnd);
		DMCode OnReleaseCaptureWnd();

		DMCode OnRegisterDragDrop(DUIWND hDUIWnd, IDropTarget *pDropTarget);
		DMCode OnUnRegisterDragDrap(DUIWND hDUIWnd);

		//---------------------------
		// 未实现
		DMCode OnFireEvent(DMEventArgs &Evt){return DM_ECODE_NOTIMPL;}
		DMCode OnGetDraw(IDMDraw** ppObj){return DM_ECODE_NOTIMPL;}
		DMCode OnGetCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas**ppCanvas){return DM_ECODE_NOTIMPL;}
		DMCode OnReleaseCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas*pCanvas){return DM_ECODE_NOTIMPL;}
		HWND   OnGetHWnd(){return NULL;}
		DMCode OnGetContainerRect(LPRECT lpRect){return DM_ECODE_NOTIMPL;}
		DMCode OnIsTranslucent(){return DM_ECODE_NOTIMPL;}
		DMCode OnUpdateWindow(){return DM_ECODE_NOTIMPL;}
		DMCode OnForceUpdateWindow(){return DM_ECODE_NOTIMPL;}
		DMCode OnUpdateRect(LPCRECT lpRect,DUIWND hDUIWnd){return DM_ECODE_NOTIMPL;}
		DMCode OnClientToScreen(LPRECT lpRect){return DM_ECODE_NOTIMPL;}
		DMCode OnIsSizChanging(){return DM_ECODE_NOTIMPL;}

		//---------------------------
		IDMAccelMgr* GetAccelMgr(){return &m_FocusMgr;}

		//---------------------------
		DMCode OnRegisterTimeline(IDMTimeline *pHandler);
		DMCode OnUnregisterTimeline(IDMTimeline *pHandler);
		DMCode OnTimeline();

	public:// 辅助
		void OnFrameMouseMove(UINT uFlag,CPoint pt);
		void OnFrameMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
		void OnFrameMouseLeave();

		DMCode OnFrameSetCursor(const CPoint &pt);
		void OnFrameKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
		void OnFrameKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

		void OnActivate(UINT nState);
		void OnActivateApp(BOOL bActive, DWORD dwThreadID);

	public:

		DUIWindow*					     m_pContainWnd;              ///<此处容器对象一般为宿主窗口(DMHWnd或DMHDialog),在面板上指面板窗口
		DUIWND                           m_hDUICaptureWnd;			 ///<当前捕获消息的DUI窗口
		DUIWND                           m_hDUIHoverWnd;			 ///<当前停留的DUI窗口
		bool                             m_bNcHover;			     ///<当前是否停留在非客户区

		DUIFocusMgr						 m_FocusMgr;
		DUIDropTargetDispatch			 m_DropTarget;
		typedef CList<IDMTimelinePtr> DMTimelineHanderList;
		DMTimelineHanderList		     m_TimelineList;
	};
}//namespace DM