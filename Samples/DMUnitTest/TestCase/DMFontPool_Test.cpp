//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMUintTestAfx.h 
// File Des: 测试字体和字符串的关联
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-31	1.0			
//-------------------------------------------------------
#pragma once 
#include "DMUintTestAfx.h"
#include "DUIFontPool.h"

class DMFontPoolTest:public::testing::Test
{
public:
	virtual void SetUp()
	{	
	}

	virtual void TearDown()
	{

	}

public:

};

TEST_F(DMFontPoolTest, 测试FontPool)
{
	CStringW str = L"face:微软雅黑,size:0,weight:400,charset:134,underline:1,italic:1,strike:1,adding:0";
	DUIFontPool fontpool;
	fontpool.SetDefaultFont(str);
	IDMFontPtr pDefFont = fontpool.GetFont(L"");// 取得默认字体
	IDMFontPtr pDFont = fontpool.GetFont(L"size:0,weight:400"); //第一次创建
	IDMFontPtr pFont1 = fontpool.GetFont(L"size:0,weight:400"); //第二次取得
	EXPECT_EQ(pDFont == pFont1,true);

	EXPECT_EQ(0==_wcsicmp(pDFont->GetFaceName(),L"微软雅黑"),true);//没有设置的就采用默认的


}