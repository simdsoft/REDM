//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMAttribute.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-10	1.0			
//-------------------------------------------------------
#pragma once
namespace DM
{
	/// <summary>
	///		XML属性解宏对外扩展接口,classtype=<see cref="DMREG_Attribute"/>
	/// </summary>
	/// <remarks>
	///     如果使用者觉得希望加入自己的方式解析XML,那么重载它吧!
	/// </remarks>
	class IDMAttribute:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMAttribute,L"IDMAttribute",DMREG_Attribute);
	public:
		virtual DMCode ParseInt(LPCWSTR lpszValue,int&Obj) = 0;				///<把lpszValue解析成Obj(int)
		virtual DMCode ParseBool(LPCWSTR lpszValue,bool&Obj) = 0;			///<把lpszValue解析成Obj(bool)
		virtual DMCode ParseColor(LPCWSTR lpszValue,DMColor&Obj) = 0;		///<把lpszValue解析成Obj(COLORREF)
		virtual DMCode ParseSize(LPCWSTR lpszValue,DM::CSize&Obj) = 0;		///<把lpszValue解析成Obj(CSize)
		virtual DMCode ParsePoint(LPCWSTR lpszValue,DM::CPoint&Obj) = 0;	///<把lpszValue解析成Obj(Obj)
		virtual DMCode ParseRect(LPCWSTR lpszValue,DM::CRect&Obj) = 0;		///<把lpszValue解析成Obj(CRect)
	};
}
