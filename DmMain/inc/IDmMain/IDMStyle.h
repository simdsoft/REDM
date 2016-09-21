//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMStyle.h  
// File Des: IDMStyle提供Style的各种对外接口
// File Summary: 参考Android Drawable
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-28	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		 属性抽象类,classtype=<see cref="DMREG_Skin"/>
	/// </summary>
	/// <remarks>
	///		部分接口未实现的，会返回DM_ECODE_NOTIMPL
	/// </remarks>
	class IDMStyle:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMStyle,L"IDMStyle",DMREG_Style);
	public:
		/// -------------------------------------------------
		/// @brief			取得每个实例化Style对象的唯一ID    
		/// @return			DMCode
		virtual DMCode GetID(wchar_t* lpszId, int iSize) = 0;
		virtual DMCode CopyData(IDMStyle* pStyle) = 0;

		//---------------------------------------------------
		// Function Des: 文本相关
		//---------------------------------------------------
		virtual DMCode GetTextAlign(UINT &ulAlign) = 0;                                          ///<获取文本Format
		virtual DMCode GetTextColor(int iState,DMColor &Clr) = 0;                                ///<取得状态文本颜色
		virtual DMCode GetTextFont(int iState, IDMFont** ppFont) = 0;                            ///<取得状态字体,内部增加引用计数，外部释放 
		virtual DMCode GetTextStates(int &iState) = 0;											 ///<取得文本状态数
	
		//---------------------------------------------------
		// Function Des: 光标相关
		//---------------------------------------------------
		virtual DMCode GetCursor(HCURSOR &hCursor) = 0;											 ///<取得光标名

		//---------------------------------------------------
		// Function Des: 窗口相关
		//---------------------------------------------------
		virtual DMCode GetBgSkin(IDMSkin** ppSkin) = 0;											 ///<取得背景skin
		virtual DMCode GetBgColor(int iState,DMColor &Clr) = 0;                                  ///<取得背景颜色	
		virtual DMCode GetBgClrStates(int  &iState) = 0;                                         ///<取得背景(客户-非客户)颜色状态数

		virtual DMCode GetNcMargin(LPRECT lpRect) = 0;				                             ///<取得非客户区四边框
		virtual DMCode GetNcColor(int iState, DMColor &Clr) = 0;                                 ///<取得非客户区颜色
		virtual DMCode GetNcSkin(IDMSkin** ppSkin) = 0;											 ///<取得非客户区skin

		virtual DMCode GetAlpha(byte &alpha) = 0;												 ///<取得窗口透明度
	};
	typedef IDMStyle* IDMStylePtr;

}//namespace DM