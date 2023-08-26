// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	Plugin.h
// File mark:   
// File summary:内置插件
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-8
// ----------------------------------------------------------------
#pragma once

class Plugin:public IDMPlugin
{
public:
	const wchar_t* GetName() const;
	void Install();
	void Uninstall();

	void Initialise();
	void Shutdown();
};
