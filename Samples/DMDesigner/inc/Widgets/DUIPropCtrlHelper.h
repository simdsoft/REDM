// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIPropCtrlHelper.h 
// File mark:   
// File summary:模拟CMFCPropertyGridProperty等特性
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-27
// ----------------------------------------------------------------
#pragma once
#include "DUIEdit.h"
#include "DUIHotKeyCtrl.h"
#include "IProp.h"

namespace DMAttr
{
	class PropGridAttr 
	{
	public:
		static wchar_t* ITEM_name;													 ///< 项的名字,示例:name="name0"
		static wchar_t* ITEM_value;													 ///< 项的名字,示例:value="value0"
		static wchar_t* ITEM_desc;													 ///< 项的描述,示例:desc="cccc"
		static wchar_t* ITEM_bgroup;                                                 ///< 是否为group，group是没有值可编辑的,示例:bgroup="1"
	};
	DMAttrValueInit(PropGridAttr,ITEM_name)DMAttrValueInit(PropGridAttr,ITEM_value)DMAttrValueInit(PropGridAttr,ITEM_desc)
	DMAttrValueInit(PropGridAttr,ITEM_bgroup)
	class PropOptionAttr
	{
	public:
		static wchar_t* ITME_options;                                                 ///< 下拉选择项的类型，示例:option="1|2|3"
	};
	DMAttrValueInit(PropOptionAttr,ITME_options)
}

namespace DM
{	
	class DUIPropList;
	/// <summary>
	///		模拟CMFCPropertyGridProperty
	/// </summary>
	class PropGrid : public IProp
	{
		DMDECLARE_CLASS_NAME(PropGrid,L"propgrid",DMREG_Unknown);
	public:
		PropGrid();
		~PropGrid();

	public:
		DMCode SetOwner(DUIPropList *pOwner);
		DMCode SetParent(IProp* pParent);
		DMCode Expand(bool bExpand = true);
		DMCode Reposition(int& y);
		DMCode Invalidate();
		DMCode AddSubItem(IPropPtr pProp);
		DMCode RemoveSubItem(IProp* &pProp,bool bDelete = true);
		DMCode RemoveAllSubItem(bool bDelete);
		DMCode PreDelete();
		
		bool IsSubItem(IPropPtr pSubProp);		
		bool IsParentExpanded();
		bool IsExpanded();
		bool IsInPlaceEdit();
		bool IsGroup();
		IProp* HitTest(CPoint pt, IProp::ClickArea* pnArea = NULL);
		int GetHierarchyLevel();
		int GetHeiWithSub();
		DUIPropList* GetOwner();
		IProp* GetParent();
		LPARAM GetData();
		void SetData(LPARAM lpData);
		IProp* FindSubItemByData(LPARAM lpData);
		void UpdateSubXml(DMXmlNode &XmlNode);
		CStringW GetName();
		CStringW GetValue();

		DMCode InitDMData(DMXmlNode &XmlNode);	

		// 绘制
		virtual DMCode OnRender(IDMCanvas* pCanvas);
		virtual DMCode OnDrawExpandBox(IDMCanvas* pCanvas,CRect rcExpand);
		virtual DMCode OnDrawName(IDMCanvas* pCanvas, CRect rcName);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);
		virtual DMCode OnDrawDesc(IDMCanvas* pCanvas, CRect rcDesc);

		// 点击
		virtual DMCode OnClickValue(UINT uiMsg, CPoint point);
		virtual DMCode OnClickName(UINT uiMsg, CPoint point);
		virtual DMCode OnClickDrag(UINT uiMsg, CPoint point);

		// 控件创建、显示、隐藏
		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);

		void SendValueChangedEvt(CStringW strOldValue);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_STRING_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_name, m_strName, DM_ECODE_OK)
			DM_STRING_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_value, m_strValue, DM_ECODE_OK)
			DM_STRING_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_desc, m_strDescr, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_bgroup, m_bGroup, DM_ECODE_OK)
		DM_END_ATTRIBUTES()

	public:
		CStringW									m_strName;		
		CStringW									m_strDescr;			
		CStringW                                    m_strValue;   

		CRect										m_Rect;				///< 总区域
		CRect                                       m_rcName;			///< name所在的区域
		CRect                                       m_rcValue;			///< value所在的区域
		CRect                                       m_rcDrag;           ///< 显示可拖动的区域
		CRect                                       m_rcExpandBox;      ///< 弹起收缩小box所在的区域

		//
		DUIPropList*								m_pOwner;			///< 属性的拥有窗口
		IPropPtr									m_pParent;			///< 属性的父属性
		bool										m_bExpanded;		///< 是否为展开
		bool										m_bVisible;			///< 是否可见
		bool										m_bInPlaceEdit;		///< 是否处理InPlace控件可见状态
		bool                                        m_bGroup;			///< 是否为group，group下没有value
		DM::CList<IPropPtr>							m_lstSubItems;		///< 子属性列表
		DUIEdit*                                    m_pValueEdit;		///< 属性中出现的编辑框

		LPARAM                                      m_lpData;
	};


	/// <summary>
	///		模拟属性中STRING
	/// </summary>
	class PropString : public PropGrid
	{
		DMDECLARE_CLASS_NAME(PropString,L"propstring",DMREG_Unknown);
	public:
		virtual DMCode OnDrawName(IDMCanvas* pCanvas, CRect rcName);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);
	}; 

	/// <summary>
	///		模拟属性中的INT
	/// </summary>
	class PropInt : public  PropString
	{
		DMDECLARE_CLASS_NAME(PropInt,L"propint",DMREG_Unknown);
	public:
		virtual DMCode OnCreateInPlaceCtrl();
	};

	/// <summary>
	///		模拟属性中的BYTE
	/// </summary>
	class PropByte : public PropString
	{
		DMDECLARE_CLASS_NAME(PropByte,L"propbyte",DMREG_Unknown);
	public:
		virtual DMCode OnCreateInPlaceCtrl();
		DMCode OnEditChange(DMEventArgs* pEvt);
	};

	/// <summary>
	///		模拟属性中的Option选择项
	/// </summary>
	class PropOption :public PropString
	{
		DMDECLARE_CLASS_NAME(PropGrid,L"propoption",DMREG_Unknown);
	public:
		PropOption();
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);
		DMCode PreDelete();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_STRING_ATTRIBUTE(DMAttr::PropOptionAttr::ITME_options,m_strOptions,DM_ECODE_OK)
		DM_END_ATTRIBUTES()
		CStringW                                    m_strOptions;		///< Comboxbox的选择项
		DUIComboBox*                                m_pValueCbx;		///< 属性中出现的Comboxbox下拉框
	};

	/// <summary>
	///		模拟属性中的Color选择项
	/// </summary>
	class PropClr :public PropString
	{
		DMDECLARE_CLASS_NAME(PropClr,L"propclr",DMREG_Unknown);
	public:
		PropClr();
		virtual DMCode Reposition(int& y);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);
		virtual DMCode OnClickValue(UINT uiMsg, CPoint point);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);

		DMCode PreDelete();

	public:
		DMCode OnEditChange(DMEventArgs *pEvt);
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_COLOR_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_value,m_crValue,DM_ECODE_OK)
		DM_END_ATTRIBUTES()
	public: 
		DMColor                                     m_crValue;          ///< 颜色
		DUIRect*                                    m_pcrRect;			 ///< 用四个值分别表示PBGRA

		CRect                                       m_rcClr;            ///< 显示颜色的区域
		CRect                                       m_rcText;
	};

	/// <summary>
	///		模拟属性中的Rect选择项
	/// </summary>
	class PropRect : public PropString
	{
		DMDECLARE_CLASS_NAME(PropRect,L"proprect",DMREG_Unknown);
	public:
		PropRect();
		virtual DMCode Reposition(int& y);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);

		DMCode PreDelete();
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_RECT_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_value,m_valueRect,DM_ECODE_OK)
		DM_END_ATTRIBUTES()
	public:
		DUIRect*                                    m_pRect;			 
		CRect                                       m_rcRect;
		CRect                                       m_valueRect;
	};

	/// <summary>
	///		模拟属性中的SIZE.POINT选择项
	/// </summary>
	class PropSize : public PropString
	{
		DMDECLARE_CLASS_NAME(PropRect,L"propsize",DMREG_Unknown)
	public:
		PropSize();
		virtual DMCode Reposition(int& y);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);

		DMCode PreDelete();
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_SIZE_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_value,m_valueSize,DM_ECODE_OK)
		DM_END_ATTRIBUTES()
	public:
		DUISize*                                    m_pSize;			 
		CRect                                       m_rcSize;
		CSize                                       m_valueSize;
	};

	/// <summary>
	///		模拟属性中的Font选择项
	/// </summary>
	class PropFont : public PropString
	{
		DMDECLARE_CLASS_NAME(PropFont,L"propfont",DMREG_Unknown);
	public:
		PropFont();
		virtual DMCode Reposition(int& y);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);
		virtual DMCode OnClickValue(UINT uiMsg, CPoint point);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);
		DMCode PreDelete();
		DMCode OnBtnClick(DMEventArgs* pEvt);

		CStringW GetFontKey(const LPLOGFONTW lpLogFont);

	public:
		DMCode OnEditChange(DMEventArgs *pEvt);
	public:
		CRect                                       m_rcEdit;
		CRect                                       m_rcBtn;

		DUIButton*                                  m_pBtn;
	};

	/// <summary>
	///		模拟属性中的ACCEL选择项
	/// </summary>
	class PropAccel : public PropString
	{
		DMDECLARE_CLASS_NAME(PropAccel,L"propaccel",DMREG_Unknown);
	public:
		PropAccel();
		virtual DMCode Reposition(int& y);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);

		DMCode PreDelete();

	public:

		CRect									   m_rcHotkey;
		DUIHotKeyCtrl*                             m_pHotkey;
	};

}//namespace DM

