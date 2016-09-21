#pragma once
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#include <windows.h>
#include <tchar.h>

#ifdef Plugin_Expand_EXPORTS
#define PLUGINEXPAND_EXPORTS __declspec( dllexport )
#else 
#define PLUGINEXPAND_EXPORTS __declspec( dllimport )
#endif

// 2  DM
#include "DmMainOutput.h"
#include "DMDIBHelper.h"

#ifdef USE_DMSKIA_RENDER_
#pragma warning (disable: 4005)  
#include "SkCanvas.h"
#include "SkBitmap.h"
#include "SkTypeface.h"
#include "SkShader.h"
#include "sktdarray.h"
#include "SkGraphics.h"
#include "skcolorpriv.h"
#include "SkXfermode.h"
#include "effects\SkDashPathEffect.h"
#include "effects\SkGradientShader.h"
#include "ports\SkTypeface_win.h"
#include "src\core\SkReadBuffer.h"
#ifdef _DEBUG
#pragma comment(lib, "Skia_d.lib")
#else
#pragma comment(lib, "Skia.lib")
#endif
#endif//USE_DMSKIA_RENDER_

// lib
#ifdef _DEBUG
#pragma comment(lib,"DmMain_d.lib")
#else
#pragma comment(lib,"DmMain.lib")
#endif



using namespace DM;
