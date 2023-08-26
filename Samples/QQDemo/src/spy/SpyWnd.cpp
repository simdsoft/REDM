﻿#include "QQDemoAfx.h"
#include "SpyWnd.h"

#pragma execution_character_set("utf-8")

BEGIN_MSG_MAP(CSpyWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(DMHWnd)
END_MSG_MAP()
BEGIN_EVENT_MAP(CSpyWnd)
	EVENT_NAME_COMMAND("spy_minbutton", OnMinimize)
	EVENT_NAME_COMMAND("spy_closebutton",OnClose)
	EVENT_NAME_HANDLER("spyfindbtn",DMSpyInitArgs::EventID,OnSpyInitEvent)
	EVENT_NAME_HANDLER("spytree",DMEventTCSelChangedArgs::EventID,OnTreeSelChanged)
END_EVENT_MAP()
CSpyWnd::CSpyWnd()
{
	m_pTreeCtrl = NULL;
	m_pTreeEdit = NULL;
	m_pSearchSta= NULL;
	m_hRootWnd  = NULL;
	m_bDraw     = false;
	m_bHittest  = false;
	m_bInit     = false;
	m_pSpyEnum  = new DMSpyEnum;
	memset(m_pSpyEnum,0,sizeof(DMSpyEnum));
}

CSpyWnd::~CSpyWnd()
{
	DM_DELETE(m_pSpyEnum);
}

DMCode CSpyWnd::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	return DM_ECODE_OK;
}

DMCode CSpyWnd::OnClose()
{
	DestroyWindow();
	return DM_ECODE_OK;
}	

BOOL CSpyWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_pTreeCtrl  = FindChildByNameT<DUITreeCtrl>("spytree");
	m_pTreeEdit  = FindChildByNameT<DUIRichEdit>("spyedit");
	m_pSearchSta = FindChildByNameT<DUIStatic>("search_sta");
	m_pSearchSta->DV_SetWindowText(L"请拖动左侧的指针到需要查看的窗口/控件上释放");
	DMASSERT(m_pTreeCtrl);DMASSERT(m_pTreeEdit);DMASSERT(m_pSearchSta);

	return TRUE;
}

DMCode CSpyWnd::OnSpyInitEvent(DMEventArgs *pEvt)
{
	DMSpyInitArgs *pInitEvt = (DMSpyInitArgs*)pEvt;
	do 
	{

		if (!pInitEvt
			||!pInitEvt->m_hwnd)
			//||pInitEvt->m_hwnd == m_hWnd)
		{
			break;
		}
		if (pInitEvt->m_hwnd == m_hRootWnd&&m_bInit)
		{
			break;
		}
		m_pTreeCtrl->RemoveAllItems();
		DM_KillTimer(10);
		m_bDraw = false;
		m_bInit = false;
		m_pTreeEdit->SetWindowText(L"");
		m_pSearchSta->DV_SetWindowText(L"请拖动左侧的指针到需要查看的窗口/控件上释放");
		// 默认wparam传什么返回什么
		DWORD_PTR lresult = 0; 
		LRESULT lr  = ::SendMessageTimeout(pInitEvt->m_hwnd,WM_DMSPY,DMSPY_INIT,(LPARAM)m_hWnd,SMTO_NORMAL,2000,&lresult);
		if (lr&&DMSPY_INIT!=lresult)
		{
			break;
		}
		m_hRootWnd = pInitEvt->m_hwnd;
		DMSpyTool::ReleaseSharedMemory();
		DMSpyTool::CreateSharedMemory(sizeof(DMSpyEnum));

		// 初始化以pInitEvt->m_hwnd为root的枚举树形DUI窗口
		InitTreeCtrl(pInitEvt->m_hwnd);

	} while (false);
	return DM_ECODE_OK;
}


DMCode CSpyWnd::OnTreeSelChanged(DMEventArgs *pEvt)
{
	if (m_bHittest)// 来自pt查找，pt查找由它自己绘
	{
		return DM_ECODE_OK;
	}
	DMEventTCSelChangedArgs *pSelEvt = (DMEventTCSelChangedArgs*)pEvt;
	HDMTREEITEM hSelItem = pSelEvt->m_hNewSel;
	if (hSelItem)
	{
		DUIWND hDUIWnd = (DUIWND)m_pTreeCtrl->GetItemData(hSelItem);
		if (hDUIWnd)
		{
			m_pSpyEnum->iCode   = GDW_FIRSTCHILD;
			m_pSpyEnum->hDUIWnd = hDUIWnd;
			DMSpyTool::WriteShareMemory(m_pSpyEnum,sizeof(DMSpyEnum));
			DWORD_PTR lresult = 0; 
			if (0!= ::SendMessageTimeout(m_hRootWnd, WM_DMSPY, DMSPY_INFO, 0,SMTO_NORMAL,2000,&lresult)&&DMSPY_INFO ==lresult)
			{
				ReflashEditInfo();
				m_bDraw = true;
				m_rcDraw = m_pSpyEnum->info.rcWindow;
				::BringWindowToTop(m_hRootWnd);
				DM_SetTimer(10,10);
			}
		}
		else
		{
			m_bDraw = false;
			DM_KillTimer(10);
		}
	}
	return DM_ECODE_OK;
}

void CSpyWnd::InitTreeCtrl(HWND hRootWnd)
{
	do 
	{
		CStringW strRoot;
		strRoot.Format(L"窗口 %08x-(root)",hRootWnd);
		HDMTREEITEM hRootTree = m_pTreeCtrl->InsertItem(strRoot,0,0,(LPARAM)-1);

		// 获得xml列表
		wchar_t szXmlPath[MAX_PATH] = {0};
		DM::GetRootFullPath(L".\\DMSpyTree.xml",szXmlPath,MAX_PATH);
		DMSpyTool::WriteShareMemory(szXmlPath,MAX_PATH);
		HRESULT hr = (HRESULT)::SendMessage(hRootWnd, WM_DMSPY, DMSPY_ENUM, 0);
		if (DMSPY_ENUM != hr)
		{
			break;
		}
		DMXmlDocument doc;
		if (!doc.LoadFromFile(szXmlPath))
		{
			break;
		}

		// 建立树形控件
		DMXmlNode XmlNode = doc.Root();
		if (!XmlNode.IsValid())
		{
			break;
		}
		InsertTreeItem(XmlNode,hRootTree);
		m_pTreeCtrl->Expand(hRootTree,TVE_COLLAPSE);
	} while (false);
	int nCount = m_pTreeCtrl->GetCount();
	if (nCount>1)
	{
		m_bInit = true;
		m_pSearchSta->DV_SetWindowText(L"DUI初始化完成,请拖动左侧的指针查看DUI窗口/控件");
	}
	else
	{
		m_bInit = false;
		m_pSearchSta->DV_SetWindowText(L"请拖动左侧的指针到需要查看的窗口/控件上释放");
	}
}

void CSpyWnd::InsertTreeItem(DMXmlNode &XmlNode,HDMTREEITEM hParentItem)
{
	int duiwnd = XmlNode.AttributeInt("duiwnd");
	int bsee = XmlNode.AttributeInt("bsee");
	int bPanel = XmlNode.AttributeInt("bpanel");
	LPCSTR classname = XmlNode.Attribute("classname");
	LPCSTR name = XmlNode.Attribute("name");
	LPCSTR id = XmlNode.Attribute("id");
	LPCSTR level = XmlNode.Attribute("level");

	CStringA strTree;
	if (1 == bPanel)
	{
		strTree.Format("(panel)窗口level(%s) ID:%d Name:%s 类名:%s",level,duiwnd,name,classname);
	}
	else
	{
		strTree.Format("窗口level(%s) ID:%d Name:%s 类名:%s",level,duiwnd,name,classname);
	}
	int iIcon = bsee?1:0;
	CStringW strTreeW = DMA2W(strTree);
	HDMTREEITEM hChildItem = m_pTreeCtrl->InsertItem(strTreeW,iIcon,iIcon,(LPARAM)duiwnd,hParentItem);
	for (DMXmlNode XmlChildNode = XmlNode.FirstChild(); XmlChildNode.IsValid(); XmlChildNode=XmlChildNode.NextSibling())
	{
		InsertTreeItem(XmlChildNode,hChildItem);
	}
}

void CSpyWnd::ReflashEditInfo()
{
	DMSpyTool::ReadShareMemory(m_pSpyEnum,sizeof(DMSpyEnum));
	CStringW strWnd;
	strWnd.Format(L"窗口区域:left:%d,top:%d,wid:%d,hei:%d\r\n",m_pSpyEnum->info.rcWindow.left,m_pSpyEnum->info.rcWindow.top,m_pSpyEnum->info.rcWindow.Width(),m_pSpyEnum->info.rcWindow.Height());
	CStringW strClient;
	strClient.Format(L"客户区域:left:%d,top:%d,wid:%d,hei:%d\r\n",m_pSpyEnum->info.rcClient.left,m_pSpyEnum->info.rcClient.top,m_pSpyEnum->info.rcClient.Width(),m_pSpyEnum->info.rcClient.Height());
	CStringW strInfo =strWnd + strClient;
	strInfo += L"XML:\r\n";
	strInfo += m_pSpyEnum->info.szXml;

	m_pTreeEdit->SetWindowText(strInfo);
}

bool CSpyWnd::HitTest(RECT &rc)
{
	bool bRet = false;
	do 
	{
		if (!m_bInit||0==m_pTreeCtrl->GetCount()||NULL==m_hRootWnd)
		{
			break;
		}
		POINT pnnt;
		::GetCursorPos(&pnnt);
		::ScreenToClient(m_hRootWnd,&pnnt);
		m_pSpyEnum->hDUIWnd = DMSPY_ENUM_BEGIN;
		m_pSpyEnum->pt = pnnt;
		DMSpyTool::WriteShareMemory(m_pSpyEnum,sizeof(DMSpyEnum));
		DWORD_PTR lresult = 0; 
		HRESULT hr = (HRESULT)::SendMessageTimeout(m_hRootWnd, WM_DMSPY, DMSPY_POINT, 0,SMTO_NORMAL,2000,&lresult);
		DMSpyTool::ReadShareMemory(m_pSpyEnum,sizeof(DMSpyEnum));
		if (hr&&DMSPY_POINT == lresult&&m_pSpyEnum->hNextDUIWnd)
		{
			// 遍历选中tree
			HDMTREEITEM hItem = m_pTreeCtrl->GetRootItem();
			while (hItem)
			{
				DUIWND hDUIWnd = (DUIWND)m_pTreeCtrl->GetItemData(hItem);
				if (m_pSpyEnum->hNextDUIWnd == hDUIWnd)
				{
					m_bHittest = true;
					rc = m_pSpyEnum->info.rcWindow;
					m_pTreeCtrl->SelectItem(hItem, TRUE);
					ReflashEditInfo();
					m_bHittest = false;
					bRet = true;
					break;
				}
				hItem = m_pTreeCtrl->GetNextItem(hItem);
			}
		}

	} while (false);
	return bRet;
}

void CSpyWnd::OnDUITimer(char id)
{
	DMHWnd::OnDUITimer(id);
	if (10 == id&&m_bDraw)
	{
		static int ncount = 40;
		ncount--;
		if (ncount!=1)
		{
			return;
		}
		HWND DeskHwnd = ::GetDesktopWindow();//取得桌面句柄
		HDC DeskDC    = ::GetWindowDC(DeskHwnd);
		int oldRop2   = SetROP2(DeskDC, R2_NOTXORPEN);
		RECT rc = m_rcDraw;
		HPEN newPen = ::CreatePen(PS_DASHDOTDOT, 1, RGB(125, 0, 125));
		HGDIOBJ oldPen = ::SelectObject(DeskDC, newPen);
		::Rectangle(DeskDC, rc.left, rc.top,rc.right, rc.bottom);
		Sleep(200); //设置闪烁时间间隔
		::Rectangle(DeskDC, rc.left, rc.top,rc.right, rc.bottom);

		::SetROP2(DeskDC, oldRop2);
		::SelectObject(DeskDC, oldPen);
		::DeleteObject(newPen);
		::ReleaseDC(DeskHwnd, DeskDC);
		DeskDC = NULL;
		ncount = 40;
	}
}
