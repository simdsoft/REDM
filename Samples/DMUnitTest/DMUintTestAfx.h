#pragma once

/// ͷ�ļ�����

// 0 ϵͳ��
#include <time.h>


// 1 gtest
#include <gtest/gtest.h>

// 2  DM
#include "DmMainOutput.h"

// lib
#ifdef _DEBUG
#pragma comment(lib,"DmMain_d.lib")
#else
#pragma comment(lib,"DmMain.lib")
#endif

#define  SHOW_TOSCREEN			  1// ����Ϊ0��ʾ����ʾ����Ļ
using namespace DM;