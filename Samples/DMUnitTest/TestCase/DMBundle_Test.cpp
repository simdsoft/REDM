//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMCanvas_Test.cpp 
// File Des: 测试Canvas绘制的测试用例
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2019-3-24	1.0			
//-------------------------------------------------------
#pragma once
#include "DMUintTestAfx.h"


class BundleTest:public::testing::Test
{
public:
	virtual void SetUp(){}
	virtual void TearDown(){}

	// 使用DMBundle设置绑定数据
	static DMBundle InitBundle()
	{
		DMBundle bundle;
		bundle.PutBool(L"1", true);
		bundle.PutByte(L"2", 'a');
		bundle.PutChar(L"3", 'b');
		bundle.PutDword(L"4", 450);
		bundle.PutDouble(L"5", 500.122);
		bundle.PutFloat(L"6", 510.2f);
		bundle.PutInt(L"7", 54546);
		bundle.PutInt64(L"8", 0xffffffffffff);
		bundle.PutShort(L"9", 0xfffff);
		bundle.PutString(L"10", "127758857");
		bundle.PutStringW(L"11", L"234565");
		bundle.PutSize(L"12", CSize(10,9));
		bundle.PutRect(L"13", CRect(1,2,3,4));
		bundle.PutVoidPtr(L"14", (void*)0x12233);
		CStringW strWXml = L"<imglist id=\"1\" src=\"png:1\" states=\"1\" />";
		CStringA strXml = DMW2A(strWXml,CP_UTF8);
		DMXmlDocument doc;
		doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
		DMXmlNode XmlNode = doc.Root();
		XmlNode.SetAttribute(L"name1",L"value");
		XmlNode.SetAttributeInt(L"name2",222);
		bundle.PutXmlNode(L"15", XmlNode);
		
		return bundle;
	}
};


TEST_F(BundleTest, 测试基础函数)
{
	DMBundle bundle = BundleTest::InitBundle();
	DMBundle bundle1 = bundle; // 指向同一块DMBundleImpl内存，引用计数+1
	DMBundle bundle2(bundle);  // 指向同一块DMBundleImpl内存，引用计数+1
	DMBundle bundle3 = bundle.Clone();// 复制一块新的DMBundleImpl内存,bundle3初始引用计数为1

	bool ret1 = bundle.GetBool(L"1");
	byte ret2 = bundle.GetByte(L"2");
	char ret3 = bundle.GetChar(L"3");
	DWORD ret4 = bundle.GetDword(L"4");
	double ret5 = bundle.GetDouble(L"5");
	float ret6 = bundle.GetFloat(L"6");;
	int ret7 = bundle.GetInt(L"7");
	INT64 ret8 = bundle.GetInt64(L"8");
	short ret9 = bundle.GetShort(L"9",122);
	CStringA ret10 = bundle.GetString(L"10");
	CStringW ret11 = bundle.GetStringW(L"11");
	CSize ret12 = bundle.GetSize(L"12");
	CRect ret13 = bundle.GetRect(L"13");
	void* ret14 = bundle.GetVoidPtr(L"14");
	DMXmlNode ret15 = bundle.GetXmlNode(L"15");

}