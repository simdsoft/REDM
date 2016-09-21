//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMDynLibMgr.h 
// File Des: 管理plugin句柄列表<plugin handle--dll name>
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-16	1.0			
//-------------------------------------------------------
#pragma once
#include "DMDynLib.h"

namespace DM
{
	/// <summary>
	///		管理<see cref="DMDynLib"/>,移植自ogre插件系统，可参考OgreDynLibManager.h/.cpp
	/// </summary>
	class DMDynLibMgr:public DMSingletonT<DMDynLibMgr>
	{
	public:
		DMDynLibMgr(void);
		virtual ~DMDynLibMgr(void);

		//---------------------------------------------------
		// Function Des: 如加载失败，返回null
		// Input Parameter:
		// Output Parameter:
		// return:
		//---------------------------------------------------
		DMDynLib* Load(const CStringW& strPluginPath);  ///<加入DMDynLib项，返回加入的项
		void Unload(DMDynLib* plib);				    ///<移除指定的MDynLib项

	protected:
		DMDynLib* FindObj(const CStringW& strPluginPath);

	protected:
		typedef DM::CArray<DMDynLib*>       DMDynLibArray;
		DMDynLibArray						m_LibArray;
	};


}//namespace DM
