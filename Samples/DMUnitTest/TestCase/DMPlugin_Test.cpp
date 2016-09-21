//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMPlugin_Test.cpp 
// File Des: 测试插件方式的测试用例
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-16	1.0			
//-------------------------------------------------------
#pragma once

#include "DMUintTestAfx.h"

class DMPluginTest:public::testing::Test
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}

};

class Plugin_Test:public IDMPlugin
{
public:
	Plugin_Test(){}
	~Plugin_Test(){}
	const wchar_t* GetName() const{return L"Plugin_Test";};
	void Install()
	{
		printf("Plugin_Test::Install\n");
	}

	void Uninstall()
	{
		printf("Plugin_Test::Uninstall\n");
	}

	void Initialise()
	{
	}

	void Shutdown()
	{
	}
};

TEST_F(DMPluginTest,测试)
{
#ifdef DLL_DMMAIN
	IDMPlugin* plugin = new Plugin_Test;// 这个不能delete，必须先反安装
	g_pDMApp->InstallPlugin(plugin);
	g_pDMApp->UninstallPlugin(plugin);
	DM_DELETE(plugin);


// 绝对路径测试
#if 0
#ifdef _DEBUG
	g_pDMApp->LoadPlugins(L"I:\\REDM\\Docs\\TestData\\dmpluginsd.cfg");
#else
	g_pDMApp->LoadPlugins(L"I:\\REDM\\Docs\\TestData\\dmplugins.cfg");
#endif if
#endif


#ifdef _DEBUG
	g_pDMApp->LoadPlugins(L"dmpluginsd.cfg");
#else
	g_pDMApp->LoadPlugins(L"dmplugins.cfg");
#endif
#endif //DLL_DMMAIN
}
