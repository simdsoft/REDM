//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMResHelper.h 
// File Des:  内置Res辅助文件
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-19	1.0			
//-------------------------------------------------------
#pragma once
namespace DM
{

#define MAX_RES_TYPE        30						 //资源类型预定义，最大长度不超过30
#define MAX_RES_NAME        100						 //注意：给资源令名时，最大长度不要超过MAX_RES_NAME
#define DMRES_DMINDEX		   _T("dmindex.xml")    //文件夹资源的文件映射表索引表文件名
#define DMRES_THEMES_FOLDER    _T("\\themes\\")
#define DMRES_LAYOUT_FOLDER    _T("\\layout\\")

	/// <summary>
	///		内置Res项包含属性
	/// </summary>
	class DM_EXPORT DMResItem
	{
	public:
		DMResItem(LPCWSTR lpszType=L"", LPCWSTR lpszName=L"", LPCWSTR lpszPath = L"");

	public:
		wchar_t m_szType[MAX_RES_TYPE+1];			///< 资源类型,如png,jpg,bmp,gif,flash
		wchar_t m_szName[MAX_RES_NAME+1];           ///< 资源名,同一资源类型下唯一标识此资源项
		wchar_t m_szPath[MAX_PATH];                 ///< 资源全路径
	};

	typedef DMResItem* DMResItemPtr;

	/// <summary>
	///		内置ResFolder使用的Array
	/// </summary>
	class DMResFolderItem:public DMRefNum
						 ,public DMArrayT<DMResItemPtr>
	{
	public:
		DMResFolderItem(){}
		virtual ~DMResFolderItem();
	public:
		virtual void PreArrayObjRemove(const DMResItemPtr &obj);
		virtual bool EqualArrayObj(const DMResItemPtr &objsrc, const DMResItemPtr &objdest);
	public:
		CStringW        m_strThemeName;			 ///< 主题包名字
	};
	typedef DMResFolderItem* DMResFolderItemPtr;

}//namespace DM