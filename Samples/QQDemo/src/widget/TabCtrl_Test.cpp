#include "QQDemoAfx.h"
#include "TabCtrl_Test.h"
#include "QQMainWnd.h"
extern CQQMainWnd* g_pMainWnd;

int TabCtrl_Test::m_nInsertCount = 0;
DMCode TabCtrl_Test::OnInsert(DMEventArgs *pEvt)
{
	do 
	{
		if (m_nInsertCount>=5)
		{
			break;
		}

		CStringW strBuf;
		strBuf.Format(L"<tabPage itemskin=\"tab_item\" clrbg=\"pbgra(cc,cc,cc,FF)\" animatetype=\"1\" title=\"insert%d\"> <window pos=\"0,0,-0,-0\" text=\"insert%d\"/></tabPage>",m_nInsertCount,m_nInsertCount);
		int ulSize = 200;
		byte pBuf[200] = {0};
		UnicodeToUtf8(strBuf.GetBuffer(),(PCHAR)pBuf,ulSize);
		strBuf.ReleaseBuffer();
		DMXmlDocument doc;
		doc.LoadFromBuffer(pBuf, ulSize);
		DMXmlNode XmlNode = doc.Root();
		if (!XmlNode.IsValid())
		{
			break;
		}
		DUITabCtrl* pTab = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUITabCtrl>(L"tabctrl1");
		if (!pTab)
		{
			break;
		}

#if 0
		// test 内存加载skin测试
		DUIButton * pBtn = g_pMainWnd->FindChildByNameT<DUIButton>(L"closebutton");
		if (pBtn)
		{
			
			DWORD dwSize = GetFileSizeW(L"c:\\1.png");
			DMBufT<byte>pBuf;
			pBuf.Allocate(dwSize);
			DWORD dwRead;
			GetFileBufW(L"c:\\1.png",(void**)&pBuf,dwSize,dwRead);
			wchar_t pWBuf[] = L"<imglist id=\"closebutton\"  states=\"3\" />";
			g_pDMApp->AddSkinFromMemory(pBuf,dwSize,L"png",pWBuf);
			//pBtn->SetAttribute(L"skin",L"123456");
		}
#endif

		// 测试重设DOC
		DMXmlDocument XmlDoc;
		XmlDoc.ResetByXmlNode(XmlNode);
		DMXmlNode Node = XmlDoc.Root();
		pTab->InsertItem(Node,1);

		m_nInsertCount++;
		
	} while (false);
	return DM_ECODE_OK;
}

DMCode TabCtrl_Test::OnDelete(DMEventArgs *pEvt)
{
	do 
	{
		if (0 == m_nInsertCount)
		{
			break;
		}
		
		DUITabCtrl* pTab = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUITabCtrl>(L"tabctrl1");
		if (!pTab)
		{
			break;
		}
		pTab->RemoveItem(1);
		m_nInsertCount--;
	} while (false);
	return DM_ECODE_OK;
}

DMCode TabCtrl_Test::OnSelChanging(DMEventArgs *pEvt)
{
#if 0
	do 
	{
		DMEventTabSelChangingArgs* pEvent = (DMEventTabSelChangingArgs*)pEvt;
		if (5 == pEvent->m_uOldSel)
		{
			// 删除子窗口
			DUITabCtrl* pTab = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUITabCtrl>(L"tabmain");
			if (!pTab)
			{
				break;
			}
			DUITabPage* pSub = pTab->GetItem(5);
			if (!pSub)
			{
				break;
			}

			pSub->DM_DestroyChildWnd(pSub->m_Node.m_pFirstChild);
		}

		if (5 == pEvent->m_uNewSel)
		{
			// 增加子窗口
			DUITabCtrl* pTab = g_pMainWnd->m_pWidgetWnd->FindChildByNameT<DUITabCtrl>(L"tabmain");
			if (!pTab)
			{
				break;
			}
			DUITabPage* pSub = pTab->GetItem(5);
			if (!pSub)
			{
				break;
			}
			DUIWindow *pChild = NULL;
			g_pDMApp->CreateRegObj((void**)&pChild, L"flash",DMREG_Window);
			if (pChild)
			{
				pSub->DM_InsertChild(pChild);
				pChild->SetAttribute(L"pos", L"%30,%30,%70,%70");
				pChild->SetAttribute(L"url", L"flash:1.swf");
			}
		}
	} while (false);
#endif
	return DM_ECODE_OK;
}