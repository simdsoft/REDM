//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMLayout.h 
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
	///		锚点布局对外扩展接口,classtype=<see cref="DMREG_Layout"/>
	/// </summary>
	class IDMLayout:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMLayout,L"IDMLayout",DMREG_Layout);
	public:
		virtual  DMCode SetOwner(LPVOID lpWnd) = 0;									///< 设置拥有此layout的DUI窗口
		virtual  DMCode UpdateLayout(LPRECT prcContainer, CRect &rcWindow) = 0;		///< 更新自身大小
		virtual  DMCode UpdateChildLayout() = 0;									///< 更新拥有此layout的DUI窗口的所有子窗口大小
	};
}//namespace DM