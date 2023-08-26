//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DDMDispatchMgr.h 
// File Des:统一管理分发、转发类
// File Summary: Mgr为Manager缩写
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-17	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{

	/// <summary>
	///		以静态方式转发xml属性解析函数<see cref="IDMAttribute"/>
	/// </summary>
	class DM_EXPORT DMAttributeDispatch
	{
	public:
		static void SetAttributeDispatch(IDMAttribute* pAttributeObj);
		static DMCode ParseInt(LPCSTR lpszValue,int&Obj);
		static DMCode ParseBool(LPCSTR lpszValue,bool&Obj);
		static DMCode ParseColor(LPCSTR lpszValue,DMColor&Obj);
		static DMCode ParseSize(LPCSTR lpszValue,DM::CSize&Obj);
		static DMCode ParsePoint(LPCSTR lpszValue,DM::CPoint&Obj);
		static DMCode ParseRect(LPCSTR lpszValue,DM::CRect&Obj);
	public: static IDMAttribute *m_pAttributeObj;
	};

	/// <summary>
	///		以静态方式转发Log函数<see cref="IDMLog"/>
	/// </summary>
	class DM_EXPORT DMLogDispatch
	{
	public:
		static void SetLogDispatch(IDMLog* pLogObj);
		static DMCode LogW(DMLOGLEVEL iLevel,LPCWSTR lpszFuncName, LPCWSTR lpszFileName, int iLine,LPCWSTR szFmt,...);
	public:static IDMLog *m_pLogObj;
		   static DMLock  m_Lock;
	};

}//namespace DM

namespace DMAttr
{
	class DM_EXPORT DMInitAttrDispatch
	{
	public:
		static const char* GetAttrValue(const char* cls, const char *pBuf);
	};
}