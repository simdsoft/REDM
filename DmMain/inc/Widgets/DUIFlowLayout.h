//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIFlowLayout.h 
// File Des: 流式布局
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-6	1.0			
//--------------------------------------------------------
#pragma once


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::FlowLayoutAttr"/>的xml属性定义
	/// </summary>
	class DUIFlowLayoutAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* INT_childpaddlen;                                    ///< 子控件之间的额外间距,示例childpaddlen="10"
	};
	DMAttrValueInit(DUIFlowLayoutAttr,INT_childpaddlen)

	/// <summary>
	///		<see cref="DM::DUITabLayoutAttr"/>的xml属性定义
	/// tab为简单的wrap布局，大小一致，不受其他限制
	/// </summary>
	class DUITabLayoutAttr:public DUIFlowLayoutAttr
	{
	public:
		static wchar_t* SIZE_itemsize;                                     ///< 控件的大小,示例itemsize="35,25"
	};
	DMAttrValueInit(DUITabLayoutAttr,SIZE_itemsize)

	class DUIWrapLayoutAttr:public DUITabLayoutAttr
	{
	public:
		static wchar_t* INT_colcount;                                     ///< 一行的控件数目,示例colcount="10"
	};
	DMAttrValueInit(DUIWrapLayoutAttr,INT_colcount)

	/// <summary>
	///		<see cref="DM::DUISplitLayout"/>的xml属性定义
	/// </summary>
	class DUISplitLayoutAttr:public DUIFlowLayoutAttr
	{
	public:
		static wchar_t* bool_bvert;                                    ///< 是否是竖直的,示例:bvert="1"
		static wchar_t* bool_bfirstchange;                             ///< 当整个外部窗口size变化时，第一个窗口变化,示例:bfirstchange="1"                          
		static wchar_t* SKIN_sliderskin;                               ///< 中间可拖动块的皮肤,示例:sliderskin="sliderskin"
		static wchar_t* COLOR_clrslider;                               ///< 中间可拖动块的背景色,示例:clrslider="pbgra(ff,ff,ff,ff)"
		static wchar_t* INT_sliderwidth;                               ///< 中间可拖动块的宽度(水平)或高度(竖直)，示例:sliderwidth="10"
		static wchar_t* INT_firstchildwidth;                           ///< 第一个窗口的开始宽度,示例:firstchildwidth="100"
		static wchar_t* STRING_firstchildpercent;                      ///< 第一个窗口的开始宽度百分比,和INT_firstchildwidth同时使用时,优先使用百分比,示例:firstchildpercent="%50"
	};
	DMAttrValueInit(DUISplitLayoutAttr,bool_bvert)DMAttrValueInit(DUISplitLayoutAttr,bool_bfirstchange)DMAttrValueInit(DUISplitLayoutAttr,INT_firstchildwidth)DMAttrValueInit(DUISplitLayoutAttr,STRING_firstchildpercent)
	DMAttrValueInit(DUISplitLayoutAttr,SKIN_sliderskin)DMAttrValueInit(DUISplitLayoutAttr,COLOR_clrslider)DMAttrValueInit(DUISplitLayoutAttr,INT_sliderwidth)
}

namespace DM
{
	/// <summary>
	///		流式布局基类
	/// </summary>
	class DM_EXPORT DUIFlowLayout:public DUIWindow
	{
	public:
		DUIFlowLayout();
		bool DV_IsStateChangeRedraw(){return false;}

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIFlowLayoutAttr::INT_childpaddlen, m_iChildPaddLen, DM_ECODE_NOXMLRELAYOUT)
		DM_END_ATTRIBUTES()
	public:
		int                            m_iChildPaddLen;		///<子控件之间的额外距离	
	};

	/// <summary>
	///		竖直流式布局
	/// </summary>
	class DM_EXPORT DUIVLayout:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUIWindow,L"VLayout",DMREG_FlowLayout);
	public:
		DMCode DV_UpdateChildLayout();	
	};

	/// <summary>
	///		水平流式布局
	/// </summary>
	class DM_EXPORT DUIHLayout:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUIWindow,L"HLayout",DMREG_FlowLayout);
	public:
		DMCode DV_UpdateChildLayout();		
	};

	/// <summary>
	///		tab流式布局，大小相同
	/// </summary>
	class DM_EXPORT DUITabLayout:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUIWindow,L"TabLayout",DMREG_FlowLayout);
	public:
		DMCode DV_UpdateChildLayout();		

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_SIZE_ATTRIBUTE(DMAttr::DUITabLayoutAttr::SIZE_itemsize, m_szItem, DM_ECODE_NOXMLRELAYOUT)
		DM_END_ATTRIBUTES()
	public:
		CSize                          m_szItem;
	};

	/// <summary>
	///		Warp流式布局
	/// </summary>
	class DM_EXPORT DUIWrapLayout:public DUITabLayout
	{
		DMDECLARE_CLASS_NAME(DUIWindow,L"WrapLayout",DMREG_FlowLayout);
	public:
		DMCode DV_UpdateChildLayout();		

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIWrapLayoutAttr::INT_colcount,m_nColumns, DM_ECODE_OK)// 在没有设置size时，依赖它来自动设置宽度
		DM_END_ATTRIBUTES()

	public:
		int                            m_nColumns;
	};

	// <summary>
	///		 DUISplitLayout分割布局的内置实现，属性：<see cref="DMAttr::DUISplitterAttr"/>
	/// 简化:仅两个窗口（A,B）构成,当整个外部窗口size变化时，要么A窗口变化，要么B窗口变化，2选一
	/// </summary>
	class DUISplitLayout:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUISplitLayout, L"splitlayout",DMREG_FlowLayout)
	public:
		DUISplitLayout();
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			DM_END_MSG_MAP()
	public:
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);

	public:
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		DMCode DV_UpdateChildLayout();		
		DMCode DV_OnSetCursor(const CPoint &pt);

	public:// 辅助      
		DMCode UpdateFixWidth();
		DMCode UpdateDrag(int iDiff);
		CRect GetSliderRect();
	public: 
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::bool_bvert,m_bVert,DM_ECODE_NOXMLRELAYOUT)
			DM_bool_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::bool_bfirstchange,m_bFirstChange,DM_ECODE_NOXMLRELAYOUT)
			DM_INT_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::INT_sliderwidth,m_iSliderWid,DM_ECODE_NOXMLRELAYOUT)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::INT_firstchildwidth,OnAttributeFirstChildWidth)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::STRING_firstchildpercent,OnAttributeFirstChildPercent)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::SKIN_sliderskin,m_pSliderSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::COLOR_clrslider,m_ClrSlider,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
		DMCode OnAttributeFirstChildWidth(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeFirstChildPercent(LPCWSTR lpszValue, bool bLoadXml);

	public:
		bool                             m_bVert;                ///< 是否竖直
		int                              m_iSliderWid;           ///< 中间可拖动块的宽度(水平)或高度(竖直)
		DMSmartPtrT<IDMSkin>             m_pSliderSkin;
		DMColor                          m_ClrSlider;
		bool                             m_bFirstChange;
		int                              m_iFirstChildWidth;     ///< 第一个窗口的初始值
		int                              m_iFirstChildPercent;   ///< 第一个窗口的初始百分比
		int                              m_iFixWid;              ///< 固定的宽度，为-1表示未初始化,其余表示size变化时，固定size的那个窗口
		bool                             m_bDrag;
		CPoint                           m_ptDrag;
		CStringW                         m_strPos;
	};
}//namespace DM