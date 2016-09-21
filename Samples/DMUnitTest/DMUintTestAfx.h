#pragma once

/// 头文件依赖

// 0 系统类
#include <time.h>


// 1 gtest
#include <gtest/gtest.h>

// 2  DM
#include "DmMainOutput.h"

// lib
#ifdef _DEBUG
#pragma comment(lib,"gtest_d.lib")
#pragma comment(lib,"DmMain_d.lib")
#else
#pragma comment(lib,"gtest.lib")
#pragma comment(lib,"DmMain.lib")
#endif

#define  SHOW_TOSCREEN			  0// 设置为0表示不显示到屏幕
using namespace DM;