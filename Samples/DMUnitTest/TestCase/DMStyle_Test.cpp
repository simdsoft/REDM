//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMStyle_Test.cpp 
// File Des: 测试内置的style
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-4	1.0			
//-------------------------------------------------------
#pragma once
#include "DMUintTestAfx.h"

class DMStyleTest:public::testing::Test
{
public:
	virtual void SetUp()
	{
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&m_pRes, DMREG_Res)),true);
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&m_pDefRender,DMREG_Render)),true);
		EXPECT_EQ(DMSUCCEEDED(m_pRes->LoadResPack((WPARAM)(L"UTRes"),NULL)),true);
		g_pDMApp->InitGlobal(); // 加载一点skin
	}

	virtual void TearDown()
	{

	}

protected:
	DMSmartPtrT<IDMRes>	   m_pRes;
	DMSmartPtrT<IDMRender> m_pDefRender; // 内部的默认使用对象
};

TEST_F(DMStyleTest, 测试)
{
	DMSmartPtrT<IDMStyle>     pStyle;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pStyle,L"style",DMREG_Style)),true);

	wchar_t pWBuf[] = L"<style id=\"custom\" "\
					L"clrtext=\"rgba(ff,ff,ff,ff)\" clrtexthover=\"rgba(ff,00,ff,ff)\" clrtextpush=\"rgba(ff,ff,00,ff)\" clrtextdisable=\"rgba(00,ff,ff,ff)\" clrtextnoactive=\"rgba(00,00,ff,ff)\" "\
					L"font=\"face:宋体,size:10,weight:400,charset:0,underline:0,italic:1,strike:1\" "\
					L"fonthover=\"face:黑体,size:11,weight:401,underline:1,italic:1,strike:1\" skin=\"DM_Frame \" NCskin=\"Btn_Close \" "\
					L"clrbg=\"rgba(fc,fc,fc,fc)\" align=\"right\" clrnc=\"rgba(fd,fd,fd,fd)\" ncmargin=\"1,2,3,4\" cursor=\" sizewe\" "\
					L"skin1=\"DM_Frame\" />";
	
	int ulSize = 720;
	byte pBuf[720] = {0};
	UnicodeToUtf8(pWBuf,(PCHAR)pBuf,ulSize);
	DMXmlDocument doc;
	doc.LoadFromBuffer(pBuf, ulSize);
	DMXmlNode XmlNode = doc.Root();
	pStyle->InitDMData(XmlNode);

	wchar_t szId[MAX_PATH] = {0};
	pStyle->GetID(szId, MAX_PATH);
	EXPECT_EQ(0==_wcsicmp(szId,L"custom"),true); // 名字

	// 文本颜色
	DMColor clr[5];
	for (int i=0;i<5;i++)
	{
		EXPECT_EQ(DMSUCCEEDED(pStyle->GetTextColor(i,clr[i])),true);
	}
	EXPECT_EQ(0xff==clr[0].r&&0xff==clr[0].g&&0xff==clr[0].b&&0xff==clr[0].a,true);
	EXPECT_EQ(0xff==clr[1].r&&0x00==clr[1].g&&0xff==clr[1].b&&0xff==clr[1].a,true);
	EXPECT_EQ(0xff==clr[2].r&&0xff==clr[2].g&&0x00==clr[2].b&&0xff==clr[2].a,true);
	EXPECT_EQ(0x00==clr[3].r&&0xff==clr[3].g&&0xff==clr[3].b&&0xff==clr[3].a,true);
	EXPECT_EQ(0x00==clr[4].r&&0x00==clr[4].g&&0xff==clr[4].b&&0xff==clr[4].a,true);

	// 字体
	DMSmartPtrT<IDMFont> pFont[5];
	for (int i=0;i<5;i++)
	{
		EXPECT_EQ(DMSUCCEEDED(pStyle->GetTextFont(i,&pFont[i])),true);
	}

	EXPECT_EQ(0==_wcsicmp(L"宋体",pFont[0]->GetFaceName())&&-10==pFont[0]->GetLogFont()->lfHeight&&400==pFont[0]->GetLogFont()->lfWeight,true);
	EXPECT_EQ(0==_wcsicmp(L"黑体",pFont[1]->GetFaceName())&&-11==pFont[1]->GetLogFont()->lfHeight&&401==pFont[1]->GetLogFont()->lfWeight,true);

	// 背景色
	DMColor crBg,crNc;
	pStyle->GetBgColor(0,crBg);
	pStyle->GetNcColor(0,crNc);

	EXPECT_EQ(0xfc==crBg.r&&0xfc==crBg.g&&0xfc==crBg.b&&0xfc==crBg.a,true);
	EXPECT_EQ(0xfd==crNc.r&&0xfd==crNc.g&&0xfd==crNc.b&&0xfd==crNc.a,true);

	//DMSmartPtrT<IDMSkin> pSkin;
	//pStyle->GetBgSkin(&pSkin);
	//DMSmartPtrT<IDMSkin> pNcSkin;
	//pStyle->GetNcSkin(&pNcSkin);

	CRect NcRc;
	pStyle->GetNcMargin(NcRc);

	HCURSOR hCursor = NULL;
	pStyle->GetCursor(hCursor);// size光标是空的
	EXPECT_EQ(NULL!=hCursor,true);

	UINT Align = 0;
	pStyle->GetTextAlign(Align);
}