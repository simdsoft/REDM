
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMDef.h 
// File Des: 基础宏定义
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once
#include "DMWidgetDef.h"

namespace DM
{

typedef int											  DMCode;					///< 错误码标识
typedef unsigned long                                 DUIWND,*DUIWNDPtr;        ///< 模拟绘制窗口句柄，也可以视为模拟子窗口的唯一标识                 
typedef ULONG_PTR									  HDMTREEITEM;				///< 树形控件的定义    

// ----------------------------------------------------
// 输入输出
#define DMIN
#define DMOUT
#define DMINOUT

// ----------------------------------------------------
// 辅助
#define DMADDEVENT(EventId)                          m_EventMgr;//.AddEvent(EventId);///< 默认不需要提前加载                                    

// ----------------------------------------------------
// 错误辅助定位
#define DMASSERT									  assert
#define DMASSERT_EXPR(expr, msg)                      _ASSERT_EXPR(expr, msg)   ///;LOG_ERR(msg)
#define DM_INVALID_VALUE                              ((LONG_PTR)-1)			///< 通用的非法返回

// ----------------------------------------------------
// HSLA变化
enum{DM_H=0,DM_S,DM_L,DM_A};

// ----------------------------------------------------
// 像素和逻辑单位转换，
#define DMHIMETRIC_PER_INCH							 2540
#define DMMAP_LOGHIM_TO_PIX(x,ppli)					 MulDiv((ppli), (x), DMHIMETRIC_PER_INCH)
#define DMMAP_PIX_TO_LOGHIM(x,ppli)					 MulDiv(DMHIMETRIC_PER_INCH, (x), (ppli))

// ----------------------------------------------------
// SPY辅助定位
#define WM_DMSPY                                      WM_USER+1314              ///< spy++消息

#define	EVEIDNAME(x)								  #x

static LPCWSTR MSGSTR(UINT uMsg)												///< 打印MSG
{// 如OXC014可能为IDLE时消息，调用：LOG_USER("[msg]:%s\n",MSGSTR(uMsg));
#define MSGDEF(x) if(uMsg==x) return L#x
	MSGDEF(WM_SETCURSOR);
	MSGDEF(WM_NCHITTEST);
	MSGDEF(WM_NCPAINT);
	MSGDEF(WM_PAINT);
	MSGDEF(WM_ERASEBKGND);
	MSGDEF(WM_NCMOUSEMOVE);  
	MSGDEF(WM_MOUSEMOVE);
	MSGDEF(WM_MOUSELEAVE);
	MSGDEF(WM_MOUSEHOVER);   
	MSGDEF(WM_NOTIFY);
	MSGDEF(WM_COMMAND);
	MSGDEF(WM_MEASUREITEM);
	MSGDEF(WM_DRAWITEM);   
	MSGDEF(WM_LBUTTONDOWN);
	MSGDEF(WM_LBUTTONUP);
	MSGDEF(WM_LBUTTONDBLCLK);
	MSGDEF(WM_RBUTTONDOWN);
	MSGDEF(WM_RBUTTONUP);
	MSGDEF(WM_RBUTTONDBLCLK);
	MSGDEF(WM_SETFOCUS);
	MSGDEF(WM_KILLFOCUS);  
	MSGDEF(WM_MOVE);
	MSGDEF(WM_SIZE);
	MSGDEF(WM_INITDIALOG);
	MSGDEF(WM_SIZING);
	MSGDEF(WM_MOVING);
	MSGDEF(WM_GETMINMAXINFO);
	MSGDEF(WM_CAPTURECHANGED);
	MSGDEF(WM_WINDOWPOSCHANGED);
	MSGDEF(WM_WINDOWPOSCHANGING);   
	MSGDEF(WM_NCCALCSIZE);
	MSGDEF(WM_NCCREATE);
	MSGDEF(WM_NCDESTROY);
	MSGDEF(WM_TIMER);
	MSGDEF(WM_KEYDOWN);
	MSGDEF(WM_KEYUP);
	MSGDEF(WM_CHAR);
	MSGDEF(WM_SYSKEYDOWN);
	MSGDEF(WM_SYSKEYUP);
	MSGDEF(WM_SYSCOMMAND);
	MSGDEF(WM_SYSCHAR);
	MSGDEF(WM_VSCROLL);
	MSGDEF(WM_HSCROLL);
	MSGDEF(WM_CHAR);
	MSGDEF(WM_SHOWWINDOW);
	MSGDEF(WM_PARENTNOTIFY);
	MSGDEF(WM_CREATE);
	MSGDEF(WM_NCACTIVATE);
	MSGDEF(WM_ACTIVATE);
	MSGDEF(WM_ACTIVATEAPP);   
	MSGDEF(WM_CLOSE);
	MSGDEF(WM_DESTROY);
	MSGDEF(WM_GETICON);   
	MSGDEF(WM_GETTEXT);
	MSGDEF(WM_GETTEXTLENGTH); 
	MSGDEF(WM_IME_SETCONTEXT);
	MSGDEF(WM_STYLECHANGING);
	MSGDEF(WM_STYLECHANGED);
	MSGDEF(WM_SETTEXT);
	MSGDEF(WM_IME_NOTIFY);
	MSGDEF(WM_IME_CONTROL);
	MSGDEF(WM_IME_COMPOSITIONFULL);
	MSGDEF(WM_IME_SELECT);
	MSGDEF(WM_IME_CHAR);
	MSGDEF(WM_NCLBUTTONDOWN);
	MSGDEF(WM_ENTERSIZEMOVE);
	MSGDEF(WM_EXITSIZEMOVE);
	MSGDEF(WM_ENTERMENULOOP);
	MSGDEF(WM_INITMENU);
	MSGDEF(WM_INITMENUPOPUP);
	MSGDEF(WM_MENUSELECT);
	MSGDEF(WM_MENUCHAR);
	MSGDEF(WM_ENTERIDLE);
	MSGDEF(WM_MENURBUTTONUP);
	MSGDEF(WM_MENUDRAG);
	MSGDEF(WM_MENUGETOBJECT);
	MSGDEF(WM_UNINITMENUPOPUP);
	MSGDEF(WM_MENUCOMMAND);
	MSGDEF(WM_EXITMENULOOP);
	MSGDEF(WM_DROPFILES);

	static wchar_t szMsg[10];
	::swprintf_s(szMsg, 10, L"0x%04X", uMsg);
	return szMsg;
}

// ----------------------------------------------------
// 类窗口
#define DM_DEF_CLASS_NAME							 L"DMClass"					///< 默认类名
#define DM_DEF_SHADOWCLASS_NAME						 L"DMShadowClass"		    ///< 默认阴影类名
#define DM_DEF_WINDOW_NAME							 L"DMWindow"                ///< 默认窗口名
#define DM_DEF_STYLE                                 WS_POPUP|WS_CLIPCHILDREN|WS_TABSTOP

enum
{
	NOMAL_PAINT = 0,       ///< 普通处理绘制
	DUMMY_PAINT = 1,       ///< 蒙版处理绘制
};

enum DMOLEDCFLAGS
{	
	DMOLEDC_NODRAW		= 0x1,		///< 获取画布，转换坐标
	DMOLEDC_PAINTBKGND	= 0x2,		///< 抓背景区域生成初始的画布，转换坐标
	DMOLEDC_OFFSCREEN	= 0x4       ///< 抓窗口区域生成初始的画布，转换坐标
};

// ----------------------------------------------------
// 字符串
#ifndef	   countof
#define    countof(x)								(sizeof(x) / sizeof((x)[0]))
#endif

#ifndef    IsValidString
#define    IsValidString(x)							((x) && (x)[0])
#endif

#define    DMABS(x)                                 ((x)>=0?(x):-(x))

// ----------------------------------------------------
// 代理Delegate,暂未使用,功能测试OK
#define    DMDLBIND(_xxx_,_bbb_)					if(!_xxx_){_xxx_ = _bbb_;}	///< 防止父类重设代理

// ----------------------------------------------------
//  XML节点宏
#define    RES_GLOBAL                               L"global"                   ///< 全局
#define	   RES_LAYOUT								L"layout"					///< 布局
#define    RES_THEMES								L"themes"					///< 主题包列表

#define	   MAINWND_NODE								L"dm"                       ///< 每个主窗口的起始Section名
#define    DUIROOT_NODE								L"root"                     ///< 解析根结点
#define    SUB_NODE								    L"sub"                      ///< 支持sub结点

// ----------------------------------------------------
// DUIWindow消息部分
#ifndef  WM_NCMOUSEFIRST
#define  WM_NCMOUSEFIRST WM_NCMOUSEMOVE
#define  WM_NCMOUSELAST  WM_NCMBUTTONDBLCLK
#endif

// 全局定时器,不允许子类覆盖!!
#define  TIMER_NEXTFRAME							2

// ----------------------------------------------------
// 禁用类不需要的复制构造函数、复制操作符以及隐式的构造函数,来自Chrome：src\base\basictypes.h
#define DM_DISALLOW_COPY(TypeName)					TypeName(const TypeName&)
#define DM_DISALLOW_ASSIGN(TypeName)				void operator=(const TypeName&)
#define DM_DISALLOW_COPY_AND_ASSIGN(TypeName)       TypeName(const TypeName&); void operator=(const TypeName&)
#define DM_DISALLOW_EVIL_CONSTRUCTORS(TypeName)     DM_DISALLOW_COPY_AND_ASSIGN(TypeName)  
#define DM_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) TypeName(); DM_DISALLOW_COPY_AND_ASSIGN(TypeName)


// ----------------------------------------------------
// LOG
#ifdef   DMLOG_ON
#define  LOG_INFO(fmt, ...)							//DMLogDispatch::LogW(DMLOG_INFO,__FUNCTIONW__,__FILEW__, __LINE__,__STR2WSTR(fmt),__VA_ARGS__)
#define  LOG_ERR(fmt, ...)							 DMLogDispatch::LogW(DMLOG_ERR,__FUNCTIONW__,__FILEW__, __LINE__,__STR2WSTR(fmt),__VA_ARGS__)
#else
#define  LOG_INFO(fmt, ...) 
#define  LOG_ERR(fmt, ...)
#endif//DMLOG_ON
// LOG_USER方便在VS调试下定位问题使用，使用完必须清除!!
#define  LOG_USER(fmt, ...)							 DMLogDispatch::LogW(DMLOG_INFO,__FUNCTIONW__,__FILEW__, __LINE__,__STR2WSTR(fmt),__VA_ARGS__)

// ----------------------------------------------------
// 常用删除宏定义
#define DM_DELETE_ARRAY(_xxx_) \
{\
	if (_xxx_) \
{ \
	delete [] _xxx_; \
	_xxx_ = NULL; \
}\
}

#define DM_DELETE(_xxx_) \
{\
	if (_xxx_) \
{ \
	delete _xxx_; \
	_xxx_ = NULL; \
}\
}

#define DM_CLOSEHANDLE(_xxx_) \
{\
	if (_xxx_) \
{ \
	CloseHandle(_xxx_); \
	_xxx_ = NULL; \
}\
}

#define DM_DELETE_OBJECT(_xxx_) \
{\
	if (_xxx_) \
{ \
	::DeleteObject(_xxx_) ; \
	_xxx_ = NULL; \
}\
}

}//namespace DM


