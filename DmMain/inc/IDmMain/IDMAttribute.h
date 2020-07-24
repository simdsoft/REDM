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
		DMDECLARE_CLASS_NAME(IDMAttribute,"IDMAttribute",DMREG_Attribute);
	public:
		virtual DMCode ParseInt(LPCSTR lpszValue,int&Obj) = 0;				///<把lpszValue解析成Obj(int)
		virtual DMCode ParseBool(LPCSTR lpszValue,bool&Obj) = 0;			///<把lpszValue解析成Obj(bool)
		virtual DMCode ParseColor(LPCSTR lpszValue,DMColor&Obj) = 0;		///<把lpszValue解析成Obj(COLORREF)
		virtual DMCode ParseSize(LPCSTR lpszValue,DM::CSize&Obj) = 0;		///<把lpszValue解析成Obj(CSize)
		virtual DMCode ParsePoint(LPCSTR lpszValue,DM::CPoint&Obj) = 0;	///<把lpszValue解析成Obj(Obj)
		virtual DMCode ParseRect(LPCSTR lpszValue,DM::CRect&Obj) = 0;		///<把lpszValue解析成Obj(CRect)
	};
}
