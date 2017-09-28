//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
//
// File Name: DMHWnd.h
// File Des: 主窗口类,用于CWnd相关Create
// File Summary:  成员变量总是放在ClassName_Data类中，XML变量总是放在ClassName_XmlInfo类中
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-11	1.0
//      k000		2017-09-07	1.0			Add OnPostCreate method called by
//																		DM_CreateWindowEx after window is created.
//-------------------------------------------------------
#pragma once
#include "DMContainerImpl.h"
#include "DMHWndHelper.h"
#include "DMMsgLoop.h"
namespace DM
{
	/// <summary>
	///		此为最重要窗口主类，所有实例主窗口都应该直接或间接继承于此类,属性：<see cref="DMAttr::DMHWndAttr"/>
	/// </summary>
	/// <remarks>
	///		内部实现了加载资源、创建窗口、消息分发处理等行为
	/// <remarks>
	class DM_EXPORT DMHWnd:public DMCWnd          // 窗口基础消息处理
						  ,public DMContainerImpl // 消息分发到子DUI窗口
						  ,public DUIWindow		  // 主窗口自身为DUI窗口
						  ,public IDMAnimateOwner // 动画窗口的拥有者
	{
	public:
		DMHWnd();
	public:
		//---------------------------------------------------
		// Function Des: 创建窗口接口函数
		//---------------------------------------------------
		HWND DM_CreateWindow(LPCWSTR lpszXmlId,int x=0, int y=0, int nWidth=0, int nHeight=0, HWND hWndParent=NULL, bool bShadow=false);///< 创建窗口

		/// -------------------------------------------------
		/// @brief			创建窗口
		/// @param[in]		lpszXmlId       使用的XML文件在dmindex.xml中的标识
		/// @param[in]		lpWindowName	窗口名
		/// @param[in]		dwStyle			样式
		/// @param[in]		dwExStyle		扩展样式
		/// @param[in]		x				x坐标
		/// @param[in]		y				y坐标
		/// @param[in]		nWidth			宽度
		/// @param[in]		nHeight			高度
		/// @param[in]		hWndParent		父窗口
		/// @param[in]		lpParam			Long pointer to a value to be passed to the window through the CREATESTRUCT structure passed in the lParam parameter the WM_CREATE message
		/// @param[in]		bShadow         是否使用阴影窗口
		/// @return			HWND
		HWND DM_CreateWindowEx(LPCWSTR lpszXmlId, LPCWSTR lpWindowName,DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, PVOID lpParam, bool bShadow=false);


		/// -------------------------------------------------
		/// @brief			创建窗口
		/// @param[in]		pXmlBuf         使用的XML文件的buf,外部请转换成utf8格式
		/// @param[in]		bufLen          使用的XML文件的buf长度
		/// @param[in]		lpWindowName	窗口名
		/// @param[in]		dwStyle			样式
		/// @param[in]		dwExStyle		扩展样式
		/// @param[in]		x				x坐标
		/// @param[in]		y				y坐标
		/// @param[in]		nWidth			宽度
		/// @param[in]		nHeight			高度
		/// @param[in]		hWndParent		父窗口
		/// @param[in]		lpParam			Long pointer to a value to be passed to the window through the CREATESTRUCT structure passed in the lParam parameter the WM_CREATE message
		/// @param[in]		bShadow         是否使用阴影窗口
		/// @return			HWND
		HWND DM_CreateWindowEx(void *pXmlBuf, size_t bufLen, LPCWSTR lpWindowName,DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, PVOID lpParam, bool bShadow=false);


		/// -------------------------------------------------
		/// @brief		更新窗口到HDC上
		/// @param[in]  hdc              要更新的hdc
		/// @param[in]  rcInvalid        要更新的区域
		virtual void UpdateHWnd(HDC hdc,const CRect &rcInvalid,bool bUpdate = false);
		void RedrawAll();

		//---------------------------------------------------
		// Function Des: XML
		//---------------------------------------------------
		DMCode LoadDMData(LPCWSTR lpszXmlId);									///< 解析XML
		DMCode LoadDMData(void *pXmlBuf, size_t bufLen);
		DMCode InitFromDMData();												///< 通过XML数据初始化

	public:

		//---------------------------------------------------
		// Function Des: 消息分发系列函数
		//---------------------------------------------------
		void OnPaint(HDC hdc);
		void OnPrint(HDC hdc, UINT uFlags);
		void OnSize(UINT nType, CSize size);									///< 重设画布大小
		void OnSizing(UINT nSide, LPRECT lpRect);
		void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);								///< 控制最大、最小窗口
		void OnDestroy();
		void OnActivate(UINT nState, BOOL bMinimized, HWND wndOther);
		void OnSetFocus(HWND wndOld);
		void OnKillFocus(HWND wndFocus);
		void OnTimer(UINT_PTR idEvent);											///< 分发FastTimer
		virtual void OnDUITimer(char id);										///< 用于主窗口自己定义了定时器，重载处理

		void OnMove(CPoint pt);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		BOOL OnNcActivate(BOOL bActive);
		BOOL OnEraseBkgnd(HDC hdc);
		BOOL OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
		UINT OnNcHitTest(CPoint point);
		int OnCreate(LPCREATESTRUCT lpCreateStruct);
		LRESULT OnNcCreate(LPCREATESTRUCT lpCreateStruct);						///< 如果OnNcCreate返回false，窗口将不会被创建！
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
		LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);			///< 用于处理鼠标点击消息
		LRESULT OnKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);			///< 键盘消息传递，如焦点
		LRESULT OnHostMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);


		//---------------------------------------------------
		// Function Des: 动画
		//---------------------------------------------------
		DMCode DM_AnimateWindow(DWORD dwTime,DWORD dwFlags);					    ///< 动画效果
		DUIWindow* GetAnimateOwnerWnd();
		DMCode AnimateBegin_Callback(IDMAnimate*pAni,WPARAM wp, LPARAM lp){return DM_ECODE_NOTIMPL;};
		DMCode AnimateMid_Callback(IDMAnimate*pAni, WPARAM wp, LPARAM lp){return DM_ECODE_NOTIMPL;};
		DMCode AnimateEnd_Callback(IDMAnimate*pAni,WPARAM wp, LPARAM lp){return DM_ECODE_NOTIMPL;};

		//---------------------------------------------------
		// Function Des: 容器部分
		//---------------------------------------------------
		DMCode OnSetCaptureWnd(DUIWND DUIWnd,DUIWNDPtr pOldDUIWnd=NULL);
		DMCode OnReleaseCaptureWnd();
		DMCode OnFireEvent(DMEventArgs &Evt);
		DMCode OnGetDraw(IDMDraw** ppObj);
		DMCode OnGetCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas**ppCanvas);
		DMCode OnReleaseCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas*pCanvas);
		HWND   OnGetHWnd();
		DMCode OnGetContainerRect(LPRECT lpRect);
		DMCode OnIsTranslucent();
		DMCode OnUpdateWindow();
		DMCode OnForceUpdateWindow();
		DMCode OnUpdateRect(LPCRECT lpRect,DUIWND hDUIWnd);
		DMCode OnClientToScreen(LPRECT lpRect);
		DMCode OnIsSizChanging();
		DMCode OnRegisterTimeline(IDMTimeline *pHandler);
		DMCode OnUnregisterTimeline(IDMTimeline *pHandler);
		CStringW OnGetTransId();

		//---------------------------------------------------
		// Function Des: spy++部分
		//---------------------------------------------------
		LRESULT OnSpy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:// 辅助
		DMCode DM_UpdateLayeredWindow(IDMCanvas* pCanvas,BYTE byAlpha, LPRECT lpRect);
		DMCode DM_UpdateShowCanvas(LPRECT lpRect);

  protected:
		virtual void OnAfterCreated();							///< 窗口创建成功后调用
		virtual void OnAfterClosed();							///< 窗口退出后,内部判断如果此窗口运行消息循环,自动发送WM_QUIT消息,用户可重载处理

	public:
		DECLARE_MSG_MAP()
		DECLARE_EVENT_MAP()

		DM_BEGIN_ATTRIBUTES()
			DM_CHAIN_ATTRIBUTTE(m_pHWndXmlInfo)
		DM_END_ATTRIBUTES()

	public:
		// 绘制区 --------------------------------------------
		DMSmartPtrT<IDMCanvas>                  m_pShowCanvas;
		DMSmartPtrT<IDMDraw>                    m_pDraw;

		// 半透明窗口蒙版 ------------------------------------
		DMDummyWnd								m_dummyWnd;

		// 其他属性 ------------------------------------------
		DMHWnd_Data								m_HWndData;
		DMSmartPtrT<DMHWnd_XmlInfo>			    m_pHWndXmlInfo;

		DMSmartPtrT<IDMToolTip>                 m_pToolTip;

		bool                                    m_bSizeChanging;///< size改变到刷新期间,认为是size改变期，此时不允许强制更新画布，因为画布此时清0了

		DMSmartPtrT<DMMsgLoop>                  m_pCurMsgLoop;  ///< 当前消息循环对象,不排除有使用多线程的

	};
}//namespace DM
