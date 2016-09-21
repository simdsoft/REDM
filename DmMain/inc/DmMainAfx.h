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

/// 此代码主要为VS2013使用设置，vs2008可以直接去掉-------
#ifndef WINVER                 
#define WINVER 0x0501         
#endif

#ifndef _WIN32_WINNT            
#define _WIN32_WINNT 0x0501     
#endif

#ifndef _WIN32_WINDOWS         
#define _WIN32_WINDOWS 0x0501 
#endif

#ifndef _WIN32_IE                  
#define _WIN32_IE 0x0501    
#endif
///------------------------------------------------------

#include "DmMainOutput.h"

// 内部使用
#include "DMAppData.h"
#include "DMDIBHelper.h"

