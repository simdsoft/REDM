//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMHWndHelper.h 
// File Des: 主窗口类DMHWnd.h的辅助数据
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-2	1.0			
//-------------------------------------------------------
#pragma once

namespace DMAttr
{ 
	/// <summary>
	///		<see cref="DM::DMHWnd"/>的xml属性定义
	/// </summary>
	class DMHWndAttr
	{
	public:
		static wchar_t* SIZE_initsize;                                    ///< 如果创建窗口时传入的size为0，则使用它创建窗口初始大小, 示例:initsize="800,572"
		static wchar_t* SIZE_minsize;                                     ///< 最小大小, 示例:minsize="600,500"
		static wchar_t* RECT_maxinset;                                    ///< 全屏时，周边预留大小,可设置负值 示例:maxinset="50,50,50,50"
		static wchar_t* SIZE_maxsize;                                     ///< 最大大小，示例:maxsize="800,800"，-1表示不做限制
		static wchar_t* STRING_title;                                     ///< 窗口标题，默认不会显示出来 示例:title="dmmagic"
		static wchar_t* STRING_regtip;                                    ///< 使用的默认tip注册类名,为NULL使用默认 示例:regtip="dui_tip"
		static wchar_t* STRING_regdraw;                                   ///< 使用的默认draw注册类名,为NULL使用默认 示例:regdraw="dui_draw"
		static wchar_t* STRING_transid;                                   ///< 使用的transid,为NULL使用默认custom 示例:transid="custom"
		static wchar_t* bool_bresize;                                     ///< 窗口可改变大小,示例:bresize="1"
		static wchar_t* bool_btranslucent;                                ///< 窗口使用UpdateLayeredWindow更新,示例:btranslucent="1"
		static wchar_t* BYTE_alpha;                                        ///< 窗口透明度,透明窗口有两种,详见http://hgy413.com/1865.html 示例:alpha="0xc0"
		static wchar_t* INT_h;                                            ///< hsl变换中h,值必须为[-180,180],代表的是人眼所能感知的颜色范围,0为默认值，h="-10"
		static wchar_t* INT_s;											  ///< hsl变换中s,值必须为[0,200],色彩的饱和度，它用0%至100%的值描述了相同色相、明度下色彩纯度的变化。数值越大，颜色中的灰色越少，颜色越鲜艳,示例:s="10"
		static wchar_t* INT_l;										      ///< hsl变换中l,值必须为[0,200],指的是色彩的明度,100为默认值，数值越大，越接近于白色,示例:l="10"
	};
	DMAttrValueInit(DMHWndAttr,SIZE_initsize)DMAttrValueInit(DMHWndAttr,SIZE_minsize)DMAttrValueInit(DMHWndAttr,RECT_maxinset)DMAttrValueInit(DMHWndAttr,SIZE_maxsize)
	DMAttrValueInit(DMHWndAttr,STRING_title)DMAttrValueInit(DMHWndAttr,STRING_regtip)DMAttrValueInit(DMHWndAttr,STRING_regdraw)DMAttrValueInit(DMHWndAttr,STRING_transid)DMAttrValueInit(DMHWndAttr,bool_bresize)DMAttrValueInit(DMHWndAttr,bool_btranslucent)
	DMAttrValueInit(DMHWndAttr,BYTE_alpha)DMAttrValueInit(DMHWndAttr,INT_h)DMAttrValueInit(DMHWndAttr,INT_s)DMAttrValueInit(DMHWndAttr,INT_l)
}

namespace DM
{
	/// <summary>
	///		<see cref="DMHWnd"/>的辅助参数
	/// </summary>
	class DM_EXPORT DMHWnd_Data
	{
	public:
		DMHWnd_Data();
		~DMHWnd_Data();
	public:
		LPCWSTR							m_lpszXmlId;        ///< 使用的XML id    
		bool                            m_bTrackFlag;       ///< 鼠标移动跟踪标志
	};

	class DMHWnd;
	/// <summary>
	///		<see cref="DMHWnd"/>的xml参数,xml参数默认不区分大小写
	/// </summary>
	class DM_EXPORT DMHWnd_XmlInfo:public DMDataBase
	{
	public:
		DMHWnd_XmlInfo(DMHWnd* pOwner);
		~DMHWnd_XmlInfo();
		void ResetXmlInfo();			///< 重置XML属性
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_SIZE_ATTRIBUTE(DMAttr::DMHWndAttr::SIZE_initsize,m_szInit,DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::SIZE_minsize,OnAttributeMinSize)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::SIZE_maxsize,OnAttributeMaxSize)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::bool_bresize,OnAttributeResize)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::bool_btranslucent,OnAttributeTranslucent)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::BYTE_alpha,OnAttributeAlpha)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::INT_h,OnAttributeH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::INT_s,OnAttributeS)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::INT_l,OnAttributeL)
			DM_RECT_ATTRIBUTE(DMAttr::DMHWndAttr::RECT_maxinset,m_rcMaxInset,DM_ECODE_OK)
			DM_STRING_ATTRIBUTE(DMAttr::DMHWndAttr::STRING_title,m_strTitle,DM_ECODE_OK)
			DM_STRING_ATTRIBUTE(DMAttr::DMHWndAttr::STRING_regtip,m_strRegTip,DM_ECODE_OK)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMHWndAttr::STRING_regdraw,OnAttributeRegDraw)
			DM_STRING_ATTRIBUTE(DMAttr::DMHWndAttr::STRING_transid,m_strTransId,DM_ECODE_OK)
			
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeMinSize(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeMaxSize(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeResize(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeTranslucent(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeAlpha(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeH(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeS(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeL(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeRegDraw(LPCWSTR lpszValue, bool bLoadXml);

	public:
		CSize							m_szInit;			 ///< 初始大小
		CSize							m_szMin;			 ///< 最小大小
		CSize                           m_szMax;             ///< 最大大小
		CRect							m_rcMaxInset;		 ///< 全屏时，周边预留大小

		bool							m_bResizable;		 ///< 是否可拖动改变大小
		bool							m_bTranslucent;		 ///< 是否使用UpdateLayeredWindow来更新透明窗口
		BYTE						    m_byAlpha;           ///< 窗口透明度
		int                             m_H;				 ///< HSL
		int                             m_S;				 ///< HSL
		int                             m_L;				 ///< HSL

		CStringW						m_strTitle;			 ///< 窗口标题
		CStringW                        m_strRegTip;         ///< tip注册类
		CStringW                        m_strRegDraw;        ///< draw注册类
		CStringW                        m_strTransId;        ///< 语言包的Node标识
		DMHWnd*                         m_pOwner;
	};

	class DMHWnd;
	/// <summary>
	///		<see cref="DMHWnd"/>的透明窗口蒙版
	/// </summary>
	class DMDummyWnd: public DMCWnd
	{
	public:
		DMDummyWnd(DMHWnd* pOwner);
		DECLARE_MSG_MAP()// 仿MFC消息映射宏

	public:

		void OnPaint(HDC hdc);
	public:
		DMHWnd					 *m_pOwner;
	};

	/// <summary>
	///		对应系统的UPDATELAYEREDWINDOWINFO 
	/// </summary>
	typedef struct tagDMUPDATELAYEREDWINDOWINFO 
	{
		DWORD				 cbSize;
		HDC					 hdcDst;
		const POINT*		 pptDst;
		const SIZE*			 size;
		HDC					 hdcSrc;
		const POINT*		 pptSrc;
		COLORREF			 crKey;
		const BLENDFUNCTION* pblend;
		DWORD				 dwFlags;
		const RECT*			 prcDirty;
	}DMUPDATELAYEREDWINDOWINFO, *PDMUPDATELAYEREDWINDOWINFO;

	// ----------------------------------------------------
	// 透明窗口更新
	typedef BOOL (WINAPI *fun_UpdateLayeredWindowIndirect)(HWND hwnd, const DMUPDATELAYEREDWINDOWINFO *pULWInfo);

}