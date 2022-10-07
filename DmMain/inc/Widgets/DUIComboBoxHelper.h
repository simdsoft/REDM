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
#include "Core/DMDropWnd.h"
#include "Widgets/DUIEdit.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIComboBoxBase"/>��xml���Զ���
	/// </summary>
	class DUIComboBoxBaseAttr:public DUIWindowAttr
	{
	public:
		static const char* bool_bhideedit;                                               ///< �Ƿ���ʾedit,ʾ����bhideedit="1"
		static const char* INT_dropheight;                                               ///< �����߶�,ȡlistbox�ĸ߶Ⱥʹ�����ֵ�Ľ�Сֵ��ʾ,Ĭ��200��ʾ����dropheight="200"
		static const char* INT_dropemptyheight;                                          ///< ��listboxΪ��ʱ����ʾ�߶�,Ĭ��0��ʾ����dropemptyheight="200"
		static const char* INT_animatetime;                                              ///< ����������ʱ�䣬����AnimateWindow,Ĭ��200,Ϊ0��ʾ��ʹ����������,ʾ����animatetime="200"
		static const char* SKIN_btnskin;                                                 ///< ������С���ǰ�ťƤ����ʾ��:btnskin="skinbtn"
		static const char* SIZE_btnsize;                                                 ///< ����С���ǵĿ��,Ϊ-1,-1ʱ��ʾʹ��btnskin��size,��ֱ����,ʾ��:btnsize="-1,-1"
		static const char* INT_cursel;                                                   ///< ��ǰѡ���ʾ��:cursel="1"
		static const char* INT_textoffset;                                 			  ///< �ı���ƫ�ƣ�Ĭ��Ϊ0,ʾ��:textoffset="10"
		static const char* bool_bdroptranslucent;										  ///< �����б����Ƿ�֧��͸��,Ĭ��Ϊ��͸��,ʾ��:bdroptranslucent="0"


		static const char* ITEM_subedit;												  ///< �ӿؼ�edit��XML��ʶ
	};
	DMAttrValueInit(DUIComboBoxBaseAttr,bool_bhideedit)DMAttrValueInit(DUIComboBoxBaseAttr,INT_dropheight)DMAttrValueInit(DUIComboBoxBaseAttr, INT_dropemptyheight)DMAttrValueInit(DUIComboBoxBaseAttr,INT_animatetime)
	DMAttrValueInit(DUIComboBoxBaseAttr,SKIN_btnskin)DMAttrValueInit(DUIComboBoxBaseAttr,SIZE_btnsize)DMAttrValueInit(DUIComboBoxBaseAttr,INT_cursel)DMAttrValueInit(DUIComboBoxBaseAttr,INT_textoffset)
	DMAttrValueInit(DUIComboBoxBaseAttr,bool_bdroptranslucent)DMAttrValueInit(DUIComboBoxBaseAttr,ITEM_subedit)
}

namespace DM
{
#define CB_EDIT_ID          -100													 ///< Ĭ��Combox�е�Edit��ID
#define CB_LIST_ID			-200													 ///< Ĭ��Combox�е�List��ID
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
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void OnMouseHover(WPARAM wParam, CPoint ptPos);
		void OnMouseLeave();
		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	public:
		//---------------------------------------------------
		// Function Des: ����
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
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags);
		void OnDestroy();

	public:
		//---------------------------------------------------
		// Function Des: IDMDropDownOwnerʵ��
		//---------------------------------------------------
		virtual DUIWindow* GetOwnerWindow();                    
		virtual DMCode OnDropDown(DMDropWnd* pDropDown);
		virtual DMCode OnCloseUp(DMDropWnd* pDropDown,UINT uCode);

	public:
		//---------------------------------------------------
		// Function Des: �ӿ�
		//---------------------------------------------------
		virtual int GetCount()const				= 0;
		virtual int GetListBoxHeight()			= 0;        
		virtual int GetCurSel()	const			= 0;    
		virtual CStringW GetLBText(int iItem)   = 0;								 ///< ȡ��listbox������
	
		virtual bool SetCurSel(int iSel)		= 0;
		virtual bool CreateListBox(const DMXmlNode &XmlNode) = 0;
		virtual bool CalcPopupRect(int nHeight,CRect& rcPopup);						 ///< ���㵯������λ��,������rcPopup�У�false��ʾ���ϵ���(virtual�������ⲿ���Ƶ����ڴ�С)			
		virtual CRect CalcBtnRect();												 ///< ��������С�������ڵ�λ��(��combox�е�����)
		virtual void OnSelChanged();
		virtual CStringW GetCBText();												 ///< ȡ��combox������

	public:
		//---------------------------------------------------
		// Function Des: ����
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
		virtual DMCode DV_GetTextRect(LPRECT lpRect);            
		virtual DMCode DV_SetFocusWnd();											 ///< �õ�����,���ComboBox�õ�����ʱ,�ӿؼ�Edit�ò������������
		virtual DMCode DV_FireEvent(DMEventArgs &Evt);
		virtual UINT DV_OnGetDlgCode();	
		virtual bool DV_IsFocusable();

	public:// ����
		void DropDown();															 ///< �����¼�
		void CloseUp();
	
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::bool_bhideedit, OnAttributeHideEdit)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_cursel,	 OnAttributeCurSel)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::SIZE_btnsize,   OnAttributeBtnSize)
			DM_INT_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_dropheight,    m_nDropHeight, DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_dropemptyheight, m_nDropEmptyHeight, DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_animatetime,   m_iAnimTime,   DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::INT_textoffset,    m_nTextOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::SKIN_btnskin,  m_pSkinBtn,    DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIComboBoxBaseAttr::bool_bdroptranslucent,m_bDropTranslucent,    DM_ECODE_OK)
		
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeHideEdit(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeCurSel(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeBtnSize(LPCSTR lpszValue, bool bLoadXml);

	public:
		DUIRichEdit*						m_pEdit;								 ///< DUIRichEditָ��
		DWORD							    m_dwBtnState;							 ///< ��ť״̬  
		IDMSkinPtr							m_pSkinBtn;								 ///< ��ť��Դ   
		CSize                               m_BtnSize;                               ///< ��ť�ĳ���

		bool 							    m_bHideEdit;							 ///< �Ƿ�ComboBox��ʾEdit
		int									m_nDropHeight;							 ///< ������߶�
		int									m_nDropEmptyHeight;						 ///< ListΪ��ʱ������߶�
		int									m_iAnimTime;						     ///< ����ʱ��  
		int									m_iInitSel;								 ///< Ĭ��ѡ������
		int                                 m_nTextOffset;							 ///< �ı�ƫ��
		DMSmartPtrT<DMDropWnd>				m_pDropDownWnd;							 ///< DropDownָ��
		bool								m_bDropTranslucent;

		DMXmlDocument                       m_EditDoc;                               ///< ����edit��xml
	};
}//namespace DM