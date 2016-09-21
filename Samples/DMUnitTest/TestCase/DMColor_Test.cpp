//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMDIBHelper_Test.cpp 
// File Des: 测试RGBA--BGRA用例
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
#include "DMColor.h"

class DMColorTest:public::testing::Test
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

TEST_F(DMColorTest, 测试BGRA和RGBA间的变化)
{
	DMColor Clr = PBGRA(0x10,0x20,0x30,0x40);
	EXPECT_EQ(0x40==Clr.a&&0x30==Clr.r&&0x20==Clr.g&&0x10==Clr.b, true);
	DMColor DMClr(Clr);
	DWORD bgraClr = DMClr.ToBGRA();
	EXPECT_EQ(0X40302010==bgraClr, true);
	DMClr.SetbyBGRA(bgraClr);
	COLORREF Clr1 = DMClr.ToCOLORREF();
	EXPECT_EQ(0X102030==(int)Clr1, true);

	DMColor clr = PBGRA(0x10,0x20,0x30,0x40);
	clr;
}

