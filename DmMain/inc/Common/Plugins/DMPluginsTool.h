//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMPluginsTool.h 
// File Des: 用于加载、管理插件
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-16	1.0			
//-------------------------------------------------------
#pragma once


namespace DM
{
	class DMDynLib;

	/// <summary>
	///		插件管理
	/// </summary>
	class DMPluginsTool
	{
		typedef void (*DLL_START_PLUGIN)(void);
		typedef void (*DLL_STOP_PLUGIN)(void);
	public:
		DMPluginsTool(void);
		virtual ~DMPluginsTool(void);

		//---------------------------------------------------
		// Function Des: 解析加载plugin-dll配置文件，注意以下四个函数都是加载动态dll
		//  dll必须提供dllStartPlugin和dllStopPlugin两个函数
		//  一般是dllStartPlugin调用InstallPlugin加载dll中的插件
		//        dllStopPlugin调用UninstallPlugin清除dll中的插件
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		DMCode LoadPlugins(const wchar_t* pPluginsFile = L"dmplugins.cfg");
		void UnloadPlugins(void);

		DMCode LoadPlugin(const CStringW& strPluginName);
		void UnloadPlugin(const CStringW& strPluginName);

		//---------------------------------------------------
		// Function Des: 加载并安装plugin,用于加载插件
		// 可以在dll中调用此函数加载插件，也可以在lib中调用
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		DMCode InstallPlugin(IDMPlugin* plugin);
		DMCode UninstallPlugin(IDMPlugin* plugin);

		void InitialisePlugins();
		void ShutdownPlugins();

		//---------------------------------------------------
		// Function Des: 设置系统加载完成状态
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		inline void SetInit(bool bInit=true){m_IsInitialised = bInit;}

	protected:
		void ParsePluginCfg(const wchar_t* pPluginsFile, wchar_t *szExeDir);

	protected:
		typedef DM::CArray<DMDynLib*>   PluginLibArray;
		typedef DM::CArray<IDMPlugin*>	PluginArray;

		PluginLibArray                  m_LibsArray;		///<包含插件的dll列
		PluginArray                     m_PluginArray;		///<插件列（dll中会注册插件列,静态lib也可以注册插件列）
		bool                            m_IsInitialised;    ///<是否系统初始化完成
	};




}//namespace DM