// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIDateTimeCtrl.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2018-12-3
// ----------------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIDateTimeCtrl"/>的xml属性定义
	/// </summary>
	class DUIDateTimeCtrlAttr:public DUIWindowAttr
	{
	public:
	};
}

namespace DM
{
	/// <summary>
	///		 DUIDateTimeCtrl的内置实现，属性：<see cref="DMAttr::DUIDateTimeCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUIDateTimeCtrl:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIDateTimeCtrl,DUINAME_DateTimeCtrl,DMREG_Window);
	public:
		DUIDateTimeCtrl();
	};

}// namespace DM