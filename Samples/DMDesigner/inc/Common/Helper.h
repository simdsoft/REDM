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

bool IsDirectoryExist(CStringW strDir);///< 判断路径是否为文件夹
bool OpenFolderAndSelectFile(CStringW strFilePath);///< 如果文件夹已经打开，就不用再重新打开一次，直接置顶显示，并选中目标文件http://www.cnblogs.com/shankun/p/4088860.html
bool CopyDirectory(CStringW strSrcDir,CStringW strDestDir);///< 复制文件夹，注意，如果目标文件夹已存在，则在源文件夹为名复制到目标文件夹的子目录下
bool SetLogInfo(CStringW strInfo);

int DM_MessageBox(LPCWSTR lpText,UINT uType = MB_OK, LPCWSTR lpCaption = L"MSG",HWND hWnd = NULL); 
int StringToInt(CStringW str);
CStringW IntToString(int id);

// 绘制函数二次封装
DMCode AutoDrawText(IDMCanvas*pCanvas,CStringW strFont,DMColor TextClr,LPCWSTR lpString, int nCount, LPRECT lpRect, UINT uFormat,BYTE alpha=0xFF);
DMCode AutoDrawRoundRect(IDMCanvas*pCanvas,DMColor TextClr,int iStyle,int iWidth,LPCRECT lpRect,POINT &pt);
DMCode AutoFillRoundRect(IDMCanvas*pCanvas,DMColor BrushClr,LPCRECT lpRect,POINT &pt);

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
	CStringW               m_strReserve;
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
	
	bool Parse(DMXmlAttributePtr ptr,CStringW &strType,CStringW &strName,CStringW& strValue,CStringW &strDesc);
	bool ParseName(DMXmlAttributePtr ptr,CStringW &strType,CStringW &strName);
	bool Parse(CStringW strClsName,CStringWList &strList);///< 从cls中解析中组成它的不同属性集名

	DMXmlNode Parse(CStringW strReg);///< 从注册类名返回它的默认XmlNode

	bool SetCopyNode(DMXmlNodePtr pNode, CRect rcWnd);
	
	bool AddRecentResDir(CStringW strNewDir);
	bool RemoveRecentResDir(CStringW strNewDir);
public:
	DMXmlDocument	m_AttrDoc;
	DMXmlDocument   m_ClsDoc;  
	DMXmlDocument   m_AddDoc;

	// copy
	DMXmlDocument   m_CopyDoc;
	DMXmlNode       m_CopyNode;
	CRect           m_rcCopy;

	// recent
	DMXmlDocument   m_RecentDoc;
};

static MultStr g_stAttrDesc[] = \
{  // 类型       默认描述														prop类型        默认值
	{L"COLOR",	 L"颜色使用预乘方式PBGRA:\r\n clr=\"PBGRA(ff,ff,ff,ff)\"",		L"propclr",		L"pbgra(ff,ff,ff,ff)"},
	{L"INT",	 L"整数型有各种用途,用于不定需求",								L"propint",		L"0"},
	{L"RECT",	 L"一般用于指定四个值,比如9宫格,非客户端的四边等",				L"proprect",	L"0,0,0,0"},
	{L"BYTE",	 L"字型一般用于指定alpha,限制在[0-255]之间",					L"propbyte",	L"255"},
	{L"FONT",	 L"字体使用字符串来描述,以逗号分隔",							L"propfont",	L"weight:400,underline:0,italic:0,strike:0,size:14,face:新宋体"},
	{L"accel",   L"快捷键相关设置",												L"propaccel",	L""},
	{L"SIZE",    L"一般用于指定2个值,比如大小",									L"propsize",	L"0,0"},
	{L"POINT",   L"一般用于指定2个值,比如坐标",									L"propsize",	L"0,0"},
	{L"bool",    L"布尔型可使用0或非0,或字符串yes或no",							L"propoption",  L""},
	{L"OPTION",	 L"可选项限制了选择的类型:\r\n align=\"left\"",					L"propoption",	L""},
	{L"STRING",  L"字符串有各种用途,用于不定需求",								L"propstring",	L""},
	{L"SKIN",	 L"皮肤指明了某个图片的绘制方式",								L"propstring",	L""},
};
