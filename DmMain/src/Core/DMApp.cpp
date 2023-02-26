#include "DmMainAfx.h"
#include "DMApp.h"
#include "DMAppData.h"

namespace DM
{
	fun_cbGetSubXmlDoc  g_pGetSubXmlDoc = NULL; 
	DMApp::DMApp(HINSTANCE hInst/* = GetModuleHandle(NULL)*/)
	{
		OleInitialize(NULL);
		// 开始初始化全局数据
		new DMAppData(hInst);
	}

	DMApp::~DMApp()
	{
		delete DMAppData::getSingletonPtr();
	}

	ATOM   DMApp::GetClassAtom(bool bShadow)
	{
		if (bShadow)
		{
			return g_pDMAppData->m_shadowAtom;
		}
		else
		{
			return g_pDMAppData->m_Atom;
		}
	}

	DMCode DMApp::InitGlobal(LPCSTR lpszXmlId/*=L"global"*/)
	{
		return g_pDMAppData->InitGlobal(lpszXmlId);
	}

	void DMApp::AddCustomImageLoader(DWORD fourccID, DM_CUSTOM_IMAGELOADER loaderFn)
	{
		g_pDMAppData->AddCustomImageLoader(fourccID, loaderFn);
	}

	DMCode DMApp::Run(HWND hWnd)
	{
		return g_pDMAppData->Run(hWnd);
	}

	DMCode DMApp::IsRun(HWND hWnd)
	{
		return g_pDMAppData->IsRun(hWnd);
	}

	/// 注册类-----------------------------------------
	DMCode DMApp::Register(const IDMReg &RegObj, bool bReplace/*=false*/)
	{
		return g_pDMAppData->Register(RegObj, bReplace);
	}

	DMCode DMApp::CreateRegObj(void** ppObj, LPCSTR lpszClassName,int RegType)
	{
		return g_pDMAppData->CreateRegObj(ppObj, lpszClassName, RegType);
	}

	DMCode DMApp::UnRegister(LPCSTR lpszClassName,int RegType)
	{
		return g_pDMAppData->UnRegister(lpszClassName,RegType);
	}

	DMCode DMApp::SetDefRegObj(LPCSTR lpszClassName,int RegType)
	{
		return g_pDMAppData->SetDefRegObj(lpszClassName,RegType);
	}

	DMCode DMApp::GetDefRegObj(void** ppObj,int RegType)
	{
		return g_pDMAppData->GetDefRegObj(ppObj,RegType);
	}

	CStringA DMApp::GetDefRegObj(int RegType)
	{
		CStringA szName = "";
		DMCode iErr = g_pDMAppData->GetDefRegObj(szName, RegType);
		return szName;
	}

	/// 注册插件-----------------------------------------
	DMCode DMApp::LoadPlugins(LPCWSTR lpszPluginsCfg)
	{
		return g_pDMAppData->LoadPlugins(lpszPluginsCfg);
	}

	DMCode DMApp::LoadPlugin(LPCWSTR lpszPluginName)
	{
		return g_pDMAppData->LoadPlugin(lpszPluginName);
	}

	DMCode DMApp::InstallPlugin(IDMPlugin* plugin)
	{
		return g_pDMAppData->InstallPlugin(plugin);
	}

	DMCode DMApp::UninstallPlugin(IDMPlugin* plugin)
	{
		return g_pDMAppData->UninstallPlugin(plugin);
	}

	DMCode DMApp::InitialisePlugins()
	{
		return g_pDMAppData->InitialisePlugins();
	}

	DMCode DMApp::ShutdownPlugins()
	{
		return g_pDMAppData->ShutdownPlugins();
	}

	/// 解析资源Res--------------------------------------
	DMCode DMApp::LoadResPack(WPARAM wp, LPARAM lp, LPCSTR lpszClassName)
	{
		return g_pDMAppData->LoadResPack(wp, lp, lpszClassName);
	}

	/// 属性相关 -----------------------------------------
	DMCode DMApp::AddSkinPoolItem(DMXmlNode &XmlNode)
	{
		return g_pDMSkinPool->AddSkinPoolItem(XmlNode);
	}

	DMCode DMApp::RemoveSkinPoolItem(LPCSTR lpszName)
	{
		return g_pDMSkinPool->RemoveSkinPoolItem(lpszName);
	}

	DMCode DMApp::RemoveAllSkinPoolItemExcept(LPCSTR lpszName)
	{
		return g_pDMSkinPool->RemoveAllSkinPoolItemExcept(lpszName);
	}

	DMCode DMApp::AddStylePoolItem(DMXmlNode &XmlNode)
	{
		return g_pDMStylePool->AddStylePoolItem(XmlNode);
	}

	DMCode DMApp::RemoveStylePoolItem(LPCSTR lpszName)
	{
		return g_pDMStylePool->RemoveStylePoolItem(lpszName);
	}

	DMCode DMApp::RemoveAllStylePoolItem()
	{
		return g_pDMStylePool->RemoveAllStylePoolItem();
	}

	IDMFont* DMApp::GetFont(LPCSTR lpszFont)
	{
		return g_pDMFontPool->GetFont(lpszFont);
	}

	IDMSkin* DMApp::GetSkin(LPCSTR lpszSkin)
	{
		return g_pDMSkinPool->FindSkin(lpszSkin);
	}

	DMCode DMApp::AddSkin(void *pBuf,size_t bufLen, LPCSTR pszType,
		LPCSTR lpszXml, LPCSTR lpszPoolName/*=NULL*/)
	{
		return g_pDMSkinPool->AddSkin(pBuf,bufLen,pszType,lpszXml,lpszPoolName);
	}


	DMCode DMApp::RemoveSkin(LPCSTR lpszKey, LPCSTR lpszName,bool bLoopFind /*= true*/)
	{
		return g_pDMSkinPool->RemoveSkin(lpszKey,lpszName,bLoopFind);
	}

	DMXmlNode DMApp::GetStyle(LPCSTR lpszStyle)
	{
		return g_pDMStylePool->FindStyle(lpszStyle);
	}

	DMCode DMApp::UpdateSkin(WPARAM wp, LPARAM lp)
	{
		return g_pDMSkinPool->UpdateSkin(wp, lp);
	}

	DMCode DMApp::RedrawAll()
	{
		return g_pDMDWndPool->RedrawAll();
	}

	bool DMApp::IsNeedUpdateSkin(IDMSkinPtr pSkin,int type/*=DMREG_Skin*/)
	{
		return g_pDMAppData->IsNeedUpdateSkin(pSkin, type);
	}

	DMCode DMApp::AdjustHSLA(int ivalue,int iType)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (iType<0||iType>DM_A)
			{
				break;
			}
			CStringA strValue;
			strValue.Format("%d",ivalue);
			CStringA strType;
			switch (iType)
			{
			case DM_H: strType = "h";break;
			case DM_S: strType = "s";break;
			case DM_L: strType = "l";break;
			case DM_A: strType = "alpha";break;
			default:break;
			}
			int iCount = (int)g_pDMDWndPool->m_MainDUIWndArray.GetCount();
			DUIWND hDUIWnd = 0;
			for (int i=0;i<iCount;i++)
			{
				hDUIWnd = g_pDMDWndPool->m_MainDUIWndArray[i];
				DUIWindowPtr pWnd = g_pDMDWndPool->FindDUIWnd(hDUIWnd);
				if (pWnd)
				{
					DMHWnd*pHost = (DMHWnd*)pWnd;
					if (pHost->IsWindow())
					{
						iErr = pHost->SetAttribute(strType, strValue,false);
					}
				}
			}
		} while (false);
		return iErr;
	}

	/// 其他接口
	DMCode DMApp::AddIdleHandler(IDMIdleHandler* pIdleHandler)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMMsgLoop* pLoop = g_pDMMsgLoopTool->GetMessageLoop(pIdleHandler->m_dwThreadId);
			if (NULL == pLoop)
			{
				break;
			}
			if (pLoop->AddIdleHandler(pIdleHandler))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMApp::RemoveIdleHandler(IDMIdleHandler* pIdleHandler)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMMsgLoop* pLoop = g_pDMMsgLoopTool->GetMessageLoop(pIdleHandler->m_dwThreadId);
			if (NULL == pLoop)
			{
				break;
			}
			if (pLoop->RemoveIdleHandler(pIdleHandler))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMApp::AddMessageFilter(IDMMessageFilter* pMessageFilter)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMMsgLoop* pLoop = g_pDMMsgLoopTool->GetMessageLoop(pMessageFilter->m_dwThreadId);
			if (NULL == pLoop)
			{
				break;
			}
			if (pLoop->AddMessageFilter(pMessageFilter))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	DMCode DMApp::RemoveMessageFilter(IDMMessageFilter* pMessageFilter)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMMsgLoop* pLoop = g_pDMMsgLoopTool->GetMessageLoop(pMessageFilter->m_dwThreadId);
			if (NULL == pLoop)
			{
				break;
			}
			if (pLoop->RemoveMessageFilter(pMessageFilter))
			{
				iErr = DM_ECODE_OK;
			}
		} while (false);
		return iErr;
	}

	bool DMApp::AddMessageLoop(DMMsgLoop* pMsgLoop)
	{
		return g_pDMMsgLoopTool->AddMessageLoop(pMsgLoop);
	}

	DMCode DMApp::InitDMXmlDocument(DMXmlDocument &XmlDoc, LPCSTR lpszType,LPCSTR lpszResName)
	{
		return g_pDMAppData->InitDMXmlDocument(XmlDoc,lpszType,lpszResName);
	}

	DUIWindowPtr DMApp::FindDUIWnd(DUIWND hDUIWnd)
	{
		return g_pDMDWndPool->FindDUIWnd(hDUIWnd);
	}

	DMCode DMApp::SetSubXmlDocCallBack(fun_cbGetSubXmlDoc fun)
	{
		g_pGetSubXmlDoc = fun;
		return DM_ECODE_OK;
	}

	DMCode DMApp::LoadTrans(WPARAM wp, LPARAM lp /*= 0*/)
	{
		return g_pDMTrans->LoadTrans(wp, lp);
	}

	DMCode DMApp::SetTrans(CStringW strLanguageName)
	{
		return g_pDMTrans->SetTrans(strLanguageName);
	}

	DMCode DMApp::AddTransItem(CStringW strSrc, CStringW strTrans,CStringW strNodeName,CStringW strLanguageName)
	{
		return g_pDMTrans->AddTransItem(strSrc,strTrans,strNodeName,strLanguageName);
	}

	CStringW DMApp::GetTrans(CStringW strSrc,CStringW strNodeName,CStringW strLanguageName)
	{
#if !defined(DM_EXCLUDE_MUI)
		return g_pDMTrans->GetTrans(strSrc,strNodeName,strLanguageName);
#endif
		return strSrc;
	}
}//namespace DM