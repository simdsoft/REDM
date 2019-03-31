#include "DMDesignerAfx.h"
#include "XmlEditor.h"
#include <olectl.h>

// +-符号
static const char *minus_xpm[] =
{	"     9     9       16            1",
"` c #8c96ac",". c #c4d0da","# c #daecf4","a c #ccdeec","b c #eceef4","c c #e0e5eb","d c #a7b7c7","e c #e4ecf0","f c #d0d8e2","g c #b7c5d4","h c #fafdfc","i c #b4becc",
"j c #d1e6f1","k c #e4f2fb","l c #ecf6fc","m c #d4dfe7","hbc.i.cbh","bffeheffb","mfllkllfm","gjkkkkkji","da`````jd","ga#j##jai","f.k##k#.a","#..jkj..#","hemgdgc#h"};

static const char *plus_xpm[] = 
{	"     9     9       16            1",
"` c #242e44",". c #8ea0b5","# c #b7d5e4","a c #dcf2fc","b c #a2b8c8","c c #ccd2dc","d c #b8c6d4","e c #f4f4f4","f c #accadc","g c #798fa4","h c #a4b0c0","i c #cde5f0",
"j c #bcdeec","k c #ecf1f6","l c #acbccc","m c #fcfefc","mech.hcem","eldikille","dlaa`akld",".#ii`ii#.","g#`````fg",".fjj`jjf.","lbji`ijbd","khb#idlhk","mkd.ghdkm"};

XmlEditor::XmlEditor()
{
	m_DirectFunction = NULL;
	m_DirectPointer  = NULL;
}

BEGIN_MSG_MAP(XmlEditor)
	MSG_WM_DESTROY(OnDestroy)
	MESSAGE_HANDLER_EX(WM_IME_COMPOSITION, OnImeChar)
END_MSG_MAP()

LRESULT XmlEditor::OnImeChar(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void XmlEditor::Destroy()
{
	UnsubclassWindow(TRUE);
	DestroyWindow();
}

LRESULT XmlEditor::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (OCM__BASE+WM_NOTIFY == uMsg)
	{ 
		SCNotification* notify = (SCNotification*)lParam; 
		if(notify->nmhdr.code == SCN_MARGINCLICK)
		{ 
			// 确定是页边点击事件 
			const int line_number = (int)Call(SCI_LINEFROMPOSITION,notify->position); 
			Call(SCI_TOGGLEFOLD, line_number); 
		} 
		static int LastProcessedChar = 0; 
		if(notify->nmhdr.code == SCN_CHARADDED) 
		{ 
			LastProcessedChar = notify->ch; 
		} 
		if(notify->nmhdr.code == SCN_UPDATEUI)
		{
			DMBufT<char> pBuf; pBuf.Allocate(500);
			int i = GetCurLine(500,pBuf.get());
			//LOG_USER("SCN_UPDATEUI-char:%d--pos:%d-line:%d-buf:%s\n",LastProcessedChar,GetCurrentPos(),i,pBuf.get());
			LastProcessedChar = 0; 
		}
	} 

	return __super::DefWindowProc(uMsg,wParam,lParam);
}

///
HWND XmlEditor::Create(HWND hWndParent)
{
	HWND hWnd = ::CreateWindowEx(NULL, L"Scintilla", L"",WS_CHILD,0,0,0,0, hWndParent,NULL,GetModuleHandle(NULL),NULL);
	if (hWnd)
	{
		SubclassWindow(hWnd);
		m_DirectFunction = SendMessage(SCI_GETDIRECTFUNCTION, 0, 0);
		m_DirectPointer = SendMessage(SCI_GETDIRECTPOINTER, 0, 0);
		InitXml();
	}
	return hWnd;
}

void XmlEditor::InitXml()
{
	// 参考Scintilla开源库使用指南
	Call(SCI_SETCODEPAGE,SC_CP_UTF8);// 使用UTF8

	Call(SCI_STYLESETFORE, STYLE_DEFAULT, RGB(0x0,0X0,0X0));// 文本的前景色，Text is drawn in the foreground colour.
	Call(SCI_STYLESETBACK, STYLE_DEFAULT, RGB(255,255,255));// 背景色，the background colour.

	Call(SCI_STYLESETSIZE, STYLE_DEFAULT, 12);// 字体大小
	Call(SCI_STYLESETFONT, STYLE_DEFAULT, reinterpret_cast<LPARAM>("新宋体"));

	Call(SCI_SETLEXER, SCLEX_XML);// 使用XML语法
	Call(SCI_SETSTYLEBITS, 7);// 字符样式位最大到7位
	Call(SCI_SETTABWIDTH, 4);// 设置tab缩进
	Call(SCI_STYLECLEARALL);

	// 下面设置各种语法元素风格 
	Call(SCI_STYLESETFORE, SCE_C_WORD, 0x00FF0000);// 关键字 
	Call(SCI_STYLESETFORE, SCE_C_WORD2, 0x00800080);// 关键字 
	Call(SCI_STYLESETBOLD, SCE_C_WORD2, TRUE); // 关键字 

	// 仿notepad++的xml 颜色
	Call(SCI_STYLESETFORE, SCE_C_STRING, RGB(128,0,255)); // 字符串 如key="value",value的颜色
	Call(SCI_STYLESETFORE, SCE_C_CHARACTER, RGB(128,0,255)); // 字符 如key='value',value的颜色
	Call(SCI_STYLESETFORE, SCE_C_COMMENT, RGB(43,43,224));// 块注释，如<xml> xml的颜色
	Call(SCI_STYLESETFORE, SCE_C_COMMENTDOC, RGB(255,0,0));//文档注释,如key="value",key的颜色
	Call(SCI_STYLESETFORE,SCE_C_PREPROCESSOR, RGB(0,128,0));//文档注释,如<!---->
	Call(SCI_STYLESETFORE,SCE_C_IDENTIFIER, RGB(0,0,255));// 如<xml/>中的/>

	Call(SCI_SETCARETLINEVISIBLE, TRUE); // 当前行高亮
	Call(SCI_SETCARETLINEBACK, RGB(232,232,255)); 

	Call(SCI_AUTOCSETSEPARATOR, static_cast<WPARAM>(10), 0);

	// 设置代码折叠规则
	Call(SCI_SETMARGINTYPEN,1,SC_MARGIN_NUMBER); 
	Call(SCI_SETMARGINTYPEN, 2, SC_MARGIN_SYMBOL);// 页边类型 

	Call(SCI_SETMARGINWIDTHN,1,28); // 页边宽度 
	Call(SCI_SETMARGINWIDTHN,2,16); 

	Call(SCI_SETMARGINSENSITIVEN, 2, TRUE); // 响应鼠标消息 
	Call(SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS); // 页边掩码 

	Call(SCI_SETPROPERTY, (WPARAM)"fold", (LPARAM)"1");
	Call(SCI_SETPROPERTY, (WPARAM)"fold.compact", (LPARAM)"0");
	Call(SCI_SETPROPERTY, (WPARAM)"fold.html", (LPARAM)"1");
	Call(SCI_SETPROPERTY, (WPARAM)"fold.html.preprocessor", (LPARAM)"1");

	// 折叠标签样式 
	Call(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS);  
	Call(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS);  
	Call(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND,  SC_MARK_CIRCLEPLUSCONNECTED); 
	Call(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_CIRCLEMINUSCONNECTED); 
	Call(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE); 
	Call(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);  
	Call(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE); 

	// 折叠标签颜色 
	Call(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0xa0a0a0); 
	Call(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0xa0a0a0); 
	Call(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0xa0a0a0); 

	Call(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDER, (sptr_t)plus_xpm); 
	Call(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPEN, (sptr_t)minus_xpm); 
	Call(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEREND, (sptr_t)plus_xpm); 
	Call(SCI_MARKERDEFINEPIXMAP, SC_MARKNUM_FOLDEROPENMID, (sptr_t)minus_xpm); 

	Call(SCI_SETFOLDFLAGS, 16, 0); //如果折叠就在折叠行的下面画一条横线
}

//
LRESULT XmlEditor::Call(UINT message,WPARAM wParam/*=0*/,LPARAM lParam/*=0*/,bool bDirect/*=true*/)
{
	DMASSERT(::IsWindow(m_hWnd)); //Window must be valid
	if (bDirect)
	{
		DMASSERT(m_DirectFunction); //Direct function must be valid
		return (reinterpret_cast<SciFnDirect>(m_DirectFunction))(m_DirectPointer, message, wParam, lParam);
	}  
	else
	{
		return SendMessage(message, wParam, lParam);
	}
}

LRESULT XmlEditor::AddText(int iLen, LPCSTR lpszText, bool bDirect /*= true*/)
{
	return Call(SCI_ADDTEXT, static_cast<WPARAM>(iLen), reinterpret_cast<LPARAM>(lpszText), bDirect);
}

LRESULT XmlEditor::Cancel(bool bDirect /*= true*/)
{
	return Call(SCI_CANCEL, 0, 0, bDirect);
}

LRESULT XmlEditor::ClearAll(bool bDirect /*= true*/)
{
	return Call(SCI_CLEARALL, 0, 0, bDirect);
}

LRESULT XmlEditor::EmptyUndoBuffer(bool bDirect /*= true*/)
{
	return Call(SCI_EMPTYUNDOBUFFER, 0, 0, bDirect);
}

LRESULT XmlEditor::GotoPos(int iPos, bool bDirect /*= true*/)
{
	return Call(SCI_GOTOPOS, static_cast<WPARAM>(iPos), 0, bDirect);
}

int XmlEditor::GetCurrentPos(bool bDirect /*= true*/)
{
	return (int)Call(SCI_GETCURRENTPOS, 0, 0, bDirect);
}

int XmlEditor::GetCurLine(int iLen, LPCSTR lpszText, bool bDirect /*= true*/)
{
	return (int)Call(SCI_GETCURLINE, static_cast<WPARAM>(iLen), reinterpret_cast<LPARAM>(lpszText), bDirect);
}

LRESULT XmlEditor::SetUndoCollection(bool bCollectUndo, bool bDirect /*= true*/)
{
	return Call(SCI_SETUNDOCOLLECTION, static_cast<WPARAM>(bCollectUndo), 0, bDirect);
}

LRESULT XmlEditor::SetSavePoint(bool bDirect /*= true*/)
{
	return Call(SCI_SETSAVEPOINT, 0, 0, bDirect);
}

bool XmlEditor::GetReadOnly(bool bDirect /*= true*/)
{
	return 0!=Call(SCI_GETREADONLY,0,0,bDirect);
}

LRESULT XmlEditor::SetReadOnly(bool bReadOnly, bool bDirect /*= true*/)
{
	return Call(SCI_SETREADONLY, static_cast<WPARAM>(bReadOnly), 0, bDirect);
}


//----------------------------------------------------------------------------
DUIRealXml::DUIRealXml()
{
	g_pMainWnd->m_EventMgr.SubscribeEvent(DUIXmlUpdateArgs::EventID, Subscriber(&DUIRealXml::OnUpdateXml, this));
	m_hParent = NULL;
}

HWND DUIRealXml::OnCreateRealWnd(LPCWSTR atom,HWND hWndParent)
{
	HWND hWnd = NULL;
	do 
	{
		m_pXmlEditor.Attach(new XmlEditor);
		hWnd = m_pXmlEditor->Create(hWndParent);
		if (NULL == hWnd)
		{
			DM_MessageBox(L"创建xml编辑窗口失败");
			break;
		}
		m_hParent = hWndParent;
	} while (false);
	return hWnd;

}

DMCode DUIRealXml::OnDestoryRealWnd()
{
	m_pObjTree = NULL;// m_pObjTree会先于DUIRealXml释放，所以把它置NULL,从而不会在~AttrBase中调用UnSubscribeEvent
	if (m_pXmlEditor&& m_pXmlEditor->IsWindow())
	{
		m_pXmlEditor->Destroy();
		m_pXmlEditor.Release();
	}
	return DM_ECODE_OK;
}

DMCode DUIRealXml::OnMoveRealWnd(LPRECT lpRect)
{
	CRect rcWnd = lpRect;
	CRect rcOldWnd;
	::GetWindowRect(m_hRealWnd,&rcOldWnd);
	::SetWindowPos(m_hRealWnd,0,rcWnd.left,rcWnd.top,rcWnd.Width(),rcWnd.Height(),SWP_NOZORDER);
	if (rcOldWnd.Height()<rcWnd.Height())
	{// 防止上拉时重影
		UpdateWindow(m_hRealWnd);
	}
	return DM_ECODE_OK;
}

DMCode DUIRealXml::UpdataInfo(HDMTREEITEM hSel,HDMTREEITEM hOldSel)
{
	do 
	{
		ObjTreeDataPtr pData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hSel);
		if (m_pXmlEditor.isNull()||NULL == pData||NULL == pData->m_pDUIWnd||false == pData->IsValid())
		{
			break;
		}

		if (hOldSel)// 判断是否为同一份XML
		{
			ObjTreeDataPtr pOldData = (ObjTreeDataPtr)m_pObjTree->GetItemData(hOldSel);
			if (pOldData&&pOldData->m_pDoc&&pOldData==pData)
			{
				break;
			}
		}

		
		bool bReadOlny = m_pXmlEditor->GetReadOnly(true);
		m_pXmlEditor->SetReadOnly(false);
		m_pXmlEditor->ClearAll();
		m_pXmlEditor->EmptyUndoBuffer();

		CStringW strWXml;
		if (hSel && 0 == m_pObjTree->GetParentItem(hSel))// 切换到最上层了，默认为DMHWnd窗口
		{
			pData->m_pDoc->m_pXmlDoc->Base().GetXmlContent(strWXml);
		}
		else
		{
			pData->m_pXmlNode->GetXmlContent(strWXml);
		}

		//Tell the control not to maintain any undo info while we stream the data 
		m_pXmlEditor->Cancel();
		m_pXmlEditor->SetUndoCollection(false);

		CStringA strAXml = DMW2A(strWXml,CP_UTF8);
		m_pXmlEditor->AddText(strAXml.GetLength(),strAXml.GetBuffer());strAXml.ReleaseBuffer();

		m_pXmlEditor->SetUndoCollection(true);
		m_pXmlEditor->EmptyUndoBuffer();
		m_pXmlEditor->SetSavePoint();
		m_pXmlEditor->GotoPos(0);// 下次addtext 从0开始
		m_pXmlEditor->SetReadOnly(bReadOlny);// 恢复原始状态
	} while (false);
	return DM_ECODE_OK;
}

DMCode DUIRealXml::UpdateLock(HDMTREEITEM hItem)
{
	do 
	{ 
		if (NULL == hItem
			||hItem != m_hObjSel
			||NULL == m_pXmlEditor)
		{
			break;
		}
		DM::LPTVITEMEX pData = m_pObjTree->GetItem(hItem);
		m_pXmlEditor->SetReadOnly(true);// 临时全只读
#if 0
		if (DMTVEXLock_UnLocked == pData->iLockValue)
		{
			m_pXmlEditor->SetReadOnly(false);
		}
		else
		{
			m_pXmlEditor->SetReadOnly(true);
		}
#endif		
	} while (false);
	return DM_ECODE_OK;
}

DMCode DUIRealXml::OnUpdateXml(DMEventArgs* pEvt)
{
	DUIXmlUpdateArgs *pEvent = (DUIXmlUpdateArgs*)pEvt;
	if (pEvent)
	{
		switch (pEvent->m_UpdateType)
		{
		case DUIXmlUpdateArgs::XMLUPDATE_DELPROP:
		case DUIXmlUpdateArgs::XMLUPDATE_ADDPROP:
		case DUIXmlUpdateArgs::XMLUPDATE_CHANGEPROP:
			{
				UpdataInfo(m_hObjSel, NULL);
			}
			break;

		default:
			break;
		}
	}
	return DM_ECODE_OK;
}



