//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMGifParse.h 
// File Des: 解析GIF89a，按格式解析，由于wic在xp下无法解析出延迟时间，所以加入此功能，跳过了实际像素解析,本人不保证解析绝对准确！仅通过gtest测试
// File Summary: 参考结构:http://blog.csdn.net/friendwaters/article/details/2737328 & gif官方89a文档
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-21	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	typedef struct               //图象扩展参数
	{   
		bool active;             //本结构中的其它参数是否可用
		bool userInputFlag;      //是否期待用户输入
		bool trsFlag;            //是否有透明色
		WORD delayTime;          //延时时间（单位1/100秒）
		UINT disposalMethod;     //处理方法（见gif89a.doc，可忽略）
		UINT trsColorIndex;      //透明色调色板索引
	}GCTRLEXT;

	typedef struct                //GIF文件的全局参数
	{  
		UINT frames;             //文件中图象帧数
		WORD scrWidth,scrHeight; //逻辑屏幕的宽度和高度（单位像素）
		bool gFlag;              //是否有全局调色板（决定其他调色板参数是否有效）
		UINT colorRes;           //色彩分辨率（不使用）
		bool gSort;              //全局调色板是否按优先排序
		UINT gSize;              //全局调色板大小（有多少个实际入口）
		UINT BKColorIdx;         //背景色的调色板索引
		UINT pixelAspectRatio;   //像素长宽比例
		UINT totalLoopCount;     //loop次数,0表示永久
		bool bLoop;              //是否存在loop信息，没有就是一次
		//BYTE *gColorTable;     //指向全局调色板的指针（256个入口，每个入口三字节）
		//调色板格式请参看gif89a.doc
	}GIFGLOBALINFO;
	typedef GIFGLOBALINFO *LPGLOBAL_INFO;

	typedef struct  
	{
		WORD imageLPos;          //图象左边沿到逻辑屏幕的距离（单位像素）
		WORD imageTPos;          //图象上边沿到逻辑屏幕的距离（单位像素）
		WORD imageWidth;         //图象的宽度（单位像素）
		WORD imageHeight;        //图象的高度（单位像素）
		GCTRLEXT ctrlExt;        //图象扩展参数（与透明背景和动画有关）
	}GIFFRAME,*GIFFRAMEPTR;

	/// <summary>
	/// 解析gif，主要是获取XP下的延迟时间，xp下使用wic可获得帧，但无法取得延迟时间
	/// </summary>
	class DM_EXPORT DMGifParse:public DMRefNum
							  ,public DMArrayT<GIFFRAMEPTR>
	{
	public:
		DMGifParse();
		virtual~DMGifParse();

	public:
		/// -------------------------------------------------
		/// @brief 从文件中解析gif
		/// @param[in]		 lpszFilePath    gif的路径
		/// @return true:解析成功
		bool LoadFromFile(LPCWSTR lpszFilePath);

		/// -------------------------------------------------
		/// @brief 从文件中解析gif
		/// @param[in]		 pBuf			gif的buf起始
		/// @param[in]		 bufLen			gif的buf大小
		/// @return true:解析成功
		bool LoadFromMemory(BYTE *pBuf,int bufLen);

		/// -------------------------------------------------
		/// @brief 取得指定帧
		/// @param[in]		 iElement		指定帧号
		/// @return 失败返回null
		GIFFRAMEPTR GetFrame(int iElement);

		/// -------------------------------------------------
		/// @brief 取得全局值，全局值包含了图形帧数、循环次数等
		/// @return 
		LPGLOBAL_INFO GetGlobalInfo(){return &m_gInfo;}

	public:// 辅助
		bool GetAllFrames(FILE*fp);
		bool ParseExtension(FILE*fp,GCTRLEXT&ctrlExt);
		bool ParseFrame(FILE*fp);
		
		bool GetAllFrames(BYTE *pBuf,int bufLen);
		bool ParseExtension(BYTE *pBuf,int bufLen,GCTRLEXT&ctrlExt);
		bool ParseFrame(BYTE *pBuf,int bufLen);

	public:
		virtual void PreArrayObjRemove(const GIFFRAMEPTR &obj);
	private:
		int                 m_nOffset;              ///< 用于内存解析全局
		GIFGLOBALINFO		m_gInfo;				///< GIF文件的全局参数
		GCTRLEXT		    m_ctrlExt;				///< 图象扩展参数（读入数据时临时使用）
	};

}//namespace DM
