//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMRegister_Test.cpp 
// File Des: 测试注册、反注册功能
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-15	1.0			
//-------------------------------------------------------
#pragma once
#include "DMUintTestAfx.h"

class DMRegisterTest:public::testing::Test
{
public:
	virtual void SetUp()
	{

	}

	virtual void TearDown()
	{

	}

protected:
};

class DMLogImplTest:public IDMLog
{
	DMDECLARE_CLASS_NAME(DMLogImplTest, L"DMLogImplTest",DMREG_Log);
public:
	// todo.
	DMCode LogW(DMLOGLEVEL iLevel,LPCWSTR lpszFuncName, LPCWSTR lpszFileName, int iLine,LPCWSTR szLogBuf){return DM_ECODE_OK;}
};

class DMLogImplTest1:public IDMLog
{
	DMDECLARE_CLASS_NAME(DMLogImplTest1, L"DMLogImplTest1",DMREG_Log);
public:
	DMLogImplTest1()
	{
		printf("DMLogImplTest1\n");
	}
	~DMLogImplTest1()
	{
		printf("~DMLogImplTest1\n");
	}
	// todo.
	DMCode LogW(DMLOGLEVEL iLevel,LPCWSTR lpszFuncName, LPCWSTR lpszFileName, int iLine,LPCWSTR szLogBuf)
	{
		OutputDebugStringW(L"gtest:DMLogImplTest1\n");
		return DM_ECODE_OK;
	}
};


TEST_F(DMRegisterTest, 测试注册功能)
{
	EXPECT_EQ(DMSUCCEEDED(DM::g_pDMApp->Register(DMRegHelperT<DMLogImplTest>(),true)),true);
	EXPECT_EQ(DMSUCCEEDED(DM::g_pDMApp->Register(DMRegHelperT<DMLogImplTest1>(),true)),true);
	EXPECT_EQ(DMSUCCEEDED(DM::g_pDMApp->Register(DMRegHelperT<DMLogImplTest>(),false)),false); // 非强制覆盖
	EXPECT_EQ(DMSUCCEEDED(DM::g_pDMApp->Register(DMRegHelperT<DMLogImplTest1>(),true)),true);  // 强制覆盖

	EXPECT_EQ(DMSUCCEEDED(DM::g_pDMApp->UnRegister(L"DMLogImplTest",DMREG_Log)),true);         // 反注册
	EXPECT_EQ(DMSUCCEEDED(DM::g_pDMApp->SetDefRegObj(DMLogImplTest1::GetClassName(),DMLogImplTest1::GetClassType())), true);

	DMSmartPtrT<IDMLog>  m_pLogObj;
	EXPECT_EQ(DMSUCCEEDED(DM::g_pDMApp->CreateRegObj((void**)&m_pLogObj, L"DMLogImplTest",IDMLog::GetClassType())),false);   // 这个类被反注册了，所以返回false
	EXPECT_EQ(DMSUCCEEDED(DM::g_pDMApp->CreateRegObj((void**)&m_pLogObj, L"DMLogImplTest1",IDMLog::GetClassType())),true);

	LPCWSTR lpszClassName = DM::g_pDMApp->GetDefRegObj(DMREG_Log);


}

