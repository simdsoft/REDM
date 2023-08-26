// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIRectTracker.h
// File mark:   
// File summary:橡皮条首次布局完成后,内部就设置成了绝对布局,也就是pos只有第一次是有效的,其余全靠自已手动设置！
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-6-20
// ----------------------------------------------------------------
#pragma once

namespace DMAttr
{
	class DUIRectTrackerAttr: public DUIWindowAttr
	{
	public:
		static const char* SKIN_moveskin;                                                 ///< move小图标，示例:moveskin="bg"
		static const char* COLOR_clrdot;				                                   ///< dot正常颜色，示例:clrdot="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrdotgray;				                               ///< dot灰掉颜色，示例:clrdotgray="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrboxsolid;                                             ///< box的PS_SOLID绘制颜色,box会使用PS_SOLID绘制一次，再使用PS_DASH绘制一次,示例:clrboxsolid="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrboxdash;                                              ///< box的PS_DASH绘制颜色,box会使用PS_SOLID绘制一次，再使用PS_DASH绘制一次,示例:clrboxdash="pbgra(ff,00,ff,ff)"
		static const char* INT_dotwidth;												   ///< dot的宽高,默认为6,示例:dotwidth="6"
		static const char* bool_bmain;													   ///< 为true时只有右下角三个dot是可拖动,其余dot全为灰,示例:bmain="0"
		static const char* bool_ballgray;												   ///< 为true时,dot全为灰,示例:ballgray="0"
	};
	DMAttrValueInit(DUIRectTrackerAttr,SKIN_moveskin)DMAttrValueInit(DUIRectTrackerAttr,COLOR_clrdot)DMAttrValueInit(DUIRectTrackerAttr,COLOR_clrdotgray)
	DMAttrValueInit(DUIRectTrackerAttr, COLOR_clrboxsolid)DMAttrValueInit(DUIRectTrackerAttr, COLOR_clrboxdash)
	DMAttrValueInit(DUIRectTrackerAttr,INT_dotwidth)DMAttrValueInit(DUIRectTrackerAttr,bool_bmain)DMAttrValueInit(DUIRectTrackerAttr,bool_ballgray)
}


namespace DM
{
	/// <summary>
	///		辅助点坐标
	/// </summary>
	enum DMDSDOT
	{
		DMDSDOT_MOVE,
		DMDSDOT_LEFT,
		DMDSDOT_LEFTOP,
		DMDSDOT_TOP,
		DMDSDOT_RIGHTOP,
		DMDSDOT_RIGHT,
		DMDSDOT_RIGHTBTM,
		DMDSDOT_BTM,
		DMDSDOT_LEFTBTM,
		DMDSDOT_BOX,
	};


	class DMDragMeta;
	typedef DMDelegate<void(DMDragMeta&, int)> DMDragMetaEventHandler;
	/// <summary>
	///		辅助点绘制,以及事件处理,用户可重载来实现自定义绘制和事件
	/// </summary>
	class DMDragMeta
	{
	public:
		DMDragMeta(int idx=-1,HCURSOR hCursor = ::LoadCursor(NULL,IDC_ARROW));

	public:
		virtual void OnRender(IDMCanvas* pCanvas);									///< 绘制dot自身
		virtual void OnRenderCursor();												///< 绘制dot上显示的光标

	public:// 辅助
		DMCode AutoDrawRoundRect(IDMCanvas*pCanvas,DMColor TextClr,int iStyle,int iWidth,LPCRECT lpRect, const POINT &pt);

	public:
		int									   m_Index;								 ///< 0为不含所有dot的矩形区（即控件区）,1为左中dot，顺时针8个点，9为左上角的mov小图标,对应DMDSDOT
		CRect								   m_Rect;								 ///< 此点的矩形区,0为最终框选的box区，默认1-9点都在box外围，不占用box区
		HCURSOR								   m_hCursor;							 ///< 停留在此dot时显示的光标
		DMColor                                m_crDot[2];                          ///< dot正常及灰掉时的填充色
		DMColor                                m_crBox[2];                           ///< box的填充色(box会先绘制PS_SOLID, 再绘制PS_DASH)
		DMSmartPtrT<IDMSkin>				   m_pMoveSkin;							 ///< 绘制mov小图标
		bool								   m_bDragDot;							 ///< 是否为拖动点(可能设置0为不可拖动点)
		bool								   m_bEnable;							 ///< 此点是否可用
		DMDragMetaEventHandler				   m_DragMetaAction;					 ///< 触发时的调用事件
		
	};

	class DM_EXPORT DUIRectTracker : public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIRectTracker,DUINAME_RectTracker,DMREG_Window);
	public:
		DUIRectTracker();

	public:
		//---------------------------------------------------
		// Function Des: 对外接口 methods
		//---------------------------------------------------
		bool SetBoxRect(CRect& rect, bool bFire = true);							///< 返回false时表示rect被内部改变			
		void InitDragMeta(CRect rcBox,bool bMain=false,bool bAllGray=false);	    ///< rcBox指控件区,	bMain时只有右下角三个dot是可拖动	
		DMDragMeta* HitTestDragMeta(CPoint pt);										
		CRect CalcTrackerRect(CRect rcBox);											///< 从box区计算出Tracker占的区域
		CRect CalcBoxRect(CRect rcTracker);											///< 从Tracker占的区域推导出当前的box区


	public:// 辅助
		//---------------------------------------------------
		// Function Des: 辅助 methods
		//---------------------------------------------------
		void LinkDragMetaEvent();
		void OnDragBox(DMDragMeta& meta, int iAction);								///< 拖动内部
		void OnDragLeft(DMDragMeta& meta, int iAction);								///< 拖动左边
		void OnDragLeftTop(DMDragMeta& meta, int iAction);							///< 拖动左上角
		void OnDragTop(DMDragMeta& meta, int iAction);								///< 拖动上边
		void OnDragRightTop(DMDragMeta& meta, int iAction);							///< 拖动右上角
		void OnDragRight(DMDragMeta& meta, int iAction);							///< 拖动右边
		void OnDragRightBottom(DMDragMeta& meta, int iAction);						///< 拖动右下角
		void OnDragBottom(DMDragMeta& meta, int iAction);							///< 拖动下边
		void OnDragLeftBottom(DMDragMeta& meta, int iAction);						///< 拖动左下角
		void OnDragMove(DMDragMeta& meta, int iAction);								///< 移动
		void InternalHandleDrag(CRect& rect, int* pHori, int* pVert);
		

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_SIZE(OnSize)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnSize(UINT nType, CSize size);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::SKIN_moveskin,OnAttributeMoveSkin)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::COLOR_clrdot, OnAttributeDotClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::COLOR_clrdotgray, OnAttributeDotGrayClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::COLOR_clrboxsolid, OnAttributeBoxSolidClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::COLOR_clrboxdash, OnAttributeBoxDashClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::INT_dotwidth, OnAttributeDotWidth)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::bool_bmain, OnAttributeIsMain)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::bool_ballgray, OnAttributeIsAllGray)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeMoveSkin(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeDotClr(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeDotGrayClr(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeBoxSolidClr(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeBoxDashClr(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeDotWidth(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeIsMain(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeIsAllGray(LPCSTR lpszValue, bool bLoadXml);

	public:
		DMColor                                 m_crDot[2];                        ///< 矩形区正常及灰掉时的填充色
		DMColor                                 m_crBox[2];                         ///< box的填充色(box会先绘制PS_SOLID, 再绘制PS_DASH)
		DMSmartPtrT<IDMSkin>				    m_pMoveSkin;						///< 绘制mov小图标
		int                                     m_dotWidth;                         ///< Dot的宽高
		bool								    m_bMain;							///< 为true时只有右下角三个dot是可拖动
		bool                                    m_bAllGray;							///< 为true时dot全灰

		//
		CPoint								    m_StartDragPt;
		CPoint									m_TrackDragPt;
		CRect									m_StartDragRc;						///< 开始拖动时元素区域大小

		DMDragMeta*								m_pCurdragMeta;
		DMDragMeta								m_dragMetas[10];
		int										m_dragMetaCount;
		bool                                    m_bDown;							///< 只有在按下时停留在dot上或box上才设置m_bDown

		bool                                    m_bFirstInit;
	};

}