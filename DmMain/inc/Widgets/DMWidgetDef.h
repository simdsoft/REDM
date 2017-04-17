//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMWidgetDef.h 
// File Des: 内置控件的简单定义
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-9	1.0			
//--------------------------------------------------------
#pragma once

namespace DM
{
// ----------------------------------------------------
// 内置窗口定义
#define      DUINAME_Static     L"Static"                       // DUI Static
#define      DUINAME_Group      L"Group"                        // DUI Group
#define      DUINAME_CheckBox   L"CheckBox"                     // DUI CheckBox
#define      DUINAME_Link       L"Link"                         // DUI Link
#define      DUINAME_ComboxBox  L"Combobox"                     // DUI Combobox
#define      DUINAME_Button     L"Button"                       // DUI Button
#define      DUINAME_HotKey     L"HotKey"                       // DUI HotKey
#define      DUINAME_IPEdit	    L"IPEdit"						// DUI IPEdit
#define      DUINAME_IPAddress  L"IPAddress"                    // DUI IPAddress
#define      DUINAME_RadioButton L"RadioButton"					// DUI RadioButton
#define      DUINAME_SliderCtrl L"SliderCtrl"					// DUI SliderCtrl
#define      DUINAME_ProcessCtrl L"ProcessCtrl"					// DUI ProcessCtrl
#define      DUINAME_TabCtrl    L"TabCtrl"                      // DUI TabCtrl
#define      DUINAME_TabPage    L"TabPage"                      // DUI TabPage
#define      DUINAME_TabAnimate L"TabAnimate"                   // DUI TabAnimate
#define      DUINAME_ActiveX    L"ActiveX"                      // DUI ActiveX
#define      DUINAME_Flash      L"Flash"                        // DUI Flash
#define      DUINAME_IE         L"IE"                           // DUI IE
#define      DUINAME_ScrollBar  L"ScrollBar"                    // DUI ScrollBar
#define      DUINAME_Panel	    L"Panel"                        // DUI Panel
#define      DUINAME_ScrollView	L"ScrollView"                   // DUI ScrollView
#define      DUINAME_RichEdit	L"RichEdit"                     // DUI RichEdit
#define      DUINAME_Edit	    L"Edit"                         // DUI Edit
#define      DUINAME_ListCtrl	L"ListCtrl"                     // DUI ListCtrl
#define      DUINAME_ListBox	L"ListBox"                      // DUI ListBox
#define      DUINAME_ListBoxEx	L"ListBoxEx"                    // DUI ListBoxEx
#define      DUINAME_TreeCtrl	L"TreeCtrl"                     // DUI TreeCtrl
#define      DUINAME_TreeCtrlEx	L"TreeCtrlEx"                   // DUI TreeCtrlEx
#define      DUINAME_HeaderCtrl	L"HeaderCtrl"                   // DUI HeaderCtrl
#define      DUINAME_ListCtrlEx	L"ListCtrlEx"                   // DUI ListCtrlEx
#define      DUINAME_Gif     	L"Gif"                          // DUI Gif
#define      DUINAME_ScrollBase L"ScrollBase"                   // DUI ScrollBase
// ----------------------------------------------------
// scroll
// 状态:正常、停留、点击、禁用、非Active
#define    DMSBST_NORMAL                             0							///< 正常状态
#define    DMSBST_HOVER                              1							///< hover状态
#define    DMSBST_PUSHDOWN                           2							///< 按下状态
#define    DMSBST_DISABLE                            3							///< 禁用状态
#define    DMSBST_NOACTIVE							 4							///< 非激活状态

// 滚动条显示状态
#define	   DMSB_NULL								 0
#define	   DMSB_HORZ								 1
#define	   DMSB_VERT								 2
#define	   DMSB_BOTH								(DMSB_HORZ|DMSB_VERT)

struct SBSTATEINFO
{
	SBSTATEINFO()
	{
		sbCode = -1;
		nState = 0;
		bVert  = false;
	}
	SBSTATEINFO(LONG obj)
	{
		memcpy(this,&obj,4);
	}
	bool operator !=(const SBSTATEINFO &obj)
	{
		return memcmp(this,&obj,sizeof(SBSTATEINFO))!=0;
	}
	LONG sbCode:16;
	LONG nState:8;
	bool bVert :8;
};

// 控件:上箭头、下箭头、坚直滚动条、竖直滑槽、左箭头、右箭头、水平滚动条、水平滑槽、小边角
#define    SB_CORNOR								10
#define    MAKESBSTATE(sbCode,nState,bVert)         MAKELONG((sbCode),MAKEWORD((nState),(bVert)))
#define	   TIMER_SBWAIT								1         // 启动连续滚动的定时器
#define    TIMER_SBGO								2         // 连续滚动的定时器
}// namespace DM