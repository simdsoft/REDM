//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMTemplate_Test.cpp 
// File Des: 测试模板类的测试用例
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-8	1.0			
//-------------------------------------------------------
#pragma once
#include "DMUintTestAfx.h"

class DMTemplateTest:public::testing::Test
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

TEST_F(DMTemplateTest, 测试CStringW功能)
{
	CStringW sz = L"ok";
	LPCWSTR lp = sz;

}

TEST_F(DMTemplateTest, 测试DMMapT功能)
{
	DMMapT<int, CStringW> MapT;
	MapT.AddKey(1, L"string1");
	EXPECT_STREQ( L"string1", MapT.GetObjByKey(1));

	MapT.AddKey(2, L"string2");
	EXPECT_EQ(2, MapT.GetCount());
	
	MapT.RemoveAll();
	EXPECT_EQ(0, MapT.GetCount());
}

TEST_F(DMTemplateTest, 测试DMArrayT功能)
{

}


class LazyTest
{
public:
	LazyTest()
	{
		m_ptr = new byte[100];
	}
	~LazyTest()
	{
		DM_DELETE_ARRAY(m_ptr);
	}

	void Show()
	{
		printf("using....\n");
	}
public: 
	byte *m_ptr;
};


TEST_F(DMTemplateTest, 测试DMLazyT功能)
{
	DM::DMLazyT<LazyTest,false>  m_pObj;
	if (m_pObj)
	{
		int i=0;
	}
	EXPECT_EQ(false, m_pObj.IsValid());
	LazyTest*ptr = m_pObj.Init();
	EXPECT_EQ(true, m_pObj.IsValid());


	// 在调用时自动初始化
	DM::DMLazyT<LazyTest>  m_pAutoObj;
	
	EXPECT_EQ(false, m_pAutoObj.IsValid());
	m_pAutoObj->Show();// 在取指针时自动Init
	EXPECT_EQ(true, m_pAutoObj.IsValid());

}

TEST_F(DMTemplateTest, 测试DMBufT功能)
{
	DMBufT<BYTE>      dmbuf;
	BYTE *ptr = dmbuf.Allocate(100);
	BYTE *ptr1 = dmbuf;
	ptr = dmbuf.Allocate(200);
	ptr1 = dmbuf;
	ptr = dmbuf.Allocate(300);
	ptr1 = dmbuf;
}

class BaseA
{
public:
	BaseA(){printf("DMNewT---BaseA构造函数\n");}
	virtual~BaseA(){printf("DMDelT---BaseA析构函数\n");}

public:
	int   m_i;
	int   m_j;
};

class Sub:public BaseA
{
public:
	Sub(){printf("DMNewT---Sub构造函数\n");}
	virtual~Sub(){printf("DMDelT---Sub析构函数\n");}
	virtual void fun(){printf("fun函数\n");}
public:
	int m_k;
};

TEST_F(DMTemplateTest, 测试DMNewT和DMDelT功能)
{
	// 我们可以在exe中使用 DMNewT 生成Sub对象，并保存成Base指针到dll中
	// 最后在dll中使用DMDelT释放Sub对象
	BaseA *pObj = DMNewT<Sub>(2); 
	((Sub*)pObj)->fun();
	((Sub*)pObj+1)->fun();
	DMDelT<BaseA>(pObj);
}