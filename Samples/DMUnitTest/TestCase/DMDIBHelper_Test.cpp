//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMDIBHelper_Test.cpp 
// File Des: 测试DIB变换的测试用例
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-10-17	1.0			
//-------------------------------------------------------
#pragma once 
#include "DMUintTestAfx.h"
#include "DMDIBHelper.h"

class DMDibHelperTest:public::testing::Test
{
public:
	virtual void SetUp()
	{
		// 
		DMSmartPtrT<IDMImgDecoder> pImgDecoder;
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,NULL,DMREG_ImgDecoder)),true);
		EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromFile(L"UTRes\\UTTest\\red-alpha50.png")), true);
		
		DMSmartPtrT<IDMImgFrame> pFrame;
		pImgDecoder->GetFrame(0, &pFrame);//jpg肯定只有一帧，所以传0
		EXPECT_EQ(pFrame!=NULL, true);
		UINT width = 0;
		UINT height = 0;
		EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);
		
		DMAutoDC hdc;
		EXPECT_EQ(NULL!=m_pHelper->CreateDIBSection(hdc, width, height),true);
		

		EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(m_pHelper->m_pPixelBits, m_pHelper->m_nImageSize,m_pHelper->m_nBPS)),true);

		//-------------------------------------------
		pImgDecoder.Release();
		EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,NULL,DMREG_ImgDecoder)),true);
		EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromFile(L"UTRes\\UTTest\\gre-alpha50.png")), true);

		pFrame.Release();
		pImgDecoder->GetFrame(0, &pFrame);//jpg肯定只有一帧，所以传0
		EXPECT_EQ(pFrame!=NULL, true);
		EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);

		EXPECT_EQ(NULL!=m_pHelperG->CreateDIBSection(hdc, width, height),true);

		EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(m_pHelperG->m_pPixelBits, m_pHelperG->m_nImageSize,m_pHelperG->m_nBPS)),true);
	}

	virtual void TearDown()
	{
		m_pHelper->DIBRelease();
		m_pHelperG->DIBRelease();
	}

public:
	DMLazyT<DMDIBHelper>  m_pHelper;
	DMLazyT<DMDIBHelper>  m_pHelperG;
};

TEST_F(DMDibHelperTest, 测试DIB功能)
{
	TearDown();
	SetUp();
	// HSL变换-------------------------------------------
	EXPECT_EQ(m_pHelper->AdjustHSL32(150, 100, 100),true);
	wchar_t *pszSaveFile = L".\\UTRes\\UTTest\\red-alpha50-hsl.bmp";
	wchar_t szPath[MAX_PATH]={0};
	GetRootFullPath(pszSaveFile, szPath,MAX_PATH);
	EXPECT_EQ(m_pHelper->SaveFile(szPath),true);

	// 灰度图-------------------------------------------
	EXPECT_EQ(m_pHelper->GreyImage(),true);
	pszSaveFile = L".\\UTRes\\UTTest\\red-alpha50-gray.bmp";
	GetRootFullPath(pszSaveFile, szPath,MAX_PATH);
	EXPECT_EQ(m_pHelper->SaveFile(szPath),true);

	// 恢复原图-------------------------------------------
	EXPECT_EQ(m_pHelper->ResetHSL32(), true);
	pszSaveFile = L".\\UTRes\\UTTest\\red-alpha50-org.bmp";
	GetRootFullPath(pszSaveFile, szPath,MAX_PATH);
	EXPECT_EQ(m_pHelper->SaveFile(szPath),true);
}


TEST_F(DMDibHelperTest, 测试DIB的存取像素)
{
	TearDown();
	SetUp();

	 // 测试像素存取---------------------------------
	DMColor clr = m_pHelper->GetPixel(10,11);
	EXPECT_EQ(0x80==clr.r, true);
	clr.a = 0x90;clr.r = 0xc0;
	m_pHelper->SetPixel(10,11,clr);
	clr = m_pHelper->GetPixel(10,11);
	EXPECT_EQ(0xc0==clr.r, true);

	// 测试alpha存取---------------------------------
	BYTE alpha = m_pHelper->GetAlpha(10,11);
	EXPECT_EQ(0x90==alpha, true);
	m_pHelper->SetAlpha(10,1,0x80);   // 预乘可能有像素损失
}


TEST_F(DMDibHelperTest, 测试GDI函数Alpha通道清0备份)
{
	TearDown();
	SetUp();

   // 测试Alpha通道备份---------------------------------
	DWORD dwTime1 = GetTickCount();
	for (int i=0;i<20;i++)
	{
		m_pHelper->AlphaBackup(NULL);
		m_pHelper->AlphaRestore();
	}
	DWORD dwTime2 = GetTickCount()-dwTime1;
	printf("AlphaBackup+AlphaRestore time:%dms-------------------------------------------\n",dwTime2);

	// 测试FillRect是否引起Alpha通道清0------------------
	DMAutoDC hdc;
	HDC dcMem = ::CreateCompatibleDC(hdc);
	::SelectObject(dcMem, m_pHelper->m_hBitmap);

	CRect rcDest(0,0,m_pHelper->m_nWidth,m_pHelper->m_nHeight);
	for (int i=3; i+3<m_pHelper->m_nImageSize; i+=4)
	{
		EXPECT_EQ(0x80==(DWORD)(byte)m_pHelper->m_pPixelBits[i],true);// 测试之前，所有的alpha都是0x80
	}
	rcDest.DeflateRect(20,8);
	m_pHelper->AlphaBackup(rcDest);
	::FillRect(dcMem,&rcDest,(HBRUSH)::GetStockObject(WHITE_BRUSH));

#if 0// 此测试只在rcDest为全区域时开启
	for (int i=3; i+3<m_pHelper->m_nImageSize; i+=4)
	{
		EXPECT_EQ(0==(DWORD)(byte)m_pHelper->m_pPixelBits[i],true);	  // FillRect操作后，所有的alpha都是0x0
	}
#endif
	m_pHelper->AlphaRestore();

#if SHOW_TOSCREEN
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA};
	bool bRet = ::AlphaBlend(hdc, 400, 0,100,50,dcMem, 0, 0,m_pHelper->m_nWidth,m_pHelper->m_nHeight,bf);// 只有Alpha能绘出透明效果
#endif
	::DeleteDC(dcMem);
}


TEST_F(DMDibHelperTest, 测试GDI函数Alpha通道混合)
{
	TearDown();
	SetUp();

	DMAutoDC hdc;
	HDC dcMem = ::CreateCompatibleDC(hdc);
	::SelectObject(dcMem, m_pHelper->m_hBitmap);
	HDC dcMem1 = ::CreateCompatibleDC(hdc);
	::SelectObject(dcMem1, m_pHelperG->m_hBitmap);
	BLENDFUNCTION bf = { AC_SRC_OVER,0,0xcf,AC_SRC_ALPHA};
	DMDIBHelper*pDest = m_pHelperG.Get();DMDIBHelper*pSrc = m_pHelper.Get();
	DWORD dwt1 = GetTickCount();
	//for (int i=0;i<100;i++)
	//bool bRet = ::AlphaBlend(dcMem1, 0, 0,m_pHelper->m_nWidth,m_pHelper->m_nHeight,dcMem, 0, 0,m_pHelper->m_nWidth,m_pHelper->m_nHeight,bf);// 只有Alpha能绘出透明效果
	//DMDIBHelper::AlphaBlend32(pDest,0,0,m_pHelper->m_nWidth,m_pHelper->m_nHeight,pSrc, 10, 10,m_pHelper->m_nWidth,m_pHelper->m_nHeight,0Xcf);
	DWORD dwt2 = GetTickCount()-dwt1;
	printf("AlphaBlend time:%d==============\n",dwt2);
    
	//DMDIBHelper*pDest = m_pHelperG.Get();DMDIBHelper*pSrc = m_pHelper.Get();
	//DMDIBHelper::AlphaBlend32(pDest,0,0,m_pHelper->m_nWidth,m_pHelper->m_nHeight,pSrc, 0, 0,m_pHelper->m_nWidth,m_pHelper->m_nHeight,255);
#if SHOW_TOSCREEN
	BLENDFUNCTION bf1 = { AC_SRC_OVER,0,255,AC_SRC_ALPHA};
    ::AlphaBlend(hdc, 400, 0,100,50,dcMem1, 0, 0,m_pHelper->m_nWidth,m_pHelper->m_nHeight,bf1);// 只有Alpha能绘出透明效果
#endif
	::DeleteDC(dcMem);
	::DeleteDC(dcMem1);
}