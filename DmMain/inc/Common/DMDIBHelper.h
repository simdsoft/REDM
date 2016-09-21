//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMDIBHelper111.h（DibBase_Hgy.h ）
// File Des: DIB辅助函数接口类，此代码基于DibBase_Hgy.h 2011年版本改写
// File Summary: 学习使用方式可以参考gtest-DMDIBHelper_Test.cpp
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2011-12-31	1.0			
//      guoyou      2014-10-14  1.1           提取部分功能支持基本DIB操作
//-------------------------------------------------------
#pragma once
#include "DMColor.h"

namespace DM
{
	/// <summary>
	///		简洁版本，简版去掉了对8、16、24位的支持,
	///     只支持32位BI_RGB创建的自上而下扫描的DIBSection（注意是自上而下，所以CreateDIBSection的nHei在内部使用了其负值）
	/// </summary>
	class DM_EXPORT DMDIBHelper
	{
		enum{Red,Green,Blue};
		struct  AlphaCheck
		{
			bool bActive;            // 本结构中的其它参数是否可用
			bool bSameAlpha;         // DIB有相同的Alpha
			byte Alpha;              // alpha值
		};

	public:
		DMDIBHelper();
		virtual~DMDIBHelper();
		void DIBRelease();

	public:
		static HBITMAP CreateDIBSection(HDC hdc,int nWid,int nHei,void** ppBits);
		static bool AlphaBlend32(DMDIBHelper* pDest, int nX, int nY, int nWd, int nHd, DMDIBHelper*pSrc,int nX2,int nY2,int nWs, int nHs,int nAlpha);

	public:

		/// <summary>
		///     创建DIBSection
		/// </summary>
		/// <returns>创建后的HBITMAP</returns>
		HBITMAP CreateDIBSection(HDC hdc, int nWid,int nHei);

		//---------------------------------------------------
		// Function Des:alpha备份,此部分功能后续使用MMX优化
		//---------------------------------------------------
		void  AdjustAlpha(byte byAlpha);  ///< 调整所有alpha
		void  AdjustAlphaRect(byte byAlpha, LPRECT lpRect);
		void  AlphaCheckInit();			  ///< check像素阵列是否是相同的alpha
		void  AlphaCheckUninit();		  ///< 反check
		void  AlphaBackup(LPRECT lpRect); ///< <see cref="AlphaRestore"/> 必须成对出现！
		void  AlphaRestore();			  ///< <see cref="AlphaBackup"/>  必须成对出现！
	   
		/// <summary>
		///     H:-180~180（度）,=0不作调整, S/L:0~200(建议，最大值可>200),=100不作调整
		/// </summary>
		/// <remarks>
		///     HSL变换,HSL操作建议是在非预乘像素阵列下进行，但为了更快的效率，目前采用预乘像素阵列
		///     HSL变换应该在整个像素阵列不再变化时开始做调整.不然原始像素阵列就不再准确
		/// </remarks>
		/// <returns>创建后的HBITMAP</returns>
		bool AdjustHSL32(int H, int S, int L);
		bool AdjustHSL32Rect(int H, int S, int L,LPRECT lpRect);
		bool ResetHSL32(void);
		
		DMColor GetPixel(int x, int y);
		bool SetPixel(int x, int y, DMColor clr);
		BYTE GetAlpha(int x, int y);
		bool SetAlpha(int x, int y, BYTE alpha);

		
		bool GreyImage(void); 

		/// <summary>
		///    可以帮我们快速的观察在自写动画中的每帧变化,绘制的区域是否正确
		///    最简单的使用方式是直接打开window图形查看器查看保存的图片(动画也会自动变化)
		/// </summary>
		bool SaveFile(LPCWSTR pszFileName);			

		inline PVOID GetPixelBits(void)       const	{ return m_pPixelBits;}
		inline PVOID GetPixelCopyBits(void)   const	{ return NULL!=m_pPixelCopyBits?m_pPixelCopyBits:m_pPixelBits;}
		inline int  GetWidth(void)			  const	{ return m_nWidth;}
		inline int  GetHeight(void)			  const	{ return m_nHeight;}
		inline int  GetSize(void)			  const { return m_nImageSize;}
		inline int  GetBPS(void)			  const { return m_nBPS;}
	
	public:// 辅助
		void DM_RGBtoHSL(BYTE &R, BYTE &G, BYTE &B, float &H, float &S, float &L);
		void DM_HSLtoRGB(float &H, float &S, float &L, BYTE &R, BYTE &G, BYTE &B);
		void RGBtoHSL(BYTE R,BYTE G,BYTE B,float* H,float* S,float* L);
		COLORREF HSLtoRGB(float H,float S,float L);

	public:
		HBITMAP                                             m_hBitmap;				///< DIBSection创建的HBitmap
		BYTE*												m_pPixelBits;	        ///< pixel array
		BYTE*												m_pPixelCopyBits;		///< copy origin pixel array
		int													m_nImageSize;			///< pixel array size
		int												    m_nWidth;				///< image pixel width
		int													m_nHeight;				///< image pixel height, positive
		int													m_nBPS;					///< byte per scan line, per plane

		LPBITMAPINFO									    m_pBMI;					///< 位图头
		
		// ---------------------------------------------------------------------------
		AlphaCheck                                          m_AlphaCheck;
#define  MAX_ALPHABUF   60000
		static BYTE											ms_AlphaBk[MAX_ALPHABUF];///< static 数据不会占用sizeof（DMDIBHelper)
		static BYTE*                                        ms_pBk;
		static DM::CRect                                    ms_ApRect;
	};

}//namespace DM