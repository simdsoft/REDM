//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMRes.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once
namespace DM
{
	/// <summary>
	///		资源打包对外扩展接口,classtype=<see cref="DMREG_Res"/>
	/// </summary>
	class DM_EXPORT IDMRes:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMRes,L"IDMRes",DMREG_Res);
	public:
		/// -------------------------------------------------
		/// @brief 加载资源扩展接口
		/// @param[in]		 wp     传入内部使用的参数1，如资源路径等，和自己扩展实现相关
		/// @param[in]		 lp     传入内部使用的参数2，如资源路径等，和自己扩展实现相关
		/// @remark 
		/// @return DM_ECODE_OK为成功
		virtual DMCode LoadResPack(WPARAM wp, LPARAM lp) = 0;

		/// -------------------------------------------------
		/// @brief 判断资源中某项是否存在扩展接口
		/// @param[in]		 lpszType    传入内部使用的参数1，一般为项类型，和自己扩展实现相关
		/// @param[in]		 lpszName    传入内部使用的参数2，一般为项名称，和自己扩展实现相关
		/// @param[in]		 lpszThemeName     主题包名，如为NULL，则使用当前主题包，和自己扩展实现相关
		/// @remark 
		/// @return DM_ECODE_OK为成功
		virtual DMCode IsItemExists(LPCWSTR lpszType, LPCWSTR lpszName,LPCWSTR lpszThemeName=NULL) = 0;

		/// -------------------------------------------------
		/// @brief 获取 资源中某项的内存大小
		/// @param[in]		 lpszType    传入内部使用的参数1，一般为项类型，和自己扩展实现相关
		/// @param[in]		 lpszName    传入内部使用的参数2，一般为项名称，和自己扩展实现相关
		/// @param[out]		 ulSize		 获取内存大小
		/// @param[in]		 lpszThemeName     主题包名，如为NULL，则使用当前主题包，和自己扩展实现相关
		/// @remark DMMain采用先获取大小，再分配大小，再拷贝内存的操作
		/// @return DM_ECODE_OK为成功
		virtual DMCode GetItemSize(LPCWSTR lpszType, LPCWSTR lpszName, unsigned long& ulSize,LPCWSTR lpszThemeName=NULL) = 0;

		/// -------------------------------------------------
		/// @brief 拷贝 资源中某项的内存
		/// @param[in]		 lpszType    传入内部使用的参数1，一般为项类型，和自己扩展实现相关
		/// @param[in]		 lpszName    传入内部使用的参数2，一般为项名称，和自己扩展实现相关
		/// @param[in]		 lpBuf		 外部分配内存指针
		/// @param[in]		 ulSize		 外部分配内存大小
			/// @param[in]		 lpszThemeName     主题包名，如为NULL，则使用当前主题包，和自己扩展实现相关
		/// @remark DMMain采用先获取大小，再分配大小，再拷贝内存的操作，结合<see cref="GetItemSize"/>
		/// @return DM_ECODE_OK为成功
		virtual DMCode GetItemBuf(LPCWSTR lpszType, LPCWSTR lpszName, LPVOID lpBuf, unsigned long ulSize,LPCWSTR lpszThemeName=NULL) = 0;

		/// -------------------------------------------------
		/// @brief 加载主题包
		/// @param[in]		 wp     传入内部使用的参数1，如资源路径等，和自己扩展实现相关
		/// @param[in]		 lp     传入内部使用的参数2，如资源路径等，和自己扩展实现相关
		/// @remark 此函数可用于从外部动态下载主题包加载进去
		/// @return DM_ECODE_OK为成功
		virtual DMCode LoadTheme(WPARAM wp, LPARAM lp) = 0;

		/// -------------------------------------------------
		/// @brief 设置当前主题包
		/// @param[in]		 lpszName     主题包名字
		/// @param[in]		 lpszOldName  当前使用的主题包名字 ，请外部分配足够的buf
		/// @remark 默认会使用名字为Default的主题包
		/// @return DM_ECODE_OK为成功
		virtual DMCode SetCurTheme(LPCWSTR lpszName, LPCWSTR lpszOldName=NULL) = 0;
	};

}//namespace DM