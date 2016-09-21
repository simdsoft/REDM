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
	///		<see cref="DM::DUIComboBox"/>的xml属性定义
	/// </summary>
	class DUIComboBoxExAttr:public DUIComboBoxBaseAttr
	{
	public:
		static wchar_t* ITEM_subshow;													///< 初始显示的XML标识
		static wchar_t* ITEM_sublistboxex;                                              ///< 子控件listbox的XML标识
	};
	DMAttrValueInit(DUIComboBoxExAttr,ITEM_sublistboxex)DMAttrValueInit(DUIComboBoxExAttr,ITEM_subshow)
}

class TGPComboboxEx: public DUIComboBoxBase
{
	DMDECLARE_CLASS_NAME(TGPComboboxEx, L"tgpcomboxex" ,DMREG_Window)
public:
	int InsertItem(int nIndex, DMXmlNode&XmlNode);
	TGPListBoxEx* GetListBox(); 

public:
	//---------------------------------------------------
	// Function Des: IDMDropDownOwner实现
	//---------------------------------------------------                  
	virtual DMCode OnDropDown(DMDropWnd *pDropDown);
	virtual DMCode OnCloseUp(DMDropWnd *pDropDown,UINT uCode);						

public:
	//---------------------------------------------------
	// Function Des: 重载
	//---------------------------------------------------
	virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
	virtual DMCode DV_GetTextRect(LPRECT lpRect);    
	virtual bool SetCurSel(int iSel);
	virtual int GetCurSel() const;
	virtual int GetCount() const;
	virtual CStringW GetLBText(int iItem);
	virtual int GetListBoxHeight();
	virtual bool CreateListBox(DMXmlNode &XmlNode);
	virtual void OnSelChanged();

	//---------------------------------------------------
	// Function Des: 重载DUIWindow
	//---------------------------------------------------
	virtual DMCode DV_FireEvent(DMEventArgs &Evt);

public:
	bool CalcPopupRect(int nHeight,CRect& rcPopup);		

public:
	DMSmartPtrT<TGPListBoxEx>					m_pListBox; 
};