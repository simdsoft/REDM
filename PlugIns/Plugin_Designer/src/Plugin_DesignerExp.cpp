#pragma once
#include "Plugin_DesignerAfx.h"
#include "Plugin_Designer.h"

static Plugin_Designer* plugin;
extern "C"
void PLUGINDESIGNER_EXPORTS dllStartPlugin(void) throw()
{
	plugin = new Plugin_Designer();
	DMApp::getSingleton().InstallPlugin(plugin);
}

extern "C"
void PLUGINDESIGNER_EXPORTS dllStopPlugin(void)
{
	DMApp::getSingleton().UninstallPlugin(plugin);
	delete plugin;
}