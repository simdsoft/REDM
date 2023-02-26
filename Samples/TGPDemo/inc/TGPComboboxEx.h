// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	TGPComboboxEx.h
// File mark:   
// File summary:

// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-4-21
// ----------------------------------------------------------------
#pragma once         

#include "DUIComboBoxHelper.h"
#include "TGPListBoxEx.h"
 
namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIComboBox"/>��xml���Զ���
	/// </summary>
	class DUIComboBoxExAttr:public DUIComboBoxBaseAttr
	{
	public:
		static const char* ITEM_subshow;													///< ��ʼ��ʾ��XML��ʶ
		static const char* ITEM_sublistboxex;                                              ///< �ӿؼ�listbox��XML��ʶ
	};
	DMAttrValueInit(DUIComboBoxExAttr,ITEM_sublistboxex)DMAttrValueInit(DUIComboBoxExAttr,ITEM_subshow)
}

class TGPComboboxEx: public DUIComboBoxBase
{
	DMDECLARE_CLASS_NAME(TGPComboboxEx, "tgpcomboxex" ,DMREG_Window)
public:
	int InsertItem(int nIndex, DMXmlNode&XmlNode);
	TGPListBoxEx* GetListBox(); 

public:
	//---------------------------------------------------
	// Function Des: IDMDropDownOwnerʵ��
	//---------------------------------------------------                  
	virtual DMCode OnDropDown(DMDropWnd *pDropDown);
	virtual DMCode OnCloseUp(DMDropWnd *pDropDown,UINT uCode);						

public:
	//---------------------------------------------------
	// Function Des: ����
	//---------------------------------------------------
	virtual DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
	virtual DMCode DV_GetTextRect(LPRECT lpRect);    
	virtual bool SetCurSel(int iSel);
	virtual int GetCurSel() const;
	virtual int GetCount() const;
	virtual CStringW GetLBText(int iItem);
	virtual int GetListBoxHeight();
	virtual bool CreateListBox(const DMXmlNode &XmlNode);
	virtual void OnSelChanged();

	//---------------------------------------------------
	// Function Des: ����DUIWindow
	//---------------------------------------------------
	virtual DMCode DV_FireEvent(DMEventArgs &Evt);

public:
	bool CalcPopupRect(int nHeight,CRect& rcPopup);		

public:
	DMSmartPtrT<TGPListBoxEx>					m_pListBox; 
};