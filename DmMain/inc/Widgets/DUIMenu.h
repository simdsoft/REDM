//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIMenu.h 
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
#include "DUIMenuHelper.h"

namespace DM
{
	/// <summary>
	///		 菜单简版
	/// </summary>
	class DM_EXPORT DUIMenu:public DMCWnd
	{
	public:
		DUIMenu();
		~DUIMenu(void);

	public:// 可重载
		virtual DMMenuItemData* ParseItem(DMXmlNode& XmlItem);
		virtual DMMenuItemData* NewMenuItemData();
		virtual void DeleteMenuItemData(DMMenuItemData*pObj);
		virtual DUIMenuItem* NewMenuItem();

	public:
		bool LoadMenu(LPCWSTR lpszType,LPCWSTR lpszResName);								   ///< 通过指定xml资源加载
		bool LoadMenu(DMXmlNode &XmlNode);													   ///< 通过xml直接加载

	public:
		int TrackPopupMenu(UINT uFlags, int x, int y, HWND hWnd, LPCRECT lpRect=NULL);         ///< 返回当前点击了哪一项
		DUIMenu GetSubMenu(int nPos);
		bool InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem,LPCTSTR strText, int iIcon);
		void DestroyMenu();

	public:
		//---------------------------------------------------
		// Function Des: 消息分发系列函数
		//---------------------------------------------------
		void OnInitMenu(HMENU menu);															///< 初始化会在TrackPopupMenu中触发
		void OnInitMenuPopup(HMENU menuPopup, UINT nIndex, BOOL bSysMenu);						///< 初始化会在TrackPopupMenu中触发
		void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU menu);
		void OnMeasureItem(UINT /*wParam*/,LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	public:// 辅助
		void BuildMenu(HMENU menuPopup,DMXmlNode xmlNode);
		void InstallMenuHook();
		void UnInstallMenuHook();

	public:
		static LRESULT CALLBACK MenuHookProc(int iCode, WPARAM wParam, LPARAM lParam);

	public:// 辅助
		DECLARE_MSG_MAP()										// 仿MFC消息映射宏

	public:
		DMSmartPtrT<DUIMenu_XmlInfo>			m_pDUIMenuXmlInfo;
		DUIMenu*								m_pParent;
		HMENU									m_hMenu;
		DM::CArray<DMMenuItemData*>				m_DmmiArray;
	
		HWND									m_hMenuOwner;       ///< DUI的宿主窗口
		HHOOK									m_hMenuHook;
		static DUIMenu*                         m_pThis;
	};
}//namespace DM