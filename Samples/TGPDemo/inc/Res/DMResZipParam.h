
//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMResZipParam.h 
// File Des: 对外资源包结构
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-25	1.0			
//-------------------------------------------------------
#pragma once

struct DMPEResZip
{
	HINSTANCE					hInst;
	LPCWSTR						lpszResName;
	LPCWSTR						lpszResType;
};

class DMResZipParam
{
public:
	DMResZipParam(LPCWSTR lpszFile=NULL,LPCSTR lpszPsw =NULL)
		:m_bPERes(false),m_lpszZipPath(lpszFile), m_lpszPsw(lpszPsw)
	{}
	DMResZipParam(HINSTANCE hInst,LPCWSTR lpszResName,LPCWSTR lpszResType=L"zip",LPCSTR lpszPsw=NULL)
		:m_bPERes(true),m_lpszPsw(lpszPsw)
	{
		m_PEResInfo.hInst = hInst;m_PEResInfo.lpszResName = lpszResName;m_PEResInfo.lpszResType = lpszResType;
	}
public:
	bool								m_bPERes;		   // 是否EXE内置资源
	LPCSTR								m_lpszPsw;		   // ZIP密码
	union
	{
		LPCWSTR							m_lpszZipPath;
		DMPEResZip                      m_PEResInfo;
	};
};