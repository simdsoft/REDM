//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMDynLib.h 
// File Des: 加载dll形式的plugin
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
	///		每个dll插件对应一个此对象,移植自ogre插件系统，可参考OgreDynLib.h/.cpp
	/// </summary>
	class DMDynLib
	{
	public:
		DMDynLib(const CStringW &strPluginPath);
		virtual ~DMDynLib(void);

		bool Load();									///<加载
		void Unload();									///<卸载

		const CStringW& GetName()const;					///<获取插件名
		void* GetSymbol(const char* pProcName)const;	///<获取导出函数地址

	protected:
		CStringW DynlibError(void);						///<获取加载失败原因

	protected:
		CStringW			m_strPluginPath;			///<插件的路径
		HMODULE             m_hModule;					///<插件加载的handle
	};

}//namespace DM
