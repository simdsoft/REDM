#include "Plugin_DesignerAfx.h"
#include "Plugin_Designer.h"

Plugin_Designer::Plugin_Designer(void)
{
}

Plugin_Designer::~Plugin_Designer(void)
{
}

const wchar_t* Plugin_Designer::GetName()const 
{
	return L"Plugin_Designer";
}

void Plugin_Designer::Install()
{
//	g_pDMApp->Register(DMRegHelperT<ListBoxEx>(),true);
//	g_pDMApp->Register(DMRegHelperT<ComboboxEx>(),true);
}

void Plugin_Designer::Uninstall()
{
//	g_pDMApp->UnRegister(ListBoxEx::GetClassName(),ListBoxEx::GetClassType());
//	g_pDMApp->UnRegister(ComboboxEx::GetClassName(),ComboboxEx::GetClassType());
}

void Plugin_Designer::Initialise()
{
	printf("Plugin_Designer::initialise\n");
}

void Plugin_Designer::Shutdown()
{
	printf("Plugin_Designer::shutdown\n");
}