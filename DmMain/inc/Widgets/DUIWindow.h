//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIWindow.h 
// File Des: 仿WPF的 Visual, 可视为模拟子窗口的HWND
// File Summary: http://msdn.microsoft.com/zh-cn/library/ms748373(v=vs.110).aspx
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-30	1.0			
//--------------------------------------------------------
#pragma once
#include "DUIWindowHelper.h"

namespace DM
{
	#define DMTR(str)									DV_GetTransText(str)          
	class DUIWindow;
	typedef DUIWindow* DUIWindowPtr;
	/// <summary>
	///		DUI窗口的基类，所有DUI均通过此类继承，以DV_开头的为虚函数，子类可重载,属性：<see cref="DMAttr::DUIWindowAttr"/>
	/// </summary>
	/// <remarks>
	///		1.DV_开头表示虚函数，子类可继承 
	///     2.DM_开头表示正常DUI函数,用于区分DMCWndBase.h中的封装
	///     3.通用的DM_Is判断函数使用bool返回值,DM_Get/DV_Get直接返回,其余使用DMCode
	/// </remarks>
	class DM_EXPORT DUIWindow:public DMBase
							 ,public DUIMsgHandler
	{
		DMDECLARE_CLASS_NAME(DUIWindow,L"window",DMREG_Window);
	public:
		DUIWindow();
		virtual~DUIWindow();

	public:
		//---------------------------------------------------
		// Function Des:基础属性
		LPCWSTR GetName();																///< 使用名字做为唯一标识
		int GetID();																	///< 使用数字做为唯一标识
		DMCode SetID(int id);															///< 设置id

		DUIWindow* FindChildByName(LPCWSTR lpszName,bool bPanelFind = false);			///< 通过名字查找DUI窗口
		DUIWindow* FindChildById(int ID,bool bPanelFind = false);						///< 通过ID查找DUI窗口
		DUIWindow* FindPanelChildByName(LPCWSTR lpszName,bool bPanelFind = false);		///< 通过名字从子panel列中查找DUI窗口
		DUIWindow* FindPanelChildById(int ID,bool bPanelFind = false);					///< 通过ID从子panel列中查找DUI窗口


		virtual DUIWND HitTestPoint(CPoint pt,bool bFindNoMsg = false);					///< 通过point来确认在哪个窗口上，用于模拟消息分发
		DUIWND GetDUIWnd(){return m_hDUIWnd;};											///< 获得模拟窗口句柄

		//---------------------------------------------------
		// 用户数据
		DMCode SetData(CStringW strKey, CStringW strValue, bool bReplace = true);       ///< CStringW也可以指定某个指针（指针和字符串互转）,strValue传空表示清空strKey
		CStringW GetData(CStringW strKey);
		
		//---------------------------------------------------
		// Function Des:初始化
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);						    ///< 1.初始化传入为本控件XmlNode 2.动态创建子控件列表(s),创建Child1,Child2格式应为:<A><Child1 ../><Child2 .../></A>,(A可为任意名)
		DMCode InitDMData(DMXmlNode &XmlNode);											///< 解析XML
		
		//---------------------------------------------------
		// Function Des:容器
		IDMContainerPtr GetContainer();													///< 获取容器
		DMCode SetContainer(IDMContainerPtr pContainer);								///< 设置容器
		DMCode DM_AddChildPanel(DUIWindowPtr pWnd);										///< 增加子容器(如DUIListBoxEx),仅用于查找panel的控件,pWnd同时也为IDMContainerPtr
		DMCode DM_RemoveChildPanel(DUIWindowPtr pWnd);									///< 移除子容器(如DUIListBoxEx),仅用于查找panel的控件,pWnd同时也为IDMContainerPtr
		DMCode DM_RemoveAllChildPanel();												///< 移除所有子容器											

		//---------------------------------------------------
		// Function Des: 换肤
		virtual DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);								///< 换肤,lp表示需换肤的类型

		//---------------------------------------------------
		// Function Des: tooltip
		virtual DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);			///< 更新tooltip显示信息
		
		//---------------------------------------------------
		// Function Des: ActiveX
		virtual DMCode DV_OnAxActivate(IUnknown *pUnknwn);								///< 将com传入到DUIWindow

		//---------------------------------------------------
		// Function Des:事件
		virtual DMCode DV_FireEvent(DMEventArgs &Evt);									///< 可重载的事件通知执行, 也是Event消息的起源
		virtual DMCode DV_DispatchEvent(DMEventArgs &Evt);                              ///< 群发事件消息到所有子DUI窗口
		DMEventMgrPtr GetEventMgrPtr(){return &m_EventMgr;};							///< 获取事件管理句柄,用于外部注册事件

		//---------------------------------------------------
		// Function Des:通用布局
		virtual DMCode DV_UpdateChildLayout();											///< 更新所有子窗口位置
		virtual DMCode DV_GetChildMeasureLayout(LPRECT lpRect);							///< 获得子窗口的布局空间大小,默认为客户区，但是tab这样的控件不一样
		virtual DMCode DV_GetDesiredSize(LPRECT pRcContainer,SIZE &sz);					///< 当没有指定窗口大小时，通过如皮肤或文字内容计算窗口的期望大小，

		bool DM_IsLayoutFinished();														///< 布局是否完成
		bool DM_IsParentFlowLayout();													///< 父窗口是否使用流式布局
		DMCode DM_FloatLayout(LPRECT lpRect);											///< 从绝对布局开始子布局
		DMCode DM_UpdateLayout(LPRECT lpRect);											///< 更新自身布局

		//---------------------------------------------------
		//  Function Des:流式布局
		virtual DMCode DV_Measure(CSize& AvailableSize);								///< 计算可用大小				
		virtual DMCode DV_Arrange(CRect& FinalRect);									///< 分配可用大小

		//---------------------------------------------------
		// Function Des:状态
		virtual DMCode DV_GetState(int& iState);										///< 获取当前绘制状态
		DWORD DM_ModifyState(DWORD dwAdd, DWORD dwRemove,bool bUpdate=false);			///< 更改状态

		//---------------------------------------------------
		// Function Des:语言包
		virtual CStringW DV_GetTransText(CStringW strSrc);								///< 获得转换的字符串

		//---------------------------------------------------
		// Function Des:属性,所有rect都是相对于宿主m_hWnd的窗口坐标

		/// 获取区域
		virtual DMCode DV_GetTextRect(LPRECT lpRect);									///< 获得文本绘制区
		virtual DMCode DV_GetWindowRect(LPRECT lpRect);									///< 获得窗口的宿主中大小,如不占位,不显示时为空
		virtual DMCode DV_GetClientRect(LPRECT lpRect);									///< 获得窗口的客户区
	
		/// 焦点相关
		virtual DMCode DV_SetFocusWnd();												///< 设置焦点,设置焦点会向新焦点窗口发送WM_SETFOCUS消息，同时向旧焦点窗口发送WM_KILLFOCUS消息
		virtual DMCode DV_KillFocusWnd();												///< 去掉焦点,可视为把焦点设置到一个空DUI窗口上
				bool   DM_IsFocusWnd();                                                 ///< 是否为焦点窗口
		virtual	bool DV_IsFocusable();													///< 是否有焦点属性,子窗口可通过此属性决定是否做焦点虚线框绘制

		virtual DMCode DV_OnNcHitTest(CPoint pt){return DM_ECODE_FAIL;};				///< 模拟非客户区,分发消息时如为true会自动转换成非客户区消息,用于scroll
		virtual DMCode DV_OnSetCursor(const CPoint &pt);								///< 设置当前的光标
		virtual UINT DV_OnGetDlgCode(){return 0;};										///< 表示需要某类键,分发时对此类键不能直接过滤,http://msdn.microsoft.com/zh-tw/library/t83978eh.aspx

		/// 刷新相关
		DMCode DM_Invalidate();															///< 刷新整个客户区域，如要刷新非客户区，使用DM_InvalidateRect(m_rcWindow)
		DMCode DM_InvalidateRect(LPRECT lpRect);										///< 刷新指定区域，内部会做合并计算真实刷新区
		DMCode DM_LockUpdate();															///< 锁定刷新				
		DMCode DM_UnlockUpdate();														///< 解锁刷新
		bool DM_IsUpdateLocked();														///< 是否锁定刷新

		/// Capture相关
		DUIWND DM_GetCapture();															///< 获取当前capture的DUI窗口
		DUIWND DM_SetCapture();															///< 设置capture，注意一定在调用DM_ReleaseCapture来释放
		DMCode DM_ReleaseCapture();														///< 释放capture

		/// 可视化相关
		bool DM_IsVisible(bool bCheckParent = false);								    ///< 是否可见
		DMCode DM_SetVisible(bool bVisible, bool bUpdate = false);						///< 设置可见
		bool DM_IsDisable(bool bCheckParent = false);									///< 是否禁用	
		DMCode DM_EnableWindow(BOOL bEnable,bool bUpdate=false);						///< enable窗口

		/// 过滤消息
		bool DM_IsMsgNoHandle();														///< 为true时,HitTestPoint时会此控件会被忽视,从而消息会被发送给同一区域的下一个控件

		/// checkbox
		bool DM_IsChecked();														    ///< 是否为checked状态
		DMCode DM_SetCheck(bool bCheck);											    ///< 设置checked状态

		/// RadioButton
		virtual bool DV_IsSiblingsAutoGroup(){return false;}							///< 是否自动把子控件放在一组,一组中默认只响应check状态的焦点，其余的忽视
		virtual DUIWindow* DV_GetSelSiblingInGroup(){return NULL;}					    ///< 获得组内的选中子控件
	
		//---------------------------------------------------
		// Function Des:缓存画布
		virtual bool DV_IsPreCmpSizeDiff();												///< 当size不变时，应该考虑不设置脏区, 可重载，防止特殊控件的父窗口背景变换
		DMCode DM_MarkCacheDirty(bool bDirty);											///< 缓存画布设置为脏																		
		bool DM_IsCacheDirty();															///< 缓存画布是否为脏
		bool DM_IsDrawToCache();														///< 是否绘制到缓存画布
		IDMCanvas* DM_GetCacheCanvas(){return m_pCacheCanvas;}							///< 获取缓存画布指针

		//---------------------------------------------------
		// Function Des: 定时器
		DMCode DM_SetTimer(char id, UINT uElapse);										///< 设置窗口相关定时器 id仅支持0-127,主窗口不允许定义成1,2
		DMCode DM_KillTimer(char id);													///< kill窗口相关定时器
		DMCode DM_SetThreadTimer(UINT_PTR id, UINT uElapse);							///< 设置线程回调定时器
		DMCode DM_KillThreadTimer(UINT_PTR id);											///< kill线程回调定时器

		//---------------------------------------------------
		// Function Des: 窗口树接口		
		inline int DM_GetChildCount(){return m_Node.m_nChildrenCount;}					///< 获取所有子窗口数目
		static DUIWindow* DM_GetNextVisibleWnd(DUIWindow* pWnd,const CRect& rcDraw);    ///< 获取rcDraw中pWnd的下一个可见窗口
		DMCode DM_InsertChild(DUIWindow* pNewChild, DUIWindow* pInsertAfter=NULL);		///< 插入子窗口
		DMCode DM_RemoveChildWnd(DUIWindow* pChild);									///< 移除子窗口
		DMCode DM_DestroyChildWnd(DUIWindow* pChild);									///< 发送WM_DESTROY,移除并释放子窗口(子窗口一般为new obj,外部不需要delete)
		DMCode DM_SetWndToTop();														///< 如为子窗口,则将自身放在firstchild位置
		DMCode DM_SetOwnerWnd(DUIWindow* pWnd);											///< 设置拥有者窗口(仅做辅助)
		DUIWindow* DM_GetWindow(int iCode);												///< 获取指定的窗口
		DUIWindow* DM_GetTopParentWnd();												///< 获取顶层父窗口
		

		//---------------------------------------------------
		// Function Des:绘制
		virtual bool DV_IsOnlyDrawClient();												///< 是否只绘制客户区
		virtual bool DV_IsStateChangeRedraw();											///< 当窗口状态变化时，是否重绘
		virtual bool DV_IsNeedDrawEnviron(){return true;};								///< 是否需要准备绘制环境
		virtual DMCode DV_OnStateChanged(DWORD dwOldState,DWORD dwNewState);		    ///< 状态改变时绘制

		/// 文字相关
		virtual DMCode DV_SetWindowText(LPCWSTR lpszText);							    ///< 设置文本
		virtual DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);						///< 绘制文字,在WM_PAINT中触发
		virtual DMCode DV_DrawMultText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat,int nLineInter);   ///< 示例代码,用于xml中的字符串\R\N解析
		
		/// 默认焦点框
		virtual DMCode DV_DrawDefFocus(IDMCanvas* pCanvas);								///< 绘制焦点框
		virtual DMCode DV_DrawDefFocusRect(IDMCanvas* pCanvas,CRect rcFocus);			///< 绘制焦点框

		/// 默认绘制环境
		virtual DMCode DV_PushDrawEnviron(IDMCanvas* pCanvas, DUIDrawEnviron&old);     ///< 设置当前字体和颜色等环境
		virtual DMCode DV_PopDrawEnviron(IDMCanvas* pCanvas, DUIDrawEnviron&old);      ///< 恢复先前字体和颜色等环境
		DMCode DV_SetDrawEnvironEx(IDMCanvas* pCanvas);								   ///< 从最顶层父窗口开始,一层层设置环境

		/// 屏幕画布相关(重要动画绘制)
		IDMCanvas* DM_GetCanvas(LPRECT lpRect,DWORD dcFlags,bool bClientDC=true);	   ///< 获取画布（主窗口提供:1.默认画布(默认字体)2.复制窗口缓存画布区域3.复制背景色）
		DMCode DM_ReleaseCanvas(IDMCanvas* pCanvas);								   ///< 更新画布到窗口（主窗口提供:在不是创建默认画布状态下）
		DMCode DM_DrawBackground(IDMCanvas* pCanvas,LPRECT lpRect);					   ///< 绘制本窗口的背景到画布,从顶层父窗口绘制到本窗口结束
		DMCode DM_DrawForeground(IDMCanvas* pCanvas,LPRECT lpRect);					   ///< 绘制本窗口前景到画布，前景指本窗口外的其他可见窗口（不是本窗口的子窗口）
		DMCode DM_RedrawRegion(IDMCanvas* pCanvas, IDMRegion* lpRgn);				   ///< 在lpRgn区域绘制自身(包括所有可见子窗口)到画布
	
		//---------------------------------------------------
		// Function Des:消息
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			DM_MSG_WM_NCPAINT(DM_OnNcPaint)
			DM_MSG_WM_ERASEBKGND(DM_OnEraseBkgnd)
			DM_MSG_WM_ENABLE(DM_OnEnable)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
			DM_MSG_WM_WINPOSCHANGED(OnWindowPosChanged)
			
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_SIZE(OnSize)
			MSG_WM_NCCALCSIZE(OnNcCalcSize)
			MSG_WM_SHOWWINDOW(OnShowWindow)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSEHOVER(OnMouseHover)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
		DM_END_MSG_INBASE()// 基类使用DM_END_MSG_INBASE，其余子类使用DM_END_MSG_MAP
	public:
		LRESULT DM_SendMessage(UINT uMsg, WPARAM wp = 0, LPARAM lp = 0,BOOL *pbMsgHandled=NULL);
		LRESULT DM_DispatchMessage(MSG* pMsg, BOOL *pbMsgHandled=NULL);
		LRESULT OnWindowPosChanged(LPRECT lpRcContainer);		  // 0表示成功
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
		int OnCreate(LPVOID);
		BOOL DM_OnEraseBkgnd(IDMCanvas* pCanvas);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void DM_OnNcPaint(IDMCanvas* pCanvas);
		void DM_OnSetFocus();
		void DM_OnKillFocus();
		void DM_OnEnable(BOOL bEnable,UINT nStatus);
		void OnDestroy();
		void OnSize(UINT nType, CSize size);
		void OnShowWindow(BOOL bShow, UINT nStatus);         
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseHover(WPARAM wParam,CPoint ptPos);
		void OnMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

		//---------------------------------------------------
		// Function Des: 模板
		template<class T> 
		T* FindChildByNameT(LPCWSTR lpszName,bool bPanelFind = false)						//< 和扩展接口<see cref="FindChildByName"/>取得一样，强制转换模板
		{	
			DUIWindow *pFindWnd = FindChildByName(lpszName,bPanelFind);
			if (NULL == pFindWnd) return NULL;
			return dynamic_cast<T*>(pFindWnd);
		}
		template<class T> 
		T* FindChildByIdT(int ID,bool bPanelFind = false)									//< 和扩展接口<see cref="FindChildById"/>取得一样，强制转换模板
		{
			DUIWindow *pFindWnd = FindChildById(ID,bPanelFind);
			if (NULL == pFindWnd)return NULL;
			return dynamic_cast<T*>(pFindWnd);
		}

	public:
		virtual BOOL DV_WndProc(UINT uMsg,WPARAM wParam,LPARAM lParam, LRESULT& lResult);	///< DUI消息最后处理函数
		virtual DMCode OnAttributeFinished(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml,DMCode iErr);
	
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CHAINFUN_ATTRIBUTTE(DM_Layout)
			DM_CHAIN_ATTRIBUTTE(m_pDUIXmlInfo)
		DM_END_ATTRIBUTES()
	
	public:
		DMCode DM_Layout(LPCWSTR lpszAttribute,LPCWSTR lpszValue,bool bLoadXml);			///< 用于非初始化时,通过属性调整布局

	public:
		DUIWND                                  m_hDUIWnd;									///< 类似m_hWnd，DUI窗口的唯一标识    
		CRect                                   m_rcWindow;									///< 窗口在容器中的位置
		DWORD									m_dwDUIState;								///< 窗口状态
		
		DMSmartPtrT<IDMCanvas>					m_pCacheCanvas;								///< 缓存画布
		DUIWindowNode                           m_Node;										///< DUI的树形窗口列表

		DMSmartPtrT<DUIWindow_XmlInfo>          m_pDUIXmlInfo;								///< 自带的XML属性
		DUIWindow_Data							m_DUIData;

		DMEventMgr								m_EventMgr;									///< 事件项管理
		IDMContainerPtr                         m_pContainer;								///< 容器对象，一般为宿主

		DMSmartPtrT<IDMLayout>                  m_pLayout;									///< 锚点布局对象
		bool                                    m_bFloatLayout;								///< 绝对坐标

		CMap<CStringW,CStringW>					m_StrDataMap;
		CArray<DUIWindowPtr>					m_ChildPanelArray;							///< 控件中包含的子容器列表
	
		//-----------------------------------------------------------------------------------------------
		CStringW								m_strXml;									///< 仅debug下有效,可提供给spy++显示
		DMXmlNode                               m_XmlNode;                                  ///< 仅设置_DMDesigner_宏时有效,design中xmldoc一直是有效的，所有它的对象也会一直有效
	};
}//namespace DM
