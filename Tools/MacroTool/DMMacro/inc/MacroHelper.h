// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	MacroHelper.h
// File mark:   
// File summary:
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-10-31
// ----------------------------------------------------------------
#pragma once
#include <vector>
using namespace std;

typedef struct stMACRO
{
	CStringW		strDef;			// 宏定义如:MSG_WM_INITDIALOG(func)
	CStringW		strContent;     // 宏对应的内容
	CStringW        strMainDef;     // 宏括号前的内容,如MSG_WM_INITDIALOG
	CStringW        strDot;			// 宏中括号内的内容,如fun
	CStringWList    strParamList;   // 把strDot按逗号分开
}MACRO,*PMACRO;

class CMacroHelper
{
public:
	bool LoadFile(CStringW strPath,CStringW strStart); 
	CStringW Convert(CStringW strMacro);

private:
	bool AddMacroItem(CStringW strFind);
	PMACRO FindMacroItem(CStringW strMainDef,int nParamCount);
	void TermSpace(CStringW& str);

public:

	vector<PMACRO>                       m_VecMacro;

};