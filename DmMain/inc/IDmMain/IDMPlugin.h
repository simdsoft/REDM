
//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMPlugin.h 
// File Des: 插件接口
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-16	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		插件接口借鉴于OGRE.每个插件应该做4件事:安装、初始化、 关闭、卸载
	/// </summary>
	/// <remarks>
	///     此插件接口即可以为dll所用，也可以为静态lib所用
	/// </remarks>
	class IDMPlugin
	{
	public:
		IDMPlugin(){}
		virtual~IDMPlugin(){}

		virtual const wchar_t* GetName() const = 0;

		//---------------------------------------------------
		// Function Des: 不依赖于系统初始化
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		virtual void Install() = 0;			///< 不依赖于系统初始化

		//---------------------------------------------------
		// Function Des: 最后的清理工作
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		virtual void Uninstall() = 0;		///< 最后的清理工作

		//---------------------------------------------------
		// Function Des: 依赖于系统初始化
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		virtual void Initialise() = 0;		///< 依赖于系统初始化

		//---------------------------------------------------
		// Function Des: 在系统关键部分没有卸载时做的清理工作
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		virtual void Shutdown() = 0;		///< 在系统关键部分没有卸载时做的清理工作
	};
}//namespace DM
