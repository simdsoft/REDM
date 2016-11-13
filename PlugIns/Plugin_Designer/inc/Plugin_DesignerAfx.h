#pragma once
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#include <windows.h>
#include <tchar.h>

#ifdef Plugin_Designer_EXPORTS
#define PLUGINDESIGNER_EXPORTS __declspec( dllexport )
#else 
#define PLUGINDESIGNER_EXPORTS __declspec( dllimport )
#endif

// 2  DM
#include "DmMainOutput.h"

// lib
#ifdef _DEBUG
#pragma comment(lib,"DmMain_d.lib")
#else
#pragma comment(lib,"DmMain.lib")
#endif

using namespace DM;
