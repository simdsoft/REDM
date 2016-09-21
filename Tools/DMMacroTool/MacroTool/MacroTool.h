
#pragma once
#include "resource.h"	

class CMacroToolApp : public CWinAppEx
{
public:
	CMacroToolApp();
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CMacroToolApp theApp;