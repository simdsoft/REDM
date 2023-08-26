#pragma once

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