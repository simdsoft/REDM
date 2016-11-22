// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	IDMTrans.h
// File mark:   
// File summary:翻译类接口
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-11-21
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		翻译类对外扩展接口,classtype=<see cref="DMREG_Trans"/>
	/// </summary>
	class DM_EXPORT IDMTrans : public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMTrans,L"IDMTrans",DMREG_Trans);
	public:
		/// -------------------------------------------------
		/// @brief 加载翻译包扩展接口
		/// @param[in]		 wp					传入内部使用的参数1，如翻译XML路径等，和自己扩展实现相关，内置为xml的ld或xml全路径
		/// @param[in]		 lp					传入内部使用的参数2，如翻译XML等，和自己扩展实现相关,内置0表示xml id方式,1表示xml相对于exe的路径
		/// @return DM_ECODE_OK为成功
		virtual DMCode LoadTrans(WPARAM wp, LPARAM lp = 0) = 0;

		/// -------------------------------------------------
		/// @brief 设置默认语言包
		// @param[in]		 strLanguageName	对应的语言包唯一标识，如果传NULL,则不使用翻译 
		/// @return DM_ECODE_OK为成功
		virtual DMCode SetTrans(CStringW strLanguageName) = 0;

		/// -------------------------------------------------
		/// @brief 新加翻译字符串
		/// @param[in]		 strSrc				XML中配置的字符串
		/// @param[in]		 strTrans			被翻译的字符串
		/// @param[in]		 strNodeName		是否指定为特定的配置,为NULL表示从通用的配置中查找
		/// @param[in]		 strLanguageName	对应的语言包唯一标识,为NULL使用当前SetTrans指定的语言包
		/// @return DM_ECODE_OK为成功
		virtual DMCode AddTransItem(CStringW strSrc, CStringW strTrans,CStringW strNodeName = L"",CStringW strLanguageName = L"") = 0;
		
		/// -------------------------------------------------
		/// @brief 获得翻译字符串
		/// @param[in]		 strSrc				XML中配置的字符串
		/// @param[in]		 strNodeName		是否指定为特定的配置
		/// @param[in]		 strLanguageName	对应的语言包唯一标识,为NULL使用当前SetTrans指定的语言包
		/// @return 转换后的字符串
		virtual CStringW GetTrans(CStringW strSrc,CStringW strNodeName = L"",CStringW strLanguageName = L"") = 0;
	};
}