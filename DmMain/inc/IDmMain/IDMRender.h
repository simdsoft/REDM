
//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMRender.h 
// File Des: 渲染接口
// File Summary: 这里所有的COLORREF都需使用RGBA宏来指定！！！！！！！！！！！！！！！
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-16	1.0			
//-------------------------------------------------------
#pragma once
#include "DMColor.h"

namespace DM
{

	// 基本图元声明-------------------------------------
	class IDMBitmap;												// SkBitmap或HBITMAP的抽象体.
	class IDMBrush;													// SkBitmap位图画刷或HBRUSH的抽象体	
	class IDMFont;													// SkTypeface或HFONT的抽象体				
	class IDMPen;													// SkBitmap或HPEN的抽象体
	class IDMRegion;												// SkRegion或HRGN的抽象体
	class IDMCanvas;                                                // SkCanvas或HDC的抽象体

	/// -------------------------------------------------
	/// <summary>
	///		渲染工厂
	/// </summary>
	class IDMRender:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMRender,L"IDMRender",DMREG_Render);
	public:
		IDMRender(){};
		virtual~IDMRender(){};

		//---------------------------------------------------
		// Function Des: 创建基本图元系列函数
		//---------------------------------------------------
		virtual DMCode CreateBitmap(IDMBitmap** ppBitmap)							= 0;  ///<创建图形
		virtual DMCode CreateFont(const LPLOGFONTW plf,IDMFont** ppFont)			= 0;  ///<创建字体
		virtual DMCode CreateRegion(IDMRegion** ppRgn)								= 0;  ///<创建绘制区
		virtual DMCode CreatePen(DMColor clr,int iStyle,int iWidth,IDMPen** ppPen)  = 0;  ///<创建Pen
		virtual DMCode CreateSolidColorBrush(DMColor clr,IDMBrush** ppBrush)		= 0;  ///<创建实色画刷
		virtual DMCode CreateBitmapBrush(IDMBitmap*pBmp,IDMBrush** ppBrush)			= 0;  ///<创建图片画刷
		virtual DMCode CreateCanvas(int nWid,int nHei,IDMCanvas** ppCanvas)			= 0;  ///<创建画布
	};

	enum DMFTYPE		 ///< MetaFile type
	{
		DMF_NULL,		
		DMF_PEN=1,		 ///< 钢笔图元
		DMF_BRUSH=2,	 ///< 画刷图元
		DMF_BITMAP=3,	 ///< 图形图元
		DMF_FONT=4,		 ///< 字体图元
		DMF_REGION=5,	 ///< 区域图元
		DMF_CANVAS=6,	 ///< 画布图元
	};

	enum DMEXPEND_MODE	 ///< 绘制模式
	{
		DEM_STRETCH,     ///< 拉伸
		DEM_TILE,        ///< 平铺
	};

	/// -------------------------------------------------
	/// <summary>
	///		基本图元预处理
	/// </summary>
	class IDMMetaFile:public DMRefNum
	{
	public:
		virtual DMFTYPE GetType() = 0;
	};

	/// ------------------------------------------------
	/// <summary>
	///		 基本图元实现.kBitmap或HBITMAP的抽象体.
	/// </summary>
	class IDMBitmap:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_BITMAP;}
	public:
		virtual DMCode Init(int nWid,int nHei,const LPVOID pBits=NULL) = 0;				///<初始化
		virtual DMCode InitFromFrame(IDMImgFrame *pImgFrame) = 0;						///<从<see cref="IDMImgFrame"/>中加载
		virtual DMCode LoadFromFile(LPCWSTR pszFileName,LPCWSTR pszType) = 0;			///<从文件中加载
		virtual DMCode LoadFromMemory(LPBYTE pBuf,size_t szLen,LPCWSTR pszType) = 0;	///<从内存中加载

		virtual DMCode AdjustHSL32(int H, int S, int L,LPRECT lpRect=NULL) = 0;			///<调整HSL
		virtual DMCode ResetHsl() = 0;												    ///<恢复未变换HSL前，清除HSL变换的像素阵列被清除
		virtual DMCode SaveFile(LPCWSTR pszFileName) = 0;								///<辅助，用于保存HSL变换后的图片,简洁版仅支持32位BITMAP

		virtual DMCode AdjustAlpha(byte byAlpha, LPRECT lpRect) = 0;					///<调整一个区域的alpha
		virtual DMCode AlphaBackup(LPRECT lpRect) = 0;									///<备份Alpha.必须成对出现！
		virtual DMCode AlphaRestore() = 0;												///<恢复Alpha.必须成对出现！

		//---------------------------------------------------
		// Function Des: 获取系列函数
		//---------------------------------------------------
		virtual int GetWidth() = 0;														///<取得宽度
		virtual int GetHeight() = 0;													///<取得高度
		virtual int GetSize() = 0;													    ///<取得大小
		virtual int GetBPS() = 0;														///<取得单平面扫描线
		virtual DMColor GetPixel(int x, int y) = 0;										///<取得像素点
		virtual bool SetPixel(int x, int y, DMColor clr) = 0;						    ///<设置像素点
		virtual PVOID GetPixelBits() = 0;                                               ///<获取当前像素阵列
		virtual HBITMAP GetBitmap() = 0;												///<获得HBitmap					
	};
	
	/// ------------------------------------------------
	/// <summary>
	///		 SkBitmap位图画刷或HBRUSH的抽象体
	/// </summary>
	class IDMBrush:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_BRUSH;}
	};

	/// ------------------------------------------------
	/// <summary>
	///		 SkTypeface或HFONT的抽象体
	/// </summary>
	class IDMFont:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_FONT;}

	public:

		virtual const LOGFONTW* GetLogFont() const = 0;	///<取得字体
		virtual LPCWSTR GetFaceName()			   = 0;	///<字体名
		virtual bool IsUnderline()				   = 0; ///<是否有下划线
		virtual bool IsItalic()					   = 0; ///<是否为斜体
		virtual bool IsStrikeOut()				   = 0; ///<是否有删除线
	};

	/// ------------------------------------------------
	/// <summary>
	///		 SkBitmap或HPEN的抽象体
	/// </summary>
	class IDMPen:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_PEN;}

	public:

	};

	/// ------------------------------------------------
	/// <summary>
	///		 SkRegion或HRGN的抽象体--可参考CRgn成员函数
	/// </summary>
	class IDMRegion:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_REGION;}
	public:

		//---------------------------------------------------
		// Function Des: fnCombineMode 可参考CombineRgn API
		//---------------------------------------------------
		virtual bool CombineRect(LPCRECT lpRect,int fnCombineMode=RGN_OR) = 0;///< 不建议使用RGN_DIFF，会产生COMPLEXREGION（非矩形的无效区）
		virtual bool PtInRegion(const POINT &pt) = 0;						  ///< 点是否在区域内
		virtual bool RectInRegion(LPCRECT lpRect) = 0;						  ///< 矩形是否和区域有交集
		virtual void GetRgnBox(LPRECT lpRect) = 0;							  ///< 包含这个区域的矩形
		virtual bool IsEmpty() = 0;											  ///< 区域是否为空
		virtual void Offset(const POINT &pt) = 0;							  ///< 偏移
		virtual void Clear() = 0;											  ///< 清空区域
		virtual DMCode GetRectNum(int &iNum) = 0;                             ///< 获得组成区域的Rect个数
		virtual DMCode GetRectArray(RECT* pRect,int iNum) = 0;                ///< 获得组成区域的Rect列表
	};
	typedef IDMRegion* IDMRegionPtr;

	/// ------------------------------------------------
	/// <summary>
	///		 画布接口
	/// </summary>
	class IDMCanvas:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_CANVAS;}

	public:

		//---------------------------------------------------
		// Function Des:基础
		//---------------------------------------------------
		virtual DMCode SelectObject(IDMMetaFile* pObj,IDMMetaFile** ppOldObj = NULL) = 0;	///<选入图元
		virtual DMCode GetObject(IDMMetaFile** ppObj, DMFTYPE DmfType) =0;					///<获取当前选入的图元
		
		virtual DMCode Resize(SIZE size) = 0;												///<重设置画布大小
		virtual DMCode Resize(int nWid,int nHei) = 0;										///<重设置画布大小
		virtual DMCode GetSize(SIZE &size) = 0;                                             ///<获取画布大小

		virtual HDC GetDC() = 0;															///<获取DC
		virtual DMCode ReleaseDC(HDC hdc) = 0;												///<释放DC
		virtual DMCode SaveCanvas(int *pnState=NULL) = 0;									///<保存DC
		virtual DMCode RestoreCanvas(int nState=-1) = 0;									///<恢复DC

		//---------------------------------------------------
		// Function Des:HSL变换
		//---------------------------------------------------
		virtual DMCode AdjustHSL32(int H, int S, int L,LPRECT lpRect=NULL) = 0;				///<调整HSL
		virtual DMCode ResetHsl() = 0;													    ///<恢复未变换HSL前，清除HSL变换的像素阵列被清除
		virtual PVOID GetPixelBits(int* pSize) = 0;                                         ///<获取当前的像素阵列和大小

		//---------------------------------------------------
		// Function Des:绘制
		//---------------------------------------------------
		virtual DMCode BitBlt(IDMCanvas*pCanvasSrc, int xSrc, int ySrc, LPCRECT lpRectDest, DWORD dwRop = SRCCOPY) = 0;	
		virtual DMCode AlphaBlend(IDMCanvas*pCanvasSrc, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha=0xFF) = 0;

		virtual DMCode DrawBitamp(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha=0xFF, DMEXPEND_MODE ExpandMode=DEM_STRETCH) = 0;
		virtual DMCode DrawBitmapNine(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpSrcMargin, LPCRECT lpRectDest, BYTE alpha=0xFF, DMEXPEND_MODE ExpandMode=DEM_STRETCH) = 0;
		
		virtual DMCode DrawRectangle(LPCRECT lpRect) = 0;
		virtual DMCode FillRectangle(LPCRECT lpRect) = 0;
		virtual DMCode FillSolidRect(LPCRECT lpRect,DMColor clr) = 0;
		virtual DMCode ClearRect(LPCRECT lpRect,DMColor clr) = 0;
		
		virtual DMCode DrawRoundRect(LPCRECT lpRect,POINT &pt) = 0;
		virtual DMCode FillRoundRect(LPCRECT lpRect,POINT &pt) = 0;

		virtual DMCode DrawEllipse(LPCRECT lpRect) = 0;
		virtual DMCode FillEllipse(LPCRECT lpRect) = 0;

		virtual DMCode Polyline(LPPOINT lpPt,int cPoints) = 0;

		virtual DMCode GradientFill(DMColor clrBegin,DMColor clrEnd,LPCRECT lpRect,BOOL bVert,BYTE alpha=0xFF) = 0;
		
		//---------------------------------------------------
		// Function Des:文本
		//---------------------------------------------------
		virtual DMColor SetTextColor(DMColor clr) = 0;
		virtual DMColor GetTextColor() = 0;

		virtual DMCode MeasureText(LPCWSTR lpString, int nCount, OUT SIZE *pSize) = 0;
		virtual DMCode DrawText(LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha=0xFF) = 0;
		virtual DMCode TextOut(LPCWSTR lpString, int nCount, int x, int  y, BYTE alpha=0xFF) = 0;

		//---------------------------------------------------
		// Function Des:坐标变换
		//---------------------------------------------------
		virtual DMCode OffsetViewportOrg(int dx, int dy, OUT LPPOINT lpPoint = NULL) = 0;
		virtual DMCode SetViewportOrgEx(int dx, int dy,  OUT LPPOINT lpPoint = NULL) = 0;
		virtual DMCode GetViewportOrg(LPPOINT lpPoint) = 0;

		//---------------------------------------------------
		// Function Des:绘制区域裁剪
		//---------------------------------------------------
		virtual DMCode PushClip(IDMRegion *pRegion,int fnCombineMode=RGN_AND) = 0;
		virtual DMCode PushClip(LPCRECT lpRect,int fnCombineMode=RGN_AND) = 0;					///<将指定的剪辑区域推送到绘图上下文上
		virtual DMCode PopClip() = 0;															///<恢复、和PushClip必须成对出现
		virtual DMCode GetClipBox(LPRECT lpRect) = 0;
		virtual DMCode GetClipRegion(IDMRegion **ppRegion) = 0;									///<必须使用智能指针，谢谢！
		
		virtual DMCode ExcludeClipRect(LPCRECT lpRect) = 0;
		virtual DMCode IntersectClipRect(LPCRECT lpRect) = 0;

		//---------------------------------------------------
		// Function Des:辅助调试使用
		//---------------------------------------------------
		virtual DMCode SaveBmpToFile(LPCWSTR pszFileName) = 0;

		//---------------------------------------------------
		// Function Des:备份整个Alpha!
		//---------------------------------------------------
		virtual DMCode AdjustAlpha(byte byAlpha, LPRECT lpRect) = 0;					///<调整一个区域的alpha
		virtual DMCode AlphaBackup(LPRECT lpRect) = 0;
		virtual DMCode AlphaRestore() = 0;

	};

}//namespace DM