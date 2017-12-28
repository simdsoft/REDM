#include "DMDesignerAfx.h"
#include "DMDesignerWnd.h"
#include "NewResDlg.h"

DesignMenu g_GlbMenuItem[] = \
{
	{GLBMENU_NEW,			  L" 新建 ctrl+n"},//0 
	{GLBMENU_SAVE,			  L" 保存 ctrl+s"},//1 
	{GLBMENU_CLOSE,			  L" 关闭"},//2 
	{GLBMENU_EXIT,		      L" 退出"},//3 

	{GLBMENU_OPTOBJPROJ,	  L" 切换对象视图 ctrl+q"},//4 
	{GLBMENU_RELOAD,		  L" 重新加载     f5"},//5 

	{GLBMENU_MOVEMODE,		  L" Move模式 ctrl+1"},//6
	{GLBMENU_SELMODE,		  L" Sel模式  ctrl+2"},//7
	{GLBMENU_ADDMODE,		  L" Add模式  ctrl+3"},//8

	{GLBMENU_HELPDOC,		  L" 帮助文档"},//9
	{GLBMENU_ONLINEHELP,	  L" 在线教程"},//10
};
enum
{
	GLBMENUBTN_ID_MIN          = 100,
	GLBMENUBTN_ID_FILE,							///< 文件
	GLBMENUBTN_ID_VIEW,							///< 视图
	GLBMENUBTN_ID_MODE,							///< 模式
	GLBMENUBTN_ID_HELP,							///< 帮助
	//
	GLBMENUBTN_ID_MAX		   = 110
};

DMDesignerWnd* g_pMainWnd = NULL;
DMSmartPtrT<ResMultFolder> g_pRes;
BEGIN_MSG_MAP(DMDesignerWnd)    
	CHAIN_MSG_STATIC_MAP(AddXml,s_ProcessWindowMessage)
	CHAIN_MSG_STATIC_MAP(RightXml,s_ProcessWindowMessage)
	MSG_WM_INITDIALOG(OnInitDialog) 
	MSG_WM_SIZE(OnSize)
	MSG_WM_DESTROY(OnDestroy)
	MSG_WM_LBUTTONDOWN(OnLButtonDown)
	MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
	MSG_WM_DROPFILES(OnDropFiles)
	MSG_WM_COMMAND(OnCommand)
	MSG_WM_TIMER(OnTimer) 
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
	REFLECT_NOTIFICATIONS_EX()
END_MSG_MAP() 
BEGIN_EVENT_MAP(DMDesignerWnd)
	EVENT_NAME_COMMAND(L"ds_closebutton",	OnClose)
	EVENT_NAME_COMMAND(L"ds_maxbutton",		OnMaximize)
	EVENT_NAME_COMMAND(L"ds_restorebutton", OnRestore)
	EVENT_NAME_COMMAND(L"ds_minbutton",		OnMinimize)
	EVENT_NAME_HANDLER(L"ds_preview",DMEVT_CMD,OnPreview)
	EVENT_ID_COMMAND_RANGE(GLBMENUBTN_ID_MIN,GLBMENUBTN_ID_MAX,OnGlobalMenuBtn)	
	CHAIN_EVENT_STATIC_MAP(RightXml,s_DMHandleEvent)
	CHAIN_EVENT_STATIC_MAP(AddXml,s_DMHandleEvent)
END_EVENT_MAP()

CStringW g_Tips[] = \
{
	L"F5可以快速重新加载工程视图/对象视图",
	L"Ctrl+N可以建立一个新资源",
	L"Ctrl+S可以保存资源",
	L"Ctrl+Q在建立对象视图和销毁对象视图间切换",
	L"在工程/对象视图中点击右键可以快速定位到相关文件",
	L"在对象视图中通过锁定来保证控件不被改变",
	L"在对象视图中右键复制/粘贴可以快速复制/粘贴控件",
	L"在对象视图中选中支持Panel可以开启Panel支持",
	L"在SEL模式下，显示成灰色的DOT不可拖动",
	L"在SEL模式下，左/右击均可快速选中控件",
	L"使用DELETE快捷键可以删除PROP的项",
	L"在工程视图中，未绑定XML的结点为灰色",
	L"在对象视图中，ItemPanel项显示成红色",
	L"CTRL+1可以快速切换到MOVE模式",
	L"CTRL+2可以快速切换到SEL模式"
	L"CTRL+3可以快速切换到Add模式"
};
#define  TIMER_TIPS                                  110
DMDesignerWnd::DMDesignerWnd()
{
	g_pMainWnd = this;
	g_pDMApp->GetDefRegObj((void**)&g_pRes, DMREG_Res);
	m_pDesignerXml = NULL;
}

DMDesignerWnd::~DMDesignerWnd()
{
	m_pDesignerXml = NULL;
}

// 消息分发
BOOL DMDesignerWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	DragAcceptFiles(TRUE);

	DWORD accel = DUIAccel::TranslateAccelKey(L"ctrl+n");
	DUIAccel acc(accel);
	GetAccelMgr()->RegisterAccel(acc,this);
	accel = DUIAccel::TranslateAccelKey(L"ctrl+s");
	
	DUIAccel accsave(accel);
	GetAccelMgr()->RegisterAccel(accsave,this);

	accel = DUIAccel::TranslateAccelKey(L"ctrl+q");
	DUIAccel accobjtree(accel);
	GetAccelMgr()->RegisterAccel(accobjtree,this);

	CStringW strAcc;
	for (int i=1;i<=4;i++)
	{
		strAcc.Format(L"ctrl+%d",i);
		accel = DUIAccel::TranslateAccelKey(strAcc);
		DUIAccel acctemp(accel);
		GetAccelMgr()->RegisterAccel(acctemp,this);
		
	}

	accel = DUIAccel::TranslateAccelKey(L"f5");
	DUIAccel accf5(accel);
	GetAccelMgr()->RegisterAccel(accf5,this);

	SetTimer(TIMER_TIPS,4000);
	return TRUE;  
}

void DMDesignerWnd::OnLButtonDown(UINT nFlags,CPoint pt)
{
	SetFocus();
	SetMsgHandled(FALSE);
}

void DMDesignerWnd::OnLButtonDbClick(UINT nFlags,CPoint pt)
{
	do 
	{	
		SetMsgHandled(FALSE);
		DUIWND hDUIWnd = 0;
		if (pt.y>=40)
		{
			break;
		}

		DUIWindow*pMaxWnd     = FindChildByName(L"ds_maxbutton");
		DUIWindow*pRestoreWnd = FindChildByName(L"ds_restorebutton");
		if (pMaxWnd->DM_IsVisible())
		{
			OnMaximize();
		}
		else
		{
			OnRestore();
		}	

	} while (false);
}

void DMDesignerWnd::OnSize(UINT nType, CSize size)
{
	DUIWindow*pMaxWnd     = FindChildByName(L"ds_maxbutton");
	DUIWindow*pRestoreWnd = FindChildByName(L"ds_restorebutton");
	if (!IsIconic())  
	{
		CRect rcWnd;
		::GetWindowRect(m_hWnd, &rcWnd);
		::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);  
		HRGN hWndRgn = ::CreateRoundRectRgn(0, 0, rcWnd.right, rcWnd.bottom,4,4);
		SetWindowRgn(hWndRgn, TRUE);
		::DeleteObject(hWndRgn);  
	}                        
	if (SIZE_MAXIMIZED == nType&&pMaxWnd&&pRestoreWnd) 
	{  
		pMaxWnd->DM_SetVisible(false);
		pRestoreWnd->DM_SetVisible(true);
	}                 
	else if (SIZE_RESTORED == nType&&pMaxWnd&&pRestoreWnd)
	{
		pMaxWnd->DM_SetVisible(true);
		pRestoreWnd->DM_SetVisible(false);
	} 
	SetMsgHandled(FALSE);
}

void DMDesignerWnd::OnDestroy()
{
	DM_DELETE(m_pDesignerXml);
	SetMsgHandled(FALSE);
}

void DMDesignerWnd::OnDropFiles(HDROP hDropInfo)
{    
	do 
	{
		int nDropCount = DragQueryFile(hDropInfo, -1, NULL, 0);
		if (1!=nDropCount)
		{
			break;
		} 

		wchar_t szPath[MAX_PATH] = {0};
		DragQueryFile(hDropInfo, 0,(LPWSTR)szPath,MAX_PATH);

		// 判断是否为目录
		if (IsDirectoryExist(szPath))
		{
			OnParseRes(szPath);
		}
	} while (false);
}

void DMDesignerWnd::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (nID>=GLBMENU_BASE)
	{
		HandleGlobalMenu(nID);
	}
	else if (nID>=OBJMENU_OPENDIR)
	{
		m_pDesignerXml->HandleObjTreeMenu(nID);
	}
	else if (nID>=PROJMENU_OPENDIR)
	{
		m_pDesignerXml->HandleProjTreeMenu(nID);
	}
}

void DMDesignerWnd::OnTimer(UINT_PTR idEvent)
{
	SetMsgHandled(FALSE);
	do 
	{
		if (TIMER_TIPS != idEvent)
		{
			break;
		}
		static int i = 0;
		int iTipsNum = sizeof(g_Tips)/sizeof(g_Tips[0]);
		if (i>=iTipsNum)
		{
			i = 0;
		}
		FindChildByNameT<DUIStatic>(L"ds_tips")->SetAttribute(XML_CLRTEXT,L"pbgra(ff,ff,00,ff)");
		FindChildByNameT<DUIStatic>(L"ds_tips")->DV_SetWindowText(g_Tips[i++]);
	} while (false);
}

// 事件分发
DMCode DMDesignerWnd::OnClose()
{
	if (m_pDesignerXml)
	{
		if (m_pDesignerXml->IsNeedSave())
		{
			m_pDesignerXml->SaveRes(false);
		}
	}

	DestroyWindow();
	return DM_ECODE_OK;
}		

DMCode DMDesignerWnd::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	SetActiveWindow();
	return DM_ECODE_OK;
}

DMCode DMDesignerWnd::OnRestore()
{
	SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	SetActiveWindow();
	return DM_ECODE_OK;
} 

DMCode DMDesignerWnd::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	return DM_ECODE_OK;
}

DMCode DMDesignerWnd::OnPreview(DMEventArgs *pEvt)
{
	if (m_pDesignerXml)
	{
		m_pDesignerXml->ShowPreview();
	}
	return DM_ECODE_OK;
}

bool DMDesignerWnd::OnAccelPressed(const DUIAccel& Accel)
{
	DUIAccel acc = Accel;
	CStringW str = acc.FormatHotkey();
	if (0 == str.CompareNoCase(L"CTRL+N"))
	{
		OpenNewProj();
	}
	else if (0 == str.CompareNoCase(L"CTRL+S"))
	{
		if (m_pDesignerXml)
		{
			m_pDesignerXml->SaveRes();
		}
	}
	else if (0 == str.CompareNoCase(L"CTRL+Q"))
	{
		OptionObjProj();
	}
	else if (0 == str.CompareNoCase(L"CTRL+1")
			||0 == str.CompareNoCase(L"CTRL+2")
			||0 == str.CompareNoCase(L"CTRL+3")
			)
	{
		int iSel = 0;
		swscanf_s((LPCWSTR)str,L"Ctrl+%d",&iSel);
		FindChildByNameT<DUITabCtrl>(L"ds_tool")->SetCurSel(iSel-1);
	}
	else if (0 == str.CompareNoCase(L"f5"))
	{
		ReloadProj();
	}

	return true;
}

DMCode DMDesignerWnd::OnParseRes(CStringW strResDir)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		CloseProj();
		m_pDesignerXml = new ObjXml;
		m_pDesignerXml->ParseRes(strResDir);
		iErr = m_pDesignerXml->InitProjTree();
		if (!DMSUCCEEDED(iErr))
		{
			DM_Invalidate();
			break;
		}
		FindChildByNameT<DUIStatic>(L"ds_resdirsta")->DV_SetWindowText(strResDir);
		g_pAttr->AddRecentResDir(strResDir);
	} while (false);
	if (!DMSUCCEEDED(iErr))
	{
		DM_DELETE(m_pDesignerXml);
	}
	return iErr;
}

DMCode DMDesignerWnd::OnGlobalMenuBtn(int idFrom)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DUIWindow* pCur = FindChildById(idFrom);
		if (NULL == pCur)
		{
			break;
		}
		
		DMXmlDocument Doc;
		g_pDMApp->InitDMXmlDocument(Doc, XML_LAYOUT,L"ds_menu_proj");
		DMXmlNode XmlNode = Doc.Root();
		XmlNode.SetAttributeInt(XML_BSHADOW,1);
		XmlNode.SetAttribute(L"clrtext",L"pbgra(ff,ff,00,ff)");
		InitFileMenu(XmlNode,idFrom);
		InitViewMenu(XmlNode,idFrom);
		InitModeMenu(XmlNode,idFrom);
		InitHelpMenu(XmlNode,idFrom);

		DUIMenu Menu;
		Menu.LoadMenu(XmlNode); 
		CRect rcButton = pCur->m_rcWindow;
		ClientToScreen(rcButton);
		CPoint pt(rcButton.left,rcButton.bottom);
		Menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode DMDesignerWnd::HandleGlobalMenu(int nID)
{
	switch (nID)
	{
	case GLBMENU_NEW:
		{
			OpenNewProj();
		}
		break;

	case GLBMENU_SAVE:
		{
			if (m_pDesignerXml)
			{
				m_pDesignerXml->SaveRes();
			}
		}
		break;

	case GLBMENU_CLOSE:
		{
			CloseProj();
		}
		break;

	case GLBMENU_EXIT:
		{
			OnClose();
		}
		break;

	case GLBMENU_OPTOBJPROJ:
		{
			OptionObjProj();
		}
		break;

	case GLBMENU_RELOAD:
		{
			ReloadProj();
		}
		break;

	case GLBMENU_MOVEMODE:
	case GLBMENU_SELMODE:
	case GLBMENU_ADDMODE:
		{
			int iSel = nID-GLBMENU_MOVEMODE;
			FindChildByNameT<DUITabCtrl>(L"ds_tool")->SetCurSel(iSel);
		}
		break;

	case GLBMENU_HELPDOC:
		{
			wchar_t szDocPath[MAX_PATH]={0};
			DM::GetRootFullPath(L".\\DesignerRes\\DMDesign-Help.docx",szDocPath,MAX_PATH);
			if (::PathFileExists(szDocPath))
			{
				::ShellExecuteW(NULL,L"open",szDocPath,NULL,NULL,SW_SHOWNORMAL);
			}
		}
		break;

	case GLBMENU_ONLINEHELP:
		{
			SHELLEXECUTEINFOW shelli = { 0 };
			shelli.cbSize = sizeof(SHELLEXECUTEINFOW);
			shelli.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
			shelli.lpVerb = L"open";
			shelli.lpFile = L"http://hgy413.com/3426.html";
			shelli.nShow = SW_SHOW;
			::ShellExecuteExW(&shelli);
		}
		break;

	default:
		break;
	}
	return DM_ECODE_OK;
}

DMCode DMDesignerWnd::InitFileMenu(DMXmlNode& XmlNode,int idFrom)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (GLBMENUBTN_ID_FILE != idFrom)
		{
			break;
		}
		
		DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_NEW-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_NEW-GLBMENU_BASE].text);
		XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_SAVE-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_SAVE-GLBMENU_BASE].text);XmlItem.SetAttributeInt(XML_BDISABLE,m_pDesignerXml?0:1);
		XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_CLOSE-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_CLOSE-GLBMENU_BASE].text);XmlItem.SetAttributeInt(XML_BDISABLE,m_pDesignerXml?0:1);
		XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_EXIT-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_EXIT-GLBMENU_BASE].text);
		Init_Debug_XmlBuf(XmlNode);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;

}

DMCode DMDesignerWnd::InitViewMenu(DMXmlNode& XmlNode,int idFrom)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (GLBMENUBTN_ID_VIEW != idFrom)
		{
			break;
		}

		DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_OPTOBJPROJ-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_OPTOBJPROJ-GLBMENU_BASE].text);XmlItem.SetAttributeInt(XML_BDISABLE,m_pDesignerXml?0:1);
		XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_RELOAD-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_RELOAD-GLBMENU_BASE].text);XmlItem.SetAttributeInt(XML_BDISABLE,m_pDesignerXml?0:1);
		Init_Debug_XmlBuf(XmlNode);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode DMDesignerWnd::InitModeMenu(DMXmlNode& XmlNode,int idFrom)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (GLBMENUBTN_ID_MODE != idFrom)
		{
			break;
		}

		DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_MOVEMODE-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_MOVEMODE-GLBMENU_BASE].text);
		XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_SELMODE-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_SELMODE-GLBMENU_BASE].text);
		XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_ADDMODE-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_ADDMODE-GLBMENU_BASE].text);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode DMDesignerWnd::InitHelpMenu(DMXmlNode& XmlNode,int idFrom)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (GLBMENUBTN_ID_HELP != idFrom)
		{
			break;
		}

		DMXmlNode XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_HELPDOC-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_HELPDOC-GLBMENU_BASE].text);
		XmlItem = XmlNode.InsertChildNode(XML_ITEM);
		XmlItem.SetAttribute(XML_ID,IntToString(g_GlbMenuItem[GLBMENU_ONLINEHELP-GLBMENU_BASE].id));XmlItem.SetAttribute(XML_TEXT,g_GlbMenuItem[GLBMENU_ONLINEHELP-GLBMENU_BASE].text);
		Init_Debug_XmlBuf(XmlNode);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode DMDesignerWnd::OpenNewProj()
{
	DMSmartPtrT<NewResDlg> pDlg;
	pDlg.Attach(new NewResDlg());
	if (IDOK == pDlg->DoModal(L"ds_newdlg",m_hWnd,true))
	{
		OnParseRes(pDlg->m_strResDir);
	}
	return DM_ECODE_OK;
}

DMCode DMDesignerWnd::CloseProj()
{
	if (m_pDesignerXml&&m_pDesignerXml->IsNeedSave())
	{
		m_pDesignerXml->SaveRes(false);
	}
	DM_DELETE(m_pDesignerXml);
	FindChildByNameT<DUIStatic>(L"ds_resdirsta")->DV_SetWindowText(L"ctrl+n打开最近列表或拖拽Res文件进来");
	return DM_ECODE_OK;
}

DMCode DMDesignerWnd::OptionObjProj()
{
	if (m_pDesignerXml)
	{
		if (m_pDesignerXml->m_bInitObjTree)
		{
			if (IDOK == DM_MessageBox(L"确认关闭对象视图?\r\n",MB_OKCANCEL))
			{
				m_pDesignerXml->ReleaseObjTree();
			}
		}
		else
		{
			if (IDOK == DM_MessageBox(L"确认加载对象视图?\r\n",MB_OKCANCEL))
			{
				m_pDesignerXml->InitObjTree();
			}
		}
	}
	return DM_ECODE_OK;
}

DMCode DMDesignerWnd::ReloadProj()
{
	if (m_pDesignerXml)
	{
		bool bInitObj = m_pDesignerXml->m_bInitObjTree;
		OnParseRes(m_pDesignerXml->m_strResDir);

		if (bInitObj)
		{
			m_pDesignerXml->InitObjTree();
		}
	}
   return DM_ECODE_OK;
}










