#include "pch.h"
#include "AutoSavePrivateReplay.h"


BAKKESMOD_PLUGIN(AutoSavePrivateReplay, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)


void AutoSavePrivateReplay::onLoad()
{
	cvarManager->log("Plugin loaded!");
}

void AutoSavePrivateReplay::onUnload()
{
}