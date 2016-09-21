//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name:DUIComboBoxHelper.h 
// File Des:
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-2	1.0			
//--------------------------------------------------------
#pragma once
#include "DMDropWnd.h"
#include "DUIEdit.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIComboBoxBase"/>的xml属性定义
	/// </summary>
	class DUIComboBoxBaseAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* bool_bhideedit;                                               ///< 是否不显示edit,示例：bhideedit="1"
		static wchar_t* INT_dropheight;                                               ///< 下拉高度,取listbox的高度和此设置值的较小值显示,默认200，示例：dropheight="200"
		static wchar_t* INT_animatetime;                                              ///< 下拉出来的时间，用于AnimateWindow,默认200,为0表示不使用下拉动画,示例：animatetime="200"
		static wchar_t* SKIN_btnskin;                                                 ///< 下拉的小三角按钮皮肤，示例:btnskin="skinbtn"
		static wchar_t* SIZE_btnsize;                                                 ///< 下拉小三角的宽高,为-1,-1时表示使用btnskin的size,竖直居中,示例:btnsize="-1,-1"
		static wchar_t* INT_cursel;                                                   ///< 当前选中项，示例:cursel="1"
		static wchar_t* INT_textoffset;                                 			  ///< 文本的偏移，默认为0,示例:textoffset="10"
		static wchar_t* bool_bdroptranslucent;										  ///< 下拉列表窗口是否支持透明,默认为不透明,示例:bdroptranslucent="0"

		static wchar_t* ITEM_subedit;												  ///< 子控件edit的XML标识
	};
	DMAttrValueInit(DUIComboBoxBaseAttr,bool_bhideedit)DMAttrValueInit(DUIComboBoxBaseAttr,INT_dropheight)DMAttrValueInit(DUIComboBoxBaseAttr,INT_animatetime)
	DMAttrValueInit(DUIComboBoxBaseAttr,SKIN_btnskin)DMAttrValueInit(DUIComboBoxBaseAttr,SIZE_btnsize)DMAttrValueInit(DUIComboBoxBaseAttr,INT_cursel)DMAttrValueInit(DUIComboBoxBaseAttr,INT_textoffset)
	DMAttrValueInit(DUIComboBoxBaseAttr,bool_bdroptranslucent)DMAttrValueInit(DUIComboBoxBaseAttr,ITEM_subedit)
}

namespace DM
{
#define CB_EDIT_ID          -100													 ///< 默认Combox中的Edit的ID
#define CB_LIST_ID			-200													 ///< 默认Combox中的List的ID
	class DUIComboBoxBase;
	class DM_EXPORT DUIComboEdit:public DUIEdit
	{
	public:
		DUIComboEdit(DUIComboBoxBase *pComBoxBase);
		virtual ~DUIComboEdit();

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_MOUSEHOVER(OnMouseHover)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_CHAR(OnChar)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void OnMouseHover(WPARAM wParam, CPoint ptPos);
		void OnMouseLeave();
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		virtual DMCode DV_FireEvent(DMEventArgs &Evt);
	};

	class DM_EXPORT DUIComboBoxBase:public DUIWindow,public IDMDropOwner
	{
	public:
		DUIComboBoxBase(void);
		virtual ~DUIComboBoxBase();
		void Init();
		void UnInit();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)        
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_KEYDOWN(OnKeyDown) 
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags);
		void OnDestroy();

	public:
		//---------------------------------------------------
		// Function Des: IDMDropDownOwner实现
		//---------------------------------------------------
		virtual DUIWindow* GetOwnerWindow();                    
		virtual DMCode OnDropDown(DMDropWnd* pDropDown);
		virtual DMCode OnCloseUp(DMDropWnd* pDropDown,UINT uCode);

	public:
		//---------------------------------------------------
		// Function Des: 接口
		//---------------------------------------------------
		virtual int GetCount()const				= 0;
		virtual int GetListBoxHeight()			= 0;        
		virtual int GetCurSel()	const			= 0;    
		virtual CStringW GetLBText(int iItem)   = 0;								 ///< 取得listbox的文字
	
		virtual bool SetCurSel(int iSel)		= 0;
		virtual bool CreateListBox(DMXmlNode &XmlNode) = 0;
		virtual bool CalcPopupRect(int nHeight,CRect& rcPopup);						 ///< 计算弹出窗口位置,保存在rcPopup中，false表示向上弹！(virtual，允许外部定制弹窗口大小)			
		virtual CRect CalcBtnRect();												 ///< 计算下拉小三角所在的位置(在combox中的坐标)
		virtual void OnSelChanged();
		virtual CStringW GetCBText();												 ///< 取得combox的文字

	public:
		//---------------------------------------------------
		// Function Des: 重载
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_GetTextRect(LPRECT lpRect);            
		virtual DMCode DV_SetFocusWnd();											 ///< 得到焦点,解决ComboBox得到焦点时,子控件Edit得不到焦点的问题
		virtual DMCode DV_FireEvent(DMEventArgs &Evt);
		virtual UINT DV_OnGetDlgCode();	
		virtual bool DV_IsFocusable();

	public:// 辅助
		void DropDown();															 ///< 下拉事件
		void CloseUp();
	
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::bool_bhideedit, OnAttributeHideEdit)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_cursel,	 OnAttributeCurSel)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::SIZE_btnsize,   OnAttributeBtnSize)
			DM_INT_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_dropheight,    m_nDropHeight, DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_animatetime,   m_iAnimTime,   DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_textoffset,    m_nTextOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::SKIN_btnskin,  m_pSkinBtn,    DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::bool_bdroptranslucent,m_bDropTranslucent,    DM_ECODE_OK)
		
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeHideEdit(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeCurSel(LPCWSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeBtnSize(LPCWSTR lpszValue, bool bLoadXml);

	public:
		DUIRichEdit*						m_pEdit;								 ///< DUIRichEdit指针
		DWORD							    m_dwBtnState;							 ///< 按钮状态  
		IDMSkinPtr							m_pSkinBtn;								 ///< 按钮资源   
		CSize                               m_BtnSize;                               ///< 按钮的长宽

		bool 							    m_bHideEdit;							 ///< 是否ComboBox显示Edit
		int									m_nDropHeight;							 ///< 下拉框高度
		int									m_iAnimTime;						     ///< 动画时间  
		int									m_iInitSel;								 ///< 默认选中索引
		int                                 m_nTextOffset;							 ///< 文本偏移
		DMDropWnd *						    m_pDropDownWnd;							 ///< DropDown指针
		bool								m_bDropTranslucent;

		DMXmlDocument                       m_EditDoc;                               ///< 备份edit的xml
	};
}//namespace DM