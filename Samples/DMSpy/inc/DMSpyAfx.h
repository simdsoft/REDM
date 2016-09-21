#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#include <windows.h>
#include <tchar.h>

// 2  DM
#include "DmMainOutput.h"

#include "DUIEdit.h"
#include "DUITreeCtrl.h"
#include "DMSpyTool.h"
#include "DUIStatic.h"

// lib
#ifdef _DEBUG
#pragma comment(lib,"DmMain_d.lib")
#else
#pragma comment(lib,"DmMain.lib")
#endif

using namespace DM;