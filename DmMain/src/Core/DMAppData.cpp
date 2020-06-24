#include "DmMainAfx.h"
#include "DMAppData.h"
#include "DMCWnd.h"

namespace DM
{
	typedef DMXmlDocument* (*fun_cbGetSubXmlDoc)(LPCWSTR,LPCWSTR);
	extern 	fun_cbGetSubXmlDoc  g_pGetSubXmlDoc;
	DMAppData::DMAppData(HINSTANCE hInst/* = GetModuleHandle(NULL)*/)
	{
		// ���������ڲ����.�ڲ������Install��������ģ�飨��Ӧfilter:Modules����ʼ������
		// ���ز��һ������ǰ�棬��ʱ�����log������أ�Ҳ������ǰ��
		m_pPlugin    = new DMPluginImpl;
		InstallPlugin(m_pPlugin);

		m_hInst		 = hInst;
		CStringW strAtom;strAtom.Format(L"%s_%d_%d",DM_DEF_CLASS_NAME,::GetTickCount(),::GetCurrentThreadId());
		CStringW strShadowAtom;strShadowAtom.Format(L"%s_%d_%d",DM_DEF_SHADOWCLASS_NAME,::GetTickCount(),::GetCurrentThreadId());
		m_Atom		 = RegisterClassEx(m_hInst, strAtom.GetBuffer());strAtom.ReleaseBuffer();
		m_shadowAtom = RegisterClassEx(m_hInst, strShadowAtom.GetBuffer(), true);strShadowAtom.ReleaseBuffer();
		m_pTaskRunnerObj->InitEvent(::GetCurrentThreadId());// ��ATOM������Ų��ܴ���UI��Ϣ����

		// ȫ�ֱ�����ʼ��
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
	}

	DMAppData::~DMAppData()
	{	
		if (m_hModuleDWM) FreeLibrary(m_hModuleDWM);

		// �ȹر�LOG
		DMLogDispatch::SetLogDispatch(NULL);

		// ж�������ڲ����.
		m_pTaskRunnerObj->UninstallEvent();
		UninstallPlugin(m_pPlugin);
		DM_DELETE(m_pPlugin);
	}

	DMCode DMAppData::InitGlobal(LPCWSTR lpszXmlId)
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

			// ����Ĭ���ı�---------------------------
			CStringW strFont = XmlNode.Attribute(DMAttr::DMGlobalAttr::XMLATTR_font);
			if (!strFont.IsEmpty())
			{
				m_FontPool->SetDefaultFont(strFont);
			}

			// ����ȫ��Skin---------------------------
			DMXmlNode XmlSkin = XmlNode.FirstChild(DMAttr::DMGlobalAttr::XMLNODE_skin);
			while (XmlSkin.IsValid())
			{
				m_SkinPool->AddSkinPoolItem(XmlSkin);
				XmlSkin = XmlSkin.NextSibling(DMAttr::DMGlobalAttr::XMLNODE_skin);
			}

			// ����ȫ��Style--------------------------
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
			if (NULL == pMsgLoop)// ��ǰ�߳�û��Loop���򴴽�Loop
			{
				DMMsgLoop *pThisLoop = new DMMsgLoop;
				if (!m_MsgLoopTool->AddMessageLoop(pThisLoop))
				{
					iErr = DMAPP_RUN_ADDMESSAGELOOP_FAIL;
					break;
				}
				pThisLoop->SetRefCount(1);// ��ʼLoop����Ϊ1
				pMsgLoop = pThisLoop;
			}
			else
			{
				pMsgLoop->AddRef(); // ���ü���+1
			}

			// HWND hLastMainWnd = ::GetActiveWindow();
			// if (::IsWindow(hWnd))
			// {
			// 	if (!(::GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW))
			// 	{// ��WS_EX_TOOLWINDOW���ԵĲ�ǿ�����ü���
			// 		hLastMainWnd = ::SetActiveWindow(hWnd);
			// 	}
			// }
			size_t index = m_RunhWndArray.Add(hWnd);// ����DestroyWindow���ж��Ƿ���WM_QUIT��Ϣ 
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
			{// ��ǰ�̵߳����һ��������ʹ��Loop��
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

	/// ע����-----------------------------------------
	DMCode DMAppData::Register(IDMReg &RegObj, bool bReplace/*=false*/)
	{
		return m_RegMgr->Register(RegObj, bReplace);
	}

	DMCode DMAppData::CreateRegObj(void** ppObj, LPCWSTR lpszClassName,int RegType)
	{
		return m_RegMgr->CreateRegObj(ppObj, lpszClassName, RegType);
	}

	DMCode DMAppData::UnRegister(LPCWSTR lpszClassName,int RegType)
	{
		return m_RegMgr->UnRegister(lpszClassName, RegType);
	}

	DMCode DMAppData::SetDefRegObj(LPCWSTR lpszClassName,int RegType)
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
					m_pAttributeObj.Release();// ���ͷ�
					iErr = CreateRegObj((void**)&m_pAttributeObj, lpszClassName, RegType);
					if (DMSUCCEEDED(iErr))
					{
						DMAttributeDispatch::SetAttributeDispatch(m_pAttributeObj);
					}		
				}
				break;
			case DMREG_Log:
				{
					DMLogDispatch::SetLogDispatch(NULL);// ����ΪNULL����Ϊ�ڽ�������CreateRegObj�п��ܻ���LOG
					m_pLogObj.Release();// ���ͷ�
					iErr = CreateRegObj((void**)&m_pLogObj, lpszClassName, RegType);
					if (DMSUCCEEDED(iErr))
					{
						DMLogDispatch::SetLogDispatch(m_pLogObj);
					}		
				}
				break;
			case DMREG_Res:
				{
					m_pResObj.Release();// ���ͷ�
					iErr = CreateRegObj((void**)&m_pResObj, lpszClassName, DMREG_Res);
				}
				break;
			case DMREG_Render:
				{
					m_pRenderObj.Release();// ���ͷ�
					iErr = CreateRegObj((void**)&m_pRenderObj, lpszClassName, DMREG_Render);
				}
				break;
			case DMREG_Trans:
				{
					m_pTransObj.Release();// ���ͷ�
					iErr = CreateRegObj((void**)&m_pTransObj, lpszClassName, DMREG_Trans);
				}
				break;
			case DMREG_TaskRunner:
				{
					m_pTaskRunnerObj.Release();// ���ͷ�
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
					// ����Ҫָ��Ĭ������,������Ψһ
				}
				break;
			case DMREG_Window:
			case DMREG_FlowLayout:
			case DMREG_Animate:
			case DMREG_Skin:
				{
					// ����ʵ����Ҫָ��Ĭ������
				}	
				break;
			default:
				{
					DMASSERT_EXPR(0,L"��֧������Ĭ�϶����RegType");
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
					DMASSERT_EXPR(0,L"��֧��ȡ��DMREG_Attribute��DMREG_Log��DMREG_Res��DMREG_Render��DMREG_Trans��DMREG_TaskRunner��RegType");
				}
				break;
			}
		} while (false);
		return iErr;
	}

	DMCode DMAppData::GetDefRegObj(CStringW &szName,int RegType)
	{
		return m_RegMgr->GetDefRegObj(szName, RegType);
	}

	/// ע����-----------------------------------------
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

	/// ������ԴRes-----------------------------------------
	DMCode DMAppData::LoadResPack(WPARAM wp, LPARAM lp,LPCWSTR lpszClassName)
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

			if (NULL!=lpszClassName) // ��ΪNULLʱ�ж��Ƿ�͵�ǰĬ�ϵ��൱
			{
				CStringW szName;
				m_RegMgr->GetDefRegObj(szName,DMREG_Res);
				if (lpszClassName != szName)
				{	
					iErr = SetDefRegObj(lpszClassName, DMREG_Res);// ���ó�Ĭ�ϵ�
				}
			}

			if (m_pResObj.isNull())
			{
				DMASSERT_EXPR(0,L"���ȵ���SetDefRegObj����m_pResObj���󣡣�");
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

	///  �ڲ�����------------------------------------------
	DMCode DMAppData::InitDMXmlDocument(DMXmlDocument &XmlDoc, LPCWSTR lpszType,LPCWSTR lpszResName)
	{
		LOG_INFO("[start]lpszType:%s,lpszResName:%s\n",lpszType,lpszResName);
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMXmlDocument* pDoc = NULL;
			if (g_pGetSubXmlDoc
				&&NULL!=(pDoc = g_pGetSubXmlDoc(lpszType,lpszResName)))// ֧���ⲿ����Doc����,�ڲ�����,Ŀǰ���������
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
			unsigned long ulSize = 0;
			if(!DMSUCCEEDED(m_pResObj->GetItemSize(lpszType,lpszResName,ulSize)))
			{
				break;
			}
			DMBufT<byte>pBuf;pBuf.Allocate(ulSize);
			if(!DMSUCCEEDED(m_pResObj->GetItemBuf(lpszType,lpszResName, pBuf, ulSize)))
			{
				break;
			}

			if (false == XmlDoc.LoadFromBuffer(pBuf, ulSize))
			{
				CStringW strInfo;
				strInfo.Format(L"%s:%s xml����ʧ��!����utf-8��ʽ����,����֤xml��ʽ����",lpszType,lpszResName);
				DMASSERT_EXPR(0,strInfo);
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

	/// ����-----------------------------------------------
	ATOM DMAppData::RegisterClassEx(HINSTANCE hInst, LPCWSTR lpClassName, bool bShadow/*=false*/)
	{
		WNDCLASSEX wcex		  = {sizeof(WNDCLASSEX)};
		wcex.style            = CS_HREDRAW | CS_VREDRAW |CS_DBLCLKS;

		//windowϵͳ��Ӱ���ɴ�������ȷ���ģ���SetClassLong(m_hWnd, GCL_STYLE, xxx|CS_DROPSHADOW)��Ӱ����������
		//�������ṩ���ִ�����
		if (bShadow)
		{
			wcex.style |= CS_DROPSHADOW;
		}
		wcex.lpfnWndProc	  = DMCWnd::FirstWindowProc; // ��һ��������
		wcex.hInstance        = m_hInst;
		wcex.hCursor		  = ::LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszClassName    = lpClassName;

		ATOM atom = ::RegisterClassEx(&wcex);
		return atom;
	}

}//namespace DM