//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des:
// File Summary:gif支持在flash背景上播放
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-5	1.0			
//--------------------------------------------------------
#pragma once
#include "DUIAnimateHelper.h"


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIGif"/>的xml属性定义
	/// </summary>
	class DUIGifAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* GIF_gifskin;                                 ///< gif的资源，不是通过skin指定，而是直接通过文件标识指定,或以文件绝对路径指定,示例:gifskin="gif:4_gif",gifskin="c:\1.gif"                            
		static wchar_t* bool_badjust;                                ///< 是否调整使图片自适应尺寸，也就是图片保持比例缩放，不会变形，默认为true,示例:badjust="1"
		static wchar_t* bool_bcalcclip;                              ///< 计算并自动调整clip区包含整个绘制区,会损失绘制效率，适用于gif控件中包含子gif控件,如果子gif透明，则每次刷新只是父gif的一部分,示例:bcalcclip="1"
	};
	DMAttrValueInit(DUIGifAttr,GIF_gifskin)DMAttrValueInit(DUIGifAttr,bool_badjust)DMAttrValueInit(DUIGifAttr,bool_bcalcclip)
}

namespace DM
{
	/// <summary>
	///		 gif的内置实现
	/// </summary>
	class DM_EXPORT DUIGif:public DUIWindow,public DMArrayT<PDMAnimateFrame>,public IDMTimeline
	{
		DMDECLARE_CLASS_NAME(DUIGif, DUINAME_Gif,DMREG_Window)
	public:
		DUIGif();
		~DUIGif();

		enum DMGifState
		{
			DMGIF_NOREADY,
			DMGIF_NODELAY,
			DMGIF_LOOPEND,
			DMGIF_PAUSE,
			DMGIF_READY,
		};
	public:
		//---------------------------------------------------
		// Function Des: 接口
		//---------------------------------------------------
		virtual DMCode LoadFromFile(LPCWSTR pszFileName);
		virtual DMCode LoadFromMemory(LPVOID lpBuf, UINT ulSize);
		virtual DMCode Start();
		virtual DMCode Pause();
		virtual DMCode Restart();
		virtual DMCode Clear();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_SHOWWINDOW(OnShowWindow)
		    MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnShowWindow(BOOL bShow, UINT nStatus);
		void OnDestroy();

	public:
		DMCode OnTimeline();

	public:
		bool  IsReady();														///< 是否准备好gif								
		bool  Reset();														    ///< 恢复从第一帧开始状态
		int   ReadyCurFrame(UINT ulFrame);										///< 返回当前要绘制帧的状态

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUIGifAttr::bool_bcalcclip,m_bCalcClip,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIGifAttr::bool_badjust,m_bAdjustPic,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIGifAttr::GIF_gifskin, OnAttrGif)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttrGif(LPCWSTR lpszValue, bool bLoadXml);

	public:// 辅助
		void PreArrayObjRemove(const PDMAnimateFrame &obj);

	public:  
		bool                                         m_bAdjustPic;				 ///< 调整使图片自适应尺寸
		bool                                         m_bCalcClip;                ///< 计算并自动调整clip区包含整个绘制区

		DMSmartPtrT<IDMCanvas>                       m_pMemCanvas;               ///< 内存画布,绘制每一帧
		DMSmartPtrT<IDMCanvas>                       m_pPreviousCanvas;          ///< 前一个Unspecified or Do Not Dispose帧
		CRect                                        m_rcGif;					 ///< gif的框架大小,帧大小不一定为框架大小

		// 帧控制
		UINT                                         m_ulCurFrame;               ///< 当前帧号
		UINT										 m_ulFrameCount;;            ///< 帧数目
		UINT                                         m_ulCurLoop;                ///< 当前循环次数
		UINT										 m_ulTotalLoopCount;		 ///< -1表示无限,或者是整数次，不会为0
		DWORD                                        m_dwPreFrameTime;           ///< 上一帧显示的时间，单位ms

		bool                                         m_bPause;                   ///< 暂停中
		
	};




}//namespace DM