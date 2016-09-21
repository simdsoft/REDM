// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	AddExpandWnd.h 
// File mark:   
// File summary:»√
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-9-8
// ----------------------------------------------------------------
#pragma once

class AddExpandWnd : public DMHWnd
{
public:
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENT_MAP() 

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnLButtonUp(UINT nFlags,CPoint pt);
public:
	DMCode Show();
	DMCode Hide();
};

