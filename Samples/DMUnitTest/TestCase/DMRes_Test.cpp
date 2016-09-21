//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMRes_Test.cpp 
// File Des: 测试资源打包方式的测试用例
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-11	1.0			
//-------------------------------------------------------
#pragma once

#include "DMUintTestAfx.h"

class DMResTest:public::testing::Test
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

TEST_F(DMResTest,测试Res文件夹打包)
{
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->LoadResPack((WPARAM)L"UTRes",NULL,L"DMResFolderImpl")),true);
	
	// 测试内置的DMResFolderImpl类是否正常
	DMSmartPtrT<IDMRes> m_pRes;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&m_pRes, DMREG_Res)),true);

	//EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&m_pRes,L"DMResFolderImpl",DMREG_Res)),true);

	// 解包功能
	EXPECT_EQ(DMSUCCEEDED(m_pRes->LoadResPack((WPARAM)(L"UTRes"),NULL)),true);

	// 判断layout中的XML_MAINWND是否存在
	EXPECT_EQ(DMSUCCEEDED(m_pRes->IsItemExists(RES_LAYOUT,L"dui_main")),true);
	EXPECT_EQ(DMSUCCEEDED(m_pRes->IsItemExists(RES_LAYOUT,L"dui_main1")),false);

	// 判断theme中的Btn_Restore是否存在
	EXPECT_EQ(DMSUCCEEDED(m_pRes->IsItemExists(L"png",L"Btn_Restore")),true);

	// 取得default主题下的btn_menu
	unsigned long size1 = 0;
	EXPECT_EQ(DMSUCCEEDED(m_pRes->GetItemSize(L"png",L"Btn_Menu",size1)),true);

	DMBufT<byte>pBuf;pBuf.Allocate(size1);
	EXPECT_EQ(DMSUCCEEDED(m_pRes->GetItemBuf(L"png",L"Btn_Menu",pBuf,size1)),true);


	EXPECT_EQ(DMSUCCEEDED(m_pRes->SetCurTheme(L"theme1")),true);

	EXPECT_EQ(DMSUCCEEDED(m_pRes->IsItemExists(L"png",L"Btn_Restore")),true);
	EXPECT_EQ(DMSUCCEEDED(m_pRes->GetItemSize(L"png",L"Btn_Menu",size1)),true);
	EXPECT_EQ(DMSUCCEEDED(m_pRes->GetItemSize(L"png",L"Btn_Restore",size1)),true);


	// 外部加载皮肤功能
	EXPECT_EQ(DMSUCCEEDED(m_pRes->LoadTheme((WPARAM)L"theme2",(LPARAM)L"UTRes\\themes\\theme2\\dmindex.xml")),true);
	EXPECT_EQ(DMSUCCEEDED(m_pRes->SetCurTheme(L"theme2")),true);
	EXPECT_EQ(DMSUCCEEDED(m_pRes->GetItemSize(L"png",L"Btn_Menu",size1)),true);
	

}