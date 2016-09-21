//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMGifParse_Test.cpp 
// File Des: 测试gif解析的测试用例
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
#include "DMGifParse.h"

class DMGifParseTest:public::testing::Test
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

TEST_F(DMGifParseTest, 测试GifParse文件加载功能)
{
	wchar_t szExeDir[MAX_PATH] = {0};
	GetRootDirW(szExeDir, MAX_PATH);
	wchar_t szPath[MAX_PATH] = {0};
	PathCombineW(szPath, szExeDir, L".\\UTRes\\UTTest\\1.gif");
	DMSmartPtrT<DMGifParse> pObj;
	pObj.Attach(new DMGifParse);
	EXPECT_EQ(pObj->LoadFromFile(szPath),true);
	LPGLOBAL_INFO gi = pObj->GetGlobalInfo();
	printf("测试GifParse文件加载功能----------------------------------\n");
	printf("逻辑屏幕宽度=%d,高度=%d\n",gi->scrWidth,gi->scrHeight);
	if (gi->bLoop)
	{
		printf("循环次数=%d\n",gi->totalLoopCount);
	}
	else
	{
		printf("循环次数1次\n");
	}
	for (int i=0;i<(int)gi->frames;i++)
	{
		GIFFRAMEPTR fm = pObj->GetFrame(i);
		EXPECT_EQ(NULL!=fm, true);

		printf("第%d帧,宽度:%d,高度:%d,left:%d,top:%d\n",i+1,fm->imageWidth,fm->imageHeight,fm->imageLPos,fm->imageTPos);
		if (fm->ctrlExt.active)
			printf("第%d帧延时（毫秒）=%d,透明色:%d,等待用户输入:%d\n",i+1,fm->ctrlExt.delayTime*10,fm->ctrlExt.trsFlag,fm->ctrlExt.userInputFlag);
	}

	printf("测试GifParse文件加载功能----------------------------------\n");
}

TEST_F(DMGifParseTest, 测试GifParse内存加载功能)
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


	DMSmartPtrT<DMGifParse> pObj;
	pObj.Attach(new DMGifParse);
	EXPECT_EQ(pObj->LoadFromMemory(pBuf,dwSize),true);
	LPGLOBAL_INFO gi = pObj->GetGlobalInfo();
	printf("测试GifParse内存加载功能----------------------------------\n");
	printf("逻辑屏幕宽度=%d,高度=%d\n",gi->scrWidth,gi->scrHeight);
	if (gi->bLoop)
	{
		printf("循环次数=%d\n",gi->totalLoopCount);
	}
	else
	{
		printf("循环次数1次\n");
	}
	for (int i=0;i<(int)gi->frames;i++)
	{
		GIFFRAMEPTR fm = pObj->GetFrame(i);
		EXPECT_EQ(NULL!=fm, true);

		printf("第%d帧,宽度:%d,高度:%d,left:%d,top:%d\n",i+1,fm->imageWidth,fm->imageHeight,fm->imageLPos,fm->imageTPos);
		if (fm->ctrlExt.active)
			printf("第%d帧延时（毫秒）=%d,透明色:%d,等待用户输入:%d\n",i+1,fm->ctrlExt.delayTime*10,fm->ctrlExt.trsFlag,fm->ctrlExt.userInputFlag);
	}

	printf("测试GifParse内存加载功能----------------------------------\n");

	DM_DELETE_ARRAY(pBuf);
}