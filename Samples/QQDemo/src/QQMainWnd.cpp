﻿#include "QQDemoAfx.h"
#include "QQMainWnd.h"
#include "resource.h"
#include "TrayIconWndMgr.h"
#include "DUIMenu.h"

BEGIN_MSG_MAP(CQQMainWnd)
	MSG_WM_CREATE(OnCreate)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SHOWWINDOW(OnShowWindow)
	MSG_WM_SIZE(OnSize)
	MSG_WM_COMMAND(OnCommand)
	//MESSAGE_HANDLER_EX(WM_SHOWTRAYMENU, OnShowTrayMenu)//老代码
	CHAIN_MSG_MAP(DMTrayIconImpl)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()

BEGIN_EVENT_MAP(CQQMainWnd)
	EVENT_NAME_COMMAND("minbutton", OnMinimize)
	EVENT_NAME_COMMAND("closebutton",OnClose)
	EVENT_NAME_COMMAND("name_edit_btn",OnNameEditBtn)
	EVENT_NAME_COMMAND("btn_spy",OnSpy)
	EVENT_NAME_COMMAND("btn_skin",OnSkin)
	EVENT_NAME_COMMAND("btn_widget",OnWidget)
END_EVENT_MAP()

CQQMainWnd::CQQMainWnd()	
{
	for (int i=0;i<3;i++)
	{
		m_pSubListTree[i] = NULL;
		m_pSubListEx[i]   = NULL;
	}
	m_pListEx      = NULL;
//	new CTrayIconWndMgr;//老代码
}

CQQMainWnd::~CQQMainWnd()
{
// 	CTrayIconWndMgr::getSingletonPtr()->SetBalloonDetails(L"DM库示例",L"QQDemon已退出");//老代码
// 	delete CTrayIconWndMgr::getSingletonPtr();
	ShowTrayBalloonTip(L"DM库示例", L"QQDemon已退出");//新trayicon代码
}

int CQQMainWnd::OnCreate(LPVOID)
{
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetPosition(0, 0);
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CQQMainWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// 设置icon
	HICON hIcon = ::LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_QQ));
	SetIcon(hIcon,FALSE);

	// 注册nameedit失去焦点事件
	DUIEdit* pNameEdit = FindChildByNameT<DUIEdit>("name_edit");
	if (pNameEdit)
	{
		// 示例绑定动态分配数据
		byte* pBuf = new byte[100];
		pNameEdit->SetData("key1",CStringA((char*)&pBuf,sizeof(LONG_PTR)));// strValue中记录的前四位内存值即pBuf地址
		CStringA strValue = pNameEdit->GetData("key1");
		byte* pByte = (byte*)(*(LONG_PTR*)strValue.GetBuffer());// 取前四位内存值转换成指针
		assert (pByte == pBuf);
		DM_DELETE_ARRAY(pByte);
		pNameEdit->SetData("key1","");// 传空值清空key1

		pNameEdit->m_EventMgr.SubscribeEvent(DM::DMEventRENotifyArgs::EventID, Subscriber(&CQQMainWnd::OnNameEditKillFocus, this));
	}

	// 模拟展开收缩小三角
	m_pListEx = FindChildByNameT<DUIListBoxEx>("listex");
	if (m_pListEx)
	{
		LPLBITEMEX pItemEx = m_pListEx->GetObj(0);m_pSubListTree[0] = pItemEx->pPanel->FindChildByNameT<DUITreeCtrl>("listex_tree0");
		pItemEx = m_pListEx->GetObj(1);m_pSubListEx[0]	 = pItemEx->pPanel->FindChildByNameT<DUIListBoxEx>("listex_listex0");
		pItemEx = m_pListEx->GetObj(2);m_pSubListTree[1] = pItemEx->pPanel->FindChildByNameT<DUITreeCtrl>("listex_tree1");
		pItemEx = m_pListEx->GetObj(3);m_pSubListEx[1]	 = pItemEx->pPanel->FindChildByNameT<DUIListBoxEx>("listex_listex1");
		pItemEx = m_pListEx->GetObj(4);m_pSubListTree[2] = pItemEx->pPanel->FindChildByNameT<DUITreeCtrl>("listex_tree2");
		pItemEx = m_pListEx->GetObj(5);m_pSubListEx[2]	 = pItemEx->pPanel->FindChildByNameT<DUIListBoxEx>("listex_listex2");
	}
	DMASSERT(m_pListEx&&m_pSubListTree[0]&&m_pSubListTree[1]&&m_pSubListTree[2]&&m_pSubListEx[0]&&m_pSubListEx[1]&&m_pSubListEx[2]);
	for (int i=0;i<3;i++)
	{
		m_pSubListTree[i]->m_EventMgr.SubscribeEvent(DMEventTCExpandArgs::EventID, Subscriber(&CQQMainWnd::OnSubListTree, this));
		m_pSubListEx[i]->m_EventMgr.SubscribeEvent(DMEventLBSelChangedArgs::EventID, Subscriber(&CQQMainWnd::OnSubListEx, this));
	}

/*	// 增加一个托盘图标
	CTrayIconWndMgr::getSingleton().InstallTrayIcon(L"欢迎使用DM库示例:QQDemo",m_hWnd,hIcon,IDI_QQ);
	// 增加一个汽泡提示
	CTrayIconWndMgr::getSingletonPtr()->SetBalloonDetails(L"DM库示例",L"QQDemo"); */

	//下面是新的trayico封装
	InstallIcon(L"欢迎使用DM库示例:QQDemo", hIcon, IDI_QQ);
	ShowTrayBalloonTip(L"DM库示例", L"QQDemo");
	return TRUE;
}

void CQQMainWnd::OnSize(UINT nType, CSize size)
{
	if (!IsIconic()) 
	{
		CRect rcWnd;
		::GetWindowRect(m_hWnd, &rcWnd);
		::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);
		HRGN hWndRgn = ::CreateRoundRectRgn(0, 0, rcWnd.right, rcWnd.bottom,1,1);
		SetWindowRgn(hWndRgn, TRUE);
		::DeleteObject(hWndRgn);

		DUITabCtrl* pTab = FindChildByNameT<DUITabCtrl>("maintab");
		if (pTab)
		{
			int itemspace = (rcWnd.Width()-50*5)/4;
			CStringA strValue;
			strValue.Format("%d\n",itemspace);
			pTab->SetAttribute("itemspace",strValue,false);
		}
	}
	m_WndShadow.SetPosition(0, 0);
	SetMsgHandled(FALSE);
}   

void CQQMainWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
	{
		m_WndShadow.SetPosition(0, 0);
	}
}

int CQQMainWnd::OnTrayOnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return OnShowTrayMenu(uMsg, wParam, lParam);
}

LRESULT CQQMainWnd::OnShowTrayMenu(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DUIMenu Menu;
	Menu.LoadMenu("LayOut","dui_exitmenu");
	POINT pt;
	GetCursorPos(&pt);
	Menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
	return TRUE;
} 

void CQQMainWnd::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (7001 == nID)
	{
		OnClose();
	}
	else if (7000 == nID)
	{
		CStringW strUrl = L"http://shang.qq.com/wpa/qunwpa?idkey=a4eb76996f3c7cb6018a3ca375a5df3360ba818579f60516092edd9ed1de23a8";
		//先使用默认浏览器打开，若打开失败（不存在默认浏览器），则强制使用IE打开
		HINSTANCE hIns = ::ShellExecute(NULL, _T("open"), strUrl, NULL, NULL, SW_SHOWNORMAL);
		if ( (LPARAM)(hIns) <= 32 )	//If the function succeeds, it returns a value greater than 32.
		{
			ShellExecute(NULL, _T("open"), _T("iexplore.exe"), strUrl, NULL, SW_SHOW);
		}
	}
}

DMCode CQQMainWnd::OnClose()
{
	m_WndShadow.SetSize();
	DM_AnimateWindow(300,AW_SLIDE|AW_VER_NEGATIVE|AW_HIDE);
	if (m_pSkinWnd&&m_pSkinWnd->IsWindow())
	{
		m_pSkinWnd->OnClose();
	}
	if (m_pWidgetWnd&&m_pWidgetWnd->IsWindow())
	{
		m_pWidgetWnd->OnClose();
	}
	if (m_pSpyWnd&&m_pSpyWnd->IsWindow())
	{
		m_pSpyWnd->OnClose();
	}
	DestroyWindow();
	return DM_ECODE_OK;
}		

DMCode CQQMainWnd::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	return DM_ECODE_OK;
}

DMCode CQQMainWnd::OnNameEditBtn()
{
	DUIButton* pNameEditBtn = FindChildByNameT<DUIButton>("name_edit_btn");
	DUIEdit* pNameEdit = FindChildByNameT<DUIEdit>("name_edit");
	if (pNameEditBtn)
	{
		pNameEditBtn->DM_SetVisible(false,true);
	}
	if (pNameEdit)
	{
		pNameEdit->DM_SetVisible(true,true);
		pNameEdit->DV_SetFocusWnd();
	}
	return DM_ECODE_OK;
}

DMCode CQQMainWnd::OnSkin()
{
	do 
	{
		if (m_pSkinWnd&&m_pSkinWnd->IsWindow())
		{
			break;
		}
		m_pSkinWnd.Release();
		m_pSkinWnd.Attach(new CSkinWnd(this));
		m_pSkinWnd->DM_CreateWindow("dui_skin");				// 创建主窗口
		m_pSkinWnd->SendMessage(WM_INITDIALOG);
		m_pSkinWnd->CenterWindow();
		m_pSkinWnd->ShowWindow(SW_SHOW);
	    m_pSkinWnd->SetWindowPos(m_hWnd, 0,0,0, 0, SWP_NOSIZE|SWP_NOMOVE);

	} while (false);
	return DM_ECODE_OK;
}

DMCode CQQMainWnd::OnSpy()
{
	do 
	{
		if (m_pSpyWnd&&m_pSpyWnd->IsWindow())
		{
			break;
		}
		m_pSpyWnd.Release();
		m_pSpyWnd.Attach(new CSpyWnd());
		m_pSpyWnd->DM_CreateWindow("dui_spy");				// 创建主窗口
		m_pSpyWnd->SendMessage(WM_INITDIALOG);
		m_pSpyWnd->CenterWindow();
		m_pSpyWnd->ShowWindow(SW_SHOW);
		m_pSpyWnd->SetWindowPos(m_hWnd, 0,0,0, 0, SWP_NOSIZE|SWP_NOMOVE);

	} while (false);
	return DM_ECODE_OK;
}

DMCode CQQMainWnd::OnWidget()
{
	do 
	{
		if (m_pWidgetWnd&&m_pWidgetWnd->IsWindow())
		{
			break;
		}
		m_pWidgetWnd.Release();
		m_pWidgetWnd.Attach(new CWidgetWnd());
		m_pWidgetWnd->DM_CreateWindow("dui_widget");				// 创建主窗口
		m_pWidgetWnd->SendMessage(WM_INITDIALOG);
		m_pWidgetWnd->CenterWindow();
		m_pWidgetWnd->ShowWindow(SW_SHOW);
		m_pWidgetWnd->SetWindowPos(m_hWnd, 0,0,0, 0, SWP_NOSIZE|SWP_NOMOVE);

	} while (false);
	return DM_ECODE_OK;
}

DMCode CQQMainWnd::OnNameEditKillFocus(DMEventArgs* pEvent)
{
	DMEventRENotifyArgs *pEvt = (DMEventRENotifyArgs*)pEvent;
	if (pEvt)
	{
		static bool bRetry = false;
		if (EN_KILLFOCUS==pEvt->m_iNotify&&false == bRetry)
		{
			DUIEdit* pNameEdit = (DUIEdit*)pEvt->m_pSender;
			CStringW strInfo = pNameEdit->GetWindowText();
			if (strInfo.IsEmpty())
			{
				strInfo = L"编辑个性签名";
			}
			bRetry = true;
			pNameEdit->DM_SetVisible(false,true);// 此函数会引发多次进入EN_KILLFOCUS，所以加判断
			DUIButton* pNameEditBtn = FindChildByNameT<DUIButton>("name_edit_btn");
			if (pNameEditBtn)
			{
				pNameEditBtn->DV_SetWindowText(strInfo);
				pNameEditBtn->DM_SetVisible(true,true);
			}
			bRetry = false;
		}
	}

	return DM_ECODE_OK;
}

DMCode CQQMainWnd::OnSubListTree(DMEventArgs* pEvent)
{
	DMEventTCExpandArgs *pEvt = (DMEventTCExpandArgs*)pEvent;
	do 
	{
		if (pEvt->m_pSender == m_pSubListTree[0])
		{
			m_pListEx->SetItemHeight(1,pEvt->m_bCollapsed?0:56);
			m_pSubListEx[0]->SetAllItemHeight(28);
			m_pSubListEx[0]->m_iHoverItem =m_pSubListEx[0]->m_iSelItem= -1;
		}
		if (pEvt->m_pSender == m_pSubListTree[1])
		{
			m_pListEx->SetItemHeight(3,pEvt->m_bCollapsed?0:140);
			m_pSubListEx[1]->SetAllItemHeight(28);
			m_pSubListEx[1]->m_iHoverItem =m_pSubListEx[1]->m_iSelItem= -1;
		}
		if (pEvt->m_pSender == m_pSubListTree[2])
		{
			m_pListEx->SetItemHeight(5,pEvt->m_bCollapsed?0:140);
			m_pSubListEx[2]->SetAllItemHeight(28);
			m_pSubListEx[2]->m_iHoverItem =m_pSubListEx[2]->m_iSelItem= -1;
		}
	} while (false);
	return DM_ECODE_OK;
}

DMCode CQQMainWnd::OnSubListEx(DMEventArgs* pEvent)
{
	DMEventLBSelChangedArgs *pEvt = (DMEventLBSelChangedArgs*)pEvent;
	do 
	{
		DUIListBoxEx* pCur = (DUIListBoxEx*)pEvt->m_pSender;
		pCur->SetItemHeight(pEvt->m_nOldSel,28);
		pCur->SetItemHeight(pEvt->m_nNewSel,54);
		if (m_pSubListEx[0] == pCur)
		{
			m_pListEx->SetItemHeight(1,pCur->GetAllItemHeight());
		}
		if (m_pSubListEx[1] == pCur)
		{
			m_pListEx->SetItemHeight(3,pCur->GetAllItemHeight());
		}
		if (m_pSubListEx[2] == pCur)
		{
			m_pListEx->SetItemHeight(5,pCur->GetAllItemHeight());
		}
	} while (false);
	return DM_ECODE_OK;
}