// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	Define.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-7
// ----------------------------------------------------------------
#pragma once

#pragma execution_character_set("utf-8")

// ----------------
// 宏
#define  XML_LAYOUT_DMINDEX                          L"layout\\dmindex.xml"
#define  XML_THEMES_DMINDEX                          L"themes\\dmindex.xml"
#define  XML_GLOBAL                                  "global"
#define  XML_THEMES                                  "themes"
#define  XML_SKIN                                    "skin"
#define  XML_STYLE                                   "style"
#define  XML_FILE                                    "file"
#define  XML_PATH                                    "path"
#define  XML_NAME                                    "name"
#define  XML_DESC                                    "desc"
#define  XML_VALUE                                   "value"
#define  XML_OPTIONS                                 "options"
#define  XML_ID										 "id"
#define  XML_SRC									 "src"
#define  XML_TREEITEM                                "treeitem"
#define  XML_STATIC                                  "label"
#define  XML_CHECKBOX                                "checkbox"
#define  XML_CHEKSKIN                                "checkskin"
#define  XML_TEXT                                    "text"
#define  XML_LAYOUT                                  "layout"
#define  XML_CLRTEXT                                 "clrtext"
#define  XML_POS                                     "pos"
#define  XML_CUSTOMSKIN                              "ds_custombutton"
#define  XML_DATASKIN								 "ds_databutton"
#define  XML_DEFSKIN								 "ds_defbutton"
#define  XML_DM										 "dm"
#define  XML_BRESIZE                                 "bresize"
#define  XML_BTRANSLUCENT							 "btranslucent"
#define  XML_CHILDOFFSET                             "childoffset"
#define  XML_FONT									 "font"
#define  XML_ITEM									 "item"
#define  XML_SEP									 "sep"
#define  XML_ALPHA                                   "alpha"
#define  XML_BSHADOW                                 "bshadow"
#define  XML_BDISABLE                                "bdisable"

// 属性xml
#define  ATTR_FILE									 L".\\DesignerRes\\DMAttr.xml"
#define  CLS_FILE									 L".\\DesignerRes\\DMCls.xml"
#define  ADD_FILE									 L".\\DesignerRes\\DMAdd.xml"
#define  ADD_DG_FILE								 L".\\DesignerRes\\DMAdd_dg.xml"
#define  REC_FILE                                    L".\\DMDesigner.xml"
#define  NEW_DIR                                     L".\\DesignerRes\\DMNewRes"
#define  NEW_DG_DIR                                  L".\\DesignerRes\\DMNewRes_dg"

#define  PROJTREE_THEMES                             L"主题包列表"
#define  PROJTREE_LAYOUTFILES                        L"文件列表"
#define  PROJTREE_GLOBALSTYLESKINS                   L"样式-皮肤(全局)列表"
#define  PROJTREE_GLOBALSTYLS                        L"样式池(全局)列表"
#define  PROJTREE_GLOBALSKINS                        L"皮肤池(全局)列表"

#define  PROJTREE_PRIVSTYLESKINS                     L"样式-皮肤(私有)列表"
#define  PROJTREE_PRIVSTYLES                         L"样式池(私有)列表"
#define  PROJTREE_PRIVSKINS                          L"皮肤池(私有)列表"

// ----------------
// 枚举
enum ProjMenuId
{
	PROJMENU_BASE    = 8000,
	PROJMENU_OPENDIR = 8000,
	PROJMENU_SCAN,

	PROJMENU_ADDSKINPOOL,
	PROJMENU_DELSKINPOOL,
	PROJMENU_EDITSKINPOOL,

	PROJMENU_ADDSTYLEPOOL,
	PROJMENU_DELSTYLEPOOL,
	PROJMENU_EDITSTYLEPOOL,

	PROJMENU_ADDSKIN,
	PROJMENU_DELSKIN,
	PROJMENU_EDITSKIN,

	PROJMENU_ADDSTYLE,
	PROJMENU_DELSTYLE,
	PROJMENU_EDITSTYLE,

	PROJMENU_ADDIMGTYPE,
	PROJMENU_DELIMGTYPE,
	PROJMENU_EDITIMGTYPE,

	PROJMENU_ADDTHEME,
	PROJMENU_DELTHEME,
	PROJMENU_EDITTHEME,
	PROJMENU_SETDEFTHEME,

	PROJMENU_ADDIMG,
	PROJMENU_DELIMG,
	PROJMENU_EDITIMG,
	
	PROJMENU_ADDLAYOUT,
	PROJMENU_DELLAYOUT,
	PROJMENU_EDITLAYOUT,
};
enum ObjMenuId
{
	OBJMENU_BASE    = 9000,
	OBJMENU_OPENDIR = 9000,
	OBJMENU_PASTE,
	OBJMENU_COPY,
	OBJMENU_LOCKALLCHILD,
	OBJMENU_UNLOCKALLCHILD,
	OBJMENU_DEL,
};
enum GlbMenuId
{
	GLBMENU_BASE   = 10000,
	GLBMENU_NEW    = 10000,
	GLBMENU_SAVE,
	GLBMENU_CLOSE,
	GLBMENU_EXIT,

	GLBMENU_UNDO,
	GLBMENU_REDO,

	GLBMENU_OPTOBJPROJ,
	GLBMENU_RELOAD,

	GLBMENU_MOVEMODE,
	GLBMENU_SELMODE,
	GLBMENU_ADDMODE,

	GLBMENU_HELPDOC,
	GLBMENU_ONLINEHELP,
};

// 指定List的项索引
enum
{
	HOST_ATTR = 0,
	WIGDET_ATTR = 1,
	POS_ATTR = 2,
};

// ToolBar的设计模式
enum DesignMode {NoneMode=-1, MoveMode,SelectMode,AddMode};

// ----------------
// 结构体
struct DesignMenu
{
	int        id;
	CStringA   text;
};

struct MultStr
{
	CStringA strOne;
	CStringA strTwo;
	CStringA strThree;
	CStringA strFour;
};

