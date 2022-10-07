//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIFlowLayout.h 
// File Des: ��ʽ����
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
	///		<see cref="DM::FlowLayoutAttr"/>��xml���Զ���
	/// </summary>
	class DUIFlowLayoutAttr:public DUIWindowAttr
	{
	public:
		static const char* INT_childpaddlen;                                    ///< �ӿؼ�֮��Ķ�����,ʾ��childpaddlen="10"
	};
	DMAttrValueInit(DUIFlowLayoutAttr,INT_childpaddlen)

	/// <summary>
	///		<see cref="DM::DUITabLayoutAttr"/>��xml���Զ���
	/// tabΪ�򵥵�wrap���֣���Сһ�£�������������
	/// </summary>
	class DUITabLayoutAttr:public DUIFlowLayoutAttr
	{
	public:
		static const char* SIZE_itemsize;                                     ///< �ؼ��Ĵ�С,ʾ��itemsize="35,25"
	};
	DMAttrValueInit(DUITabLayoutAttr,SIZE_itemsize)

	class DUIWrapLayoutAttr:public DUITabLayoutAttr
	{
	public:
		static const char* INT_colcount;                                     ///< һ�еĿؼ���Ŀ,ʾ��colcount="10"
	};
	DMAttrValueInit(DUIWrapLayoutAttr,INT_colcount)

	/// <summary>
	///		<see cref="DM::DUISplitLayout"/>��xml���Զ���
	/// </summary>
	class DUISplitLayoutAttr:public DUIFlowLayoutAttr
	{
	public:
		static const char* bool_bvert;                                    ///< �Ƿ�����ֱ��,ʾ��:bvert="1"
		static const char* bool_bfirstchange;                             ///< �������ⲿ����size�仯ʱ����һ�����ڱ仯,ʾ��:bfirstchange="1"                          
		static const char* SKIN_sliderskin;                               ///< �м���϶����Ƥ��,ʾ��:sliderskin="sliderskin"
		static const char* COLOR_clrslider;                               ///< �м���϶���ı���ɫ,ʾ��:clrslider="pbgra(ff,ff,ff,ff)"
		static const char* INT_sliderwidth;                               ///< �м���϶���Ŀ��(ˮƽ)��߶�(��ֱ)��ʾ��:sliderwidth="10"
		static const char* INT_firstchildwidth;                           ///< ��һ�����ڵĿ�ʼ���,ʾ��:firstchildwidth="100"
		static const char* STRING_firstchildpercent;                      ///< ��һ�����ڵĿ�ʼ��Ȱٷֱ�,��INT_firstchildwidthͬʱʹ��ʱ,����ʹ�ðٷֱ�,ʾ��:firstchildpercent="%50"
		static const char* INT_minimumwidth;							   ///< �ǵ�һ���仯���ڿ���ק��С���(ˮƽ)��߶�(��ֱ) :minimumwidth="30"  lzlong add
		static const char* INT_maximumwidth;							   ///< �ǵ�һ���仯���ڿ���ק�����(ˮƽ)��߶�(��ֱ) :maximumwidth="300"  lzlong add
	};
	DMAttrValueInit(DUISplitLayoutAttr,bool_bvert)DMAttrValueInit(DUISplitLayoutAttr,bool_bfirstchange)DMAttrValueInit(DUISplitLayoutAttr,INT_firstchildwidth)DMAttrValueInit(DUISplitLayoutAttr,STRING_firstchildpercent)
		DMAttrValueInit(DUISplitLayoutAttr, SKIN_sliderskin)DMAttrValueInit(DUISplitLayoutAttr, COLOR_clrslider)DMAttrValueInit(DUISplitLayoutAttr, INT_sliderwidth)DMAttrValueInit(DUISplitLayoutAttr, INT_minimumwidth)
		DMAttrValueInit(DUISplitLayoutAttr, INT_maximumwidth)
}

namespace DM
{
	/// <summary>
	///		��ʽ���ֻ���
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
		int                            m_iChildPaddLen;		///<�ӿؼ�֮��Ķ������	
	};

	/// <summary>
	///		��ֱ��ʽ����
	/// </summary>
	class DM_EXPORT DUIVLayout:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUIWindow,"VLayout",DMREG_FlowLayout);
	public:
		DMCode DV_UpdateChildLayout();	
	};

	/// <summary>
	///		ˮƽ��ʽ����
	/// </summary>
	class DM_EXPORT DUIHLayout:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUIWindow,"HLayout",DMREG_FlowLayout);
	public:
		DMCode DV_UpdateChildLayout();		
	};

	/// <summary>
	///		tab��ʽ���֣���С��ͬ
	/// </summary>
	class DM_EXPORT DUITabLayout:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUIWindow,"TabLayout",DMREG_FlowLayout);
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
	///		Warp��ʽ����
	/// </summary>
	class DM_EXPORT DUIWrapLayout:public DUITabLayout
	{
		DMDECLARE_CLASS_NAME(DUIWindow,"WrapLayout",DMREG_FlowLayout);
	public:
		DMCode DV_UpdateChildLayout();		

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIWrapLayoutAttr::INT_colcount,m_nColumns, DM_ECODE_OK)// ��û������sizeʱ�����������Զ����ÿ��
		DM_END_ATTRIBUTES()

	public:
		int                            m_nColumns;
	};

	// <summary>
	///		 DUISplitLayout�ָ�ֵ�����ʵ�֣����ԣ�<see cref="DMAttr::DUISplitterAttr"/>
	/// ��:���������ڣ�A,B������,�������ⲿ����size�仯ʱ��ҪôA���ڱ仯��ҪôB���ڱ仯��2ѡһ
	/// </summary>
	class DUISplitLayout:public DUIFlowLayout
	{
		DMDECLARE_CLASS_NAME(DUISplitLayout, "splitlayout",DMREG_FlowLayout)
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
		DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
		DMCode DV_UpdateChildLayout();		
		DMCode DV_OnSetCursor(const CPoint &pt);

	public:// ����      
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
			DM_INT_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::INT_minimumwidth, m_iMinimumWid, DM_ECODE_NOXMLRELAYOUT)
			DM_INT_ATTRIBUTE(DMAttr::DUISplitLayoutAttr::INT_maximumwidth, m_iMaximumWid, DM_ECODE_NOXMLRELAYOUT)
		DM_END_ATTRIBUTES()
		DMCode OnAttributeFirstChildWidth(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeFirstChildPercent(LPCSTR lpszValue, bool bLoadXml);

	public:
		bool                             m_bVert;                ///< �Ƿ���ֱ
		int                              m_iSliderWid;           ///< �м���϶���Ŀ��(ˮƽ)��߶�(��ֱ)
		int								 m_iMinimumWid;			 ///< ��С���
		int								 m_iMaximumWid;			 ///< �����
		DMSmartPtrT<IDMSkin>             m_pSliderSkin;
		DMColor                          m_ClrSlider;
		bool                             m_bFirstChange;
		int                              m_iFirstChildWidth;     ///< ��һ�����ڵĳ�ʼֵ
		int                              m_iFirstChildPercent;   ///< ��һ�����ڵĳ�ʼ�ٷֱ�
		int                              m_iFixWid;              ///< �̶��Ŀ�ȣ�Ϊ-1��ʾδ��ʼ��,�����ʾsize�仯ʱ���̶�size���Ǹ�����
		bool                             m_bDrag;
		CPoint                           m_ptDrag;
		CStringA                         m_strPos;
	};
}//namespace DM