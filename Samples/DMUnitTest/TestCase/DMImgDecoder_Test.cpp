//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMImgDecoder_Test.cpp 
// File Des: 测试图形解码器的测试用例
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-20	1.0			
//-------------------------------------------------------
#pragma once
#include "DMUintTestAfx.h"

class ImgDecoderTest:public::testing::Test
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
protected:
	HBITMAP CreateGDIBitmap( int nWid,int nHei,void ** ppBits)
	{
		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth       = nWid;
		bmi.bmiHeader.biHeight      = -nHei; // top-down image 
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biBitCount    = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage   = 0;

		LPVOID pBits = NULL;
		HBITMAP hBmp = CreateDIBSection(NULL,&bmi,DIB_RGB_COLORS,ppBits,0,0);
		return hBmp;
	}

	UINT GetStride(const UINT width, const UINT bitCount/*=32*/) 
	{
		//ASSERT(0 == bitCount % 8);
		const UINT byteCount = bitCount / 8;
		const UINT stride = (width * byteCount + 3) & ~3;
		//ASSERT(0 == stride % sizeof(DWORD));
		return stride;
	}

protected:
	HBITMAP                        m_hBmp;
	void*                          pBits;//像素阵列
};

//---------------------------------------------------------------------
// 测试内存加载接口是否OK
TEST_F(ImgDecoderTest, LoadFromMemoryIsSuccess_PNG)
{
	wchar_t szExeDir[MAX_PATH] = {0};
	GetRootDirW(szExeDir, MAX_PATH);
	wchar_t szPath[MAX_PATH] = {0};
	PathCombineW(szPath, szExeDir, L".\\UTRes\\UTTest\\1.png");

	DWORD dwSize = DM::GetFileSizeW(szPath);
	EXPECT_EQ(dwSize!=0, true);
	PBYTE pBuf = new BYTE[dwSize];
	DWORD dwRead;
	GetFileBufW(szPath, (void**)&pBuf, dwSize,dwRead);
	EXPECT_EQ(dwRead == dwSize, true);

	LPCWSTR lpDefName = g_pDMApp->GetDefRegObj(DMREG_ImgDecoder);
	printf("lpDefName:%ws\n",lpDefName);
	DMSmartPtrT<IDMImgDecoder> pImgDecoder;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,lpDefName,DMREG_ImgDecoder)),true);

	EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromMemory(pBuf, dwSize)), true);

	DMSmartPtrT<IDMImgFrame> pFrame;
	pImgDecoder->GetFrame(0, &pFrame);//png肯定只有一帧，所以传0
	EXPECT_EQ(pFrame!=NULL, true);
	UINT width = 0;
	UINT height = 0;
	EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);
	m_hBmp = CreateGDIBitmap(width,height,&pBits);
	const int stride = GetStride(width, 32);
	EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(pBits, stride*height,stride)), true);

#if SHOW_TOSCREEN//绘制到屏幕
	HDC dc = GetDC(NULL);
	HDC dcMem = CreateCompatibleDC(dc);
	SelectObject(dcMem,m_hBmp);
	StretchBlt(dc, 0, 0, width, height, dcMem,0, 0, width, height, SRCCOPY);//绘制到屏幕
#endif
	DM_DELETE_ARRAY(pBuf);
}


// 测试文件加载接口是否OK
TEST_F(ImgDecoderTest, LoadFromFileIsSuccess_PNG)
{
	LPCWSTR lpDefName = g_pDMApp->GetDefRegObj(DMREG_ImgDecoder);
	printf("lpDefName:%ws\n",lpDefName);
	DMSmartPtrT<IDMImgDecoder> pImgDecoder;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,lpDefName,DMREG_ImgDecoder)),true);
	EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromFile(L"UTRes\\UTTest\\1.png")), true);

	
	DMSmartPtrT<IDMImgFrame> pFrame;
	pImgDecoder->GetFrame(0, &pFrame);//png肯定只有一帧，所以传0
	EXPECT_EQ(pFrame!=NULL, true);
	UINT width = 0;
	UINT height = 0;
	EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);
	m_hBmp = CreateGDIBitmap(width,height,&pBits);
	const int stride = GetStride(width, 32);
	EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(pBits, stride*height,stride)), true);

#if SHOW_TOSCREEN//绘制到屏幕
	HDC dc = GetDC(NULL);
	HDC dcMem = CreateCompatibleDC(dc);
	SelectObject(dcMem,m_hBmp);
	StretchBlt(dc, 0, 50, width, height, dcMem,0, 0, width, height, SRCCOPY);//绘制到屏幕
#endif

}

//---------------------------------------------------------------------
// 测试内存加载接口是否OK
TEST_F(ImgDecoderTest, LoadFromMemoryIsSuccess_JPG)
{
	wchar_t szExeDir[MAX_PATH] = {0};
	GetRootDirW(szExeDir, MAX_PATH);
	wchar_t szPath[MAX_PATH] = {0};
	PathCombineW(szPath, szExeDir, L".\\UTRes\\UTTest\\1.jpg");

	DWORD dwSize = DM::GetFileSizeW(szPath);
	EXPECT_EQ(dwSize!=0, true);
	PBYTE pBuf = new BYTE[dwSize];
	DWORD dwRead;
	GetFileBufW(szPath, (void**)&pBuf, dwSize,dwRead);
	EXPECT_EQ(dwRead == dwSize, true);

	LPCWSTR lpDefName = g_pDMApp->GetDefRegObj(DMREG_ImgDecoder);
	printf("lpDefName:%ws\n",lpDefName);
	DMSmartPtrT<IDMImgDecoder> pImgDecoder;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,lpDefName,DMREG_ImgDecoder)),true);

	EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromMemory(pBuf, dwSize)), true);

	DMSmartPtrT<IDMImgFrame> pFrame;
	pImgDecoder->GetFrame(0, &pFrame);//jpg肯定只有一帧，所以传0
	EXPECT_EQ(pFrame!=NULL, true);
	UINT width = 0;
	UINT height = 0;
	EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);
	m_hBmp = CreateGDIBitmap(width,height,&pBits);
	const int stride = GetStride(width, 32);
	EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(pBits, stride*height,stride)), true);

#if SHOW_TOSCREEN//绘制到屏幕
	HDC dc = GetDC(NULL);
	HDC dcMem = CreateCompatibleDC(dc);
	SelectObject(dcMem,m_hBmp);
	StretchBlt(dc, 100, 0, width, height, dcMem,0, 0, width, height, SRCCOPY);//绘制到屏幕
#endif
	DM_DELETE_ARRAY(pBuf);
}


// 测试文件加载接口是否OK
TEST_F(ImgDecoderTest, LoadFromFileIsSuccess_JPG)
{
	LPCWSTR lpDefName = g_pDMApp->GetDefRegObj(DMREG_ImgDecoder);
	printf("lpDefName:%ws\n",lpDefName);
	DMSmartPtrT<IDMImgDecoder> pImgDecoder;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,lpDefName,DMREG_ImgDecoder)),true);
	EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromFile(L"UTRes\\UTTest\\1.jpg")), true);


	DMSmartPtrT<IDMImgFrame> pFrame;
	pImgDecoder->GetFrame(0, &pFrame);//jpg肯定只有一帧，所以传0
	EXPECT_EQ(pFrame!=NULL, true);
	UINT width = 0;
	UINT height = 0;
	EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);
	m_hBmp = CreateGDIBitmap(width,height,&pBits);
	const int stride = GetStride(width, 32);
	EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(pBits, stride*height,stride)), true);

#if SHOW_TOSCREEN//绘制到屏幕
	HDC dc = GetDC(NULL);
	HDC dcMem = CreateCompatibleDC(dc);
	SelectObject(dcMem,m_hBmp);
	StretchBlt(dc, 100, 50, width, height, dcMem,0, 0, width, height, SRCCOPY);//绘制到屏幕
#endif

}

//---------------------------------------------------------------------
// 测试内存加载接口是否OK
TEST_F(ImgDecoderTest, LoadFromMemoryIsSuccess_BMP)
{
	wchar_t szExeDir[MAX_PATH] = {0};
	GetRootDirW(szExeDir, MAX_PATH);
	wchar_t szPath[MAX_PATH] = {0};
	PathCombineW(szPath, szExeDir, L".\\UTRes\\UTTest\\1.bmp");

	DWORD dwSize = DM::GetFileSizeW(szPath);
	EXPECT_EQ(dwSize!=0, true);
	PBYTE pBuf = new BYTE[dwSize];
	DWORD dwRead;
	GetFileBufW(szPath, (void**)&pBuf, dwSize,dwRead);
	EXPECT_EQ(dwRead == dwSize, true);

	LPCWSTR lpDefName = g_pDMApp->GetDefRegObj(DMREG_ImgDecoder);
	printf("lpDefName:%ws\n",lpDefName);
	DMSmartPtrT<IDMImgDecoder> pImgDecoder;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,lpDefName,DMREG_ImgDecoder)),true);

	EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromMemory(pBuf, dwSize)), true);

	DMSmartPtrT<IDMImgFrame> pFrame;
	pImgDecoder->GetFrame(0, &pFrame);//bmp肯定只有一帧，所以传0
	EXPECT_EQ(pFrame!=NULL, true);
	UINT width = 0;
	UINT height = 0;
	EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);
	m_hBmp = CreateGDIBitmap(width,height,&pBits);
	const int stride = GetStride(width, 32);
	EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(pBits, stride*height,stride)), true);

#if SHOW_TOSCREEN//绘制到屏幕
	HDC dc = GetDC(NULL);
	HDC dcMem = CreateCompatibleDC(dc);
	SelectObject(dcMem,m_hBmp);
	StretchBlt(dc, 200, 0, width, height, dcMem,0, 0, width, height, SRCCOPY);//绘制到屏幕
#endif
	DM_DELETE_ARRAY(pBuf);
}


// 测试文件加载接口是否OK
TEST_F(ImgDecoderTest, LoadFromFileIsSuccess_BMP)
{
	LPCWSTR lpDefName = g_pDMApp->GetDefRegObj(DMREG_ImgDecoder);
	printf("lpDefName:%ws\n",lpDefName);
	DMSmartPtrT<IDMImgDecoder> pImgDecoder;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,lpDefName,DMREG_ImgDecoder)),true);
	EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromFile(L"UTRes\\UTTest\\1.bmp")), true);


	DMSmartPtrT<IDMImgFrame> pFrame;
	pImgDecoder->GetFrame(0, &pFrame);//bmp肯定只有一帧，所以传0
	EXPECT_EQ(pFrame!=NULL, true);
	UINT width = 0;
	UINT height = 0;
	EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);
	m_hBmp = CreateGDIBitmap(width,height,&pBits);
	const int stride = GetStride(width, 32);
	EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(pBits, stride*height,stride)), true);

#if SHOW_TOSCREEN//绘制到屏幕
	HDC dc = GetDC(NULL);
	HDC dcMem = CreateCompatibleDC(dc);
	SelectObject(dcMem,m_hBmp);
	StretchBlt(dc, 200, 50, width, height, dcMem,0, 0, width, height, SRCCOPY);//绘制到屏幕
#endif

}

//---------------------------------------------------------------------
// 测试内存加载接口是否OK
TEST_F(ImgDecoderTest,LoadFromMemoryIsSuccess_GIF)
{
	wchar_t szExeDir[MAX_PATH] = {0};
	GetRootDirW(szExeDir, MAX_PATH);
	wchar_t szPath[MAX_PATH] = {0};
	PathCombineW(szPath, szExeDir, L".\\UTRes\\UTTest\\1.gif");

	DWORD dwSize = DM::GetFileSizeW(szPath);
	EXPECT_EQ(dwSize!=0, true);
	PBYTE pBuf = new BYTE[dwSize];
	DWORD dwRead;
	GetFileBufW(szPath, (void**)&pBuf, dwSize,dwRead);
	EXPECT_EQ(dwRead == dwSize, true);

	LPCWSTR lpDefName = g_pDMApp->GetDefRegObj(DMREG_ImgDecoder);
	printf("lpDefName:%ws\n",lpDefName);
	DMSmartPtrT<IDMImgDecoder> pImgDecoder;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,lpDefName,DMREG_ImgDecoder)),true);

	EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromMemory(pBuf, dwSize)), true);


	// 简单循环播放
	UINT ulFrameCount = 0;
	pImgDecoder->GetFrameCount(ulFrameCount);
	printf("ImgDecoderTest-LoadFromFileIsSuccess_GIF-ulFrameCount:%d\n",ulFrameCount);
	UINT ulTotalLoopCount = INFINITE;
	pImgDecoder->GetTotalLoopCount(ulTotalLoopCount);
	printf("ImgDecoderTest-LoadFromFileIsSuccess_GIF-ulTotalLoopCount:%d\n",ulTotalLoopCount);
	EXPECT_EQ(0!=ulFrameCount,true);
#if SHOW_TOSCREEN//绘制到屏幕
	HDC dc = GetDC(NULL);
	HDC dcMem = CreateCompatibleDC(dc);
#endif
	for (int i=0;i<(int)ulFrameCount;i++)
	{
		DMSmartPtrT<IDMImgFrame> pFrame;
		EXPECT_EQ(DMSUCCEEDED(pImgDecoder->GetFrame(i, &pFrame)), true);

		UINT ulDelay = 0;
		pFrame->GetDelay(ulDelay);
		EXPECT_EQ(0!=ulDelay,true);
		printf("ImgDecoderTest-LoadFromFileIsSuccess_GIF-%i 帧的延迟为:%d ms\n",i,ulDelay);

		UINT width = 0;
		UINT height = 0;
		EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);

		m_hBmp = CreateGDIBitmap(width,height,&pBits);
		const int stride = GetStride(width, 32);
		EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(pBits, stride*height,stride)), true);

#if SHOW_TOSCREEN//绘制到屏幕
		SelectObject(dcMem,m_hBmp);
		StretchBlt(dc, 300, 0, width, height, dcMem,0, 0, width, height, SRCCOPY);//绘制到屏幕
		Sleep(ulDelay);
#endif
	}

	DM_DELETE_ARRAY(pBuf);
}


TEST_F(ImgDecoderTest, LoadFromFileIsSuccess_GIF)
{
	LPCWSTR lpDefName = g_pDMApp->GetDefRegObj(DMREG_ImgDecoder);
	printf("lpDefName:%ws\n",lpDefName);
	DMSmartPtrT<IDMImgDecoder> pImgDecoder;
	EXPECT_EQ(DMSUCCEEDED(g_pDMApp->CreateRegObj((void**)&pImgDecoder,lpDefName,DMREG_ImgDecoder)),true);
	EXPECT_EQ(DMSUCCEEDED(pImgDecoder->LoadFromFile(L"UTRes\\UTTest\\1.gif")), true);


	// 简单循环播放
	UINT ulFrameCount = 0;
	pImgDecoder->GetFrameCount(ulFrameCount);
	printf("ImgDecoderTest-LoadFromMemoryIsSuccess_GIF-ulFrameCount:%d\n",ulFrameCount);
	UINT ulTotalLoopCount = INFINITE;
	pImgDecoder->GetTotalLoopCount(ulTotalLoopCount);
	printf("ImgDecoderTest-LoadFromMemoryIsSuccess_GIF-ulTotalLoopCount:%d\n",ulTotalLoopCount);
	EXPECT_EQ(0!=ulFrameCount,true);
#if SHOW_TOSCREEN//绘制到屏幕
	HDC dc = GetDC(NULL);
	HDC dcMem = CreateCompatibleDC(dc);
#endif

	for (int i=0;i<(int)ulFrameCount;i++)
	{
		DMSmartPtrT<IDMImgFrame> pFrame;
		EXPECT_EQ(DMSUCCEEDED(pImgDecoder->GetFrame(i, &pFrame)), true);

		UINT ulDelay = 0;
		pFrame->GetDelay(ulDelay);
		EXPECT_EQ(0!=ulDelay,true);
		printf("ImgDecoderTest-LoadFromMemoryIsSuccess_GIF-%i 帧的延迟为:%d ms\n",i,ulDelay);

		UINT width = 0;
		UINT height = 0;
		EXPECT_EQ(DMSUCCEEDED(pFrame->GetSize(width, height)), true);

		m_hBmp = CreateGDIBitmap(width,height,&pBits);
		const int stride = GetStride(width, 32);
		EXPECT_EQ(DMSUCCEEDED(pFrame->CopyPixels(pBits, stride*height,stride)), true);

#if SHOW_TOSCREEN//绘制到屏幕
		SelectObject(dcMem,m_hBmp);
		StretchBlt(dc, 300, 50, width, height, dcMem,0, 0, width, height, SRCCOPY);//绘制到屏幕
		Sleep(ulDelay);
#endif
	}
}