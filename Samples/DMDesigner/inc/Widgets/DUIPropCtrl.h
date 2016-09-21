// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIPropCtrl.h 
// File mark:   
/*DUIPropFrame:
1.创建以下子控件:
DUIHeaderCtrl+DUISplitLayout(DUIPropList + DUIPropDesc)
2.记录以下参数:
m_iHeadHei:DUIHeaderCtrl的高度
m_iDescHei:DUIPropDesc的初始高度
m_iLeftColWid:DUIHeaderCtrl左边的宽度
			  3.交互消息:
OnHeaderSizeChanging:当DUIHeaderCtrl改变宽度时，必须保证它右边同时变化，同时通知到DUIPropList
						 4.布局：
						 DUIHeaderCtrl+DUISplitLayout使用绝对布局，在OnSize中改变时重新布局


						 DUIPropList
						 1.记录以下参数
m_iRowHei:行高
m_iLeftColWid:在DUIPropFrame的OnHeaderSizeChanging变化时同步变化
			  m_pExpandSkin：展开收缩的通用box皮肤

			  2.交互消息:
					 当m_iLeftColWid改变时，必须保证DUIHeaderCtrl同时被改变*/
// File summary:模仿MFCPropertyGridCtrl
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-2
// ----------------------------------------------------------------
#pragma once 
#include "DUIScrollBase.h"
#include "DUIHeaderCtrl.h"
#include "IProp.h"

namespace DMAttr
{
	class DUIPropListAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* INT_rowhei;													 ///< 项的高度,默认为20,示例:rowhei="20"
		static wchar_t* INT_leftcolwid;												 ///< 左边值的默认宽度,默认为30,示例:leftcolwid="30"
		static wchar_t* SKIN_expandskin;                                             ///< 展开收缩通用skin,示例:expandskin="skin"
		static wchar_t* SKIN_itembgskin;                                             ///< 左右项的通用背景skin,示例:itembgskin="skin"
	};
	DMAttrValueInit(DUIPropListAttr,INT_rowhei)DMAttrValueInit(DUIPropListAttr,INT_leftcolwid)DMAttrValueInit(DUIPropListAttr,SKIN_expandskin)DMAttrValueInit(DUIPropListAttr,SKIN_itembgskin)

	class DUIPropFrameAttr : public DUIScrollBaseAttr
	{
	public:
		static wchar_t* INT_headhei;												 ///< 头部的高度,默认为24,示例:headhei="24"
		static wchar_t* INT_deschei;												 ///< 描述区的默认高度,默认为20,示例:deschei="20"
		static wchar_t* bool_bshowhead;												 ///< 是否显示头，示例:bshowhead="1"
		static wchar_t* bool_bshowdesc;												 ///< 是否显示描述区,示例:bshowdesc="1"
		static wchar_t* SKIN_headitemskin;                                           ///< head头的itemskin,示例:headitemskin="skin"
	};
	DMAttrValueInit(DUIPropFrameAttr,INT_headhei)DMAttrValueInit(DUIPropFrameAttr,INT_deschei)
	DMAttrValueInit(DUIPropFrameAttr,bool_bshowhead)DMAttrValueInit(DUIPropFrameAttr,bool_bshowdesc)DMAttrValueInit(DUIPropFrameAttr,SKIN_headitemskin)
}

namespace DM 
{
	class DUIPropList;
	class DUIPropDesc;
	class DUIPropFrame : public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIPropFrame,L"propframe",DMREG_Window);
	public:
		DUIPropFrame();
		IPropPtr AddInitAttrProperty(DMXmlInitAttrPtr pInitAttr,IPropPtr pParent = NULL, CStringW strValue = L"");
		IPropPtr FindItemByData(LPARAM lpData);
	public: 
		DM_BEGIN_MSG_MAP()
			MSG_WM_SIZE(OnSize)
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
		void OnSize(UINT nType,CSize size);
		void OnDestroy();

	public:
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		DMCode AdjustLayout();
		DMCode OnHeaderSizeChanging(DMEventArgs *pEvt);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIPropFrameAttr::INT_headhei, m_iHeadHei, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIPropFrameAttr::INT_deschei, m_iDescHei, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIPropFrameAttr::bool_bshowhead,m_bShowHead,DM_ECODE_NOXMLLOADREFRESH);
			DM_bool_ATTRIBUTE(DMAttr::DUIPropFrameAttr::bool_bshowdesc,m_bShowDesc,DM_ECODE_NOXMLLOADREFRESH);
			DM_STRING_ATTRIBUTE(DMAttr::DUIPropFrameAttr::SKIN_headitemskin,m_strHeadItemSkin,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()

	public:
		int                                    m_iHeadHei;              ///< 头高
		int                                    m_iDescHei;              ///< 描述区高度
		bool                                   m_bShowHead;             ///< 是否显示头
		bool                                   m_bShowDesc;             ///< 是否显示desc
		CStringW                               m_strHeadItemSkin;		///< 用于创建head时使用

		//
		DUIHeaderCtrl*                         m_pHeadCtrl;
		DUISplitLayout*                        m_pSplit;
		DUIPropList*                           m_pPropList;
		DUIPropDesc*                           m_pPropDesc;
		bool                                   m_bFirst;
	};

	class DUIPropList : public DUIScrollBase, public DMArrayT<IDMRegPtr>, public IDMAccelHandler
	{
		DMDECLARE_CLASS_NAME(DUIPropList,L"proplist",DMREG_Window);
	public:
		DUIPropList();
		~DUIPropList();

		DMCode AdjustLayout();
		DMCode RegisterProp(IDMReg &RegObj,bool bReplace);
		DMCode CreateRegObj(void** ppObj, LPCWSTR lpszClassName);
		int AddProperty(IPropPtr pProp, bool bRedraw = true);
		bool DelProperty(IPropPtr pProp, bool bRedraw = true);
		bool RemoveAllProp(bool bDelete = true,bool bRedraw = true);
		void UpdateXml(DMXmlNode &XmlNode);
		IPropPtr FindItemByData(LPARAM lpData);
		IPropPtr SetCurSel(IPropPtr pProp, bool bRedraw = true);
		IPropPtr GetCurSel(); 
		IPropPtr HitTest(CPoint pt,IProp::ClickArea* pnArea = NULL,bool bPropsOnly = false);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_SIZE(OnSize)
			MSG_WM_DESTROY(OnDestroy)
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			DM_MSG_WM_ENABLE(DM_OnEnable)
		DM_END_MSG_MAP()
		void OnSize(UINT nType,CSize size);
		void OnDestroy();
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonDbClick(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void DM_OnEnable(BOOL bEnable,UINT nStatus);

	public:
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_OnSetCursor(const CPoint &pt); 
		virtual bool EqualArrayObj(const IDMRegPtr &objsrc, const IDMRegPtr &objdest);
		virtual void PreArrayObjRemove(const IDMRegPtr &obj);
		void UpdateScrollRangeSize();
		void OnRangeSizeChanged(CSize szOld,CSize szNew);
		void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);

		bool OnAccelPressed(const DUIAccel& Accel);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIPropListAttr::INT_rowhei, m_iRowHei, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIPropListAttr::INT_leftcolwid, m_iLeftColWid,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIPropListAttr::SKIN_expandskin,m_pExpandSkin,DM_ECODE_NOXMLLOADREFRESH);
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIPropListAttr::SKIN_itembgskin,m_pItemBgSkin,DM_ECODE_NOXMLLOADREFRESH);
		DM_END_ATTRIBUTES()

	public:
		int                                    m_iRowHei;               ///< 行高
		int                                    m_iLeftColWid;           ///< 左边name的宽度
		IDMSkinPtr                             m_pExpandSkin;			///< 展开收缩的box皮肤
		IDMSkinPtr                             m_pItemBgSkin;			///< name，value的通用背景皮肤

		DM::CList<IPropPtr>					   m_lstProps;				///< 属性列表
		IPropPtr							   m_pSel;					///< 当前选中的属性列表

		CPoint                                 m_ptDrag;
		bool                                   m_bDragging;
		DUIPropFrame*                          m_pFrame;
		DUIPropDesc*                           m_pDesc;

		DWORD								   m_accel;					///< 删除键
	};

	class DUIPropDesc : public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIPropDesc,L"propdesc",DMREG_Window);
	public:
		DUIPropDesc();
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
		DM_END_MSG_MAP()
		void DM_OnPaint(IDMCanvas* pCanvas);
	public:
		DUIPropList*                           m_pPropList;
	};

	
}//namespace DM