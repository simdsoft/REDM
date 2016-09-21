//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMImgDecoder.h 
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

namespace DM
{
	/// <summary>
	///		参考WIC，图片解码后可能有多帧(Frame)
	/// <summary>
	class IDMImgFrame:public DMRefNum
	{
	public:
		virtual DMCode GetSize(UINT &ulWid,UINT &ulHei) = 0;		///< 帧大小
		virtual DMCode SetDelay(UINT ulDelay) = 0;					///< 设置帧延迟
		virtual DMCode GetDelay(UINT &ulDelay) = 0;				    ///< 帧延迟,针对GIF等多帧

		/// -------------------------------------------------
		/// @brief			 复制像素阵列
		/// @param[int]		 cbStride  在32位DIB下为单页,所以总是宽*4
		/// @param[int]		 lpRect    要复制的大小,为NULL表示复制整帧
		/// @return DM_ECODE_OK为成功
		virtual DMCode CopyPixels(LPVOID lpBuf, UINT ulSize,UINT cbStride,const RECT *lpRect=NULL) = 0;
	};

	/// <summary>
	///		资源打包对外扩展接口,classtype=<see cref="DMREG_ImgDecoder"/>
	/// </summary>
	class IDMImgDecoder:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMImgDecoder,L"IDMImgDecoder",DMREG_ImgDecoder);
	public:
		virtual ~IDMImgDecoder(){};

		virtual DMCode LoadFromMemory(void *pBuf,size_t bufLen) = 0;		///< 从内存中加载
		virtual DMCode LoadFromFile(LPCWSTR pszFileName) = 0;				///< 从文件中加载
		virtual DMCode GetFrameCount(UINT &ulCount) = 0;					///< 获得当前存储帧的数目

		/// -------------------------------------------------
		/// @brief 返回图片解码后的多帧循环次数
		/// @param[out]		 ulCount    图片解码后的多帧循环次数(-1表示永远循环)
		/// @remark 按GIF的逻辑,三种状态，1.没有循环信息（1次） 2.有循环信息（次数）3.有循环信息（0次表示永远循环）
		/// @return DM_ECODE_OK为成功
		virtual DMCode GetTotalLoopCount(UINT &ulCount) = 0;	

		/// -------------------------------------------------
		/// @brief 返回帧对象
		/// @param[in]  iFrame		要获取的帧index
		/// @param[out] ppFrame		要获取的帧
		/// @remark 内部会自己增加引用计数,外部建议使用智能指针
		/// @return DM_ECODE_OK为成功
		virtual DMCode GetFrame(UINT iFrame, IDMImgFrame**ppFrame) = 0;   
	};

}//namespace DM