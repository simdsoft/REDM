//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIStatic.h 
// File Des: 内置static实现
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-28	1.0			
//--------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIStatic"/>的xml属性定义
	/// </summary>
	class DUIStaticAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* bool_bmultiLines;                                     ///< 是否支持多行,示例:bmultiLines="1"
		static wchar_t* INT_lineinter;                                        ///< 行间距,默认为5，示例:lineinter="5"
	};
	DMAttrValueInit(DUIStaticAttr,bool_bmultiLines)DMAttrValueInit(DUIStaticAttr,INT_lineinter)
}

namespace DM
{
	/// <summary>
	///		 DUIStatic的内置实现
	/// </summary>
	class DM_EXPORT DUIStatic:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIStatic,DUINAME_Static,DMREG_Window);
	public:
		DUIStatic();

		//---------------------------------------------------
		// Function Des: 重载DUIWindow
		//---------------------------------------------------
		DMCode DV_DrawText(IDMCanvas* pCanvas, LPCWSTR pszBuf,int cchText,LPRECT lpRect,UINT uFormat);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUIStaticAttr::bool_bmultiLines, m_bMultiLines, DM_ECODE_OK)
			DM_INT_ATTRIBUTE(DMAttr::DUIStaticAttr::INT_lineinter, m_nLineInter, DM_ECODE_OK)
		DM_END_ATTRIBUTES()
	public:
		bool                                m_bMultiLines;
		int                                 m_nLineInter;
	};

}//namespace DM