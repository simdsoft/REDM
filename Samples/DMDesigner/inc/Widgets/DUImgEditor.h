// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUImgEditor.h
// File mark:   
// File summary:图片编辑器
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-15
// ----------------------------------------------------------------
#pragma once 
#include <GdiPlus.h>
using namespace Gdiplus;

class DUImgEditor : public DUIScrollBase
{
	DMDECLARE_CLASS_NAME(DUImgEditor, L"duimgeditor",DMREG_Window)
public:
	DUImgEditor();
	~DUImgEditor();

public:
	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
	DM_END_MSG_MAP()
public:
	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
	void DM_OnPaint(IDMCanvas* pCanvas);

public:
	bool AddImg(CStringW strPath);
	bool Clear();
	bool SetVert(bool bVert);
	bool Save(CStringW strPath);
	int GetImgCount();

	void UpdateScrollRangeSize();
	CSize GetRangeSize();
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);  
private:
	CList<IDMBitmap*>						m_BmpList;
	bool                                    m_bVert;
	ULONG_PTR								m_token;
};