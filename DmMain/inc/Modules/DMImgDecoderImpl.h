//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMImgDecoderImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-20	1.0			
//-------------------------------------------------------
#pragma once
#include <wincodec.h>
#pragma comment(lib, "WindowsCodecs.lib")

namespace DM
{
	class DMImgFrameImpl:public IDMImgFrame
	{
	public:
		DMImgFrameImpl(IWICBitmapSource* pBmpSource,UINT uFrameDelay=0);
	public:
		virtual DMCode GetSize(UINT &ulWid,UINT &ulHei);
		virtual DMCode SetDelay(UINT ulDelay);
		virtual DMCode GetDelay(UINT &ulDelay);
		virtual DMCode CopyPixels(LPVOID lpBuf, UINT ulSize,UINT cbStride,const RECT *lpRect=NULL);

	protected:	// 辅助
		DMSmartPtrT<IWICBitmapSource>    m_pBmpSource;
		UINT							 m_uFrameDelay;// 多帧时使用（如gif)
	};

	typedef DMImgFrameImpl*   DMImgFrameImplPtr;
	class DMImgDecoderImpl:public IDMImgDecoder
						   ,public DMArrayT<DMImgFrameImplPtr>
	{
		DMDECLARE_CLASS_NAME(DMImgDecoderImpl,L"DMImgDecoderImpl",DMREG_ImgDecoder);
	public:
		DMImgDecoderImpl();
		virtual~DMImgDecoderImpl();
	public:
		virtual DMCode LoadFromMemory(void *pBuf,size_t bufLen);	
		virtual DMCode LoadFromFile(LPCWSTR pszFileName);				
		virtual DMCode GetFrameCount(UINT &ulCount);	
		virtual DMCode GetTotalLoopCount(UINT &ulCount);		
		virtual DMCode GetFrame(UINT iFrame, IDMImgFrame**ppFrame);  

	public:
		virtual void PreArrayObjRemove(const DMImgFrameImplPtr &obj);

	protected:// 辅助
		 bool DoDecode(IWICBitmapDecoder* pBmpDecoder);
		 bool ParseFrame(IWICBitmapDecoder* pBmpDecoder,unsigned int iFrame);

		 UINT GetFrameDelay(IWICBitmapFrameDecode* pBmpFrameDecoder);
		 UINT GetTotalLoopCount(IWICBitmapDecoder* pBmpDecoder);// 这是全局数据，不是帧数据

		 bool CheckFramesValid();
		 void RepairFrames(void *pBuf,size_t bufLen);	 // 在XP下需要修复
		 void RepairFrames(LPCWSTR pszFileName);


	protected:
		static  DMSmartPtrT<IWICImagingFactory>  ms_ImgFactory;
		UINT									 m_uTotalLoopCount;
		UINT									 m_ulFrameCount;
	};




}//namespace DM