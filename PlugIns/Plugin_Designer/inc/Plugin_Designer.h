#pragma once

class Plugin_Designer:public IDMPlugin
{
public:
	Plugin_Designer(void);
	~Plugin_Designer(void);

	const wchar_t* GetName() const;
	void Install();
	void Uninstall();

	void Initialise();
	void Shutdown();
};