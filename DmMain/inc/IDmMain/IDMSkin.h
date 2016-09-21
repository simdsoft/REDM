//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMSkin.h  
// File Des: IDMSkin是一个通用的抽象类，它的目的是告诉你怎么画一样东西到Canvas上
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
	///		皮肤抽象类,如状态图、9宫格、动画、,classtype=<see cref="DMREG_Skin"/>
	/// </summary>
	/// <remarks>
	///		部分接口未实现的，会返回DM_ECODE_NOTIMPL
	/// </remarks>
	class IDMSkin:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMSkin,L"IDMSkin",DMREG_Skin);
	public:
		/// -------------------------------------------------
		/// @brief			取得每个实例化Skin对象的唯一ID    
		/// @return			DMCode
		virtual DMCode GetID(wchar_t* lpszId, int iSize) = 0;											              
		virtual	DMCode Draw(IDMCanvas *pCanvas, LPCRECT lpRectDraw, int iState,BYTE alpha=0xFF) = 0;	///<按状态绘制
		virtual DMCode GetStateSize(SIZE &sz, int iState=0) = 0;                                        ///<取得指定状态的大小
		virtual DMCode GetStates(int &iStates) = 0;													    ///<取得状态数
		virtual DMCode GetBitmap(IDMBitmap** ppBitmap) = 0;                                             ///<取得bitmap
		virtual DMCode SetBitmap(LPBYTE pBuf,size_t szLen,LPCWSTR pszType) = 0;							///<通过内存设置bitmap
		virtual DMCode UpdateSkin(WPARAM wp, LPARAM lp) = 0;										    ///<换肤
		virtual DMCode IsValid() = 0;                                                                   ///<判断skin是否有效，如果无效,内部skin池将自动移除此skin
	};
	typedef IDMSkin* IDMSkinPtr;

}//namespace DM