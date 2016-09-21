#include "DMDesignerAfx.h"
#include "AttrExpandWnd.h"

BEGIN_MSG_MAP(AttrExpandWnd)      
	MSG_WM_INITDIALOG(OnInitDialog) 
	MSG_WM_SIZE(OnSize)
	MSG_WM_DESTROY(OnDestroy)
	MSG_WM_SHOWWINDOW(OnShowWindow)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP() 

AttrExpandWnd::AttrExpandWnd(IAttrExpandOwner* pOwner)
{
	m_pOwner = pOwner;
	m_pAttrTree = NULL;
}

AttrExpandWnd::~AttrExpandWnd()
{
	m_pOwner = NULL;
	m_pAttrTree = NULL;
}

BOOL AttrExpandWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_pAttrTree = FindChildByNameT<AttrTree>(L"ds_attrtree");DMASSERT(m_pAttrTree);
	m_pAttrTree->m_EventMgr.SubscribeEvent(DM::DMEventTCDbClickArgs::EventID, Subscriber(&AttrExpandWnd::OnTreeDbChick, this));
	
	return TRUE;
}

void AttrExpandWnd::OnSize(UINT nType, CSize size)
{  
	if (!IsIconic()) 
	{
		CRect rcWnd;
		::GetWindowRect(m_hWnd, &rcWnd);
		::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);  
		HRGN hWndRgn = ::CreateRoundRectRgn(0, 0, rcWnd.right, rcWnd.bottom,4,4);
		SetWindowRgn(hWndRgn, TRUE);
		::DeleteObject(hWndRgn); 
	}                 
	SetMsgHandled(FALSE);
}

void AttrExpandWnd::OnDestroy()
{
	if (m_pAttrTree)
	{
		m_pAttrTree->m_EventMgr.UnSubscribeEvent(DM::DMEventTCDbClickArgs::EventID, Subscriber(&AttrExpandWnd::OnTreeDbChick, this));
	}
	SetMsgHandled(FALSE);
}

DMCode AttrExpandWnd::InitTreeNode(DMXmlNode& TreeNode,bool IsData)
{
	TreeNode = TreeNode.InsertChildNode(XML_TREEITEM);
	TreeNode.SetAttribute(XML_SKIN,IsData? XML_DATASKIN:XML_CUSTOMSKIN);
	TreeNode.SetAttribute(XML_CLRTEXT,L"pbgra(ff,ff,ff,ff)");
	TreeNode.SetAttribute(XML_FONT,L"face:新宋体,size:12,weight:100");
	TreeNode.SetAttribute(XML_CHILDOFFSET,L"0");
	return DM_ECODE_OK;
}

DMCode AttrExpandWnd::Show()
{
	m_pOwner->OnTreeInit(m_pAttrTree);
	ShowWindow(SW_SHOW);
	DM_Invalidate();
	return DM_ECODE_OK;
}

DMCode AttrExpandWnd::Hide()
{
	m_pOwner->OnTreeEnd(m_pAttrTree);
	DM_Invalidate();
	ShowWindow(SW_HIDE);
	return DM_ECODE_OK;
}

DMCode AttrExpandWnd::OnTreeDbChick(DMEventArgs *pEvt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		DMEventTCDbClickArgs *pEvent = (DMEventTCDbClickArgs*)pEvt;
		DMXmlAttributePtr pAttr = (DMXmlAttributePtr)m_pAttrTree->GetItemData(pEvent->m_hItem);
		if (NULL == pAttr)
		{
			break;
		}
		m_pOwner->OnTreeSel(m_pAttrTree);
		Hide();

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}


