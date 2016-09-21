
//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMPluginImpl.h 
// File Des: 插件内部实现,用于加载所有内部实现模块
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-17	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class DMPluginImpl:public IDMPlugin
	{
	public:
		const wchar_t* GetName() const;
		void Install();
		void Uninstall();

		void Initialise();
		void Shutdown();
	};


}//namespace DM