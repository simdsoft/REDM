
//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMRender.h 
// File Des: ��Ⱦ�ӿ�
// File Summary: �������е�COLORREF����ʹ��RGBA����ָ��������������������������������
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-16	1.0			
//-------------------------------------------------------
#pragma once
#include "Common/DMColor.h"

namespace DM
{

	// ����ͼԪ����-------------------------------------
	class IDMBitmap;												// SkBitmap��HBITMAP�ĳ�����.
	class IDMBrush;													// SkBitmapλͼ��ˢ��HBRUSH�ĳ�����	
	class IDMFont;													// SkTypeface��HFONT�ĳ�����				
	class IDMPen;													// SkBitmap��HPEN�ĳ�����
	class IDMRegion;												// SkRegion��HRGN�ĳ�����
	class IDMCanvas;                                                // SkCanvas��HDC�ĳ�����

	/// -------------------------------------------------
	/// <summary>
	///		��Ⱦ����
	/// </summary>
	class IDMRender:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMRender,"IDMRender",DMREG_Render);
	public:
		IDMRender(){};
		virtual~IDMRender(){};

		//---------------------------------------------------
		// Function Des: ��������ͼԪϵ�к���
		//---------------------------------------------------
		virtual DMCode CreateBitmap(IDMBitmap** ppBitmap)							= 0;  ///<����ͼ��
		virtual DMCode CreateFont(const LPLOGFONTW plf,IDMFont** ppFont)			= 0;  ///<��������
		virtual DMCode CreateRegion(IDMRegion** ppRgn)								= 0;  ///<����������
		virtual DMCode CreatePen(DMColor clr,int iStyle,int iWidth,IDMPen** ppPen)  = 0;  ///<����Pen
		virtual DMCode CreateSolidColorBrush(DMColor clr,IDMBrush** ppBrush)		= 0;  ///<����ʵɫ��ˢ
		virtual DMCode CreateBitmapBrush(IDMBitmap*pBmp,IDMBrush** ppBrush)			= 0;  ///<����ͼƬ��ˢ
		virtual DMCode CreateCanvas(int nWid,int nHei,IDMCanvas** ppCanvas)			= 0;  ///<��������
	};

	enum DMFTYPE		 ///< MetaFile type
	{
		DMF_NULL,		
		DMF_PEN=1,		 ///< �ֱ�ͼԪ
		DMF_BRUSH=2,	 ///< ��ˢͼԪ
		DMF_BITMAP=3,	 ///< ͼ��ͼԪ
		DMF_FONT=4,		 ///< ����ͼԪ
		DMF_REGION=5,	 ///< ����ͼԪ
		DMF_CANVAS=6,	 ///< ����ͼԪ
	};

	enum DMEXPEND_MODE	 ///< ����ģʽ
	{
		DEM_STRETCH,     ///< ����
		DEM_TILE,        ///< ƽ��
	};

	/// -------------------------------------------------
	/// <summary>
	///		����ͼԪԤ����
	/// </summary>
	class IDMMetaFile:public DMRefNum
	{
	public:
		virtual DMFTYPE GetType() = 0;
	};

	/// ------------------------------------------------
	/// <summary>
	///		 ����ͼԪʵ��.kBitmap��HBITMAP�ĳ�����.
	/// </summary>
	class IDMBitmap:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_BITMAP;}
	public:
		virtual DMCode Init(int nWid,int nHei,const LPVOID pBits=NULL) = 0;				///<��ʼ��
		virtual DMCode InitFromFrame(IDMImgFrame *pImgFrame) = 0;						///<��<see cref="IDMImgFrame"/>�м���
		virtual DMCode InitFromHBITMAP(HBITMAP hbit) = 0;
		virtual DMCode LoadFromFile(LPCWSTR pszFileName,LPCSTR pszType) = 0;			///<���ļ��м���
		virtual DMCode LoadFromMemory(LPBYTE pBuf,size_t szLen,LPCSTR pszType) = 0;	///<���ڴ��м���

		virtual DMCode AdjustHSL32(int H, int S, int L,LPRECT lpRect=NULL) = 0;			///<����HSL
		virtual DMCode ResetHsl() = 0;												    ///<�ָ�δ�任HSLǰ�����HSL�任���������б����
		virtual DMCode SaveFile(LPCWSTR pszFileName) = 0;								///<���������ڱ���HSL�任���ͼƬ,�����֧��32λBITMAP

		virtual DMCode AdjustAlpha(byte byAlpha, LPRECT lpRect) = 0;					///<����һ�������alpha
		virtual DMCode AlphaBackup(LPRECT lpRect) = 0;									///<����Alpha.����ɶԳ��֣�
		virtual DMCode AlphaRestore() = 0;												///<�ָ�Alpha.����ɶԳ��֣�

		//---------------------------------------------------
		// Function Des: ��ȡϵ�к���
		//---------------------------------------------------
		virtual int GetWidth() = 0;														///<ȡ�ÿ��
		virtual int GetHeight() = 0;													///<ȡ�ø߶�
		virtual int GetSize() = 0;													    ///<ȡ�ô�С
		virtual int GetBPS() = 0;														///<ȡ�õ�ƽ��ɨ����
		virtual DMColor GetPixel(int x, int y) = 0;										///<ȡ�����ص�
		virtual bool SetPixel(int x, int y, DMColor clr) = 0;						    ///<�������ص�
		virtual PVOID GetPixelBits() = 0;                                               ///<��ȡ��ǰ��������
		virtual HBITMAP GetBitmap() = 0;												///<���HBitmap					
	};
	
	/// ------------------------------------------------
	/// <summary>
	///		 SkBitmapλͼ��ˢ��HBRUSH�ĳ�����
	/// </summary>
	class IDMBrush:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_BRUSH;}
	};

	/// ------------------------------------------------
	/// <summary>
	///		 SkTypeface��HFONT�ĳ�����
	/// </summary>
	class IDMFont:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_FONT;}

	public:

		virtual const LOGFONTW* GetLogFont() const = 0;	///<ȡ������
		virtual LPCWSTR GetFaceName()			   = 0;	///<������
		virtual bool IsUnderline()				   = 0; ///<�Ƿ����»���
		virtual bool IsItalic()					   = 0; ///<�Ƿ�Ϊб��
		virtual bool IsStrikeOut()				   = 0; ///<�Ƿ���ɾ����
	};

	/// ------------------------------------------------
	/// <summary>
	///		 SkBitmap��HPEN�ĳ�����
	/// </summary>
	class IDMPen:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_PEN;}

	public:

	};

	/// ------------------------------------------------
	/// <summary>
	///		 SkRegion��HRGN�ĳ�����--�ɲο�CRgn��Ա����
	/// </summary>
	class IDMRegion:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_REGION;}
	public:

		//---------------------------------------------------
		// Function Des: fnCombineMode �ɲο�CombineRgn API
		//---------------------------------------------------
		virtual bool CombineRect(LPCRECT lpRect,int fnCombineMode=RGN_OR) = 0;///< ������ʹ��RGN_DIFF�������COMPLEXREGION���Ǿ��ε���Ч����
		virtual bool PtInRegion(const POINT &pt) = 0;						  ///< ���Ƿ���������
		virtual bool RectInRegion(LPCRECT lpRect) = 0;						  ///< �����Ƿ�������н���
		virtual void GetRgnBox(LPRECT lpRect) = 0;							  ///< �����������ľ���
		virtual bool IsEmpty() = 0;											  ///< �����Ƿ�Ϊ��
		virtual void Offset(const POINT &pt) = 0;							  ///< ƫ��
		virtual void Clear() = 0;											  ///< �������
		virtual DMCode GetRectNum(int &iNum) = 0;                             ///< �����������Rect����
		virtual DMCode GetRectArray(RECT* pRect,int iNum) = 0;                ///< �����������Rect�б�
	};
	typedef IDMRegion* IDMRegionPtr;

	/// ------------------------------------------------
	/// <summary>
	///		 �����ӿ�
	/// </summary>
	class IDMCanvas:public IDMMetaFile
	{
	public:
		DMFTYPE GetType(){return DMF_CANVAS;}

	public:

		//---------------------------------------------------
		// Function Des:����
		//---------------------------------------------------
		virtual DMCode SelectObject(IDMMetaFile* pObj,IDMMetaFile** ppOldObj = NULL) = 0;	///<ѡ��ͼԪ
		virtual DMCode GetObject(IDMMetaFile** ppObj, DMFTYPE DmfType) =0;					///<��ȡ��ǰѡ���ͼԪ
		
		virtual DMCode Resize(SIZE size) = 0;												///<�����û�����С
		virtual DMCode Resize(int nWid,int nHei) = 0;										///<�����û�����С
		virtual DMCode GetSize(SIZE &size) = 0;                                             ///<��ȡ������С

		virtual HDC GetDC() = 0;															///<��ȡDC
		virtual DMCode ReleaseDC(HDC hdc) = 0;												///<�ͷ�DC
		virtual DMCode SaveCanvas(int *pnState=NULL) = 0;									///<����DC
		virtual DMCode RestoreCanvas(int nState=-1) = 0;									///<�ָ�DC

		//---------------------------------------------------
		// Function Des:HSL�任
		//---------------------------------------------------
		virtual DMCode AdjustHSL32(int H, int S, int L,LPRECT lpRect=NULL) = 0;				///<����HSL
		virtual DMCode ResetHsl() = 0;													    ///<�ָ�δ�任HSLǰ�����HSL�任���������б����
		virtual PVOID GetPixelBits(int* pSize) = 0;                                         ///<��ȡ��ǰ���������кʹ�С

		//---------------------------------------------------
		// Function Des:����
		//---------------------------------------------------
		virtual DMCode BitBlt(IDMCanvas*pCanvasSrc, int xSrc, int ySrc, LPCRECT lpRectDest, DWORD dwRop = SRCCOPY) = 0;	
		virtual DMCode AlphaBlend(IDMCanvas*pCanvasSrc, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha=0xFF) = 0;

		virtual DMCode DrawBitamp(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpRectDest, BYTE alpha=0xFF, DMEXPEND_MODE ExpandMode=DEM_STRETCH) = 0;
		virtual DMCode DrawBitmapNine(IDMBitmap *pBitamp, LPCRECT lpRectSrc, LPCRECT lpSrcMargin, LPCRECT lpRectDest, BYTE alpha=0xFF, DMEXPEND_MODE ExpandMode=DEM_STRETCH) = 0;
		
		virtual DMCode DrawRectangle(LPCRECT lpRect) = 0;
		virtual DMCode FillRectangle(LPCRECT lpRect) = 0;
		virtual DMCode FillSolidRect(LPCRECT lpRect,DMColor clr) = 0;
		virtual DMCode ClearRect(LPCRECT lpRect,DMColor clr) = 0;
		
		virtual DMCode DrawRoundRect(LPCRECT lpRect,const POINT &pt) = 0;
		virtual DMCode FillRoundRect(LPCRECT lpRect,POINT &pt) = 0;

		virtual DMCode DrawEllipse(LPCRECT lpRect) = 0;
		virtual DMCode FillEllipse(LPCRECT lpRect) = 0;

		virtual DMCode Polyline(LPPOINT lpPt,int cPoints) = 0;

		virtual DMCode GradientFill(DMColor clrBegin,DMColor clrEnd,LPCRECT lpRect,BOOL bVert,BYTE alpha=0xFF) = 0;
		
		//---------------------------------------------------
		// Function Des:�ı�
		//---------------------------------------------------
		virtual DMColor SetTextColor(DMColor clr) = 0;
		virtual DMColor GetTextColor() = 0;

		virtual DMCode MeasureText(LPCWSTR lpString, int nCount, OUT SIZE *pSize) = 0;
		virtual DMCode DrawText(LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha=0xFF) = 0;
		virtual DMCode TextOut(LPCWSTR lpString, int nCount, int x, int  y, BYTE alpha=0xFF) = 0;

		//---------------------------------------------------
		// Function Des:����任
		//---------------------------------------------------
		virtual DMCode OffsetViewportOrg(int dx, int dy, OUT LPPOINT lpPoint = NULL) = 0;
		virtual DMCode SetViewportOrgEx(int dx, int dy,  OUT LPPOINT lpPoint = NULL) = 0;
		virtual DMCode GetViewportOrg(LPPOINT lpPoint) = 0;

		//---------------------------------------------------
		// Function Des:��������ü�
		//---------------------------------------------------
		virtual DMCode PushClip(IDMRegion *pRegion,int fnCombineMode=RGN_AND) = 0;
		virtual DMCode PushClip(LPCRECT lpRect,int fnCombineMode=RGN_AND) = 0;					///<��ָ���ļ����������͵���ͼ��������
		virtual DMCode PopClip() = 0;															///<�ָ�����PushClip����ɶԳ���
		virtual DMCode GetClipBox(LPRECT lpRect) = 0;
		virtual DMCode GetClipRegion(IDMRegion **ppRegion) = 0;									///<����ʹ������ָ�룬лл��
		
		virtual DMCode ExcludeClipRect(LPCRECT lpRect) = 0;
		virtual DMCode IntersectClipRect(LPCRECT lpRect) = 0;

		//---------------------------------------------------
		// Function Des:��������ʹ��
		//---------------------------------------------------
		virtual DMCode SaveBmpToFile(LPCWSTR pszFileName) = 0;

		//---------------------------------------------------
		// Function Des:��������Alpha!
		//---------------------------------------------------
		virtual DMCode AdjustAlpha(byte byAlpha, LPRECT lpRect) = 0;							///<����һ�������alpha
		virtual DMCode AlphaBackup(LPRECT lpRect) = 0;
		virtual DMCode AlphaRestore() = 0;

		//---------------------------------------------------
		// Function Des:���������������ӵ���չ����,�������,�Ա�֤���ϼ���
		//---------------------------------------------------
		virtual DMCode DrawArc(LPCRECT lpRect,float startAngle, float sweepAngle) = 0;			///<startAngle:�� x ����˳ʱ����ת����������һ���ߵĽǶ�,sweepAngle��startAngle������˳ʱ����ת���ڶ����ߵĽǶȣ��Զ�Ϊ��λ��
		virtual DMCode FillPie(LPCRECT lpRect,float startAngle, float sweepAngle) = 0;			///<https://msdn.microsoft.com/zh-cn/library/2cd07tb4(v=vs.110).aspx?cs-save-lang=1&cs-lang=cpp#code-snippet-1
	};

}//namespace DM