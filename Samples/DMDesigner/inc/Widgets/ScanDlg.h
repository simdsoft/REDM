﻿// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ScanDlg.h
// File mark:   
// File summary:扫描错误，并修正错误
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-14
// ----------------------------------------------------------------
#pragma once

class ScanDlg : public DMHDialog
{
public:
	DECLARE_MESSAGE_MAP()
	void OnSize(UINT nType, CSize size);

protected:
	DMCode OnOK();

private:
	DMCode Scan(bool bRepair = false);

private:
	bool            m_bExistErr;
};