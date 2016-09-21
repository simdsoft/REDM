//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSkinPool_Test.cpp 
// File Des: 测试皮肤绘制池类
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-28	1.0			
//-------------------------------------------------------
#pragma once
#include "DMUintTestAfx.h"
#include "DUISkinPool.h"


class DMSkinPoolTest:public::testing::Test
{
public:
	virtual void SetUp()
	{
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&m_pRes, DMREG_Res)),true);
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&m_pDefRender,DMREG_Render)),true);
		EXPECT_EQ(DMSUCCEEDED(m_pRes->LoadResPack((WPARAM)(L"UTRes"),NULL)),true);
	}

	virtual void TearDown()
	{

	}

protected:
	DMSmartPtrT<IDMRes>	   m_pRes;
	DMSmartPtrT<IDMRender> m_pDefRender; // 内部的默认使用对象
	DUISkinPool             m_pSkinPool;
};

TEST_F(DMSkinPoolTest, 测试)
{

	DMSmartPtrT<IDMRes> pRes;
	g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);

	unsigned long ulSize = 0;
	CStringW strType = L"global";CStringW strResName=L"global";
	pRes->GetItemSize(strType,strResName,ulSize);
	//byte *pBuf = new byte[ulSize];
	DMBufT<byte>pBuf;pBuf.Allocate(ulSize);
	pRes->GetItemBuf(strType,strResName, pBuf, ulSize);
	DMXmlDocument doc;
	doc.LoadFromBuffer(pBuf,ulSize);
	DMXmlNode XmlNode = doc.Root(L"global");
	EXPECT_EQ(XmlNode.IsValid(),true);
	DMXmlNode XmlSkin = XmlNode.FirstChild(L"skin");
	while (XmlSkin.IsValid())
	{
		m_pSkinPool.AddSkinPoolItem(XmlSkin);
		XmlSkin = XmlSkin.NextSibling(L"skin");
	}

	CStringW strName = L"";CStringW strKey = L"Btn_Close";
	IDMSkinPtr pSkin = m_pSkinPool.FindSkin(strKey,strName,true);

	m_pSkinPool.RemoveSkinPoolItem(strName);

	//DM_DELETE_ARRAY(pBuf);
}