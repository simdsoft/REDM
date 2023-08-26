// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ResFolder.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once
#include "ResHelper.h"

namespace DM
{
	/// <summary>
	///		内置Res打包文件夹为以下结构:如果你觉得不能满足需求，可以自己修改一份，自己注册进去
	///     Res文件夹包含两个子文件夹：1.layout文件夹 2.themes文件夹
	///		1.1.layout文件夹定义了所有布局xml和skin、style的全局xml
	///     2.1.themes文件夹下定义了多个主题包
	///     3.每个文件夹下均有一个dmindex.xml来标识资源索引
	/// </summary>
	class  ResFolder:public IDMRes,public DMArrayT<ResFolderItemPtr>
						 //,public DMMapT<CStringW,ResFolderItemPtr>// themes的列表Map
	{
		DMDECLARE_CLASS_NAME(ResFolder,"ResFolder",DMREG_Res);
	public:
		ResFolder();
		~ResFolder();
	public:
		virtual DMCode LoadResPack(WPARAM wp, LPARAM lp);
		virtual DMCode IsItemExists(LPCSTR lpszType, LPCSTR lpszName, LPCSTR lpszThemeName=NULL);
		virtual DMCode GetItemSize(LPCSTR lpszType, LPCSTR lpszName, unsigned long& ulSize, LPCSTR lpszThemeName=NULL);
		virtual DMCode GetItemBuf(LPCSTR lpszType, LPCSTR lpszName, DMBufT<byte>&, PULONG lpULSize, LPCSTR lpszThemeName = NULL);
		virtual DMCode LoadTheme(WPARAM wp, LPARAM lp);
		virtual DMCode SetCurTheme(LPCSTR lpszName, LPCSTR lpszOldName=NULL);
		virtual DMCode SendExpandInfo(WPARAM wp, LPARAM lp);  ///< lp为0时，把长度赋值到*wp

	public:// 辅助
		DMCode ParseThemes(LPCWSTR lpszIndexPath);
		DMCode ParseIndex(LPCWSTR lpszIndexPath,ResFolderItem** ppItem);
		LPCWSTR GetItemPath(LPCSTR lpszType, LPCSTR lpszName, LPCSTR lpszThemeName);
		ResFolderItemPtr FindResItemObj(LPCSTR lpszName);
		DMCode RemoveResItem(LPCSTR lpszType, LPCSTR lpszName, LPCSTR lpszThemeName);
		DMCode AddResItem(LPCSTR lpszType, LPCSTR lpszName,LPCWSTR lpszPath, LPCSTR lpszThemeName);

	protected:
		virtual void PreArrayObjRemove(const ResFolderItemPtr &obj);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE("bloop", m_bThemeLoop, DM_ECODE_OK)
			DM_STRING_ATTRIBUTE("name", m_strCurTheme, DM_ECODE_OK)
			DM_bool_ATTRIBUTE("boutstyle", m_bOutStyle, DM_ECODE_OK)
			DM_bool_ATTRIBUTE("bassert",m_bAssert, DM_ECODE_OK)
		DM_END_ATTRIBUTES()
	
	public:
		DM::CStringW                          m_strDir;
		DM::CStringA                          m_strCurTheme;       // 当前使用的theme Key
		ResFolderItemPtr			          m_pCurTheme;         // 当前使用的theme
		bool                                  m_bThemeLoop;        // 是否启用循环查找theme(当前theme中找不到资源,直接去其他theme中查找)
		bool                                  m_bOutStyle;         // 当前是外部模式（外部强制改变了skin池的数据，这时所有的theme应该都需要刷新)
		bool                                  m_bAssert;		   // 是否在debug弹出警告
		DMSmartPtrT<ResFolderItem>			  m_pLayout;		   // Layout的列表
		
	};

}//namespace DM