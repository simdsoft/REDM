//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMSkin_Test.cpp 
// File Des: 测试皮肤绘制类
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

class DMSkinTest:public::testing::Test
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
};

// imglist
TEST_F(DMSkinTest, imglist_Test)
{
	DMXmlDocument doc;
	EXPECT_EQ(doc.LoadFromFile(L"UTRes\\UTTest\\SkinTest.xml"), true);
	DMXmlNode XmlNode = doc.Root(L"skin");
	XmlNode = XmlNode.FirstChild(L"imglist");
	while (XmlNode.IsValid())
	{
		DMSmartPtrT<IDMSkin> m_pSkin;
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&m_pSkin, L"imglist",DMREG_Skin)),true);
		m_pSkin->InitDMData(XmlNode);
		wchar_t szId[MAX_PATH] = {0};
		m_pSkin->GetID(szId, MAX_PATH);		// 取得对象唯一ID名
		int iStates = 0;m_pSkin->GetStates(iStates); // 取得状态数
		SIZE sz;
		EXPECT_EQ(DMSUCCEEDED(m_pSkin->GetStateSize(sz)),true);

		DMSmartPtrT<IDMCanvas> pCanvas;
		EXPECT_EQ(DMSUCCEEDED(m_pDefRender->CreateCanvas(100,100,&pCanvas)),true);
		CRect rcDraw(0,0,100,100);
		m_pSkin->Draw(pCanvas,rcDraw,0);

#if SHOW_TOSCREEN
		DMAutoDC hdc;
		HDC dcMem = pCanvas->GetDC();
		BLENDFUNCTION bf = {AC_SRC_OVER,0,0xff,AC_SRC_ALPHA};
		bool bRet = ::AlphaBlend(hdc, 800, 0,100,100,dcMem, 0, 0,100,100,bf);
		pCanvas->ReleaseDC(dcMem);
#endif

		XmlNode = XmlNode.NextSibling(L"imglist");
	}	
}

// img9
TEST_F(DMSkinTest, img9_Test)
{
	DMXmlDocument doc;
	EXPECT_EQ(doc.LoadFromFile(L"UTRes\\UTTest\\SkinTest.xml"), true);
	DMXmlNode XmlNode = doc.Root(L"skin");
	XmlNode = XmlNode.FirstChild(L"img9");
	while (XmlNode.IsValid())
	{
		DMSmartPtrT<IDMSkin> m_pSkin;
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&m_pSkin, L"img9",DMREG_Skin)),true);
		m_pSkin->InitDMData(XmlNode);
		wchar_t szId[MAX_PATH] = {0};
		m_pSkin->GetID(szId, MAX_PATH);		// 取得对象唯一ID名
		int iStates = 0;m_pSkin->GetStates(iStates); // 取得状态数
		SIZE sz;
		EXPECT_EQ(DMSUCCEEDED(m_pSkin->GetStateSize(sz)),true);

		CRect rcMagin;
		m_pSkin->SendExpandInfo((WPARAM)&rcMagin,0);

		DMSmartPtrT<IDMCanvas> pCanvas;
		EXPECT_EQ(DMSUCCEEDED(m_pDefRender->CreateCanvas(100,100,&pCanvas)),true);
		CRect rcDraw(0,0,100,100);
		m_pSkin->Draw(pCanvas,rcDraw,1);

#if SHOW_TOSCREEN
		DMAutoDC hdc;
		HDC dcMem = pCanvas->GetDC();
		BLENDFUNCTION bf = {AC_SRC_OVER,0,0xff,AC_SRC_ALPHA};
		bool bRet = ::AlphaBlend(hdc, 901, 0,100,100,dcMem, 0, 0,100,100,bf);
		pCanvas->ReleaseDC(dcMem);
#endif
		XmlNode = XmlNode.NextSibling(L"img9");
	}	
}

// img9
TEST_F(DMSkinTest, scroll_Test)
{
	DMXmlDocument doc;
	EXPECT_EQ(doc.LoadFromFile(L"UTRes\\UTTest\\SkinTest.xml"), true);
	DMXmlNode XmlNode = doc.Root(L"skin");
	XmlNode = XmlNode.FirstChild(L"scrollbar");
	while (XmlNode.IsValid())
	{
		DMSmartPtrT<IDMSkin> m_pSkin;
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&m_pSkin, L"scrollbar",DMREG_Skin)),true);
		m_pSkin->InitDMData(XmlNode);

		DMSmartPtrT<IDMCanvas> pCanvas;
		EXPECT_EQ(DMSUCCEEDED(m_pDefRender->CreateCanvas(200,200,&pCanvas)),true);

	
		// normal 竖直----
		CSize sz(16,16);
		CRect rcArrowUp(CPoint(0,0),sz);CRect rcPage(0,17,16,64);CRect rcAllowDown(CPoint(0,65),sz);CRect rcThumb(0,25,16,50);
		int nState = DMSBST_NORMAL;
		for (nState=0;nState<5;nState++)
		{
			rcArrowUp.OffsetRect(17,0);rcPage.OffsetRect(17,0);rcAllowDown.OffsetRect(17,0);rcThumb.OffsetRect(17,0);
			m_pSkin->Draw(pCanvas,rcArrowUp,MAKESBSTATE(SB_LINEUP,nState,1));
			m_pSkin->Draw(pCanvas,rcPage,MAKESBSTATE(SB_PAGEUP,nState,1));
			m_pSkin->Draw(pCanvas,rcAllowDown,MAKESBSTATE(SB_LINEDOWN,nState,1));
			m_pSkin->Draw(pCanvas,rcThumb,MAKESBSTATE(SB_THUMBTRACK,nState,1));
		}
	
		// normal 水平----
		CRect rcArrowLeft(CPoint(102,-17),sz);CRect rcPageH(119,-17,183,-1);CRect rcAllowRight(184,-17,200,-1); CRect rcThumbH(123,-17,169,-1);              // 第一次会下移17，所以直接取-17
		nState = DMSBST_NORMAL;
		for (nState=0;nState<5;nState++)
		{
			rcArrowLeft.OffsetRect(0,17);rcPageH.OffsetRect(0,17);rcAllowRight.OffsetRect(0,17);rcThumbH.OffsetRect(0,17);
			m_pSkin->Draw(pCanvas,rcArrowLeft,MAKESBSTATE(SB_LINELEFT,nState,0));
			m_pSkin->Draw(pCanvas,rcPageH,MAKESBSTATE(SB_PAGELEFT,nState,0));
			m_pSkin->Draw(pCanvas,rcAllowRight,MAKESBSTATE(SB_LINERIGHT,nState,0));
			m_pSkin->Draw(pCanvas,rcThumbH,MAKESBSTATE(SB_THUMBTRACK,nState,0));
		}

#if  SHOW_TOSCREEN
		DMAutoDC hdc;
		HDC dcMem = pCanvas->GetDC();
		BLENDFUNCTION bf = {AC_SRC_OVER,0,0xff,AC_SRC_ALPHA};
		bool bRet = ::AlphaBlend(hdc, 1001, 0,200,200,dcMem, 0, 0,200,200,bf);
		pCanvas->ReleaseDC(dcMem);
#endif

		XmlNode = XmlNode.NextSibling(L"scrollbar");
	}	
}