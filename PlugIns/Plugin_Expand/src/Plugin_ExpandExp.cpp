#pragma once
#include "Plugin_ExpandAfx.h"
#include "Plugin_Expand.h"

#ifdef Plugin_Expand_EXPORTS
#define PLUGINEXPAND_EXPORTS __declspec( dllexport )
#else 
#define PLUGINEXPAND_EXPORTS __declspec( dllimport )
#endif

static Plugin_Expand* plugin;
extern "C"
void PLUGINEXPAND_EXPORTS dllStartPlugin(void) throw()
{
	plugin = new Plugin_Expand();
	DMApp::getSingleton().InstallPlugin(plugin);
}

extern "C"
void PLUGINEXPAND_EXPORTS dllStopPlugin(void)
{
	DMApp::getSingleton().UninstallPlugin(plugin);
	delete plugin;
}