#include "DmMainAfx.h"
#include "DMSpyTool.h"

namespace DM
{
	DMSpyTool::DMSpyTool()
	{
		m_hMainDUIWnd = -1;
		m_hSpyWnd     = NULL;
		m_pSpyEnum    = NULL;
	}
	DMSpyTool::~DMSpyTool()
	{
		DM_DELETE(m_pSpyEnum);
	}

	LRESULT DMSpyTool::OnSpy(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lr = 0;
		switch (wParam)
		{
		case DMSPY_INIT:// 准备spy的信息
			{
				lr = DMSPY_INIT;
				m_hSpyWnd = (HWND)lParam;
				if (NULL == m_hSpyWnd||!::IsWindow(m_hSpyWnd))
				{
					m_hSpyWnd = NULL;
					lr = 0;// 错误！
				}
				if (!m_pSpyEnum)
				{
					m_pSpyEnum = new DMSpyEnum;
				}
				m_PanelParseMap.RemoveAll();
				memset(m_pSpyEnum,0,sizeof(DMSpyEnum));
			}
			break;
		case DMSPY_ENUM:
			{
				lr = DMSPY_ENUM;
				wchar_t szXmlPath[MAX_PATH] = {0};
				DMSpyTool::ReadShareMemory(szXmlPath,MAX_PATH);

				// 开始构建XMLTree.xml
				EnumDUITree(szXmlPath);
			}
			break;
		case DMSPY_INFO:
			{
				lr = DMSPY_INFO;
				memset(m_pSpyEnum,0,sizeof(DMSpyEnum));
				if (false == ReadShareMemory(m_pSpyEnum,sizeof(DMSpyEnum)))
				{
					lr = 0;
					break;
				}
				if (DMSPY_ENUM_BEGIN == m_pSpyEnum->hDUIWnd)
				{
					m_pSpyEnum->hDUIWnd = m_hMainDUIWnd;
				}	
				DUIWindow* pDUIWnd = g_pDMDWndPool->FindDUIWnd(m_pSpyEnum->hDUIWnd);
				if (!pDUIWnd)
				{
					lr = 0;
					break;
				}
				EnumInfo(pDUIWnd,m_pSpyEnum);
				WriteShareMemory(m_pSpyEnum,sizeof(DMSpyEnum));
			}
			break;

		case DMSPY_POINT:
			{
				lr = DMSPY_POINT;
				memset(m_pSpyEnum,0,sizeof(DMSpyEnum));
				if (false == ReadShareMemory(m_pSpyEnum,sizeof(DMSpyEnum)))
				{
					lr = 0;
					break;
				}
				if (DMSPY_ENUM_BEGIN == m_pSpyEnum->hDUIWnd)
				{
					m_pSpyEnum->hDUIWnd = m_hMainDUIWnd;
				}	
				DUIWindow* pDUIWnd = g_pDMDWndPool->FindDUIWnd(m_pSpyEnum->hDUIWnd);
				if (!pDUIWnd)
				{
					lr = 0;
					break;
				}

				m_pSpyEnum->hNextDUIWnd = pDUIWnd->HitTestPoint(m_pSpyEnum->pt,true);
				DUIWindow* pFind = g_pDMDWndPool->FindDUIWnd(m_pSpyEnum->hNextDUIWnd);
				if (pFind)
				{
					EnumInfo(pFind,m_pSpyEnum);
				}
				WriteShareMemory(m_pSpyEnum,sizeof(DMSpyEnum));
			}
			break;
		}
		return lr;
	}
	
	void DMSpyTool::EnumDUITree(LPCWSTR lpszPath)
	{
		DMXmlDocument doc;
		m_XmlBase = doc.Base();
		DUIWindow* pWnd = g_pDMDWndPool->FindDUIWnd(m_hMainDUIWnd);
		int iLevel = 0;
		EnumTreeItem(pWnd,m_XmlBase,iLevel,false); // 插入结点
		Init_Debug_XmlBuf(m_XmlBase);
		doc.SaveXml(lpszPath);
	}

	void DMSpyTool::EnumTreeItem(DUIWindow* pWnd,DMXmlNode XmlNode,int iLevel,bool bPanel)
	{
		do 
		{
			Init_Debug_XmlBuf(m_XmlBase);
			int nL = iLevel;
			if (NULL == pWnd)
			{
				break;
			}

			// 插入子结点
			CStringW bsee;bsee.Format(L"%d",pWnd->DM_IsVisible(true));
			CStringW id;id.Format(L"%d",pWnd->GetID());
			CStringW name = pWnd->GetName();
			CStringW classname = pWnd->V_GetClassName();
			DMXmlNode ChildNode = XmlNode.InsertChildNode(L"N",NULL);

			CStringW duiwnd;duiwnd.Format(L"%d",pWnd->GetDUIWnd());
			ChildNode.SetAttribute(L"duiwnd",duiwnd);
			CStringW nl;nl.Format(L"%d",nL);
			ChildNode.SetAttribute(L"level",nl);
			ChildNode.SetAttribute(L"bpanel",bPanel?L"1":L"0");

			ChildNode.SetAttribute(L"classname", classname);
			ChildNode.SetAttribute(L"bsee",bsee);
			ChildNode.SetAttribute(L"id", id);
			ChildNode.SetAttribute(L"name", name);

			// 子结点递归
			DUIWindow* pChild = pWnd->m_Node.m_pFirstChild;
			while (pChild)
			{
				int tempLevel = nL+1;
				EnumTreeItem(pChild,ChildNode,tempLevel,false);// 进入下一层
				pChild = pChild->m_Node.m_pNextSibling;
			}

			// 子panel结点递归
			int tempLevel = nL+1;
			EnumPanelTreeItem(pWnd,ChildNode,tempLevel);
			Init_Debug_XmlBuf(m_XmlBase);
		} while (false);
	}

	void DMSpyTool::EnumPanelTreeItem(DUIWindow* pWnd,DMXmlNode XmlNode,int iLevel)
	{
		do 
		{
			int nL = iLevel;
			Init_Debug_XmlBuf(m_XmlBase);
			if (true == m_PanelParseMap[pWnd->GetDUIWnd()])// 理论上我们不需要关心这个
			{
				DMASSERT_EXPR(false, L"难道算法错了！");
				break;
			}
			m_PanelParseMap[pWnd->GetDUIWnd()] = true;//强加个标记,用于验证算法的正确性下次此窗口不再遍历panel
			if (NULL == pWnd)
			{
				break;
			}

			int count = (int)pWnd->m_ChildPanelArray.GetCount();
			for (int i=0; i<count; i++)
			{
				DUIWindowPtr pPanel = pWnd->m_ChildPanelArray[i];
				if (pPanel)
				{
					int tempLevel = nL;
					EnumTreeItem(pPanel,XmlNode,tempLevel,true);
				}
			}
		} while (false);
	}

	void DMSpyTool::EnumInfo(DUIWindow*pWnd,PDMSpyEnum m_pSpyEnum)
	{
		m_pSpyEnum->info.bVisible = pWnd->DM_IsVisible(true);
		m_pSpyEnum->info.hDUIWnd  = pWnd->GetDUIWnd();
		m_pSpyEnum->info.iId      = pWnd->GetID();
		m_pSpyEnum->info.rcWindow = pWnd->m_rcWindow;
		pWnd->GetContainer()->OnClientToScreen(m_pSpyEnum->info.rcWindow);
		pWnd->DV_GetClientRect(m_pSpyEnum->info.rcClient);

		CStringW strName = pWnd->GetName();
		memset(m_pSpyEnum->info.szName, 0, DMSPY_NAME_LEN*2);
		if (strName.GetLength()<DMSPY_NAME_LEN)
		{
			wcscpy_s(m_pSpyEnum->info.szName,strName);
		}
		else
		{
			wcscpy_s(m_pSpyEnum->info.szName,L"[名字过长,无法显示!]");
		}

		CStringW strClassName = pWnd->V_GetClassName();
		if (strClassName.GetLength()<DMSPY_CLASSNAME_LEN)
		{
			wcscpy_s(m_pSpyEnum->info.szClassName,strClassName);
		}
		else
		{
			wcscpy_s(m_pSpyEnum->info.szClassName,L"[类名过长,无法显示!]");
		}

		memset(m_pSpyEnum->info.szXml, 0, DMSPY_XML_LEN*2);
#ifdef _DEBUG
		if (pWnd->m_strXml.GetLength()<DMSPY_XML_LEN)
		{
			wcscpy_s(m_pSpyEnum->info.szXml,pWnd->m_strXml);
		}
		else
		{
			wcscpy_s(m_pSpyEnum->info.szXml,L"[XML过长,无法显示!]");
		}
#endif 
	}

	//---------------------------------------------------------
	bool DMSpyTool::CreateSharedMemory(UINT nSize)
	{/// spy使用，就不做已存在判断了
		bool bRet = false;
		HANDLE hSize = NULL;
		HANDLE hFileMapping = NULL;
		do 
		{
			// 创建一块内存存放共享内存的大小  
			hSize = CreateFileMapping(NULL, NULL, PAGE_READWRITE, 0, sizeof(UINT),DMSPY_SHAREMEMORYSIZE);  
			if ((hSize == NULL) || (hSize == INVALID_HANDLE_VALUE)) 
			{
				DMASSERT_EXPR(0,L"创建DMSPY_SHAREMEMORYSIZE共享内存失败");
				break;
			}
			UINT *pSize = (UINT *)MapViewOfFile(hSize, FILE_MAP_WRITE, 0, 0, sizeof(UINT)); 
			memcpy(pSize, &nSize, sizeof(UINT));  
			UnmapViewOfFile(pSize);  

			// 创建共享内存块 
			HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSize, DMSPY_SHAREMEMORY);
			if ((hFileMapping == NULL) || (hFileMapping == INVALID_HANDLE_VALUE))
			{
				DMASSERT_EXPR(0,L"创建DMSPY_SHAREMEMORY共享内存失败");
				break;
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMSpyTool::GetShareMemorySize(UINT& nSize)
	{
		bool bRet = false;
		HANDLE hFileMapping = NULL;
		do 
		{
			hFileMapping = OpenFileMappingW(FILE_MAP_READ, FALSE, DMSPY_SHAREMEMORYSIZE);
			if (NULL == hFileMapping)
			{
				break;
			}
			void *pMapView = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, nSize); 
			if (NULL == pMapView)
			{
				break;
			}
			memcpy(&nSize,pMapView,sizeof(UINT));  
			UnmapViewOfFile(pMapView);  
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMSpyTool::WriteShareMemory(void *pDate, UINT nSize)
	{
		bool bRet = false;
		HANDLE hFileMapping = NULL;
		do 
		{
			UINT nSharedMemorySize = 0;
			if (!GetShareMemorySize(nSharedMemorySize))
			{
				break;
			}
			if (nSharedMemorySize<nSize)
			{
				break;
			}

			hFileMapping = OpenFileMappingW(FILE_MAP_WRITE, FALSE, DMSPY_SHAREMEMORY);
			if (NULL == hFileMapping)
			{
				break;
			}
			void *pMapView = MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, nSize); 
			if (NULL == pMapView)
			{
				break;
			}
			memset(pMapView, 0, nSize);
			memcpy(pMapView, pDate, nSize);  
			UnmapViewOfFile(pMapView);  
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMSpyTool::ReadShareMemory(void *pDate, UINT nSize)
	{
		bool bRet = false;
		HANDLE hFileMapping = NULL;
		do 
		{
			hFileMapping = OpenFileMappingW(FILE_MAP_READ, FALSE, DMSPY_SHAREMEMORY);
			if (NULL == hFileMapping)
			{
				break;
			}
			void *pMapView = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, nSize); 
			if (NULL == pMapView)
			{
				break;
			}
			memcpy(pDate,pMapView,nSize);  
			UnmapViewOfFile(pMapView);  
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMSpyTool::ReleaseSharedMemory()
	{
		HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, DMSPY_SHAREMEMORY);
		DM_CLOSEHANDLE(hFileMapping);
		HANDLE hSize = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, DMSPY_SHAREMEMORYSIZE);
		DM_CLOSEHANDLE(hSize);
		return true;
	}
}//namespace DM