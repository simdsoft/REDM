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
		static wchar_t* ITEM_holdskin;		
		static wchar_t* ITEM_bhold;  
		static wchar_t* ITEM_bsel;     
	};
	DMAttrValueInit(TGPMenuAttr,ITEM_holdskin)DMAttrValueInit(TGPMenuAttr,ITEM_bhold)DMAttrValueInit(TGPMenuAttr,ITEM_bsel)
}

namespace DM
{
	class TGPMenuItemData : public DMMenuItemData
	{
	public:
		IDMSkin*		pHoldSkin;													///< 停留图片
		bool            bHold;														///< 点击后不消失,只切换pHoldSkin
		bool            bSel;														///< pHoldSkin0-1状态相切换
	};

	class TGPMenuItem : public DUIMenuItem
	{
	public:
		void OnRelease();	
		void DrawOwnerItem(IDMCanvas* pCanvas, CRect& rcItem, MENUITEMINFO &mii);	///< 自己处理菜单某一项的绘制,mii中包含了TGPMenuItemData的内容
		LRESULT MenuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		///< 强制处理菜单消息，hold状态不允许它退出
		
	public:
		LRESULT On_MN_BUTTON(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	class TGPMenu:public DUIMenu
	{
	public:
		~TGPMenu();

	public:
		DMMenuItemData* ParseItem(DMXmlNode& XmlItem);								///< 增加自己的扩展XML项解析
		DMMenuItemData* NewMenuItemData();											///< 外部创建自己的扩展XML项TGPMenuItemData
		void DeleteMenuItemData(DMMenuItemData*pObj);								///< 外部释放自己的扩展XML项TGPMenuItemData
		DUIMenuItem* NewMenuItem();
	};
}//namespace DM