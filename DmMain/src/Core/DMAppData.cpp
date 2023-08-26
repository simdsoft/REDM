﻿#include "DmMainAfx.h"
#include "DMAppData.h"
#include "DMCWnd.h"

#ifndef USER_DEFAULT_SCREEN_DPI
#define USER_DEFAULT_SCREEN_DPI 96
#endif

namespace DM
{
	extern 	fun_cbGetSubXmlDoc  g_pGetSubXmlDoc;
	DMAppData::DMAppData(HINSTANCE hInst/* = GetModuleHandle(NULL)*/)
	{
		// 加载自身内部插件.内部插件的Install函数会做模块（对应filter:Modules）初始化操作
		// 加载插件一般在最前面，这时如果有log插件加载，也会在最前面
		m_pPlugin    = new DMPluginImpl;
		InstallPlugin(m_pPlugin);

		m_hInst		 = hInst;
		CStringW strAtom;strAtom.Format(L"%s_%d_%d",DM_DEF_CLASS_NAME,::GetTickCount(),::GetCurrentThreadId());
		CStringW strShadowAtom;strShadowAtom.Format(L"%s_%d_%d",DM_DEF_SHADOWCLASS_NAME,::GetTickCount(),::GetCurrentThreadId());
		m_Atom		 = RegisterClassEx(m_hInst, strAtom.GetBuffer());strAtom.ReleaseBuffer();
		m_shadowAtom = RegisterClassEx(m_hInst, strShadowAtom.GetBuffer(), true);strShadowAtom.ReleaseBuffer();
		m_pTaskRunnerObj->InitEvent(::GetCurrentThreadId());// 在ATOM建立后才才能创建UI消息窗口

		// 全局变量初始化
		m_fun_UpdateLayeredWindowIndirect = NULL;
		HMODULE hMod = GetModuleHandleW(L"user32");
		if (hMod)
		{
			m_fun_UpdateLayeredWindowIndirect =
				(fun_UpdateLayeredWindowIndirect)GetProcAddress(hMod,"UpdateLayeredWindowIndirect");
		}

		// DWM
		m_hModuleDWM = LoadLibraryW(L"dwmapi");
		if (m_hModuleDWM) {
			m_fun_DwmIsCompositionEnabled = (fun_DwmIsCompositionEnabled)GetProcAddress(m_hModuleDWM, "DwmIsCompositionEnabled");
			m_fun_DwmSetWindowAttribute = (fun_DwmSetWindowAttribute)GetProcAddress(m_hModuleDWM, "DwmSetWindowAttribute");
			m_fun_DwmExtendFrameIntoClientArea = (fun_DwmExtendFrameIntoClientArea)GetProcAddress(m_hModuleDWM, "DwmExtendFrameIntoClientArea");
		}

		// DPI scale
		UINT xdpi, ydpi;

		const HDC dc = GetDC(NULL);
		xdpi = GetDeviceCaps(dc, LOGPIXELSX);
		ydpi = GetDeviceCaps(dc, LOGPIXELSY);
		ReleaseDC(NULL, dc);

		m_dpiScale = xdpi / (float)USER_DEFAULT_SCREEN_DPI;
	}

	DMAppData::~DMAppData()
	{	
		if (m_hModuleDWM) FreeLibrary(m_hModuleDWM);

		// 先关闭LOG
		DMLogDispatch::SetLogDispatch(NULL);

		// 卸载自身内部插件.
		m_pTaskRunnerObj->UninstallEvent();
		UninstallPlugin(m_pPlugin);
		DM_DELETE(m_pPlugin);
	}

	DMCode DMAppData::InitGlobal(LPCSTR lpszXmlId)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (m_pResObj.isNull())
			{
				break;
			}

			DMXmlDocument doc;
			if (!DMSUCCEEDED(InitDMXmlDocument(doc,RES_GLOBAL,lpszXmlId)))
			{
				break;
			}

			DMXmlNode XmlNode = doc.Root(DMAttr::DMGlobalAttr::XMLNODE_global);
			if (!XmlNode.IsValid())
			{
				break;
			}

			// 设置默认文本---------------------------
			CStringA strFont = XmlNode.Attribute(DMAttr::DMGlobalAttr::XMLATTR_font);
			if (!strFont.IsEmpty())
			{
				m_FontPool->SetDefaultFont(strFont);
			}

			// 设置全局Skin---------------------------
			DMXmlNode XmlSkin = XmlNode.FirstChild(DMAttr::DMGlobalAttr::XMLNODE_skin);
			while (XmlSkin.IsValid())
			{
				m_SkinPool->AddSkinPoolItem(XmlSkin);
				XmlSkin = XmlSkin.NextSibling(DMAttr::DMGlobalAttr::XMLNODE_skin);
			}

			// 设置全局Style--------------------------
			DMXmlNode XmlStyle = XmlNode.FirstChild(DMAttr::DMGlobalAttr::XMLNODE_style);
			while (XmlStyle.IsValid())
			{
				m_StylePool->AddStylePoolItem(XmlStyle);
				XmlStyle = XmlStyle.NextSibling(DMAttr::DMGlobalAttr::XMLNODE_style);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMAppData::Run(HWND hWnd)
	{
		LOG_INFO("[start]\n");
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMMsgLoop* pMsgLoop = m_MsgLoopTool->GetMessageLoop();
			if (NULL == pMsgLoop)// 当前线程没有Loop，则创建Loop
			{
				DMMsgLoop *pThisLoop = new DMMsgLoop;
				if (!m_MsgLoopTool->AddMessageLoop(pThisLoop))
				{
					iErr = DMAPP_RUN_ADDMESSAGELOOP_FAIL;
					break;
				}
				pThisLoop->SetRefCount(1);// 初始Loop总是为1
				pMsgLoop = pThisLoop;
			}
			else
			{
				pMsgLoop->AddRef(); // 引用计数+1
			}

			// HWND hLastMainWnd = ::GetActiveWindow();
			// if (::IsWindow(hWnd))
			// {
			// 	if (!(::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW))
			// 	{// 有WS_EX_TOOLWINDOW属性的不强制设置激活
			// 		hLastMainWnd = ::SetActiveWindow(hWnd);
			// 	}
			// }
			size_t index = m_RunhWndArray.Add(hWnd);// 用于DestroyWindow中判断是否发送WM_QUIT消息 
			pMsgLoop->Run();
			if (index<m_RunhWndArray.GetCount() && hWnd == m_RunhWndArray[index])
			{
				m_RunhWndArray.RemoveAt(index);
			}

			if (pMsgLoop->GetRefCount()>1)
			{
				pMsgLoop->Release();
			}
			else if (1==pMsgLoop->GetRefCount())
			{// 当前线程的最后一个窗口在使用Loop了
				m_MsgLoopTool->RemoveMessageLoop();
				pMsgLoop->Release();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		LOG_INFO("[end]iErr:%d\n",iErr);
		return iErr;
	}

	DMCode DMAppData::IsRun(HWND hWnd)
	{
		DMCode iErr = DM_ECODE_FAIL;
		int iCount = (int)m_RunhWndArray.GetCount();
		for (int i=0;i<iCount; i++)
		{
			if (hWnd == m_RunhWndArray[i])
			{
				iErr = DM_ECODE_OK;
				break;
			}
		}
		return iErr;
	}

	/// 注册类-----------------------------------------
	DMCode DMAppData::Register(const IDMReg &RegObj, bool bReplace/*=false*/)
	{
		return m_RegMgr->Register(RegObj, bReplace);
	}

	DMCode DMAppData::CreateRegObj(void** ppObj, LPCSTR lpszClassName,int RegType)
	{
		return m_RegMgr->CreateRegObj(ppObj, lpszClassName, RegType);
	}

	DMCode DMAppData::UnRegister(LPCSTR lpszClassName,int RegType)
	{
		return m_RegMgr->UnRegister(lpszClassName, RegType);
	}

	DMCode DMAppData::SetDefRegObj(LPCSTR lpszClassName,int RegType)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			iErr = m_RegMgr->SetDefRegObj(lpszClassName, RegType);
			if (!DMSUCCEEDED(iErr))
			{
				break;
			}

			switch (RegType)
			{
			case DMREG_Attribute:
				{
					m_pAttributeObj.Release();// 先释放
					iErr = CreateRegObj((void**)&m_pAttributeObj, lpszClassName, RegType);
					if (DMSUCCEEDED(iErr))
					{
						DMAttributeDispatch::SetAttributeDispatch(m_pAttributeObj);
					}		
				}
				break;
			case DMREG_Log:
				{
					DMLogDispatch::SetLogDispatch(NULL);// 设置为NULL，因为在接下来的CreateRegObj中可能会有LOG
					m_pLogObj.Release();// 先释放
					iErr = CreateRegObj((void**)&m_pLogObj, lpszClassName, RegType);
					if (DMSUCCEEDED(iErr))
					{
						DMLogDispatch::SetLogDispatch(m_pLogObj);
					}		
				}
				break;
			case DMREG_Res:
				{
					m_pResObj.Release();// 先释放
					iErr = CreateRegObj((void**)&m_pResObj, lpszClassName, DMREG_Res);
				}
				break;
			case DMREG_Render:
				{
					m_pRenderObj.Release();// 先释放
					iErr = CreateRegObj((void**)&m_pRenderObj, lpszClassName, DMREG_Render);
				}
				break;
			case DMREG_Trans:
				{
					m_pTransObj.Release();// 先释放
					iErr = CreateRegObj((void**)&m_pTransObj, lpszClassName, DMREG_Trans);
				}
				break;
			case DMREG_TaskRunner:
				{
					m_pTaskRunnerObj.Release();// 先释放
					iErr = CreateRegObj((void**)&m_pTaskRunnerObj, lpszClassName, DMREG_TaskRunner);
				}
				break;
			case DMREG_Draw:
			case DMREG_Layout:
			case DMREG_ImgDecoder:
			case DMREG_Style:
			case DMREG_ToolTip:
			case DMREG_Script:
				{
					// 它需要指定默认类型,但不是唯一
				}
				break;
			case DMREG_Window:
			case DMREG_FlowLayout:
			case DMREG_Animate:
			case DMREG_Skin:
				{
					// 它其实不需要指定默认类型
				}	
				break;
			default:
				{
					DMFAIL_MSG("default object RegType not allowed"/*0,L"不支持设置默认对象的RegType"*/);
				}
				break;
			}
		} while (false);
		return iErr;
	}

	DMCode DMAppData::GetDefRegObj(void** ppObj,int RegType)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (NULL == ppObj)
			{
				iErr = DM_ECODE_FAIL;
				break;
			}
			switch (RegType)
			{
			case DMREG_Attribute:*ppObj = m_pAttributeObj; m_pAttributeObj->AddRef(); break;
			case DMREG_Log:      *ppObj = m_pLogObj;       m_pLogObj->AddRef();		  break;
			case DMREG_Res:      *ppObj = m_pResObj;       m_pResObj->AddRef();		  break;
			case DMREG_Render:   *ppObj = m_pRenderObj;	   m_pRenderObj->AddRef();    break;
			case DMREG_Trans:    *ppObj = m_pTransObj;     m_pTransObj->AddRef();     break;
			case DMREG_TaskRunner:*ppObj = m_pTaskRunnerObj;m_pTaskRunnerObj->AddRef();break;
			default:
				{
				DMFAIL_MSG_FMT("Unsupported RegType %d", RegType/*0,L"仅支持取得DMREG_Attribute、DMREG_Log、DMREG_Res、DMREG_Render、DMREG_Trans、DMREG_TaskRunner的RegType"*/);
				}
				break;
			}
		} while (false);
		return iErr;
	}

	DMCode DMAppData::GetDefRegObj(CStringA &szName,int RegType)
	{
		return m_RegMgr->GetDefRegObj(szName, RegType);
	}

	void DMAppData::AddCustomImageLoader(DWORD fourccID, DM_CUSTOM_IMAGELOADER loaderFn)
	{
		m_customImageLoaders.AddKey(fourccID, loaderFn);
	}
	DM_CUSTOM_IMAGELOADER DMAppData::GetCustomImageLoader(DWORD fourccID)
	{
		DM_CUSTOM_IMAGELOADER loader = nullptr;
        m_customImageLoaders.GetObjByKey(fourccID, loader);
		return loader;
	}

	/// 注册插件-----------------------------------------
	DMCode DMAppData::LoadPlugins(LPCWSTR lpszPluginsCfg)
	{
		return m_PluginsTool->LoadPlugins(lpszPluginsCfg);
	}

	DMCode DMAppData::LoadPlugin(LPCWSTR lpszPluginName)
	{
		return m_PluginsTool->LoadPlugin(lpszPluginName);
	}

	DMCode DMAppData::InstallPlugin(IDMPlugin* plugin)
	{
		return m_PluginsTool->InstallPlugin(plugin);
	}

	DMCode DMAppData::UninstallPlugin(IDMPlugin* plugin)
	{
		return m_PluginsTool->UninstallPlugin(plugin);
	}

	DMCode DMAppData::InitialisePlugins()
	{
		m_PluginsTool->InitialisePlugins();
		return DM_ECODE_OK;
	}

	DMCode DMAppData::ShutdownPlugins()
	{
		m_PluginsTool->ShutdownPlugins();
		return DM_ECODE_OK;
	}

	/// 解析资源Res-----------------------------------------
	DMCode DMAppData::LoadResPack(WPARAM wp, LPARAM lp, LPCSTR lpszClassName)
	{
		LOG_INFO("[start]lpszClassName:%s\n",lpszClassName);
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (NULL == wp&&NULL == lp)
			{
				iErr = DM_ECODE_FAIL;
				break;
			}

			if (NULL!=lpszClassName) // 不为NULL时判断是否和当前默认的相当
			{
				CStringA szName;
				m_RegMgr->GetDefRegObj(szName,DMREG_Res);
				if (lpszClassName != szName)
				{	
					iErr = SetDefRegObj(lpszClassName, DMREG_Res);// 设置成默认的
				}
			}

			if (m_pResObj.isNull())
			{
				DMFAIL_MSG("please call SetDefRegObj firstly");
				iErr = DM_ECODE_FAIL;
				break;
			}

			if (DMSUCCEEDED(iErr))
			{
				iErr = m_pResObj->LoadResPack(wp, lp);
			}

		} while (false);
		LOG_INFO("[end]iErr:%d\n",iErr);
		return iErr;
	}

	///  内部辅助------------------------------------------
	DMCode DMAppData::InitDMXmlDocument(DMXmlDocument &XmlDoc, LPCSTR lpszType,LPCSTR lpszResName)
	{
		LOG_INFO("[start]lpszType:%s,lpszResName:%s\n",lpszType,lpszResName);
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{ 
			DMXmlDocument* pDoc = NULL;
			if (g_pGetSubXmlDoc
				&&NULL!=(pDoc = g_pGetSubXmlDoc(lpszType, lpszResName)))// 支持外部生成Doc传入,内部复制,目前用于设计器
			{
				DMXmlNode XmlBase = XmlDoc.Base();
				DMXmlNode XmlCopy = pDoc->Root();
				XmlBase.InsertCopyChildNode(&XmlCopy);
				iErr = DM_ECODE_OK;
				break;
			}

			if (NULL==lpszType||NULL==lpszResName)
			{
				break;
			}
			DMBufT<byte> pBuf;
			unsigned long ulSize = 0;
			if (!DMSUCCEEDED(m_pResObj->GetItemBuf(lpszType, lpszResName, pBuf, &ulSize)))
				break;

			if (false == XmlDoc.LoadFromBuffer(pBuf, ulSize))
			{
				DMFAIL_MSG_FMT("%s:%s parse xml fail", lpszType, lpszResName);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		LOG_INFO("[end]iErr:%d\n",iErr);
		return iErr;
	}

	bool  DMAppData::IsNeedUpdateSkin(IDMSkinPtr pSkin,int type)
	{
		bool bRet = false;
		if (type == DMREG_Skin&&pSkin)
		{
			bRet = m_SkinPool->IsNeedUpdateSkin(pSkin);
		}
		return bRet;
	}

	DMCode DMAppData::ClearUpdateSkinArray()
	{
		return m_SkinPool->ClearUpdateSkinArray();
	}

	/// 辅助-----------------------------------------------
	ATOM DMAppData::RegisterClassEx(HINSTANCE hInst, LPCWSTR lpClassName, bool bShadow/*=false*/)
	{
		WNDCLASSEX wcex		  = {sizeof(WNDCLASSEX)};
		wcex.style            = CS_HREDRAW | CS_VREDRAW |CS_DBLCLKS;

		//window系统阴影是由窗口类来确定的，但SetClassLong(m_hWnd, GCL_STYLE, xxx|CS_DROPSHADOW)会影响其他窗体
		//故这里提供两种窗口类
		if (bShadow)
		{
			wcex.style |= CS_DROPSHADOW;
		}
		wcex.lpfnWndProc	  = DMCWnd::FirstWindowProc; // 第一个处理函数
		wcex.hInstance        = m_hInst;
		wcex.hCursor		  = ::LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszClassName    = lpClassName;

		ATOM atom = ::RegisterClassEx(&wcex);
		return atom;
	}

}//namespace DM