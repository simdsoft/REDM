//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: MainWnd.h 
// File Des: 主窗口
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-2	1.0		
//-------------------------------------------------------
#pragma once
#include "SkinWnd.h"
#include "AttributeTestWnd.h"
#include "DUIWebkit.h"
#include "IDMMsg.h"

class CMainWnd:public DMHWnd, public IDMWebEvent
{
public:
	CMainWnd();
	~CMainWnd();
	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	// Input Parameter:
	// Output Parameter:
	// return:
	//---------------------------------------------------
	DMCode OnTest(); // 通用的测试
	DMCode OnClose();
	DMCode OnMaximize();
	DMCode OnRestore();
	DMCode OnMinimize();
	DMCode OnSkin();
	DMCode OnAttrTest();
	DMCode OnDlgTest();
	DMCode OnListTest();
	DMCode OnReplayGif();
	DMCode OnOpenGif();
	DMCode OnBtnMenu();

	// 定时器
	DMCode OnStartBtn();
	DMCode OnStopBtn();

	// 菜单测试
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);

	// Webkit测试
	DMCode OnBtnWebBack();
	DMCode OnBtnWebRefresh(); 
	DMCode OnBtnWebfront();
	static void OnTitleChanged(const DMClientHandler*, DMString title);
	static void OnURLChanged(const DMClientHandler*, DMString url);

	// IE测试
	DMCode OnBtnIeBack();
	DMCode OnBtnIeRefresh();
	DMCode OnBtnIefront();
	DMCode IERefreshWeb(DMEventArgs* pEvent);

	DMCode NavigateComplete2(DUIWND hWnd,DMIN IDispatch *pDisp,DMIN wchar_t *pUrl);

	// 动画------------------------------------
	DMCode AnimateBegin_Callback(IDMAnimate*pAni,WPARAM wp, LPARAM lp);		
	DMCode AnimateMid_Callback(IDMAnimate*pAni, WPARAM wp, LPARAM lp);
	DMCode AnimateEnd_Callback(IDMAnimate*pAni,WPARAM wp, LPARAM lp);
	// 
	DMCode OnScreen4Start();
	DMCode OnScreenMeltStart();
	DMCode OnLineOpenAnimateStart();
	DMCode OnLineCloseAnimateStart();
	DMCode On3DAnimateStart();
	DMCode On3Dx(DMEventArgs* pEvent);
	DMCode On3Dy(DMEventArgs* pEvent);
	DMCode On3Dz(DMEventArgs* pEvent);
	DMCode On3Df(DMEventArgs* pEvent);
public:
	DMCode RefreshWeb(DMEventArgs* pEvent);

public:
	DMCode MyTimer();
	DMCode OnTimeline();

protected:
	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	DECLARE_EVENT_MAP()

private:
	DMSmartPtrT<CSkinWnd>			m_pSkinWnd;
	DMSmartPtrT<CAttributeTestWnd>	m_pAttrTestWnd;
	DUIWebKit*					    m_pWebkit;
	IDUIWeb*						m_pIE;
	bool                            m_bStartTimer;
public:
	int                             m_HBk;
	int                             m_SBk;
	int                             m_LBk;
	int                             m_ABk;

	DMClientHandler					m_handler;// 必须是全局的，不然会崩

public:// 动画
	DMSmartPtrT<IDMAnimate>         m_pAnimate;

};