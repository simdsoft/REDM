//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DmMainOutput.h 
// File Des: 用于预编译中包含，以及扩展插件中包含
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once

/// 导出宏处理
#ifdef DLL_DMMAIN

#ifdef	 DmMain_EXPORTS
	#define  DM_EXPORT		 __declspec( dllexport )
#else
	#define	 DM_EXPORT		 __declspec( dllimport )
	#pragma message("DmMain使用dll版本")
#endif// DmMain_EXPORTS

#else

	#pragma message("DmMain使用lib版本")
	#define DM_EXPORT

#endif// DLL_DMMAIN

// 由于编译器对template要求声明和实现一起提供，所以产生此告警
// 也可导出模板,如:template class DM_EXPORT CStringT<wchar_t>
__pragma(warning(disable: 4251))
__pragma(warning(disable: 4275))
/// 头文件依赖

// 0 系统类
#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <crtdbg.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")// 此文件比较小，基本不影响编译大小
#pragma comment(lib,"msimg32.lib")

//#define DM_EXCLUDE_SPY 1
//#define DM_EXCLUDE_MUI 1
//#define DM_EXCLUDE_ACTIVEX 1
//#define DM_EXCLUDE_IE 1

#define dm_stricmp _stricmp
#define dm_strnicmp _strnicmp
#define dm_xmlstrcmp _stricmp
#define dm_xmlstrncmp _strnicmp
#define DM_XNTEXT(s) s
#define dm_isnilstr(s) (!s || !*s)

typedef const char* DMPCSTR;

// 1 宏定义
#include "Common/DMDef.h"
#include "Common/DMError.h"

// 2 没有任何依赖的辅助类
#include "Common/DMMemDispatch.h"
#include "Common/DMHelper.h"
#include "Common/DMRefNum.h"
#include "Common/DMDC.h"
#include "Common/DMMisc.h"
#include "Common/Template/DMLock.h"
#include "Common/Template/DMSingletonT.h"
#include "Common/Template/DMSmartPtrT.h"
#include "Common/Template/DMArrayT.h"
#include "Common/Template/DMMapT.h"
#include "Common/Template/DMAutoResetT.h"
#include "Common/Template/DMAllocT.h"
#include "IDmMain/IDMRegT.h"
#include "Common/DMXmlParse.h"
#include "Core/DMBase.h"
#include "Core/Dui/DUIAccel.h"
#include "Common/DMColor.h"
#include "Common/DMBundle.h"

// 3 事件
#include "Core/Event/DMEventMgr.h"
#include "Core/Event/DMEvents.h"
#include "Core/Msg/DMEventCrack.h"

// 4 模块扩展接口
#include "IDmMain/IDMPlugin.h"
#include "IDmMain/IDMMsg.h"
#include "IDmMain/IDMRes.h"
#include "IDmMain/IDMLog.h"
#include "IDmMain/IDMAttribute.h"
#include "IDmMain/IDMImgDecoder.h"
#include "IDmMain/IDMRender.h"
#include "IDmMain/IDMDraw.h"
#include "IDmMain/IDMSkin.h"
#include "IDmMain/IDMStyle.h"
#include "IDmMain/IDMLayout.h"
#include "IDmMain/IDMContainer.h"
#include "IDmMain/IDMToolTip.h"
#include "IDmMain/IDMAnimate.h"
#include "IDmMain/IDMScript.h"
#include "IDmMain/IDMTrans.h"
#include "IDmMain/IDMTaskRunner.h"

// 5 消息
#include "Core/Msg/DMMsgCrack.h"
#include "Core/Msg/DUIMsgCrack.h"
#include "Core/DMDispatch.h"				
#include "Core/Msg/DMXmlCrack.h"

// 6 基础窗口类
#include "Core/Msg/DMMsgThunk.h"
#include "Core/DMCWndBase.h"
#include "Core/DMCWnd.h"
#include "Widgets/DUIWindow.h"
#include "Widgets/IDUIRealWnd.h"
#include "Common/DMHWndHelper.h"
#include "Core/DMHWnd.h"
#include "Core/DMHDialog.h"
#include "Core/DMApp.h"
