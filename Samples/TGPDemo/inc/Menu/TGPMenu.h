//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: TGPMenu.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-10	1.0			
//--------------------------------------------------------
#pragma once
#include "DUIMenu.h"

namespace DMAttr
{
	class TGPMenuAttr
	{
	public:
		static const char* ITEM_holdskin;		
		static const char* ITEM_bhold;
		static const char* ITEM_bsel;
	};
	DMAttrValueInit(TGPMenuAttr,ITEM_holdskin)DMAttrValueInit(TGPMenuAttr,ITEM_bhold)DMAttrValueInit(TGPMenuAttr,ITEM_bsel)
}

namespace DM
{
	class TGPMenuItemData : public DMMenuItemData
	{
	public:
		IDMSkin*		pHoldSkin;													///< ͣ��ͼƬ
		bool            bHold;														///< �������ʧ,ֻ�л�pHoldSkin
		bool            bSel;														///< pHoldSkin0-1״̬���л�
	};

	class TGPMenuItem : public DUIMenuItem
	{
	public:
		void OnRelease();	
		void DrawOwnerItem(IDMCanvas* pCanvas, CRect& rcItem, MENUITEMINFO &mii);	///< �Լ�����˵�ĳһ��Ļ���,mii�а�����TGPMenuItemData������
		LRESULT MenuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		///< ǿ�ƴ���˵���Ϣ��hold״̬���������˳�
		
	public:
		LRESULT On_MN_BUTTON(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	class TGPMenu:public DUIMenu
	{
	public:
		~TGPMenu();

	public:
		DMMenuItemData* ParseItem(DMXmlNode& XmlItem);								///< �����Լ�����չXML�����
		DMMenuItemData* NewMenuItemData();											///< �ⲿ�����Լ�����չXML��TGPMenuItemData
		void DeleteMenuItemData(DMMenuItemData*pObj);								///< �ⲿ�ͷ��Լ�����չXML��TGPMenuItemData
		DUIMenuItem* NewMenuItem();
	};
}//namespace DM