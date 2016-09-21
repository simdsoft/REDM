//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMXmlParse_Test.cpp 
// File Des: 测试Xml解析封装类
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
#include "DMXmlParse.h"

class DMXmlParseTest:public::testing::Test
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{

	}
};

TEST_F(DMXmlParseTest,test1)
{
	DMXmlDocument doc;
	EXPECT_EQ(doc.LoadFromFile(L"UTRes\\UTTest\\UTXmlParse.xml"), true);

	// 用于打印Xml内容
	// 先做遍历
	DMXmlNode XmlElementNoFind = doc.Root(L"hgy");
	EXPECT_EQ(XmlElementNoFind.IsValid(), false);// 并没有hgy这个节
	DMXmlNode XmlElementRoot = doc.Root();
	DMXmlNode XmlElementRoot1 = doc.Root(L"xmlui");
	EXPECT_EQ(XmlElementRoot.IsEqual(XmlElementRoot1), true);
	EXPECT_EQ(XmlElementRoot.IsEqual(XmlElementNoFind), false);

	DMXmlNode XmlElement = XmlElementRoot.FirstChild().FirstChild().FirstChild();
	LPCWSTR szName = XmlElement.GetName();
	LPCWSTR szValue = XmlElement.GetValue();
	XmlElement = XmlElement.NextSibling();

	DMXmlAttribute XmlAttribute;
	EXPECT_EQ(XmlAttribute.IsValid(), false);
	XmlAttribute = XmlElement.FirstAttribute();
	EXPECT_EQ(XmlAttribute.IsValid(), true);
	XmlAttribute = XmlAttribute.NextAttribute();
	szName = XmlAttribute.GetName();
	szValue = XmlAttribute.GetValue();
}

