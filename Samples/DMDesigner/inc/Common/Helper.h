// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	Helper.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-7
// ----------------------------------------------------------------
#pragma once

#pragma execution_character_set("utf-8")

bool IsDirectoryExist(CStringW strDir);///< 判断路径是否为文件夹
bool OpenFolderAndSelectFile(CStringW strFilePath);///< 如果文件夹已经打开，就不用再重新打开一次，直接置顶显示，并选中目标文件http://www.cnblogs.com/shankun/p/4088860.html
bool CopyDirectory(CStringW strSrcDir,CStringW strDestDir);///< 复制文件夹，注意，如果目标文件夹已存在，则在源文件夹为名复制到目标文件夹的子目录下
bool SetLogInfo(CStringW strInfo);
DMXmlDocument* GetExistXmlDoc(LPCSTR lpszType,LPCSTR lpszResName);///< 获得已解析过的xml doc(未保存时可能和xml文件不同）

int DM_MessageBox(LPCSTR lpText, UINT uType = MB_OK, LPCSTR lpCaption = "MSG", HWND hWnd = NULL);
int DM_MessageBox(LPCWSTR lpText,UINT uType = MB_OK, LPCWSTR lpCaption = L"MSG",HWND hWnd = NULL); 
int StringToInt(CStringW str);
CStringW IntToString(int id);
bool IsUseDgSkin();

// 绘制函数二次封装
DMCode AutoDrawText(IDMCanvas*pCanvas,LPCSTR strFont,DMColor TextClr,LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha=0xFF);
DMCode AutoDrawRoundRect(IDMCanvas*pCanvas,DMColor TextClr,int iStyle,int iWidth,LPCRECT lpRect,const POINT &pt);
DMCode AutoFillRoundRect(IDMCanvas*pCanvas,DMColor BrushClr,LPCRECT lpRect,const POINT &pt);

// Hook二次封装,hook仅用于处理prop的系统弹出框(颜色框、字体框),无关代码,所以此部分hook不开源
class DMHook
{
public:
	DMHook();
	~DMHook();
	HMODULE m_hDll;
};

// 每个xml属性绑定一个DMXmlInitAttr,用于区分此属性是否已被使用,被使用的属性不会出现在expand框中
class DMXmlInitAttr
{
public:
	DMXmlInitAttr(DMXmlAttribute& XmlAttr);
	~DMXmlInitAttr();

public:
	DMXmlAttributePtr      m_pAttr;
	bool                   m_bUse;
	CStringA               m_strReserve;
};
typedef DMXmlInitAttr* DMXmlInitAttrPtr;

// DMAttr.xml记录了所有属性的prop类型,初始值,描述,可由DM库自动生成.
// DMCls.xml记录了控件->属性集合
// DMAdd.xml用于记录add模式下默认的属性
// DMDesigner.xml记录了最近打开的项目
class DMCtrlXml
{
public:
	DMCtrlXml();
	~DMCtrlXml();
	
	bool Parse(DMXmlAttributePtr ptr,CStringA &strType,CStringA &strName,CStringA& strValue,CStringA &strDesc);
	bool ParseName(DMXmlAttributePtr ptr,CStringA &strType,CStringA &strName);
	bool Parse(CStringA strClsName,CStringAList &strList);///< 从cls中解析中组成它的不同属性集名

	DMXmlNode Parse(CStringA strReg);///< 从注册类名返回它的默认XmlNode
	DMXmlNode GetAttrNode(CStringA strNode);

	bool SetCopyNode(DMXmlNodePtr pNode, CRect rcWnd);
	
	bool AddRecentResDir(CStringW strNewDir);
	bool RemoveRecentResDir(CStringW strNewDir);
public:
	DMXmlDocument	m_AttrDoc;
	DMXmlDocument   m_ClsDoc;  
	DMXmlDocument   m_AddDoc;
	DMXmlDocument   m_AddDgDoc;

	// copy
	DMXmlDocument   m_CopyDoc;
	DMXmlNode       m_CopyNode;
	CRect           m_rcCopy;

	// recent
	DMXmlDocument   m_RecentDoc;
};

static MultStr g_stAttrDesc[] = \
{  // 类型       默认描述														prop类型        默认值
	{"COLOR",	 "颜色使用预乘方式PBGRA:\r\n clr=\"PBGRA(ff,ff,ff,ff)\"",	"propclr",		"pbgra(ff,ff,ff,ff)"},
	{"INT",	 "整数型有各种用途,用于不定需求",								"propint",		"0"},
	{"RECT",	 "一般用于指定四个值,比如9宫格,非客户端的四边等",				"proprect",	"0,0,0,0"},
	{"BYTE",	 "字型一般用于指定alpha,限制在[0-255]之间",					"propbyte",	"255"},
	{"FONT",	 "字体使用字符串来描述,以逗号分隔",							"propfont",	"weight:400,underline:0,italic:0,strike:0,size:14,face:新宋体"},
	{"accel",   "快捷键相关设置",											"propaccel",	""},
	{"SIZE",    "一般用于指定2个值,比如大小",								"propsize",	"0,0"},
	{"POINT",   "一般用于指定2个值,比如坐标",								"propsize",	"0,0"},
	{"bool",    "布尔型可使用0或非0,或字符串yes或no",						"propoption",  ""},
	{"OPTION",	 "可选项限制了选择的类型:\r\n align=\"left\"",				"propoption",	""},
	{"STRING",  "字符串有各种用途,用于不定需求",								"propstring",	""},
	{"SKIN",	 "皮肤指明了某个图片的绘制方式",								"propstring",	""},
};
