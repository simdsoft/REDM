#include "DmMainAfx.h"
#include "DMPluginsTool.h"
#include "DMDynLibMgr.h"

namespace DM
{
	DMPluginsTool::DMPluginsTool(void)
	{
		m_IsInitialised = false;
		new DMDynLibMgr();
	}

	DMPluginsTool::~DMPluginsTool(void)
	{
		UnloadPlugins();//插件卸载(清理工作，同时通过DMDynLibManager删除插件所在dll)
		delete DMDynLibMgr::getSingletonPtr();
	}


	DMCode DMPluginsTool::LoadPlugins(const wchar_t* pPluginsFile/* = L"plugins.cfg"*/)
	{
		LOG_DEBUG("[start]\n");
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			// dir路径
			wchar_t szExeDir[MAX_PATH] = {0};
			if (!GetRootDirW(szExeDir, MAX_PATH))
			{
				iErr = DM_ECODE_GETROOTDIR_FAIL;
				break;
			}

			wchar_t szPath[MAX_PATH] = {0};
			if (0 == PathCombineW(szPath, szExeDir, pPluginsFile))
			{
				iErr = DM_ECODE_PATHCOMBINE_FAIL;
				break;
			}

			if (!CheckFileExistW(szPath))
			{
				iErr = DMPLUGINSTOOL_LOADPLUGINS_CFGFILE_NOTEXIST;
				break;
			}

			ParsePluginCfg(szPath, szExeDir);
		} while (false);
		LOG_DEBUG("[end]iErr:%d\n",iErr);
		return iErr;
	}

	void DMPluginsTool::UnloadPlugins(void)
	{
		int nCount = (int)m_LibsArray.GetCount();
		// hgy413:一般我们会把LOG插件放在最前面加载，那么就应该最后卸载，这样可以保证其他的析构函数仍可调用它打印LOG,所以这里倒着卸载
		for (int i=nCount-1; i>=0; i--)
		{
			DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)m_LibsArray[i]->GetSymbol("dllStopPlugin");
			if (pFunc)
			{
				pFunc();
			}

			// Unload library & destroy
			DMDynLibMgr::getSingleton().Unload(m_LibsArray[i]);
		}
		m_LibsArray.RemoveAll();

		// 移除以其他方式注册进来的插件.一般非dll
		nCount = (int)m_PluginArray.GetCount();
		for (int i=0; i<nCount; i++)
		{
			m_PluginArray[i]->Uninstall();
		}
		m_PluginArray.RemoveAll();
	}

	DMCode DMPluginsTool::LoadPlugin(const CStringW& strPluginName)
	{
		LOG_DEBUG("[start]\n");
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			DMDynLib *pLib = DMDynLibMgr::getSingletonPtr()->Load(strPluginName);
			if (NULL == pLib)
			{
				iErr = DMPLUGINSTOOL_LOADPLUGIN_LOAD_FAIL;
				break;
			}

			bool bFind = false;
			size_t nCount = m_LibsArray.GetCount();
			for (size_t i=0; i<nCount; i++)
			{
				if (pLib == m_LibsArray[i])
				{
					bFind = true;
					break;
				}
			}
			if (bFind) // 模块已存在
			{  
				iErr = DMPLUGINSTOOL_LOADPLUGIN_ALREADY_EXIST;
				break;
			}

			DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)pLib->GetSymbol("dllStartPlugin");
			if (!pFunc)
			{
				DMASSERT_EXPR(NULL!=pFunc, L"未找到导出函数dllStartPlugin");
				//DMDynLibMgr::getSingletonPtr()->Unload(pLib);//直接卸载
				iErr = DMPLUGINSTOOL_LOADPLUGIN_NOFIND_START_EXPORT;
				break;
			}

			pFunc();
			m_LibsArray.Add(pLib);
		} while (false);
		LOG_DEBUG("[end]iErr:%d\n",iErr);
		return iErr;
	}

	void DMPluginsTool::UnloadPlugin(const CStringW& strPluginName)
	{
		size_t nCount = m_LibsArray.GetCount();
		for (size_t i=0; i<nCount; i++)
		{
			if (strPluginName == m_LibsArray[i]->GetName())
			{
				// Call plugin shutdown
				DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN) m_LibsArray[i]->GetSymbol("dllStopPlugin");
				// this must call uninstallPlugin
				if (pFunc)
				{
					pFunc();
				}

				// Unload library (destroyed by DynLibManager)
				DMDynLibMgr::getSingleton().Unload(m_LibsArray[i]);
				m_LibsArray.RemoveAt(i);
				break;
			}
		}
	}


	DMCode DMPluginsTool::InstallPlugin(IDMPlugin* plugin)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == plugin)
			{
				break;
			}
			bool bFind = false;
			size_t nCount = m_PluginArray.GetCount();
			for (int i=0;i<(int)nCount; i++)
			{
				if (m_PluginArray[i] == plugin)
				{
					bFind = true;
					break;
				}
			}
			if (bFind)
			{
				break;
			}

			m_PluginArray.Add(plugin);
			plugin->Install();
			if (m_IsInitialised)
			{
				plugin->Initialise();
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMPluginsTool::UninstallPlugin(IDMPlugin* plugin)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			size_t nCount = m_PluginArray.GetCount();
			for (int i=0; i<(int)nCount; i++)
			{
				if (m_PluginArray[i] == plugin)
				{
					iErr = DM_ECODE_OK;
					if (m_IsInitialised)
					{
						plugin->Shutdown();
					}

					plugin->Uninstall();
					m_PluginArray.RemoveAt(i);
					break;
				}
			}
		} while (false);
		return iErr;
	}

	void DMPluginsTool::InitialisePlugins(void)
	{
		size_t nCount = m_PluginArray.GetCount();
		for (size_t i=0; i<nCount; i++)
		{
			m_PluginArray[i]->Initialise();
		}
	}

	void DMPluginsTool::ShutdownPlugins(void)
	{
		size_t nCount = m_PluginArray.GetCount();
		for (size_t i=0; i<nCount; i++)
		{
			m_PluginArray[i]->Shutdown();
		}
	}

	// 辅助
	void DMPluginsTool::ParsePluginCfg(const wchar_t* pPluginsFile, wchar_t *szExeDir)
	{
		/*
		<?xml version="1.0" encoding="utf-8"?>
		<body>
			<!--directory指明了plugin存放在哪个文件夹内-->
			<directory name=".\">
			<item name="Plugin_D3D9.dll"/>
			<item name="Plugin_GL.dll"/>
		</directory>
		</body>
		*/
		do 
		{
			DMXmlDocument xmlDoc;
			if (false == xmlDoc.LoadFromFile(pPluginsFile))
			{
				break;
			}

			DMXmlNode body = xmlDoc.Root(L"body");
			if (false == body.IsValid())
			{
				break;
			}

			DMXmlNode directory = body.FirstChild(L"directory");
			while (directory.IsValid())
			{
				wchar_t *pDir = (wchar_t*)directory.Attribute(L"name");
				wchar_t szPluginDir[MAX_PATH] = {0};
				if (NULL == PathCombineW(szPluginDir, szExeDir, pDir))
				{
					directory = directory.NextSibling(L"directory");
					continue;
				}
				CStringW strDir = szPluginDir;
				DMXmlNode item = directory.FirstChild(L"item");
				while (!strDir.IsEmpty()&&item.IsValid())
				{
					if (strDir.Right(1)!=L'/'&&strDir.Right(1)!=L'\\')
					{// 自动加上后缀
						strDir += L'\\';
					}

					wchar_t *pPluginName = (wchar_t*)item.Attribute(L"name");
					LoadPlugin(strDir+pPluginName);
					item = item.NextSibling(L"item");
				}

				directory = directory.NextSibling(L"directory");
			}
		} while (false);
	}


}//namespace DM