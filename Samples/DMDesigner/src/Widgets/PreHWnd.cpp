#include "DMDesignerAfx.h"
#include "PreHWnd.h"

DMXmlDocument* GetPreSubXmlDoc(LPCWSTR lpszType,LPCWSTR lpszResName)
{
	DMXmlDocument* pXmlDoc = NULL;
	do 
	{
		if (!g_pMainWnd->m_pDesignerXml)
		{
			break; 
		}
		if (NULL==lpszType||NULL==lpszResName)
		{
			break;
		}
		DMSmartPtrT<ResFolder>pRes = g_pMainWnd->m_pDesignerXml->m_pRes;
		CStringW strPath = pRes->GetItemPath(lpszType,lpszResName,L"");
		DocDataPtr pData = g_pMainWnd->m_pDesignerXml->FindDocData(strPath);
		if (pData&&pData->IsValid())
		{
			pXmlDoc = pData->m_pXmlDoc;
		}
	} while (false);
	return pXmlDoc;
}

BEGIN_MSG_MAP(PreHWnd)  
	MSG_WM_KEYDOWN(OnKeyDown)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP() 
HWND PreHWnd::ObjCreateWindow(DMXmlDocument &XmlDoc)
{
	do 
	{
		// 窗口已创建
		if (m_hWnd)
		{
			break;
		}

		ATOM Atom = g_pDMApp->GetClassAtom();  // 是否使用阴影窗口类创建
		// 创建时会先调用到OnNcCreate来解析XML资源，如OnNcCreate返回失败,则创建失败，m_hWnd为NULL
		DMCWnd::CreateWindowEx((LPCWSTR)Atom,DM_DEF_WINDOW_NAME, DM_DEF_STYLE, 0, 0, 0, 0, 0, g_pMainWnd->m_hWnd, NULL);
		if (NULL == m_hWnd)
		{
			break;
		}

		// 解析xml
		g_pDMApp->SetSubXmlDocCallBack(GetPreSubXmlDoc);
		LoadDMData(XmlDoc);
		g_pDMApp->SetSubXmlDocCallBack(NULL);
	} while (false);
	if (m_hWnd)
	{
		// 开始布局
		InitFromDMData();			

		// 注册可拖动
		::RegisterDragDrop(m_hWnd, &m_DropTarget);
	}

	return m_hWnd;
}

DMCode PreHWnd::LoadDMData(DMXmlDocument &XmlDoc)
{
	DMCode iErr = DM_ECODE_OK;
	do 
	{
		DMXmlNode XmlNode = XmlDoc.Root(MAINWND_NODE);
		if (!XmlNode.IsValid())
		{
			break;
		}

		m_pHWndXmlInfo->ResetXmlInfo();                // 重设置XML数据

		// 解析私有Skin节点,外部可以释放它---------------
		DMXmlNode XmlSkin = XmlNode.FirstChild(L"skin");
		while (XmlSkin.IsValid())
		{
			g_pDMApp->AddSkinPoolItem(XmlSkin);
			XmlSkin = XmlSkin.NextSibling(L"skin");
		}

		// 解析私有Style节点,外部可以释放它--------------
		DMXmlNode XmlStyle = XmlNode.FirstChild(L"style");
		while (XmlStyle.IsValid())
		{
			g_pDMApp->AddStylePoolItem(XmlStyle);
			XmlStyle = XmlStyle.NextSibling(L"style");
		}

		// 解析自身的XML-Attribute-----------------------
		DMBase::InitDMData(XmlNode);

		// 循环解析DUI的XML -----------------------------
		// root节点本身也是一个DUIWindow窗口对象，但是在这里必须是"root"才能识别，在这个节点中可以有DUIWindow的各种属性，但是和布局位置相关的属性自动无效，因为该窗口总是充满整个宿主窗口。
		DUIWindow::InitDMData(XmlNode.FirstChild(DUIROOT_NODE));
	} while (false);
	return iErr;
}

void PreHWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	do 
	{
		DMHWnd::OnKeyEvent(WM_KEYDOWN,(WPARAM)nChar,MAKELPARAM(nRepCnt,nFlags));
		if (VK_ESCAPE!=nChar)
		{
			break;
		}
		ShowWindow(SW_HIDE);
	} while (false);
}
