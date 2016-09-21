#pragma once

#include "DmMainOutput.h"

// lib
#ifdef _DEBUG
#pragma comment(lib,"DmMain_d.lib")
#else
#pragma comment(lib,"DmMain.lib")
#endif

using namespace DM;

class Plugin_Expand:public IDMPlugin
{
public:
	Plugin_Expand(void);
	~Plugin_Expand(void);

	const wchar_t* GetName() const;
	void Install();
	void Uninstall();

	void Initialise();
	void Shutdown();

};