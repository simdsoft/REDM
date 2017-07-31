//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMHelper.h 
// File Des: 通用辅助函数接口类
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-14	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	//---------------------------------------------------
	// Function Des: 获取EXE所在全路径、判断文件是否存在、获得文件的大小、内容
	// Input Parameter:
	// Output Parameter:
	// return:
	//---------------------------------------------------
	bool DM_EXPORT GetRootPathW(wchar_t *pszPath, DWORD dwSize);
	bool DM_EXPORT GetRootDirW(wchar_t *pszPath, DWORD dwSize);
	bool DM_EXPORT GetRootFullPath(const wchar_t *pszSrcPath, wchar_t *pszDestPath, DWORD dwSize);
	bool DM_EXPORT CheckFileExistW(const wchar_t *pszFilePath);
	DWORD DM_EXPORT GetFileSizeW(const wchar_t *pszFilePath);
	bool DM_EXPORT GetFileBufW(const wchar_t *pszFilePath, void **ppBuf, DWORD dwSize, DWORD &dwReadSize);

	//---------------------------------------------------
	// Function Des: 字体转换函数,dwLen为字符数，非字节数
	// Input Parameter:
	// Output Parameter:
	// return:
	//---------------------------------------------------
	bool DM_EXPORT UnicodeToAscii(const wchar_t * pwszSrc, PCHAR pszDst, DWORD dwLen);
	bool DM_EXPORT UnicodeToUtf8(const wchar_t * pwszSrc, PCHAR pszDst, DWORD dwLen);

	//---------------------------------------------------
	// Function Des: 转换
	// Input Parameter:
	// Output Parameter:
	// return:
	//---------------------------------------------------
	bool DM_EXPORT MeetRect(LPRECT lpRect, SIZE szPic);
	void DM_EXPORT DMHiMetricToPixel(const SIZEL * lpSizeInHiMetric, LPSIZEL lpSizeInPix,HWND hWnd);
	void DM_EXPORT DMPixelToHiMetric(const SIZEL * lpSizeInPix, LPSIZEL lpSizeInHiMetric,HWND hWnd);
	HCURSOR DM_EXPORT CreateCursorFromBitmap(HBITMAP hSourceBitmap,bool bSimple=true, COLORREF clrTransparent=0, DWORD xHotspot=0,DWORD yHotspot=0);//http://blog.csdn.net/hgy413/article/details/6563815

}//namespace DM