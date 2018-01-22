//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: 
// File Summary:关键计算公式：nPage/(nMax-nMin+1)=滑块长度/滚动条的总长度（就是滑块可滑动的槽）
// Cur Version: 1.0
// Author:http://hgy413.com/1894.html
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-10	1.0			
//--------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIScrollBase"/>的xml属性定义
	/// </summary>
	class DUIScrollBaseAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* SKIN_sbskin;								    ///< 滚动条图片,示例:sbskin="ScrollBar"
		static wchar_t* INT_sbwidth;									///< 滚动条的宽度，默认为-1(滚动条图片宽/9),示例:sbwidth="16"
		static wchar_t* INT_arrowwidth;                                 ///< 上下（左右）箭头的高度(宽度),示例:arrowwidth="10"
		static wchar_t* INT_minithumblen;								///< 最小滚轮高度, 默认18, 示例:minithumblen="18"
		static wchar_t* OPTION_sbenable;							    ///< 是否启用滚动条,对应宏DMSB_NULL、DMSB_HORZ、DMSB_VERT,示例:sbenable="1"
		static wchar_t* SIZE_range;                                     ///< 滚动范围,示例:range="1,100"
		static wchar_t* POINT_curpos;                                   ///< 当前滚动位置,x指向水平scroll的nPos,y指向竖直scroll的nPos,示例:curpos="10,20"
		static wchar_t* bool_bnowheelscroll;                            ///< 禁用滚轮滚动，示例:bnowheelscroll="1"
		static wchar_t* bool_bpagesplit;                                ///< 默认为true,此时区分SB_PAGEUP+SB_PAGEDOWN(上下滑槽区域),为false表示(滑槽区域=上滑槽+滚动块+下滑槽),示例:bpagesplit="1"
	};
	DMAttrValueInit(DUIScrollBaseAttr,SKIN_sbskin)DMAttrValueInit(DUIScrollBaseAttr,INT_sbwidth)DMAttrValueInit(DUIScrollBaseAttr,INT_arrowwidth)DMAttrValueInit(DUIScrollBaseAttr,INT_minithumblen)
	DMAttrValueInit(DUIScrollBaseAttr,OPTION_sbenable)DMAttrValueInit(DUIScrollBaseAttr,SIZE_range)
	DMAttrValueInit(DUIScrollBaseAttr,POINT_curpos)DMAttrValueInit(DUIScrollBaseAttr,bool_bnowheelscroll)DMAttrValueInit(DUIScrollBaseAttr,bool_bpagesplit)
}


namespace DM
{
	/// <summary>
	///		 DUIScrollBase的内置实现,仅用于为窗口扩展scrollbar,属性：<see cref="DMAttr::DUIScrollBaseAttr"/>
	/// </summary>
	class DM_EXPORT DUIScrollBase:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIWindow,DUINAME_ScrollBase,DMREG_Window);
	public:
		DUIScrollBase();

	public:
		//---------------------------------------------------
		// Function Des: 对外接口
		//---------------------------------------------------
		bool GetScrollInfo(bool bVert, LPSCROLLINFO lpsi);						///< 获得滚动条信息
		bool SetScrollInfo(bool bVert,LPSCROLLINFO lpsi, bool bRedraw);			///< 强制改变滚动条信息
		bool ShowScrollBar(int wBar, bool bShow);								///< 是否显示滚动条
		bool EnableScrollBar(int wBar,bool bEnable);							///< 是否禁用滚动条					
		bool SetScrollPos(bool bVert, int nNewPos,bool bRedraw);                ///< 设置滚动条新的滚动位置
		bool SetScrollRange(bool bVert,int nMinPos,int nMaxPos,bool bRedraw);	///< 设置滚动条的滚动范围
		bool GetScrollRange(bool bVert,LPINT lpMinPos,LPINT lpMaxPos);			///< 获得滚动条的滚动范围
		bool IsScrollBarEnable(bool bVert);	                                    ///< 滚动条是否可用							
		void SetScrollInfo(SCROLLINFO si,bool bVert);							///< 按掩码设置滚动条信息
		int GetScrollPos(bool bVert);                                           ///< 获取滚动条当前位置

		// 更新scroll！！！
		virtual void UpdateScrollBar();											///< 根据size和viewsize自动计算，是否启用水平、竖直滚动条
		void SetRangeSize(CSize szRange);										///< 设置滚动范围
		void SetRangeCurPos(CPoint pt);											///< 设置当前滚动位置

		// 关键函数！！！！
		virtual bool OnScroll(bool bVert,UINT uCode,int nPos);					///< 滚动,子类可重载这个函数来实现点击滚动条滚动窗口内容
		virtual int GetScrollLineSize(bool bVert){return 10;}					///< 点击上下箭头滚动距离

		virtual void OnRangeSizeChanged(CSize szOld,CSize szNew){}              ///< 当滚动范围改变时触发
		virtual void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);           ///< 当滚动位置改变时触发
		
	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_CREATE(OnCreate)
			MSG_WM_SIZE(OnSize)
			DM_MSG_WM_NCPAINT(DM_OnNcPaint)
			MSG_WM_NCLBUTTONDOWN(OnNcLButtonDown)
			MSG_WM_NCLBUTTONUP(OnNcLButtonUp)
			MSG_WM_NCMOUSEMOVE(OnNcMouseMove)
			MSG_WM_NCMOUSELEAVE(OnNcMouseLeave)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
			MSG_WM_NCCALCSIZE(OnNcCalcSize)
			MSG_WM_SHOWWINDOW(OnShowWindow)
			DM_MSG_WM_TIMER(DM_OnTimer)
		DM_END_MSG_MAP()

	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		int OnCreate(LPVOID);
		void OnSize(UINT nType,CSize size);
		void DM_OnNcPaint(IDMCanvas *pCanvas);
		void OnNcLButtonDown(UINT nFlags, CPoint point);
		void OnNcLButtonUp(UINT nFlags,CPoint pt);
		void OnNcMouseMove(UINT nFlags, CPoint point) ;
		void OnNcMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);// 此处重设客户区大小
		void OnShowWindow(BOOL bShow, UINT nStatus); 
		void DM_OnTimer(char cTimerID);

	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		DMCode DV_GetClientRect(LPRECT lpRect);										///< 此时计算宿主客户区要排除滚动条
		DMCode DV_OnNcHitTest(CPoint pt);											///< 模拟非客户区
		DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);								    ///< 换肤
		
	public:
		bool HasScrollBar(bool bVert);												///< 是否存在滚动条（水平/竖直）
		CRect GetScrollBarRect(bool bVert);											///< 获得滚动条所在区域（水平/竖直）
		CRect GetSbPartRect(bool bVert,UINT uSBCode);								///< 获取滚动条指定某部分的区域
		CRect GetSbRailwayRect(bool bVert);											///< 获取滚动条除箭头外的区域
		CRect GetSbSlideRectByPos(bool bVert,int nPos);								///< 根据nPos获得滚动滑块所在的位置
		SBSTATEINFO HitTest(CPoint pt);												///< 获取状态等信息组合
		int GetSbSlideLength(bool bVert);											///< 获取滚动滑块的长度
		void ScrollUpdateWindow();													///< 滚动条更新到窗口显示

	public:
		void DrawScrollBar(IDMCanvas *pCanvas,LPCRECT lpRectDraw,int iSbCode,int iState,bool bVert, BYTE alpha=0xff);
		void DrawMoreScrollBar(IDMCanvas *pCanvas,int iSbCode,int iState,bool bVert, BYTE alpha=0xff);
		int CalcState(bool bVert,int sbCode, SBSTATEINFO& sbCmpInfo);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::bool_bnowheelscroll,m_bnowheelscroll,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::bool_bpagesplit,m_bpagesplit,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::SKIN_sbskin, OnAttributesbSkin)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::INT_sbwidth,OnAttributesbWidth)
			DM_INT_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::INT_arrowwidth,m_isbAllowSize,DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::INT_minithumblen,m_iMiniThumbLen,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::OPTION_sbenable,OnAttributesbEnable)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::SIZE_range,OnAttributeRange)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::POINT_curpos,OnAttributePos)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributesbSkin(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributesbWidth(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributesbEnable(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeRange(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributePos(LPCWSTR lpszValue, bool bLoadXml);

	public:
		bool                        m_bnowheelscroll;   ///< 禁止通过滚轮滚动
		bool                        m_bpagesplit;       ///< true时区分SB_PAGEUP+SB_PAGEDOWN(上下滑槽区域)
		DMSmartPtrT<IDMSkin>        m_psbSkin;			///< 滚动条图形
		CRect						m_rcsbClient;       ///< 当前客户区域(如果有滚动条，则除去滚动条)
		int							m_isbWid;			///< 滚动条的宽度(默认水平和竖直滚动条宽度相同,就使用一个参数吧
		int							m_isbVisible;		///< 滚动条显示信息,对应宏DMSB_NULL、DMSB_HORZ、DMSB_VERT
		int							m_isbEnable;	    ///< 滚动条可操作信息,对应宏DMSB_NULL、DMSB_HORZ、DMSB_VERT
		int                         m_isbAllowSize;     ///< 滚动条上下箭头高度(或左右)
		int                         m_iMiniThumbLen;    ///< 最小滚轮高度

		SBSTATEINFO                 m_sbInfo;           ///< 记录状态等信息组合，和DMScrollBarSkinImpl相呼应

		SCROLLINFO					m_siVer;			///< 关键结构,子类可通过控制它来控制滚动条的状态（竖直）
		SCROLLINFO                  m_siHoz;			///< 关键结构,子类可通过控制它来控制滚动条的状态（水平）

		int							m_isbDragPos;		///< 对应SCROLLINFO.nPos(水平、竖直)
		CPoint						m_sbDragPt;			///< 拖动的位置（x表示水平，y表示竖直)
		bool						m_bsbDrag;          ///< 拖动状态（鼠标按下时在滑块上）

		DWORD						m_dwUpdateTime;		///< 记录调用UpdateSWindow的时间
		DWORD						m_dwUpdateInterval;
		
		CSize                       m_szRange;          ///< 滚动区的宽高
		CPoint                      m_ptCurPos;         ///< x指向水平scroll的nPos,y指向竖直scroll的nPos
		bool                        m_bUseRange;        ///< 针对edit，它自动计算是否显示滚动条，不需要m_szRange + m_ptCurPos
	};

}//namespace DM
