//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMContainer.h 
// File Des: 容器，外部只能继承，无法直接注册到内部使用
// File Summary: 用于辅助分发各类消息到DUI窗口.设置DUI窗口状态
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-4	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		全局定时器扩展对象
	/// </summary>
	class IDMTimeline
	{
	public:
		virtual DMCode OnTimeline()=0;
	};
	typedef IDMTimeline* IDMTimelinePtr;

	/// <summary>
	///		容器扩展对象
	/// </summary>
	class DM_EXPORT IDMContainer
	{
	public:
		virtual ~IDMContainer(){}

		virtual LRESULT OnFrameEvent(UINT uMsg,WPARAM wp,LPARAM lp) = 0;					///<分发消息到容器内的各DUI窗口，非常重要！

		virtual DMCode OnSetFocusWnd(DUIWND DUIWnd) = 0;									///<设置当前有焦点的DUI窗口
		virtual DMCode OnGetFocusWnd(DUIWND &DUIWnd) = 0;									///<获取当前有焦点的DUI窗口

		virtual DMCode OnSetCaptureWnd(DUIWND DUIWnd,DUIWNDPtr pOldDUIWnd=NULL) = 0;	    ///<设置容器内指定DUI窗口焦点
		virtual DMCode OnGetCaptureWnd(DUIWND &DUIWnd) = 0;									///<获取容器内拥有焦点的DUI窗口
		virtual DMCode OnReleaseCaptureWnd() = 0;											///<释放焦点

		virtual DMCode OnRegisterDragDrop(DUIWND hDUIWnd, IDropTarget *pDropTarget) = 0;    ///<给指定DUI窗口注册拖放对象
		virtual DMCode OnUnRegisterDragDrap(DUIWND hDUIWnd) = 0;						    ///<反注册拖放

		virtual DMCode OnFireEvent(DMEventArgs &Evt) = 0;									///<消息分发

		//----------------------------------------------
		// 画布可以抓取主窗口的区域部分背景,再和指定的前景做动画
		virtual DMCode OnGetDraw(IDMDraw** ppObj) = 0;
		virtual DMCode OnGetCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas**ppCanvas) = 0;	///<获取画布,画布可包含当前容器的当前绘制状态
		virtual DMCode OnReleaseCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas*pCanvas) = 0;///<释放画布

		//----------------------------------------------
		// 当前容器的主窗口相关
		virtual HWND   OnGetHWnd() = 0;													    ///<获取当前容器的主窗口				
		virtual DMCode OnGetContainerRect(LPRECT lpRect) = 0;								///<获取容器区域，请使用*lpRect赋值！
		virtual DMCode OnIsTranslucent() = 0;												///<是否分层窗口
		virtual DMCode OnUpdateWindow() = 0;												///<调用系统UpdateWindow				
		virtual DMCode OnForceUpdateWindow() = 0;											///<强制立即调用刷新									
		virtual DMCode OnUpdateRect(LPCRECT lpRect,DUIWND hDUIWnd) = 0;						///<增加无效区
		virtual DMCode OnClientToScreen(LPRECT lpRect) = 0;                                 ///<用于面板子控件转换成屏幕坐标
		virtual DMCode OnIsSizChanging() = 0;                                               ///<用于判断容器是否正在布局

		//----------------------------------------------
		// 加速相关
		virtual IDMAccelMgr* GetAccelMgr() = 0;

		//----------------------------------------------
		// 动画帧相关
		virtual DMCode OnRegisterTimeline(IDMTimeline *pHandler) = 0;						///< 注册使用全局定时器
		virtual DMCode OnUnregisterTimeline(IDMTimeline *pHandler) = 0;						///< 反注册不使用全局定时器
		virtual DMCode OnTimeline() = 0;													///< 不要在动画帧中调用OnRegisterTimeline、OnUnregisterTimeline
	
		//----------------------------------------------
		// 语言包相关
		virtual CStringW OnGetTransId() = 0;												///< 返回语言包特定id,内置如为NULL,则默认从custom中查找,否则,优先从此特定id查找,查不到,再去custom
	};
	typedef IDMContainer* IDMContainerPtr;

}//namespace DM