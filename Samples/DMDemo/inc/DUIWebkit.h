//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIWebkit.h 
// File Des: WebKit
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-11-24	1.0			
//--------------------------------------------------------
#pragma once
#include "DMWebkit.h"
#include <Imm.h>
#pragma comment(lib,"imm32.lib")

// WebKit的辅助类
class  DUIWebkitHelper:public DMSingletonT<DUIWebkitHelper>,public DMRefNum
{
public:
	DUIWebkitHelper();
	virtual~DUIWebkitHelper();
	typedef void (*FunDMInit)();
	typedef void (*FunDMShutdown)();
	typedef IDMWebViewPtr (*FunDMCreateWebView)();
	typedef void (*FunWkeDestroyWebView)(IDMWebViewPtr);
public:
	HINSTANCE             m_hDMWebkit;
	FunDMInit			  m_funDMInit;
	FunDMCreateWebView    m_funDMCreateWebView;
	FunWkeDestroyWebView  m_funDMDestroyWebView;
	FunDMShutdown		  m_funDMShutdown;
};

// WebKit
const char TM_TICKER = 1;
class DUIWebKit : public DUIWindow, protected DUIWndWebHost, protected IDMIdleHandler
{
	DMDECLARE_CLASS_NAME(DUIWebKit, L"Webkit",DMREG_Window)
public:
	DUIWebKit(void);
	~DUIWebKit(void);
	void Init();
	void UnInit();

	//---------------------------------------------------
	// Function Des: 对外接口
	//---------------------------------------------------
	IDMWebViewPtr GetWebView();
	void Zoom(bool zoomIn);

public:
	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SIZE(OnSize)
		DM_MSG_WM_TIMER(OnDUITimer)
		DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
		DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST,WM_MBUTTONDBLCLK,OnMouseEvent)
		MESSAGE_HANDLER_EX(WM_MOUSEWHEEL,OnMouseWheel)
		MESSAGE_HANDLER_EX(WM_KEYDOWN,OnKeyDown)
		MESSAGE_HANDLER_EX(WM_KEYUP,OnKeyUp)
		MESSAGE_HANDLER_EX(WM_CHAR,OnChar)
		MESSAGE_HANDLER_EX(WM_IME_STARTCOMPOSITION,OnImeStartComposition)
		DM_END_MSG_MAP()

public:
	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
	int OnCreate(LPVOID);	
	void OnDestroy();
	void DM_OnPaint(IDMCanvas *pCanvas);
	void OnSize(UINT nType, CSize size);
	LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam,LPARAM lParam); 
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam,LPARAM lParam); 
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam,LPARAM lParam);
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam,LPARAM lParam);
	LRESULT OnChar(UINT uMsg, WPARAM wParam,LPARAM lParam);
	LRESULT OnImeStartComposition(UINT uMsg, WPARAM wParam,LPARAM lParam);
	void DM_OnSetFocus();
	void DM_OnKillFocus();
	void OnDUITimer(char id);

public:
	//---------------------------------------------------
	// Function Des: 重载
	//---------------------------------------------------
	virtual DMCode DV_OnSetCursor(const CPoint &pt);

public:
	bool OnAttrUrl(LPCWSTR lpszValue, bool bLoadXml);
public:
	DM_BEGIN_ATTRIBUTES()
		DM_CUSTOM_ATTRIBUTE(L"url",OnAttrUrl)
		DM_INT_ATTRIBUTE(L"alpha",m_byAlpha,TRUE)
		DM_bool_ATTRIBUTE(L"enablezoom",m_bEnableZoom, TRUE)
		DM_END_ATTRIBUTES()

protected:
	virtual void DUIWndUpdate(const HDC hdc,int x, int y, int cx, int cy);
	virtual BOOL OnIdle();

public:
	IDMWebViewPtr			m_pWebView;
	CStringW				m_strUrl;
	BYTE					m_byAlpha;
	bool                    m_bEnableZoom;
	DMSmartPtrT<DUIWebkitHelper> m_pHelper;

};