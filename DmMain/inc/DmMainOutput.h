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

// 1 宏定义
#include "DMDef.h"
#include "DMError.h"

// 2 没有任何依赖的辅助类
#include "DMMemDispatch.h"
#include "DMHelper.h"
#include "DMRefNum.h"
#include "DMDC.h"
#include "DMMisc.h"
#include "DMLock.h"
#include "DMSingletonT.h"
#include "DMSmartPtrT.h"
#include "DMArrayT.h"
#include "DMMapT.h"
#include "DMAutoResetT.h"
#include "DMAllocT.h"
#include "IDMRegT.h"
#include "DMXmlParse.h"
#include "DMBase.h"
#include "DUIAccel.h"
#include "DMColor.h"

// 3 事件
#include "DMEventMgr.h"
#include "DMEvents.h"
#include "DMEventCrack.h"

// 4 模块扩展接口
#include "IDMPlugin.h"
#include "IDMMsg.h"
#include "IDMRes.h"
#include "IDMLog.h"
#include "IDMAttribute.h"
#include "IDMImgDecoder.h"
#include "IDMRender.h"
#include "IDMDraw.h"
#include "IDMSkin.h"
#include "IDMStyle.h"
#include "IDMLayout.h"
#include "IDMContainer.h"
#include "IDMToolTip.h"
#include "IDMAnimate.h"
#include "IDMScript.h"
#include "IDMTrans.h"

// 5 消息
#include "DMMsgCrack.h"
#include "DUIMsgCrack.h"
#include "DMDispatch.h"				
#include "DMXmlCrack.h"

// 6 基础窗口类
#include "DMMsgThunk.h"
#include "DMCWndBase.h"
#include "DMCWnd.h"
#include "DUIWindow.h"
#include "IDUIRealWnd.h"
#include "DMHWndHelper.h"
#include "DMHWnd.h"
#include "DMHDialog.h"
#include "DMApp.h"








