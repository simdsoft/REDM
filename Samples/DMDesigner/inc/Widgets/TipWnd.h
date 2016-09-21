// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	TipWnd.h
// File mark:   
// File summary:专用于特殊的tips显示
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-3
// ----------------------------------------------------------------
#pragma once

class AttrTipWnd : public DMHWnd
{					
public:
	bool InitAttr(DMXmlInitAttrPtr ptr);
};

class AddTipWnd : public DMHWnd
{
public:
	bool InitAdd(CStringW strInfo);
};