#include "QQDemoAfx.h"
#include "WidgetWnd.h"
#include "DUIMenu.h"
#include "DUIListCtrlEx.h"

//
#include "TabCtrl_Test.h"
#include "ListBox_Test.h"
#include "ComboBox_Test.h"
#include "Gif_Test.h"
#include "IE_Test.h"

BEGIN_MSG_MAP(CWidgetWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SIZE(OnSize)
	MSG_WM_COMMAND(OnCommand)
	CHAIN_MSG_MAP(DMHWnd)
END_MSG_MAP()

BEGIN_EVENT_MAP(CWidgetWnd)
	EVENT_NAME_COMMAND(L"btn_close",OnClose)
	EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
	EVENT_ID_COMMAND(10, OnRestore)
	EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
	EVENT_NAME_COMMAND(L"btn_menu",OnBtnMenu)
	EVENT_NAME_COMMAND(L"btn_attrtest",OnAttrTest)

	// 控件测试
	EVENT_NAME_HANDLER(L"btn_tabctrl_insert",DMEVT_CMD,TabCtrl_Test::OnInsert);
	EVENT_NAME_HANDLER(L"btn_tabctrl_del",DMEVT_CMD,TabCtrl_Test::OnDelete);
	EVENT_NAME_HANDLER(L"tabmain", DMEVT_TAB_SELCHANGING, TabCtrl_Test::OnSelChanging)
	EVENT_NAME_HANDLER(L"btn_listbox_insert",DMEVT_CMD,ListBox_Test::OnInsert);
	EVENT_NAME_HANDLER(L"btn_listbox_del",DMEVT_CMD,ListBox_Test::OnDelete);
	EVENT_NAME_HANDLER(L"btn_combobox_sel",DMEVT_CMD,ComboBox_Test::OnSel);

	EVENT_NAME_HANDLER(L"btn_gif_open",DMEVT_CMD,Gif_Test::OnOpenGif);
	EVENT_NAME_HANDLER(L"btn_gif_replay",DMEVT_CMD,Gif_Test::OnReplayGif);

	EVENT_NAME_COMMAND(L"webback",OnBtnWebBack)
	EVENT_NAME_COMMAND(L"webrefresh",OnBtnWebRefresh)
	EVENT_NAME_COMMAND(L"webfront",OnBtnWebfront)

	EVENT_NAME_COMMAND(L"ieback",IE_Test::OnBtnIeBack)
	EVENT_NAME_COMMAND(L"ierefresh",IE_Test::OnBtnIeRefresh)
	EVENT_NAME_COMMAND(L"iefront",IE_Test::OnBtnIefront)
	EVENT_NAME_COMMAND(L"iejstest",IE_Test::OnBtnIeJsTest)

	EVENT_NAME_HANDLER(L"richedit1",DMEVT_RENOTIFY,OnEditChange)
	EVENT_NAME_HANDLER(L"richedit1",DMEVT_RLBUTTONMENU,OnEditRButtonMenu)
END_EVENT_MAP()
CWidgetWnd::CWidgetWnd()
{
	m_pWebkit     = NULL;
}

BOOL CWidgetWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// 加下richedit的变字体大小，颜色，下划线, 字符通知事件,示例
	DUIRichEdit* pEdit = FindChildByNameT<DUIRichEdit>(L"richedit1");
	if (pEdit)
	{
		CHARFORMAT2W cf;
		ZeroMemory(&cf, sizeof(CHARFORMAT2W));
		cf.cbSize = sizeof(CHARFORMAT2W);
		cf.dwMask = CFM_COLOR|CFM_FACE|CFM_SIZE|CFM_UNDERLINE|CFM_UNDERLINETYPE|CFM_LINK;
		cf.dwEffects = CFE_LINK|CFE_UNDERLINE;  
		cf.yHeight = 500;
		cf.crTextColor = RGB(255, 0, 0); 
		pEdit->SetSel(MAKELONG(2,4));//选取2-4字符
		pEdit->DM_SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf);// 设置带下划线超链接

		cf.dwMask = CFM_COLOR|CFM_FACE|CFM_SIZE;
		cf.yHeight = 100;
		pEdit->SetSel(MAKELONG(5,8));//选取5-8字符
		cf.crTextColor = RGB(255, 255, 0); 
		pEdit->DM_SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf);

		// 字符变化通知
		pEdit->SetEventMask(ENM_CHANGE|pEdit->GetEventMask());
	}

	// 增加ListCtrlEx排序示例
	DUIListCtrlEx *pList = FindChildByNameT<DUIListCtrlEx>(L"listctrlex");
	if (pList)
	{
		pList->m_pHeaderCtrl->m_EventMgr.SubscribeEvent(DM::DMEventHeaderClickArgs::EventID, Subscriber(&CWidgetWnd::ListCtrlExHeaderClick, this));
	}
	

	m_pWebkit = FindChildByNameT<DUIWebKit>(L"webobj");
	DUIRichEdit *pRichEdit = FindChildByNameT<DUIRichEdit>(L"web_url");
	if (pRichEdit)
	{
		pRichEdit->SetWindowText(L"http://ka.duowan.com/");
		pRichEdit->m_EventMgr.SubscribeEvent(DM::DMEventREWantReturnArgs::EventID, Subscriber(&CWidgetWnd::RefreshWeb, this));
	}

	if (m_pWebkit)
	{
		m_handler.onTitleChanged = CWidgetWnd::OnTitleChanged;
		m_handler.onURLChanged = CWidgetWnd::OnURLChanged;
		m_handler.pdata = this;
		m_pWebkit->GetWebView()->setClientHandler(&m_handler);
		//http://www.baidu.com/s?ie=utf-8&f=8&wd=素材
		//http://www.baidu.com/s?ie=utf-8&f=8&wd=%E7%B4%A0%E6%9D%90
		//CStringA szUrl = "http://www.baidu.com/s?ie=utf-8&f=8&wd=素材";
		m_pWebkit->GetWebView()->loadURL(L"http://ka.duowan.com/");
	}

	IERealWnd * pIEWnd = FindChildByNameT<IERealWnd>(L"ieobj");
	if (pIEWnd)
	{
		IDUIWeb* pIE = pIEWnd->m_pIEWnd->FindChildByNameT<IDUIWeb>(L"ie");
		if (pIE)
		{
			pIE->SetEvtHandler(this);
		}
	}

	DUIRichEdit* pRichEditIE = FindChildByNameT<DUIRichEdit>(L"ieurl");
	if (pRichEditIE)
	{
		pRichEditIE->m_EventMgr.SubscribeEvent(DM::DMEventREWantReturnArgs::EventID, Subscriber(&IE_Test::IERefreshWeb));
	}

	m_ScriptHelper.InitScriptHelper(this);

	return TRUE;
}

void CWidgetWnd::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);  // 由DMHWnd继续处理
	if (0 == size.cx||0 == size.cy)
	{
		return;
	}
	if (SIZE_MAXIMIZED == nType)
	{
		DUIWindow*pMaxWnd     = FindChildByName(L"btn_max");
		DUIWindow*pRestoreWnd = FindChildById(10);
		if (pMaxWnd&&pRestoreWnd)
		{
			pMaxWnd->DM_SetVisible(false);
			pRestoreWnd->DM_SetVisible(true);
		}
	}
	else if (SIZE_RESTORED == nType)
	{
		DUIWindow*pMaxWnd     = FindChildByName(L"btn_max");
		DUIWindow*pRestoreWnd = FindChildById(10);
		if (pMaxWnd&&pRestoreWnd)
		{
			pMaxWnd->DM_SetVisible(true);
			pRestoreWnd->DM_SetVisible(false);
		}
	}
}

void CWidgetWnd::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (3335 == nID)
	{
		OnClose();
	}
	if (33361 == nID)
	{
		g_pDMApp->SetTrans(L"");
		g_pDMApp->RedrawAll();
	}
	if (33362 == nID)
	{
		g_pDMApp->SetTrans(L"dmlg_fh");
		g_pDMApp->RedrawAll();
	}
	if (33363 == nID)
	{
		g_pDMApp->SetTrans(L"dmlg_en");
		g_pDMApp->RedrawAll();
	}
}

//
DMCode CWidgetWnd::OnClose()
{
	if (m_pAttrTestWnd&&m_pAttrTestWnd->IsWindow())
	{
		m_pAttrTestWnd->DestroyWindow();
	}
	DM_AnimateWindow(400,AW_SLIDE|AW_HOR_POSITIVE|AW_HIDE);
	DestroyWindow();
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	SetActiveWindow();
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::OnRestore()
{
	SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	SetActiveWindow();
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::OnBtnMenu()
{
	DUIMenu Menu;
	Menu.LoadMenu(L"LayOut",L"dui_menu");
	POINT pt;
	GetCursorPos(&pt);
	Menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::OnAttrTest()
{
	if (m_pAttrTestWnd&&m_pAttrTestWnd->IsWindow())
	{
		return DM_ECODE_OK;
	}
	m_pAttrTestWnd.Release();
	m_pAttrTestWnd.Attach(new CAttributeTestWnd(this));
	m_pAttrTestWnd->DM_CreateWindow(L"dui_attrtest",0,0,0,0,m_hWnd);				// 创建主窗口
	m_pAttrTestWnd->SendMessage(WM_INITDIALOG);
	m_pAttrTestWnd->CenterWindow();
	m_pAttrTestWnd->ShowWindow(SW_SHOW);
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::OnEditChange(DMEventArgs *pEvt)
{
	DMEventRENotifyArgs *pEvent = (DMEventRENotifyArgs*)(pEvt);
	if (EN_CHANGE == pEvent->m_iNotify)
	{
		LOG_USER("richedit1 字符变化了\n");
	}
	return DM_ECODE_OK;
}

void SortItems(DUIListCtrlEx* pList, int nCol)
{
	if (1 == nCol// 只有点击1列头时才触发(0列开始)
		&&pList->GetItemCount()>=2)
	{
		// 简单的示例,交换m_DMArray[0]和m_DMArray[1]
		// 你也可以使用qsort_s函数来实现自己的快速排序
		// 最终目的是调整m_DMArray中各项的顺序
		LPLCITEMEX  pItem = pList->m_DMArray[0]; //等同于pList->GetObj(0);
		pList->m_DMArray.RemoveAt(0);
		pList->m_DMArray.InsertAt(1,pItem);

		// 最后要更新下panel索引和刷新一下
		pList->UpdateItemPanelId();
		pList->DM_Invalidate();
	}
}

DMCode CWidgetWnd::OnEditRButtonMenu(DMEventArgs *pEvt)
{ 
	DMEventRButtonMenuCmdArgs *pRMenuEvt = (DMEventRButtonMenuCmdArgs*)pEvt;
	if (pRMenuEvt)
	{
		DUIMenu Menu;
		Menu.LoadMenu(L"LayOut",L"dui_editmenu");

		DUIRichEdit *pEdit = dynamic_cast<DUIRichEdit*> (pRMenuEvt->m_pSender);
		if (pEdit)
		{
			BOOL bCanPaste = (BOOL)pEdit->DM_SendMessage(EM_CANPASTE);// wParam:Specifies the clipboard format to try. Set this parameter to zero to try any format currently on the clipboard
			int nStartChar=0,nEndChar=0;
			pEdit->DM_SendMessage(EM_GETSEL,(WPARAM)&nStartChar,(LPARAM)&nEndChar);// 参看CEdit::GetSel
			BOOL bCopy    = nStartChar<nEndChar;
			BOOL bCut	  = (bCopy && !pEdit->GetReadOnly());
			BOOL bCanUndo = (BOOL)pEdit->DM_SendMessage(EM_CANUNDO);
			BOOL bSelAll  = pEdit->GetWindowTextLength()>0;

			// 这里要注意一点,step会占用pos的,dui_editmenu只演示了五个菜单项
			EnableMenuItem(Menu.m_hMenu,0,MF_BYPOSITION|(bCut?MF_ENABLED:MF_GRAYED));
			EnableMenuItem(Menu.m_hMenu,1,MF_BYPOSITION|(bCopy?MF_ENABLED:MF_GRAYED));
			EnableMenuItem(Menu.m_hMenu,2,MF_BYPOSITION|(bCanPaste?MF_ENABLED:MF_GRAYED));
			EnableMenuItem(Menu.m_hMenu,3,MF_BYPOSITION|(bCanUndo?MF_ENABLED:MF_GRAYED));
			EnableMenuItem(Menu.m_hMenu,4,MF_BYPOSITION|(bCut?MF_ENABLED:MF_GRAYED));
			EnableMenuItem(Menu.m_hMenu,5,MF_BYPOSITION|(bSelAll?MF_ENABLED:MF_GRAYED));

		}     
		POINT pt;      
		GetCursorPos(&pt);
		int iRet = Menu.TrackPopupMenu(0,pt.x,pt.y,m_hWnd);
		switch (iRet)
		{
		case 8000:
			{
				pEdit->DM_SendMessage(WM_CUT);
			}
			break;
		case 8001:
			{
				pEdit->DM_SendMessage(WM_COPY);
			}
			break;
		case 8002:
			{
				pEdit->DM_SendMessage(WM_PASTE);
			}
			break;
		case 8003:
			{
				pEdit->DM_SendMessage(EM_UNDO);
			}
			break;
		case 8004:
			{
				pEdit->DM_SendMessage(EM_REPLACESEL,TRUE,(LPARAM)TEXT(""));///< CEdit::ReplaceSel,WPARAM设置为true,支持undo
			}
			break;
		case 8005:
			{
				pEdit->DM_SendMessage(EM_SETSEL,0,-1);
			}
			break;

		default:
			break;
		}
	}

	return DM_ECODE_OK;
}

DMCode CWidgetWnd::ListCtrlExHeaderClick(DMEventArgs* pEvt)
{
	DMEventHeaderClickArgs *pEvtCheck = (DMEventHeaderClickArgs*)pEvt;
	DUIHeaderCtrl *pHeader = (DUIHeaderCtrl*)pEvtCheck->m_pSender;
	if (pHeader)
	{
		DUIListCtrlEx *pList = (DUIListCtrlEx*)pHeader->DM_GetWindow(GDW_PARENT);
		DMHDITEM hditem;
		hditem.mask = DMHDI_ORDER;
		pHeader->GetItem(pEvtCheck->m_iItem,&hditem);
		SortItems(pList,hditem.iOrder);// iOrder为初始header的列头序号，从0开始
	}

	return DM_ECODE_OK;
}

//--------webkit
DMCode CWidgetWnd::OnBtnWebBack()
{
	if (m_pWebkit)
	{
		m_pWebkit->GetWebView()->goBack();
	}
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::OnBtnWebRefresh()
{
	if (m_pWebkit)
	{
		m_pWebkit->GetWebView()->reload();
	}
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::OnBtnWebfront()
{
	if (m_pWebkit)
	{
		m_pWebkit->GetWebView()->goForward();
	}
	return DM_ECODE_OK;
}

DMCode CWidgetWnd::RefreshWeb(DMEventArgs* pEvent)
{
	DUIRichEdit *pRichEdit = FindChildByNameT<DUIRichEdit>(L"web_url");
	if (pRichEdit&&m_pWebkit)
	{
		CStringW szUrl = pRichEdit->GetWindowText();
		if (!szUrl.IsEmpty())
		{
			m_pWebkit->GetWebView()->loadURL(szUrl);
		}
		szUrl.ReleaseBuffer();
	}

	return DM_ECODE_OK;
}

void CWidgetWnd::OnTitleChanged(const DMClientHandler*, DMString title)
{
}

void CWidgetWnd::OnURLChanged(const DMClientHandler*handler, DMString url)
{
	CWidgetWnd *pWnd = (CWidgetWnd *)handler->pdata;
	DUIRichEdit *pRichEdit = pWnd->FindChildByNameT<DUIRichEdit>(L"web_url");
	DUIWebKit *pWebKit = pWnd->FindChildByNameT<DUIWebKit>(L"webobj");
	if (pRichEdit&&pWebKit)
	{
		pRichEdit->SetWindowText(pWebKit->GetWebView()->toStringW(url));
	}
}

// ie
HRESULT CWidgetWnd::NavigateComplete2(DUIWND hWnd,DMIN IDispatch *pDisp,DMIN wchar_t *pUrl)
{
	DUIRichEdit *pRichEdit = FindChildByNameT<DUIRichEdit>(L"ieurl");
	if (pRichEdit)
	{
		pRichEdit->SetWindowText(pUrl);
	}
	return S_OK;
}
 
DMCode CWidgetWnd::OnFireEvent(DMEventArgs &Evt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		if (m_ScriptHelper.m_pScript)
		{
			if (Evt.GetEventID()>= DMEVT_SCRIPT_START && Evt.GetEventID() <= DMEVT_SCRIPT_END)
			{
				iErr = m_ScriptHelper.m_pScript->RunEvent(&Evt);
			}
		}
		if (DMSUCCEEDED(iErr))
		{
			break;
		}

		iErr = __super::OnFireEvent(Evt);
	} while (false);
	return iErr;
}

void CWidgetWnd::OnDUITimer(char id)
{
	if (id == 123)
	{
		m_ScriptHelper.RunScriptTimer();
	}
	__super::OnDUITimer(id);
}
