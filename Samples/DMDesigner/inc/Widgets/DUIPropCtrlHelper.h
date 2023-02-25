// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIPropCtrlHelper.h 
// File mark:   
// File summary:ģ��CMFCPropertyGridProperty������
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
		static const char* ITEM_name;													 ///< �������,ʾ��:name="name0"
		static const char* ITEM_value;													 ///< �������,ʾ��:value="value0"
		static const char* ITEM_desc;													 ///< �������,ʾ��:desc="cccc"
		static const char* ITEM_bgroup;                                                 ///< �Ƿ�Ϊgroup��group��û��ֵ�ɱ༭��,ʾ��:bgroup="1"
	};
	DMAttrValueInit(PropGridAttr,ITEM_name)DMAttrValueInit(PropGridAttr,ITEM_value)DMAttrValueInit(PropGridAttr,ITEM_desc)
	DMAttrValueInit(PropGridAttr,ITEM_bgroup)
	class PropOptionAttr
	{
	public:
		static const char* ITME_options;                                                 ///< ����ѡ��������ͣ�ʾ��:option="1|2|3"
	};
	DMAttrValueInit(PropOptionAttr,ITME_options)
}

namespace DM
{	
	class DUIPropList;
	/// <summary>
	///		ģ��CMFCPropertyGridProperty
	/// </summary>
	class PropGrid : public IProp
	{
		DMDECLARE_CLASS_NAME(PropGrid,"propgrid",DMREG_Unknown);
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
		CStringA GetName();
		CStringA GetValue();

		DMCode InitDMData(DMXmlNode &XmlNode);	

		// ����
		virtual DMCode OnRender(IDMCanvas* pCanvas);
		virtual DMCode OnDrawExpandBox(IDMCanvas* pCanvas,CRect rcExpand);
		virtual DMCode OnDrawName(IDMCanvas* pCanvas, CRect rcName);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);
		virtual DMCode OnDrawDesc(IDMCanvas* pCanvas, CRect rcDesc);

		// ���
		virtual DMCode OnClickValue(UINT uiMsg, CPoint point);
		virtual DMCode OnClickName(UINT uiMsg, CPoint point);
		virtual DMCode OnClickDrag(UINT uiMsg, CPoint point);

		// �ؼ���������ʾ������
		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);

		void SendValueChangedEvt(CStringW strOldValue);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_WSTRING_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_name, m_strName, DM_ECODE_OK)
			DM_WSTRING_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_value, m_strValue, DM_ECODE_OK)
			DM_WSTRING_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_desc, m_strDescr, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::PropGridAttr::ITEM_bgroup, m_bGroup, DM_ECODE_OK)
		DM_END_ATTRIBUTES()

	public:
		CStringW									m_strName;		
		CStringW									m_strDescr;			
		CStringW                                    m_strValue;   

		CRect										m_Rect;				///< ������
		CRect                                       m_rcName;			///< name���ڵ�����
		CRect                                       m_rcValue;			///< value���ڵ�����
		CRect                                       m_rcDrag;           ///< ��ʾ���϶�������
		CRect                                       m_rcExpandBox;      ///< ��������Сbox���ڵ�����

		//
		DUIPropList*								m_pOwner;			///< ���Ե�ӵ�д���
		IPropPtr									m_pParent;			///< ���Եĸ�����
		bool										m_bExpanded;		///< �Ƿ�Ϊչ��
		bool										m_bVisible;			///< �Ƿ�ɼ�
		bool										m_bInPlaceEdit;		///< �Ƿ���InPlace�ؼ��ɼ�״̬
		bool                                        m_bGroup;			///< �Ƿ�Ϊgroup��group��û��value
		DM::CList<IPropPtr>							m_lstSubItems;		///< �������б�
		DUIEdit*                                    m_pValueEdit;		///< �����г��ֵı༭��

		LPARAM                                      m_lpData;
	};


	/// <summary>
	///		ģ��������STRING
	/// </summary>
	class PropString : public PropGrid
	{
		DMDECLARE_CLASS_NAME(PropString,"propstring",DMREG_Unknown);
	public:
		virtual DMCode OnDrawName(IDMCanvas* pCanvas, CRect rcName);
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);
	}; 

	/// <summary>
	///		ģ�������е�INT
	/// </summary>
	class PropInt : public  PropString
	{
		DMDECLARE_CLASS_NAME(PropInt,"propint",DMREG_Unknown);
	public:
		virtual DMCode OnCreateInPlaceCtrl();
	};

	/// <summary>
	///		ģ�������е�BYTE
	/// </summary>
	class PropByte : public PropString
	{
		DMDECLARE_CLASS_NAME(PropByte,"propbyte",DMREG_Unknown);
	public:
		virtual DMCode OnCreateInPlaceCtrl();
		DMCode OnEditChange(DMEventArgs* pEvt);
	};

	/// <summary>
	///		ģ�������е�Optionѡ����
	/// </summary>
	class PropOption :public PropString
	{
		DMDECLARE_CLASS_NAME(PropGrid,"propoption",DMREG_Unknown);
	public:
		PropOption();
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue);

		virtual DMCode OnCreateInPlaceCtrl();
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true);
		DMCode PreDelete();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_WSTRING_ATTRIBUTE(DMAttr::PropOptionAttr::ITME_options,m_strOptions,DM_ECODE_OK)
		DM_END_ATTRIBUTES()
		CStringW                                    m_strOptions;		///< Comboxbox��ѡ����
		DUIComboBox*                                m_pValueCbx;		///< �����г��ֵ�Comboxbox������
	};

	/// <summary>
	///		ģ�������е�Colorѡ����
	/// </summary>
	class PropClr :public PropString
	{
		DMDECLARE_CLASS_NAME(PropClr,"propclr",DMREG_Unknown);
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
		DMColor                                     m_crValue;          ///< ��ɫ
		DUIRect*                                    m_pcrRect;			 ///< ���ĸ�ֵ�ֱ��ʾPBGRA

		CRect                                       m_rcClr;            ///< ��ʾ��ɫ������
		CRect                                       m_rcText;
	};

	/// <summary>
	///		ģ�������е�Rectѡ����
	/// </summary>
	class PropRect : public PropString
	{
		DMDECLARE_CLASS_NAME(PropRect,"proprect",DMREG_Unknown);
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
	///		ģ�������е�SIZE.POINTѡ����
	/// </summary>
	class PropSize : public PropString
	{
		DMDECLARE_CLASS_NAME(PropRect,"propsize",DMREG_Unknown)
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
	///		ģ�������е�Fontѡ����
	/// </summary>
	class PropFont : public PropString
	{
		DMDECLARE_CLASS_NAME(PropFont,"propfont",DMREG_Unknown);
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
	///		ģ�������е�ACCELѡ����
	/// </summary>
	class PropAccel : public PropString
	{
		DMDECLARE_CLASS_NAME(PropAccel,"propaccel",DMREG_Unknown);
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

